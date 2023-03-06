#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include <string>
#include <unordered_map>

/* Used for returning multiple Values */
struct ShaderSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
public:
    Shader() : m_ID(0), sourcePath(nullptr) {}

    /* Create the Shader */
    void Create(const std::string& filepath);

    /* Recreates the Shader using the Filepath used in Shader::Create() */
    void Reload();

    /* Tell OpenGL to use this Shader */
    void Bind() const;
    void Unbind() const;

    /* Deletes the Current Shader */
    void Delete();

    /* Setting Uniforms */
    void UploadMat4f(const std::string& name, const glm::mat4& mat4);

    void UploadVec4f(const std::string& name, const glm::vec4& vec4);

    void UploadVec3f(const std::string& name, const glm::vec3& vec3);

private:
    /* ID assigned by OpenGL */
    unsigned int m_ID;

    const char* sourcePath;

    /* Used for Bind() so the Shader doesn't get bound when it doesn't even exist */
    bool m_Created = false;

    /* Store allready fetched locations to increase Performance */
    std::unordered_map<std::string, int> m_UniformLocationCache;

    /* Returns the Location of a Uniform. */
    int GetUniformLocation(const std::string& name);

    /* Compiles the Shader. Errors get printed to the Console */
    static unsigned int CompileShader(unsigned int type, const std::string& source);

    /* Links all ShaderSource together */
    void LinkShaderSource(const std::string& vertexShader, const std::string& fragmentShader);

    /* retrieves the Code of the Shader from the File */
    static ShaderSource ParseShader(const std::string& filepath);

    static std::string GetFailMessage(unsigned int type);
};