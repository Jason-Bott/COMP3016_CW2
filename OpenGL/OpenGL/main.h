#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//Called on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Processes user input on a particular window
void ProcessUserInput(GLFWwindow* WindowIn);
void ProcessMouseInput(GLFWwindow* window, float deltaTime);

//Sets the model-view-projection matrix
void SetMatrices(Shader& ShaderProgramIn);
GLuint LoadCubemap(std::vector<std::string> faces);

GLuint program;