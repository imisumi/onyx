#include "EditorCamera.h"

#include <glfw/glfw3.h>
#include <algorithm>
#include <glm/gtx/quaternion.hpp>

#include "input/Input.h"

EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
    : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
      m_Projection(glm::perspectiveLH(glm::radians(fov), aspectRatio, nearClip, farClip))
{
    // Initialize camera position to look from (0,5,-5) toward origin
    m_FocalPoint = glm::vec3(0.0f);
    m_Distance = 7.07f; // Approximate distance from origin to (0,5,-5)
    m_Pitch = -glm::atan(5.0f, 5.0f); // Negative pitch to look down from height of 5
    m_Yaw = 0.0f; // No left/right rotation
    UpdateView();
}

void EditorCamera::UpdateProjection()
{
    m_AspectRatio = static_cast<float>(m_ViewportWidth) / static_cast<float>(m_ViewportHeight);
    m_Projection = glm::perspectiveLH(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void EditorCamera::UpdateView()
{
    // Calculate position based on focal point, distance, and orientation
    m_Position = CalculatePosition();
    
    // Create view matrix from position and orientation
    glm::quat orientation = GetOrientation();
    glm::mat4 rotation = glm::toMat4(orientation);
    
    // First translate to position, then apply rotation
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * rotation;
    
    // Invert to get the view matrix (camera is opposite of world transform)
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

void EditorCamera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    UpdateView();
}

void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
{
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    UpdateProjection();
}

glm::quat EditorCamera::GetOrientation() const
{
    return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
}

glm::vec3 EditorCamera::GetUpDirection() const
{
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 EditorCamera::GetRightDirection() const
{
    return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 EditorCamera::GetForwardDirection() const
{
    // Left-handed system uses positive Z for forward
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::vec3 EditorCamera::CalculatePosition() const
{
    // Since we're using flipped axes, adjust how we calculate the position
    return m_FocalPoint - GetForwardDirection() * m_Distance;
}

std::pair<float, float> EditorCamera::PanSpeed() const
{
    // Calculate pan speed based on viewport dimensions with quadratic formula
    float x = std::min(m_ViewportWidth / 1000.0f, MAX_PAN_SPEED);
    float xFactor = PAN_COEF_A * (x * x) + PAN_COEF_B * x + PAN_COEF_C;

    float y = std::min(m_ViewportHeight / 1000.0f, MAX_PAN_SPEED);
    float yFactor = PAN_COEF_A * (y * y) + PAN_COEF_B * y + PAN_COEF_C;

    return {xFactor, yFactor};
}

float EditorCamera::RotationSpeed() const
{
    return ROTATION_SPEED;
}

float EditorCamera::ZoomSpeed() const
{
    float distance = m_Distance * ZOOM_FACTOR;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    return std::min(speed, MAX_ZOOM_SPEED);
}

bool EditorCamera::OnUpdate(Timestep ts)
{
    if (!Input::IsKeyPressed(Key::LEFT_ALT))
        return false;
        
    // Get current mouse position and calculate delta
    auto [mouseX, mouseY] = Input::GetMousePosition();
    const glm::vec2 mouse{mouseX, mouseY};
    glm::vec2 delta = (mouse - m_InitialMousePosition) * MOUSE_SENSITIVITY;
    m_InitialMousePosition = mouse;

    // Handle different mouse buttons for different camera operations
    if (Input::IsMouseButtonPressed(MouseButton::MIDDLE))
        MousePan(delta);
    else if (Input::IsMouseButtonPressed(MouseButton::LEFT))
        MouseRotate(delta);
    else if (Input::IsMouseButtonPressed(MouseButton::RIGHT))
        MouseZoom(delta.y);
    else
        return false;
        
    UpdateView();
    return true;
}

void EditorCamera::MousePan(const glm::vec2& delta)
{
    auto [xSpeed, ySpeed] = PanSpeed();
    // Since we flipped the X and Y axes, adjust panning directions accordingly
    m_FocalPoint += GetRightDirection() * -delta.x * xSpeed * m_Distance;
    m_FocalPoint += -GetUpDirection() * -delta.y * ySpeed * m_Distance;
}

void EditorCamera::MouseRotate(const glm::vec2& delta)
{
    float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    m_Yaw -= yawSign * delta.x * RotationSpeed();
    m_Pitch -= delta.y * RotationSpeed();
}

void EditorCamera::MouseZoom(float delta)
{
    m_Distance -= delta * ZoomSpeed();
    
    // If distance becomes too small, move focal point forward
    if (m_Distance < MIN_DISTANCE)
    {
        m_FocalPoint += GetForwardDirection();
        m_Distance = MIN_DISTANCE;
    }
}