#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle.h"
#include <vector>
#include <math.h>
# define MY_PI 3.14159265358979323846
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
            float t = p.Life / p.tLife;
            p.Color = t * (glm::vec4(1.0f, .9f, .01f, .8f)) + (1 - t) * (glm::vec4(1.0f, .0f, .0f, .0f));
            p.scale -= dt/ (p.tLife*2.0f);
            if (p.Life*2.0f < p.tLife ) {
                p.Position.x += p.Velocity.x * dt;
                p.Position.z += p.Velocity.z * dt;
            }
            else {
                p.Position.x -= p.Velocity.x * dt;
                p.Position.z -= p.Velocity.z * dt;
            }
            p.Position.y -= p.Velocity.y * dt;

            /*p.Color.x += dt / 2.5f;
            p.Color.y += dt / 2.5f;
            p.Color.z += dt / 2.5f;
            p.Color.w -= dt / 2.5f;*/
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
                glm::translate(glm::mat4(1.0f), particle.Position) *
                glm::rotate(glm::mat4(1.0f), .0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), .0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(.5f, .5f, .5f)*particle.scale);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform4f(colorLoc, particle.Color.x, particle.Color.y, particle.Color.z, particle.Color.w);
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, this->size/ sizeof(GLfloat));
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
    float radius = 1.0f;
    int sectorCount = 30,stackCount=10;
    float x, y, z, xy;                              // vertex position
    double PI = MY_PI;
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;
    std::vector<GLfloat> vertices;
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(roundf(x *100)/100);
            vertices.push_back(roundf(y * 100)/100);
            vertices.push_back(roundf(z * 100)/100);
        }
    }
    std::vector<GLfloat> circle;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                circle.push_back(vertices[k1*3]);
                circle.push_back(vertices[k1*3+1]);
                circle.push_back(vertices[k1*3+2]);
                circle.push_back(vertices[k2*3]);
                circle.push_back(vertices[k2*3+1]);
                circle.push_back(vertices[k2*3+2]);
                circle.push_back(vertices[(k1 + 1)*3]);
                circle.push_back(vertices[(k1 + 1)*3+1]);
                circle.push_back(vertices[(k1 + 1)*3+2]);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                circle.push_back(vertices[(k1 + 1)*3]);
                circle.push_back(vertices[(k1 + 1)*3+1]);
                circle.push_back(vertices[(k1 + 1)*3+2]);
                circle.push_back(vertices[k2*3]);
                circle.push_back(vertices[k2*3+1]);
                circle.push_back(vertices[k2*3+2]);
                circle.push_back(vertices[(k2 + 1)*3]);
                circle.push_back(vertices[(k2 + 1)*3+1]);
                circle.push_back(vertices[(k2 + 1)*3+2]);
            }

        }
    }
    /*
    // set up mesh and attribute properties
    //GLfloat cube_vertices[] =
    //{
    //    // back
    //    0.5,0.5, -0.5,
    //    0.5,-0.5, -0.5,
    //   -0.5,0.5,-0.5,  
    //   -0.5,0.5,-0.5,
    //    0.5,-0.5, -0.5,
    //   -0.5,-0.5,-0.5,

    //   // front 
    //  -0.5,0.5,0.5,  
    //  -0.5,-0.5,0.5,  
    //   0.5,-0.5,0.5,  
    //  -0.5,0.5,0.5,  
    //   0.5,-0.5,0.5, 
    //   0.5,0.5,0.5,  

    //   // left
    //   -0.5,0.5,-0.5, 
    //   -0.5,-0.5,-0.5, 
    //   -0.5,-0.5,0.5, 
    //   -0.5,0.5,-0.5, 
    //   -0.5,-0.5,0.5, 
    //   -0.5,0.5,0.5, 



    //   // right
    //    0.5,0.5,0.5,  
    //    0.5,-0.5,0.5, 
    //    0.5,-0.5,-0.5, 
    //    0.5,0.5,0.5, 
    //    0.5,-0.5,-0.5, 
    //    0.5,0.5,-0.5, 


    //    // bottom
    //   -0.5,-0.5,0.5, 
    //   -0.5,-0.5,-0.5, 
    //    0.5,-0.5,-0.5,
    //   -0.5,-0.5,0.5, 
    //    0.5,-0.5,-0.5, 
    //    0.5,-0.5,0.5, 


    //    // top
    //    -0.5,0.5,-0.5,  
    //    -0.5,0.5,0.5,  
    //     0.5,0.5,0.5,  
    //    -0.5,0.5,-0.5,   
    //     0.5,0.5,0.5, 
    //     0.5,0.5,-0.5
    //};*/
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    glGenBuffers(1, &this->VBO);
    
    // fill mesh buffer
    size = circle.size()*sizeof(GLfloat*);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->size, circle.data(), GL_STATIC_DRAW);
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
    particle.scale = .5f;
    particle.Position = glm::vec3(.0f,.0f,.0f);
    particle.Color = glm::vec4(1.0f, (rand() % 50) / 100.0f, (rand() % 30) / 100.0f, 1.0f);
    particle.Life = 0.8f+sin(rand())/10.0f;
    particle.tLife = particle.Life;
    particle.Velocity = glm::vec3(((rand() % 100) - 50) / 20.0f, -(rand() % 100) / 10.0f, ((rand() % 100) - 50) / 20.0f)/5.0f ;
}