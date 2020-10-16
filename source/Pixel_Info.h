#pragma once
#include "EMath.h"
#include "ERGBColor.h"

using namespace Elite;

struct Pixel_Info
{
public:
	Pixel_Info(float depth = FLT_MAX, const FVector3& baryCoordinate = {}, const FVector2& uv = {}, const RGBColor& color = {}, const RGBColor& destColor = {}, const FVector3& normal = {}, const FPoint3& worldSpacePoint = {});

	float GetDepth() const;
	void SetDepth(float depth);

	float GetDepth_Z() const;
	void SetDepth_Z(float depth);

	FVector2 GetUV() const;
	void SetUV(const FVector2& uv);

	FVector3 GetBaryCoordinate() const;
	void SetBaryCoordinate(const FVector3& coordinate);

	RGBColor GetColor() const;
	void SetColor(const RGBColor& color);

	RGBColor GetDestColor() const;
	void SetDestColor(const RGBColor& color);

	FVector3 GetNormal() const;
	void SetNormal(const FVector3& normal);

	FPoint3 GetWorldSpacePoint() const;
	void SetWorldSpacePoint(const FPoint3& point);

	FVector3 GetTangent() const;
	void SetTangent(const FVector3& tangent);

private:
	float m_Depth; // w
	float m_Depth_z; // z
	FVector3 m_BaryCoordinate;
	FPoint3 m_P_WorldSpace;
	FVector2 m_UV;
	RGBColor m_Color;
	RGBColor m_DestColor; // Original color of that pixel on the backbuffer
	FVector3 m_Normal; // In world space
	FVector3 m_Tangent;
};

