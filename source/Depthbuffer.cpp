#include "pch.h"
#include "Depthbuffer.h"
#include "Utils.h"

Depthbuffer::Depthbuffer(uint32_t width, uint32_t height, float resetValue)
	: m_ResetValue{resetValue}
	, m_Height{float(height)}
	, m_Width{float(width)}
{
	m_DepthBuffer.reserve(width * height);

	for (uint32_t i{}; i < width * height; ++i)
	{
		m_DepthBuffer.emplace_back(resetValue);
	}
}

void Depthbuffer::SetResetValue(float newResetValue)
{
	m_ResetValue = newResetValue;
}


bool Depthbuffer::DepthCheck(const Pixel_Info& pixInfo, Utils::Pixel pixel) 
{
	float z = pixInfo.GetDepth_Z();

	if (z > 1.f || z < 0.f) return false;

	if (pixel.x < 0 || pixel.y < 0) return false;
	if (pixel.x >= m_Width || pixel.y >= m_Height) return false;

	// The deeper, the more negative the value:
	if (z < m_DepthBuffer[pixel.x + (pixel.y * unsigned int(m_Width))])
	{
		// z is greater than a more negative value --> It's closer
		m_DepthBuffer[pixel.x + (pixel.y * unsigned int(m_Width))] = z;
		return true;
	}

	return false;
}


void Depthbuffer::Reset()
{
	for (float& value : m_DepthBuffer)
	{
		value = m_ResetValue;
	}
}