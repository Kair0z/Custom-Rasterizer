#pragma once
#include "Material.h"
class MRawColor : public Material
{
public:
	MRawColor(float r, float g, float b);
	MRawColor(const Elite::RGBColor& color);

	virtual RGBColor Shade(
		const Pixel_Info& pixelInfo,
		const FPoint3& camPos,
		const std::vector<Light*>& pLights) 
		const override final;

private:
	
};

