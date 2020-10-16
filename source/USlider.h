#pragma once
#include "EMath.h"
#include "UIElement.h"
#include "UBox.h"

class USlider : public UIElement
{
public:
	USlider(float minValue, float maxValue, float defaultValue, 
		const Elite::FPoint2& lt, float width, float height, bool shouldRender = true);

	~USlider();

	void SetHighlight(const Elite::RGBColor& highlight);
	void SetInactiveMultiplier(float newMultiplier);

	virtual void Render(Window& window) const override;

	// Input:
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed) override;
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed) override;

private:
	float m_MinValue{};
	float m_MaxValue{};
	float m_CurrentValue{};

	Elite::FPoint2 m_LT;
	float m_Width;
	float m_Height;

	float m_InactiveBrightnessMultiplier;

	UBox m_SliderBar;
	UBox m_SliderButton;

	bool m_IsHeld;

public:
	USlider(const USlider&) = delete;
	USlider(USlider&&) = delete;
	USlider& operator=(const USlider&) = delete;
	USlider& operator=(USlider&&) = delete;
};

