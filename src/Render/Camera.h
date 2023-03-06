#pragma once


#include "glm/glm.hpp"

class Camera {
public:
    Camera() : m_Position(glm::vec3(0.0f)) {}
    explicit Camera(glm::vec3 position);

    void Update(float deltaTime);

    [[nodiscard]] glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
    [[nodiscard]] glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

private:

    void UpdateProjection();

    void UpdateView();

    void HandleInput(float deltaTime);

public:
    glm::vec3 m_Position{};
    glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 m_Orientation = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_Sensitivity = 300.0f;
    float m_Speed = 2.0f;

    bool m_FirstClick = true;

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
};
