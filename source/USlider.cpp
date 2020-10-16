#include "pch.h"
#include "USlider.h"
#include "Albedo.h"

USlider::USlider(float minValue, float maxValue, float defaultValue,
	const Elite::FPoint2& lt, float width, float height, bool shouldRender)
	: UIElement("Slider", shouldRender)
	, m_LT{lt}
	, m_MaxValue{maxValue}
	, m_MinValue{minValue}
	, m_CurrentValue{defaultValue}
	, m_Width{width}
	, m_Height{height}
	, m_SliderBar{ Elite::FPoint2{lt.x, lt.y + (height / 2.f)}, width + width / 7.f, height / 5.f, shouldRender }
	, m_SliderButton{ Elite::FPoint2{lt.x, lt.y}, width / 7.f, height, shouldRender}
{
	m_SliderBar.SetColor(Albedo::Gold );
	m_SliderButton.SetColor(Albedo::DeepPurple);
}

USlider::~USlider()
{

}

void USlider::Render(Window& window) const 
{
	m_SliderBar.Render(window);
	m_SliderButton.Render(window);
}

#pragma region Input
void USlider::ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{}
void USlider::ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{}
void USlider::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed)
{
	if (m_IsHeld)
	{
		Elite::FPoint2 newPos{ float(e.x), m_SliderButton.GetLT().y };

		newPos.x = Elite::Clamp(newPos.x, m_LT.x, m_LT.x + m_Width);

		m_SliderButton.SetLT(newPos);

		inputConsumed = true;
	}
}
void USlider::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{
	if (m_SliderButton.IsPointInside(Elite::FPoint2{ float(e.x), float(e.y) }))
	{
		m_IsHeld = true;
	}
}
void USlider::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{
	m_IsHeld = false;
}
#pragma endregion