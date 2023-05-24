#pragma once

#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



//class graphic
//{
//public:
//    GLuint Program;
//    graphic();
//    // Constructor generates the shader on the fly
//    graphic(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath)
//    {
//        // 1. Retrieve the vertex/fragment source code from filePath
//        std::string vertexCode;
//        std::string geometryCode;
//        std::string fragmentCode;
//
//        std::ifstream vShaderFile;
//        std::ifstream gShaderFile;
//        std::ifstream fShaderFile;
//        // ensures ifstream objects can throw exceptions:
//        vShaderFile.exceptions(std::ifstream::badbit);
//        if (geometryPath != "")
//            gShaderFile.exceptions(std::ifstream::badbit);
//
//        fShaderFile.exceptions(std::ifstream::badbit);
//        try
//        {
//            // Open files
//            vShaderFile.open(vertexPath);
//            if (geometryPath != "")
//                gShaderFile.open(geometryPath);
//            fShaderFile.open(fragmentPath);
//            std::stringstream vShaderStream, gShaderStream, fShaderStream;
//            // Read file's buffer contents into streams
//            vShaderStream << vShaderFile.rdbuf();
//            if (geometryPath != "")
//                gShaderStream << gShaderFile.rdbuf();
//            fShaderStream << fShaderFile.rdbuf();
//            // close file handlers
//            vShaderFile.close();
//            if (geometryPath != "")
//                gShaderFile.close();
//            fShaderFile.close();
//            // Convert stream into string
//            vertexCode = vShaderStream.str();
//            if (geometryPath != "")
//                geometryCode = gShaderStream.str();
//            fragmentCode = fShaderStream.str();
//        }
//        catch (std::ifstream::failure e)
//        {
//            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
//        }
//        const GLchar* vShaderCode = vertexCode.c_str();
//        const GLchar* gShaderCode = "";
//        if (geometryPath != "")
//            gShaderCode = geometryCode.c_str();
//        const GLchar* fShaderCode = fragmentCode.c_str();
//        // 2. Compile shaders
//        GLuint vertex, geometry, fragment;
//        GLint success;
//        GLchar infoLog[512];
//        // Vertex Shader
//        vertex = glCreateShader(GL_VERTEX_SHADER);
//        glShaderSource(vertex, 1, &vShaderCode, NULL);
//        glCompileShader(vertex);
//        // Print compile errors if any
//        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
//        if (!success)
//        {
//            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//        }
//        //geometry Shader
//        if (geometryPath != "") {
//            geometry = glCreateShader(GL_GEOMETRY_SHADER);
//            glShaderSource(geometry, 1, &gShaderCode, NULL);
//            glCompileShader(geometry);
//            // Print compile errors if any
//            glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
//            if (!success)
//            {
//                glGetShaderInfoLog(geometry, 512, NULL, infoLog);
//                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//            }
//        }
//        // Fragment Shader
//        fragment = glCreateShader(GL_FRAGMENT_SHADER);
//        glShaderSource(fragment, 1, &fShaderCode, NULL);
//        glCompileShader(fragment);
//        // Print compile errors if any
//        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
//        if (!success)
//        {
//            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//        }
//        // Shader Program
//        this->Program = glCreateProgram();
//        glAttachShader(this->Program, vertex);
//        if (geometryPath != "")
//            glAttachShader(this->Program, geometry);
//        glAttachShader(this->Program, fragment);
//        glLinkProgram(this->Program);
//        // Print linking errors if any
//        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
//        if (!success)
//        {
//            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//        }
//        // Delete the shaders as they're linked into our program now and no longer necessery
//        glDeleteShader(vertex);
//        if (geometryPath != "")
//            glDeleteShader(geometry);
//        glDeleteShader(fragment);
//
//    }
//    // Uses the current shader
//    void Use()
//    {
//        glUseProgram(this->Program);
//    }
//};
// Represents a single particle and its state
struct Particle {
    int id;
    glm::vec3 Position, Direction, currentPosition;
    glm::vec3 deviation;
    glm::vec4 Color,midColor,finColor,currentColor;
    float speed;
    float currentScale, scale, scaleFin;
    float size;
    float Life,mLife,tLife;
    float mass;
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    // constructor
    ParticleGenerator();
    // update all particles
    void Update(float dt);
    // render all particles
    void Draw( glm::mat4 view, glm::mat4 proj);

    void setTexture(unsigned char* data, int width, int height, bool active);
    bool getTexture();
    void setColor(glm::vec4 ini, glm::vec4 fin);
    float* getInitColor();
    float* getFinColor();
    void setRGBAvariance(glm::vec4 ini,glm::vec4 fin);
    float* getInitColorVariance();
    float* getFinColorVariance();

    void setMinMax(glm::vec3 min, glm::vec3 max);
    float* getMin();
    float* getMax();
    void setDirection(glm::vec3 dir);
    float* getDirection();
    void setDirectionVar(glm::vec3 dir);
    float* getDirectionVar();
    void setMaxParticles(int n);
    int getMaxParticles();
    void setNewParticles(int n);
    int getNewParticles();
    void setNewParticlesVariance(int n);
    int getNewParticlesVariance();
    void setAnimSpeed(float n);
    void setGravity(bool opt);

    void setLifeTime(float t);
    float getLifeTime();
    void setLifeTimeVar(float t);
    float getLifeTimeVar();
    void setSpeed(float t);
    float getSpeed();
    void setSpeedVar(float t);
    float getSpeedVar();
    void setSize(float s);
    float getSize();
    void setSizeVar(float s);
    float getSizeVar();
    void setScale(float s_ini, float s_fin);
    float getScaleIni();
    float getScaleFin();
    void setScaleVar(float s_ini, float s_fin);
    float getScaleIniVar();
    float getScaleFinVar();
    void setMass(float m);
    float getMass();
    void setMassVar(float m);
    float getMassVar();

    void setOptPoint(bool o);
    bool getOptPoint();
private:
    int iterator[2]{ -1, 1 };
    int partition(std::vector<Particle>& arr, int start, int end);
    void quickSort(std::vector<Particle>& arr, int start, int end);
    // state
    std::vector<Particle> particles;
    //particles options
    glm::vec3 spawn_min=glm::vec3(-1.0f), spawn_max = glm::vec3(1.0f); //area de spawn
    glm::vec4 color_ini = glm::vec4(1.0f), color_fin = glm::vec4(.1f); //colores
    glm::vec4 color_ini_variance = glm::vec4(.1f), color_fin_variance = glm::vec4(.1f);; //varianza color
    glm::vec3 direction = glm::vec3(.0f,1.0f,.0f);
    glm::vec3 direction_variance = glm::vec3(.1f);
    glm::vec3 dirDesviation = glm::vec3(0.1f);
    glm::vec3 dirDesviation_var = glm::vec3(.001f);
    float speed = 1.0f;
    float speed_variance = 1.0f;
    float scale_ini = 10.0f, scale_fin = .1f;
    float scale_ini_var = 1.0f, scale_fin_var = 1.0f;
    float mass=.1f; //masa de la particula
    float mass_variance = 1.0f;
    float anim_speed=1.0f ;
    float lifetime = 2.0f;
    float lifetime_var = 0.1f;
    
    float size_particle = 3.0f;
    float size_variance = 1.0f;
    int new_particles = 2;
    int new_particles_variance = 1;
    //menu options
    bool opt_mass = false;
    bool opt_blending = false;
    bool opt_point = true;
    bool opt_texture = false;
    int max_particles = 10;
    

    // render state

    float secondCounter = .0f;
    GLuint Program;
    GLuint texture;
    GLint viewLoc, projLoc, modelLoc;
    GLint colorLoc, sizeLoc, texturizedLoc;
    int size;
    GLuint pointVAO,pointVBO, quadVAO, quadVBO, boundingVAO, boundingVBO;
    glm::mat4 ident_matrix= glm::rotate(glm::mat4(1.0f), .0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), .0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(.5f));
    GLuint point_shader,quad_shader;
    // initializes buffer and vertex attributes
    void init();
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle& particle);

    glm::vec4 colorVariance(glm::vec4 color, glm::vec4 variance);

    float modNumber(int n, float div);
    
};

#endif