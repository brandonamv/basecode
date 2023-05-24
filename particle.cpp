#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle.h"
#include <math.h>
# define MY_PI 3.14159265358979323846

ParticleGenerator::ParticleGenerator()
{
    
    const GLchar* vShaderCode = 
        "#version 330 core \n"
        "layout(location = 0) in vec3 position;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * view * model * vec4(position, 1.0);\n"
        "}";
    const GLchar* gShaderCode =
        "#version 330 core \n"
        "layout(points) in;\n"
        "layout(triangle_strip, max_vertices = 4) out;\n"
        "out vec2 TexCoord;\n"
        "uniform float size;\n"
        "void main() {\n"
        "   gl_Position = gl_in[0].gl_Position + vec4(-size, -size, 0.0, 0.0);\n"
        "   TexCoord = vec2(0.0, 0.0);\n"
        "    EmitVertex();\n"
        "    gl_Position = gl_in[0].gl_Position + vec4(size, -size, 0.0, 0.0);\n"
        "    TexCoord = vec2(1.0, 0.0);\n"
        "    EmitVertex();\n"
        "   gl_Position = gl_in[0].gl_Position + vec4(-size, size, 0.0, 0.0);\n"
        "    TexCoord = vec2(0.0, 1.0);\n"
        "    EmitVertex();\n"
        "    gl_Position = gl_in[0].gl_Position + vec4(size, size, 0.0, 0.0);\n"
        "    TexCoord = vec2(1.0, 1.0);\n"
        "    EmitVertex();\n"
        "    EndPrimitive();\n"
        "}";
    const GLchar* fShaderCode = 
        "#version 330 core \n"
        "out vec4 color;\n"
        "uniform vec4 Color;\n"
        "uniform bool texturized;\n"
        "uniform sampler2D ourTexture;\n"
        "in vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "    if (texturized)\n"
        "        color = texture(ourTexture, TexCoord) * Color;\n"
        "    else\n"
        "        color = Color;\n"
        "}";
    // 2. Compile shaders
    GLuint vertex, geometry, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX1::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //geometry Shader
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    // Print compile errors if any
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Shader Program
    this->quad_shader = glCreateProgram();
    glAttachShader(this->quad_shader, vertex);
    glAttachShader(this->quad_shader, geometry);
    glAttachShader(this->quad_shader, fragment);
    glLinkProgram(this->quad_shader);
    // Print linking errors if any
    glGetProgramiv(this->quad_shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->quad_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
    const GLchar* vShaderCodePoint = 
        "#version 330 core \n"
        "layout(location = 0) in vec3 position;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * view * model * vec4(position, 1.0);\n"
        "}";
    const GLchar* fShaderCodePoint = 
        "#version 330 core \n"
        "out vec4 color;\n"
        "uniform vec4 Color;\n"
        "void main()\n"
        "{\n"
        "   if (dot(gl_PointCoord - 0.5, gl_PointCoord - 0.5) > 0.25)\n"
        "        discard;\n"
        "   else\n"
        "       color = (Color);\n"
        "}  ";
    // 2. Compile shaders
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCodePoint, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCodePoint, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Shader Program
    this->point_shader = glCreateProgram();
    glAttachShader(this->point_shader, vertex);
    glAttachShader(this->point_shader, fragment);
    glLinkProgram(this->point_shader);
    // Print linking errors if any
    glGetProgramiv(this->point_shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->point_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    this->init();
}
void ParticleGenerator::Update(float dt)
{
    
    // add new particles 
    this->secondCounter+= dt * this->anim_speed;
    if (this->secondCounter >= 1.0f/this->new_particles)
    {
        int unusedParticle = this->firstUnusedParticle();
        if (unusedParticle >= 0)
            this->respawnParticle(this->particles[unusedParticle]);
        
        this->secondCounter = .0f;
    }
    // update all particles
    for (unsigned int i = 0; i < this->max_particles; ++i)
    {
        
        Particle& p = this->particles[i];
        p.Life -= dt * this->anim_speed; // reduce life
        
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            
            float t = p.Life / p.tLife;
            p.currentColor = t * p.Color + (1 - t) * p.finColor;
            p.currentScale = t * p.scale + (1 - t) * p.scaleFin;
            if (fmod(rand(), 1000 / this->anim_speed) == 0) p.Direction.x += p.deviation.x;
            if (fmod(rand() , 1000 / this->anim_speed) == 0) p.Direction.y += p.deviation.y;
            if (fmod(rand() , 1000 / this->anim_speed) == 0) p.Direction.z += p.deviation.z;
            p.Direction = glm::normalize(p.Direction);
            p.currentPosition = p.Position + p.Direction * p.speed * (p.tLife - p.Life);
            if (this->opt_mass)
            {
                float tempSpeed = p.speed - p.mass * 9.8f * (p.tLife - p.Life);
                p.currentPosition.y = p.Position.y + p.Direction.y * tempSpeed * (p.tLife - p.Life);
            }
        }
    }
}

// render all particles
void ParticleGenerator::Draw( glm::mat4 view, glm::mat4 proj)
{
    
    if (opt_point)
    {
        glUseProgram(this->point_shader);
        viewLoc = glGetUniformLocation(this->point_shader, "view");
        projLoc = glGetUniformLocation(this->point_shader, "projection");
        modelLoc = glGetUniformLocation(this->point_shader, "model");
        colorLoc = glGetUniformLocation(this->point_shader, "Color");
    }
    else {
        glUseProgram(this->quad_shader);
        viewLoc = glGetUniformLocation(this->quad_shader, "view");
        projLoc = glGetUniformLocation(this->quad_shader, "projection");
        modelLoc = glGetUniformLocation(this->quad_shader, "model");
        colorLoc = glGetUniformLocation(this->quad_shader, "Color");
        sizeLoc = glGetUniformLocation(this->quad_shader, "size");
        texturizedLoc = glGetUniformLocation(this->quad_shader, "texturized");
        
    }
    
    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glm::vec3 eye =glm::vec3(glm::inverse(view)[3]);
    //quickSort(particles, 0, particles.size()-1);
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            glm::mat4 model =
                glm::translate(glm::mat4(1.0f), particle.currentPosition) *
                ident_matrix;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform4f(colorLoc, particle.currentColor.x, particle.currentColor.y, particle.currentColor.z, particle.currentColor.w);
            float distance = (glm::distance(eye, particle.currentPosition) / 5)+0.0001f;
            if (!opt_point)
            {
                glUniform1i(texturizedLoc, opt_texture);
                glUniform1f(sizeLoc, particle.size * particle.currentScale / distance);
                if (opt_texture)
                    glBindTexture(GL_TEXTURE_2D, texture);
            }
            glBindVertexArray(this->pointVAO);
            if (opt_point)
            {
                
                glPointSize(particle.size * particle.currentScale/distance);
                glDrawArrays(GL_POINTS, 0, this->size/ sizeof(GLfloat));
            }
            else
            {
                
                glDrawArrays(GL_POINTS, 0, (this->size*4 ) / sizeof(GLfloat));
            }
        }
    }
}

void ParticleGenerator::setTexture(unsigned char* data, int width, int height, bool active)
{
    this->opt_texture = active;
    if (active)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    

}

bool ParticleGenerator::getTexture()
{
    return this->opt_texture;
}

void ParticleGenerator::setColor(glm::vec4 ini, glm::vec4 fin)
{
    this->color_ini = ini;
    this->color_fin = fin;
}

float* ParticleGenerator::getInitColor()
{
    float color[4] = {this->color_ini.x,this->color_ini.y,this->color_ini.z,this->color_ini.w};
    return color;
}


float* ParticleGenerator::getFinColor()
{
    float color[4] = { this->color_fin.x,this->color_fin.y,this->color_fin.z,this->color_fin.w };
    return color;
}

void ParticleGenerator::setRGBAvariance(glm::vec4 ini, glm::vec4 fin)
{
    this->color_ini_variance = ini;
    this->color_fin_variance = fin;
}

float* ParticleGenerator::getInitColorVariance()
{
    float color[4] = { this->color_ini_variance.x,this->color_ini_variance.y,this->color_ini_variance.z,this->color_ini_variance.w };
    return color;
}

float* ParticleGenerator::getFinColorVariance()
{
    float color[4] = { this->color_fin_variance.x,this->color_fin_variance.y,this->color_fin_variance.z,this->color_fin_variance.w };
    return color;
}

void ParticleGenerator::setMinMax(glm::vec3 min, glm::vec3 max)
{
    this->spawn_max = max;
    this->spawn_min = min;
}

float* ParticleGenerator::getMin()
{
    float vec[3] = { this->spawn_min.x,this->spawn_min.y,this->spawn_min.z};
    return vec;
}

float* ParticleGenerator::getMax()
{
    float vec[3] = { this->spawn_max.x,this->spawn_max.y,this->spawn_max.z };
    return vec;
}

void ParticleGenerator::setDirection(glm::vec3 dir)
{
    this->direction = dir;
}

float* ParticleGenerator::getDirection()
{
    float vec[3] = { this->direction.x,this->direction.y,this->direction.z };
    return vec;
}

void ParticleGenerator::setDirectionVar(glm::vec3 dir)
{
    this->direction_variance = dir;
}

float* ParticleGenerator::getDirectionVar()
{
    float vec[3] = { this->direction_variance.x,this->direction_variance.y,this->direction_variance.z };
    return vec;
}

void ParticleGenerator::setMaxParticles(int n)
{
    this->particles.resize(n);
    this->max_particles = n;
}

int ParticleGenerator::getMaxParticles()
{
    return this->max_particles;
}

void ParticleGenerator::setNewParticles(int n)
{
    this->new_particles = n;
}

int ParticleGenerator::getNewParticles()
{
    return this->new_particles;
}

void ParticleGenerator::setNewParticlesVariance(int n)
{
    this->new_particles_variance = n;
}

int ParticleGenerator::getNewParticlesVariance()
{
    return this->new_particles_variance;
}

void ParticleGenerator::setAnimSpeed(float n)
{
    this->anim_speed= n;
}

void ParticleGenerator::setGravity(bool opt)
{
    this->opt_mass = opt;
}

void ParticleGenerator::setLifeTime(float t)
{
    this->lifetime = t;
}

float ParticleGenerator::getLifeTime()
{
    return this->lifetime;
}

void ParticleGenerator::setLifeTimeVar(float t)
{
    this->lifetime_var = t;
}

float ParticleGenerator::getLifeTimeVar()
{
    return this->lifetime_var;
}

void ParticleGenerator::setSpeed(float t)
{
    this->speed = t;

}

float ParticleGenerator::getSpeed()
{
    return this->speed;
}

void ParticleGenerator::setSpeedVar(float t)
{
    this->speed_variance = t;
}

float ParticleGenerator::getSpeedVar()
{
    return this->speed_variance;
}

void ParticleGenerator::setSize(float s)
{
    this->size_particle = s;
}

float ParticleGenerator::getSize()
{
    return this->size_particle;
}

void ParticleGenerator::setSizeVar(float s)
{
    this->size_variance = s;
}

float ParticleGenerator::getSizeVar()
{
    return this->size_variance;
}

void ParticleGenerator::setScale(float s_ini, float s_fin)
{
    this->scale_ini = s_ini;
    this->scale_fin = s_fin;
}

float ParticleGenerator::getScaleIni()
{
    return this->scale_ini;
}

float ParticleGenerator::getScaleFin()
{
    return this->scale_fin;
}

void ParticleGenerator::setScaleVar(float s_ini, float s_fin)
{
    this->scale_ini_var = s_ini;
    this->scale_fin_var = s_fin;
}

float ParticleGenerator::getScaleIniVar()
{
    return this->scale_ini_var;
}

float ParticleGenerator::getScaleFinVar()
{
    return this->scale_fin_var;
}

void ParticleGenerator::setMass(float m)
{
    this->mass=m;
}

float ParticleGenerator::getMass()
{
    return this->mass;
}

void ParticleGenerator::setMassVar(float m)
{
    this->mass_variance = m;
}

float ParticleGenerator::getMassVar()
{
    return this->mass_variance;
}

void ParticleGenerator::setOptPoint(bool o)
{
    opt_point = o;
}

bool ParticleGenerator::getOptPoint()
{
    return this->opt_point;
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
    if (lastUsedParticle>this->max_particles)
    {
        lastUsedParticle = this->max_particles;
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

void ParticleGenerator::respawnParticle(Particle& particle)
{
    glm::vec3 temp3;
    glm::vec3 min, max;
    int pivot = rand();
    float temp = this->scale_ini + iterator[rand() % 2] * modNumber(pivot, this->scale_ini_var);
    if (temp <= 0)temp = .1f;
    particle.scale = temp;
    temp = this->scale_fin + iterator[rand() % 2] * modNumber(pivot, this->scale_fin_var);
    if (temp <= 0)temp = .1f;
    particle.scaleFin = temp;
    if (this->spawn_min.x < this->spawn_max.x)
    {
        min.x = this->spawn_min.x;
        max.x = this->spawn_max.x;
    }
    else
    {
        max.x = this->spawn_min.x;
        min.x = this->spawn_max.x;
    }
    if (this->spawn_min.y < this->spawn_max.y)
    {
        min.y = this->spawn_min.y;
        max.y = this->spawn_max.y;
    }
    else
    {
        max.y = this->spawn_min.y;
        min.y = this->spawn_max.y;
    }
    if (this->spawn_min.z < this->spawn_max.z)
    {
        min.z = this->spawn_min.z;
        max.z = this->spawn_max.z;
    }
    else
    {
        max.z = this->spawn_min.z;
        min.z = this->spawn_max.z;
    }
    
    particle.Position = glm::vec3(
        fmod(rand(), max.x - min.x + 1) + min.x,
        fmod(rand(), max.y - min.y + 1) + min.y,
        fmod(rand(), max.z - min.z + 1) + min.z
    );
    
    particle.Color = colorVariance(color_ini,color_ini_variance);

    particle.finColor = colorVariance(color_fin, color_fin_variance);

    temp = this->lifetime + iterator[rand() % 2] * modNumber(pivot, this->lifetime_var);
    if (temp <= 0)temp = .1f;
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
        if (div==1.0f||div==.5f)
        {
            return fmod(n, div + .01f);
        }
        return fmod(n, div);
    }
}

