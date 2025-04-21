#ifndef SKELETALMODEL_H
#define SKELETALMODEL_H

#include "MatrixStack.h"
#include "Joint.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class SkeletalModel {
public:
    SkeletalModel();

    Joint* getRootJoint() const;
    void setRootJoint(Joint* rootJoint);
    void addJointChild(int parentIndex, Joint* child);

    const std::vector<Joint*>& getJoints() const;
    void setJoints(const std::vector<Joint*>& joints);

    const std::vector<glm::vec3>& getJointCenters() const;
    const std::vector<std::pair<glm::vec3, glm::vec3>> getBonePairs() const;

    MatrixStack& getMatrixStack();

    void setJointTransform(int jointIndex, float rX, float rY, float rZ);

    void computeBindWorldToJointTransforms();
    void updateCurrentJointToWorldTransforms();


private:   
    std::vector<Joint*> m_joints;
    Joint* m_rootJoint;
    MatrixStack m_matrixStack;
    std::vector<glm::vec3> jointCenters;
    std::vector<std::pair<glm::vec3, glm::vec3>> bonePairs;    

    void bindWorldToJointTransformRecursive(Joint* joint, MatrixStack& myStack);
    void currentJointToWorldTransformsRecursive(Joint* joint, MatrixStack& myStack);     
 
};

#endif
