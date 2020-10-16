#pragma once
#include <string>
#include <vector>
#include "Light.h"
#include "Pixel_Info.h"
#include "Utils.h"

#include "Map.h"
#include "Texture.h"

using namespace Elite;

class Material
{
public:
	// Shade function returns a final color
	virtual RGBColor Shade(
		const Pixel_Info& pixelInfo,
		const FPoint3& camPos,
		const std::vector<Light*>& pLights) const = 0;

	virtual ~Material();

	// Shared maps across that all materials might need:
	void OverwriteNormalMap(NormalMap* pNormalmap);
	void OverwriteDiffuseMap(Texture* pDiffusemap);

	NormalMap* GetNormalmap() const;
	Texture* GetDiffusemap() const;

	void SetDefaultColor(const RGBColor& color);
	void SetDefaultColor(float r, float g, float b);

protected:
	// All materials derive from this, but a raw Material Class isn't supported
	Material(const std::string& name);

	RGBColor m_DefaultColor;
	NormalMap* m_pNormalmap;
	Texture* m_pDiffusemap;

	std::string m_Name;
};

