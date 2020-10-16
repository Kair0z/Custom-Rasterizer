#include "pch.h"
#include "MLambertDiffuse.h"

MLambertDiffuse::MLambertDiffuse(
	Texture* pDiffusemap,
	NormalMap* pNormalmap)
	: Material("Lambert diffuse (normalmap supported)")
{
	m_pDiffusemap = pDiffusemap;
	m_pNormalmap = pNormalmap;

	if (!m_pDiffusemap || !m_pNormalmap) std::cout << "Error: MLambertDiffuse::Constructor( ): Issue with one of the maps! \n";
}

MLambertDiffuse::~MLambertDiffuse()
{

}

RGBColor MLambertDiffuse::Shade(
	const Pixel_Info& pixelInfo,
	const FPoint3& camPos,
	const std::vector<Light*>& pLights) const 
{
	RGBColor finalColor{};
	FVector3 normal{};

	// NORMAL:
	// If normalmap exists, sample a world-space normal into normal
	if (m_pNormalmap) m_pNormalmap->Sample(pixelInfo.GetUV(), pixelInfo, normal);
	// Else: use the interpolated normal:
	else normal = pixelInfo.GetNormal();

	// COLOR:
	RGBColor diffuse{m_DefaultColor};
	float alpha{};
	if (m_pDiffusemap) m_pDiffusemap->Sample(pixelInfo.GetUV(), diffuse, alpha);

	// LIGHTING:
	for (Light* pLight : pLights)
	{
		if (!pLight->IsOn()) continue;

		float dot = Elite::Dot(-normal, pLight->GetDirection(pixelInfo));

		if (dot < 0.f)
		{
			continue;
		}

		finalColor += diffuse * pLight->BiradianceValue(pixelInfo) * dot;
	}

	return finalColor;
}