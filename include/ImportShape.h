#ifndef IMPORTSHAPE_H
#define IMPORTSHAPE_H

#include "Shape.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class ImportShape : public Shape {
public:
    ImportShape(float x, float y, float z, float scale, int colorIndex, int id);
    ~ImportShape();

    void draw(GLuint shaderProgram) override;
    void setupShape();
    
 private:
    GLuint VAO, VBO, EBO;
    std::vector<float> vertexData;
    std::vector<unsigned int> indexData;   
};

#endif
