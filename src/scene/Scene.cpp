#include "Scene.h"

#include "renderer/Renderer.h"

#include <iostream>
#include <vector>
#include <ranges>

Scene::Scene()
{
	{
		auto shader = Shader::Create("CubeNormalShader", "Shaders/normal.vert", "Shaders/normal.frag");
		m_ShaderLibrary->Add(shader);
		m_MaterialLibrary->AddMaterial(Material::Create(shader));

		auto mesh = Mesh::LoadFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/cube.obj");

		// auto object = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/cube.obj");
		// auto object = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/1cube.obj");
		auto object = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/1sphere.obj");
		// auto object = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/dragon.fbx");

		object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
		object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		const float scale = 1.0f;
		object->SetScale(glm::vec3(scale, scale, scale));
		object->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		m_RootObjects.push_back(object);
		// m_RootObjects.push_back(object);

		auto object2 = Object3D::CreateMeshFromFile("C:/Users/ichir/Desktop/rndr-rework-lbvh/working-sample/1sphere.obj");
		object2->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
		object2->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
		const float scale2 = 1.0f;
		object2->SetScale(glm::vec3(scale2, scale2, scale2));
		object2->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		m_RootObjects.push_back(object2);
	}

	{
		auto object = Object3D::CreateSphere("NewObject");
		object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
		object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		m_RootObjects.push_back(object);
	}

	m_AABBBuffer.initEmpty(1024 * 8 * sizeof(float), 11, GL_STREAM_DRAW);
	m_AABBBufferCount = 0;

	// auto positions = object->GetMesh()->GetPositions();
	// m_PositionsBuffer.init(positions.data(), static_cast<uint32_t>(sizeof(float) * positions.size()), 20, GL_STATIC_DRAW);
	// auto normals = object->GetMesh()->GetNormals();
	// m_NormalsBuffer.init(normals.data(), static_cast<uint32_t>(sizeof(float) * normals.size()), 21, GL_STATIC_DRAW);
	// auto indices = object->GetMesh()->GetIndices();
	// m_IndicesBuffer.init(indices.data(), static_cast<uint32_t>(sizeof(uint32_t) * indices.size()), 22, GL_STATIC_DRAW);

	const int maxMeshCount = 5000;
	m_PositionsBuffer.initEmpty(maxMeshCount * 6 * sizeof(uint32_t), 20, GL_STREAM_DRAW);
	m_NormalsBuffer.initEmpty(maxMeshCount * 6 * sizeof(uint32_t), 21, GL_STREAM_DRAW);
	m_IndicesBuffer.initEmpty(maxMeshCount * 6 * sizeof(uint32_t), 22, GL_STREAM_DRAW);

	m_OffsetBuffer.initEmpty(maxMeshCount * 6 * sizeof(uint32_t), 30, GL_STREAM_DRAW);

	// auto positions = object->GetMesh()->GetPositions();
	// m_PositionsBuffer.pushData(positions.data(), static_cast<uint32_t>(sizeof(float) * positions.size()), GL_STATIC_DRAW);
	// auto normals = object->GetMesh()->GetNormals();
	// m_NormalsBuffer.pushData(normals.data(), static_cast<uint32_t>(sizeof(float) * normals.size()), GL_STATIC_DRAW);
	// auto indices = object->GetMesh()->GetIndices();
	// m_IndicesBuffer.pushData(indices.data(), static_cast<uint32_t>(sizeof(uint32_t) * indices.size()), GL_STATIC_DRAW);

	m_OutputAABB.initEmpty(maxMeshCount * 6 * sizeof(float), 23, GL_STREAM_DRAW);

	m_ReccursiveAABB.initEmpty(maxMeshCount * 6 * sizeof(float), 24, GL_STREAM_DRAW);

	// m_Shader = m_ShaderLibrary->Get("CubeNormalShader");
	m_Shader = Shader::Create("aabbShader", "Shaders/aabb/aabb.vert", "Shaders/aabb/aabb.frag");

	if (!m_ComputeShader->LoadFromFile("Shaders/lbvh/createNodes.comp"))
	{
		exit(1);
	}
	if (!m_ComputeCalcMinMax->LoadFromFile("Shaders/lbvh/calculateBounds.comp"))
	{
		exit(1);
	}
}

Scene::~Scene()
{
}

void Scene::Update(float deltaTime)
{
	// Update logic for the scene
}

void Scene::RenderUI()
{
	ImGui::Begin(m_Name.c_str());

	ImGui::Checkbox("Visualize BLAS", &m_VisualizeBLAS);

	// ImGui::Text("Hello, world!");
	// render all refrenc count for materials and shaders
	for (const auto &[name, material] : *m_MaterialLibrary)
	{
		ImGui::Text("Material name: %s", name.c_str());
		ImGui::Text("Material ref count: %d", material.use_count());
	}
	for (const auto &[name, shader] : *m_ShaderLibrary)
	{
		ImGui::Text("Shader name: %s", name.c_str());
		ImGui::Text("Shader ref count: %d", shader.use_count());
	}

	// if (ImGui::Button("Add Object"))
	// {
	// 	auto object = Object3D::Create("NewObject");
	// 	object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
	// 	object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	// 	m_RootObjects.push_back(object);
	// }
	if (ImGui::Button("Add Cube"))
	{
		auto object = Object3D::CreateCube("NewObject");
		object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
		object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		m_RootObjects.push_back(object);
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Sphere"))
	{
		auto object = Object3D::CreateSphere("NewObject");
		object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
		object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		m_RootObjects.push_back(object);
	}
	ImGui::SameLine();

	if (ImGui::Button("Add Triangle"))
	{
		auto object = Object3D::CreateTriangle("NewObject");
		object->SetMaterial(m_MaterialLibrary->GetMaterial("CubeNormalShader"));
		object->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		m_RootObjects.push_back(object);
	}

	ImGui::End();
	{
		// Apply window styling for this specific window
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

		// Start the window
		ImGui::Begin("Scene Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);

		// Optional: Add a separator
		ImGui::Separator();
		ImGui::Spacing();

		// Render objects in hierarchy
		for (const auto &object : m_RootObjects)
		{
			// Improved tree node flags
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
									   ImGuiTreeNodeFlags_SpanAvailWidth |
									   ImGuiTreeNodeFlags_FramePadding;

			if (object == m_SelectedObject)
				flags |= ImGuiTreeNodeFlags_Selected;

			// Use a custom ID for the tree node (fixed cast)
			const void *id = reinterpret_cast<const void *>(static_cast<uintptr_t>(object->GetID()));

			// Important: Push color AFTER setting flags but BEFORE TreeNodeEx
			bool isSelected = (object == m_SelectedObject);
			if (isSelected)
			{
				// Subtle blue-grey color instead of bright blue
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.60f, 0.65f, 0.70f, 1.0f));
			}

			// Display the node
			std::string displayName = object->GetName();
			bool opened = ImGui::TreeNodeEx(id, flags, "%s", displayName.c_str());

			// Handle selection
			if (ImGui::IsItemClicked())
			{
				m_SelectedObject = object;
			}

			// Enhanced context menu
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete", "Del"))
				{
					LOG_WARN("Object deletion not yet implemented.");
				}

				ImGui::EndPopup();
			}

			// Important: Pop color AFTER TreeNodeEx
			if (isSelected)
			{
				ImGui::PopStyleColor(); // Matching pop for each push
			}

			if (opened)
			{
				// Render child entities here if needed
				// DrawChildEntityNode(entity);
				ImGui::TreePop();
			}
		}

		ImGui::End();
		ImGui::PopStyleVar(); // Matching pop for the window padding style
	}
	{
		ImGui::Begin("Properties");

		if (!m_SelectedObject)
		{
			ImGui::Text("No object selected");
			ImGui::End();
			return;
		}

		// Header styling
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));

		ImGui::Text("Transform");
		ImGui::Separator();

		const float labelWidth = 100.0f;
		const float axisButtonWidth = 20.0f;

		// Lambda function to handle a transform property (position, rotation, scale)
		auto HandleTransformProperty = [&](const char *label, glm::vec3 &values, float resetValue,
										   std::function<void(const glm::vec3 &)> updateFunc,
										   float speed = 0.1f, float minValue = 0.0f)
		{
			ImGui::Text("%s", label);
			ImGui::SameLine(labelWidth);

			bool valueChanged = false;
			const char *axisLabels[3] = {"X", "Y", "Z"};
			ImVec4 axisColors[3] = {
				ImVec4(0.7f, 0.2f, 0.2f, 1.0f), // Red for X
				ImVec4(0.2f, 0.7f, 0.2f, 1.0f), // Green for Y
				ImVec4(0.2f, 0.2f, 0.7f, 1.0f)	// Blue for Z
			};

			for (int i = 0; i < 3; i++)
			{
				// Only add SameLine after first component
				if (i > 0)
					ImGui::SameLine(0, 2);

				// Axis button with color
				ImGui::PushStyleColor(ImGuiCol_Button, axisColors[i]);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
									  ImVec4(axisColors[i].x + 0.2f, axisColors[i].y + 0.2f, axisColors[i].z + 0.2f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive,
									  ImVec4(axisColors[i].x - 0.2f, axisColors[i].y - 0.2f, axisColors[i].z - 0.2f, 1.0f));

				// Create unique ID for each button
				char buttonId[16];
				snprintf(buttonId, sizeof(buttonId), "%s##%s%d", axisLabels[i], label, i);

				if (ImGui::Button(buttonId, ImVec2(axisButtonWidth, 0)))
				{
					values[i] = resetValue;
					valueChanged = true;
				}
				ImGui::PopStyleColor(3);

				// Value field
				ImGui::SameLine(0, 2);

				// Only set item width on first element
				if (i == 0)
				{
					ImGui::PushItemWidth((ImGui::GetContentRegionAvail().x - axisButtonWidth * 2 - 4) / 3);
				}

				// Create unique ID for each drag field
				char dragId[16];
				snprintf(dragId, sizeof(dragId), "##%s%d", label, i);

				if (ImGui::DragFloat(dragId, &values[i], speed, minValue))
					valueChanged = true;
			}

			if (valueChanged)
				updateFunc(values);
		};

		// Handle Position
		{
			glm::vec3 position = m_SelectedObject->GetPosition();
			HandleTransformProperty("Translation", position, 0.0f,
									[&](const glm::vec3 &pos)
									{ m_SelectedObject->SetPosition(pos); });
		}

		// Handle Rotation
		{
			glm::vec3 rotation = m_SelectedObject->GetRotation();
			HandleTransformProperty("Rotation", rotation, 0.0f,
									[&](const glm::vec3 &rot)
									{ m_SelectedObject->SetRotation(rot); });
		}

		// Handle Scale
		{
			glm::vec3 scale = m_SelectedObject->GetScale();
			HandleTransformProperty("Scale", scale, 1.0f, [&](const glm::vec3 &scl)
									{ m_SelectedObject->SetScale(scl); }, 0.1f, 0.01f);
		}

		ImGui::PopStyleVar(); // FramePadding
		ImGui::End();
	}
}

void Scene::RenderViewport()
{
	return ;
	{
		std::shared_ptr<Shader> cubeShader = m_ShaderLibrary->Get("CubeNormalShader");
		cubeShader->Bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f));

		cubeShader->SetMat4("viewProj", m_EditorCamera->GetViewProjection());
		cubeShader->SetMat4("model", model);

		// Renderer::Submit(m_CubeMesh->GetVertexArray());
		// Renderer::Submit(Mesh::CreateDefaultCube()->GetVertexArray());

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f));

		cubeShader->SetMat4("viewProj", m_EditorCamera->GetViewProjection());
		cubeShader->SetMat4("model", model);

		// Renderer::Submit(m_CubeMesh->GetVertexArray());
		Renderer::Submit(Mesh::CreateDefaultCube()->GetVertexArray());

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		// model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f));

		cubeShader->SetMat4("viewProj", m_EditorCamera->GetViewProjection());
		cubeShader->SetMat4("model", model);

		// Renderer::Submit(Mesh::CreateDefaultSphere()->GetVertexArray());
		Renderer::Submit(Mesh::CreateDefaultTriangle()->GetVertexArray());

		cubeShader->Unbind();
	}
}

void Scene::PrepMeshBuffers()
{
	m_MeshInfos.clear();
	m_PositionsBuffer.clear(GL_STREAM_DRAW);
	m_NormalsBuffer.clear(GL_STREAM_DRAW);
	m_IndicesBuffer.clear(GL_STREAM_DRAW);
	m_OffsetBuffer.clear(GL_STREAM_DRAW);

	uint32_t positionOffsetCounter = 0;
	uint32_t normalOffsetCounter = 0;
	uint32_t indexOffsetCounter = 0;
	uint32_t bvhNodeOffsetCounter = 0; // Assuming you have a counter for BVH nodes
	uint32_t aabbOffsetCounter = 0;	   // Assuming you have a counter for AABBs

	uint32_t totalFaces = 0;

	for (const auto &object : m_RootObjects)
	{
		const auto &mesh = object->GetMesh();
		const auto &positions = mesh->GetPositions();
		const auto &normals = mesh->GetNormals();
		const auto &indices = mesh->GetIndices();

		glm::mat4 meshMatrix = object->GetWorldTransform();
		auto inverseMeshMatrix = glm::inverse(meshMatrix);

		m_PositionsBuffer.pushData(positions.data(), static_cast<uint32_t>(sizeof(float) * positions.size()), GL_STREAM_DRAW);
		m_NormalsBuffer.pushData(normals.data(), static_cast<uint32_t>(sizeof(float) * normals.size()), GL_STATIC_DRAW);
		m_IndicesBuffer.pushData(indices.data(), static_cast<uint32_t>(sizeof(uint32_t) * indices.size()), GL_STATIC_DRAW);

		MeshInfo meshInfo;
		meshInfo.invModelMatrix = inverseMeshMatrix; // Store the inverse model matrix for this mesh
		meshInfo.positionOffset = positionOffsetCounter;
		meshInfo.normalOffset = normalOffsetCounter;
		meshInfo.indexOffset = indexOffsetCounter;
		meshInfo.numVertices = static_cast<uint32_t>(positions.size() / 3);
		meshInfo.numIndices = static_cast<uint32_t>(indices.size());
		meshInfo.numFaces = meshInfo.numIndices / 3;
		meshInfo.bvhNodeOffset = bvhNodeOffsetCounter; // Assuming you have a counter for BVH nodes
		meshInfo.aabbOffset = aabbOffsetCounter;	   // Assuming you have a counter for AABBs

		positionOffsetCounter += meshInfo.numVertices * 3; // Assuming 3 floats per vertex
		normalOffsetCounter += meshInfo.numVertices * 3;   // Assuming 3 floats per normal
		indexOffsetCounter += meshInfo.numIndices;		   // Assuming 1 uint32_t per index
		bvhNodeOffsetCounter += meshInfo.numFaces * 2 - 1; // Assuming 2 nodes per face (for a BVH tree)
		aabbOffsetCounter += meshInfo.numFaces * 6;		   // Assuming 6 floats per AABB (min and max for x, y, z)
		totalFaces += meshInfo.numFaces;
		m_MeshInfos.push_back(meshInfo);

		m_OffsetBuffer.pushData(&meshInfo, sizeof(MeshInfo), GL_STREAM_DRAW);
	}
}

void Scene::Render()
{
	{
		PrepMeshBuffers();
	}

	{ //? BLAS
		// for (const auto &[index, mesh] : std::views::enumerate(m_MeshInfos))
		for (size_t meshIndex = 0; meshIndex < m_MeshInfos.size(); meshIndex++)
		{
			const auto &mesh = m_MeshInfos[meshIndex];
			const uint32_t index = static_cast<uint32_t>(meshIndex);

			const uint32_t aabbCount = mesh.numFaces;
			const uint32_t totalNodes = aabbCount * 2 - 1;

			//? Cretaes all the leaf nodes
			{
				glm::mat4 meshMatrix = glm::inverse(mesh.invModelMatrix);
				m_ComputeShader->Use();
				// m_ComputeShader->SetMat4("model", meshMatrix);
				m_ComputeShader->SetUnsignedInt("meshIndex", index);

				m_ComputeShader->Dispatch((totalNodes + 31) / 32, 1, 1);
			}

			//? Generates and links all internal nodes
			{
				int treeHeight = static_cast<int>(std::ceil(std::log2(aabbCount)));

				for (int level = treeHeight - 1; level >= 0; level--)
				{
					m_ComputeCalcMinMax->Use();
					m_ComputeCalcMinMax->SetInt("currentLevel", level);
					m_ComputeCalcMinMax->SetUnsignedInt("meshIndex", index);

					// Calculate the number of nodes at this level
					uint32_t levelStart = (1u << level) - 1;
					uint32_t levelEnd = std::min((1u << (level + 1)) - 2, aabbCount - 2);
					uint32_t levelSize = levelEnd - levelStart + 1;

					// Dispatch enough threads to cover all nodes at this level
					uint32_t dispatchSize = (levelSize + 31) / 32;
					m_ComputeCalcMinMax->Dispatch(dispatchSize, 1, 1);

					// Ensure all writes are visible before processing the next level
					glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
				}
			}
		}
	}

	{ //? Render objects in viewport
		for (const auto &object : m_RootObjects)
		{
			auto shader = object->GetMaterial()->GetShader();
			shader->Bind();
			shader->SetMat4("viewProj", m_EditorCamera->GetViewProjection());
			shader->SetMat4("model", object->GetWorldTransform());
			Renderer::Submit(object->GetMesh()->GetVertexArray());
		}
	}

	if (m_VisualizeBLAS)
	{ //? Visualize AABBs in viewport
		m_Shader->Bind();
		m_Shader->SetMat4("viewProj", m_EditorCamera->GetViewProjection());
		for (size_t i = 0; i < m_MeshInfos.size(); ++i)
		{
			const auto &meshInfo = m_MeshInfos[i];

			glm::mat4 meshMatrix = glm::inverse(meshInfo.invModelMatrix);

			m_Shader->SetUnsignedInt("meshIndex", static_cast<uint32_t>(i));
			m_Shader->SetMat4("model", meshMatrix);

			Renderer::SubmitInstancedWireframe(Mesh::CreateWireframeCube()->GetVertexArray(), meshInfo.numFaces);
		}
	}
}