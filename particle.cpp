#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle.h"
#include <math.h>
# define MY_PI 3.14159265358979323846
ParticleGenerator::ParticleGenerator()
{
    this->init();
}
float secondCounter = .0f;
void ParticleGenerator::Update(float dt, unsigned int newParticles, glm::vec3 offset)
{
    // add new particles 
    secondCounter += dt;
    if (secondCounter >=1.0f)
    {
        for (unsigned int i = 0; i < newParticles; ++i)
        {
            int unusedParticle = this->firstUnusedParticle();
            if (unusedParticle >= 0)
                this->respawnParticle(this->particles[unusedParticle], offset);

        }
        secondCounter = .0f;
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
            if (rand() % 100 == 0) p.Direction.x += p.deviation.x;
            if (rand() % 100 == 0) p.Direction.y += p.deviation.y;
            if (rand() % 100 == 0) p.Direction.z += p.deviation.z;
            p.Direction = glm::normalize(p.Direction);
            p.Position += p.Direction * p.speed;
            if (this->opt_mass)
            {
                //p.Position.y -= p.mass*9.8f;
                p.Position.y -= p.speed+ p.mass * 9.8f*(p.tLife-p.Life);
            }

        }
    }
}

// render all particles
void ParticleGenerator::Draw(Shader point_shader, Shader quad_shader, glm::mat4 view, glm::mat4 proj)
{
    
    if (opt_point)
    {
        point_shader.Use();
        viewLoc = glGetUniformLocation(point_shader.Program, "view");
        projLoc = glGetUniformLocation(point_shader.Program, "projection");
        modelLoc = glGetUniformLocation(point_shader.Program, "model");
        colorLoc = glGetUniformLocation(point_shader.Program, "Color");
        pointLoc = glGetUniformLocation(point_shader.Program, "point");
    }
    else {
        quad_shader.Use();
        viewLoc = glGetUniformLocation(quad_shader.Program, "view");
        projLoc = glGetUniformLocation(quad_shader.Program, "projection");
        modelLoc = glGetUniformLocation(quad_shader.Program, "model");
        colorLoc = glGetUniformLocation(quad_shader.Program, "Color");
        sizeLoc = glGetUniformLocation(quad_shader.Program, "size");
        pointLoc = glGetUniformLocation(quad_shader.Program, "point");
    }
    
    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    // use additive blending to give it a 'glow' effect
    //quickSort(particles, 0, particles.size()-1);
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            glm::mat4 model =
                glm::translate(glm::mat4(1.0f), particle.Position) *
                ident_matrix;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform4f(colorLoc, particle.Color.x, particle.Color.y, particle.Color.z, particle.Color.w);
            glUniform1i(pointLoc, opt_point);
            if (!opt_point)
                glUniform1f(sizeLoc, particle.size * particle.scale);
            
            glBindVertexArray(this->pointVAO);
            if (opt_point)
            {
                glPointSize(particle.size * particle.scale);
                glDrawArrays(GL_POINTS, 0, this->size/ sizeof(GLfloat));
            }
            else
            {
                glDrawArrays(GL_POINTS, 0, (this->size*4 ) / sizeof(GLfloat));
            }
            
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
    return -1;
}

void ParticleGenerator::respawnParticle(Particle& particle, glm::vec3 offset)
{
    glm::vec3 temp3;
    
    int pivot = rand();
    float temp = this->size_ini + iterator[rand() % 2] * modNumber(pivot, this->size_ini_var);
    if (temp <= 0)temp = .000000001f;
    particle.scale = temp;
    temp = this->size_fin + iterator[rand() % 2] * modNumber(pivot, this->size_fin_var);
    if (temp <= 0)temp = .000000001f;
    particle.scaleFin = temp;
    particle.Position = glm::vec3(
        fmod(pivot, this->spawn_max.x - this->spawn_min.x + 1) + this->spawn_min.x,
        fmod(pivot, this->spawn_max.y - this->spawn_min.y + 1) + this->spawn_min.y,
        fmod(pivot, this->spawn_max.z - this->spawn_min.z + 1) + this->spawn_min.z
    );
    
    particle.Color = colorVariance(color_ini,color_ini_variance);
    
    particle.midColor = colorVariance(color_mid, color_mid_variance);

    particle.finColor = colorVariance(color_fin, color_fin_variance);

    temp = this->lifetime + iterator[rand() % 2] * modNumber(pivot, this->lifetime_var);
    if (temp <= 0)temp = .000000001f;
    particle.Life = temp;
    particle.tLife = particle.Life;
    particle.mLife = particle.Life / 2;
    particle.Direction = glm::vec3(
        this->direction.x + iterator[rand() % 2] * modNumber(pivot, this->direction_variance.x),
        this->direction.y + iterator[rand() % 2] * modNumber(pivot, this->direction_variance.y),
        this->direction.z + iterator[rand() % 2] * modNumber(pivot, this->direction_variance.z)
    );
    particle.Direction = glm::normalize(particle.Direction);
    particle.speed = this->speed + iterator[rand() % 2] * modNumber(pivot, this->speed_variance);
    particle.mass = this->mass + iterator[rand() % 2] * modNumber(pivot, this->mass_variance);

    temp = this->size_particle + iterator[rand() % 2] * modNumber(pivot, this->size_variance);
    if (temp <= 0)temp = .1f;
    particle.size = temp;

    particle.deviation = glm::vec3(
        this->dirDesviation.x + this->iterator[rand() % 2] * this->modNumber(pivot, this->dirDesviation_var.x),
        this->dirDesviation.y + this->iterator[rand() % 2] * this->modNumber(pivot, this->dirDesviation_var.y),
        this->dirDesviation.z + this->iterator[rand() % 2] * this->modNumber(pivot, this->dirDesviation_var.z)
    );
}

glm::vec4 ParticleGenerator::colorVariance(glm::vec4 color, glm::vec4 variance)
{
    int pivot = rand();
    glm::vec4 temp4 = glm::vec4(
        color.x + iterator[rand() % 2] * modNumber(pivot, variance.x),
        color.y + iterator[rand() % 2] * modNumber(pivot, variance.y),
        color.z + iterator[rand() % 2] * modNumber(pivot, variance.z),
        color.w + iterator[rand() % 2] * modNumber(pivot, variance.w)
    );
    if (temp4.x > 1)temp4.x = 1.0f;
    if (temp4.x < 0)temp4.x = .0f;
    if (temp4.y > 1)temp4.y = 1.0f;
    if (temp4.y < 0)temp4.y = .0f;
    if (temp4.z > 1)temp4.z = 1.0f;
    if (temp4.z < 0)temp4.z = .0f;
    if (temp4.w > 1)temp4.w = 1.0f;
    if (temp4.w < 0)temp4.w = .0f;
    return temp4;
}

float ParticleGenerator::modNumber(int n, float div)
{
    if (div==0)return 0.0f;
    else
    {
        return fmod(n, div);
    }
}
