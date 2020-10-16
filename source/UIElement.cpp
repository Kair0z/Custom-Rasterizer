#include "pch.h"
#include "UIElement.h"

UIElement::UIElement(const std::string& tag, bool shouldRender)
	: m_Tag{tag}
	, m_ShouldRender{shouldRender}
{

}

UIElement::~UIElement()
{

}

void UIElement::Update(float elapsedSec)
{

}

void UIElement::Render(Window& window) const
{
	
}

#pragma region Input
void UIElement::ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{

}
void UIElement::ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{

}
void UIElement::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed)
{

}
void UIElement::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{

}
void UIElement::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{

}
#pragma endregion

bool UIElement::ShouldRender() const
{
	return m_ShouldRender;
}