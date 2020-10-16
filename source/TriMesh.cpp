#include "pch.h"
#include "TriMesh.h"
#include "Utils.h"
#include "LDirection.h"
#include "LPoint.h"

#pragma region constructor
TriMesh::TriMesh(
	const std::string& filename,
	const FVector3& posOffset,
	float rotSpeed,
	bool isVisible,
	float scale,
	bool shouldEverUpdate)
	: m_RotationSpeed{rotSpeed}
	, m_IsVisible{isVisible}
	, m_Scale{scale}
	, m_Scale_Original{scale}
	, m_ShouldUpdate{shouldEverUpdate}
	, m_Parser{}
	, m_CurMatIdx{}
{
	// Set Vertexbuffer & idxbuffer:
	m_Parser(filename, m_Vertexbuffer, m_Idxbuffer);

	m_Proj_Vertexbuffer.reserve(m_Vertexbuffer.size());

	// Setup transform matrix:
	m_Transform_Original = m_Transform = FMatrix4::Identity();

	if (scale <= 0.f) m_Scale_Original = m_Scale = 1.f;

	Scale(m_Scale);
	Translate(posOffset);
}
#pragma endregion

TriMesh::~TriMesh()
{
	for (Material* pMat : m_pMaterials)
	{
		delete pMat;
		pMat = nullptr;
	}
}

void TriMesh::Update(float elapsedSec)
{
	if (!m_ShouldUpdate) return;

	RotateLocal(0.f, elapsedSec, 0.f);
}

#pragma region Materials
void TriMesh::AddMaterial(Material* pMat)
{
	m_pMaterials.emplace_back(pMat);
}

void TriMesh::ToggleMaterial()
{
	++m_CurMatIdx;
	if (m_CurMatIdx >= m_pMaterials.size()) m_CurMatIdx = 0;
}
#pragma endregion

#pragma region Transformation
void TriMesh::Translate(const FVector3& translation)
{
	m_Transform = FMatrix4{ Elite::MakeTranslation(translation) } * m_Transform;
}
void TriMesh::RotateLocal(float x, float y, float z)
{
	m_Transform *= FMatrix4{ Elite::MakeRotationZYX(x, y, z) };
}
void TriMesh::Scale(float x, float y, float z)
{
	m_Transform = FMatrix4{ Elite::MakeScale(x, y, z) } * m_Transform;
}
void TriMesh::Scale(float uniscale)
{
	Scale(uniscale, uniscale, uniscale);
}
#pragma endregion

#pragma region Rasterizer-Pipeline

void TriMesh::Project(const IVector2& windowDim, const FMatrix4& viewMatrix, const FMatrix4& projMatrix)
{
	m_Proj_Vertexbuffer.clear();
	
	for (const Input_Vertex& in_Vertex : m_Vertexbuffer)
	{
		Output_Vertex output{ in_Vertex };

		// Original world points == vertices * world matrix
		output.SetWorldPoint(FPoint3{ m_Transform * FPoint4{ output.GetPoint() } });

		// Points: World Space to Camera Space
		output.SetPoint(projMatrix * viewMatrix * m_Transform * output.GetPoint());
	
		// Normals & tangents: rotated by object transform
		output.SetNormal(FVector3{ m_Transform * FVector4{output.GetNormal()} });
		output.SetTangent(FVector3{ m_Transform * FVector4{output.GetTangent()} });

		// Perspective divide:
		FPoint3 temp{ output.GetPoint() };
		temp.x /= output.GetPoint().w;
		temp.y /= output.GetPoint().w;
		temp.z /= output.GetPoint().w;

		// NDC to Raster space: (Technically already rasterization, but running a loop twice might not be the best choice)
		temp.x = ((temp.x + 1) / 2) * windowDim.x;
		temp.y = ((1 - temp.y) / 2) * windowDim.y;
		output.SetPoint(FPoint4{ temp.x, temp.y, temp.z, output.GetPoint().w });

		m_Proj_Vertexbuffer.emplace_back(output);
	}
}

void TriMesh::Rasterize(Window& window, const RasterDesc& desc, Depthbuffer& depthbuffer, const FPoint3& camPos, const std::vector<Light*>& pLights)
{
	// For every triangle:
	for (int i{}; i < m_Idxbuffer.size(); i += 3)
	{
		// Get the 3 vertices:
		std::array<Output_Vertex, 3> faceVertices
		{
			m_Proj_Vertexbuffer[i],
			m_Proj_Vertexbuffer[i + 1],
			m_Proj_Vertexbuffer[i + 2]
		};

		// Frustrum Culling:
		if (faceVertices[0].GetPoint().z > 1.f || faceVertices[0].GetPoint().z < 0.f) continue;
		if (faceVertices[1].GetPoint().z > 1.f || faceVertices[1].GetPoint().z < 0.f) continue;
		if (faceVertices[2].GetPoint().z > 1.f || faceVertices[2].GetPoint().z < 0.f) continue;

		// Get The boundingbox:
		Utils::IRect boundingBox{
			faceVertices[0].GetPoint().xy,
			faceVertices[1].GetPoint().xy,
			faceVertices[2].GetPoint().xy,
			FVector2{window.GetWindowDimensions()}
		};

		// For every pixel in the boundingloop:
		for (int r{ boundingBox.m_LT.y }; r < boundingBox.m_LT.y + boundingBox.m_Height + 1; ++r)
		{
			for (int c{ boundingBox.m_LT.x }; c < boundingBox.m_LT.x + boundingBox.m_Width + 1; ++c)
			{
				Pixel_Info pixInfo{};
				Utils::Pixel pixel{ c, r };

				// If viewtype == boundingbox --> Don't do any checks
				if (desc.m_Viewtype == Viewtype::Boundingbox)
				{
					window.SetBBPixelColor(r, c, RGBColor{ 1.f, 1.f, 1.f });
					continue;
				}

				// Does this pixel fall into the projection? (if so, interpolate)
				if (!PixelInProjection(faceVertices, pixel, pixInfo, desc)) continue;

				// Does the pixel pass the depth check?
				if (!depthbuffer.DepthCheck(pixInfo, pixel)) continue;

				// Shade the pixel:
				pixInfo.SetDestColor(window.GetBBPixelColor(r, c)); // Set the destColor for blending
				ShadePixel(pixInfo, desc, camPos, pLights);

				window.SetBBPixelColor(r, c, pixInfo.GetColor());
			}
		}
	}
}

bool TriMesh::PixelInProjection(const std::array<Output_Vertex, 3>& vertices, Utils::Pixel pix, Pixel_Info& pixInfo, const RasterDesc& desc) const
{
	// temps:
	FVector2 pointToPixel{};
	FVector2 pointToNextPoint{};
	FPoint2 pixel{ float(pix.x), float(pix.y) };
	float totalArea{ std::abs(
			Elite::Cross(
				vertices[1].GetPoint().xy - vertices[0].GetPoint().xy,
				vertices[2].GetPoint().xy - vertices[0].GetPoint().xy
			)
		)

	};
	float signedArea{};
	float weights[3]{};

	// For every vertex:
	for (int i{}; i < 3; ++i)
	{
		pointToPixel = pixel - vertices[i].GetPoint().xy;

		int nextPointIdx{ (i + 1) % 3 };
		pointToNextPoint = vertices[nextPointIdx].GetPoint().xy - vertices[i].GetPoint().xy;

		signedArea = Elite::Cross(pointToPixel, pointToNextPoint);

		if (desc.m_Cullmode == Cullmode::Frontface && signedArea > 0.f) return false;
		if (desc.m_Cullmode == Cullmode::Backface && signedArea < 0.f) return false;
		if (desc.m_Cullmode == Cullmode::None && signedArea < 0.f) return false;

		int weightIdx{ (i + 2) % 3 };
		weights[weightIdx] = std::abs(signedArea / totalArea);
	}

	//------------------------------------------------------------------------------------------------------
	// THE PIXEL SUCCEEDED!
	//------------------------------------------------------------------------------------------------------

	// Attribute interpolation:
	float w_Interpol = (1.f / vertices[0].GetPoint().w) * weights[0] + (1.f / vertices[1].GetPoint().w) * weights[1] + (1.f / vertices[2].GetPoint().w) * weights[2];
	w_Interpol = 1.f / w_Interpol;

	float z_Interpol = (1.f / vertices[0].GetPoint().z) * weights[0] + (1.f / vertices[1].GetPoint().z) * weights[1] + (1.f / vertices[2].GetPoint().z) * weights[2];
	z_Interpol = 1.f / z_Interpol;

	FVector2 uv_Interpol = (vertices[0].GetUV() / vertices[0].GetPoint().w) * weights[0] + (vertices[1].GetUV() / vertices[1].GetPoint().w) * weights[1] + (vertices[2].GetUV() / vertices[2].GetPoint().w) * weights[2];
	uv_Interpol *= w_Interpol;

	FVector3 normal_Interpol = (vertices[0].GetNormal() / vertices[0].GetPoint().w) * weights[0] + (vertices[1].GetNormal() / vertices[1].GetPoint().w) * weights[1] + (vertices[2].GetNormal() / vertices[2].GetPoint().w) * weights[2];
	Elite::Normalize(normal_Interpol);

	FVector3 tangent_Interpol = (vertices[0].GetTangent() / vertices[0].GetPoint().w) * weights[0] + (vertices[1].GetTangent() / vertices[1].GetPoint().w) * weights[1] + (vertices[2].GetTangent() / vertices[2].GetPoint().w) * weights[2];
	Elite::Normalize(tangent_Interpol);

	FPoint3 worldPoint_Interpol = vertices[0].GetWorldPoint() * weights[0] + FVector3{ vertices[1].GetWorldPoint() * weights[1] } +FVector3{ vertices[2].GetWorldPoint() * weights[2] };

	pixInfo.SetBaryCoordinate(FVector3{ weights[0], weights[1], weights[2] });
	pixInfo.SetColor(RGBColor{ weights[0], weights[1], weights[2] });
	pixInfo.SetDepth(w_Interpol);
	pixInfo.SetDepth_Z(z_Interpol);
	pixInfo.SetUV(uv_Interpol);
	pixInfo.SetWorldSpacePoint(worldPoint_Interpol);
	pixInfo.SetNormal(normal_Interpol);
	pixInfo.SetTangent(tangent_Interpol);

	return true;
}

void TriMesh::ShadePixel(Pixel_Info& pixInfo, const RasterDesc& rasterDesc, const FPoint3& camPos, const std::vector<Light*>& pLights)
{
	RGBColor finalColor{};

	switch (rasterDesc.m_Viewtype)
	{
	case Viewtype::Boundingbox:
		finalColor = RGBColor{ 1.f, 1.f, 1.f };
		break;

	case Viewtype::Materials:
		if (!m_pMaterials.empty()) finalColor = m_pMaterials[m_CurMatIdx]->Shade(pixInfo, camPos, pLights);
		else finalColor = RGBColor{ 1.f, 0.f, 1.f };
		break;

	case Viewtype::Triangledata:
		finalColor = RGBColor{ pixInfo.GetBaryCoordinate().x, pixInfo.GetBaryCoordinate().y, pixInfo.GetBaryCoordinate().z };
		break;

	case Viewtype::Normals:
		FVector3 normal{pixInfo.GetNormal()};
		if (!m_pMaterials.empty())
		{
			if (m_pMaterials[m_CurMatIdx]->GetNormalmap()) m_pMaterials[m_CurMatIdx]->GetNormalmap()->Sample(pixInfo.GetUV(), pixInfo, normal);
		}

		finalColor = RGBColor{ (normal.x + 1) / 2.f, (normal.y + 1) / 2.f, (normal.z + 1) / 2.f };
		break;

	case Viewtype::Depth:
		float depth = Elite::Remap(pixInfo.GetDepth_Z(), 0.997f, 1.f);
		finalColor = RGBColor{ 1.f-depth, 1.f-depth, 1.f-depth };
		break;
	}

	finalColor.Clamp();

	pixInfo.SetColor(finalColor);
}
#pragma endregion