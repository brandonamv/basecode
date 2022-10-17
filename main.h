#pragma once



#include <GL/glew.h>

// pueden utilizar este después
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

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement(GLfloat delta);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

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

string openFile();

vector<obj*> objects;

bool firstMouse = true;


void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void do_movement(GLfloat delta);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main();
