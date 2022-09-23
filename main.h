#pragma once

#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>

// pueden utilizar este después
// #include <GL/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#define GLM_FORCE_RADIANS// force everything in radian
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"

#include <tinyfiledialogs/tinyfiledialogs.h>
using namespace std;

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

GLfloat zDist = -3.0f;
GLfloat angle = 0.0f;

//funciones para manejo de archivos
char const* lTheSaveFileName;
char const* lTheOpenFileName;
char const* lFilterPatterns[2] = { "*.obj" };
FILE* lIn;
vector<string> text;

//funciones para cargas de objetos
vector< unsigned int > vertexIndices, uvIndices, normalIndices;
vector< glm::vec3 > temp_vertices;
vector< glm::vec2 > temp_uvs;
vector< glm::vec3 > temp_normals;
vector< glm::vec3 > out_vertices;
vector < glm::vec2 > out_uvs;
vector < glm::vec3 > out_normals;
vector<GLfloat> objVertex;

int nFaces=0;

class My3DObject{
public:
    int size;

    // data =  x,y,z,nx,ny,nz,x,y,z,nx,ny,nz, .... 
    My3DObject(GLfloat* data, int size){
        this->size = size;
        // First, set the container's VAO (and VBO)
        glGenVertexArrays(1, &containerVAO);
        glBindVertexArray(containerVAO);

        glGenBuffers(1, &VBO);
        // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // Color attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    }

    void draw(){
        bind();
        glDrawArrays(GL_TRIANGLES, 0, size);
    }

private:
    GLuint VBO, containerVAO;

    void bind(){
        glBindVertexArray(containerVAO);
    }

};


vector<string> split(string s, string del);

void loadObj();

string openFile();

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void do_movement(GLfloat delta);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main();
