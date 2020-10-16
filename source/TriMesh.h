#pragma once
#include "OBJ_Parser.h"
#include "Window.h"
#include "Depthbuffer.h"
#include "Light.h"
#include "RasterDesc.h"
#include "Material.h"

using namespace Elite;

class TriMesh
{
public:
	TriMesh(
		const std::string& filename,
		const FVector3& posOffset	= {},
		float rotSpeed				= {},
		bool isVisible				= true,
		float scale					= 1.f,
		bool shouldEverUpdate		= true);

	~TriMesh();

	void Update(float elapsedSec);

	// Adding Materials:
	void AddMaterial(Material* pMat);
	void ToggleMaterial();

	// Transformations:
	void Translate(const FVector3& translation);
	void RotateLocal(float x, float y, float z); // around own axis
	void Scale(float x, float y, float z);
	void Scale(float uniscale);

	// Rasterization Pipeline:
	void Project(const IVector2& windowDim, const FMatrix4& viewMatrix, const FMatrix4& projMatrix);
	void Rasterize(Window& window, const RasterDesc& desc, Depthbuffer& depthbuffer, const FPoint3& camPos, const std::vector<Light*>& pLights);

private:
	OBJ_Parser m_Parser;

	FMatrix4 m_Transform;
	FMatrix4 m_Transform_Original;

	// Buffers:
	std::vector<Input_Vertex> m_Vertexbuffer;
	std::vector<Output_Vertex> m_Proj_Vertexbuffer;
	std::vector<uint32_t> m_Idxbuffer;

	// Materials:
	std::vector<Material*> m_pMaterials;
	uint32_t m_CurMatIdx;

	bool m_IsVisible;
	bool m_ShouldUpdate;

	float m_Scale;
	float m_Scale_Original;

	float m_RotationSpeed;

	// Private submethods:
	bool PixelInProjection(const std::array<Output_Vertex, 3>& vertices, Utils::Pixel pix, Pixel_Info& pixInfo, const RasterDesc& desc) const;
	void ShadePixel(Pixel_Info& pixInfo, const RasterDesc& rasterDesc, const FPoint3& camPos, const std::vector<Light*>& pLights);
};

