#include "SkeletalModel.h"
#include <iostream>
#include <functional>

SkeletalModel::SkeletalModel() {}

// Getters and setters for root joint
Joint* SkeletalModel::getRootJoint() const { return m_rootJoint; }
void SkeletalModel::setRootJoint(Joint* rootJoint) { m_rootJoint = rootJoint; }

// Getters and setters for all joints
const std::vector<Joint*>& SkeletalModel::getJoints() const { return m_joints; }
void SkeletalModel::setJoints(const std::vector<Joint*>& joints) { m_joints = joints; }

// Getter for joint centers and bone pairs
const std::vector<glm::vec3>& SkeletalModel::getJointCenters() const { return jointCenters; }
const std::vector<std::pair<glm::vec3, glm::vec3>> SkeletalModel::getBonePairs() const { return bonePairs; }

MatrixStack& SkeletalModel::getMatrixStack() { return m_matrixStack; }

void SkeletalModel::addJointChild(int parentIndex, Joint* child) {
    if (parentIndex < 0 || parentIndex >= static_cast<int>(m_joints.size())) {
        std::cerr << "Error: Invalid parent index provided." << std::endl;
        return;
    }

    Joint* parent = m_joints[parentIndex];
    parent->addChild(child);
    m_joints.push_back(child);
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ) {

    // 3.3. Implement this method to compute a per-joint transform 
    // the sliders on the user interface. This will connect the values 
    // generated on the user interface and store the values onto the 
    // joints transform attribute.
    // 
    //

    if (jointIndex < 0 || jointIndex >= static_cast<int>(m_joints.size())) {
        std::cerr << "Error: Invalid joint index!" << std::endl;
        return;
    }

    Joint* joint = m_joints[jointIndex];

    // Set rotation angles (as Euler angles)
    glm::vec3 rotation(rX, rY, rZ);
    joint->setRotation(rotation);

    // Convert degrees to radians
    float radX = glm::radians(rX);
    float radY = glm::radians(rY);
    float radZ = glm::radians(rZ);

    // Build local transform from translation and rotation
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), radX, glm::vec3(1, 0, 0));
    rotationMat = glm::rotate(rotationMat, radY, glm::vec3(0, 1, 0));
    rotationMat = glm::rotate(rotationMat, radZ, glm::vec3(0, 0, 1));

    glm::vec3 translation = glm::vec3(joint->getTransform()[3]);
    glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), translation);
    localTransform *= rotationMat;

    joint->setTransform(localTransform);
}




void SkeletalModel::bindWorldToJointTransformRecursive(Joint* joint, MatrixStack& myStack) {

    // 4.4.1.1. Recursive function for computeBindWorldToJointTransforms()
    //
    //
    // Push the full local bind transform
    myStack.push(joint->getTransform());

    // Compute and store the inverse world-to-joint transform in bind pose
    glm::mat4 jointToWorld = myStack.top();
    glm::mat4 worldToJoint = glm::inverse(jointToWorld);
    joint->setBindWorldToJointTransform(worldToJoint);

    for (Joint* child : joint->getChildren()) {
        bindWorldToJointTransformRecursive(child, myStack);
    }

    myStack.pop();

}

void SkeletalModel::computeBindWorldToJointTransforms() {

    // 4.4.1.1. Implement this method to compute a per-joint transform from
    // world-space to joint space in the BIND POSE.
    //
    // Note that this needs to be computed only once since there is only
    // a single bind pose.
    //
    // This method should update each joint's bindWorldToJointTransform.
    // You will need to add a recursive helper function to traverse the joint hierarchy.

    if (!m_rootJoint) {
        std::cerr << "Error: Root joint not set!" << std::endl;
        return;
    }

    m_matrixStack.clear();
    m_matrixStack.push(glm::mat4(1.0f));

    bindWorldToJointTransformRecursive(m_rootJoint, m_matrixStack);


}

void SkeletalModel::currentJointToWorldTransformsRecursive(Joint* joint, MatrixStack& myStack) {

    // 4.4.1.2. Recursive function for updateCurrentJointToWorldTransforms()
    //
    //
     // Push the current joint’s local transform (setJointTransform must have set this)
     myStack.push(joint->getTransform());

    glm::mat4 jointToWorld = myStack.top();
    joint->setCurrentJointToWorldTransform(jointToWorld);

    for (Joint* child : joint->getChildren()) {
        currentJointToWorldTransformsRecursive(child, myStack);
    }

    myStack.pop();
    

}

void SkeletalModel::updateCurrentJointToWorldTransforms() {

    // 4.4.1.2. Implement this method to compute a per-joint transform from
    // joint space to world space in the CURRENT POSE.
    //
    // The current pose is defined by the rotations you've applied to the
    // joints and hence needs to be *updated* every time the joint angles change.
    //
    // This method should update each joint's bindWorldToJointTransform.
    // You will need to add a recursive helper function to traverse the joint hierarchy.
	

    if (!m_rootJoint) {
        std::cerr << "Error: Root joint not set!" << std::endl;
        return;
    }

    m_matrixStack.clear();
    currentJointToWorldTransformsRecursive(m_rootJoint, m_matrixStack);


}


