#pragma once

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>



class LveCamera {
public:
    void setOrthographicProjection(
            float left, float right, float top, float bottom, float near, float far);
    void setPerspectiveProjection(float fovy, float aspect, float near, float far);

    void setViewDirection(
            glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setViewTarget(
            glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
    void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

    const glm::mat4& getProjection() const { return projectionMatrix; }
    const glm::mat4& getView() const { return viewMatrix; }
    void decomposeView(glm::mat4 viewMat);

public:
    glm::mat4 projectionMatrix{1.f};
    glm::mat4 viewMatrix{1.f};

    glm::vec3 scale_d;
    glm::quat rotation_d;
    glm::vec3 translation_d;
    glm::vec3 skew_d;
    glm::vec4 perspective_d;
    float std_scale = 1.f;
    bool isperspective = true;

};

