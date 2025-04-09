#ifndef RENDERER_H
#define RENDERER_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>
#include <algorithm>  // For std::find

#include "Globals.h"
#include "ShapeManager.h"
#include "Shape.h"
#include "Cube.h"
#include "Custom.h"
#include "ImportShape.h"
#include "Icosahedron.h" 
#include "Pyramid.h"
#include "Sphere.h"
#include "Teapot.h"
#include "FileImporter.h"

class Renderer {
public:
    Renderer();

    // Shader utilities
    GLuint getShaderProgram() const;
    void setShaderProgram(GLuint shader);

    // Public methods for controlling the rendering pipeline
    void setupLighting(GLuint shaderProg);
    void drawAxis(GLuint shaderProgram);
    void renderScene(ShapeManager& shapeManager);


private:
    // Camera and transformation variables
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;
    
    float theta;  // Yaw angle (rotation around Y-axis)
    float phi;    // Pitch angle (rotation up/down)
    float radius; // Distance from target    
    
    void updateCameraPosition(); // Function to compute position from spherical coordinates
    
    float translateX, translateY, translateZ;
    float angleX, angleY;
    float scaleFactor;
    
    // Boolean variable to track axis visibility
    bool showAxis = true;

    GLuint shaderProgram; // Holds the active shader program

};

#endif  // RENDERER_H
