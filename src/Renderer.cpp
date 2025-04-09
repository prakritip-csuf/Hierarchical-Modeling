#include "Application.h"
#include "Renderer.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>                  // Core GLM types
#include <glm/gtc/matrix_transform.hpp> // Transformations (translate, rotate, scale)
#include <glm/gtc/type_ptr.hpp>         // To pass matrices to OpenGL shaders


Renderer::Renderer()
    : translateX(0.0f), translateY(0.0f), translateZ(0.0f),
      angleX(0.0f), angleY(0.0f), scaleFactor(1.0f),
      cameraTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
      cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      theta(glm::pi<float>() / 2.0f), phi(0.0f), radius(5.0f) // Default distance from origin      
      {

    // Set initial camera position
    updateCameraPosition();
}
      

// Getter for Shader Program
GLuint Renderer::getShaderProgram() const {
    return shaderProgram;
}


// Getter for Shader Program
void Renderer::setShaderProgram(GLuint shaderProg) {
    shaderProgram = shaderProg;
}


void Renderer::updateCameraPosition() {

    // Convert spherical coordinates to Cartesian coordinates
    cameraPosition.x = cameraTarget.x + radius * cos(phi) * cos(theta);
    cameraPosition.y = cameraTarget.y + radius * sin(phi);
    cameraPosition.z = cameraTarget.z + radius * cos(phi) * sin(theta);
}


// Lighting setup
void Renderer::setupLighting(GLuint shaderProgram) {

    // Define light propertiess
    glm::vec3 lightPos(10.0f, 10.0f, 30.0f); // Position of the light source
    glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f); // Ambient light color
    glm::vec3 lightDiffuse(0.8f, 0.8f, 0.8f); // Diffuse light color
    glm::vec3 lightSpecular(0.2f, 0.2f, 0.2f); // Specular light color

    // Activate the shader program to set uniforms
    glUseProgram(shaderProgram);

    // Get uniform locations for light properties
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.ambient"), 1, glm::value_ptr(lightAmbient));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.diffuse"), 1, glm::value_ptr(lightDiffuse));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.specular"), 1, glm::value_ptr(lightSpecular));

}


// Function to draw axis lines
void Renderer::drawAxis(GLuint shaderProgram) {

    // Use the shader program
    glUseProgram(shaderProgram);
    
    // Ensure model matrix is identity for the axis
    glm::mat4 model = glm::mat4(1.0f);
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Disable lighting for the axis
    GLint lightingLoc = glGetUniformLocation(shaderProgram, "useLighting");
    if (lightingLoc != -1) {
        glUniform1i(lightingLoc, 0); // Ensure lighting is OFF for the axis
    }

    // Vertex data for axis lines, including positions and colors
    float axisVertices[] = {
        // Positions        // Colors
        // X-axis (Red)
        -50.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Origin
         50.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // +X

        // Y-axis (Green)
        0.0f, -50.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Origin
        0.0f,  50.0f, 0.0f,  0.0f, 1.0f, 0.0f, // +Y

        // Z-axis (Blue)
        0.0f, 0.0f, -50.0f,  0.0f, 0.0f, 1.0f, // Origin
        0.0f, 0.0f,  50.0f,  0.0f, 0.0f, 1.0f  // +Z
    };

    GLuint axisVAO, axisVBO;
    // Generate and bind a Vertex Array Object (VAO)
    glGenVertexArrays(1, &axisVAO);
    glBindVertexArray(axisVAO);

    // Generate and bind a Vertex Buffer Object (VBO)
    glGenBuffers(1, &axisVBO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    // Configure the position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Configure the color attribute (location 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Draw the axis lines
    glBindVertexArray(axisVAO);
    glLineWidth(1.0f); // Choose a float value > 1.0f for thicker lines
    glDrawArrays(GL_LINES, 0, 6);

    // Re-enable lighting for shapes
    if (lightingLoc != -1) {
        glUniform1i(lightingLoc, 1);
    }

    // Unbind and clean up
    glBindVertexArray(0);
    glDeleteBuffers(1, &axisVBO);
    glDeleteVertexArrays(1, &axisVAO);
    
}


// Render the scene and the shapes
void Renderer::renderScene(ShapeManager& shapeManager) {

    // Get the latest window size
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);

    // Ensure viewport matches window size
    glViewport(0, 0, width, height);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the ImGui frame
    ImGuiIO& io = ImGui::GetIO();

    // Main Menu Bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) { }
            if (ImGui::MenuItem("Open")) {
//                    Application::getInstance().loadScene();        
            }
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save")) {
//                    Application::getInstance().saveScene();
            }
            if (ImGui::MenuItem("Save as .. ")) { }

            ImGui::Separator();
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Esc")) {
                exit(0);
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Insert")) {
            if (ImGui::MenuItem("Cube")) {
				shapeManager.addShape(new Cube(0.0f, 0.0f, 0.0f, 1.0f, 2, shapeManager.incrementShapeCounter()));
				shapeManager.setSelectedShapeByLastAdded();  // Select the last shape added
            }
            if (ImGui::MenuItem("Sphere")) {
				shapeManager.addShape(new Sphere(0.0f, 0.0f, 0.0f, 1.0f, 3, shapeManager.incrementShapeCounter()));
				shapeManager.setSelectedShapeByLastAdded();  // Select the last shape added
            }
            if (ImGui::MenuItem("Pyramid")) {
				shapeManager.addShape(new Pyramid(0.0f, 0.0f, 0.0f, 1.0f, 5, shapeManager.incrementShapeCounter()));
				shapeManager.setSelectedShapeByLastAdded();  // Select the last shape added
            }
            if (ImGui::MenuItem("Teapot")) {
				shapeManager.addShape(new Teapot(0.0f, 0.0f, 0.0f, 1.0f, 4, shapeManager.incrementShapeCounter()));
				shapeManager.setSelectedShapeByLastAdded();  // Select the last shape added
            }
            if (ImGui::MenuItem("Icosahedron")) {
				shapeManager.addShape(new Icosahedron(0.0f, 0.0f, 0.0f, 1.0f, 6, shapeManager.incrementShapeCounter()));
				shapeManager.setSelectedShapeByLastAdded();  // Select the last shape added
            }
            if (ImGui::MenuItem(customShapeName.c_str())) {
				shapeManager.addShape(new Custom(0.0f, 0.0f, 0.0f, 1.0f, 11, shapeManager.incrementShapeCounter()));
				shapeManager.setSelectedShapeByLastAdded();  // Select the last shape added
            }
			

			
	    ImGui::Separator();

	    if (ImGui::BeginMenu("Import"))
	    {

	        if (ImGui::MenuItem("Import Shape")) {
	            FileImporter fileImporter;  // This creates an instance of the FileImporter class
	            if (fileImporter.importObjFile(shapeManager)) {  // Function to import .obj files
	                shapeManager.setSelectedShapeByLastAdded();
		    }
	        }

	        if (ImGui::MenuItem("Import Curve")) {
	            FileImporter fileImporter;  // This creates an instance of the FileImporter class
	            if (fileImporter.importSwpFile(shapeManager)) {  // Function to import .obj files
		        shapeManager.setSelectedShapeByLastAdded();
		    }
	        }

	        if (ImGui::MenuItem("Import Character")) {
	            FileImporter fileImporter;  // This creates an instance of the FileImporter class
	            if (fileImporter.importCharacterFile(shapeManager)) {  // Function to import .obj files
		        shapeManager.setSelectedShapeByLastAdded();
		    }
	        }

	        ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {

            // Camera view presets
            if (ImGui::MenuItem("View from +X")) {
                cameraPosition = glm::vec3(5.0f, 0.0f, 0.0f);
                cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                theta = 0.0f;                    // Left
                phi = 0.0f;                      // Level
                radius = 5.0f;
                updateCameraPosition();
            }
            if (ImGui::MenuItem("View from -X")) {
                cameraPosition = glm::vec3(-5.0f, 0.0f, 0.0f);
                cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                theta = glm::pi<float>();        // Right
                phi = 0.0f;                      // Level
                radius = 5.0f;
                updateCameraPosition();
            }
            if (ImGui::MenuItem("View from +Y")) {
                cameraPosition = glm::vec3(0.0f, 5.0f, 0.0f);
                cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                theta = glm::pi<float>() / 2.0f;  // Same as default
                phi = glm::half_pi<float>();      // 90 degrees upward
                radius = 5.0f;
                updateCameraPosition();
            }
            if (ImGui::MenuItem("View from -Y")) {
                cameraPosition = glm::vec3(0.0f, -5.0f, 0.0f);
                cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                theta = glm::pi<float>() / 2.0f; // Same as default
                phi = -glm::half_pi<float>();    // 90 degrees downward
                radius = 5.0f;
                updateCameraPosition();
            }
            if (ImGui::MenuItem("View from +Z")) {
                cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
                cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                theta = glm::pi<float>() / 2.0f;  // Default View
                phi = 0.0f;                       // Level
                radius = 5.0f;
                updateCameraPosition();
            }
            if (ImGui::MenuItem("View from -Z")) {
                cameraPosition = glm::vec3(0.0f, 0.0f, -5.0f);
                cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                theta = glm::pi<float>() * 1.5f;  // Opposite of +Z
                phi = 0.0f;
                radius = 5.0f;
                updateCameraPosition();
            }

            ImGui::Separator();

            // Reset to default camera view
            if (ImGui::MenuItem("Reset to Default Camera View")) {
                cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
                cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                theta = glm::pi<float>() / 2.0f;
                phi = 0.0f;
                radius = 5.0f;
                updateCameraPosition();
            }

            ImGui::Separator();

            // Toggle axis visibility with updated label
            std::string axisToggleText = showAxis ? "Toggle Axis Off" : "Toggle Axis On";
            if (ImGui::MenuItem(axisToggleText.c_str(),"T")) {
                showAxis = !showAxis;
            }

            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
    
    // Check if the Esc key was pressed using ImGui
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        exit(0);  // Exit the program if the Esc key is pressed
    }    

    // Check for keyboard shortcut 'T' (toggles axis visibility)
    if (!ImGui::IsAnyItemActive() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        if (ImGui::IsKeyPressed(ImGuiKey_T)) {
            showAxis = !showAxis;
        }
    }

    // Right-side ImGui panel for shape selection and properties
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 280, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(280, io.DisplaySize.y - 20), ImGuiCond_Always);
    ImGui::Begin("Shape Selector and Properties", NULL, ImGuiWindowFlags_NoResize);

    // Shape Selector Panel
    ImGui::Text("Shape Selector");
    for (int i = 0; i < shapeManager.getShapes().size(); i++) {
		Shape* shape = shapeManager.getShapeById(i);
        char buffer[64];
        sprintf(buffer, "[%d] %s (ID: %d)", i + 1, shape->getShapeType().c_str(), shape->getId());
        
        if (ImGui::Selectable(buffer, shapeManager.getShapeById(i) == shapeManager.getSelectedShape())) {
			shapeManager.setSelectedShapeById(shape->getId()); 
        }
		
    }

    ImGui::Separator();

    // Shape Properties Panel
    if (shapeManager.getSelectedShape()) {
        ImGui::Text("Selected Shape ID: %d", shapeManager.getSelectedShape()->getId());

	// Position
        float pos[3] = { shapeManager.getSelectedShape()->getX(), shapeManager.getSelectedShape()->getY(), shapeManager.getSelectedShape()->getZ() };
        if (ImGui::DragFloat3("Position", pos, 0.1f)) {
            shapeManager.getSelectedShape()->setPosition(pos[0], pos[1], pos[2]);
        }

	// Scaling options
	bool useUniformScaling = shapeManager.getSelectedShape()->isUsingUniformScaling();
	if (ImGui::Checkbox("Uniform Scaling", &useUniformScaling)) {
		shapeManager.getSelectedShape()->useUniformScaling(useUniformScaling);
	}

	if (useUniformScaling) {
		float scale = shapeManager.getSelectedShape()->getScale();
		if (ImGui::DragFloat("Uniform Scale", &scale, 0.1f, 0.1f, 10.0f)) {
			shapeManager.getSelectedShape()->setScale(scale);
		}
	} else {
		glm::vec3 nonUniformScale = shapeManager.getSelectedShape()->getNonUniformScale();
		float scale[3] = {nonUniformScale.x, nonUniformScale.y, nonUniformScale.z};
		if (ImGui::DragFloat3("Scale (X, Y, Z)", scale, 0.1f, 0.1f, 10.0f)) {
			shapeManager.getSelectedShape()->setScale(scale[0], scale[1], scale[2]);
		}
	}

	// Rotation
        float rotation[3] = { shapeManager.getSelectedShape()->getAngleX(), shapeManager.getSelectedShape()->getAngleY(), shapeManager.getSelectedShape()->getAngleZ() };
        if (ImGui::DragFloat3("Rotation", rotation, 1.0f, -360.0f, 720.0f)) {
            shapeManager.getSelectedShape()->setRotation(rotation[0], rotation[1], rotation[2]);
        }

        // Color Selection
        static int selectedColorIdx = 0;
        static float customColor[3] = {0.0f, 0.0f, 0.0f};  // Initialize for custom color picker

        if (shapeManager.getSelectedShape()) {
            selectedColorIdx = shapeManager.getSelectedShape()->getColorIndex();  // Sync with shape's current color

            // If it's a custom color, update customColor array with shape's current custom color
            if (selectedColorIdx == 31) {
                const float* currentCustomColor = shapeManager.getSelectedShape()->getCustomColor();
                customColor[0] = currentCustomColor[0];
                customColor[1] = currentCustomColor[1];
                customColor[2] = currentCustomColor[2];
            }
        }


        if (ImGui::BeginCombo("Color", colorPresets[selectedColorIdx].name)) {
            for (int i = 0; i < 32; i++) {
                ImGui::PushID(i);
                ImGui::ColorButton("##color", ImVec4(colorPresets[i].color[0], colorPresets[i].color[1], colorPresets[i].color[2], 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs, ImVec2(20, 20));
                ImGui::SameLine();
                if (ImGui::Selectable(colorPresets[i].name, selectedColorIdx == i)) {
                    selectedColorIdx = i;
                    if (i != 31) {  // Not "Custom"
                        shapeManager.getSelectedShape()->setColor(i);
                    } else {
                        // If "Custom" is selected, preserve the current custom color
                        shapeManager.getSelectedShape()->setColor(i);
                        shapeManager.getSelectedShape()->setCustomColor(customColor[0], customColor[1], customColor[2]);
                    }
                }
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }

        // Custom Color Picker if "Custom" is selected
        if (selectedColorIdx == 31) {
            if (ImGui::ColorEdit3("Custom Color", customColor)) {
                // Update the custom color preset and shape color dynamically
                shapeManager.getSelectedShape()->setColor(31); // Use custom color
                shapeManager.getSelectedShape()->setCustomColor(customColor[0], customColor[1], customColor[2]);
            }
        }


		// For ImportCurves only

		if (ImportCurve* importCurve = dynamic_cast<ImportCurve*>(shapeManager.getSelectedShape())) {
			// Toggle for showing control points
			bool showControlPoints = importCurve->isControlPointsVisible();
			if (ImGui::Checkbox("Show Control Points", &showControlPoints)) {
				importCurve->setControlPointsVisible(showControlPoints);
			}

			// Combo box for curve visibility mode
			const char* curveOptions[] = { "Off", "On", "On with Normals/Tangents/Binormals" };
			int curveVisibilityMode = importCurve->getCurveVisibilityMode();
			ImGui::Text("Curve Visibility");  // This adds the label above the dropdown
			ImGui::SetNextItemWidth(200);  // Adjust the dropdown width as needed			
			if (ImGui::Combo("##curveVisibility", &curveVisibilityMode, curveOptions, IM_ARRAYSIZE(curveOptions))) {
				importCurve->setCurveVisibilityMode(curveVisibilityMode);
			}
			
			// Combo box for surface visibility mode
			const char* surfaceOptions[] = { "Off", "Wireframe with Normals", "Shaded" };
			int surfaceVisibilityMode = importCurve->getSurfaceVisibilityMode();
			ImGui::Text("Surface Visibility");  // This adds the label above the dropdown
			ImGui::SetNextItemWidth(200);  // Adjust the dropdown width as needed			
			if (ImGui::Combo("##surfaceVisibility", &surfaceVisibilityMode, surfaceOptions, IM_ARRAYSIZE(surfaceOptions))) {
				importCurve->setSurfaceVisibilityMode(surfaceVisibilityMode);
			}
						
		}

		// For ImportCharacter only


        ImGui::Separator();
        if (ImportCharacter* importCharacter = dynamic_cast<ImportCharacter*>(shapeManager.getSelectedShape())) {

			ImGui::Separator();
			ImGui::Text("Model Display Mode");

			// Change display mode between mesh and skeletal
			const char* modelModes[] = { "Skeletal Model", "Mesh Model" };
			int currentMode = importCharacter->getDisplayMode();  // Assume this function exists in ImportCharacter

			if (ImGui::Combo("Display Mode", &currentMode, modelModes, IM_ARRAYSIZE(modelModes))) {
				importCharacter->setDisplayMode(static_cast<ImportCharacter::DisplayMode>(currentMode));  // Cast to DisplayMode
			}

            ImGui::Separator();
            ImGui::Text("Joint Rotations (x, y, z)");

			// Get references to vertices and bindVertices
			std::vector<glm::vec3>& currentVertices = importCharacter->getVertices();  // Direct access to vertices from Shape
			const std::vector<glm::vec3>& bindVertices = importCharacter->getBindVertices();
			const std::vector<std::vector<float>>& attachments = importCharacter->getAttachments();



			// Call updateMesh on the skeletal model to update current vertices
			// importCharacter->updateMeshVertices();

			// ImGui control to apply joint transformations and trigger the mesh update
			for (size_t i = 0; i < importCharacter->getSkeletalModel().getJoints().size(); ++i) {

				// Retrieve stored rotation as a mutable array
				glm::vec3 jointRotationVec = importCharacter->getSkeletalModel().getJoints()[i]->getRotation();
				float jointRotation[3] = { jointRotationVec.x, jointRotationVec.y, jointRotationVec.z };

				// ImGui sliders for joint rotation control with 1 degree per unit
				char label[128];
				sprintf(label, "%s", jointName[i].c_str()); // Construct the label using the joint name
				if (ImGui::DragFloat3(label, jointRotation, 1.0f, -360.0f, 720.0f)) {

					jointRotation[0] = (jointRotation[0] < 0) ? 360.0f + fmod(jointRotation[0], 360.0f) : fmod(jointRotation[0], 360.0f);
					jointRotation[1] = (jointRotation[1] < 0) ? 360.0f + fmod(jointRotation[1], 360.0f) : fmod(jointRotation[1], 360.0f);
					jointRotation[2] = (jointRotation[2] < 0) ? 360.0f + fmod(jointRotation[2], 360.0f) : fmod(jointRotation[2], 360.0f);

					// Set joint transform with updated rotations
					importCharacter->getSkeletalModel().setJointTransform(i, jointRotation[0], jointRotation[1], jointRotation[2]);
					importCharacter->getSkeletalModel().updateCurrentJointToWorldTransforms();

				}
			}
        }


        // Add the Delete button
        ImGui::Separator();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
        if (ImGui::Button("Delete", ImVec2(120, 30))) {
            // Delete the selected shape
			shapeManager.deleteShape(shapeManager.getSelectedShape());
        }
        ImGui::PopStyleVar();  // Restore the style

        ImGui::SameLine();  // Place the Reset button on the same line as the Delete button

        // Add the Reset button
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
        if (ImGui::Button("Reset", ImVec2(120, 30))) {

            // Reset the selected shape to default settings
            shapeManager.getSelectedShape()->resetToDefault();

	    ImportCharacter* importChar = dynamic_cast<ImportCharacter*>(shapeManager.getSelectedShape());

	    if (importChar) {
		    importChar->resetPose(); // Call the resetPose method if it's an ImportCharacter
	    }
        }
        ImGui::PopStyleVar();  // Restore the style

    }

    ImGui::End();

    // Handle mouse wheel zoom (scrolling in/out)
    if (!ImGui::IsAnyItemActive() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        if (io.MouseWheel != 0) {
            float zoomSpeed = 0.5f;
            radius -= io.MouseWheel * zoomSpeed;

            // Prevent flipping by ensuring the radius does not reach zero
            radius = glm::max(radius, 0.5f);
            updateCameraPosition();
        }
    }

    // Ensure no UI elements are being interacted with before handling inputs
    if (!ImGui::IsAnyItemActive() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {

        // Left Click + Drag → Pan Camera
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            float panSpeed = 0.01f;
            glm::vec3 right = glm::normalize(glm::cross(cameraTarget - cameraPosition, cameraUp));
            glm::vec3 up = cameraUp;

            cameraTarget -= right * io.MouseDelta.x * panSpeed;
            cameraTarget += up * io.MouseDelta.y * panSpeed;
            updateCameraPosition();
        }

        // Right Click + Drag → Rotate Camera
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
            float rotationSpeed = 0.005f;
            theta -= io.MouseDelta.x * rotationSpeed; // Rotate left/right (Yaw)
            phi -= io.MouseDelta.y * rotationSpeed;   // Rotate up/down (Pitch)

            // Clamp phi to prevent flipping
            phi = glm::clamp(phi, -glm::half_pi<float>() + 0.1f, glm::half_pi<float>() - 0.1f);
            updateCameraPosition();
        }
    }


    // Compute View Matrix Using Matrix Transformations
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    // Compute Projection Matrix
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    // Get the shader program and activate it
    GLuint shaderProgram = getShaderProgram();
    glUseProgram(shaderProgram);

    // Pass matrices to shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup lighting
    setupLighting(shaderProgram);

    // Draw axis only if enabled
    if (showAxis) {
        drawAxis(getShaderProgram());
    }

    // Draw all shapes
    for (Shape* shape : shapeManager.getShapes()) {
        shape->applyTransform(shaderProgram);
        shape->draw(shaderProgram);
    }

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  
    // Disable 
    glUseProgram(0);
}

