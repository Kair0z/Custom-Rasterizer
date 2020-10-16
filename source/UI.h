#pragma once
#include <vector>

class UIElement;
class UISheet;

class UI
{
public:
	UI();
	~UI();

	void AddSheet(UISheet* pSheet);
	UISheet* GetActiveSheet();

	bool IsEmpty() const;

	void ToggleActiveSheet();

	void Update(float elapsedSec);

	// Input:
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed);

private:
	// UI-elements
	std::vector<UISheet*> m_pSheets;
	uint32_t m_CurrentSheetIdx;

public:
	UI(const UI& ) = delete;
	UI(UI&& ) = delete;
	UI& operator=(const UI& ) = delete;
	UI& operator=(UI&& ) = delete;
};

