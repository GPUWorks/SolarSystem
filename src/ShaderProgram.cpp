#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
    m_ID = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteShader(m_ID);
}

void ShaderProgram::Use() const {
    glUseProgram(m_ID);
}

void ShaderProgram::AttachShaders(const char *vertex_path, const char *fragment_path, const char *geometry_path) {
    unsigned int vertex_shader = AttachShader(vertex_path, GL_VERTEX_SHADER);
    unsigned int fragment_shader = AttachShader(fragment_path, GL_FRAGMENT_SHADER);
    unsigned int geometry_shader = geometry_path != nullptr ? AttachShader(geometry_path, GL_GEOMETRY_SHADER) : -1;
    
    LinkProgram();
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if(geometry_path != nullptr) {
        glDeleteShader(geometry_shader);
    }
}

void ShaderProgram::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void ShaderProgram::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::SetVec2(const std::string &name, const glm::vec2 &vec) const {
    glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
}

void ShaderProgram::SetVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void ShaderProgram::SetVec3(const std::string &name, const glm::vec3 &vec) const {
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
}

void ShaderProgram::SetVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void ShaderProgram::SetVec4(const std::string &name, const glm::vec4 &vec) const {
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
}

void ShaderProgram::SetVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void ShaderProgram::SetMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderProgram::SetMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

int ShaderProgram::GetID() const {
    return m_ID;
}

unsigned int ShaderProgram::AttachShader(const char *path, GLenum shader_type) {
    std::string shader_code;
    std::fstream shader_file;
    
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        shader_file.open(path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        
        shader_code = shader_stream.str();
    } catch(const std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
    }
    
    unsigned int shader = glCreateShader(shader_type);
    const char *shader_code_ptr = shader_code.c_str();
    glShaderSource(shader, 1, &shader_code_ptr, nullptr);
    glCompileShader(shader);
    
    // check compile errors
    GLint success;
    GLchar info_log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR\n" << info_log << '\n';
    }
    
    glAttachShader(m_ID, shader);
    
    return shader;
}

void ShaderProgram::LinkProgram() {
    glLinkProgram(m_ID);
    
    // check linking errors
    int success;
    char info_log[1024];
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ID, 1024, nullptr, info_log);
        std::cout << "ERROR::LINKING_SHADERS_ERROR\n" << info_log << '\n';
    }
}
