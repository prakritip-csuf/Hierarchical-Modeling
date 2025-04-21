#include "ImportCharacter.h"
#include <iostream>

ImportCharacter::ImportCharacter(float x, float y, float z, float scale, int colorIndex, int id)
    : Shape(x, y, z, scale, colorIndex, id), m_skeletalModel(),
      meshVAO(0), meshVBO(0), meshEBO(0), 
      jointVAO(0), jointVBO(0), jointEBO(0), 
      boneVAO(0), boneVBO(0), boneEBO(0),
      jointIndexCount(0), boneIndexCount(0) {

}

ImportCharacter::~ImportCharacter() {
    glDeleteVertexArrays(1, &meshVAO);
    glDeleteBuffers(1, &meshVBO);
    glDeleteBuffers(1, &meshEBO);

    glDeleteVertexArrays(1, &jointVAO);
    glDeleteBuffers(1, &jointVBO);
    glDeleteBuffers(1, &jointEBO);

    glDeleteVertexArrays(1, &boneVAO);
    glDeleteBuffers(1, &boneVBO);
    glDeleteBuffers(1, &boneEBO);
}

void ImportCharacter::setupMeshBuffer() {

    // Clear existing data
    if (meshVAO) glDeleteVertexArrays(1, &meshVAO);
    if (meshVBO) glDeleteBuffers(1, &meshVBO);
    if (meshEBO) glDeleteBuffers(1, &meshEBO);

    // Collect vertices, normals, colors, and indices
    std::vector<float> meshVertices;
    std::vector<unsigned int> meshIndices;

    for (size_t i = 0; i < faces.size(); ++i) {
    
        glm::vec3 normal = normals[i]; // Assign face normal
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

            // Append position, normal, and color to meshVertices
            meshVertices.insert(meshVertices.end(), {position.x, position.y, position.z});
            meshVertices.insert(meshVertices.end(), {normal.x, normal.y, normal.z});
            meshVertices.insert(meshVertices.end(), {color.r, color.g, color.b});
        }

        meshIndices.insert(meshIndices.end(), {static_cast<unsigned int>(i * 3), static_cast<unsigned int>(i * 3 + 1), static_cast<unsigned int>(i * 3 + 2)});
    }

    // Create and bind meshVAO, meshVBO, and meshEBO
    glGenVertexArrays(1, &meshVAO);
    glGenBuffers(1, &meshVBO);
    glGenBuffers(1, &meshEBO);

    glBindVertexArray(meshVAO);

    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(float), meshVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(unsigned int), meshIndices.data(), GL_STATIC_DRAW);

    // Configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))); // Color
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind meshVAO
}



void ImportCharacter::setupJointBuffer() {

    // 3.2.2.1. Build the joint mesh using GL_POINTS or you can
    // build a sphere mesh, and create the buffer VAO, VBO, and  
    // EBO for use.
    // 
    
    if (jointVAO) glDeleteVertexArrays(1, &jointVAO);
    if (jointVBO) glDeleteBuffers(1, &jointVBO);
    if (jointEBO) glDeleteBuffers(1, &jointEBO);

    std::vector<glm::vec3> jointVertices;
    std::vector<glm::vec3> jointNormals;
    std::vector<glm::uvec3> jointFaces;

    for (const auto& joint : m_skeletalModel.getJoints()) {
        glm::vec3 center = glm::vec3(joint->getCurrentJointToWorldTransform() * glm::vec4(0,0,0,1));

        unsigned int baseIndex = jointVertices.size();

        std::vector<glm::vec3> localVertices;
        std::vector<glm::vec3> localNormals;
        std::vector<glm::uvec3> localFaces;

        generateSphere(0.02f, center, localVertices, localNormals, localFaces);

        jointVertices.insert(jointVertices.end(), localVertices.begin(), localVertices.end());
        jointNormals.insert(jointNormals.end(), localNormals.begin(), localNormals.end());

        for (auto& f : localFaces) {
            jointFaces.push_back(glm::uvec3(f.x + baseIndex, f.y + baseIndex, f.z + baseIndex));
        }
    }

    jointIndexCount = jointFaces.size() * 3;

    std::vector<float> vertexData;
    for (size_t i = 0; i < jointVertices.size(); ++i) {
        vertexData.push_back(jointVertices[i].x);
        vertexData.push_back(jointVertices[i].y);
        vertexData.push_back(jointVertices[i].z);

        vertexData.push_back(jointNormals[i].x);
        vertexData.push_back(jointNormals[i].y);
        vertexData.push_back(jointNormals[i].z);
    }

    std::vector<unsigned int> indices;
    for (auto& f : jointFaces) {
        indices.push_back(f.x);
        indices.push_back(f.y);
        indices.push_back(f.z);
    }

    glGenVertexArrays(1, &jointVAO);
    glGenBuffers(1, &jointVBO);
    glGenBuffers(1, &jointEBO);

    glBindVertexArray(jointVAO);

    glBindBuffer(GL_ARRAY_BUFFER, jointVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, jointEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

void ImportCharacter::setupBoneBuffer() {

    // 3.2.2.2. Build the bone mesh using GL_LINES or you can
    // build a cube or cuboid mesh, and create the buffer VAO, VBO, and  
    // EBO for use.
    // 
    //
    
    if (boneVAO) glDeleteVertexArrays(1, &boneVAO);
    if (boneVBO) glDeleteBuffers(1, &boneVBO);
    if (boneEBO) glDeleteBuffers(1, &boneEBO);

    std::vector<glm::vec3> boneVertices;
    std::vector<glm::vec3> boneNormals;
    std::vector<glm::uvec3> boneFaces;

    for (const auto& joint : m_skeletalModel.getJoints()) {
        glm::vec3 parentPos = glm::vec3(joint->getCurrentJointToWorldTransform() * glm::vec4(0,0,0,1));

        for (const auto& child : joint->getChildren()) {
            glm::vec3 childPos = glm::vec3(child->getCurrentJointToWorldTransform() * glm::vec4(0,0,0,1));

            unsigned int baseIndex = boneVertices.size();

            std::vector<glm::vec3> localVertices;
            std::vector<glm::vec3> localNormals;
            std::vector<glm::uvec3> localFaces;

            generateCuboid(parentPos, childPos, localVertices, localNormals, localFaces);

            boneVertices.insert(boneVertices.end(), localVertices.begin(), localVertices.end());
            boneNormals.insert(boneNormals.end(), localNormals.begin(), localNormals.end());

            for (auto& f : localFaces) {
                boneFaces.push_back(glm::uvec3(f.x + baseIndex, f.y + baseIndex, f.z + baseIndex));
            }
        }
    }

    boneIndexCount = boneFaces.size() * 3;

    std::vector<float> vertexData;
    for (size_t i = 0; i < boneVertices.size(); ++i) {
        vertexData.push_back(boneVertices[i].x);
        vertexData.push_back(boneVertices[i].y);
        vertexData.push_back(boneVertices[i].z);

        vertexData.push_back(boneNormals[i].x);
        vertexData.push_back(boneNormals[i].y);
        vertexData.push_back(boneNormals[i].z);
    }

    // Flatten face indices
    std::vector<unsigned int> indices;
    for (auto& f : boneFaces) {
        indices.push_back(f.x);
        indices.push_back(f.y);
        indices.push_back(f.z);
    }

    glGenVertexArrays(1, &boneVAO);
    glGenBuffers(1, &boneVBO);
    glGenBuffers(1, &boneEBO);

    glBindVertexArray(boneVAO);

    glBindBuffer(GL_ARRAY_BUFFER, boneVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boneEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position (3 floats) + Normal (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    
}



// Getter for bindVertices
const std::vector<glm::vec3>& ImportCharacter::getBindVertices() const {
    return bindVertices;
}

// Setter for bindVertices
void ImportCharacter::setBindVertices(const std::vector<glm::vec3>& vertices) {
    bindVertices = vertices;
}

// Getter for skeletal model
SkeletalModel& ImportCharacter::getSkeletalModel() {
    return m_skeletalModel;
}

// Set joint transform by index
void ImportCharacter::setJointTransform(int jointIndex, float rX, float rY, float rZ) {
    m_skeletalModel.setJointTransform(jointIndex, rX, rY, rZ);
}

// Getter for attachments
const std::vector<std::vector<float>>& ImportCharacter::getAttachments() const {
    return attachments;
}

// Setter for attachments
void ImportCharacter::setAttachments(const std::vector<std::vector<float>>& attachments) {
    this->attachments = attachments;
}

// Getter for display mode
ImportCharacter::DisplayMode ImportCharacter::getDisplayMode() const {
    return displayMode;
}

// Setter for display mode
void ImportCharacter::setDisplayMode(DisplayMode mode) {
    displayMode = mode;
}

void ImportCharacter::updateMeshVertices() {

    // 4.4.2. This is the core of SSD.
    // Implement this method to update the vertices of the mesh
    // given the current state of the skeleton.
    // You will need both the bind pose world --> joint transforms.
    // and the current joint --> world transforms.

    m_skeletalModel.updateCurrentJointToWorldTransforms();

    vertices.clear();
    vertices.resize(bindVertices.size(), glm::vec3(0.0f));

    for (size_t i = 0; i < bindVertices.size(); ++i) {
        glm::vec3 newPos(0.0f);

        for (size_t j = 0; j < attachments[i].size(); ++j) {
            float weight = attachments[i][j];
            if (weight == 0.0f) continue;

            Joint* joint = m_skeletalModel.getJoints()[j];
            glm::mat4 T = joint->getCurrentJointToWorldTransform();
            glm::mat4 B_inv = joint->getBindWorldToJointTransform();

            glm::vec4 transformed = T * B_inv * glm::vec4(bindVertices[i], 1.0f);
            newPos += weight * glm::vec3(transformed);
        }

        vertices[i] = newPos;
    }
   
    setupMeshBuffer();
    setupJointBuffer();
    setupBoneBuffer();

}


void ImportCharacter::draw(GLuint shaderProgram) {
    
    glUseProgram(shaderProgram);
    
    // Lighting setup
    GLint lightingLoc = glGetUniformLocation(shaderProgram, "useLighting");
    if (lightingLoc != -1) glUniform1i(lightingLoc, 1);

    // Critical fix: Apply transforms AFTER updating vertices
    if (displayMode == SKELETAL) {
        updateMeshVertices(); // Update before applying transforms
    }
    applyTransform(shaderProgram); // Applies to current geometry

    // Material properties
    GLint colorLoc = glGetUniformLocation(shaderProgram, "material.color");
    if (colorLoc != -1) {
        glUniform3fv(colorLoc, 1, (colorIndex == 31) ? customColor : colorPresets[colorIndex].color);
    }

    updateMeshVertices();

    if (displayMode == MESH) {
        glBindVertexArray(meshVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(faces.size() * 3), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    } 
    else if (displayMode == SKELETAL) {
            float white[3] = {1.0f, 1.0f, 1.0f};
            glUniform3fv(colorLoc, 1, white);

        // Draw joints
        glBindVertexArray(jointVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(jointIndexCount), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Draw bones (as cuboids)
        glBindVertexArray(boneVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(boneIndexCount), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); 
    }

    if (lightingLoc != -1) glUniform1i(lightingLoc, 0);

}


void ImportCharacter::resetPose() {
    for (size_t i = 0; i < m_skeletalModel.getJoints().size(); ++i) {
        m_skeletalModel.setJointTransform(i, 0.0f, 0.0f, 0.0f);
    }

    m_skeletalModel.updateCurrentJointToWorldTransforms();
    updateMeshVertices();
}



void ImportCharacter::generateSphere(float radius, glm::vec3 center,
                                      std::vector<glm::vec3>& jointVertices,
                                      std::vector<glm::vec3>& jointNormals,
                                      std::vector<glm::uvec3>& jointFaces) {
                         
// Extra credit - Helper utility to generate a sphere instead of a point for joints
    int sectorCount = 16; 
    int stackCount = 16; 

    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stackCount; 
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * glm::pi<float>() / sectorCount;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            glm::vec3 pos = glm::vec3(x, y, z) + center;
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

            jointVertices.push_back(pos);
            jointNormals.push_back(normal);
        }
    }

    
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                jointFaces.push_back(glm::uvec3(k1, k2, k1 + 1));
            }
            if (i != (stackCount - 1)) {
                jointFaces.push_back(glm::uvec3(k1 + 1, k2, k2 + 1));
            }
        }
    }
    }



void ImportCharacter::generateCuboid(const glm::vec3& parentPos, const glm::vec3& childPos,
                                      std::vector<glm::vec3>& boneVertices,
                                      std::vector<glm::vec3>& boneNormals,
                                      std::vector<glm::uvec3>& boneFaces) {

    // Extra credit - Helper utility to generate a cube or cuboid instead of a line for bones
    glm::vec3 z = glm::normalize(childPos - parentPos);
    float length = glm::length(childPos - parentPos);

   
    glm::vec3 rnd(0, 0, 1);
    if (glm::abs(glm::dot(z, rnd)) > 0.99f)
        rnd = glm::vec3(0, 1, 0); 

    glm::vec3 y = glm::normalize(glm::cross(z, rnd));
    glm::vec3 x = glm::normalize(glm::cross(y, z));

    glm::mat4 transform(1.0f);
    transform[0] = glm::vec4(x * 0.01f, 0.0f);
    transform[1] = glm::vec4(y * 0.01f, 0.0f);
    transform[2] = glm::vec4(z * length, 0.0f);
    transform[3] = glm::vec4(parentPos, 1.0f);

    std::vector<glm::vec3> unitCube = {
        {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f},
        {-0.5f, -0.5f, 1.0f}, {0.5f, -0.5f, 1.0f}, {0.5f, 0.5f, 1.0f}, {-0.5f, 0.5f, 1.0f}
    };

    for (const auto& v : unitCube) {
        glm::vec4 worldV = transform * glm::vec4(v, 1.0f);
        boneVertices.push_back(glm::vec3(worldV));
    }

    std::vector<glm::vec3> cubeNormals = {
        {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
        {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}
    };
    for (const auto& n : cubeNormals) {
        boneNormals.push_back(n);
    }

    std::vector<glm::uvec3> faces = {
        {0, 1, 2}, {2, 3, 0}, // bottom
        {4, 5, 6}, {6, 7, 4}, // top
        {0, 1, 5}, {5, 4, 0}, // side
        {1, 2, 6}, {6, 5, 1},
        {2, 3, 7}, {7, 6, 2},
        {3, 0, 4}, {4, 7, 3}
    };
    for (auto f : faces) {
        boneFaces.push_back(f);
    }
    

}


