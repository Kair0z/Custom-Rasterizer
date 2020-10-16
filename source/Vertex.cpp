#include "pch.h"
#include "Vertex.h"

using namespace Elite;

// ---------------------------------------------------
// Input Vertex (before projection)
// ---------------------------------------------------
Input_Vertex::Input_Vertex(const FPoint3& point,
	const FVector3& normal,
	const FVector2& uv,
	const RGBColor& color,
	const FVector3& tangent,
	const FPoint3& worldPoint)
	: m_Point{point}
	, m_Normal{normal}
	, m_UV{uv}
	, m_Color{color}
	, m_Tangent{tangent}
	, m_WorldPoint{worldPoint}
{

}

#pragma region Setters
void Input_Vertex::SetPoint(const FPoint3& point)
{
	m_Point = point;
}
void Input_Vertex::SetNormal(const FVector3& normal)
{
	m_Normal = normal;
}
void Input_Vertex::SetTangent(const FVector3& tangent)
{
	m_Tangent = tangent;
}
void Input_Vertex::SetUV(const FVector2& uv)
{
	m_UV = uv;
}
void Input_Vertex::SetColor(const RGBColor& color)
{
	m_Color = color;
}
void Input_Vertex::SetWorldPoint(const FPoint3& worldPoint)
{
	m_WorldPoint = worldPoint;
}
#pragma endregion

#pragma region Getters
FPoint3 Input_Vertex::GetPoint() const
{
	return m_Point;
}
FVector3 Input_Vertex::GetNormal() const
{
	return m_Normal;
}
FVector3 Input_Vertex::GetTangent() const
{
	return m_Tangent;
}
FVector2 Input_Vertex::GetUV() const
{
	return m_UV;
}
RGBColor Input_Vertex::GetColor() const
{
	return m_Color;
}
FPoint3 Input_Vertex::GetWorldPoint() const
{
	return m_WorldPoint;
}
#pragma endregion


// ---------------------------------------------------
// Output Vertex (after projection)
// ---------------------------------------------------

Output_Vertex::Output_Vertex(const FPoint4& point,
	const FVector3& normal,
	const FVector2& uv,
	const RGBColor& color,
	const FVector3& tangent,
	const FPoint3& worldPoint)
	: m_Point{point}
	, m_Normal{normal}
	, m_Color{color}
	, m_Tangent{tangent}
	, m_WorldPoint{worldPoint}
{

}

Output_Vertex::Output_Vertex(const Input_Vertex& input)
	: m_Point{input.GetPoint()}
	, m_Normal{input.GetNormal()}
	, m_Tangent{input.GetTangent()}
	, m_UV{input.GetUV()}
	, m_Color{input.GetColor()}
	, m_WorldPoint{input.GetWorldPoint()}

{

}

#pragma region Setters
void Output_Vertex::SetPoint(const FPoint4& point)
{
	m_Point = point;
}
void Output_Vertex::SetNormal(const FVector3& normal)
{
	m_Normal = normal;
}
void Output_Vertex::SetTangent(const FVector3& tangent)
{
	m_Tangent = tangent;
}
void Output_Vertex::SetUV(const FVector2& uv)
{
	m_UV = uv;
}
void Output_Vertex::SetColor(const RGBColor& color)
{
	m_Color = color;
}
void Output_Vertex::SetWorldPoint(const FPoint3& worldPoint)
{
	m_WorldPoint = worldPoint;
}
#pragma endregion

#pragma region Getters
FPoint4 Output_Vertex::GetPoint() const
{
	return m_Point;
}
FVector3 Output_Vertex::GetNormal() const
{
	return m_Normal;
}
FVector3 Output_Vertex::GetTangent() const
{
	return m_Tangent;
}
FVector2 Output_Vertex::GetUV() const
{
	return m_UV;
}
RGBColor Output_Vertex::GetColor() const
{
	return m_Color;
}
FPoint3 Output_Vertex::GetWorldPoint() const
{
	return m_WorldPoint;
}
#pragma endregion