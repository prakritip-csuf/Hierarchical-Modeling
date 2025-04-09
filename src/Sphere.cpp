#include "Sphere.h"

#include <vector>
#include <cmath>

Sphere::Sphere(float x, float y, float z, float scale, int colorIndex, int id)
    : Shape(x, y, z, scale, colorIndex, id), VAO(0), VBO(0), EBO(0) {
    shapeType = "Sphere";  // Set the type as "Sphere"
    
    setupSphere(); // Initialize OpenGL objects for the sphere
}

Sphere::~Sphere() {
    // Cleanup OpenGL resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Sphere::setupSphere() {
    const unsigned int latitudeSegments = 20; // Number of latitude lines
    const unsigned int longitudeSegments = 20; // Number of longitude lines
    const float radius = 0.5f;

    // Generate sphere vertices, normals, and texture coordinates
    for (unsigned int y = 0; y <= latitudeSegments; ++y) {
        for (unsigned int x = 0; x <= longitudeSegments; ++x) {
            float xSegment = static_cast<float>(x) / longitudeSegments;
            float ySegment = static_cast<float>(y) / latitudeSegments;

            float xPos = radius * std::cos(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);
            float yPos = radius * std::cos(ySegment * M_PI);
            float zPos = radius * std::sin(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);

            // Store vertices and normals in the inherited attributes
            glm::vec3 position = glm::vec3(xPos, yPos, zPos);
            vertices.push_back(position);
            normals.push_back(glm::normalize(position));

            // Texture coordinates (can be used if needed)
            // texCoords.emplace_back(xSegment, ySegment);
        }
    }

    // Generate sphere faces (indices)
    for (unsigned int y = 0; y < latitudeSegments; ++y) {
        for (unsigned int x = 0; x < longitudeSegments; ++x) {
            unsigned int first = (y * (longitudeSegments + 1)) + x;
            unsigned int second = first + longitudeSegments + 1;

            faces.push_back({first, second, first + 1});
            faces.push_back({second, second + 1, first + 1});
        }
    }

    // Prepare OpenGL buffers using the populated attributes
    std::vector<float> vertexData;
    std::vector<unsigned int> indexData;

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

    for (const auto& face : faces) {
        for (int vertexIndex : face) {
            const glm::vec3& position = vertices[vertexIndex];
            const glm::vec3& normal = normals[vertexIndex];

            vertexData.insert(vertexData.end(), {position.x, position.y, position.z});
            vertexData.insert(vertexData.end(), {normal.x, normal.y, normal.z});
            vertexData.insert(vertexData.end(), {color.r, color.g, color.b});
        }
    }

    for (size_t i = 0; i < faces.size(); ++i) {
        indexData.insert(indexData.end(), {i * 3, i * 3 + 1, i * 3 + 2});
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

void Sphere::draw(GLuint shaderProgram) {

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

    // Disable lighting after drawing the sphere (for axis rendering)
    if (lightingLoc != -1) {
        glUniform1i(lightingLoc, 0);
    }

}
