#pragma once
#include <vector>

class UIElement;

class UISheet
{
public:
	UISheet(const std::string& sheetName);
	~UISheet();

	void SetName(const std::string& newName);
	std::string GetName() const;

	void AddElement(UIElement* pElement);
	std::vector<UIElement*> GetElements();

	void Update(float elapsedSec);

	// Input:
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed);

private:
	std::vector<UIElement*> m_pElements;

	std::string m_Name;

public:
	UISheet(const UISheet&) = delete;
	UISheet(UISheet&&) = delete;
	UISheet& operator=(const UISheet&) = delete;
	UISheet& operator=(UISheet&&) = delete;
};

