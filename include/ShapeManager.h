#ifndef SHAPEMANAGER_H
#define SHAPEMANAGER_H

#include <vector>
#include "Shape.h"

class ShapeManager {
public:
    // Constructor
    ShapeManager();

    // Destructor (will clean up dynamically allocated shapes)
    ~ShapeManager();

    // Adds a new shape to the list
    void addShape(Shape* shape);

    // Deletes a specific shape from the list
    void deleteShape(Shape* shape);

    // Returns a reference to the list of shapes
    std::vector<Shape*>& getShapes();

    // Selects a shape (by pointer, ID, or last selected)
    void setSelectedShape(Shape* shape);
    void setSelectedShapeById(int id);
    void setSelectedShapeByLastAdded();

    // Returns the currently selected shape
    Shape* getSelectedShape() const;

    // Returns a shape by ID
    Shape* getShapeById(int id) const;

    // Add, Get and Set shapecounter
    int getShapeCounter();
    void setShapeCounter(int counter);
    int incrementShapeCounter();


    //Reset all shapes to default
    void resetAllShapes();

private:
    std::vector<Shape*> shapes;  // List of shapes
    Shape* selectedShape;        // Currently selected shape
	int shapeCounter;  // Keeps track of unique IDs for shapes
};

#endif  // SHAPEMANAGER_H
