#ifndef SHAPE_H
#define SHAPE_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>
#include <string>
#include "ColorPresets.h"
#include "Globals.h"

class Shape {

public:
    Shape(float x, float y, float z, float uniformScale, int colorIndex, int id, 
          float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f, bool useUniformScale = true);
    virtual ~Shape() = default;

    virtual void draw(GLuint shaderProgram) = 0;

    // Transformation-related methods
    void setRotation(float ax, float ay, float az);
    void rotate(float dAngleX, float dAngleY, float dAngleZ);
    void setPosition(float nx, float ny, float nz);

    void setScale(float s); // Uniform scale
    void setScale(float sx, float sy, float sz); // Non-uniform scale
    void useUniformScaling(bool flag); // Toggle scaling mode

    // Apply transformations using shaders
    void applyTransform(GLuint shaderProgram) const;

    // Color-related methods
    void setColor(int newColorIndex);
    void setCustomColor(float r, float g, float b);

    // Calculate normal values
    void calculateNormals();
    
    // Getter methods
    float getX() const;
    float getY() const;
    float getZ() const;

    float getAngleX() const;
    float getAngleY() const;
    float getAngleZ() const;

    float getScale() const;
    glm::vec3 getNonUniformScale() const;
    bool isUsingUniformScaling() const;

    int getColorIndex() const;
    const float* getCustomColor() const;
    int getId() const;
    std::string getShapeType() const;

    // Set the shape type
    void setShapeType(std::string newShapeType);

    void addVertex(const glm::vec3 vertex);
    void addNormal(const glm::vec3 normal);
    void addFace(int v1, int v2, int v3);
    void setVertices(const std::vector<glm::vec3>& verts);
    void setNormals(const std::vector<glm::vec3>& norms);
    void setFaces(const std::vector<std::vector<int>>& facs);

    // Add these methods to allow access to vertices, faces, and normals
    std::vector<glm::vec3>& getVertices();
    const std::vector<glm::vec3>& getVertices() const;

    std::vector<std::vector<int>>& getFaces();
    const std::vector<std::vector<int>>& getFaces() const;

    std::vector<glm::vec3>& getNormals();
    const std::vector<glm::vec3>& getNormals() const;

    // Reset to default values
    void resetToDefault();


protected:
    float x, y, z;
    float scale;
    float scaleX, scaleY, scaleZ; // Non-uniform scaling along axes
    bool useUniformScale;     // Flag to toggle between scaling modes
    float angleX, angleY, angleZ;
    int colorIndex;
    float customColor[3];
    int id;
    std::string shapeType;

    // Transformation helpers
    glm::mat4 getModelMatrix() const;

    // Vertices, normals, and faces
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<std::vector<int>> faces;

private:

    // Default values for reset
    float defaultX, defaultY, defaultZ;
    float defaultScaleX, defaultScaleY, defaultScaleZ;
    float defaultScale;
	bool defaultUseUniformScale;
    float defaultRotationX, defaultRotationY, defaultRotationZ;
    int defaultColorIndex;
    float defaultCustomColor[3];  // For custom color if used    
    
};

#endif
