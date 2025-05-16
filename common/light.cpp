#include <common/light.hpp>

void Light::addPointLight(const glm::vec3 position, const glm::vec3 colour,
    const float constant, const float linear,
    const float quadratic)
{
    LightSource light;
    light.position = position;
    light.colour = colour;
    light.constant = constant;
    light.linear = linear;
    light.quadratic = quadratic;
    light.type = 1;
    lightSources.push_back(light);
}

void Light::addSpotLight( glm::vec3 position,  glm::vec3 direction,
    const glm::vec3 colour, const float constant,
    const float linear, const float quadratic,
    const float cosPhi, bool isPlayer)
{
    LightSource light;
    light.position = position;
    light.direction = direction;
    light.colour = colour;
    light.constant = constant;
    light.linear = linear;
    light.quadratic = quadratic;
    light.cosPhi = cosPhi;
    light.type = 2;
    light.isPlayer = isPlayer;
    lightSources.push_back(light);
}

void Light::addDirectionalLight(const glm::vec3 direction, const glm::vec3 colour)
{
    LightSource light;
    light.direction = direction;
    light.colour = colour;
    light.type = 3;
    lightSources.push_back(light);
}

void Light::toShader(unsigned int shaderID, glm::mat4 view, Camera camera)
{
    unsigned int numLights = static_cast<unsigned int>(lightSources.size());
    glUniform1i(glGetUniformLocation(shaderID, "numLights"), numLights);

    for (unsigned int i = 0; i < numLights; i++)
    {
        std::string idx = std::to_string(i);
        if (lightSources[i].isPlayer) {
            lightSources[i].position = camera.eye;
            lightSources[i].direction = camera.front;
            if (isOn) 
                lightSources[i].colour = onColour;
            else
                lightSources[i].colour = offColour;
        }

        glm::vec3 VSLightPosition = glm::vec3(view * glm::vec4(lightSources[i].position, 1.0f));
        glm::vec3 VSLightDirection = glm::vec3(view * glm::vec4(lightSources[i].direction, 0.0f));
        glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].position").c_str()), 1, &VSLightPosition[0]);
        glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].direction").c_str()), 1, &VSLightDirection[0]);
        glUniform3fv(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].colour").c_str()), 1, &lightSources[i].colour[0]);
        glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].constant").c_str()), lightSources[i].constant);
        glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].linear").c_str()), lightSources[i].linear);
        glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].quadratic").c_str()), lightSources[i].quadratic);
        glUniform1f(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].cosPhi").c_str()), lightSources[i].cosPhi);
        glUniform1i(glGetUniformLocation(shaderID, ("lightSources[" + idx + "].type").c_str()), lightSources[i].type);
    }
}

void Light::draw(unsigned int shaderID, glm::mat4 view, glm::mat4 projection, Model lightModel)
{
    glUseProgram(shaderID);
    for (unsigned int i = 0; i < static_cast<unsigned int>(lightSources.size()); i++)
    {
        // Ignore directional lights
        if (lightSources[i].type == 3)
            continue;

        // Calculate model matrix
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), lightSources[i].position);
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        glm::mat4 model = translate * scale;

        // Send the MVP and MV matrices to the vertex shader
        glm::mat4 MVP = projection * view * model;
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

        // Send model, view, projection matrices and light colour to light shader
        glUniform3fv(glGetUniformLocation(shaderID, "lightColour"), 1, &lightSources[i].colour[0]);

        // Draw light source
        lightModel.draw(shaderID);
    }
}

void Light::updateLight(glm::vec3 position, glm::vec3 rotation, Camera camera, unsigned int shaderID, Model lightModel)
{
    for each(LightSource lightSource in lightSources)
    {
        if (lightSource.isPlayer) {
            lightSource.position = position;
            lightSource.direction = rotation;
          /*  std::cout << "In procedure";
            std::cout << lightSource.position;
            std::cout << "\n";*/
        }
    }
}

void Light::changeColour(int id)
{
    lightSources[id + 1].colour = glm::vec3(5.0, 1.0, 1.0);
}

void Light::changeLights()
{
    if (lightSources[9].colour.x == 100.0f) {
        lightSources[9].colour.x = 1.0f;
        lightSources[9].colour.z = 100.0f;
    }else if (lightSources[9].colour.z == 100.0f) {
        lightSources[9].colour.x = 100.0f;
        lightSources[9].colour.z = 1.0f;
    }
}

void Light::nearLights(int id)
{
    lightSources[id + 1].colour = glm::vec3(1.0, 1.0, 0.0);
}

void Light::awayLights(int id)
{
    lightSources[id + 1].colour = glm::vec3(1.0, 1.0, 1.0);
}



