#include "pch.h"
#include "MRawTexture.h"

MRawTexture::MRawTexture(Texture* pDiffusemap)
	: Material("Raw Texture (No Lighting)")
{
	m_pDiffusemap = pDiffusemap;
}

MRawTexture::~MRawTexture()
{
	
}

RGBColor MRawTexture::Shade(
	const Pixel_Info& pixelInfo,
	const FPoint3& camPos,
	const std::vector<Light*>& pLights) const
{
	RGBColor returnColor{1.f, 0.f, 1.f};

	float out_Alpha{};
	if (m_pDiffusemap) m_pDiffusemap->Sample(pixelInfo.GetUV(), returnColor, out_Alpha);

	return returnColor;
}