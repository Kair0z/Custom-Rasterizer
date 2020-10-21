#include "pch.h"
#include "MLambertPhong.h"
#include "BRDF.h"
#include "Albedo.h"

MLambertPhong::MLambertPhong(
	ValueMap* pSpecularmap,
	ValueMap* pGlossmap,
	NormalMap* pNormalmap,
	Texture* pDiffusemap,
	float shininess)
	: Material("Lambertdiffuse + Phongspecular")
	, m_Shininess{shininess}
{
	m_pGlossmap = pGlossmap;
	m_pSpecmap = pSpecularmap;
	m_pNormalmap = pNormalmap;
	m_pDiffusemap = pDiffusemap;

	if (!m_pDiffusemap || !m_pGlossmap || !m_pSpecmap || !m_pNormalmap)
	{
		std::cout << "Error: MLambertPhong::Constructor( ): Problem using 1 or more of the provided maps! \n";
	}
}

MLambertPhong::~MLambertPhong()
{
	if (m_pSpecmap)
	{
		delete m_pSpecmap;
		m_pSpecmap = nullptr;
	}

	if (m_pGlossmap)
	{
		delete m_pGlossmap;
		m_pGlossmap = nullptr;
	}
}

RGBColor MLambertPhong::Shade(
	const Pixel_Info& pixelInfo,
	const FPoint3& camPos,
	const std::vector<Light*>& pLights)
	const
{
	RGBColor finalColor{};

	// NORMAL:
	FVector3 normal{};
	if (m_pNormalmap) m_pNormalmap->Sample(pixelInfo.GetUV(), pixelInfo, normal);
	else normal = pixelInfo.GetNormal();
	normal = pixelInfo.GetNormal();

	// SPECULAR:
	float specRefl{0.5f};
	if (m_pSpecmap) m_pSpecmap->Sample(pixelInfo.GetUV(), specRefl);

	// GLOSSINESS:
	float phongExp{1.0f};
	if (m_pGlossmap) m_pGlossmap->Sample(pixelInfo.GetUV(), phongExp);

	// VIEWDIRECTION:
	FVector3 toView = camPos - pixelInfo.GetWorldSpacePoint();
	Elite::Normalize(toView);

	// DIFFUSE:
	RGBColor baseColor{};
	float out_Alpha{};
	if (m_pDiffusemap) m_pDiffusemap->Sample(pixelInfo.GetUV(), baseColor, out_Alpha);

	// For every light in scene:
	for (Light* pLight : pLights)
	{
		if (!pLight->IsOn()) continue;

		RGBColor sumBRDF{ BRDF::Lambert(1.0f - specRefl, baseColor) + BRDF::Phong(specRefl, 60.0f, pLight->GetDirection(pixelInfo), toView, normal) };

		RGBColor biradValue = pLight->BiradianceValue(pixelInfo);
		float dot = Elite::Dot(normal, -pLight->GetDirection(pixelInfo));
		if (dot < 0.0f) continue;

		finalColor += sumBRDF
			* biradValue
			* dot;
	}

	return finalColor;
}