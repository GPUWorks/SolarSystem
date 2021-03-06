#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> indicies, std::vector<Texture> textures)
    : m_Vertices(verticies)
    , m_Indices(indicies)
    , m_Textures(textures) {
    SetupMesh();
}

Mesh::~Mesh() {
}

void Mesh::Draw(ShaderProgram shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    
    for (unsigned int i = 0; i < m_Textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        
        std::string number;
        std::string name = m_Textures[i].Type;
        
        if(name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
        } else if(name == "texture_specular") {
            number = std::to_string(specularNr++);
        }
        
        glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, m_Textures[i].ID);
    }
    
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);
    
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    
    // Normal vectors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    
    // Rexture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
