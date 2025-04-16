#include "SkeletalModel.h"

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

    Joint* joint = m_joints[jointIndex];

    joint->setRotation(glm::vec3(rX, rY, rZ));

    glm::mat4 rotationM = glm::rotate(glm::mat4(1.0f), glm::radians(rX), glm::vec3(1.0f, 0.0f, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rY), glm::vec3(0.0f, 1.0f, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rZ), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 newTransform = joint->getTransform();

    //code to replace newTransform rotation portion with rotationM



    joint->setTransform(newTransform);



    
}


void bindWorldToJointTransformRecursive(Joint* joint, MatrixStack& myStack) {

    // 4.4.1.1. Recursive function for computeBindWorldToJointTransforms()
    //
    //
    
    myStack.push(joint->getTransform());

    joint->setBindWorldToJointTransform(glm::inverse(myStack.top()));

    for (auto* child : joint->getChildren()) {

        bindWorldToJointTransformRecursive(child, myStack);
    }
    
    
    
    
    

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


    m_matrixStack.clear();
    bindWorldToJointTransformRecursive(m_rootJoint, m_matrixStack);




}

void SkeletalModel::currentJointToWorldTransformsRecursive(Joint* joint, MatrixStack& myStack) {

    // 4.4.1.2. Recursive function for updateCurrentJointToWorldTransforms()
    //
    //
    
    
    myStack.push(joint->getTransform());

    joint->setCurrentJointToWorldTransform(myStack.top());
    
    for (auto* child : joint->getChildren()) {

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
	
    m_matrixStack.clear();
    currentJointToWorldTransformsRecursive(m_rootJoint, m_matrixStack);







}


