#pragma once

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

//Quaternion Class
class Quaternion {
public:
    float w, x, y, z;

    //Constructors
    Quaternion();
    Quaternion(const float w, const float x, const float y, const float z);
    Quaternion(const float pitch, const float yaw);

    glm::mat4 matrix();
};

// Maths class
class Maths
{
public:
    // Transformation matrices
    static glm::mat4 translate(const glm::vec3& v);
    static glm::mat4 scale(const glm::vec3& v);
    static float radians(float angle);
    static glm::mat4 rotate(const float& angle, glm::vec3 v);
    static Quaternion SLERP(const Quaternion q1, const Quaternion q2, const float t);

    static glm::mat4 LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 worldUp);
    static glm::mat4 Perspective(float fov, float aspect, float near, float far);

    static glm::vec3 Cross(glm::vec3 a, glm::vec3 b);

    static float Dot(glm::vec3 a, glm::vec3 b);

    static float Length(glm::vec3 a);

    static glm::vec3 Normalize(glm::vec3 a);
};

