#include <glm/gtc/matrix_transform.hpp>
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
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "void main() {\n"
        "    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-size, -size, 0.0, 0.0));\n"
        "    TexCoord = vec2(0.0, 0.0);\n"
        "    EmitVertex();\n"
        "    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(size, -size, 0.0, 0.0));\n"
        "    TexCoord = vec2(1.0, 0.0);\n"
        "    EmitVertex();\n"
        "    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-size, size, 0.0, 0.0));\n"
        "    TexCoord = vec2(0.0, 1.0);\n"
        "    EmitVertex();\n"
        "    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(size, size, 0.0, 0.0));\n"
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
        "    if (texturized){\n"
        "        vec4 temp = texture(ourTexture, TexCoord) * Color;\n"
        "        if(temp.a==0)discard;\n"
        "        color = temp;}\n"
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
    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->max_particles; ++i) {
        Particle temp;
        temp.id = i;
        this->particles.push_back(temp);
    }

}

void ParticleGenerator::setTexture(unsigned char* data, int width, int height, bool active, std::string img)
{
    this->opt_texture = active;
    textureData = img;
    if (active)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (img[img.find_last_of(".") + 1] == 'j')
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }



}

bool ParticleGenerator::getTexture()
{
    return this->opt_texture;
}

std::string ParticleGenerator::getTextureData()
{
    return this->textureData;
}

void ParticleGenerator::setColor(glm::vec4 ini, glm::vec4 fin)
{
    this->color_ini = ini;
    this->color_fin = fin;
}

float* ParticleGenerator::getInitColor()
{
    float color[4] = { this->color_ini.x,this->color_ini.y,this->color_ini.z,this->color_ini.w };
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
    float vec[3] = { this->spawn_min.x,this->spawn_min.y,this->spawn_min.z };
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

void ParticleGenerator::setDesviation(glm::vec3 dir)
{
    this->dirDesviation = dir;
}

float* ParticleGenerator::getDeviation()
{
    float vec[3] = { this->dirDesviation.x,this->dirDesviation.y,this->dirDesviation.z };
    return vec;
}

void ParticleGenerator::setDesviationVar(glm::vec3 dir)
{
    this->dirDesviation_var = dir;
}

float* ParticleGenerator::getDeviationVar()
{
    float vec[3] = { this->dirDesviation_var.x,this->dirDesviation_var.y,this->dirDesviation_var.z };
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
    this->anim_speed = n;
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
    this->mass = m;
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

void ParticleGenerator::setOptBlending(bool o)
{
    opt_blending = o;
}

std::string ParticleGenerator::save()
{
    std::string temp = "";
    temp.append("spawn_min " + std::to_string(spawn_min.x) + " " + std::to_string(spawn_min.y) + " " + std::to_string(spawn_min.z) + "\n");
    temp.append("spawn_max " + std::to_string(spawn_max.x) + " " + std::to_string(spawn_max.y) + " " + std::to_string(spawn_max.z) + "\n");
    temp.append("color_ini " + std::to_string(color_ini.x) + " " + std::to_string(color_ini.y) + " " + std::to_string(color_ini.z) + " " + std::to_string(color_ini.w) + "\n");
    temp.append("color_ini_var " + std::to_string(color_ini_variance.x) + " " + std::to_string(color_ini_variance.y) + " " + std::to_string(color_ini_variance.z) + " " + std::to_string(color_ini_variance.w) + "\n");
    temp.append("color_fin " + std::to_string(color_fin.x) + " " + std::to_string(color_fin.y) + " " + std::to_string(color_fin.z) + " " + std::to_string(color_fin.w) + "\n");
    temp.append("color_fin_var " + std::to_string(color_fin_variance.x) + " " + std::to_string(color_fin_variance.y) + " " + std::to_string(color_fin_variance.z) + " " + std::to_string(color_fin_variance.w) + "\n");
    temp.append("direction " + std::to_string(direction.x) + " " + std::to_string(direction.y) + " " + std::to_string(direction.z) + "\n");
    temp.append("direction_var " + std::to_string(direction_variance.x) + " " + std::to_string(direction_variance.y) + " " + std::to_string(direction_variance.z) + "\n");
    temp.append("dirDesviation " + std::to_string(dirDesviation.x) + " " + std::to_string(dirDesviation.y) + " " + std::to_string(dirDesviation.z) + "\n");
    temp.append("dirDesviation_var " + std::to_string(dirDesviation_var.x) + " " + std::to_string(dirDesviation_var.y) + " " + std::to_string(dirDesviation_var.z) + "\n");
    temp.append("speed " + std::to_string(speed) + "\n");
    temp.append("speed_variance " + std::to_string(speed_variance) + "\n");
    temp.append("scale_ini " + std::to_string(scale_ini) + "\n");
    temp.append("scale_ini_var " + std::to_string(scale_ini_var) + "\n");
    temp.append("scale_fin " + std::to_string(scale_fin) + "\n");
    temp.append("scale_fin_var " + std::to_string(scale_fin_var) + "\n");
    temp.append("mass " + std::to_string(mass) + "\n");
    temp.append("mass_variance " + std::to_string(mass_variance) + "\n");
    temp.append("anim_speed " + std::to_string(anim_speed) + "\n");
    temp.append("lifetime " + std::to_string(lifetime) + "\n");
    temp.append("lifetime_var " + std::to_string(lifetime_var) + "\n");
    temp.append("size_particle " + std::to_string(size_particle) + "\n");
    temp.append("size_variance " + std::to_string(size_variance) + "\n");
    temp.append("new_particles " + std::to_string(new_particles) + "\n");
    temp.append("new_particles_variance " + std::to_string(new_particles_variance) + "\n");
    temp.append("max_particles " + std::to_string(max_particles) + "\n");
    temp.append("point " + std::to_string(opt_point) + "\n");
    if (!opt_point && opt_texture)
    {
        temp.append("texture " + std::to_string(opt_texture) + "\n");
        temp.append("texture_img " + textureData + "\n");
    }
    for (Particle p : this->particles)
    {
        if (p.Life > 0.0f)
        {
            temp.append("Position " + std::to_string(p.Position.x) + " " + std::to_string(p.Position.y) + " " + std::to_string(p.Position.z) + "\n");
            temp.append("Direction " + std::to_string(p.Direction.x) + " " + std::to_string(p.Direction.y) + " " + std::to_string(p.Direction.z) + "\n");
            temp.append("CurrentPosition " + std::to_string(p.currentPosition.x) + " " + std::to_string(p.currentPosition.y) + " " + std::to_string(p.currentPosition.z) + "\n");
            temp.append("Deviation " + std::to_string(p.deviation.x) + " " + std::to_string(p.deviation.y) + " " + std::to_string(p.deviation.z) + "\n");
            temp.append("Color " + std::to_string(p.Color.x) + " " + std::to_string(p.Color.y) + " " + std::to_string(p.Color.z) + " " + std::to_string(p.Color.w) + "\n");
            temp.append("FinColor " + std::to_string(p.finColor.x) + " " + std::to_string(p.finColor.y) + " " + std::to_string(p.finColor.z) + " " + std::to_string(p.finColor.w) + "\n");
            temp.append("CurrentColor " + std::to_string(p.currentColor.x) + " " + std::to_string(p.currentColor.y) + " " + std::to_string(p.currentColor.z) + " " + std::to_string(p.currentColor.w) + "\n");
            temp.append("Speed " + std::to_string(p.speed) + "\n");
            temp.append("CurrentScale " + std::to_string(p.currentScale) + "\n");
            temp.append("Scale " + std::to_string(p.scale) + "\n");
            temp.append("ScaleFin " + std::to_string(p.scaleFin) + "\n");
            temp.append("Size " + std::to_string(p.size) + "\n");
            temp.append("Life " + std::to_string(p.Life) + "\n");
            temp.append("tLife " + std::to_string(p.tLife) + "\n");
            temp.append("Mass " + std::to_string(p.mass) + "\n");
        }
    }
    return temp;
}

void ParticleGenerator::load(std::string t)
{
    FILE* file;
    char actual_line[128] = { 0 };
    char tempS[128] = { 0 };
    if (fopen_s(&file, t.data(), "r") != 0)
    {
        printf("Error al cargar el objeto!\n");
        return;
    }

    int count = -1;
    glm::vec3 tempV;
    glm::vec4 tempC;
    float tempF;
    int tempI;
    Particle temp;
    while (1)
    {
        if (fscanf(file, "%127s", &actual_line) == EOF)
            break;

        if (strcmp(actual_line, "spawn_min") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &tempV.x, &tempV.y, &tempV.z);
            this->spawn_min = tempV;
        }
        if (strcmp(actual_line, "spawn_max") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &tempV.x, &tempV.y, &tempV.z);
            this->spawn_max = tempV;
        }
        if (strcmp(actual_line, "color_ini") == 0)
        {
            fscanf_s(file, "%f %f %f %f\n", &tempC.x, &tempC.y, &tempC.z, &tempC.w);
            this->color_ini = tempC;
        }
        if (strcmp(actual_line, "color_ini_var") == 0)
        {
            fscanf_s(file, "%f %f %f %f\n", &tempC.x, &tempC.y, &tempC.z, &tempC.w);
            this->color_ini_variance = tempC;
        }
        if (strcmp(actual_line, "color_fin") == 0)
        {
            fscanf_s(file, "%f %f %f %f\n", &tempC.x, &tempC.y, &tempC.z, &tempC.w);
            this->color_fin = tempC;

        }
        if (strcmp(actual_line, "color_fin_var") == 0)
        {
            fscanf_s(file, "%f %f %f %f\n", &tempC.x, &tempC.y, &tempC.z, &tempC.w);
            this->color_fin_variance = tempC;
        }
        if (strcmp(actual_line, "direction") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &tempV.x, &tempV.y, &tempV.z);
            this->direction = tempV;
        }
        if (strcmp(actual_line, "dirDesviation") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &tempV.x, &tempV.y, &tempV.z);
            this->dirDesviation = tempV;

        }
        if (strcmp(actual_line, "direction_var") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &tempV.x, &tempV.y, &tempV.z);
            this->direction_variance = tempV;
        }
        if (strcmp(actual_line, "dirDesviation_var") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &tempV.x, &tempV.y, &tempV.z);
            this->dirDesviation_var = tempV;
        }
        if (strcmp(actual_line, "speed") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->speed = tempF;
        }
        if (strcmp(actual_line, "speed_variance") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->speed_variance = tempF;
        }
        if (strcmp(actual_line, "scale_ini") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->scale_ini = tempF;
        }
        if (strcmp(actual_line, "scale_ini_var") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->scale_ini_var = tempF;
        }
        if (strcmp(actual_line, "scale_fin") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->scale_fin = tempF;
        }
        if (strcmp(actual_line, "scale_fin_var") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->scale_fin_var = tempF;
        }
        if (strcmp(actual_line, "mass") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->mass = tempF;
        }
        if (strcmp(actual_line, "mass_variance") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->mass_variance = tempF;
        }
        if (strcmp(actual_line, "anim_speed") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->anim_speed = tempF;
        }
        if (strcmp(actual_line, "lifetime") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->lifetime = tempF;
        }
        if (strcmp(actual_line, "lifetime_var") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->lifetime_var = tempF;
        }
        if (strcmp(actual_line, "size_particle") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->size_particle = tempF;
        }
        if (strcmp(actual_line, "size_variance") == 0)
        {
            fscanf_s(file, "%f\n", &tempF);
            this->size_variance = tempF;
        }
        if (strcmp(actual_line, "new_particles") == 0)
        {
            fscanf_s(file, "%d\n", &tempI);
            this->new_particles = tempI;
        }
        if (strcmp(actual_line, "new_particles_variance") == 0)
        {
            fscanf_s(file, "%d\n", &tempI);
            this->new_particles_variance = tempI;
        }
        if (strcmp(actual_line, "max_particles") == 0)
        {
            fscanf_s(file, "%d\n", &tempI);
            this->max_particles = tempI;
            this->particles.resize(this->max_particles);

        }
        if (strcmp(actual_line, "point") == 0)
        {
            fscanf_s(file, "%d\n", &tempI);
            this->opt_point = tempI == 1;
        }
        if (strcmp(actual_line, "texture") == 0)
        {
            fscanf_s(file, "%d\n", &tempI);
            this->opt_texture = tempI == 1;
        }
        if (strcmp(actual_line, "texture_img") == 0)
        {
            if (fscanf(file, "%127s", &tempS) != EOF)
                this->textureData = tempS;
        }
        if (strcmp(actual_line, "Position") == 0)
        {
            count++;
            fscanf_s(file, "%f %f %f\n", &temp.Position.x, &temp.Position.y, &temp.Position.z);
        }
        if (strcmp(actual_line, "Direction") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &temp.Direction.x, &temp.Direction.y, &temp.Direction.z);
        }
        if (strcmp(actual_line, "CurrentPosition") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &temp.currentPosition.x, &temp.currentPosition.y, &temp.currentPosition.z);
        }
        if (strcmp(actual_line, "Deviation") == 0)
        {
            fscanf_s(file, "%f %f %f\n", &temp.deviation.x, &temp.deviation.y, &temp.deviation.z);
        }
        if (strcmp(actual_line, "Color") == 0)
        {
            fscanf_s(file, "%f %f %f %f\n", &temp.Color.x, &temp.Color.y, &temp.Color.z, &temp.Color.w);
        }
        if (strcmp(actual_line, "FinColor") == 0)
        {
            fscanf_s(file, "%f %f %f %f\n", &temp.finColor.x, &temp.finColor.y, &temp.finColor.z, &temp.finColor.w);
        }
        if (strcmp(actual_line, "CurrentColor") == 0)
        {
            fscanf_s(file, "%f %f %f %f\n", &temp.currentColor.x, &temp.currentColor.y, &temp.currentColor.z, &temp.currentColor.w);
        }
        if (strcmp(actual_line, "Speed") == 0)
        {
            fscanf_s(file, "%f\n", &temp.speed);
        }
        if (strcmp(actual_line, "CurrentScale") == 0)
        {
            fscanf_s(file, "%f\n", &temp.currentScale);
        }
        if (strcmp(actual_line, "Scale") == 0)
        {
            fscanf_s(file, "%f\n", &temp.scale);
        }
        if (strcmp(actual_line, "ScaleFin") == 0)
        {
            fscanf_s(file, "%f\n", &temp.scaleFin);
        }
        if (strcmp(actual_line, "Size") == 0)
        {
            fscanf_s(file, "%f\n", &temp.size);
        }
        if (strcmp(actual_line, "Life") == 0)
        {
            fscanf_s(file, "%f\n", &temp.Life);
        }
        if (strcmp(actual_line, "tLife") == 0)
        {
            fscanf_s(file, "%f\n", &temp.tLife);
        }
        if (strcmp(actual_line, "Mass") == 0)
        {
            fscanf_s(file, "%f\n", &temp.mass);
            temp.id = count;
            this->particles[count] = temp;
        }

    }
}

void ParticleGenerator::Update(float dt)
{
    
    // add new particles 
    this->secondCounter+= dt * this->anim_speed;
    int newParticles;
    if (this->new_particles_variance==0)
    {
        newParticles = this->new_particles;
    }
    else {
        newParticles= this->new_particles + iterator[rand() % 2] * rand() % this->new_particles_variance;
        if (newParticles==0)
            newParticles++;
    }
     
    if (this->secondCounter >= 1.0f/ newParticles)
    {
        int unusedParticle = this->firstUnusedParticle();
        if (unusedParticle >= 0)
            this->respawnParticle(this->particles[unusedParticle]);
        
        this->secondCounter = .0f;
    }
    // update all particles
    aliveP.clear();
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
            aliveP.push_back(p);
        }
    }
    if (opt_blending)
    {
        quickSort(aliveP, 0, aliveP.size() - 1);
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
    
    for (Particle particle : this->aliveP)
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



int ParticleGenerator::partition(std::vector<Particle>& arr, int start, int end)
{
    Particle pivot = arr[end];
    int index = start;
    for (int i = start; i < end; i++)
    {
        if (arr[i].Life>0.0f)
        {
            if (arr[i].currentColor.w>pivot.currentColor.w)
            {
                Particle temp = arr[i];
                arr[i] = arr[index];
                arr[index] = temp;
                index++;
            }
        }
    }
    Particle temp = arr[end];
    arr[end] = arr[index];
    arr[index] = temp;
    return index; // returning pivotIndex
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
    float temp = this->scale_ini + iterator[rand() % 2] * modNumber(this->scale_ini_var);
    if (temp <= 0)temp = .1f;
    particle.scale = temp;
    temp = this->scale_fin + iterator[rand() % 2] * modNumber(this->scale_fin_var);
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
        min.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max.x - min.x))),
        min.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max.y - min.y))),
        min.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max.z - min.z)))
    );
    
    particle.Color = colorVariance(color_ini,color_ini_variance);

    particle.finColor = colorVariance(color_fin, color_fin_variance);

    temp = this->lifetime + iterator[rand() % 2] * modNumber(this->lifetime_var);
    if (temp <= 0)temp = .1f;
    particle.Life = temp;
    particle.tLife = particle.Life;
    particle.Direction = glm::vec3(
        this->direction.x + iterator[rand() % 2] * modNumber(this->direction_variance.x),
        this->direction.y + iterator[rand() % 2] * modNumber(this->direction_variance.y),
        this->direction.z + iterator[rand() % 2] * modNumber(this->direction_variance.z)
    );
    particle.Direction = glm::normalize(particle.Direction);
    particle.speed = this->speed + iterator[rand() % 2] * modNumber(this->speed_variance);
    particle.mass = this->mass + iterator[rand() % 2] * modNumber(this->mass_variance);

    temp = this->size_particle + iterator[rand() % 2] * modNumber(this->size_variance);
    if (temp <= 0)temp = .1f;
    particle.size = temp;

    particle.deviation = glm::vec3(
        this->dirDesviation.x + this->iterator[rand() % 2] * this->modNumber(this->dirDesviation_var.x),
        this->dirDesviation.y + this->iterator[rand() % 2] * this->modNumber(this->dirDesviation_var.y),
        this->dirDesviation.z + this->iterator[rand() % 2] * this->modNumber(this->dirDesviation_var.z)
    );
}

glm::vec4 ParticleGenerator::colorVariance(glm::vec4 color, glm::vec4 variance)
{
    glm::vec4 temp4 = glm::vec4(
        color.x + iterator[rand() % 2] * modNumber(variance.x),
        color.y + iterator[rand() % 2] * modNumber(variance.y),
        color.z + iterator[rand() % 2] * modNumber(variance.z),
        color.w + iterator[rand() % 2] * modNumber(variance.w)
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

float ParticleGenerator::modNumber(float div)
{
    if (div==0)return 0.0f;
    else
    {
        return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / div));
    }
}

