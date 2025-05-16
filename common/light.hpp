#pragma once

#include <external/glm-0.9.7.1/glm/gtc/matrix_transform.hpp>
#include <common/model.hpp>
#include <common/camera.hpp>

struct LightSource
{
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float cosPhi;
    unsigned int type;
    bool isPlayer = false;
};

class Light
{
public:
    std::vector<LightSource> lightSources;
    unsigned int lightShaderID;
    glm::vec3 offColour = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 onColour = glm::vec3(1.0f, 1.0f, 1.0f);
    bool isOn;
    // Add lightSources
    void addPointLight(const glm::vec3 position, const glm::vec3 colour,
        const float constant, const float linear,
        const float quadratic);
    void addSpotLight( glm::vec3 position,  glm::vec3 direction,
        const glm::vec3 colour, const float constant,
        const float linear, const float quadratic,
        const float cosPhi, const bool isPlayer);
    void addDirectionalLight(const glm::vec3 direction, const glm::vec3 colour);

    // Send to shader
    void toShader(unsigned int shaderID, glm::mat4 view, Camera camera);

    // Draw light source
    void draw(unsigned int shaderID, glm::mat4 view, glm::mat4 projection, Model lightModel);

    //Move player light
    void updateLight(glm::vec3 position, glm::vec3 rotation, Camera camera, unsigned int shaderID, Model lightModel);

    void changeColour(int id);

    void changeLights();
};
