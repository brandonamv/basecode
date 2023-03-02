#pragma once

#define GLM_FORCE_RADIANS// force everything in radian

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>


#include <GL/glew.h>

// pueden utilizar este después
// #include <GL/glad.h>

// GLFW
#include <glfw3.h>

// GLM Mathematics

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class subObj {
public:
    int size;

    // data =  x,y,z,nx,ny,nz,x,y,z,nx,ny,nz, .... 
    subObj(GLfloat* data, int size, std::string image);

    ~subObj();
    bool getTexture();
    void draw();
    
private:
    GLuint VBO, containerVAO;
    GLuint texture;
    bool texturized;

    void bind() {
        glBindVertexArray(containerVAO);
    }

};
