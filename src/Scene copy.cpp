// #include "Scene.h"

// #include "Renderer.h"

// // struct MeshInfo
// // {
// // 	uint32_t positionOffset; // Offset into the positions array
// // 	uint32_t normalOffset;	 // Offset into the normals array
// // 	uint32_t indexOffset;	 // Offset into the indices array
// // 	uint32_t numVertices;	 // Number of vertices in this mesh
// // 	uint32_t numIndices;	 // Number of indices in this mesh
// // 	uint32_t numFaces;		 // Number of triangles (numIndices / 3)
// // 	uint32_t bvhNodeOffset;	 // Offset into the BVH nodes array for this mesh
// // 	uint32_t aabbOffset;	 // Offset into the AABB array for this mesh
// // };

// Scene::Scene()
// {
// 	auto shader = Shader::Create("CubeNormalShader", "Shaders/normal.vert", "Shaders/normal.frag");
// 	m_ShaderLibrary->Add(shader);
// 	m_MaterialLibrary->AddMaterial(Material::Create(shader));

// 	auto mesh = Mesh::LoadFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/cube.obj");

// 	// auto object = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/cube.obj");
// 	// auto object = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/1cube.obj");
// 	auto object = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/1sphere.obj");
// 	// auto object = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/dragon.fbx");
// 	object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
// 	object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
// 	const float scale = 1.0f;
// 	object->SetScale(glm::vec3(scale, scale, scale));
// 	object->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
// 	m_RootObjects.push_back(object);
// 	// m_RootObjects.push_back(object);

// 	// auto object2 = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/1sphere.obj");
// 	// object2->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
// 	// object2->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
// 	// const float scale2 = 1.0f;
// 	// object2->SetScale(glm::vec3(scale2, scale2, scale2));
// 	// object2->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
// 	// m_RootObjects.push_back(object2);


// 	m_AABBBuffer.initEmpty(1024 * 8 * sizeof(float), 11, GL_STREAM_DRAW);
// 	m_AABBBufferCount = 0;

// 	// auto positions = object->GetMesh()->GetPositions();
// 	// m_PositionsBuffer.init(positions.data(), static_cast<uint32_t>(sizeof(float) * positions.size()), 20, GL_STATIC_DRAW);
// 	// auto normals = object->GetMesh()->GetNormals();
// 	// m_NormalsBuffer.init(normals.data(), static_cast<uint32_t>(sizeof(float) * normals.size()), 21, GL_STATIC_DRAW);
// 	// auto indices = object->GetMesh()->GetIndices();
// 	// m_IndicesBuffer.init(indices.data(), static_cast<uint32_t>(sizeof(uint32_t) * indices.size()), 22, GL_STATIC_DRAW);

// 	m_PositionsBuffer.initEmpty(50000 * 6 * sizeof(uint32_t), 20, GL_STREAM_DRAW);
// 	m_NormalsBuffer.initEmpty(50000 * 6 * sizeof(uint32_t), 21, GL_STREAM_DRAW);
// 	m_IndicesBuffer.initEmpty(50000 * 6 * sizeof(uint32_t), 22, GL_STREAM_DRAW);

// 	m_OffsetBuffer.initEmpty(50000 * 6 * sizeof(uint32_t), 30, GL_STREAM_DRAW);

// 	// auto positions = object->GetMesh()->GetPositions();
// 	// m_PositionsBuffer.pushData(positions.data(), static_cast<uint32_t>(sizeof(float) * positions.size()), GL_STATIC_DRAW);
// 	// auto normals = object->GetMesh()->GetNormals();
// 	// m_NormalsBuffer.pushData(normals.data(), static_cast<uint32_t>(sizeof(float) * normals.size()), GL_STATIC_DRAW);
// 	// auto indices = object->GetMesh()->GetIndices();
// 	// m_IndicesBuffer.pushData(indices.data(), static_cast<uint32_t>(sizeof(uint32_t) * indices.size()), GL_STATIC_DRAW);

// 	m_OutputAABB.initEmpty(50000 * 6 * sizeof(float), 23, GL_STREAM_DRAW);

// 	m_ReccursiveAABB.initEmpty(50000 * 6 * sizeof(float), 24, GL_STREAM_DRAW);

// 	// m_Shader = m_ShaderLibrary->Get("CubeNormalShader");
// 	m_Shader = Shader::Create("aabbShader", "Shaders/aabb/aabb.vert", "Shaders/aabb/aabb.frag");

// 	if (!m_ComputeShader->LoadFromFile("Shaders/lbvh/createNodes.comp"))
// 	{
// 		exit(1);
// 	}
// 	if (!m_ComputeCalcMinMax->LoadFromFile("Shaders/lbvh/calculateBounds.comp"))
// 	{
// 		exit(1);
// 	}
// }

// Scene::~Scene()
// {
// }

// void Scene::Update(float deltaTime)
// {
// 	// Update logic for the scene
// }

// void Scene::OnImGuiRender()
// {
// 	ImGui::Begin(m_Name.c_str());
// 	// ImGui::Text("Hello, world!");
// 	// render all refrenc count for materials and shaders
// 	for (const auto &[name, material] : *m_MaterialLibrary)
// 	{
// 		ImGui::Text("Material name: %s", name.c_str());
// 		ImGui::Text("Material ref count: %d", material.use_count());
// 	}
// 	for (const auto &[name, shader] : *m_ShaderLibrary)
// 	{
// 		ImGui::Text("Shader name: %s", name.c_str());
// 		ImGui::Text("Shader ref count: %d", shader.use_count());
// 	}

// 	// if (ImGui::Button("Add Object"))
// 	// {
// 	// 	auto object = Object3D::Create("NewObject");
// 	// 	object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
// 	// 	object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
// 	// 	m_RootObjects.push_back(object);
// 	// }
// 	if (ImGui::Button("Add Cube"))
// 	{
// 		auto object = Object3D::CreateCube("NewObject");
// 		object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
// 		object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
// 		m_RootObjects.push_back(object);
// 	}
// 	ImGui::SameLine();
// 	if (ImGui::Button("Add Sphere"))
// 	{
// 		auto object = Object3D::CreateSphere("NewObject");
// 		object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
// 		object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
// 		m_RootObjects.push_back(object);
// 	}
// 	ImGui::SameLine();

// 	if (ImGui::Button("Add Triangle"))
// 	{
// 		auto object = Object3D::CreateTriangle("NewObject");
// 		object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
// 		object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
// 		m_RootObjects.push_back(object);
// 	}

// 	ImGui::Text("Objects in scene: %d", m_RootObjects.size());
// 	for (const auto &object : m_RootObjects)
// 	{
// 		ImGui::PushID(object->GetID()); // Unique ID for each object
// 		// ImGui::Text("Object name: %s", object->GetName().c_str());
// 		glm::vec3 position = object->GetPosition();
// 		ImGui::DragFloat3("Object Position", glm::value_ptr(position), 0.1f); // Modify the local copy
// 		object->SetPosition(position);										  // Update the object with the modified position

// 		glm::vec3 rotation = object->GetRotation();
// 		ImGui::DragFloat3("Object Rotation", glm::value_ptr(rotation), 0.1f); // Modify the local copy
// 		object->SetRotation(rotation);										  // Update the object with the modified position

// 		glm::vec3 scale = object->GetScale();
// 		ImGui::DragFloat3("Object Scale", glm::value_ptr(scale), 0.1f); // Modify the local copy
// 		object->SetScale(scale);										// Update the object with the modified position

// 		ImGui::PopID(); // Pop the ID for the object
// 	}
// 	ImGui::End();
// }

// void Scene::Render()
// {
// 	// Render logic for the scene

// 	{
// 		std::shared_ptr<Shader> cubeShader = m_ShaderLibrary->Get("CubeNormalShader");
// 		cubeShader->Bind();

// 		glm::mat4 model = glm::mat4(1.0f);
// 		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
// 		// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
// 		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
// 		model = glm::scale(model, glm::vec3(1.0f));

// 		cubeShader->SetMat4("viewProj", m_Camera->Matrix(45.0f, 0.1f, 100.0f));
// 		cubeShader->SetMat4("model", model);

// 		// Renderer::Submit(m_CubeMesh->GetVertexArray());
// 		// Renderer::Submit(Mesh::CreateDefaultCube()->GetVertexArray());

// 		model = glm::mat4(1.0f);

// 		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
// 		// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
// 		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
// 		model = glm::scale(model, glm::vec3(1.0f));

// 		cubeShader->SetMat4("viewProj", m_Camera->Matrix(45.0f, 0.1f, 100.0f));
// 		cubeShader->SetMat4("model", model);

// 		// Renderer::Submit(m_CubeMesh->GetVertexArray());
// 		Renderer::Submit(Mesh::CreateDefaultCube()->GetVertexArray());

// 		model = glm::mat4(1.0f);

// 		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
// 		// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
// 		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
// 		model = glm::scale(model, glm::vec3(1.0f));

// 		cubeShader->SetMat4("viewProj", m_Camera->Matrix(45.0f, 0.1f, 100.0f));
// 		cubeShader->SetMat4("model", model);

// 		// Renderer::Submit(Mesh::CreateDefaultSphere()->GetVertexArray());
// 		Renderer::Submit(Mesh::CreateDefaultTriangle()->GetVertexArray());
// 	}

// 	struct MeshInfo
// 	{
// 		uint32_t positionOffset; // Offset into the positions array
// 		uint32_t normalOffset;	 // Offset into the normals array
// 		uint32_t indexOffset;	 // Offset into the indices array
// 		uint32_t numVertices;	 // Number of vertices in this mesh
// 		uint32_t numIndices;	 // Number of indices in this mesh
// 		uint32_t numFaces;		 // Number of triangles (numIndices / 3)
// 		uint32_t bvhNodeOffset;	 // Offset into the BVH nodes array for this mesh
// 		uint32_t aabbOffset;	 // Offset into the AABB array for this mesh
// 	};

// 	std::vector<MeshInfo> meshInfos;
// 	uint32_t positionOffsetCounter = 0;
// 	uint32_t normalOffsetCounter = 0;
// 	uint32_t indexOffsetCounter = 0;
// 	uint32_t bvhNodeOffsetCounter = 0; // Assuming you have a counter for BVH nodes
// 	uint32_t aabbOffsetCounter = 0;	   // Assuming you have a counter for AABBs

// 	uint32_t totalFaces = 0;
// 	{
// 		// std::shared_ptr<Shader> cubeShader = m_ShaderLibrary->Get("CubeNormalShader");
// 		// cubeShader->Bind();
// 		m_PositionsBuffer.clear(GL_STREAM_DRAW);
// 		m_NormalsBuffer.clear(GL_STREAM_DRAW);
// 		m_IndicesBuffer.clear(GL_STREAM_DRAW);

// 		m_OffsetBuffer.clear(GL_STREAM_DRAW);
// 		glm::mat4 model = glm::mat4(1.0f);

// 		uint32_t meshIndex = 0;
// 		for (const auto &object : m_RootObjects)
// 		{
// 			model = glm::mat4(1.0f);
// 			auto mesh = object->GetMesh();
// 			auto positions = mesh->GetPositions();
// 			auto normals = mesh->GetNormals();
// 			auto indices = mesh->GetIndices();

// 			m_PositionsBuffer.pushData(positions.data(), static_cast<uint32_t>(sizeof(float) * positions.size()), GL_STREAM_DRAW);
// 			m_NormalsBuffer.pushData(normals.data(), static_cast<uint32_t>(sizeof(float) * normals.size()), GL_STATIC_DRAW);
// 			m_IndicesBuffer.pushData(indices.data(), static_cast<uint32_t>(sizeof(uint32_t) * indices.size()), GL_STATIC_DRAW);

// 			MeshInfo meshInfo;
// 			meshInfo.positionOffset = positionOffsetCounter;
// 			meshInfo.normalOffset = normalOffsetCounter;
// 			meshInfo.indexOffset = indexOffsetCounter;
// 			meshInfo.numVertices = static_cast<uint32_t>(positions.size() / 3);
// 			meshInfo.numIndices = static_cast<uint32_t>(indices.size());
// 			meshInfo.numFaces = meshInfo.numIndices / 3;
// 			meshInfo.bvhNodeOffset = bvhNodeOffsetCounter; // Assuming you have a counter for BVH nodes
// 			meshInfo.aabbOffset = aabbOffsetCounter;	   // Assuming you have a counter for AABBs

// 			positionOffsetCounter += meshInfo.numVertices * 3; // Assuming 3 floats per vertex
// 			normalOffsetCounter += meshInfo.numVertices * 3;   // Assuming 3 floats per normal
// 			indexOffsetCounter += meshInfo.numIndices;		   // Assuming 1 uint32_t per index
// 			bvhNodeOffsetCounter += meshInfo.numFaces * 2 - 1; // Assuming 2 nodes per face (for a BVH tree)
// 			aabbOffsetCounter += meshInfo.numFaces * 6;		   // Assuming 6 floats per AABB (min and max for x, y, z)
// 			totalFaces += meshInfo.numFaces;
// 			meshInfos.push_back(meshInfo);

// 			m_OffsetBuffer.pushData(&meshInfo, sizeof(MeshInfo), GL_STREAM_DRAW);

// 			// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
// 			auto shader = object->GetMaterial()->GetShader();
// 			glm::vec3 position = object->GetPosition();
// 			glm::vec3 rotation = object->GetRotation();
// 			glm::vec3 scale = object->GetScale();
// 			model = glm::translate(model, position);
// 			// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
// 			// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
// 			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
// 			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
// 			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
// 			model = glm::scale(model, scale);
// 			// model = glm::scale(model, glm::vec3(1.0f));

// 			shader->Bind();
// 			shader->SetMat4("viewProj", m_Camera->Matrix(45.0f, 0.1f, 100.0f));
// 			shader->SetMat4("model", model);
// 			// shader->SetMat4("model", glm::mat4(1.0f));
// 			// object->Render(m_Camera);
// 			// object->GetMesh()->GetVertexArray()->Bind();
// 			Renderer::Submit(object->GetMesh()->GetVertexArray());

// 			// continue;
// 			// LOG_INFO("Mesh index: {0}", meshIndex);
// 			//? LBVH pipeline
// 			// {
// 			// 	const uint32_t faceCount = meshInfo.numFaces;
// 			// 	const uint32_t nodeCount = faceCount * 2 - 1;

// 			// 	m_ComputeShader->Use();
// 			// 	// m_ComputeShader->SetMat4("model", model);
// 			// 	m_ComputeShader->SetMat4("model", glm::mat4(1.0f));
// 			// 	m_ComputeShader->SetUnsignedInt("meshIndex", meshIndex);

// 			// 	m_ComputeShader->Dispatch((nodeCount + 31) / 32, 1, 1);

// 			// 	// //? BVH visulization
// 			// 	int treeHeight = static_cast<int>(std::ceil(std::log2(faceCount)));
// 			// 	m_ComputeCalcMinMax->Use();
// 			// 	// m_ComputeCalcMinMax->SetUnsignedInt("numFaces", faceCount);

// 			// 	for (int level = treeHeight - 1; level >= 0; level--)
// 			// 	{
// 			// 		m_ComputeCalcMinMax->SetInt("currentLevel", level);
// 			// 		m_ComputeCalcMinMax->SetUnsignedInt("meshIndex", meshIndex);


// 			// 		// Calculate the number of nodes at this level
// 			// 		uint32_t levelStart = (1u << level) - 1;
// 			// 		uint32_t levelEnd = std::min((1u << (level + 1)) - 2, faceCount - 2);
// 			// 		uint32_t levelSize = levelEnd - levelStart + 1;

// 			// 		// Dispatch enough threads to cover all nodes at this level
// 			// 		uint32_t dispatchSize = (levelSize + 31) / 32;
// 			// 		m_ComputeCalcMinMax->Dispatch(dispatchSize, 1, 1);

// 			// 		// Ensure all writes are visible before processing the next level
// 			// 		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
// 			// 	}


// 			// 	m_Shader->Bind();
// 			// 	m_Shader->SetMat4("viewProj", m_Camera->Matrix(45.0f, 0.1f, 100.0f));
// 			// 	m_Shader->SetUnsignedInt("meshIndex", meshIndex);

// 			// 	// m_Shader->SetFloat("gridSize", 10.0f);
// 			// 	// m_Shader->SetMat4("model", model);

// 			// 	// Renderer::SubmitInstancedWireframe(Mesh::CreateWireframeCube()->GetVertexArray(), aabbCount);
// 			// 	Renderer::SubmitInstancedWireframe(Mesh::CreateWireframeCube()->GetVertexArray(), meshInfo.numFaces);
// 			// }
// 			meshIndex++;
// 		}

// 		// const uint32_t aabbCount = 12;
// 		// const uint32_t aabbCount = 8711;
// 		const uint32_t aabbCount = 224;
// 		// const uint32_t
// 		m_ComputeShader->Use();
// 		m_ComputeShader->SetMat4("model", model);
// 		m_ComputeShader->SetUnsignedInt("meshIndex", 0);

// 		// m_ComputeShader->Dispatch(5000, 1, 1);
// 		const uint32_t totalNodes = aabbCount * 2 - 1;
// 		m_ComputeShader->Dispatch((totalNodes + 31) / 32, 1, 1);

// 		int treeHeight = static_cast<int>(std::ceil(std::log2(aabbCount)));
// 		m_ComputeCalcMinMax->Use();
// 		m_ComputeCalcMinMax->SetUnsignedInt("numFaces", aabbCount);

// 		for (int level = treeHeight - 1; level >= 0; level--)
// 		{
// 			m_ComputeCalcMinMax->SetInt("currentLevel", level);

// 			// Calculate the number of nodes at this level
// 			uint32_t levelStart = (1u << level) - 1;
// 			uint32_t levelEnd = std::min((1u << (level + 1)) - 2, aabbCount - 2);
// 			uint32_t levelSize = levelEnd - levelStart + 1;

// 			// Dispatch enough threads to cover all nodes at this level
// 			uint32_t dispatchSize = (levelSize + 31) / 32;
// 			m_ComputeCalcMinMax->Dispatch(dispatchSize, 1, 1);

// 			// Ensure all writes are visible before processing the next level
// 			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
// 		}

// 		// ? Visualize the AABBs
// 		m_Shader->Bind();
// 		m_Shader->SetMat4("viewProj", m_Camera->Matrix(45.0f, 0.1f, 100.0f));
// 		m_Shader->SetUnsignedInt("meshIndex", 0);
// 		// m_Shader->

// 		// m_Shader->SetFloat("gridSize", 10.0f);
// 		// m_Shader->SetMat4("model", model);

// 		// Renderer::SubmitInstancedWireframe(Mesh::CreateWireframeCube()->GetVertexArray(), aabbCount);
// 		Renderer::SubmitInstancedWireframe(Mesh::CreateWireframeCube()->GetVertexArray(), totalFaces);
// 	}
// }