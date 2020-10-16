#include "pch.h"
#include "UI.h"

#include "UISheet.h"

UI::UI()
	: m_CurrentSheetIdx{}
{

}

void UI::Update(float elapsedSec)
{
	if (m_pSheets.empty()) return;

	m_pSheets[m_CurrentSheetIdx]->Update(elapsedSec);
}

void UI::AddSheet(UISheet* pSheet)
{
	m_pSheets.emplace_back(pSheet);
}

UISheet* UI::GetActiveSheet()
{
	return m_pSheets[m_CurrentSheetIdx];
}

void UI::ToggleActiveSheet()
{
	++m_CurrentSheetIdx;

	if (m_CurrentSheetIdx > m_pSheets.size()) m_CurrentSheetIdx = 0;
}

UI::~UI()
{
	for (UISheet* pSheet : m_pSheets)
	{
		delete pSheet;
		pSheet = nullptr;
	}
}

#pragma region Input
void UI::ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{
	if (m_pSheets.empty()) return;

	m_pSheets[m_CurrentSheetIdx]->ProcessKeyDownEvent(e, inputConsumed);
}
void UI::ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed)
{
	if (m_pSheets.empty()) return;

	m_pSheets[m_CurrentSheetIdx]->ProcessKeyUpEvent(e, inputConsumed);
}
void UI::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed)
{
	if (m_pSheets.empty()) return;

	m_pSheets[m_CurrentSheetIdx]->ProcessMouseMotionEvent(e, inputConsumed);
}
void UI::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{
	if (m_pSheets.empty()) return;

	m_pSheets[m_CurrentSheetIdx]->ProcessMouseDownEvent(e, inputConsumed);
}
void UI::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed)
{
	if (m_pSheets.empty()) return;

	m_pSheets[m_CurrentSheetIdx]->ProcessMouseUpEvent(e, inputConsumed);
}
#pragma endregion

bool UI::IsEmpty() const
{
	return m_pSheets.empty();
}