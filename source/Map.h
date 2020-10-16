#pragma once
#include "Pixel_Info.h"

using namespace Elite;

class NormalMap
{
public:
	NormalMap(const std::string& filepath);

	void Sample(const FVector2& uv, const Pixel_Info& pixInfo, FVector3& out_Normal) const;

	NormalMap(const NormalMap&) = delete;
	NormalMap(NormalMap&&) = delete;
	NormalMap& operator=(const NormalMap&) = delete;
	NormalMap& operator=(NormalMap&&) = delete;
	~NormalMap();

private:
	SDL_Surface* m_pSurface;
};

struct ValueMap
{
public:
	ValueMap(const std::string& filepath);
	
	void Sample(const Elite::FVector2& uv, float& returnValue) const; // Average of 3 channels (not using alpha)

	ValueMap(const ValueMap&) = delete;
	ValueMap(ValueMap&&) = delete;
	ValueMap& operator=(const ValueMap&) = delete;
	ValueMap& operator=(ValueMap&&) = delete;
	~ValueMap();

private:
	SDL_Surface* m_pSurface;

};
