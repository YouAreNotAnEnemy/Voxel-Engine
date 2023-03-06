#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Core/Input/Input.h"

#include <iostream>

Camera::Camera(glm::vec3 position) : m_Position(position), m_ViewMatrix(), m_ProjectionMatrix() {
    Update(0);
}

void Camera::Update(float deltaTime) {
    HandleInput(deltaTime);

    UpdateProjection();
    UpdateView();
}

void Camera::UpdateProjection() {
    m_ProjectionMatrix = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100000.0f);
}

void Camera::UpdateView() {
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

void Camera::HandleInput(float deltaTime) {
    if (Input::GetKey(Key::W)) { m_Position += deltaTime * m_Speed * m_Orientation; }
    if (Input::GetKey(Key::A)) { m_Position += deltaTime * m_Speed * -glm::normalize(glm::cross(m_Orientation, m_Up)); }
    if (Input::GetKey(Key::S)) { m_Position += deltaTime * m_Speed * -m_Orientation; }
    if (Input::GetKey(Key::D)) { m_Position += deltaTime * m_Speed * glm::normalize(glm::cross(m_Orientation, m_Up)); }
    if (Input::GetKey(Key::Space)) { m_Position += deltaTime * m_Speed * m_Up; }
    if (Input::GetKey(Key::LeftShift)) { m_Position += deltaTime * m_Speed * -m_Up; }
    if (Input::GetKey(Key::LeftControl)) { m_Speed = 6.0f; } else if (Input::GotKeyReleased(Key::LeftControl)) { m_Speed = 2.0f; }

    if (Input::GetMouseButton(1)) {
        Input::HideCursor();

        auto windowSize = Input::GetWindowSize();

        int centerX = (int) windowSize.x / 2;
        int centerY = (int) windowSize.y / 2;

        /* prevents the camera from jumping on the first click */
        if (m_FirstClick) {
            Input::SetMousePosition(centerX, centerY);
            m_FirstClick = false;
        }

        auto mousePosition = Input::GetMousePosition();

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float rotX = m_Sensitivity * (float)(mousePosition.y - centerY) / windowSize.y;
        float rotY = m_Sensitivity * (float)(mousePosition.x - centerX) / windowSize.x;

        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Up)));

        // Decides whether the next vertical Orientation is legal or not
        if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
            m_Orientation = newOrientation;
        }

        m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);

        Input::SetMousePosition(centerX, centerY);
    } else if (Input::GotMouseButtonReleased(1)) {
        Input::ShowCursor();
        m_FirstClick = true;
    }
}
