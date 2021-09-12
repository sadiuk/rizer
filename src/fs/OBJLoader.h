#include <string>
#include <filesystem>
#include <cassert>
#include "../math/vector.h"
#include "FileManager.h"

namespace fs
{
	class OBJLoader
	{
	public:
		struct ModelData
		{
			std::vector<vector3<float>> positions;
			std::vector<vector3<float>> normals;
			std::vector<vector3<uint32_t>> vtxIndices;
		};
		ModelData LoadModel(const std::filesystem::path& path)
		{
			std::istringstream entire_file(FileManager::GetFileContent(path));
			std::string current_line;
			ModelData res;
			while (std::getline(entire_file, current_line))
			{
				switch (current_line[0])
				{
				case 'v': // vertex-related stuff
				{
					switch (current_line[1])
					{
					case ' ': // vertex
					{
						vector3<float> vtx = LineToVector(current_line.substr(2, current_line.size() - 2));
						res.positions.push_back(vtx);
						break;
					}
					case 'n': // vertex normal
					{
						vector3<float> vtx = LineToVector(current_line.substr(3, current_line.size() - 3));
						res.normals.push_back(vtx);
						break;
					}
					default: continue; // TODO others;
					}
					break;
				}
				case 'f':
				{
					vector3<uint32_t> vertexIndices{}, normalIndices{};
					GetFace(current_line.substr(2, current_line.size() - 2), normalIndices, vertexIndices);
					res.vtxIndices.push_back(vertexIndices);
					break;
				}
				default: continue;
				}
			}
			return res;
		}

		vector3<float> LineToVector(const std::string& str)
		{
			std::string s = str;
			size_t pos = 0;
			std::string token;
			vector3<float> ret;
			size_t i = 0;
			while ((pos = s.find(' ')) != std::string::npos) {
				token = s.substr(0, pos);
				ret[i] = std::stof(token);
				s.erase(0, pos + 1);
				i++;
			}
			token = s.substr(0, pos);
			ret[i] = std::stof(token);
			return ret;
		}

		void GetFace(const std::string& str, vector3<uint32_t>& normalIndices, vector3<uint32_t>& vertexIndices)
		{
			std::string s = str;
			size_t pos = 0;
			std::string token1, token2, token3;
			size_t i = 0;
			while ((pos = s.find(' ')) != std::string::npos) {
				auto slash1Pos = s.find("/");
				auto slash2Pos = s.substr(slash1Pos + 1, s.size() - slash1Pos).find("/") + slash1Pos + 1;
				bool hasUV = slash2Pos - slash1Pos != 1;
				token1 = s.substr(0, slash1Pos);
				if(hasUV)
					token2 = s.substr(slash1Pos + 1, slash2Pos - slash1Pos - 1);
				token3 = s.substr(slash2Pos + 1, pos - slash2Pos - 1);
				vertexIndices[i] = stoi(token1) - 1;
				normalIndices[i] = stoi(token3) - 1;
				s.erase(0, pos + 1);
				i++;
			}
			auto slash1Pos = s.find("/");
			auto slash2Pos = s.substr(slash1Pos + 1, s.size() - slash1Pos).find("/") + slash1Pos + 1;
			bool hasUV = slash2Pos - slash1Pos != 1;
			token1 = s.substr(0, slash1Pos);
			if (hasUV)
				token2 = s.substr(slash1Pos + 1, slash2Pos - slash1Pos - 1);
			token3 = s.substr(slash2Pos + 1, pos - slash2Pos - 1);
			vertexIndices[i] = stoi(token1) - 1;
			normalIndices[i] = stoi(token3) - 1;
		}
	};
}