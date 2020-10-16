#pragma once
#include <vector>
#include "Pixel_Info.h"
#include "Utils.h"

class Depthbuffer
{
public:
	Depthbuffer() = default;
	Depthbuffer(uint32_t width, uint32_t height, float resetValue = FLT_MAX);
	void SetResetValue(float newResetValue);
	void Reset();

	bool DepthCheck(const Pixel_Info& pixInfo, Utils::Pixel pixel);

private:
	std::vector<float> m_DepthBuffer;
	float m_ResetValue;
	float m_Width;
	float m_Height;
};

