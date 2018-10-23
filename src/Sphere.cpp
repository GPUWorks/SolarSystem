#include "Sphere.h"

unsigned int Sphere::m_VAO = 0;
unsigned int Sphere::m_VBO = 0;
unsigned int Sphere::m_EBO = 0;
unsigned int Sphere::m_VerticesCount = 0;

Sphere::Sphere() {
    
}

Sphere::~Sphere() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Sphere::Init() {
    float lats = 40;
    float longs = 40;
    
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    int indicator = 0;
    for(int i = 0; i <= lats; i++) {
        double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);
        
        double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);
        
        for(int j = 0; j <= longs; j++) {
            double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);
            
            vertices.push_back(x * zr0);
            vertices.push_back(y * zr0);
            vertices.push_back(z0);
            indices.push_back(indicator);
            indicator++;
            
            vertices.push_back(x * zr1);
            vertices.push_back(y * zr1);
            vertices.push_back(z1);
            indices.push_back(indicator);
            indicator++;
        }
        
        indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    }
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    m_VerticesCount = static_cast<unsigned int>(indices.size());
}

void Sphere::Draw(const ShaderProgram& shader_program) {
    shader_program.Use();
    shader_program.SetMat4("model", m_Model);
    shader_program.SetVec4("Color", m_Color);
    
    glBindVertexArray(m_VAO);
    glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glDrawElements(GL_TRIANGLE_STRIP, m_VerticesCount, GL_UNSIGNED_INT, NULL);
}

void Sphere::Position(const glm::vec3& position) {
    m_Position = position;
    
    m_Model = glm::translate(glm::mat4(), m_Position);
}

void Sphere::Rotation(float angle, const glm::vec3& rotation_axis) {
    m_Rotation = rotation_axis;
    
    m_Model = glm::rotate(glm::mat4(), angle, rotation_axis);
}

void Sphere::Scale(const glm::vec3& scale) {
    m_Scale = scale;
    
    m_Model = glm::scale(glm::mat4(), m_Scale);
}
