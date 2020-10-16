#include "pch.h"
#include "UButton.h"
#include "Texture.h"
#include <iostream>

UButton::UButton(const Elite::FPoint2& lt, float width, float height, bool shouldRender)
	: UBox(lt, width, height, shouldRender)
	, m_ShouldHighlight{false}
	, m_Highlight{0.2f, 0.2f, 0.2f}
	, m_InactiveBrightnessMultiplier{0.8f}
{

}

void UButton::SetOnPressed(std::function<void()> onPressed)
{
	m_OnPressed = onPressed;
}

void UButton::SetOnReleased(std::function<void()> onReleased)
{
	m_OnReleased = onReleased;
}

void UButton::SetHighlight(const Elite::RGBColor& highlight)
{
	m_Highlight = highlight;
}

void UButton::SetInactiveMultiplier(float newMultiplier)
{
	m_InactiveBrightnessMultiplier = newMultiplier;
}

void UButton::Render(Window& window) const
{
	for (uint32_t r{ uint32_t(m_LT.y) }; r < uint32_t(m_LT.y + m_Height); ++r)
	{
		for (uint32_t c{ uint32_t(m_LT.x) }; c < uint32_t(m_LT.x + m_Width); ++c)
		{
			if (c >= window.GetWindowDimensions().x || r >= window.GetWindowDimensions().y) continue;

			Elite::RGBColor color{ m_Color };
			float out_Alpha{};
			if (m_pTexture) m_pTexture->Sample(CalcUVCoord(c, r), color, out_Alpha);

			if (m_ShouldHighlight) color = color * (Elite::RGBColor{ 1.f, 1.f, 1.f } +m_Highlight);
			else color *= m_InactiveBrightnessMultiplier;

			color.Clamp();

			window.SetBBPixelColor(r, c, color);
		}
	}
}

#pragma region Input
void UButton::ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{}
void UButton::ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{
}
void UButton::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed)
{
	Elite::FPoint2 point{ float(e.x), float(e.y) };

	if (!IsPointInside(point))
	{
		m_ShouldHighlight = false;
		return;
	}

	m_ShouldHighlight = true;
}
void UButton::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{
	Elite::FPoint2 point{float(e.x), float(e.y)};

	if (!IsPointInside(point)) return;
	if (m_OnPressed) m_OnPressed();
}
void UButton::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{
	Elite::FPoint2 point{ float(e.x), float(e.y) };

	if (!IsPointInside(point)) return;

	if (m_OnReleased) m_OnReleased();
}
#pragma endregion