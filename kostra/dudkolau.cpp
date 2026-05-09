//----------------------------------------------------------------------------------------
/**
 * \file    skeleton.cpp : This file contains the 'main' function and callbacks.
			Program execution begins and ends there.
 * \author  Jaroslav Sloup, Petr Felkel
 * \date    2022/03/03
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

 /**
  * \brief	\mainpage Documentation of the skeleton for the Computer graphics course on FEE and FIT CTU CZ
  *
  * This is the skeleton of an OpenGL application written in FreeGLUT and PGR framework.
  * It serves as a starter boiler-plate code for a semester project - a simple interactive application.
  * Write your code into the functions prepared. Add your data (models, textures, ...) and logic.
  *
  *
  * Comment your code using the [doxygen](https://www.doxygen.nl/index.html) documenting system style.
  * Create "doxygen" directory, make it current by "cd doxygen", prepare a configuration file with "doxygen -g" and edit the details.
  *
  * Start by renaming of this file from skeleton.cpp to <your_name>.cpp and the project to <your_name>.vcxproj
  *
  * In <your_name>.vcxproj:
  *   - rename skeleton in <RootNamespace>skeleton</RootNamespace> to <your_name>
  *   - replace skeleton.cpp in <ClCompile Include="skeleton.cpp" /> to <your_name>.cpp
  *
  * Start coding and documenting. Generate the documentation by the command "doxygen" in the "doxygen" directory.
  *
  */


  // TODO: tabulka klaves a jeji obsluha keyPressed/keyReleased a timer


#include <iostream>
#include <cctype>
#include "pgr.h"
#include "object.h"
#include "singlemesh.h"
#include "Camera.h"
#include "Skybox.h"
#include "SceneObjectsData.h"
#include "HardcodedObject.h"
#include "FlatShadedObject.h"
#include "ColorChangingCrystals.h"
#include "Light.h"
#include "Billboard.h"
#include "Parameters.h"

//mouse movement handling
float lastMousePosX = WINDOW_WIDTH/2;
float lastMousePosY = WINDOW_HEIGHT/2;
bool firstMouse = true;
bool showCursor = true;

ObjectList objects;

// shared shader programs
ShaderProgram commonShaderProgram;
ShaderProgram billboardShaderProgram;

Camera myCamera;
Skybox mySkybox;
SceneLights sceneLights;

Billboard* fireflyGlow;
Billboard* cursor;

Crystal* crystals[crystalsNum];


/*
THE CORE ENGINE LOOP

1. Setup
* create window
* load shaders
* create objects

2. Main loop
* Handled by GLUT, not while(true)

3. Delegation
actual rendering -> object->draw(...)
updates -> object->update(...)
*/
void setUpLights() {
	//Moon Light
	sceneLights.moonLight.ambient = moonLightAmbient;
	sceneLights.moonLight.diffuse = moonLightDiffuse;
	sceneLights.moonLight.specular = moonLightSpecular;
	sceneLights.moonLight.direction = moonLightDirection;

	//Firefly
	sceneLights.firefly.position = fireflyPosition; //
	sceneLights.firefly.ambient = fireflyAmbient;
	sceneLights.firefly.diffuse = fireflyDiffuse;
	sceneLights.firefly.specular = fireflySpecular;
	sceneLights.firefly.constant = fireflyConstant;
	sceneLights.firefly.linear = fireflyLinear;
	sceneLights.firefly.quadratic = fireflyQuadratic;

	//Lantern
	sceneLights.lantern.ambient = lanternAmbient;
	sceneLights.lantern.diffuse = lanternDiffuse;
	sceneLights.lantern.position = lanternPosition;
	sceneLights.lantern.specular = lanternSpecular;
	sceneLights.lantern.constant = lanternConstant;
	sceneLights.lantern.linear = lanternLinear;
	sceneLights.lantern.quadratic = lanternQuadratic;

	//Lamp
	sceneLights.lamp_1.ambient = lamp_1Ambient;
	sceneLights.lamp_1.diffuse = lamp_1Diffuse;       // Cold LED white/blue
	sceneLights.lamp_1.specular = lamp_1Specular;      // High glare
	sceneLights.lamp_1.position = lamp_1Position;      // Hardcoded world position
	sceneLights.lamp_1.direction = lamp_1Direction;    // Hardcoded pointing forward (-Z)
	sceneLights.lamp_1.constant = lamp_1Constant;
	sceneLights.lamp_1.linear = lamp_1Linear;                      // Good for ~50 meters
	sceneLights.lamp_1.quadratic = lamp_1Quadratic;
	sceneLights.lamp_1.spotCosCutOff = lamp_1SpotCosCutOff;               // ~15 degree cone
	sceneLights.lamp_1.spotExponent = lamp_1SpotExponent;                 // Soft edge
	
}

// -----------------------  OpenGL stuff ---------------------------------

/**
 * \brief Load and compile shader programs. Get attribute locations.
 */
void loadShaderPrograms()
{
	GLuint shaders[] = {
	  pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/simple.vert"),
	  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/simple.frag"),
	  0
	};

	commonShaderProgram.program = pgr::createProgram(shaders);
	commonShaderProgram.locations.position = glGetAttribLocation(commonShaderProgram.program, "position");
	commonShaderProgram.locations.texCoord = glGetAttribLocation(commonShaderProgram.program, "texCoord");
	commonShaderProgram.locations.color = glGetAttribLocation(commonShaderProgram.program, "color");
	commonShaderProgram.locations.normal = glGetAttribLocation(commonShaderProgram.program, "normal");

	// other attributes and uniforms
	commonShaderProgram.locations.texSampler   = glGetUniformLocation(commonShaderProgram.program, "texSampler");
	commonShaderProgram.locations.PVMmatrix    = glGetUniformLocation(commonShaderProgram.program, "PVMmatrix");
	commonShaderProgram.locations.hasTexture   = glGetUniformLocation(commonShaderProgram.program, "hasTexture");
	commonShaderProgram.locations.Mmatrix	   = glGetUniformLocation(commonShaderProgram.program, "Mmatrix");
	commonShaderProgram.locations.normalMatrix = glGetUniformLocation(commonShaderProgram.program, "normalMatrix");
	commonShaderProgram.locations.Vmatrix	  = glGetUniformLocation(commonShaderProgram.program, "Vmatrix");
	commonShaderProgram.locations.UVMatrix	  = glGetUniformLocation(commonShaderProgram.program, "UVMatrix");
	commonShaderProgram.locations.alpha	  = glGetUniformLocation(commonShaderProgram.program, "alpha");
	//materials
	commonShaderProgram.locations.matAmbient   = glGetUniformLocation(commonShaderProgram.program, "material.ambient");
	commonShaderProgram.locations.matDiffuse   = glGetUniformLocation(commonShaderProgram.program, "material.diffuse");
	commonShaderProgram.locations.matSpecular  = glGetUniformLocation(commonShaderProgram.program, "material.specular");
	commonShaderProgram.locations.matShininess = glGetUniformLocation(commonShaderProgram.program, "material.shininess");
	//lights
	commonShaderProgram.locations.moonLightAmbient = glGetUniformLocation(commonShaderProgram.program, "moonLight.ambient");
	commonShaderProgram.locations.moonLightDiffuse = glGetUniformLocation(commonShaderProgram.program, "moonLight.diffuse");
	commonShaderProgram.locations.moonLightSpecular = glGetUniformLocation(commonShaderProgram.program, "moonLight.specular");
	commonShaderProgram.locations.moonLightDirection = glGetUniformLocation(commonShaderProgram.program, "moonLight.direction");

	commonShaderProgram.locations.fireflyAmbient = glGetUniformLocation(commonShaderProgram.program, "firefly.ambient");
	commonShaderProgram.locations.fireflyDiffuse = glGetUniformLocation(commonShaderProgram.program, "firefly.diffuse");
	commonShaderProgram.locations.fireflyPosition = glGetUniformLocation(commonShaderProgram.program, "firefly.position");
	commonShaderProgram.locations.fireflySpecular = glGetUniformLocation(commonShaderProgram.program, "firefly.specular");
	commonShaderProgram.locations.fireflyConstant = glGetUniformLocation(commonShaderProgram.program, "firefly.constant");
	commonShaderProgram.locations.fireflyLinear = glGetUniformLocation(commonShaderProgram.program, "firefly.linear");
	commonShaderProgram.locations.fireflyQuadratic = glGetUniformLocation(commonShaderProgram.program, "firefly.quadratic");

	commonShaderProgram.locations.lanternAmbient = glGetUniformLocation(commonShaderProgram.program, "lantern.ambient");
	commonShaderProgram.locations.lanternDiffuse = glGetUniformLocation(commonShaderProgram.program, "lantern.diffuse");
	commonShaderProgram.locations.lanternPosition = glGetUniformLocation(commonShaderProgram.program, "lantern.position");
	commonShaderProgram.locations.lanternSpecular = glGetUniformLocation(commonShaderProgram.program, "lantern.specular");
	commonShaderProgram.locations.lanternConstant = glGetUniformLocation(commonShaderProgram.program, "lantern.constant");
	commonShaderProgram.locations.lanternLinear = glGetUniformLocation(commonShaderProgram.program, "lantern.linear");
	commonShaderProgram.locations.lanternQuadratic = glGetUniformLocation(commonShaderProgram.program, "lantern.quadratic");

	commonShaderProgram.locations.lamp1Ambient = glGetUniformLocation(commonShaderProgram.program, "lamp_1.ambient");
	commonShaderProgram.locations.lamp1Diffuse = glGetUniformLocation(commonShaderProgram.program, "lamp_1.diffuse");
	commonShaderProgram.locations.lamp1Specular = glGetUniformLocation(commonShaderProgram.program, "lamp_1.specular");
	commonShaderProgram.locations.lamp1Position = glGetUniformLocation(commonShaderProgram.program, "lamp_1.position");
	commonShaderProgram.locations.lamp1Direction = glGetUniformLocation(commonShaderProgram.program, "lamp_1.direction");
	commonShaderProgram.locations.lamp1Constant = glGetUniformLocation(commonShaderProgram.program, "lamp_1.constant");
	commonShaderProgram.locations.lamp1Linear = glGetUniformLocation(commonShaderProgram.program, "lamp_1.linear");
	commonShaderProgram.locations.lamp1Quadratic = glGetUniformLocation(commonShaderProgram.program, "lamp_1.quadratic");
	commonShaderProgram.locations.lamp1SpotCosCutOff = glGetUniformLocation(commonShaderProgram.program, "lamp_1.spotCosCutOff");
	commonShaderProgram.locations.lamp1SpotExponent = glGetUniformLocation(commonShaderProgram.program, "lamp_1.spotExponent");


	assert(commonShaderProgram.locations.PVMmatrix != -1);
	assert(commonShaderProgram.locations.position != -1);
	// ...

	//SKYBOX SHADERS
    // create the program with two shaders

	skyboxShaderProgram.program = pgr::createProgram({ 
		pgr::createShaderFromSource(GL_VERTEX_SHADER, skyboxVertexShaderSrc) , 
		pgr::createShaderFromSource(GL_FRAGMENT_SHADER, skyboxFragmentShaderSrc) });

	skyboxShaderProgram.positionLocation =
		glGetAttribLocation(skyboxShaderProgram.program, "position");

	skyboxShaderProgram.skyboxSamplerLocation =
		glGetUniformLocation(skyboxShaderProgram.program, "skyboxSampler");

	skyboxShaderProgram.viewNoTranslationLocation =
		glGetUniformLocation(skyboxShaderProgram.program, "viewNoTranslation");

	skyboxShaderProgram.projectionMatrixLocation =
		glGetUniformLocation(skyboxShaderProgram.program, "projection");

	if (!skyboxShaderProgram.program) {
		std::cout << "SKY BOX SHADER COMPILATION FAILED!\N" << std::endl;
	}

	//BILLBOARD SHADERS ---
	GLuint bbShaders[] = {
	  pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/billboard.vert"),
	  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/billboard.frag"),
	  0
	};

	billboardShaderProgram.program = pgr::createProgram(bbShaders);
	billboardShaderProgram.locations.position = glGetAttribLocation(billboardShaderProgram.program, "position");
	billboardShaderProgram.locations.texCoord = glGetAttribLocation(billboardShaderProgram.program, "texCoord");

	//uniforms
	billboardShaderProgram.locations.PVMmatrix = glGetUniformLocation(billboardShaderProgram.program, "PVMmatrix");
	billboardShaderProgram.locations.texSampler = glGetUniformLocation(billboardShaderProgram.program, "texSampler");
	// -----------------------------

	commonShaderProgram.initialized = true;
	skyboxShaderProgram.initialized = true;
	billboardShaderProgram.initialized = true;

	//set static uniforms, taht do not change:
	glUseProgram(commonShaderProgram.program);

	//lights ---
	//moonlight
	glUniform3fv(commonShaderProgram.locations.moonLightAmbient, 1, glm::value_ptr(sceneLights.moonLight.ambient));
	glUniform3fv(commonShaderProgram.locations.moonLightDiffuse, 1, glm::value_ptr(sceneLights.moonLight.diffuse));
	glUniform3fv(commonShaderProgram.locations.moonLightSpecular, 1, glm::value_ptr(sceneLights.moonLight.specular));
	glUniform3fv(commonShaderProgram.locations.moonLightDirection, 1, glm::value_ptr(sceneLights.moonLight.direction));

	//firefly
	glUniform3fv(commonShaderProgram.locations.fireflyAmbient, 1, glm::value_ptr(sceneLights.firefly.ambient));
	glUniform3fv(commonShaderProgram.locations.fireflyDiffuse, 1, glm::value_ptr(sceneLights.firefly.diffuse));
	glUniform3fv(commonShaderProgram.locations.fireflySpecular, 1, glm::value_ptr(sceneLights.firefly.specular));
	glUniform3fv(commonShaderProgram.locations.fireflyPosition, 1, glm::value_ptr(sceneLights.firefly.position));
	glUniform1f(commonShaderProgram.locations.fireflyConstant, sceneLights.firefly.constant);
	glUniform1f(commonShaderProgram.locations.fireflyLinear, sceneLights.firefly.linear);
	glUniform1f(commonShaderProgram.locations.fireflyQuadratic,  sceneLights.firefly.quadratic);

	//lantern
	glUniform3fv(commonShaderProgram.locations.lanternAmbient, 1, glm::value_ptr(sceneLights.lantern.ambient));
	glUniform3fv(commonShaderProgram.locations.lanternDiffuse, 1, glm::value_ptr(sceneLights.lantern.diffuse));
	glUniform3fv(commonShaderProgram.locations.lanternSpecular, 1, glm::value_ptr(sceneLights.lantern.specular));
	glUniform3fv(commonShaderProgram.locations.lanternPosition, 1, glm::value_ptr(sceneLights.lantern.position));
	glUniform1f(commonShaderProgram.locations.lanternConstant, sceneLights.lantern.constant);
	glUniform1f(commonShaderProgram.locations.lanternLinear, sceneLights.lantern.linear);
	glUniform1f(commonShaderProgram.locations.lanternQuadratic,  sceneLights.lantern.quadratic);

	//lamp 1
	glUniform3fv(commonShaderProgram.locations.lamp1Ambient, 1, glm::value_ptr(sceneLights.lamp_1.ambient));
	glUniform3fv(commonShaderProgram.locations.lamp1Diffuse, 1, glm::value_ptr(sceneLights.lamp_1.diffuse));
	glUniform3fv(commonShaderProgram.locations.lamp1Specular, 1, glm::value_ptr(sceneLights.lamp_1.specular));
	glUniform3fv(commonShaderProgram.locations.lamp1Position, 1, glm::value_ptr(sceneLights.lamp_1.position));
	glUniform3fv(commonShaderProgram.locations.lamp1Direction, 1, glm::value_ptr(sceneLights.lamp_1.direction));
	glUniform1f(commonShaderProgram.locations.lamp1Constant, sceneLights.lamp_1.constant);
	glUniform1f(commonShaderProgram.locations.lamp1Linear, sceneLights.lamp_1.linear);
	glUniform1f(commonShaderProgram.locations.lamp1Quadratic, sceneLights.lamp_1.quadratic);
	glUniform1f(commonShaderProgram.locations.lamp1SpotExponent, sceneLights.lamp_1.spotExponent);
	glUniform1f(commonShaderProgram.locations.lamp1SpotCosCutOff, sceneLights.lamp_1.spotCosCutOff);
	
}

/**
 * \brief Delete all shader program objects.
 */
void cleanupShaderPrograms(void) {

	pgr::deleteProgramAndShaders(commonShaderProgram.program);
	//TODO pgr::deleteProgramAndShaders(skyboxFarPlaneShaderProgram.program);
}

/**
 * \brief Draw all scene objects.
 */
void drawScene(void)
{
	// setup parallel projection
	const glm::mat4 orthoProjectionMatrix = glm::ortho(
		-SCENE_WIDTH, SCENE_WIDTH,
		-SCENE_HEIGHT, SCENE_HEIGHT,
		-10.0f * SCENE_DEPTH, 10.0f * SCENE_DEPTH
	);
	
	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(50.0f), 
		WINDOW_WIDTH / (float)WINDOW_HEIGHT, 
		0.1f, 
		FAR_CLIPPING_PLANE
	);

	glm::vec3 cameraCenter = myCamera.direction + myCamera.position;

	glm::mat4 viewMatrix = glm::lookAt(
		myCamera.position,
		cameraCenter,
		myCamera.upVector
	);

	//draw skybox
	glDepthMask(GL_FALSE);
	mySkybox.draw(viewMatrix, projectionMatrix);
	glDepthMask(GL_TRUE);


	//TODO cant i set this in load shaders???
	glUseProgram(commonShaderProgram.program);

	glUniformMatrix4fv(commonShaderProgram.locations.Vmatrix, 1, GL_FALSE,
		glm::value_ptr(viewMatrix));

	//glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.0f, 0.0f));

	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, projectionMatrix);
	}

	glClearStencil(0);
	// enable stencil test
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF); //why???
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	for (int i = 0; i < crystalsNum; i++) {
		Crystal * obj = crystals[i];
		if (obj != nullptr) {
			glStencilFunc(GL_ALWAYS, i + 1, -1); //0 is reserved for blank stencil buffer
			obj->crystal->draw(viewMatrix, projectionMatrix);
		}
	}

	// disable stencil test
	glDisable(GL_STENCIL_TEST);
}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// draw the window contents (scene objects)
	drawScene();

	glutSwapBuffers();
}

/**
 * \brief Window was reshaped.
 * \param newWidth New window width
 * \param newHeight New window height
 */
void reshapeCb(int newWidth, int newHeight) {
	// TODO: Take new window size and update the application state,
	// window and projection.

	// glViewport(...);
};




// -----------------------  Keyboard ---------------------------------
#pragma region KeyboardHandling

/**
 * \brief Handle the key pressed event.
 * Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
 * parameter, which is an ASCII character. It's often a good idea to have the escape key (ASCII value 27)
 * to call glutLeaveMainLoop() to exit the program.
 * \param keyPressed ASCII code of the key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardCb(unsigned char keyPressed, int mouseX, int mouseY) {
	//pressing CTRL at the same time causes the value of keyPressed to change
	//NOTE standart Ctrl + '' combinations now dont work!!
	if (keyPressed >= 1 && keyPressed <= 26) {
		keyPressed += 96;
		
	}

	unsigned char lowerKey = std::tolower(keyPressed);
	keyPressed = lowerKey;

	switch (keyPressed) {
	case 27:
		glutLeaveMainLoop();
		exit(EXIT_SUCCESS);
	case ('d'):
		keyMap[KEY_RIGHT] = true;
		//std::cout << myCamera.position.x << std::endl; 
		break;
	case ('a'):
		keyMap[KEY_LEFT] = true;
		break;
	case ('w'):
		keyMap[KEY_FORWARD] = true;
		//std::cout << myCamera.position.z << std::endl;
		break;
	case ('s'):
		keyMap[KEY_BACKWARDS] = true;
		break;
	case ('c'):
		if (!keyMap[KEY_SWITCH_CAMERA]) {
			keyMap[KEY_SWITCH_CAMERA] = true;
			myCamera.switchMode();
		}
		break;
	default:
		;
	}
}

// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
/**
 * \brief Handle the key released event.
 * \param keyReleased ASCII code of the released key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardUpCb(unsigned char keyReleased, int mouseX, int mouseY) {
	//pressing CTRL at the same time causes the value of keyPressed to change
	//NOTE standart Ctrl + '' combinations now dont work!!
	if (keyReleased >= 1 && keyReleased <= 26) {
		keyReleased += 96;

	}

	unsigned char lowerKey = std::tolower(keyReleased);
	keyReleased = lowerKey;

	switch (keyReleased) {
	case ('d') :
		keyMap[KEY_RIGHT] = false;
		break;
	case ('a'):
		keyMap[KEY_LEFT] = false;
		break;
	case ('w'):
		keyMap[KEY_FORWARD] = false;
		break;
	case ('s'):
		keyMap[KEY_BACKWARDS] = false;
		break;
	case ('c'):
		keyMap[KEY_SWITCH_CAMERA] = false;
		break;
	default:
		;
	}
}


//
/**
 * \brief Handle the non-ASCII key pressed event (such as arrows or F1).
 *  The special keyboard callback is triggered when keyboard function (Fx) or directional
 *  keys are pressed.
 * \param specKeyPressed int value of a predefined glut constant such as GLUT_KEY_UP
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void specialKeyboardCb(int specKeyPressed, int mouseX, int mouseY) {
	switch (specKeyPressed) {
	case GLUT_KEY_CTRL_L :
		keyMap[KEY_DOWN] = true;
		break;
	case GLUT_KEY_SHIFT_L: 
		keyMap[KEY_UP] = true;
		break;
	case GLUT_KEY_F2 :
		if (!keyMap[KEY_GET_STATUS]) {
			keyMap[KEY_GET_STATUS] = true;
			std::cout << "Current status: feeling good!" << std::endl;
		}
		break;
	default:
		;
	}
}

void specialKeyboardUpCb(int specKeyReleased, int mouseX, int mouseY) {
	switch (specKeyReleased) {
	case GLUT_KEY_CTRL_L :
		keyMap[KEY_DOWN] = false;
		break;
	case GLUT_KEY_SHIFT_L: 
		keyMap[KEY_UP] = false;
		break;
	case GLUT_KEY_F2:
		keyMap[KEY_GET_STATUS] = false;
		break;
	default:
		;
	}
} // key released

// -----------------------  Mouse ---------------------------------
// three events - mouse click, mouse drag, and mouse move with no button pressed

// 
/**
 * \brief React to mouse button press and release (mouse click).
 * When the user presses and releases mouse buttons in the window, each press
 * and each release generates a mouse callback (including release after dragging).
 *
 * \param buttonPressed button code (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON)
 * \param buttonState GLUT_DOWN when pressed, GLUT_UP when released
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseCb(int buttonPressed, int buttonState, int mouseX, int mouseY) {
	if (buttonPressed == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN) {
		unsigned char clickedID = 0;

		int xPos = mouseX;
		int yPos = WINDOW_HEIGHT - mouseY;

		glReadPixels(
			xPos,				// X coordinate of mouse position on screen
			yPos,				// Y coordinate of mouse position on screen
			1, 1,               // width and heights of what we are picking (just 1 pixel)
			GL_STENCIL_INDEX,
			GL_UNSIGNED_BYTE,  
			&clickedID          //save answer
		);

		if (clickedID != 0) {
			changeColor(crystals[clickedID-1]);
		}
	}
			
}

/**
 * \brief Handle mouse dragging (mouse move with any button pressed).
 *        This event follows the glutMouseFunc(mouseCb) event.
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseMotionCb(int mouseX, int mouseY) {
}

/**
 * \brief Handle mouse movement over the window (with no button pressed).
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void passiveMouseMotionCb(int mouseX, int mouseY) {
	//most of this code was written based on https://learnopengl.com/Getting-Started/Camera
	// mouse hovering over window
	glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	if (myCamera.currMode != FREE_LOOK) {
		return;
	}

	float xoffset = mouseX - lastMousePosX;
	float yoffset = mouseY - lastMousePosY;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//TODO maybe refactor this to be a camera function

	myCamera.yaw += xoffset;
	myCamera.pitch -= yoffset;

	if (myCamera.pitch > 89.0f)
		myCamera.pitch = 89.0f;
	if (myCamera.pitch < -89.0f)
		myCamera.pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(myCamera.yaw)) * cos(glm::radians(myCamera.pitch));
	direction.y = sin(glm::radians(myCamera.pitch));
	direction.z = sin(glm::radians(myCamera.yaw)) * cos(glm::radians(myCamera.pitch));
	myCamera.direction = glm::normalize(direction);

	// set mouse pointer to the window center
	glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	glutPostRedisplay();
	

	
	
}

void processInput() {
	glm::vec3 moveVector(0.0f);

	if (keyMap[KEY_RIGHT] == true) {
		moveVector += (-1.0f * glm::cross(myCamera.upVector, myCamera.direction));
	}
	if (keyMap[KEY_LEFT] == true) {
		moveVector += (glm::cross(myCamera.upVector, myCamera.direction));
	}
	if (keyMap[KEY_FORWARD] == true) {
		//std::cout << myCamera.position.x << " " << myCamera.position.y << " " << myCamera.position.z << std::endl;
		moveVector += (myCamera.direction);
	}
	if (keyMap[KEY_BACKWARDS] == true) {
		moveVector += (myCamera.direction * (-1.0f));
	}
	if (keyMap[KEY_UP] == true) {
		moveVector += (myCamera.upVector);
	}
	if (keyMap[KEY_DOWN] == true) {
		moveVector += (myCamera.upVector * (-1.0f) );
	}

	if (glm::length(moveVector) != 0.0f) {
		myCamera.Move(glm::normalize(moveVector));
	}
}

#pragma endregion


// -----------------------  Timer ---------------------------------

/**
 * \brief Callback responsible for the scene update.
 * Physics, animation, movement...
 */
void timerCb(int)
{
#ifndef SKELETON // @task_1_0
	const glm::mat4 sceneRootMatrix = glm::mat4(1.0f);

	float elapsedTime = 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // milliseconds => seconds

	//BILLBOARDS 
	//Update the billboard to always face the camera
	if (fireflyGlow != nullptr) {
		glm::vec3 localFireflyOffset = glm::vec3(0.0f, 0.05f, 0.1f); //local offset of the light in firefly coord system
		fireflyGlow->setPosition(sceneLights.firefly.position - localFireflyOffset);
		glm::vec3 directionToCamera = myCamera.position - (sceneLights.firefly.position -localFireflyOffset);
		fireflyGlow->setDirection(directionToCamera);
		fireflyGlow->transformObject();
	}

	//OBJECTS UPDATE --------
	// update the application state
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->update(elapsedTime, &sceneRootMatrix);
	}

	for (int i = 0; i < crystalsNum; i++) {
		if (crystals[i] != nullptr) {
			FlatShadedObject* obj = crystals[i]->crystal;
			if (obj != nullptr) {
				obj->update(elapsedTime, &sceneRootMatrix);
			}
		}
		
	}

	//PROCESS INPUT --------
	processInput();

#endif // task_1_0

	// and plan a new event
	glutTimerFunc(33, timerCb, 0);

	// create display event
	glutPostRedisplay();
}


// -----------------------  Application ---------------------------------

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	// - all programs (shaders), buffers, textures, ...
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	setUpLights();

	loadShaderPrograms();

	mySkybox.init();

	//ADD ALL OBJECTS TO SCENE--------------------------------------------------------
	//objects.push_back(new HardcodedObject(&commonShaderProgram));
	for (auto m : myModels) {
		auto obj = new SingleMesh(m.obj_address, m.texture_address, &commonShaderProgram);

		//setup object
		obj->setScale(m.scale);
		obj->setStartPosition(m.position);
		obj->setDirection(m.direction);
		obj->setIsDynamic(m.isDynamic);
		obj->setHasAnimatedTexture(m.hasAnimatedTexture);

		if (m.name == "firefly") {
			obj->setModelRotationOffset(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (m.name == "mine") {
			obj->setModelRotationOffset(-125.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (m.name == "crocus") {
			obj->setModelRotationOffset(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		obj->transformObject();

		objects.push_back(obj);
	}

	for (const auto& m : HardCodedObjects) {
		objects.push_back(new HardcodedObject(&commonShaderProgram, &m));
	}

	for (const auto& m : FlatShadedObjects) {
		if (m.vertices != crystal_vertices) {
			objects.push_back(new FlatShadedObject(&commonShaderProgram, &m));
		}

		
	}

	unsigned int i = 0;
	for (const auto& m : FlatShadedObjects) {
		if (m.vertices == crystal_vertices) {
			crystals[i] = new Crystal;
			crystals[i]->crystal = new FlatShadedObject(&commonShaderProgram, &m);
			crystals[i]->id = i;
			crystals[i]->currColor = i%COLOR_COUNT;
			std::cout << i%COLOR_COUNT << std::endl;
			setColor(crystals[i]->crystal, i%COLOR_COUNT);
			i++;
		}
	}

	// Create the glow effect (no .obj file needed!)
	fireflyGlow = new Billboard("resources/glow-.png", &billboardShaderProgram);
	fireflyGlow->setScale(0.7f);
	fireflyGlow->setStartPosition(sceneLights.firefly.position);
	fireflyGlow->setModelRotationOffset(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	fireflyGlow->transformObject();

	objects.push_back(fireflyGlow);

	//initialize keyboard map
	for (int i = 0; i < KEYS_COUNT; i++)
		keyMap[i] = false;

	// init your Application
	// - setup the initial application state
}

/**
 * \brief Delete all OpenGL objects and application data.
 */
void finalizeApplication(void) {

	// cleanUpObjects();

	// delete buffers
	// cleanupModels();

	// delete shaders
	cleanupShaderPrograms();
}


/**
 * \brief Entry point of the application.
 * \param argc number of command line arguments
 * \param argv array with argument strings
 * \return 0 if OK, <> elsewhere
 */
int main(int argc, char** argv) {


	// initialize the GLUT library (windowing system)
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// for each window
	{
		//   initial window size + title
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutCreateWindow(WINDOW_TITLE);

		// callbacks - use only those you need
		glutDisplayFunc(displayCb);
		glutReshapeFunc(reshapeCb);
		glutKeyboardFunc(keyboardCb);
		glutKeyboardUpFunc(keyboardUpCb);
		glutSpecialFunc(specialKeyboardCb);     // key pressed
		glutSpecialUpFunc(specialKeyboardUpCb); // key released
		glutPassiveMotionFunc(passiveMouseMotionCb);
		glutMouseFunc(mouseCb);
		//glutMotionFunc(mouseMotionCb);

		glutSetCursor(GLUT_CURSOR_NONE);

#ifndef SKELETON // @task_1_0
		glutTimerFunc(33, timerCb, 0);
#else
		// glutTimerFunc(33, timerCb, 0);
#endif // task_1_0

	}
	// end for each window 

	// initialize pgr-framework (GL, DevIl, etc.)
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	// init your stuff - shaders & program, buffers, locations, state of the application
	initApplication();

	// handle window close by the user
	glutCloseFunc(finalizeApplication);

	//MAIN LOOP----------------------------------------------------------
	// Infinite loop handling the events
	glutMainLoop(); //calls displaz and update (timerFc)

	// code after glutLeaveMainLoop()
	// cleanup

	return EXIT_SUCCESS;
}
