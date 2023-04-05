#pragma once

#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"


// Represents a single particle and its state
struct Particle {
    int id;
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float scale;
    float Life,tLife;
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    // constructor
    ParticleGenerator(Shader shader, unsigned int amount);
    // update all particles
    void Update(float dt, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f,.0f));
    // render all particles
    void Draw(glm::mat4 view, glm::mat4 proj);
private:

    int partition(std::vector<Particle>& arr, int start, int end);
    void quickSort(std::vector<Particle>& arr, int start, int end);
    // state
    std::vector<Particle> particles;
    unsigned int amount;
    // render state
    GLint viewLoc, projLoc, modelLoc;
    GLint offsetLoc, colorLoc;
    Shader shader;
    int size;
    GLuint pointVAO,pointVBO, quadVAO, quadVBO;
    // initializes buffer and vertex attributes
    void init();
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle& particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f,.0f));
};

#endif