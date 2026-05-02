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
#include "pgr.h"
#include "object.h"
#include "singlemesh.h"
#include "Camera.h"
#include "Skybox.h"
#include "SceneObjectsData.h"
#include "HardcodedObject.h"
#include "Light.h"

#define SCENE_WIDTH  1.0f
#define SCENE_HEIGHT 1.0f
#define SCENE_DEPTH  1.0f

#define FAR_CLIPPING_PLANE 100.f

enum { KEY_LEFT, KEY_RIGHT, KEY_FORWARD, KEY_BACKWARDS, KEY_UP, KEY_DOWN, KEY_SWITCH_CAMERA, KEYS_COUNT };
bool keyMap[KEYS_COUNT];

constexpr int WINDOW_WIDTH = 1080;
constexpr int WINDOW_HEIGHT = 850;
constexpr char WINDOW_TITLE[] = "PGR: dudkolau";

//mouse movement handling
float lastMousePosX = WINDOW_WIDTH/2;
float lastMousePosY = WINDOW_HEIGHT/2;
bool firstMouse = true;

ObjectList objects;

// shared shader programs
ShaderProgram commonShaderProgram;

Camera myCamera;
Skybox mySkybox;
SceneLights sceneLights;


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
	sceneLights.moonLight.ambient = glm::vec3(0.2f, 0.2f, 0.5f);
	sceneLights.moonLight.diffuse = glm::vec3(0.4f, 0.5f, 0.8f);
	sceneLights.moonLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneLights.moonLight.direction = glm::normalize(glm::vec3(1.0f, -1.0f, -0.1f));
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

	commonShaderProgram.initialized = true;
	skyboxShaderProgram.initialized = true;
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

	glUniform3fv(commonShaderProgram.locations.moonLightAmbient, 1,
		glm::value_ptr(sceneLights.moonLight.ambient));

	glUniform3fv(commonShaderProgram.locations.moonLightDiffuse, 1,
		glm::value_ptr(sceneLights.moonLight.diffuse));

	glUniform3fv(commonShaderProgram.locations.moonLightSpecular, 1,
		glm::value_ptr(sceneLights.moonLight.specular));

	glUniform3fv(commonShaderProgram.locations.moonLightDirection, 1,
		glm::value_ptr(sceneLights.moonLight.direction));

	//glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.0f, 0.0f));

	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, projectionMatrix);
	}
}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	// update the application state
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->update(elapsedTime, &sceneRootMatrix);
	}

	if (keyMap[KEY_RIGHT] == true)
		myCamera.Move(-0.05f* glm::cross(myCamera.upVector, myCamera.direction));

	if (keyMap[KEY_LEFT] == true)
		myCamera.Move(0.05f * glm::cross(myCamera.upVector, myCamera.direction));

	if (keyMap[KEY_FORWARD] == true)
		myCamera.Move(myCamera.direction * 0.1f); 

	if (keyMap[KEY_BACKWARDS] == true)
		myCamera.Move(myCamera.direction * -0.1f);

	if (keyMap[KEY_UP] == true)
		myCamera.Move(myCamera.upVector * 0.1f);

	if (keyMap[KEY_DOWN] == true)
		myCamera.Move(myCamera.upVector * -0.1f);

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
		obj->transformObject();

		objects.push_back(obj);
	}

	for (const auto& m : HardCodedObjects) {
		objects.push_back(new HardcodedObject(&commonShaderProgram, &m));
	}

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
		//glutMouseFunc(mouseCb);
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
