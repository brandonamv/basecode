#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
// pueden utilizar este despu?s
// #include <GL/glad.h>
#include <GL/glew.h>

// pueden utilizar este despu?s
// #include <GL/glad.h>

// GLFW
#include <glfw3.h>

// GLM Mathematics
#define GLM_FORCE_RADIANS// force everything in radian
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"

#include <tinyfiledialogs/tinyfiledialogs.h>
using namespace std;

#include <obj.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <camera.h>
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseClick(GLFWwindow* window, int button, int action, int mods);
void do_movement(GLfloat delta);
string openFile();

// Window dimensions
const GLuint WIDTH = 1400, HEIGHT = 1000;

// Camera
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


//funciones para manejo de archivos
char const* lTheSaveFileName;
char const* lTheOpenFileName;
char const* lFilterPatterns[2] = { "*.obj" };
FILE* lIn;

//scene atributes
float backgroundColor[4] = {0.0f, 1.0f, 1.0f,1.0f};


//obj atributes
vector<obj*> objects;
obj* actual=nullptr;



bool firstMouse = false;
bool rotation=false, moving=false;
double xini, yini;

camera *cam;

int main();


