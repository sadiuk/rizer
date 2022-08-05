#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include <string>
#include <filesystem>
#include <cassert>
#include "../math/vector.h"
#include "FileManager.h"
#include "tiny_obj_loader.h"
#include <iostream>
namespace fs
{
	class OBJLoader
	{
	public:
		struct ModelData
		{
			struct Object
			{
				std::vector<glm::vec3> positions;
				std::vector<glm::vec3> normals;
				std::vector<glm::uvec3> vtxIndices;
			};
			std::vector<Object> objects;
		};
		ModelData LoadModel(const std::filesystem::path& path)
		{
			tinyobj::ObjReaderConfig reader_config;
			reader_config.mtl_search_path = "./"; // Path to material files

			tinyobj::ObjReader reader;

			if (!reader.ParseFromFile(path.string().c_str(), reader_config)) {
				if (!reader.Error().empty()) {
					std::cerr << "TinyObjReader: " << reader.Error();
				}
				exit(1);
			}
			if (!reader.Warning().empty()) {
				std::cout << "TinyObjReader: " << reader.Warning();
			}

			auto& attrib = reader.GetAttrib();
			auto& shapes = reader.GetShapes();
			auto& materials = reader.GetMaterials();
			ModelData ret;
			ret.objects.resize(shapes.size());
			for (int s = 0; s < shapes.size(); s++)
			{
				size_t indexOffset = 0;
				for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
				{
					size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
					for (size_t v = 0; v < fv; v++) {
						// access to vertex
						tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
						tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
						tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
						tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
						uint32_t index = (3 * size_t(idx.vertex_index));
						ret.objects[s].vtxIndices.push_back(glm::uvec3(index, index + 1, index + 2));
						ret.objects[s].positions.push_back(glm::vec3(vx, vy, vz));
						// Check if `normal_index` is zero or positive. negative = no normal data
						if (idx.normal_index >= 0) {
							tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
							tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
							tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
						}

						// Check if `texcoord_index` is zero or positive. negative = no texcoord data
						if (idx.texcoord_index >= 0) {
							tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
							tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
						}
					}
					indexOffset += fv;
				}
			}
			return ret;
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