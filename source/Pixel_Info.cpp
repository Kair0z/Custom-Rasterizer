#include "pch.h"
#include "Pixel_Info.h"

Pixel_Info::Pixel_Info(float depth, const FVector3& baryCoordinate, const FVector2& uv, const RGBColor& color, const RGBColor& destColor, const FVector3& normal, const FPoint3& worldSpacePoint)
	: m_Depth{depth}
	, m_BaryCoordinate{baryCoordinate}
	, m_UV{uv}
	, m_Color{color}
	, m_DestColor{destColor}
	, m_Normal{normal}
	, m_P_WorldSpace{worldSpacePoint}
{

}

#pragma region GetnSet

FVector3 Pixel_Info::GetTangent() const
{
	return m_Tangent;
}

void Pixel_Info::SetTangent(const FVector3& tangent)
{
	m_Tangent = tangent;
}

float Pixel_Info::GetDepth_Z() const
{
	return m_Depth_z;
}

void Pixel_Info::SetDepth_Z(float depth)
{
	m_Depth_z = depth;
}

FPoint3 Pixel_Info::GetWorldSpacePoint() const
{
	return m_P_WorldSpace;
}

void Pixel_Info::SetWorldSpacePoint(const FPoint3& point)
{
	m_P_WorldSpace = point;
}

float Pixel_Info::GetDepth() const
{
	return m_Depth;
}

void Pixel_Info::SetDepth(float depth)
{
	m_Depth = depth;
}

FVector2 Pixel_Info::GetUV() const
{
	return m_UV;
}

void Pixel_Info::SetUV(const FVector2& uv)
{
	m_UV = uv;
}

FVector3 Pixel_Info::GetBaryCoordinate() const
{
	return m_BaryCoordinate;
}

void Pixel_Info::SetBaryCoordinate(const FVector3& coordinate)
{
	m_BaryCoordinate = coordinate;
}

RGBColor Pixel_Info::GetColor() const
{
	return m_Color;
}

void Pixel_Info::SetColor(const RGBColor& color)
{
	m_Color = color;
}

FVector3 Pixel_Info::GetNormal() const
{
	return m_Normal;
}

void Pixel_Info::SetNormal(const FVector3& normal)
{
	m_Normal = normal;
}

RGBColor Pixel_Info::GetDestColor() const
{
	return m_DestColor;
}

void Pixel_Info::SetDestColor(const RGBColor& color)
{
	m_DestColor = color;
}

#pragma endregion

