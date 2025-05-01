#pragma once

#include "Mesh.h"
#include <numbers>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

void Mesh::Init(const std::vector<float> &vertex_data, const std::vector<uint32_t> &indices, BufferLayout layout)
{
	m_VertexArray = std::make_shared<VertexArray>();

	std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertex_data);
	// std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertex_data);
	vertexBuffer->SetLayout(layout);

	m_VertexArray->AddVertexBuffer(vertexBuffer);

	std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices);
	m_VertexArray->SetIndexBuffer(indexBuffer);
}

std::shared_ptr<Mesh> Mesh::CreateDefaultCube()
{
	auto mesh = std::make_shared<Mesh>();

	// Define positions (8 unique corners)

	const float halfSize = 1.0f; // Half size of the cube
	std::vector<float> positions = {
		// Front face
		-halfSize, -halfSize, halfSize, // 0: front-bottom-left
		halfSize, -halfSize, halfSize,	// 1: front-bottom-right
		halfSize, halfSize, halfSize,	// 2: front-top-right
		-halfSize, halfSize, halfSize,	// 3: front-top-left

		// Back face
		-halfSize, -halfSize, -halfSize, // 4: back-bottom-left
		halfSize, -halfSize, -halfSize,	 // 5: back-bottom-right
		halfSize, halfSize, -halfSize,	 // 6: back-top-right
		-halfSize, halfSize, -halfSize	 // 7: back-top-left
	};

	// Define indices (for positions)
	std::vector<uint32_t> indices = {
		// Front face
		0, 1, 2, 2, 3, 0,

		// Right face
		1, 5, 6, 6, 2, 1,

		// Back face
		5, 4, 7, 7, 6, 5,

		// Left face
		4, 0, 3, 3, 7, 4,

		// Top face
		3, 2, 6, 6, 7, 3,

		// Bottom face
		4, 5, 1, 1, 0, 4};

	// Define normals (for each face)
	std::vector<float> faceNormals = {
		0.0f, 0.0f, 1.0f,  // Front
		1.0f, 0.0f, 0.0f,  // Right
		0.0f, 0.0f, -1.0f, // Back
		-1.0f, 0.0f, 0.0f, // Left
		0.0f, 1.0f, 0.0f,  // Top
		0.0f, -1.0f, 0.0f  // Bottom
	};

	// Now combine the data into an interleaved vertex buffer
	std::vector<float> vertexData;

	// For each face (6 faces)
	for (int face = 0; face < 6; face++)
	{
		// Get face normal
		float nx = faceNormals[face * 3];
		float ny = faceNormals[face * 3 + 1];
		float nz = faceNormals[face * 3 + 2];

		// Get the 6 indices for this face (2 triangles)
		for (int i = 0; i < 6; i++)
		{
			int vertexIndex = indices[face * 6 + i];

			// Add position
			vertexData.push_back(positions[vertexIndex * 3]);	  // x
			vertexData.push_back(positions[vertexIndex * 3 + 1]); // y
			vertexData.push_back(positions[vertexIndex * 3 + 2]); // z

			// Add normal
			vertexData.push_back(nx);
			vertexData.push_back(ny);
			vertexData.push_back(nz);

			// You can add additional attributes here in the future
			// e.g., texture coordinates, tangents, etc.
		}
	}

	// Create sequential indices for the final mesh
	std::vector<uint32_t> finalIndices;
	for (uint32_t i = 0; i < 36; i++)
	{ // 36 vertices total (6 faces * 2 triangles * 3 vertices)
		finalIndices.push_back(i);
	}

	// Initialize the mesh with the vertex layout
	mesh->Init(vertexData, finalIndices, {
											 {ShaderDataType::Float3, "aPos"}, {ShaderDataType::Float3, "aNormal"} // Add more attributes here as needed
										 });

	return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateDefaultSphere(float radius, int latitudeSegments, int longitudeSegments)
{
	auto mesh = std::make_shared<Mesh>();

	// Store positions, normals, and indices separately

	// mesh->m_Indices = indices;
	// mesh->m_Positions = positions;
	// mesh->m_Normals = normals;

	const float PI = std::numbers::pi_v<float>;

	// Create sphere vertices
	for (int lat = 0; lat <= latitudeSegments; lat++)
	{
		// Calculate phi (vertical angle)
		float phi = lat * PI / latitudeSegments;
		float sinPhi = std::sin(phi);
		float cosPhi = std::cos(phi);

		for (int lon = 0; lon <= longitudeSegments; lon++)
		{
			// Calculate theta (horizontal angle)
			float theta = lon * 2 * PI / longitudeSegments;
			float sinTheta = std::sin(theta);
			float cosTheta = std::cos(theta);

			// Calculate position on unit sphere
			float x = cosTheta * sinPhi;
			float y = cosPhi;
			float z = sinTheta * sinPhi;

			// Add position (scaled by radius)
			mesh->m_Positions.push_back(radius * x);
			mesh->m_Positions.push_back(radius * y);
			mesh->m_Positions.push_back(radius * z);

			// For a sphere, the normal is just the normalized position
			mesh->m_Normals.push_back(x);
			mesh->m_Normals.push_back(y);
			mesh->m_Normals.push_back(z);
		}
	}

	// Create indices for triangles
	for (int lat = 0; lat < latitudeSegments; lat++)
	{
		for (int lon = 0; lon < longitudeSegments; lon++)
		{
			// Calculate the indices of the quad's corners
			int current = lat * (longitudeSegments + 1) + lon;
			int next = current + 1;
			int bottom = current + (longitudeSegments + 1);
			int bottomNext = bottom + 1;

			// Top triangle of the quad
			mesh->m_Indices.push_back(current);
			mesh->m_Indices.push_back(next);
			mesh->m_Indices.push_back(bottom);

			// Bottom triangle of the quad
			mesh->m_Indices.push_back(next);
			mesh->m_Indices.push_back(bottomNext);
			mesh->m_Indices.push_back(bottom);
		}
	}

	// Combine positions and normals into interleaved vertex data
	std::vector<float> vertexData;
	int numVertices = mesh->m_Positions.size() / 3;

	for (int i = 0; i < numVertices; i++)
	{
		// Add position
		vertexData.push_back(mesh->m_Positions[i * 3]);
		vertexData.push_back(mesh->m_Positions[i * 3 + 1]);
		vertexData.push_back(mesh->m_Positions[i * 3 + 2]);

		// Add normal
		vertexData.push_back(mesh->m_Normals[i * 3]);
		vertexData.push_back(mesh->m_Normals[i * 3 + 1]);
		vertexData.push_back(mesh->m_Normals[i * 3 + 2]);

		// Additional vertex attributes can be added here
	}

	// Initialize the mesh
	mesh->Init(vertexData, mesh->m_Indices, {
												{ShaderDataType::Float3, "aPos"}, {ShaderDataType::Float3, "aNormal"} // Add more attributes as needed
											});
	return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateDefaultTriangle()
{
	auto mesh = std::make_shared<Mesh>();

	// Define positions (3 unique corners)
	std::vector<float> positions = {
		-0.5f, 0.0f, -0.5f, // Vertex 0
		0.5f, 0.0f, -0.5f,	// Vertex 1
		0.0f, 0.0f, 0.5f,	// Vertex 2
	};

	// Define normals (for each vertex)
	std::vector<float> normals = {
		0.0f, 1.0f, 0.0f, // Normal for vertex 0
		0.0f, 1.0f, 0.0f, // Normal for vertex 1
		0.0f, 1.0f, 0.0f  // Normal for vertex 2
	};

	// Now combine the data into an interleaved vertex buffer
	std::vector<float> vertexData;

	// For each vertex
	for (int i = 0; i < 3; i++)
	{
		// Add position
		vertexData.push_back(positions[i * 3]);		// x
		vertexData.push_back(positions[i * 3 + 1]); // y
		vertexData.push_back(positions[i * 3 + 2]); // z

		// Add normal
		vertexData.push_back(normals[i * 3]);	  // nx
		vertexData.push_back(normals[i * 3 + 1]); // ny
		vertexData.push_back(normals[i * 3 + 2]); // nz
	}

	// Create sequential indices for the final mesh
	std::vector<uint32_t> finalIndices = {0, 1, 2};

	// Initialize the mesh with the vertex layout
	mesh->Init(vertexData, finalIndices, {{ShaderDataType::Float3, "aPos"}, {ShaderDataType::Float3, "aNormal"}});

	return mesh;
}

struct LogStream : public Assimp::LogStream
{
	static void Initialize()
	{
		if (Assimp::DefaultLogger::isNullLogger())
		{
			Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
			Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
		}
	}

	void write(const char *message) override
	{
		LOG_ERROR("Assimp error: {0}", message);
	}
};

namespace
{
	const unsigned int ImportFlags =
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_PreTransformVertices |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_OptimizeMeshes |
		aiProcess_Debone |
		aiProcess_ValidateDataStructure;
}

// std::shared_ptr<Mesh> ExtractMesh(aiMesh *aiMesh);

std::shared_ptr<Mesh> Mesh::LoadFromFile(const std::filesystem::path &file)
{
	LogStream::Initialize();

	LOG_INFO("Loading mesh from file: {0}", file.string());

	// check if regular file and if exists
	MY_ASSERT(std::filesystem::is_regular_file(file), "File does not exist or is not a regular file: {0}", file.string());

	Assimp::Importer importer;

	const aiScene *aiScene = importer.ReadFile(file.string(), ImportFlags);
	if (!aiScene->HasMeshes())
	{
		LOG_ERROR("Failed to load mesh file: {0}", file.string().c_str());
	}

	uint32_t numMeshes = aiScene->mNumMeshes;
	LOG_INFO("Number of meshes: {0}", numMeshes);

	std::shared_ptr<Mesh> mesh;
	for (uint32_t i = 0; i < numMeshes; i++)
	{
		aiMesh *aiMesh = aiScene->mMeshes[i];
		aiAABB aiAABB = aiMesh->mAABB;
		const char *meshName = aiMesh->mName.C_Str();

		mesh = ExtractMesh(aiMesh);
	}
	// LOG_INFO("Mesh name: {0}", mesh->GetName().c_str());
	return mesh;
	return nullptr;
};

std::shared_ptr<Mesh> Mesh::ExtractMesh(aiMesh *aiMesh)
{
	MY_ASSERT(aiMesh->HasPositions(), "Meshes require positions.");
	MY_ASSERT(aiMesh->HasNormals(), "Meshes require normals.");

	LOG_INFO("Mesh name: {0}", aiMesh->mName.C_Str());
	LOG_INFO("Mesh has {0} vertices", aiMesh->mNumVertices);
	LOG_INFO("Mesh has {0} faces", aiMesh->mNumFaces);
	LOG_INFO("Mesh has {0} texture coordinates", aiMesh->GetNumUVChannels());
	LOG_INFO("Mesh has {0} tangents", aiMesh->HasTangentsAndBitangents() ? "yes" : "no");
	LOG_INFO("Mesh has {0} colors", aiMesh->GetNumColorChannels());
	LOG_INFO("Mesh has {0} materials", aiMesh->mMaterialIndex);
	LOG_INFO("Mesh has {0} indices", aiMesh->mNumFaces * 3);

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	mesh->m_Positions.reserve(aiMesh->mNumVertices * 3);
	mesh->m_Normals.reserve(aiMesh->mNumVertices * 3);

	for (uint32_t i = 0; i < aiMesh->mNumVertices; i++)
	{
		mesh->m_Positions.push_back(aiMesh->mVertices[i].x);
		mesh->m_Positions.push_back(aiMesh->mVertices[i].y);
		mesh->m_Positions.push_back(aiMesh->mVertices[i].z);

		mesh->m_Normals.push_back(aiMesh->mNormals[i].x);
		mesh->m_Normals.push_back(aiMesh->mNormals[i].y);
		mesh->m_Normals.push_back(aiMesh->mNormals[i].z);
	}

	mesh->m_Indices.reserve(aiMesh->mNumFaces * 3);
	for (uint32_t i = 0; i < aiMesh->mNumFaces; i++)
	{
		aiFace face = aiMesh->mFaces[i];
		MY_ASSERT(face.mNumIndices == 3, "Mesh faces must be triangles.");

		mesh->m_Indices.push_back(face.mIndices[0]);
		mesh->m_Indices.push_back(face.mIndices[1]);
		mesh->m_Indices.push_back(face.mIndices[2]);
	}

	LOG_INFO("Mesh has {0} indices", mesh->m_Indices.size());
	LOG_INFO("Mesh has {0} positions", mesh->m_Positions.size() / 3);
	LOG_INFO("Mesh has {0} normals", mesh->m_Normals.size() / 3);
	LOG_INFO("Mesh has {0} faces", mesh->m_Indices.size() / 3);

	std::vector<float> vertexData;
	vertexData.reserve(aiMesh->mNumVertices * 6); // 3 for position + 3 for normal

	for (uint32_t i = 0; i < aiMesh->mNumVertices; i++)
	{
		// Position
		vertexData.push_back(aiMesh->mVertices[i].x);
		vertexData.push_back(aiMesh->mVertices[i].y);
		vertexData.push_back(aiMesh->mVertices[i].z);

		// Normal
		vertexData.push_back(aiMesh->mNormals[i].x);
		vertexData.push_back(aiMesh->mNormals[i].y);
		vertexData.push_back(aiMesh->mNormals[i].z);
	}

	// for (auto i : mesh->m_Indices)
	// {
	// 	LOG_INFO("Index: {0}", i);
	// }

	// mesh->Init(vertexData, mesh->m_Indices, {{ShaderDataType::Float3, "aPos"}});
	mesh->Init(vertexData, mesh->m_Indices, {{ShaderDataType::Float3, "aPos"}, {ShaderDataType::Float3, "aNormal"}});

	return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateWireframeCube()
{
	auto mesh = std::make_shared<Mesh>();

	// Define positions (8 unique corners)
	const float halfSize = 1.0f; // Half size of the cube
	std::vector<float> positions = {
		-halfSize, -halfSize, halfSize,	 // 0: front-bottom-left
		halfSize, -halfSize, halfSize,	 // 1: front-bottom-right
		halfSize, halfSize, halfSize,	 // 2: front-top-right
		-halfSize, halfSize, halfSize,	 // 3: front-top-left
		-halfSize, -halfSize, -halfSize, // 4: back-bottom-left
		halfSize, -halfSize, -halfSize,	 // 5: back-bottom-right
		halfSize, halfSize, -halfSize,	 // 6: back-top-right
		-halfSize, halfSize, -halfSize	 // 7: back-top-left
	};

	// Define indices for lines (12 edges of a cube)
	std::vector<uint32_t> indices = {
		0, 1, // Front bottom
		1, 2, // Front right
		2, 3, // Front top
		3, 0, // Front left

		4, 5, // Back bottom
		5, 6, // Back right
		6, 7, // Back top
		7, 4, // Back left

		0, 4, // Bottom left
		1, 5, // Bottom right
		2, 6, // Top right
		3, 7  // Top left
	};

	// Create vertex data
	std::vector<float> vertexData;
	for (int i = 0; i < 8; i++)
	{
		// Position
		vertexData.push_back(positions[i * 3]);		// x
		vertexData.push_back(positions[i * 3 + 1]); // y
		vertexData.push_back(positions[i * 3 + 2]); // z

		// Normal (not really needed for lines, but keeping for compatibility)
		vertexData.push_back(0.0f);
		vertexData.push_back(0.0f);
		vertexData.push_back(0.0f);
	}

	// Initialize the mesh with the vertex layout
	mesh->Init(vertexData, indices, {{ShaderDataType::Float3, "aPos"}, {ShaderDataType::Float3, "aNormal"}});

	return mesh;
}