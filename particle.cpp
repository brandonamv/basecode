#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle.h"
#include <math.h>
# define MY_PI 3.14159265358979323846
ParticleGenerator::ParticleGenerator()
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
    for (unsigned int i = 0; i < this->max_particles; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            
            float t = p.Life / p.tLife;
            p.Color = t * p.Color + (1 - t) * p.finColor;
            p.scale = t * p.scale + (1 - t) * p.scaleFin;
            p.Position += p.Direction * p.speed;
            if (this->opt_mass)
            {
                p.Position.y -= p.mass*9.8f;
            }

        }
    }
}

// render all particles
void ParticleGenerator::Draw(Shader particle_shader, glm::mat4 view, glm::mat4 proj)
{
    viewLoc = glGetUniformLocation(particle_shader.Program, "view");
    projLoc = glGetUniformLocation(particle_shader.Program, "projection");
    modelLoc = glGetUniformLocation(particle_shader.Program, "model");
    colorLoc = glGetUniformLocation(particle_shader.Program, "Color");
    sizeLoc = glGetUniformLocation(particle_shader.Program, "size");
    
    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    // use additive blending to give it a 'glow' effect
    quickSort(particles, 0, particles.size()-1);
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
            glBindVertexArray(this->pointVAO);
            glDrawArrays(GL_POINTS, 0, this->size/ sizeof(GLfloat));
            glPointSize(10.0f);
        }
    }
}

int ParticleGenerator::partition(std::vector<Particle>& arr, int start, int end)
{
    // assuspawn_ming last element as pivotElement
    int index = 0, pivotIndex{};
    Particle pivotElement = arr[end];
    std::vector<Particle> temp(end - start + 1); // making an array whose size is equal to current partition range...
    for (int i = start; i <= end; i++) // pushing all the elements in temp which are smaller than pivotElement
    {
        if (arr[i].Color.w > pivotElement.Color.w)
        {
            temp[index] = arr[i];
            index++;
        }
    }

    temp[index] = pivotElement; // pushing pivotElement in temp
    index++;

    for (int i = start; i < end; i++) // pushing all the elements in temp which are greater than pivotElement
    {
        if (arr[i].Color.w < pivotElement.Color.w)
        {
            temp[index] = arr[i];
            index++;
        }
    }
    // all the elements now in temp array are order :
    // leftmost elements are lesser than pivotElement and rightmost elements are greater than pivotElement



    index = 0;
    for (int i = start; i <= end; i++) // copying all the elements to original array i.e arr
    {
        if (arr[i].id == pivotElement.id)
        {
            // for getting pivot index in the original array.
            // we need the pivotIndex value in the original and not in the temp array
            pivotIndex = i;
        }
        arr[i] = temp[index];
        index++;
    }
    return pivotIndex; // returning pivotIndex
}

void ParticleGenerator::quickSort(std::vector<Particle>& arr, int start, int end)
{
    if (start < end)
    {
        int partitionIndex = partition(arr, start, end); // for getting partition
        quickSort(arr, start, partitionIndex - 1); // sorting left side array
        quickSort(arr, partitionIndex + 1, end); // sorting right side array
    }
    return;
}

void ParticleGenerator::init()
{
    GLfloat point[] = { .0f,.0f,.0f };
    
    glGenVertexArrays(1, &this->pointVAO);
    glBindVertexArray(this->pointVAO);
    
    glGenBuffers(1, &this->pointVBO);
    this->size = sizeof(point);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->pointVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    // set mesh attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    GLfloat bounding[] = {
        spawn_min.x,spawn_max.y,spawn_max.z,
        spawn_max.x,spawn_max.y,spawn_max.z,
        spawn_min.x,spawn_min.y,spawn_max.z,
        spawn_max.x,spawn_min.y,spawn_max.z,
        spawn_max.x,spawn_min.y,spawn_max.z,
        spawn_max.x,spawn_max.y,spawn_max.z,
        spawn_min.x,spawn_min.y,spawn_max.z,
        spawn_min.x,spawn_max.y,spawn_max.z,//cuadrado frontal
        spawn_max.x,spawn_max.y,spawn_min.z,
        spawn_min.x,spawn_max.y,spawn_min.z,
        spawn_max.x,spawn_max.y,spawn_min.z,
        spawn_max.x,spawn_min.y,spawn_min.z,
        spawn_min.x,spawn_min.y,spawn_min.z,
        spawn_max.x,spawn_min.y,spawn_min.z,
        spawn_min.x,spawn_min.y,spawn_min.z,
        spawn_min.x,spawn_max.y,spawn_min.z, //cuadrado trasero
        spawn_max.x,spawn_max.y,spawn_max.z,
        spawn_max.x,spawn_max.y,spawn_min.z,
        spawn_max.x,spawn_min.y,spawn_max.z,
        spawn_max.x,spawn_min.y,spawn_min.z,
        spawn_min.x,spawn_max.y,spawn_max.z,
        spawn_min.x,spawn_max.y,spawn_min.z,
        spawn_min.x,spawn_min.y,spawn_max.z,
        spawn_min.x,spawn_min.y,spawn_min.z
    };
    glGenVertexArrays(1, &boundingVAO);
    // 1. bind Vertex Array Object
    glBindVertexArray(boundingVAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glGenBuffers(1, &boundingVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boundingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat*) * 72, bounding, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->max_particles; ++i) {
        Particle temp;
        temp.id = i;
        this->particles.push_back(temp);
    }
        
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->max_particles; ++i) {
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
    int pivot = rand();
    particle.scale =  this->size_ini + (iterator[rand()%1] * (fmod(pivot, this->size_ini_var)));
    particle.scaleFin = this->size_fin + (iterator[rand() % 1] * (fmod(pivot, this->size_fin_var)));
    particle.Position = glm::vec3(
        fmod(pivot, this->spawn_max.x - this->spawn_min.x + 1) + this->spawn_min.x,
        fmod(pivot, this->spawn_max.y - this->spawn_min.y + 1) + this->spawn_min.y,
        fmod(pivot, this->spawn_max.z - this->spawn_min.z + 1) + this->spawn_min.z
    );
    particle.Color = glm::vec4(
        this->color_ini.x + (iterator[rand() % 1] * (fmod(pivot, this->color_ini_variance.x))),
        this->color_ini.y + (iterator[rand() % 1] * (fmod(pivot, this->color_ini_variance.y))),
        this->color_ini.z + (iterator[rand() % 1] * (fmod(pivot, this->color_ini_variance.z))),
        this->color_ini.w + (iterator[rand() % 1] * (fmod(pivot, this->color_ini_variance.w)))
    );
    particle.midColor = glm::vec4(
        this->color_mid.x + (iterator[rand() % 1] * (fmod(pivot, this->color_mid_variance.x))),
        this->color_mid.y + (iterator[rand() % 1] * (fmod(pivot, this->color_mid_variance.y))),
        this->color_mid.z + (iterator[rand() % 1] * (fmod(pivot, this->color_mid_variance.z))),
        this->color_mid.w + (iterator[rand() % 1] * (fmod(pivot, this->color_mid_variance.w)))
    );
    particle.finColor = glm::vec4(
        this->color_fin.x + (iterator[rand() % 1] * (fmod(pivot, this->color_fin_variance.x))),
        this->color_fin.y + (iterator[rand() % 1] * (fmod(pivot, this->color_fin_variance.y))),
        this->color_fin.z + (iterator[rand() % 1] * (fmod(pivot, this->color_fin_variance.z))),
        this->color_fin.w + (iterator[rand() % 1] * (fmod(pivot, this->color_fin_variance.w)))
    );
    particle.Life = this->lifetime + (iterator[rand() % 1] * (fmod(pivot, this->lifetime_var)));
    particle.tLife = particle.Life;
    particle.mLife = particle.Life / 2;
    particle.Direction = glm::vec3(
        this->direction.x + iterator[rand()%1] * (fmod(pivot, this->direction_variance.x)),
        this->direction.y + iterator[rand()%1] * (fmod(pivot, this->direction_variance.y)),
        this->direction.z + iterator[rand()%1] * (fmod(pivot, this->direction_variance.z))
    );
    particle.speed = this->speed + iterator[rand()%1] * (fmod(pivot, this->speed_variance));
    particle.mass = this->mass + iterator[rand()%1] * (fmod(pivot, this->mass_variance));
}