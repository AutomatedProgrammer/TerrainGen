#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
    public:
    Shader() {id = glCreateProgram();}

    void LoadVertexShader(std::string path);
    void LoadFragmentShader(std::string path);
    void Link();
    void Use();
    void Unload();

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetVec4(const std::string &name, const glm::vec4 &value) const;
    void SetMat2(const std::string &name, const glm::mat2 &mat) const;
    void SetMat3(const std::string &name, const glm::mat3 &mat) const;
    void SetMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
    unsigned int id = 0, vertex_shader = 0, fragment_shader = 0;
    std::string vertex_shader_source = "";
    std::string fragment_shader_source = "";
    int success;
    char log[512];

};

void Shader::LoadVertexShader(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "Unable to open vertex shader..." << std::endl;
    }
    std::string line = "";
    while (std::getline(file, line))
    {
        vertex_shader_source += line + "\n";
    }
    file.close();
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_shader_source_c = vertex_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vertex_shader_source_c, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
    }
    glAttachShader(id, vertex_shader);
}

void Shader::LoadFragmentShader(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "Unable to open fragment shader..." << std::endl;
    }
    std::string line = "";
    while (std::getline(file, line))
    {
        fragment_shader_source += line + "\n";
    }
    file.close();
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader_source_c = fragment_shader_source.c_str();
    glShaderSource(fragment_shader, 1, &fragment_shader_source_c, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log << std::endl;
    }
    glAttachShader(id, fragment_shader);
}

void Shader::Link()
{
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
    }
    if (vertex_shader) glDeleteShader(vertex_shader);
    if (fragment_shader) glDeleteShader(fragment_shader);
}

void Shader::Use()
{
    glUseProgram(id);
}

void Shader::Unload()
{
    glDeleteProgram(id);
}

void Shader::SetBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
}

void Shader::SetInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
}

void Shader::SetFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
{ 
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{ 
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{ 
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}