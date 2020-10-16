#pragma once
#include "Material.h"
#include "Texture.h"

class MRawTexture : public Material
{
public:
	MRawTexture(Texture* pDiffusemap); 

	virtual RGBColor Shade(
		const Pixel_Info& pixelInfo,
		const FPoint3& camPos,
		const std::vector<Light*>& pLights)
		const override final;

	MRawTexture(const MRawTexture&) = delete;
	MRawTexture(MRawTexture&&) = delete;
	MRawTexture& operator=(const MRawTexture&) = delete;
	MRawTexture& operator=(MRawTexture&&) = delete;
	~MRawTexture();

private:
	
};

