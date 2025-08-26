#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <noise/noise.h>

#include <iostream>
#include <vector>
#include <bits/stdc++.h>

#include "Utilities/Shader.h"
#include "Utilities/Camera.h"
#include "Utilities/Mesh.h"

void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);

const int HEIGHT = 800, WIDTH = 600;
float delta_time = 0.0f, last_frame = 0.0f;

Camera camera(HEIGHT, WIDTH, glm::vec3(2.5f, 30.0f, 2.5f));

int main()
{ 
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Terrain Generator", NULL, NULL);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (!window)
    {
        std::cout << "Failed to create GLFW window..." << std::endl;
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);
    std::srand(std::time(0));
    int seed = 0;
    seed = std::rand();
    //std::cout << "Please enter a seed (Integers Only):";
    //std::cin >> seed;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, MouseCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    //Terrain Gen
    noise::module::Perlin noise;
    noise::module::ScaleBias flat;
    noise.SetFrequency(2.0f);
    //noise.SetPersistence(0.4f);
    //noise.SetOctaveCount(6);
    flat.SetSourceModule(0, noise);
    flat.SetScale(0.1);
    flat.SetBias(0.35);
    std::vector<Vertex> terrain_vertices;
    std::vector<unsigned int> terrain_indices;
    std::vector<Texture> terrain_textures;
    int size = 1000;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            Vertex vertex;
            float x = (float)i / (float)size;
            //float y = 0;
            float y = (float)flat.GetValue((float)i / (float)size, 0, (float)j / (float)size);
            float z = (float)j / (float)size;
            vertex.Position.x = x;
            vertex.Position.y = y;
            vertex.Position.z = z;
            vertex.TexCoords.x = x;
            vertex.TexCoords.y = y;
            terrain_vertices.push_back(vertex);
        }
    }
    
    for (int i = 0; i < size-1; i++)
    {
        for (int j = 0; j < size-1; j++)
        {
            unsigned int v = i*size+j;

            terrain_indices.push_back(v);
            terrain_indices.push_back(v+1);
            terrain_indices.push_back(v+size);

            terrain_indices.push_back(v+1);
            terrain_indices.push_back(v+1+size);
            terrain_indices.push_back(v+size);
        }
    }

    Shader terrain_shader;
    terrain_shader.LoadVertexShader("terrain_shader.vert");
    terrain_shader.LoadFragmentShader("terrain_shader.frag");
    terrain_shader.Link();
    Mesh terrain(terrain_vertices, terrain_indices, terrain_textures);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        float currentFrame = glfwGetTime();
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;
        ProcessInput(window);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        terrain_shader.Use();
        camera.Update();
        terrain_shader.SetMat4("view", camera.GetView());
        terrain_shader.SetMat4("projection", camera.GetProjection());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(50.0f));
        terrain_shader.SetMat4("model", model);

        terrain.Draw(terrain_shader);

        glfwSwapBuffers(window);


        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 10.0f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.MoveCamera(cameraSpeed, W);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.MoveCamera(cameraSpeed, S);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.MoveCamera(cameraSpeed, A);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.MoveCamera(cameraSpeed, D);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.MoveCamera(cameraSpeed, Q);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.MoveCamera(cameraSpeed, Z);
}

void MouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    camera.Rotate(x_pos, y_pos);
}