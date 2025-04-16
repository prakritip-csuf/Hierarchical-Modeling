#include "MatrixStack.h"

MatrixStack::MatrixStack() {

    // 3.1.1. Initialize the matrix stack with the identity matrix.

    m_matrices.push_back(glm::mat4(1.0f));
	
}

void MatrixStack::clear() {

    // 3.1.2. Revert to just containing the identity matrix.

    m_matrices.clear();
    m_matrices.push_back(glm::mat4(1.0f));

}

glm::mat4 MatrixStack::top() const {

    // 3.1.3. Return the top of the stack

    //return(last item in the std:vector list);

    return m_matrices.back();

}

void MatrixStack::push(const glm::mat4& m) {

    // 3.1.4. Push m onto the stack.
    // Your stack should have OpenGL semantics:
    // the new top should be the old top multiplied by m

    // calculate last item on m_matrices and multiply it by m
    // add calculation to the end of m_matrices

    glm::mat4 newTop = m_matrices.back() * m; // Multiply current top with new matrix
    m_matrices.push_back(newTop); // Push composite matrix
}


void MatrixStack::pop() {

    // 3.1.5. Remove the top element from the stack (ensure stack isn't empty)
    

    // check if m_matrices >1 (size is greater than 1)
   // std:vector item to remove top of stack

   if (m_matrices.size() > 1) { // Prevent popping base identity
    m_matrices.pop_back();
   }
}

