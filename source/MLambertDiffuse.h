#pragma once
#include "Material.h"

#include "Texture.h"
#include "Map.h"

class MLambertDiffuse : public Material
{
public:
	MLambertDiffuse(
		Texture* pDiffusemap,
		NormalMap* pNormalmap);

	virtual RGBColor Shade(
		const Pixel_Info& pixelInfo,
		const FPoint3& camPos,
		const std::vector<Light*>& pLights)
		const override final;

	// ROF
	MLambertDiffuse(const MLambertDiffuse&) = delete;
	MLambertDiffuse(MLambertDiffuse&&) = delete;
	MLambertDiffuse& operator=(const MLambertDiffuse&) = delete;
	MLambertDiffuse& operator=(MLambertDiffuse&&) = delete;
	~MLambertDiffuse();
	
private:

};

