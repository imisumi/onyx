#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "core/Timestep.h"

class EditorCamera
{
public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

    bool OnUpdate(Timestep ts);
    // bool OnEvent(Event &e);

    // Getters
    float GetDistance() const { return m_Distance; }
    const glm::vec3& GetPosition() const { return m_Position; }
    float GetPitch() const { return m_Pitch; }
    float GetYaw() const { return m_Yaw; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
    glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
    const glm::mat4& GetProjection() const { return m_Projection; }
    glm::quat GetOrientation() const;
    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;

    // Setters
    void SetDistance(float distance) { m_Distance = distance; }
    void SetPosition(const glm::vec3& position);
    void SetViewportSize(uint32_t width, uint32_t height);

private:
    // Constants
    static constexpr float MAX_PAN_SPEED = 2.4f;
    static constexpr float PAN_COEF_A = 0.0366f;
    static constexpr float PAN_COEF_B = -0.1778f;
    static constexpr float PAN_COEF_C = 0.3021f;
    static constexpr float ROTATION_SPEED = 0.8f;
    static constexpr float ZOOM_FACTOR = 0.2f;
    static constexpr float MAX_ZOOM_SPEED = 100.0f;
    static constexpr float MIN_DISTANCE = 1.0f;
    static constexpr float MOUSE_SENSITIVITY = 0.003f;

    // Core methods
    void UpdateProjection();
    void UpdateView();
    
    // Mouse interaction
    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    void MouseZoom(float delta);
    
    // Helper methods
    glm::vec3 CalculatePosition() const;
    std::pair<float, float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

    // Camera properties
    float m_FOV = 45.0f;
    float m_AspectRatio = 1.778f;
    float m_NearClip = 0.1f;
    float m_FarClip = 1000.0f;
    float m_Distance = 10.0f;
    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;
    uint32_t m_ViewportWidth = 1280;
    uint32_t m_ViewportHeight = 720;

    // Matrices and vectors
    glm::mat4 m_ViewMatrix{1.0f};
    glm::mat4 m_Projection{1.0f};
    glm::vec3 m_Position{5.0f, 5.0f, 0.0f};
    glm::vec3 m_FocalPoint{0.0f, 0.0f, 0.0f};
    glm::vec2 m_InitialMousePosition{0.0f, 0.0f};
};