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
            const glm::vec3& position = bindVertices[vertexIndex];

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

    std::vector<glm::vec3> jointPositions;

    for (const auto& joint : m_skeletalModel.getJoints()) {
        glm::vec3 pos = glm::vec3(joint->getCurrentJointToWorldTransform() * glm::vec4(0, 0, 0, 1));
        jointPositions.push_back(pos);
    }

    jointIndexCount = static_cast<float>(jointPositions.size());

    glGenVertexArrays(1, &jointVAO);
    glGenBuffers(1, &jointVBO);

    glBindVertexArray(jointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, jointVBO);
    glBufferData(GL_ARRAY_BUFFER, jointPositions.size() * sizeof(glm::vec3), jointPositions.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

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

    std::vector<glm::vec3> bonePositions;

    for (const auto& joint : m_skeletalModel.getJoints()) {
        glm::vec3 parentPos = glm::vec3(joint->getCurrentJointToWorldTransform() * glm::vec4(0, 0, 0, 1));
        for (const auto& child : joint->getChildren()) {
            glm::vec3 childPos = glm::vec3(child->getCurrentJointToWorldTransform() * glm::vec4(0, 0, 0, 1));
            bonePositions.push_back(parentPos);
            bonePositions.push_back(childPos);
        }
    }

    boneIndexCount = static_cast<float>(bonePositions.size());

    glGenVertexArrays(1, &boneVAO);
    glGenBuffers(1, &boneVBO);

    glBindVertexArray(boneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boneVBO);
    glBufferData(GL_ARRAY_BUFFER, bonePositions.size() * sizeof(glm::vec3), bonePositions.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

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

    if (displayMode == MESH) {
        // Use shader program
        glUseProgram(shaderProgram);

        // Enable lighting
        GLint lightingLoc = glGetUniformLocation(shaderProgram, "useLighting");
        if (lightingLoc != -1) {
            glUniform1i(lightingLoc, 1);
        }

        // Apply transformations
        applyTransform(shaderProgram);

        // Set material color
        GLint colorLoc = glGetUniformLocation(shaderProgram, "material.color");
        if (colorLoc != -1) {
            glUniform3fv(colorLoc, 1, (colorIndex == 31) ? customColor : colorPresets[colorIndex].color);
        }

        // Draw mesh only
        setupMeshBuffer();
        glBindVertexArray(meshVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(faces.size() * 3), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Disable lighting afterwards
        if (lightingLoc != -1) {
            glUniform1i(lightingLoc, 0);
        }
    }

    else if (displayMode == SKELETAL) {
        // Use shader program
        glUseProgram(shaderProgram);

        // Enable lighting (if needed for skeleton)
        GLint lightingLoc = glGetUniformLocation(shaderProgram, "useLighting");
        if (lightingLoc != -1) {
            glUniform1i(lightingLoc, 1);
        }

        // Apply transformations
        applyTransform(shaderProgram);

        // Set material color (optional for joints/bones)
        GLint colorLoc = glGetUniformLocation(shaderProgram, "material.color");
        if (colorLoc != -1) {
            glUniform3fv(colorLoc, 1, (colorIndex == 31) ? customColor : colorPresets[colorIndex].color);
        }

        // Draw joints
        glPointSize(8.0f);
        glBindVertexArray(jointVAO);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(jointIndexCount));
        glBindVertexArray(0);

        // Draw bones
        glLineWidth(2.0f);
        glBindVertexArray(boneVAO);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(boneIndexCount));
        glBindVertexArray(0);

        // Disable lighting afterwards
        if (lightingLoc != -1) {
            glUniform1i(lightingLoc, 0);
        }
    }

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

    


}


void ImportCharacter::generateCuboid(const glm::vec3& parentPos, const glm::vec3& childPos,
                                      std::vector<glm::vec3>& boneVertices,
                                      std::vector<glm::vec3>& boneNormals,
                                      std::vector<glm::uvec3>& boneFaces) {

// Extra credit - Helper utility to generate a cube or cuboid instead of a line for bones

    

}






