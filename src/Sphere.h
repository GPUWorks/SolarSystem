#pragma once

#include <vector>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "Time.h"

enum class Space {
    Local = 0,
    World
};

class Sphere {
public:
    Sphere();
    ~Sphere();
    
    /// This function should be called before any drawing related calls
    static void Init();
    
    void Draw(const ShaderProgram& shader_program) const;
    void Move(const glm::vec3& vector, Space space);
    void Rotate(const glm::vec3& rotation, Space space);
    
    const glm::mat4& Model() const { return m_Model; }
    
    const glm::vec3& Position() const { return m_Position; }
    void Position(const glm::vec3& position);
    
    const glm::vec3& Rotation() const { return m_Rotation; }
    void Rotation(const glm::vec3& rotation);
    
    const glm::vec3& Scale() const { return m_Scale; }
    void Scale(const glm::vec3& scale);
    
    const glm::vec4 Color() const { return m_Color; }
    void Color(const glm::vec4& color) { m_Color = color; }
    
private:
    glm::mat4 m_Model;
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
    
    glm::vec4 m_Color;
    
    static unsigned int m_VBO;
    static unsigned int m_VAO;
    static unsigned int m_EBO;
    static unsigned int m_VerticesCount;
    
    void UpdateModel();
};
