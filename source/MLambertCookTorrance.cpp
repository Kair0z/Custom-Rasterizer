#include "pch.h"
#include "MLambertCookTorrance.h"
#include "BRDF.h"

MLambertCookTorrance::MLambertCookTorrance(
	ValueMap* pRoughnessmap,
	ValueMap* pMetalnessmap,
	Texture* pDiffusemap,
	NormalMap* pNormalmap)
	: Material("Lambert diffuse + CookTorrance reflectance")
	, m_pRoughness{nullptr}
	, m_pMetalness{nullptr}
{
	m_pDiffusemap = pDiffusemap;
	m_pNormalmap = pNormalmap;
	m_pRoughness = pRoughnessmap;
	m_pMetalness = pMetalnessmap;

	if (!m_pDiffusemap || !m_pNormalmap || !m_pRoughness || !m_pMetalness)
	{
		std::cout << "Error: LambertCookTorrance::Constructor( ): Problem with one of the maps! \n";
	}
}

MLambertCookTorrance::MLambertCookTorrance(const RGBColor& albedo, float roughness, bool isMetal)
	: Material("Lambert diffuse + CookTorrance reflectance")
	, m_DefaultRoughness{roughness}
	, m_DefaultMetalness{}
{
	m_DefaultColor = albedo;

	m_DefaultRoughness = Elite::Clamp(m_DefaultRoughness, 0.01f, 1.f);
	if (isMetal) m_DefaultMetalness = 1.f;
	else m_DefaultMetalness = 0.f;
}

MLambertCookTorrance::~MLambertCookTorrance()
{
	if (m_pRoughness)
	{
		delete m_pRoughness;
		m_pRoughness = nullptr;
	}

	if (m_pMetalness)
	{
		delete m_pMetalness;
		m_pMetalness = nullptr;
	}
}

RGBColor MLambertCookTorrance::Shade(
	const Pixel_Info& pixelInfo,
	const FPoint3& camPos,
	const std::vector<Light*>& pLights) const
{
	RGBColor finalColor{};

	// NORMAL:
	FVector3 normal{ pixelInfo.GetNormal() };
	if (m_pNormalmap) m_pNormalmap->Sample(pixelInfo.GetUV(), pixelInfo, normal);

	// VIEWDIRECTION:
	FVector3 toView = camPos - pixelInfo.GetWorldSpacePoint();
	Elite::Normalize(toView);
	toView = -toView;

	// ROUGHNESS:
	float roughness{m_DefaultRoughness};
	if (m_pRoughness) m_pRoughness->Sample(pixelInfo.GetUV(), roughness);

	// METALNESS:
	float metalness{m_DefaultMetalness};
	if (m_pRoughness) m_pRoughness->Sample(pixelInfo.GetUV(), metalness);

	// DIFFUSE:
	RGBColor diffuse{m_DefaultColor};
	float out_Alpha{};
	if (m_pDiffusemap) m_pDiffusemap->Sample(pixelInfo.GetUV(), diffuse, out_Alpha);

	RGBColor cookTorranceReflect{};

	for (Light* pLight : pLights)
	{
		if (!pLight->IsOn()) continue;

		float dot{ Elite::Dot(-normal, pLight->GetDirection(pixelInfo)) };

		if (dot < 0.f) continue;

		Elite::FVector3 halfVector{ toView + pLight->GetDirection(pixelInfo) };
		
		RGBColor dFactor = RGBColor{ 1.f, 1.f, 1.f } - BRDF::FF::Schlick(halfVector, toView, diffuse, bool(metalness));
		RGBColor rFactor = RGBColor{ 1.f, 1.f, 1.f } - dFactor;
		if (bool(metalness)) dFactor = RGBColor{ 0.f, 0.f, 0.f };

		cookTorranceReflect = BRDF::CookTorrance(diffuse, toView, pLight->GetDirection(pixelInfo), normal, roughness, bool(metalness));

		finalColor += BRDF::Lambert(dFactor.r, diffuse) + cookTorranceReflect 
			* pLight->BiradianceValue(pixelInfo)
			* dot;
	}

	

	return finalColor;
}

void MLambertCookTorrance::OverwriteRoughnessMap(ValueMap* pRoughnessmap)
{
	if (m_pRoughness)
	{
		delete m_pRoughness;
		m_pRoughness = nullptr;
	}

	m_pRoughness = pRoughnessmap;
}

void MLambertCookTorrance::OverwriteMetalnessMap(ValueMap* pMetalnessmap)
{
	if (m_pMetalness)
	{
		delete m_pMetalness;
		m_pMetalness = nullptr;
	}

	m_pMetalness = pMetalnessmap;
}

