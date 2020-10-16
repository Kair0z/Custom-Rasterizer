#include "pch.h"
#include "Window.h"

#pragma region Initialization
Window::Window(const std::string& windowTitle, uint32_t width, uint32_t height)
{
	m_pWindow = MakeWindow(windowTitle, width, height);

	int w, h{};

	SDL_GetWindowSize(m_pWindow, &w, &h);
	m_Width = static_cast<uint32_t>(w);
	m_Height = static_cast<uint32_t>(h);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pFrontBuffer = SDL_GetWindowSurface(m_pWindow);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
}

SDL_Window* Window::MakeWindow(const std::string& windowTitle, uint32_t width, uint32_t height)
{
	SDL_Window* pWindow = SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height, 0);

	if (!pWindow)
	{
		std::cout << "Error: Master::MakeWindow( ): Could not create window! \n";
		return nullptr;
	}

	return pWindow;
}

#pragma endregion

SDL_Window* Window::GetSDLWindow()
{
	return m_pWindow;
}

void Window::SetBBPixelColor(uint32_t r, uint32_t c, const Elite::RGBColor& color)
{
	m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
		static_cast<uint8_t>(255.f * color.r),
		static_cast<uint8_t>(255.f * color.g),
		static_cast<uint8_t>(255.f * color.b));
}

Elite::RGBColor Window::GetBBPixelColor(uint32_t r, uint32_t c) const
{
	Uint8 red, green, blue;
	SDL_GetRGB(m_pBackBufferPixels[c + (r * m_Width)], m_pBackBuffer->format, &red, &green, &blue);

	Elite::RGBColor color{ float(red), float(green), float(blue) };
	color /= 255.f;

	return color;
}

void Window::Blit()
{
	UnlockBB();
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

void Window::ColorClear(const Elite::RGBColor& color)
{
	for (uint32_t r{}; r < m_Height; ++r)
	{
		for (uint32_t c{}; c < m_Width; ++c)
		{
			SetBBPixelColor(r, c, color);
		}
	}
}

void Window::TextureClear(Texture const* pTexture)
{
	Elite::RGBColor color{};
	Elite::FVector2 uv{};

	float out_Alpha{};

	for (uint32_t r{}; r < m_Height; ++r)
	{
		for (uint32_t c{}; c < m_Width; ++c)
		{
			uv.x = Elite::Remap(float(c), 0.f, float(m_Width));
			uv.y = Elite::Remap(float(r), 0.f, float(m_Height));

			if (pTexture) pTexture->Sample(uv, color, out_Alpha);

			SetBBPixelColor(r, c, color);
		}
	}
}

#pragma region SimpleFunctions
void Window::LockBB()
{
	SDL_LockSurface(m_pBackBuffer);
}

void Window::UnlockBB()
{
	SDL_UnlockSurface(m_pBackBuffer);
}

Elite::IVector2 Window::GetWindowDimensions() const
{
	return Elite::IVector2{ int(m_Width), int(m_Height) };
}
#pragma endregion

#pragma region Cleanup
Window::~Window()
{
	SDL_DestroyWindow(m_pWindow);
}
#pragma endregion