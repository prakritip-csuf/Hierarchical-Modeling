#include "Shape.h"

#include <vector>
#include <cmath>
#include <iostream>

Shape::Shape(float x, float y, float z, float uniformScale, int colorIndex, int id,
             float scaleX, float scaleY, float scaleZ, bool useUniformScale) 
    : x(x), y(y), z(z), scale(uniformScale), scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ),
      useUniformScale(useUniformScale), colorIndex(colorIndex), angleX(0.0f), angleY(0.0f), angleZ(0.0f), id(id),
      defaultX(x), defaultY(y), defaultZ(z),
      defaultScale(uniformScale), defaultScaleX(scaleX), defaultScaleY(scaleY), defaultScaleZ(scaleZ),
      defaultUseUniformScale(useUniformScale),
      defaultRotationX(0.0f), defaultRotationY(0.0f), defaultRotationZ(0.0f),
      defaultColorIndex(colorIndex) {

    if (colorIndex == 31) {  // If custom color, initialize default custom color
        defaultCustomColor[0] = 1.0f;  // Example default value
        defaultCustomColor[1] = 0.0f;
        defaultCustomColor[2] = 0.0f;
    }
}

// Apply transformations using the model matrix

glm::mat4 Shape::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::rotate(model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));

    if (useUniformScale) {
        model = glm::scale(model, glm::vec3(scale));
    } else {
        model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
    }

    return model;
}

// Apply the model matrix to the shader
void Shape::applyTransform(GLuint shaderProgram) const {
    glm::mat4 modelMatrix = getModelMatrix();

    // Pass the model matrix to the shader
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    } else {
        std::cerr << "Warning: 'model' uniform not found in shader program." << std::endl;
    }
}


void Shape::setRotation(float ax, float ay, float az) {

    // Wrap values between 0 and 360 degrees
    angleX = (ax < 0) ? 360.0f + fmod(ax, 360.0f) : fmod(ax, 360.0f);
    angleY = (ay < 0) ? 360.0f + fmod(ay, 360.0f) : fmod(ay, 360.0f);
    angleZ = (az < 0) ? 360.0f + fmod(az, 360.0f) : fmod(az, 360.0f);

}

void Shape::rotate(float dAngleX, float dAngleY, float dAngleZ) {
    angleX += dAngleX;
    angleY += dAngleY;
    angleZ += dAngleZ;
}

void Shape::setPosition(float nx, float ny, float nz) {
    x = nx;
    y = ny;
    z = nz;
}

void Shape::setScale(float s) {
    scale = s;
    useUniformScale = true;
}

void Shape::setScale(float sx, float sy, float sz) {
    if (sx <= 0.0f || sy <= 0.0f || sz <= 0.0f) {
        std::cerr << "Warning: Scaling values must be positive." << std::endl;
        return;
    }
    scaleX = sx;
    scaleY = sy;
    scaleZ = sz;
    useUniformScale = false;
}

void Shape::useUniformScaling(bool flag) {
    useUniformScale = flag;
    // std::cout << "Scaling mode set to: " << (useUniformScale ? "Uniform" : "Non-Uniform") << std::endl;
}

glm::vec3 Shape::getNonUniformScale() const {
    return glm::vec3(scaleX, scaleY, scaleZ);
}

bool Shape::isUsingUniformScaling() const {
    return useUniformScale;
}

void Shape::setColor(int newColorIndex) {
    colorIndex = newColorIndex;
}

void Shape::setCustomColor(float r, float g, float b) {
    customColor[0] = r;
    customColor[1] = g;
    customColor[2] = b;
}

void Shape::calculateNormals() {
    normals.clear(); // Clear existing normals

    for (const auto& face : faces) {
        // Calculate two edges of the triangle
        glm::vec3 v1 = vertices[face[1]] - vertices[face[0]];
        glm::vec3 v2 = vertices[face[2]] - vertices[face[0]];

        // Compute the normal using the cross product
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        // Store the calculated normal
        normals.push_back(normal);
    }
}


float Shape::getX() const { return x; }
float Shape::getY() const { return y; }
float Shape::getZ() const { return z; }
float Shape::getAngleX() const { return angleX; }
float Shape::getAngleY() const { return angleY; }
float Shape::getAngleZ() const { return angleZ; }
float Shape::getScale() const { return scale; }
int Shape::getColorIndex() const { return colorIndex; }
const float* Shape::getCustomColor() const { return customColor; }
int Shape::getId() const { return id; }
std::string Shape::getShapeType() const { return shapeType; }

void Shape::setShapeType(std::string newShapeType) { shapeType = newShapeType; }

void Shape::addVertex(const glm::vec3 vertex) { vertices.push_back(vertex); }
void Shape::addNormal(const glm::vec3 normal) { normals.push_back(normal); }
void Shape::addFace(int v1, int v2, int v3) { faces.push_back({v1, v2, v3}); }
void Shape::setVertices(const std::vector<glm::vec3>& verts) { vertices = verts; }
void Shape::setNormals(const std::vector<glm::vec3>& norms) { normals = norms; }
void Shape::setFaces(const std::vector<std::vector<int>>& facs) { faces = facs; }


// Implementation of the getVertices method
std::vector<glm::vec3>& Shape::getVertices() {
    return vertices;
}

const std::vector<glm::vec3>& Shape::getVertices() const {
    return vertices;
}

// Implementation of the getFaces method
std::vector<std::vector<int>>& Shape::getFaces() {
    return faces;
}

const std::vector<std::vector<int>>& Shape::getFaces() const {
    return faces;
}

// Implementation of the getNormals method
std::vector<glm::vec3>& Shape::getNormals() {
    return normals;
}

const std::vector<glm::vec3>& Shape::getNormals() const {
    return normals;
}

// Reset to default
void Shape::resetToDefault() {
    setPosition(defaultX, defaultY, defaultZ);
    setScale(defaultScale);
    setScale(defaultScaleX, defaultScaleY, defaultScaleZ);
    useUniformScaling(defaultUseUniformScale);
    setRotation(defaultRotationX, defaultRotationY, defaultRotationZ);
    setColor(defaultColorIndex);

    if (defaultColorIndex == 31) {  // If custom color, reset it
        setCustomColor(defaultCustomColor[0], defaultCustomColor[1], defaultCustomColor[2]);
    }
}
