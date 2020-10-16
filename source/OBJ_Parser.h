#pragma once
#include <unordered_map>
#include "Vertex.h"
#include <string>
#include <vector>

using namespace Elite;

struct Face
{
	Face() = default;
	Face(int v0, int v1, int v2) : m_0{ v0 }, m_1{ v1 }, m_2{ v2 }
	{

	}

	int m_0, m_1, m_2;

	int& operator[](int idx)
	{
		switch (idx)
		{
		case 1:
			return m_1;
			break;

		case 2:
			return m_2;
			break;

		case 0:
			return m_0;
			break;
		}

		return m_2;
	}

	int operator[](int idx) const
	{
		switch (idx)
		{
		case 1:
			return m_1;
			break;

		case 2:
			return m_2;
			break;

		case 0:
			return m_0;
			break;
		}

		return m_2;
	}
};

enum class PrimitiveTopology
{
	Trianglelist, Trianglestrip
};

class OBJ_Parser
{
public:
	OBJ_Parser(PrimitiveTopology topology = PrimitiveTopology::Trianglelist);

	// Parse call (operates on output-parameters)
	std::vector<std::string> operator()(const std::string& filename, std::vector<Input_Vertex>& out_Vertices, std::vector<uint32_t>& out_Indices) const;
	PrimitiveTopology GetTopology() const;

private:
	PrimitiveTopology m_Topology;

	// That's a lot of parameters :p

	// Helper functions of parsing the file:
	FPoint3 CapturePoint(const std::string& dataString) const;
	FVector2 CaptureTexCoord(const std::string& dataString) const;
	FVector3 CaptureNormal(const std::string& dataString) const;

	// Build all data in temporary vectors 
	void BuildData(const std::string& path,
		std::vector<std::string>& out_Comments,
		std::vector<FPoint3>& out_Points,
		std::vector<FVector2>& out_TexCoords,
		std::vector<FVector3>& out_Normals,
		int& amountOfDataTypesPerVertex) const;

	// Sort Built data with Idxbuffer
	void BuildIdxBuffer(const std::string& path,
		std::vector<Input_Vertex>& out_VertexBuffer,
		std::vector<uint32_t>& out_IdxBuffer,
		const std::vector<FPoint3>& built_Points,
		const std::vector<FVector2>& built_UVs,
		const std::vector<FVector3>& built_Normals,
		const int amountOfDatatypesPerVertex) const;

	// Capture 1 face (arrange data in vertexBuffer accordingly)
	void CaptureFace(const std::string& faceString,
		std::vector<Input_Vertex>& out_Vertices,
		std::vector<uint32_t>& out_Indices,
		const std::vector<FPoint3>& builtPoints,
		const std::vector<FVector2>& built_UVs,
		const std::vector<FVector3>& built_Normals,
		int amountOfDatatypes) const;

	void BuildTangents(std::vector<Input_Vertex>& out_Vertices, const std::vector<uint32_t>& out_Indices) const;

	// maps of parsed objects so we don't have to re-parse them
	static std::unordered_map <std::string, std::vector<Input_Vertex>> stat_LoadedVertexbuffers;
	static std::unordered_map <std::string, std::vector<uint32_t>> stat_LoadedIndexbuffers;
};

