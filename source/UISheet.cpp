#include "pch.h"
#include "UISheet.h"

#include "UIElement.h"

UISheet::UISheet(const std::string& name)
	: m_Name{name}
{

}

UISheet::~UISheet()
{
	for (UIElement* pElement : m_pElements)
	{
		delete pElement;
		pElement = nullptr;
	}
}

void UISheet::SetName(const std::string& newName)
{
	m_Name = newName;
}

std::string UISheet::GetName() const
{
	return m_Name;
}

void UISheet::AddElement(UIElement* pElement)
{
	m_pElements.emplace_back(pElement);
}

std::vector<UIElement*> UISheet::GetElements()
{
	return m_pElements;
}

void UISheet::Update(float elapsedSec)
{
	for (UIElement* pElement : m_pElements)
	{
		pElement->Update(elapsedSec);
	}
}

#pragma region Input
void UISheet::ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{
	for (UIElement* pElement : m_pElements)
	{
		pElement->ProcessKeyDownEvent(e, inputConsumed);
	}
}
void UISheet::ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{
	for (UIElement* pElement : m_pElements)
	{
		pElement->ProcessKeyUpEvent(e, inputConsumed);
	}
}
void UISheet::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed)
{
	for (UIElement* pElement : m_pElements)
	{
		pElement->ProcessMouseMotionEvent(e, inputConsumed);
	}
}
void UISheet::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{
	for (UIElement* pElement : m_pElements)
	{
		pElement->ProcessMouseDownEvent(e, inputConsumed);
	}
}
void UISheet::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{
	for (UIElement* pElement : m_pElements)
	{
		pElement->ProcessMouseUpEvent(e, inputConsumed);
	}
}
#pragma endregion