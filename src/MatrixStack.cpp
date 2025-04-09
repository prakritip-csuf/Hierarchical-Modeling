#include "MatrixStack.h"

MatrixStack::MatrixStack() {

    // 3.1.1. Initialize the matrix stack with the identity matrix.




	
}

void MatrixStack::clear() {

    // 3.1.2. Revert to just containing the identity matrix.






}

glm::mat4 MatrixStack::top() const {

    // 3.1.3. Return the top of the stack





}

void MatrixStack::push(const glm::mat4& m) {

    // 3.1.4. Push m onto the stack.
    // Your stack should have OpenGL semantics:
    // the new top should be the old top multiplied by m





}

void MatrixStack::pop() {

    // 3.1.5. Remove the top element from the stack (ensure stack isn't empty)
    




}

