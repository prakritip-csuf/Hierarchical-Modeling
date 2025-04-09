#include "ShapeManager.h"
#include <algorithm>  // For std::find

ShapeManager::ShapeManager() : selectedShape(nullptr) {}

ShapeManager::~ShapeManager() {
    // Clean up all dynamically allocated shapes
    for (Shape* shape : shapes) {
        if (shape) {  // Ensure it's not already deleted
            delete shape;
        }
    }
    shapes.clear();  // Ensure the vector is empty after deletion
 
}

void ShapeManager::addShape(Shape* shape) {
    shapes.push_back(shape);
}

void ShapeManager::deleteShape(Shape* shape) {
    auto it = std::find(shapes.begin(), shapes.end(), shape);
    if (it != shapes.end()) {
        delete *it;              // Free the memory
        shapes.erase(it);        // Remove shape from the list
        if (selectedShape == shape) {
            selectedShape = nullptr;  // Deselect the shape if it was selected
        }
    }
}

std::vector<Shape*>& ShapeManager::getShapes() {
    return shapes;
}

void ShapeManager::setSelectedShape(Shape* shape) {
    selectedShape = shape;
}

void ShapeManager::setSelectedShapeById(int id) {
    for (Shape* shape : shapes) {
        if (shape->getId() == id) {
            selectedShape = shape;
            break;
        }
    }
}

void ShapeManager::setSelectedShapeByLastAdded() {
    selectedShape = shapes.back();
}

Shape* ShapeManager::getSelectedShape() const {
    return selectedShape;
}

Shape* ShapeManager::getShapeById(int id) const {
    return shapes[id];
}

int ShapeManager::getShapeCounter() {
	return shapeCounter;
}

void ShapeManager::setShapeCounter(int counter) {
	shapeCounter = counter;
}

int ShapeManager::incrementShapeCounter() {
	return ++shapeCounter;
}
	
void ShapeManager::resetAllShapes() {
    for (Shape* shape : shapes) {  // Assuming shapes is a std::vector<Shape*>
        if (shape) {
            shape->resetToDefault();
        }
    }
}

