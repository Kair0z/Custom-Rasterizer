#include "pch.h"
#include "OBJ_Parser.h"
#include <regex>
#include <fstream>
#include <sstream>
#include "Utils.h"

std::unordered_map<std::string, std::vector<uint32_t>> OBJ_Parser::stat_LoadedIndexbuffers{};
std::unordered_map<std::string, std::vector<Input_Vertex>> OBJ_Parser::stat_LoadedVertexbuffers{};

enum class Linetype
{
	Comment, Vertex, Face, VertexNormal, VertTexCoordinate, Unknown
};

OBJ_Parser::OBJ_Parser(PrimitiveTopology topology)
	: m_Topology{topology}
{

}

PrimitiveTopology OBJ_Parser::GetTopology() const
{
	return m_Topology;
}

std::vector<std::string> OBJ_Parser::operator()(const std::string& filename, std::vector<Input_Vertex>& out_Vertices, std::vector<uint32_t>& out_Indices) const
{
	std::vector<std::string> out_Comments;

	// Check if already parsed once this run
	if (stat_LoadedIndexbuffers.find(filename) == stat_LoadedIndexbuffers.cend() && stat_LoadedVertexbuffers.find(filename) == stat_LoadedVertexbuffers.cend())
	{
		std::string fullPath = "Resources/" + filename + ".obj";

		std::vector<FPoint3> temp_Points;
		std::vector<FVector2> temp_UVs;
		std::vector<FVector3> temp_Normals;

		// BuildData() decides how many components every part (1/3) of the face has
		// 1 --> only vertex data! (bare minimum)
		// 2 --> only vertex coordinates!
		// 3 --> only vertex normals!
		int amountOfDatatypes{ 1 };

		BuildData(fullPath, out_Comments, temp_Points, temp_UVs, temp_Normals, amountOfDatatypes);

		BuildIdxBuffer(fullPath, out_Vertices, out_Indices, temp_Points, temp_UVs, temp_Normals, amountOfDatatypes);

		BuildTangents(out_Vertices, out_Indices);

		stat_LoadedIndexbuffers[filename] = out_Indices;
		stat_LoadedVertexbuffers[filename] = out_Vertices;

		return out_Comments;
	}
	else
	{
		out_Vertices = stat_LoadedVertexbuffers[filename];
		out_Indices = stat_LoadedIndexbuffers[filename];
	}

	return out_Comments;
}

#pragma region subMethods
void OBJ_Parser::BuildData(const std::string& filepath
	, std::vector<std::string>& returnComments
	, std::vector<FPoint3>& temp_Points
	, std::vector<FVector2>& temp_UVs
	, std::vector<FVector3>& temp_Normals
	, int& amountOfDatatypes) const
{
	std::ifstream input(filepath, std::ios::in);

	if (!input)
	{
		std::cout << "Error loading file! " << filepath << " \n";
	}

	std::string line{};
	Linetype lineType{};
	bool hasNormals{ false };
	bool hasCoordinates{ false };

	std::string signature{};
	std::vector<std::string> capturedGroups;

	// build all data except idx-buffer
	while (!input.eof())
	{
		std::getline(input, line);

		std::string regString{ "^\\s*(\\S+)\\s+(.*)" };
		std::regex captureSignRegex{ regString };
		capturedGroups = Utils::GetCaptureGroups(line, captureSignRegex);

		// if we captured something:
		if (capturedGroups.size() > 0)
		{
			signature = capturedGroups[1];

			// Set up linetype
			if (signature == "#") lineType = Linetype::Comment;
			else if (signature == "v") lineType = Linetype::Vertex;
			else if (signature == "f") lineType = Linetype::Face;
			else if (signature == "vn") lineType = Linetype::VertexNormal;
			else if (signature == "vt") lineType = Linetype::VertTexCoordinate;
			else lineType = Linetype::Unknown;

			//std::cout << capturedGroups[2] << "\n"; Actual content of the line (#: string of text), (v: 3 numbers), ...

			switch (lineType)
			{
			case Linetype::Vertex:
				temp_Points.emplace_back(CapturePoint(capturedGroups[2]));
				break;

			case Linetype::Comment:
				returnComments.push_back(capturedGroups[2]);
				break;

			case Linetype::Face:
				// Do nothing yet!
				break;

			case Linetype::VertexNormal:
				temp_Normals.emplace_back(CaptureNormal(capturedGroups[2]));
				hasNormals = true;
				break;

			case Linetype::VertTexCoordinate:
				temp_UVs.emplace_back(CaptureTexCoord(capturedGroups[2]));
				hasCoordinates = true;
				break;

			case Linetype::Unknown:
				// std::cout << "Unknown linetype found in file! \n";
				//std::cout << line << "\n";
				break;
			}

		}
	}

	amountOfDatatypes += int(hasCoordinates) + int(hasNormals);
}

void OBJ_Parser::BuildIdxBuffer(const std::string& filepath
	, std::vector<Input_Vertex>& vertices
	, std::vector<uint32_t>& indices
	, const std::vector<FPoint3>& temp_Points
	, const std::vector<FVector2>& temp_UVs
	, const std::vector<FVector3>& temp_Normals
	, int amountOfDatatypes) const
{
	std::string line{};
	std::vector<std::string> capturedGroups;

	std::ifstream input2(filepath, std::ios::in);

	if (!input2)
	{
		std::cout << "Error loading file! (input2) " << filepath << "\n";
	}

	while (!input2.eof())
	{
		std::getline(input2, line);

		std::string regString{ "^\\s*(\\S+)\\s+(.*)" };
		std::regex captureSignRegex{ regString };
		capturedGroups = Utils::GetCaptureGroups(line, captureSignRegex);

		if (capturedGroups.size() > 0)
		{
			if (capturedGroups[1] == "f")
			{
				CaptureFace(capturedGroups[2], vertices, indices, temp_Points, temp_UVs, temp_Normals, amountOfDatatypes);
			}
		}
	}
}

void OBJ_Parser::CaptureFace(const std::string& faceString
	, std::vector<Input_Vertex>& vertices
	, std::vector<uint32_t>& indices
	, const std::vector<FPoint3>& temp_Points
	, const std::vector<FVector2>& temp_UVs
	, const std::vector<FVector3>& temp_Normals
	, int amountOfDatatypes) const
{
	std::string group{};

	switch (amountOfDatatypes)
	{
	case 1:
		group = "(\\d+)"; // vertices only!
		break;

	case 2:
		group = "(\\d+\/\\d+)"; // vertices + vt!
		break;

	case 3:
		group = "(\\d+\/\\d+\/\\d+)"; // vertices + vt + vn!
		break;

	default:
		group = "DEADMEAT!";
		std::cout << "Uh oh!, Deadmeat!\n";
		break;
	}

	std::string fullString{ "(" };
	for (int i{}; i < 3; ++i)
	{
		fullString += group;
		fullString += "\\s*";
	}
	fullString += ")";

	std::regex reg(fullString);

	std::vector<std::string> captureElements = Utils::GetCaptureGroups(faceString, reg);

	std::string substrings[3];

	// If not all 3 + (full capture-offsets(2)) elements were captured, nothing more we can do!
	if (captureElements.size() != 5) return;

	substrings[0] = captureElements[2];
	substrings[1] = captureElements[3];
	substrings[2] = captureElements[4];

	Face face;
	switch (amountOfDatatypes)
	{
	case 1:
		group = "(\\d+)"; // vertices only!
		break;

	case 2:
		group = "(\\d+)\/(\\d+)"; // vertices + vt!
		break;

	case 3:
		group = "(\\d+)\/(\\d+)\/(\\d+)";; // vertices + vt + vn!
		break;

	default:
		group = "DEADMEAT!";
		std::cout << "Uh oh!, Deadmeat!\n";
		break;
	}

	// For each of the [amountOfDatatypes] face-elements
	for (int i{}; i < 3; ++i)
	{
		Input_Vertex vertex{};
		std::vector<std::string> capture;

		std::regex regex(group);
		capture = Utils::GetCaptureGroups(substrings[i], regex);

		std::stringstream ss;
		// Store data in your buffers: [1] & [2] & [3]
		// (1) idx:
		int idx{};
		ss.str(capture[1]);
		ss >> idx;
		ss.clear();

		face[i] = idx;
		vertex.SetPoint(temp_Points[face[i] - 1]);

		if (amountOfDatatypes > 1)
		{
			// (2) vt:
			int vt_Idx{};
			ss.str(capture[2]);
			ss >> vt_Idx;
			ss.clear();

			if (temp_UVs.size() > vt_Idx - 1) vertex.SetUV(temp_UVs[vt_Idx - 1]);
		}

		if (amountOfDatatypes > 2)
		{
			// (3) vn:
			int vn_Idx{};
			ss.str(capture[3]);
			ss >> vn_Idx;
			ss.clear();

			if (temp_Normals.size() > vn_Idx - 1) vertex.SetNormal(temp_Normals[vn_Idx - 1]);
		}

		indices.push_back(unsigned int(vertices.size()));
		vertices.push_back(vertex);
	}

	//// RHS 
	//indices.push_back(face[0] - 1);
	//indices.push_back(face[1] - 1);
	//indices.push_back(face[2] - 1);
}

FPoint3 OBJ_Parser::CapturePoint(const std::string& vertexString) const
{
	std::stringstream ss{ vertexString };

	FPoint3 temp{};

	float tempFloat{};

	for (int i{}; i < 3; ++i)
	{
		ss >> tempFloat;
		temp[i] = tempFloat;
	}

	return temp;
}

FVector2 OBJ_Parser::CaptureTexCoord(const std::string& vtString) const
{
	std::stringstream ss{ vtString };

	FVector2 uv_Temp{};

	for (int i{}; i < 2; ++i)
	{
		ss >> uv_Temp[i];
	}

	uv_Temp.y = 1 - uv_Temp.y;

	return uv_Temp;
}

FVector3 OBJ_Parser::CaptureNormal(const std::string& vnString) const
{
	std::stringstream ss{ vnString };

	FVector3 vn_Temp{};

	for (int i{}; i < 3; ++i)
	{
		ss >> vn_Temp[i];
	}

	Elite::Normalize(vn_Temp);

	return vn_Temp;
}
#pragma endregion

void OBJ_Parser::BuildTangents(std::vector<Input_Vertex>& out_Vertices, const std::vector<uint32_t>& out_Indices) const
{
	for (uint32_t i{}; i < out_Indices.size(); i += 3)
	{
		uint32_t i0 = out_Indices[i];
		uint32_t i1 = out_Indices[i + 1];
		uint32_t i2 = out_Indices[i + 2];

		const FPoint3& p0 = out_Vertices[i0].GetPoint();
		const FPoint3& p1 = out_Vertices[i1].GetPoint();
		const FPoint3& p2 = out_Vertices[i2].GetPoint();

		const FVector2& uv0 = out_Vertices[i0].GetUV();
		const FVector2& uv1 = out_Vertices[i1].GetUV();
		const FVector2& uv2 = out_Vertices[i2].GetUV();

		const FVector3 edge0 = p1 - p0;
		const FVector3 edge1 = p2 - p0;
		const FVector2 diffX = FVector2(uv1.x - uv0.x, uv2.x - uv0.x);
		const FVector2 diffY = FVector2(uv1.y - uv0.y, uv2.y - uv0.y);

		float r = 1.f / Elite::Cross(diffX, diffY);

		FVector3 tangent = (edge0 * diffY.y - edge1 * diffY.x) * r;

		out_Vertices[i0].SetTangent(tangent);
		out_Vertices[i1].SetTangent(tangent);
		out_Vertices[i2].SetTangent(tangent);
	}
	

	for (Input_Vertex& v : out_Vertices)
	{
		v.SetTangent(Elite::GetNormalized(Elite::Reject(v.GetTangent(), v.GetNormal())));
	}
}