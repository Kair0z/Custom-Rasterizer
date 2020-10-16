#include "pch.h"
#include "UBox.h"

#include "Texture.h"

UBox::UBox(const Elite::FPoint2& lt, float width, float height, bool shouldRender)
	: UIElement("box", shouldRender)
	, m_LT{lt}
	, m_Width{width}
	, m_Height{height}
	, m_Color{1.f, 0.f, 1.f}
{

}

UBox::~UBox()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

bool UBox::IsPointInside(const Elite::FPoint2& point)
{
	if (point.x < m_LT.x || point.x > m_LT.x + m_Width) return false;

	if (point.y < m_LT.y || point.y > m_LT.y + m_Height) return false;

	return true;
}

void UBox::Update(float elapsedSec)
{

}

void UBox::Move(const Elite::FVector2& movement)
{
	m_LT += movement;
}

void UBox::SetLT(const Elite::FPoint2& position)
{
	m_LT = position;
}

Elite::FPoint2 UBox::GetLT() const
{
	return m_LT;
}

void UBox::Render(Window& window) const
{
	for (uint32_t r{ uint32_t(m_LT.y) }; r < uint32_t(m_LT.y + m_Height); ++r)
	{
		for (uint32_t c{ uint32_t(m_LT.x) }; c < uint32_t(m_LT.x + m_Width); ++c)
		{
			if (c >= window.GetWindowDimensions().x || r >= window.GetWindowDimensions().y) continue;

			Elite::RGBColor color{ m_Color };
			float out_Alpha{};
			if (m_pTexture) m_pTexture->Sample(CalcUVCoord(c, r), color, out_Alpha);

			window.SetBBPixelColor(r, c, color);
		}
	}
}

#pragma region Input
void UBox::ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{

}
void UBox::ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{

}
void UBox::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed)
{

}
void UBox::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{

}
void UBox::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{

}
#pragma endregion

void UBox::SetColor(const Elite::RGBColor& color)
{
	m_Color = color;
}

void UBox::OverwriteTexture(Texture* pTexture)
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	m_pTexture = pTexture;
}

Elite::FVector2 UBox::CalcUVCoord(uint32_t c, uint32_t r) const
{
	Elite::FVector2 result{};

	result.x = Elite::Remap(float(c), m_LT.x, m_LT.x + m_Width);
	result.y = Elite::Remap(float(r), m_LT.y, m_LT.y + m_Height);

	return result;
}