#include "pch.h"
#include "MRawColor.h"

MRawColor::MRawColor(float r, float g, float b)
	: MRawColor{RGBColor{r, g, b}}
{
	
}

MRawColor::MRawColor(const Elite::RGBColor& color)
	: Material("Raw Color")
{
	m_DefaultColor = color;
	m_DefaultColor.MaxToOne();
}

RGBColor MRawColor::Shade(
	const Pixel_Info& pixelInfo,
	const FPoint3& camPos,
	const std::vector<Light*>& pLights) const
{
	return m_DefaultColor;
}