#pragma once
#include "UIElement.h"
#include "EMath.h"

class Texture;

class UBox : public UIElement
{
public:
	UBox(const Elite::FPoint2& lt, float width, float height, bool shouldRender = true);
	~UBox();

	bool IsPointInside(const Elite::FPoint2& point);

	void SetColor(const Elite::RGBColor& color);
	void OverwriteTexture(Texture* pTexture);

	virtual void Update(float elapsedSec) override;
	virtual void Render(Window& window) const override;

	void Move(const Elite::FVector2& movement);
	void SetLT(const Elite::FPoint2& position);
	Elite::FPoint2 GetLT() const;

	// Input:
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed) override;
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed) override;
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed) override;

protected:
	Elite::FPoint2 m_LT;
	float m_Width;
	float m_Height;

	Elite::RGBColor m_Color;
	Texture* m_pTexture;

	Elite::FVector2 CalcUVCoord(uint32_t c, uint32_t r) const;

public:
	UBox(const UBox&) = delete;
	UBox(UBox&&) = delete;
	UBox& operator=(const UBox&) = delete;
	UBox& operator=(UBox&&) = delete;
};

