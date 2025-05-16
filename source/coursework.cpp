#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>

// Function prototypes
void keyboardInput(GLFWwindow* window, Light& lightSource);
void mouseInput(GLFWwindow* window);
bool containsName(int id);
bool hasAllStolen();

// Frame timers
float previousTime = 0.0f;  // time of previous iteration of the loop
float deltaTime = 0.0f;  // time elapsed since the previous frame

// Create camera object
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f));

bool isJumping, goDown, isTurning, changeColour;
float t, timer, colourTimer;
int count;
std::string names[9];
bool stolen[9];
bool alreadySet = false;
// Object struct
struct Object
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float angle = 0.0f;
    std::string name;
};
std::vector<Object> objects;
int main( void )
{
    // =========================================================================
   // Window creation - you shouldn't need to change this code
   // -------------------------------------------------------------------------
   // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework", NULL, NULL);

    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Use back face culling
    glEnable(GL_CULL_FACE);

    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Capture mouse inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Compile shader program
    unsigned int shaderID, lightShaderID;
    shaderID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
    lightShaderID = LoadShaders("lightVertexShader.glsl", "lightFragmentShader.glsl");

    // Activate shader
    glUseProgram(shaderID);

    // Load models

    Model cube("../assets/cube.obj");
    bool claimedSprite[8];
    std::vector<Model> cubes;
    for (int i = 0; i < 8; i++) {
        cubes.push_back(cube);
        claimedSprite[i] = false;
        stolen[i] = false;
    }
  
    Model sphere("../assets/sphere.obj");
    Model wall("../assets/plane.obj");
    Model wallFlipped("../assets/plane.obj");
    Model floor("../assets/plane.obj");
    Model display("../assets/cube.obj");

    

    for (int i = 0; i < 8; i++) {
        bool completed = false;
        while (!completed) {
            int randomNum = rand() % 8;
            if (randomNum == 0 && !claimedSprite[0]) {
                cubes[i].addTexture("../assets/Grey.bmp", "diffuse");
                claimedSprite[0] = true;
                names[i] = "grey";
                completed = true;
            }
            if (randomNum == 1 && !claimedSprite[1]) {
                cubes[i].addTexture("../assets/blue.bmp", "diffuse");
                claimedSprite[1] = true;
                names[i] = "blue";
                completed = true;
            }
            if (randomNum == 2 && !claimedSprite[2]) {
                cubes[i].addTexture("../assets/crate.jpg", "diffuse");
                claimedSprite[2] = true;
                names[i] = "crate";
                completed = true;
            }
            if (randomNum == 3 && !claimedSprite[3]) {
                cubes[i].addTexture("../assets/kratos.png", "diffuse");
                claimedSprite[3] = true;
                names[i] = "kratos";
                completed = true;
            }
            if (randomNum == 4 && !claimedSprite[4]) {
                cubes[i].addTexture("../assets/mario.png", "diffuse");
                claimedSprite[4] = true;
                names[i] = "mario";
                completed = true;
            }
            if (randomNum == 5 && !claimedSprite[5]) {
                cubes[i].addTexture("../assets/Green.bmp", "diffuse");
                claimedSprite[5] = true;
                names[i] = "green";
                completed = true;
            }
            if (randomNum == 6 && !claimedSprite[6]) {
                cubes[i].addTexture("../assets/Red.bmp", "diffuse");
                claimedSprite[6] = true;
                names[i] = "red";
                completed = true;
            }
            if (randomNum == 7 && !claimedSprite[7]) {
                cubes[i].addTexture("../assets/Yellow.bmp", "diffuse");
                claimedSprite[7] = true;
                names[i] = "yellow";
                completed = true;
            }
            if (randomNum == 8 && !claimedSprite[8]) {
                cubes[i].addTexture("../assets/Purple.bmp", "diffuse");
                claimedSprite[8] = true;
                names[i] = "purple";
                completed = true;
            }

        }
        cubes[i].addTexture("../assets/neutral_normal.png", "normal");
        cubes[i].addTexture("../assets/neutral_specular.png", "specular");
    }

    //// Load the textures
    //cube.addTexture("../assets/Grey.bmp", "diffuse");
    //cube.addTexture("../assets/neutral_normal.png", "normal");
    //cube.addTexture("../assets/neutral_specular.png", "specular");

    display.addTexture("../assets/Grey.bmp", "diffuse");
    display.addTexture("../assets/neutral_normal.png", "normal");
    display.addTexture("../assets/neutral_specular.png", "specular");

    // Load the textures
    wall.addTexture("../assets/bricks_diffuse.png", "diffuse");
    wall.addTexture("../assets/bricks_normal.png", "normal");
    wall.addTexture("../assets/bricks_specular.png", "specular");

    wallFlipped.addTexture("../assets/bricks_diffuse - Copy.png", "diffuse");
    wallFlipped.addTexture("../assets/bricks_normal - Copy.png", "normal");
    wallFlipped.addTexture("../assets/bricks_specular - Copy.png", "specular");

    floor.addTexture("../assets/stones_diffuse.png", "diffuse");
    floor.addTexture("../assets/stones_normal.png", "normal");
    floor.addTexture("../assets/stones_specular.png", "specular");

    // Use wireframe rendering (comment out to turn off)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Define wall object lighting properties
    wall.ka = 0.2f;
    wall.kd = 0.7f;
    wall.ks = 1.0f;
    wall.Ns = 20.0f;

    // Define wall object lighting properties
    wallFlipped.ka = 0.2f;
    wallFlipped.kd = 0.7f;
    wallFlipped.ks = 1.0f;
    wallFlipped.Ns = 20.0f;

    // Define floor object lighting properties
    floor.ka = 0.2f;
    floor.kd = 0.7f;
    floor.ks = 1.0f;
    floor.Ns = 20.0f;


    // Define display object lighting properties
    display.ka = 0.2f;
    display.kd = 0.7f;
    display.ks = 1.0f;
    display.Ns = 20.0f;

    // Define cube object lighting properties
    //cube.ka = 0.2f;
    //cube.kd = 0.7f;
    //cube.ks = 0.0f;
    //cube.Ns = 20.0f;

    for (int i = 0; i < 8; i++) {
        cubes[i].ka = 0.2f;
        cubes[i].kd = 0.7f;
        cubes[i].ks = 0.0f;
        cubes[i].Ns = 20.0f;
    }

    //Define light source properties

   // Add light sources
    Light lightSources;
    //lightSources.addDirectionalLight(glm::vec3(1.0f, -1.0f, 0.0f),  // direction
    //    glm::vec3(1.0f, 1.0f, 0.0f));  // colour


    lightSources.addSpotLight(glm::vec3(1.0f, 2.0f, 0.0f),          // position
        glm::vec3(0.0f, 0.0f, -1.0f),         // direction
        glm::vec3(1.0f, 1.0f, 1.0f),          // colour
        1.0f, 0.1f, 0.02f,                    // attenuation
        std::cos(Maths::radians(23.5f)), true);     // cos(phi)

    glm::vec3 lightPositions[] = {
        glm::vec3(0.0f,1.0f,-0.5f),
        glm::vec3(0.0f,1.0f,1.0f),
        glm::vec3(0.0f,1.0f,-2.0f),
        glm::vec3(4.3f,1.0f,0.0f),
        glm::vec3(4.3f,1.0f,2.0f),
        glm::vec3(4.3f,1.0f,-2.0f),
        glm::vec3(-4.3f,1.0f,0.0f),
        glm::vec3(-4.3f,1.0f,2.0f),
        glm::vec3(-4.3f,1.0f,-2.0f)
    };

    for (int i = 0; i < 8; i++) {
        lightSources.addSpotLight(lightPositions[i],          // position
            glm::vec3(0.0f, -1.0f, 0.0f),         // direction
            glm::vec3(1.0f, 1.0f, 1.0f),          // colour
            1.0f, 0.1f, 0.02f,                    // attenuation
            std::cos(Maths::radians(20.0f)), false);     // cos(phi)
    }

    // Cube positions
    glm::vec3 positions[] = {
     glm::vec3(0.0f,0.0f,-0.5f),
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.0f,0.0f,-2.0f),
        glm::vec3(4.3f,0.0f,0.0f),
        glm::vec3(4.3f,0.0f,2.0f),
        glm::vec3(4.3f,0.0f,-2.0f),
        glm::vec3(-4.3f,0.0f,0.0f),
        glm::vec3(-4.3f,0.0f,2.0f),
        glm::vec3(-4.3f,0.0f,-2.0f)
    };

    //Wall/Floor Positions, Rotations and Angle
    glm::vec3 wallPositions[] = {
        glm::vec3(0.0f, -0.85f, 0.0f),
        glm::vec3(0.0f, 0.0f, -5.0f),
        glm::vec3(-5.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(5.0f, 0.0f, 0.0f)
    };
    glm::vec3 wallRotation[] = {
     glm::vec3(1.0f, 1.0f, 1.0f),
     glm::vec3(1.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, 1.0f, 0.0f),
     glm::vec3(1.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, -1.0f, 0.0f)
    };
    float wallAngles[] = {
        0.0f,
        90.0f,
        180.0f,
        -90.0f,
        -180.0f
    };

    //Display positions and scale
    glm::vec3 displayPositions[] = {
        glm::vec3(0.0f,-0.85f,-0.5f),
        glm::vec3(4.3f,-0.85f,0.0f),
        glm::vec3(-4.3f,-0.85f,0.0f),
        glm::vec3(0.0f,-0.85f,4.3f)
    };
    glm::vec3 displayScales[] = {
      glm::vec3(0.5f, 0.5f, 2.5f),
      glm::vec3(0.5f, 0.5f, 3.0f),
      glm::vec3(0.5f, 0.5f, 3.0f),
      glm::vec3(3.0f, 0.5f, 0.5f)
    };

    // Add cubes to objects vector
  
    Object object;

    for (unsigned int i = 0; i < 8; i++)
    {
        object.name = names[i];
        object.position = positions[i];
        object.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
        object.scale = glm::vec3(0.1f, 0.1f, 0.1f);
        object.angle = Maths::radians(26.0f * (i + 1));
        objects.push_back(object);
    }

    object.name = "floor";
    object.position = wallPositions[0];
    object.rotation = wallRotation[0];
    object.scale = glm::vec3(0.5f, 0.5f, 0.5f);
    object.angle = wallAngles[0];
    objects.push_back(object);
   
    bool isFlipped = false;
    for (unsigned int i = 1; i <= wallPositions->length() + 1; i++) {
        if (isFlipped) {
            isFlipped = false;
            object.name = "wallFlipped";
        }
        else {
            isFlipped = true;
            object.name = "wall";
        }
        
        object.position = wallPositions[i];
        object.rotation = wallRotation[i];
        object.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        object.angle = Maths::radians(wallAngles[i]);
        objects.push_back(object);
   
    }
    object.name = "display";
    for (int i = 0; i <= displayPositions->length(); i++) {
        object.position = displayPositions[i];
        object.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
        object.scale = displayScales[i];
        object.angle = wallAngles[0];
        objects.push_back(object);
    }
    //Testing Math functions
    std::cout << "Libary: ";
    std::cout << glm::cross(glm::vec3(12,15,3), glm::vec3(4,9,2));
    std::cout << "\n";
    std::cout << "My code: ";
    std::cout << Maths::Cross(glm::vec3(12, 15, 3), glm::vec3(4, 9, 2));
    std::cout << "\n";

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Update timer
        float time = glfwGetTime();
        deltaTime = time - previousTime;
        previousTime = time;


        // Get inputs
        keyboardInput(window, lightSources);
        mouseInput(window);

        // Clear the window
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate view and projection matrices
        camera.target = camera.eye + camera.front;
        camera.quaternionCamera();
        camera.calculateMatrices();

        // Activate shader
        glUseProgram(shaderID);

        // Send light source properties to the shader
        lightSources.toShader(shaderID, camera.view, camera);


        // Send view matrix to the shader
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "V"), 1, GL_FALSE, &camera.view[0][0]);

        // Loop through objects
        for (unsigned int i = 0; i < static_cast<unsigned int>(objects.size()); i++)
        {
            // Calculate model matrix
            glm::mat4 translate = Maths::translate(objects[i].position);
            glm::mat4 scale = Maths::scale(objects[i].scale);
            glm::mat4 rotate = Maths::rotate(objects[i].angle, objects[i].rotation);
            glm::mat4 model = translate * rotate * scale;

            // Send the MVP and MV matrices to the vertex shader
            glm::mat4 MV = camera.view * model;
            glm::mat4 MVP = camera.projection * MV;
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "MV"), 1, GL_FALSE, &MV[0][0]);

            // Draw the model
            if (objects[i].name == "cube")
                cube.draw(shaderID);
            if (objects[i].name == "wall")
                wall.draw(shaderID);
            if (objects[i].name == "wallFlipped")
                wallFlipped.draw(shaderID);
            if (objects[i].name == "floor")
                floor.draw(shaderID);
            if (objects[i].name == "display")
                display.draw(shaderID);
            for (int k = 0; k < 8; k++) {
                if (objects[i].name == names[k]) {
                    cubes[k].draw(shaderID);
                }
            }
       //Collision System (Needs fixed!)
        
            if(containsName(i)){
            glm::vec3 distance = objects[i].position - camera.eye;
            //float value = pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2);
            float value = ((objects[i].position.x - camera.eye.x) * (objects[i].position.x - camera.eye.x)) +
                ((objects[i].position.y - camera.eye.y) * (objects[i].position.y - camera.eye.y)) + 
                ((objects[i].position.z - camera.eye.z) * (objects[i].position.z - camera.eye.z));

            float result = sqrt(value);

               if (result < 1) {
                   objects[i].position.x = 100;
                   std::string tempName = objects[i].name;
                   int index;
                   for (int k = 0; k < 8; k++) {
                       if (tempName == names[k]) {
                           index = k;
                       }
                   }          
                   lightSources.changeColour(index);
                   stolen[index] = true;
               }
            }

           
       
        }
        // Draw light sources
        lightSources.updateLight(camera.eye, camera.front, camera, lightShaderID, sphere);
        lightSources.draw(lightShaderID, camera.view, camera.projection, sphere);
      
        for (int i = 0; i < objects.size(); i++) {
            if (containsName(i)) {
               // objects[i].rotation = glm::vec3(objects[i].rotation.x + 1.0f , objects[i].rotation.y, objects[i].rotation.z);
                objects[i].angle = objects[i].angle + 0.05f;
            }
            if (objects[i].angle > 360) {
                objects[i].angle = 0;
            }
        }

        if (hasAllStolen()) {
            if (!alreadySet) {
                alreadySet = true;
               lightSources.addDirectionalLight(glm::vec3(1.0f, -1.0f, 0.0f),  // direction
                                                glm::vec3(100.0f, 1.0f, 1.0f));  // colour
            }
            float colourTime = glfwGetTime();
            if (!changeColour) {
                lightSources.changeLights();
                changeColour = true;
                colourTimer = colourTime;
            }
            if (changeColour) {
                if (colourTime - colourTimer > 0.5f)
                    changeColour = false;
            }
          
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    cube.deleteBuffers();
    wall.deleteBuffers();
    wallFlipped.deleteBuffers();
    floor.deleteBuffers();
    glDeleteProgram(shaderID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

bool hasAllStolen() {
    for (int i = 0; i < 8; i++) {
        if (!stolen[i])
            return false;
    }
    return true;
}

bool containsName(int id) {
    for (int i = 0; i < 8; i++) {
        if (objects[id].name == names[i]) {
            return true;
        }
    }
    return false;
}
void keyboardInput(GLFWwindow* window, Light& lightSources)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    camera.eye.y = 0;
    glm::vec3 front = glm::vec3(camera.front.x, 0, camera.front.z);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * front;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.eye -= 5.0f * deltaTime * camera.right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.eye += 5.0f * deltaTime * camera.right;

    float lightTime = glfwGetTime();
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !lightSources.isOn && !isTurning) {
        lightSources.isOn = true;
        isTurning = true;
        timer = lightTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && lightSources.isOn && !isTurning) {
        lightSources.isOn = false;
        isTurning = true;
        timer = lightTime;
    }
    if (isTurning) {
        if (lightTime - timer > 0.2f)
            isTurning = false;
    }
    
    float time = glfwGetTime();
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping)
    {
        isJumping = true;
        t = time;
    }

    if (isJumping) {
        if (time - t > 1.0f)
            isJumping = false;

        camera.eye.y = 1.0f * sin(3.1415f * (time - t));

    }
  

}

void mouseInput(GLFWwindow* window)
{
    // Get mouse cursor position and reset to centre
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Update yaw and pitch angles
    camera.yaw += 0.005f * float(xPos - 1024 / 2);
    camera.pitch += 0.005f * float(768 / 2 - yPos);

    // Calculate camera vectors from the yaw and pitch angles
    camera.calculateCameraVectors();
}

