#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>

#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/Mesh.h"

#include "Libs/stb_image.h"

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

float yaw = -90.0f;
float pitch = 0.0f;


//Vertex Shader
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateTriangle()
{
    GLfloat vertices[] =
    {
        //pos                   //TexCoord
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        0.0, -1.0f, 1.0f,       0.5f, 0.0f,
        1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.0f
    };

    unsigned int indices[] = 
    {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 20, 12);
    for (int i = 0; i < 10; i++)
    {
    meshList.push_back(obj1);
    }
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
} 

void checkMouse(){
    double mouseX,mouseY;
    glfwGetCursorPos(mainWindow.getWindow(), &mouseX, &mouseY);

    static float lastX = mouseX;
    static float lastY = mouseY;

    float xOffset = mouseX - lastX;
    float yOffset = lastY - mouseY;

    lastX = mouseX;
    lastY = mouseY;
 
    float sensitivity = 0.1f;
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;
    
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}
void CreateOBJ(){
    Mesh *obj1 = new Mesh();
    bool loaded = obj1->CreateMeshFromOBJ("Models/suzanne.obj");
    if (loaded){
        for (int i = 0; i < 10; i++){
            meshList.push_back(obj1);
        }
    }
    else{
        std::cout<<"Failed to load model"<<std::endl;
    }
}

int main()
{
    // 1) Create & initialize window/context first
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();
    //CreateTriangle();
    CreateOBJ();
    CreateShaders();
    
    GLuint uniformModel = 0, uniformProjection = 0,uniformView = 0;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
    
    glm::mat4 view = glm::mat4 (1.0f);
    glm::vec3 cameraPos = glm::vec3(0.0f, -0.3f,-1.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos );
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
    glm::vec3 cameraUp = glm::cross(cameraRight, cameraDirection);
    glm::vec3 lightColour = glm::vec3(0.0f, 1.0f, 1.0f); // สีฟ้า (cyan)

    /*
    glm::mat4 cameraPosMat (1.0f);
    cameraPosMat[3][0] = -cameraPos.x;
    cameraPosMat[3][1] = -cameraPos.y;
    cameraPosMat[3][2] = -cameraPos.z;
    glm::mat4 cameraRotateMat(1.0f);
    cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, -cameraDirection.x, 0.0f);
    cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, -cameraDirection.y, 0.0f);
    cameraRotateMat[2] = glm::vec4(cameraRight.z, cameraUp.z, -cameraDirection.z, 0.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraDiection, cameraUp);
    */
    
    // (Optional, harmless for triangle)
    glEnable(GL_DEPTH_TEST);
    
    view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
    
    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width1, height1, nrChannels1;
    unsigned char* data1 = stbi_load("Textures/uvmap.png", &width1, &height1, &nrChannels1, 0);

    if (!data1)
    {
        std::cerr << "Failed to load texture: " << stbi_failure_reason() << "\n";
    }
    else
    {
    // Handle 1/3/4 channel images correctly
    GLenum format = GL_RGB;
    if (nrChannels1 == 1) format = GL_RED;
    else if (nrChannels1 == 3) format = GL_RGB;
    else if (nrChannels1 == 4) format = GL_RGBA;
    // Avoid row-alignment issues for non-4-byte widths
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width1, height1, 0, format,GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data1);
    }

   

    while (!mainWindow.getShouldClose())
    {
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        // Handle input/events first (order doesn’t matter much, but this is common)
        glfwPollEvents();   
        checkMouse();

        glm::vec3 direction;
        direction.x=cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y=sin(glm::radians(pitch));
        direction.z=sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraDirection = glm::normalize(direction);

        cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        cameraUp = glm::cross(cameraRight, cameraDirection);

        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_W) == GLFW_PRESS){
            cameraPos += cameraDirection * deltaTime * 5.0f;
        }
         if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_S) == GLFW_PRESS){
            cameraPos -= cameraDirection * deltaTime * 5.0f;
        }
         if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_A) == GLFW_PRESS){
            cameraPos -= cameraRight * deltaTime * 5.0f;
        }
         if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_D) == GLFW_PRESS){
            cameraPos += cameraRight * deltaTime * 5.0f;
        }
        view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::vec3 pyramidPositions[] ={
        glm::vec3(0.0f, 0.0f, -2.5f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
            };

        // Use shader and fetch uniform locations 
        shaderList[0]->UseShader();


        uniformModel      = shaderList[0]->GetUniformLocation("model");
        uniformProjection = shaderList[0]->GetUniformLocation("projection");
        uniformView       = shaderList[0]->GetUniformLocation("view"); //Lab8 Upgrade

        /*
        glm::mat4 view (1.0f);
        glm::vec3 cameraPos = glm::vec3(0.0f, -0.3f,-1.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
        glm::mat4 cameraPosMat (1.0f);
        cameraPosMat[3][0] = -cameraPos.x;
        cameraPosMat[3][1] = -cameraPos.y;
        cameraPosMat[3][2] = -cameraPos.z;
        glm::mat4 cameraRotateMat(1.0f);
        cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, -cameraDirection.x, 0.0f);
        cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, -cameraDirection.y, 0.0f);
        cameraRotateMat[2] = glm::vec4(cameraRight.z, cameraUp.z, -cameraDirection.z, 0.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraDiection, cameraUp);
        */
        //Object
        for (int i = 0; i < 10; i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, pyramidPositions[i]);
            model = glm::scale(model, glm::vec3(0.8f, 0.8f, 1.0f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));

            // 💡 ส่งค่าสีแสงเข้าไปใน shader
            glUniform3fv(shaderList[0]->GetUniformLocation("lightColour"), 1, glm::value_ptr(lightColour));

            GLuint uniformTexture1 = shaderList[0]->GetUniformLocation("Texture1");
            glUniform1i(uniformTexture1, 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            meshList[i]->RenderMesh();
        }


        glUseProgram(0);
        //end draw
        mainWindow.swapBuffers();
    }
    return 0;
}