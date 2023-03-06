#include <fstream>
#include <filesystem>
#include "Shader.h"

#include "Util/Logger.h"

void Shader::Create(const std::string& filepath) {
    sourcePath = filepath.c_str();

    ShaderSource source = Shader::ParseShader(filepath);
    LinkShaderSource(source.VertexSource, source.FragmentSource);
}

void Shader::Reload()
{
    Delete();
    Create(sourcePath);
}

void Shader::Bind() const {
    if (m_Created)
        glUseProgram(m_ID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::Delete()
{
    if(m_Created)
        glDeleteProgram(m_ID);

    m_ID = 0;
}

void Shader::UploadMat4f(const std::string& name, const glm::mat4& mat4) {
    Bind();
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat4[0][0]);
}

void Shader::UploadVec4f(const std::string& name, const glm::vec4& vec4) {
    Bind();
    glUniform4f(GetUniformLocation(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::UploadVec3f(const std::string& name, const glm::vec3& vec3) {
    Bind();
    glUniform3f(GetUniformLocation(name), vec3.x, vec3.y, vec3.z);
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_ID, name.c_str());
    if (location != -1)
    {
        m_UniformLocationCache[name] = location;
        return location;
    }

    LOG_WARN("Uniform '", name, "' does not exist!");
    return location;
}

void Shader::LinkShaderSource(const std::string& vertexShader, const std::string& fragmentShader) {
    m_ID = glCreateProgram();
    auto vs = Shader::CompileShader(GL_VERTEX_SHADER, vertexShader);
    auto fs = Shader::CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(m_ID, vs);
    glAttachShader(m_ID, fs);

    glLinkProgram(m_ID);
    glValidateProgram(m_ID);

    m_Created = true;

    glDeleteShader(vs);
    glDeleteShader(fs);
}

ShaderSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    if (!stream)
        LOG_ERROR("Invalid Filepath -> ", std::filesystem::path(filepath));

    std::string line;

    std::stringstream ss[2];

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") == std::string::npos)
            ss[(int)type] << line << '\n';

        else {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    auto id = glCreateShader(type);
    auto src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result != GL_FALSE) return id;

    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    char* message = (char*)_malloca(length * sizeof(char));

    glGetShaderInfoLog(id, length, &length, message);

    LOG_ERROR(GetFailMessage(type));
    LOG_ERROR(message);

    glDeleteShader(id);

    return 0;
}

std::string Shader::GetFailMessage(unsigned int type) {
    std::stringstream message;
    message << "Failed to compile '" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "' Shader!";
    return message.str();
}