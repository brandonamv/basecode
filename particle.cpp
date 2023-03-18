#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle.h"
#include <vector>
ParticleGenerator::ParticleGenerator(Shader shader, unsigned int amount)
    : shader(shader), amount(amount)
{
    this->init();
}

void ParticleGenerator::Update(float dt, unsigned int newParticles, glm::vec3 offset)
{
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.scale -= dt / 10.0f;
            p.Position -= p.Velocity * dt;
            p.Color.x += dt / 2.5f;
            p.Color.y += dt / 2.5f;
            p.Color.z += dt / 2.5f;
            p.Color.w -= dt / 2.5f;
        }
    }
}

// render all particles
void ParticleGenerator::Draw(glm::mat4 view, glm::mat4 proj)
{
    this->shader.Use();
    
    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            glm::mat4 model =
                glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f) + particle.Position) *
                glm::rotate(glm::mat4(1.0f), .0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), .0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(.5f, .5f, .5f)*particle.scale);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform4f(colorLoc, particle.Color.x, particle.Color.y, particle.Color.z, particle.Color.w);
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, this->size);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    this->shader.Use();
    viewLoc = glGetUniformLocation(this->shader.Program, "view");
    projLoc = glGetUniformLocation(this->shader.Program, "projection");
    modelLoc = glGetUniformLocation(this->shader.Program, "model");
    colorLoc = glGetUniformLocation(this->shader.Program, "Color");
    // set up mesh and attribute properties
    GLfloat cube_vertices[] =
    {
        // back
        0.5,0.5, -0.5,
        0.5,-0.5, -0.5,
       -0.5,0.5,-0.5,  
       -0.5,0.5,-0.5,
        0.5,-0.5, -0.5,
       -0.5,-0.5,-0.5,

       // front 
      -0.5,0.5,0.5,  
      -0.5,-0.5,0.5,  
       0.5,-0.5,0.5,  
      -0.5,0.5,0.5,  
       0.5,-0.5,0.5, 
       0.5,0.5,0.5,  

       // left
       -0.5,0.5,-0.5, 
       -0.5,-0.5,-0.5, 
       -0.5,-0.5,0.5, 
       -0.5,0.5,-0.5, 
       -0.5,-0.5,0.5, 
       -0.5,0.5,0.5, 



       // right
        0.5,0.5,0.5,  
        0.5,-0.5,0.5, 
        0.5,-0.5,-0.5, 
        0.5,0.5,0.5, 
        0.5,-0.5,-0.5, 
        0.5,0.5,-0.5, 


        // bottom
       -0.5,-0.5,0.5, 
       -0.5,-0.5,-0.5, 
        0.5,-0.5,-0.5,
       -0.5,-0.5,0.5, 
        0.5,-0.5,-0.5, 
        0.5,-0.5,0.5, 


        // top
        -0.5,0.5,-0.5,  
        -0.5,0.5,0.5,  
         0.5,0.5,0.5,  
        -0.5,0.5,-0.5,   
         0.5,0.5,0.5, 
         0.5,0.5,-0.5
    };
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    glGenBuffers(1, &this->VBO);
    
    // fill mesh buffer
    size = sizeof(cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->size, cube_vertices, GL_STATIC_DRAW);
    // set mesh attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, glm::vec3 offset)
{
    particle.scale = 1.0f;
    particle.Position = glm::vec3(.0f,.0f,.0f);
    particle.Color = glm::vec4(1.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = glm::vec3(((rand() % 100) - 50) / 30.0f, -(rand() % 100) / 5.0f, (rand() % 100) - 50 / 30.0f) ;
}