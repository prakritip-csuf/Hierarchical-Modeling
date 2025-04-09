#include "Cube.h"

Cube::Cube(float x, float y, float z, float scale, int colorIndex, int id)
	: Shape(x, y, z, scale, colorIndex, id), VAO(0), VBO(0), EBO(0) {
    shapeType = "Cube";  // Set the type as "Cube"

    // Set up OpenGL buffers
    setupCube();
}

Cube::~Cube() {
    // Cleanup OpenGL resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cube::setupCube() {
    std::vector<float> vertexData;
    std::vector<unsigned int> indexData;


    // Define cube vertices
    vertices = {
        {-0.5f, 0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f}, {0.5f,  0.5f,  0.5f},  // Front face
        {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f,  0.5f, -0.5f}   // Back face
    };

    // Define cube normals
    normals = {
        { 0.0f,  0.0f,  1.0f}, { 0.0f,  0.0f, -1.0f}, // Front/Back
        {-1.0f,  0.0f,  0.0f}, { 1.0f,  0.0f,  0.0f}, // Left/Right
        { 0.0f, -1.0f,  0.0f}, { 0.0f,  1.0f,  0.0f}  // Bottom/Top
    };

    // Define cube faces (each face is two triangles)
    faces = {
        {0, 1, 2}, {2, 3, 0},  // Front
        {4, 7, 6}, {6, 5, 4},  // Back
        {0, 4, 5}, {5, 1, 0},  // Left
        {3, 2, 6}, {6, 7, 3},  // Right
        {1, 5, 6}, {6, 2, 1},  // Bottom
        {0, 3, 7}, {7, 4, 0}   // Top
    };
    
    // Build vertex data and index data for OpenGL
   for (size_t i = 0; i < faces.size(); ++i) {
   
        glm::vec3 normal = normals[i / 2]; // Assign face normal
        glm::vec3 color = (colorIndex == 31) 
            ? glm::vec3(
                customColor[0], 
                customColor[1], 
                customColor[2]
            )
            : glm::vec3(
                colorPresets[colorIndex].color[0], 
                colorPresets[colorIndex].color[1], 
                colorPresets[colorIndex].color[2]
             );

        for (int j = 0; j < 3; ++j) {
            int vertexIndex = faces[i][j];
            const glm::vec3& position = vertices[vertexIndex];

            // Append position, normal, and color to vertexData
            vertexData.insert(vertexData.end(), {position.x, position.y, position.z});
            vertexData.insert(vertexData.end(), {normal.x, normal.y, normal.z});
            vertexData.insert(vertexData.end(), {color.r, color.g, color.b});
        }

        indexData.insert(indexData.end(), {static_cast<unsigned int>(i * 3), static_cast<unsigned int>(i * 3 + 1), static_cast<unsigned int>(i * 3 + 2)});
    }


    // Create and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), indexData.data(), GL_STATIC_DRAW);

    // Configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))); // Color
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
}


void Cube::draw(GLuint shaderProgram) {

    // Use the shader program
    glUseProgram(shaderProgram);
    
    // Enable lighting for the cube
    GLint lightingLoc = glGetUniformLocation(shaderProgram, "useLighting");
    if (lightingLoc != -1) {
        glUniform1i(lightingLoc, 1); // Enable lighting for the cube
    }

    // Apply transformations and pass to the shader
    applyTransform(shaderProgram);

    // Pass material properties
    GLint colorLoc = glGetUniformLocation(shaderProgram, "material.color");
    if (colorLoc != -1) {
        glUniform3fv(colorLoc, 1, (colorIndex == 31) ? customColor : colorPresets[colorIndex].color);
    }

    // Render the cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(faces.size() * 3), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Disable lighting after drawing the cube (for axis rendering)
    if (lightingLoc != -1) {
        glUniform1i(lightingLoc, 0);
    }
}

