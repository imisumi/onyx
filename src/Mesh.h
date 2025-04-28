#pragma once

#include "VertexArray.h"

#include <filesystem>

#include <assimp/mesh.h>

#include "Base.h"

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;

	void Init(const std::vector<float> &vertices, const std::vector<uint32_t> &indices, BufferLayout layout);

	std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }

	static std::shared_ptr<Mesh> CreateDefaultCube();
	static std::shared_ptr<Mesh> CreateWireframeCube();
	static std::shared_ptr<Mesh> CreateDefaultTriangle();
	static std::shared_ptr<Mesh> CreateDefaultSphere(float radius = 0.5f, int latitudeSegments = 5, int longitudeSegments = 5);

	static std::shared_ptr<Mesh> LoadFromFile(const std::filesystem::path &file);

	// const std::vector<float> &GetVertices() const { return m_Vertices; }
	const std::vector<float> &GetPositions() const
	{
		if (m_Positions.empty())
		{
			LOG_WARN("Mesh::GetPositions() - Positions are empty. Ensure the mesh is initialized correctly.");
		}
		return m_Positions;
	}
	const std::vector<float> &GetNormals() const
	{
		if (m_Normals.empty())
		{
			LOG_WARN("Mesh::GetNormals() - Normals are empty. Ensure the mesh is initialized correctly.");
		}
		return m_Normals;
	}
	const std::vector<uint32_t> &GetIndices() const
	{
		if (m_Indices.empty())
		{
			LOG_WARN("Mesh::GetIndices() - Indices are empty. Ensure the mesh is initialized correctly.");
		}
		return m_Indices;
	}

private:
	static std::shared_ptr<Mesh> ExtractMesh(aiMesh *aiMesh);

	// static std::shared_ptr<Mesh> CreateDefaultCube(const std::string &name = "DefaultObject3D")
	// {
	// 	auto object = std::make_shared<Mesh>();
	// 	object->SetMesh(Mesh::CreateDefaultCube());
	// 	return object;
	// }

private:
	std::vector<float> m_Vertices;
	std::shared_ptr<VertexArray> m_VertexArray;

	std::vector<float> m_Positions;
	std::vector<float> m_Normals;
	std::vector<uint32_t> m_Indices;

	// const uint32_t m_
	// std::vector<float> m_Vertices;
	// std::vector<uint32_t> m_Indices;
};
