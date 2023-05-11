#pragma once

#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"


// Represents a single particle and its state
struct Particle {
    int id;
    glm::vec3 Position, Direction;
    glm::vec3 deviation;
    glm::vec4 Color,midColor,finColor;
    float speed;
    float scale;
    float size;
    float scaleFin;
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
    void Update(float dt, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f,.0f));
    // render all particles
    void Draw(Shader point_shader, Shader quad_shader, glm::mat4 view, glm::mat4 proj);

    void setColor(glm::vec4 init, glm::vec4 mid, glm::vec4 fin);
    glm::vec4 getInitColor();
    glm::vec4 getMidColor();
    glm::vec4 getFinColor();
    void setRGBvariance();

    void setMinMax(glm::vec3 min, glm::vec3 max);
    glm::vec3 getMin();
    glm::vec3 getMax();

    void setMaxParticles(int n);
    int getMaxParticles();
private:
    int iterator[2]{ -1, 1 };
    int partition(std::vector<Particle>& arr, int start, int end);
    void quickSort(std::vector<Particle>& arr, int start, int end);
    // state
    std::vector<Particle> particles;
    //particles options
    glm::vec3 spawn_min=glm::vec3(.0f), spawn_max = glm::vec3(.0f); //area de spawn
    glm::vec4 color_ini = glm::vec4(1.0f), color_mid = glm::vec4(.0f), color_fin = glm::vec4(.5f); //colores
    glm::vec4 color_ini_variance = glm::vec4(.1f), color_mid_variance = glm::vec4(.1f), color_fin_variance = glm::vec4(.1f);; //varianza color
    glm::vec3 direction = glm::vec3(.0f,1.0f,.0f);
    glm::vec3 direction_variance = glm::vec3(.1f);
    glm::vec3 dirDesviation = glm::vec3(0.1f);
    glm::vec3 dirDesviation_var = glm::vec3(.001f);
    float speed = 2.0f;
    float speed_variance = 1.0f;
    float size_ini = 5.0f, size_fin = .0f;
    float size_ini_var = 1.0f, size_fin_var = 1.0f;
    float mass=.1f; //masa de la particula
    float mass_variance = 1.0f;
    float anim_speed=1.0f ;
    float lifetime = 2.0f;
    float lifetime_var = 0.1f;
    
    float size_particle = 3.0f;
    float size_variance = 1.0f;

    
    //menu options
    bool opt_mass = false;
    bool opt_blending = false;
    int max_particles = 10;
    bool opt_point = true;

    // render state
    GLuint Program;
    GLint viewLoc, projLoc, modelLoc;
    GLint colorLoc, sizeLoc, pointLoc;
    int size;
    GLuint pointVAO,pointVBO, quadVAO, quadVBO, boundingVAO, boundingVBO;
    glm::mat4 ident_matrix= glm::rotate(glm::mat4(1.0f), .0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), .0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(.5f));
    // initializes buffer and vertex attributes
    void init();
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle& particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f,.0f));

    glm::vec4 colorVariance(glm::vec4 color, glm::vec4 variance);

    float modNumber(int n, float div);
    
};

#endif