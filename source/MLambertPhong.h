#pragma once
#include "Material.h"
#include "Map.h"
#include "Texture.h"

class MLambertPhong : public Material
{
public:
	MLambertPhong(
		ValueMap* pSpecularmap,
		ValueMap* pGlossmap,
		NormalMap* pNormalmap,
		Texture* pDiffusemap,
		float shininess = 25.f);

	virtual RGBColor Shade(
		const Pixel_Info& pixelInfo,
		const FPoint3& camPos,
		const std::vector<Light*>& pLights)
		const override final;

private:
	ValueMap* m_pGlossmap;
	ValueMap* m_pSpecmap;

	float m_Shininess;

public:
	// ROF
	MLambertPhong(const MLambertPhong&) = delete;
	MLambertPhong(MLambertPhong&&) = delete;
	MLambertPhong& operator=(const MLambertPhong&) = delete;
	MLambertPhong& operator=(MLambertPhong&&) = delete;
	~MLambertPhong();
};

