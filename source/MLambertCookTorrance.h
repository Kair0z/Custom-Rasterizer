#pragma once
#include "Material.h"
#include "Map.h"
#include "Texture.h"

class MLambertCookTorrance : public Material
{
public:
	MLambertCookTorrance(
		ValueMap* pRoughnessmap,
		ValueMap* pMetalnessmap,
		Texture* pDiffusemap,
		NormalMap* pNormalmap
	);

	MLambertCookTorrance(const RGBColor& albedo, float roughness, bool isMetal);

	virtual RGBColor Shade(
		const Pixel_Info& pixelInfo,
		const FPoint3& camPos,
		const std::vector<Light*>& pLights)
		const override final;

	void OverwriteRoughnessMap(ValueMap* pRoughnessmap);
	void OverwriteMetalnessMap(ValueMap* pMetalnessmap);

private:
	ValueMap* m_pRoughness;
	float m_DefaultRoughness;

	ValueMap* m_pMetalness;
	float m_DefaultMetalness;

public:
	// ROF
	~MLambertCookTorrance();
	MLambertCookTorrance(const MLambertCookTorrance&) = delete;
	MLambertCookTorrance(MLambertCookTorrance&&) = delete;
	MLambertCookTorrance& operator=(const MLambertCookTorrance&) = delete;
	MLambertCookTorrance& operator=(MLambertCookTorrance&&) = delete;
};

