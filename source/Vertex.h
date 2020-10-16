#pragma once
#include "EMath.h"
#include "ERGBColor.h"

using namespace Elite;

struct Input_Vertex
{
	Input_Vertex(const FPoint3& point = {}, 
		const FVector3& normal = {}, 
		const FVector2& uv = {}, 
		const RGBColor& color = {}, 
		const FVector3& tangent = {}, 
		const FPoint3& worldPoint = {});

	void SetPoint(const FPoint3& point);
	void SetNormal(const FVector3& normal);
	void SetTangent(const FVector3& tangent);
	void SetUV(const FVector2& uv);
	void SetColor(const RGBColor& color);
	void SetWorldPoint(const FPoint3& worldPoint);

	FPoint3 GetPoint() const;
	FVector3 GetNormal() const;
	FVector3 GetTangent() const;
	FVector2 GetUV() const;
	RGBColor GetColor() const;
	FPoint3 GetWorldPoint() const;


private:
	FPoint3 m_Point;
	FVector3 m_Normal;
	FVector3 m_Tangent;
	FVector2 m_UV;
	RGBColor m_Color;
	FPoint3 m_WorldPoint;
};

struct Output_Vertex
{
	Output_Vertex(const FPoint4& point = {},
		const FVector3& normal = {},
		const FVector2& uv = {},
		const RGBColor& color = {},
		const FVector3& tangent = {},
		const FPoint3& worldPoint = {});

	Output_Vertex(const Input_Vertex& input);

	void SetPoint(const FPoint4& point);
	void SetNormal(const FVector3& normal);
	void SetTangent(const FVector3& tangent);
	void SetUV(const FVector2& uv);
	void SetColor(const RGBColor& color);
	void SetWorldPoint(const FPoint3& worldPoint);

	FPoint4 GetPoint() const;
	FVector3 GetNormal() const;
	FVector3 GetTangent() const;
	FVector2 GetUV() const;
	RGBColor GetColor() const;
	FPoint3 GetWorldPoint() const;

private:
	FPoint4 m_Point;
	FVector3 m_Normal;
	FVector3 m_Tangent;
	FVector2 m_UV;
	RGBColor m_Color;
	FPoint3 m_WorldPoint;
};

