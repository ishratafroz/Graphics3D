//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawbed(unsigned int VAO1, Shader ourShader, glm::mat4 isha);
void wall(unsigned int VAO, Shader ourShader, glm::mat4 isha);
void wall2(unsigned int VAO, Shader ourShader, glm::mat4 isha);
void floor(unsigned int VAO, Shader ourShader, glm::mat4 isha);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float Fan_rotateAngle_Y = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool fan_turn = false;
bool rotate_around = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 transform(float tx, float ty, float tz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    return model;
}
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cube_vertices[] = {
       -0.25f, -0.25f, -0.25f, 0.0f, 0.0f, 0.0f,
        0.25f, -0.25f, -0.25f, 0.0f, 0.0f, 0.0f,
        0.25f, 0.25f, -0.25f, 0.0f, 0.0f, 0.0f,
        -0.25f, 0.25f, -0.25f, 0.0f, 0.0f, 0.0f,
        -0.25f, -0.25f, 0.25f, 0.3f, 0.8f, 0.5f,
        0.25f, -0.25f, 0.25f, 0.5f, 0.4f, 0.3f,
        0.25f, 0.25f, 0.25f, 0.2f, 0.7f, 0.3f,
        -0.25f, 0.25f, 0.25f, 0.6f, 0.2f, 0.8f
    };

    float bed[] = {
     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 0.3f, 0.8f, 0.5f,
      0.5f, 0.5f, 0.5f, 0.5f, 0.4f, 0.3f,
      0.5f, 0.5f, -0.5f, 0.2f, 0.7f, 0.3f,
      -0.5f, 0.5f, -0.5f, 0.6f, 0.2f, 0.8f
    };
    unsigned int bed_indices[] = {
        0,1,2, 0,3,2, 0,1,5, 0,4,5, 0,3,7, 0,4,7, 2,1,5, 2,6,5, 3,2,7, 7,6,2, 4,5,6, 6,7,4,
    };

    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f
    };*/
    
    
    unsigned int cube_indices[] = {
         0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };*/
    /*float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };*/
    // world space positions of our cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/
    float axisVertices[] = {
        // X-axis
        0.0f, 0.0f, 0.0f,   // Start point
        5.0f, 0.0f, 0.0f,   // End point
        // Y-axis
        0.0f, 0.0f, 0.0f,   // Start point
        0.0f, 5.0f, 0.0f,   // End point
        // Z-axis
        0.0f, 0.0f, 0.0f,   // Start point
        0.0f, 0.0f, 5.0f    // End point
    };

    //axis line VBO,VAO
    unsigned int axisVAO, axisVBO;
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    // Bind the axis VAO and VBO
    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
   

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    unsigned int VBO1, VAO1, EBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bed), bed, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bed_indices), bed_indices, GL_STATIC_DRAW);
    
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // activate shader
        ourShader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, Fan_rotateYMatrix, bed_rotateMatrix, scaleMatrix,
            fan_scaleMatrix, bed_scaleMatrix, model, fan_model, bed_model, table_scaleMatrix;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -0.5f, translate_Z));
        drawbed(VAO1, ourShader, translateMatrix);
        wall(VAO, ourShader, translateMatrix); 
        wall(VAO, ourShader, glm::translate(identityMatrix, glm::vec3(14.7f, -0.5f,-0.2f)));
        wall2(VAO, ourShader, translateMatrix);
        floor(VAO, ourShader, translateMatrix);
        // Modelling Transformation
       
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.8f, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;


        //For fan

        fan_scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, .01f, .1f));
        Fan_rotateYMatrix = glm::rotate(identityMatrix, glm::radians(Fan_rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        fan_model = translateMatrix * rotateXMatrix * Fan_rotateYMatrix * rotateZMatrix * fan_scaleMatrix;
        ourShader.setMat4("model", fan_model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        fan_scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, .01f, 3.0f));
        fan_model = translateMatrix * rotateXMatrix * Fan_rotateYMatrix * rotateZMatrix * fan_scaleMatrix;
        ourShader.setMat4("model", fan_model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        fan_scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.30f, 0.20f, 0.30f));
        fan_model = translateMatrix * rotateXMatrix * Fan_rotateYMatrix * rotateZMatrix * fan_scaleMatrix;
        ourShader.setMat4("model", fan_model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); Fan_rotateAngle_Y += 0.1f;


        glBindVertexArray(VAO);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 1.0f, translate_Z));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.09f, 1.0f, 0.1f));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
      
        //for table
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.30f, 0.0f, 0.0f));
        table_scaleMatrix= glm::scale(translateMatrix, glm::vec3(1.0f, 0.2f, 1.0f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.4f, 0.2f, 0.0f));  
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
       
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, -0.25f, -0.20f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 1.0f, 0.2f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.6f, 0.4f, 0.2f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.1f, -0.25f, -0.20f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 1.0f, 0.2f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.1f, -0.25f, 0.2f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 1.0f, 0.2f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, -0.25f, 0.2f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 1.0f, 0.2f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //for drawer
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8f, 0.6f, -5.0f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(2.0f, 4.2f, 2.0f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);  ourShader.setVec3("color", glm::vec3(0.7f, 0.0f, 0.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.0f, 0.2f, 2.5f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);  ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8f, -0.0f, -5.0f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.0f, 0.2f, 2.5f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);  ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8f, 1.3f, -5.0f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.0f, 0.2f, 2.5f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);  ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //for chair
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.805f, -0.15f, 0.0f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.5f, 0.2f, 1.0f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);  ourShader.setVec3("color", glm::vec3(0.7f, 0.0f, 0.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.89f, -0.35f, -0.20f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 0.68f, 0.2f));
        model = table_scaleMatrix; ourShader.setMat4("model", model); ourShader.setVec3("color", glm::vec3(1.0f, 0.4f, 0.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.89f, -0.35f, 0.20f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 0.68f, 0.2f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.73f, -0.35f, 0.20f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 0.68f, 0.2f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.73f, -0.35f, -0.20f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 0.68f, 0.2f));
        model = table_scaleMatrix; ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.72f, 0.1f, -0.0f));
        table_scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 1.0f, 1.0f));
        model = table_scaleMatrix; ourShader.setMat4("model", model); ourShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
         

        //Axis line draw
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
            model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            ourShader.setMat4("model", model);

            glUseProgram(ourShader.ID);
            glUniform3f(glGetUniformLocation(ourShader.ID, "lineColor"), 1.0f, 0.0f, 0.0f);
            glBindVertexArray(axisVAO);
           // glDrawArrays(GL_LINES, 0, 2);

            // Draw the y-axis line
            glUniform3f(glGetUniformLocation(ourShader.ID, "lineColor"), 0.0f, 1.0f, 0.0f);
//            glDrawArrays(GL_LINES, 2, 2);

            // Draw the z-axis line
            glUniform3f(glGetUniformLocation(ourShader.ID, "lineColor"), 0.0f, 0.0f, 0.0f);
  //          glDrawArrays(GL_LINES, 4, 2);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
void wall(unsigned int VAO, Shader ourShader, glm::mat4 isha) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, Fan_rotateYMatrix, bed_rotateMatrix, scaleMatrix,
        fan_scaleMatrix, bed_scaleMatrix, model, fan_model, bed_model, table_scaleMatrix;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.0f, 3.43f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 13.8f, 20.0f));
    model = translateMatrix * isha * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec3("color", glm::vec3(0.8f, 0.5f, 0.2f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void wall2(unsigned int VAO, Shader ourShader, glm::mat4 isha) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, Fan_rotateYMatrix, bed_rotateMatrix, scaleMatrix,
        fan_scaleMatrix, bed_scaleMatrix, model, fan_model, bed_model, table_scaleMatrix;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5f, 3.33f, -10.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(33.7f, 13.8f, 0.5f));
    model = translateMatrix * isha * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec3("color", glm::vec3(0.8f, 0.6f, 0.2f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void floor(unsigned int VAO, Shader ourShader, glm::mat4 isha) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, Fan_rotateYMatrix, bed_rotateMatrix, scaleMatrix,
        fan_scaleMatrix, bed_scaleMatrix, model, fan_model, bed_model, table_scaleMatrix;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 0.0f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.0f, 0.1f, 20.0f));
    model = translateMatrix * isha * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec3("color", glm::vec3(0.9f, 0.7f, 0.5f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawbed(unsigned int VAO1,Shader ourShader, glm::mat4 isha) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, Fan_rotateYMatrix, bed_rotateMatrix, scaleMatrix,
        fan_scaleMatrix, bed_scaleMatrix, model, fan_model, bed_model, table_scaleMatrix;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.75f, 0.1f, 0.5f));
    // rotateXMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, .0f, 0.0f));
    bed_scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, .2f, .5f));
    bed_model = isha * translateMatrix * bed_scaleMatrix;
    ourShader.setMat4("model", bed_model);   ourShader.setVec3("color", glm::vec3(1.0f, 0.984f, 0.0f));
    glBindVertexArray(VAO1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    identityMatrix = glm::translate(identityMatrix, glm::vec3(1.25f, 0.3f, 0.5f));
    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    bed_scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.60f, 0.5f));
    //translateMatrix = glm::translate(bed_scaleMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    bed_model = isha * bed_scaleMatrix;
    ourShader.setMat4("model", bed_model); ourShader.setVec3("color", glm::vec3(0.118f, 1.0f, 0.0f));
    glBindVertexArray(VAO1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    identityMatrix = glm::translate(identityMatrix, glm::vec3(-1.0f, -0.15f, 0.0f));
    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    bed_scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.30f, 0.5f));
    //translateMatrix = glm::translate(bed_scaleMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    bed_model = isha * bed_scaleMatrix;
    ourShader.setMat4("model", bed_model);
    glBindVertexArray(VAO1);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (!fan_turn) {
            fan_turn = true;
        }
        else {
            fan_turn = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (!rotate_around) {
            rotate_around = true;
        }
        else {
            rotate_around = false;
        }
    }

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
