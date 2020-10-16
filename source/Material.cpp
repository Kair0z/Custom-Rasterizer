#include "pch.h"
#include "Material.h"

Material::Material(const std::string& name)
	: m_Name{name}
	, m_pNormalmap{nullptr}
	, m_pDiffusemap{nullptr}
{

}

Material::~Material()
{
	if (m_pDiffusemap)
	{
		delete m_pDiffusemap;
		m_pDiffusemap = nullptr;
	}

	if (m_pNormalmap)
	{
		delete m_pNormalmap;
		m_pNormalmap = nullptr;
	}
}

void Material::OverwriteNormalMap(NormalMap* pNormalmap)
{
	if (m_pNormalmap)
	{
		delete m_pNormalmap;
		m_pNormalmap = nullptr;
	}

	m_pNormalmap = pNormalmap;
}

void Material::OverwriteDiffuseMap(Texture* pDiffusemap)
{
	if (m_pDiffusemap)
	{
		delete m_pDiffusemap;
		m_pDiffusemap = nullptr;
	}

	m_pDiffusemap = pDiffusemap;
}

void Material::SetDefaultColor(const RGBColor& color)
{
	m_DefaultColor = color;
}

void Material::SetDefaultColor(float r, float g, float b)
{
	m_DefaultColor.r = r;
	m_DefaultColor.g = g;
	m_DefaultColor.b = b;
}

NormalMap* Material::GetNormalmap() const
{
	return m_pNormalmap;
}

Texture* Material::GetDiffusemap() const
{
	return m_pDiffusemap;
}