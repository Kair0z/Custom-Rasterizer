#pragma once

// This describes specific states the rasterizer is in:

enum class Viewtype
{
	Materials,
	Triangledata,
	Depth,
	Normals,
	Boundingbox,
	END
};

enum class Backgroundtype
{
	Color,
	Texture,
	END
};

enum class Cullmode
{
	Frontface, Backface, None, END
};

struct RasterDesc
{
	Elite::RGBColor m_ClearColor;
	Viewtype m_Viewtype;
	Backgroundtype m_BGType;
	Cullmode m_Cullmode;
};

struct PixelShadeDesc
{
	Viewtype m_Viewtype;
	Backgroundtype m_BGType;
	Cullmode m_Cullmode;
};