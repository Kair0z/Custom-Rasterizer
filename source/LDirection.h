#pragma once
#include "Light.h"
class LDirection : public Light
{
public:
	LDirection(const Elite::FVector3& direction, const Elite::RGBColor& color, float intensity, bool isOn = true);

	Elite::FVector3 GetDirection(const Pixel_Info& hitInfo) const override final;
	Elite::RGBColor BiradianceValue(const Pixel_Info& hitInfo) const override final;
	Elite::FPoint3 GetPos() const override final; 
	void Update(float elapsedSec) override final;

private:
	Elite::FVector3 m_Direction;
};

