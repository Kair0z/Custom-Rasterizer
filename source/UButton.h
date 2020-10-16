#pragma once
#include "UBox.h"
#include <functional>

class UButton : public UBox
{
public:
	UButton(const Elite::FPoint2& lt, float width, float height, bool shouldRender = true);

	void SetOnPressed(std::function<void()> onPressed);
	void SetOnReleased(std::function<void()> onReleased);
	void SetHighlight(const Elite::RGBColor& highlight);
	void SetInactiveMultiplier(float newMultiplier);

	void Render(Window& window) const;

	// Input:
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed) override;
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed) override;

private:
	std::function<void()> m_OnPressed;
	std::function<void()> m_OnReleased;

	Elite::RGBColor m_Highlight;
	bool m_ShouldHighlight;

	float m_InactiveBrightnessMultiplier;
};

