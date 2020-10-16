#pragma once

// SDL_Window wrapper
#include "EMath.h"

#include "Texture.h"

class Window
{
public:
	Window(const std::string& windowTitle, uint32_t width, uint32_t height);
	~Window();

	SDL_Window* GetSDLWindow();
	void SetBBPixelColor(uint32_t r, uint32_t c, const Elite::RGBColor& color);
	Elite::RGBColor GetBBPixelColor(uint32_t r, uint32_t c) const;
	void Blit();

	void ColorClear(const Elite::RGBColor& color);
	void TextureClear(Texture const* pTexture);

	void LockBB();
	void UnlockBB();

	Elite::IVector2 GetWindowDimensions() const;

private:
	SDL_Window* m_pWindow;
	SDL_Surface* m_pFrontBuffer;
	SDL_Surface* m_pBackBuffer;
	uint32_t* m_pBackBufferPixels;
	uint32_t m_Width;
	uint32_t m_Height;

	SDL_Window* MakeWindow(const std::string& windowTitle, uint32_t width, uint32_t height);

public:
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;
};

