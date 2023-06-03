#ifndef OBJ_H
#define OBJ_H

#include <stdio.h>
#include <stdlib.h>
#include <subobj.h>

#include <Shader.h>


struct material
{
    std::string name="";
    std::string map_ka="";
    float kd[3] = {.7f,.7f,.7f}, ka[3] = {.0f,.0f,.0f}, ks[3] = {.7f,.7f,.7f};
    float ns=20.0, d=1.0;
};
struct face
{
    int vertex[4]{ -1,-1,-1,-1 };
    int texture[4]{ -1,-1,-1,-1 };
    glm::vec3 normal=glm::vec3(.0f);
};

struct baseNormal {
    glm::vec3 normal = { .0f,.0f,.0f };
    float adjacents = 0.0f;
};

//struct para el subMesh
struct mesh {
    std::string nombre="";//nombre del mesh
    subObj* meshs=nullptr;//data para el draw
    int s = 0;//sombras
    std::vector <GLfloat> obj_data;//data de los triangulos
    material mat;//data del material
    std::vector <face> vector_faces;//data de las caras (calculo de normales)
};

class obj
{
	public:
		obj(std::string fileName);
		~obj();
        

        glm::vec3 getTam() {
            return scale;
        }
        void setTam(float sx,float sy,float sz) {
            scale = {sx,sy,sz};
        }

        glm::vec3 getPos() {
            return tras_vector;
        }
        
        bool getSelect() {
            return selected;
        }
        void setSelect(bool value) {
            selected = value;
        }

        bool onClick(int x, int y);
        

        void draw(Shader basic_shader);

        void rotateObj(float x, float y);
        void traslateObj(float x, float y,float z);

	private:
        // Variables para los shaders
        bool selected;
        glm::mat4 model;
            //colores
        float difuse_color[4] = { .7f,.7f,.7f,1.0f };
        float specular_color[4] = { .7f,.7f,.7f,1.0f };
        float ambient_color[4] = { .0f,.0f,.0f,1.0f };
        float shininess = 10.0;
        float bounding_color[4] = { 0.0f,0.0f,1.0f,1.0f };

        // Variables de datos temporales de objetos
        std::vector <glm::vec3> temp_vertices;
        std::vector <glm::vec3> temp_normals;
        std::vector <glm::vec2> temp_textures;
        std::vector <material> vector_materials;

        //datos de objeto
        std::string file_path;

        //variables de control de objetos
        glm::vec3 max = { -INFINITY,-INFINITY ,-INFINITY }, min = { INFINITY,INFINITY,INFINITY };
        glm::vec3 scale = { 1.0f,1.0f,1.0f };
        glm::vec3 tras_vector = { 0.0f,0.0f,-2.0f };
        glm::vec3 center_vector = { 0.0f,0.0f,0.0f };
        glm::vec3 scale_vector = { 1.0f,1.0f,1.0f };
        glm::quat MyQuaternion;
        glm::mat4 rotation = glm::mat4(1.0f);
        glm::vec3 FRONT = { 0,0,1 }, UP = { 0,1,0 };
        float x_angle = 0.0f;
        float y_angle = 0.0f;
        bool texture;
        glm::mat4 ident = glm::mat4(1.0f);
        //calculo de normales de no tener
        std::vector<baseNormal> calcNormal;
        std::vector<mesh> vector_mesh;

        //bounding box
        GLuint boundingVBO, boundingVAO;

        void loadObj(std::string filename);
        void loadVertex(int vi, int ni, int ti, std::vector<glm::vec3> auxVertices, std::vector<glm::vec2> auxTextures, std::vector<glm::vec3> auxNormals);
        void loadVertexNormal(int vi, int ti, std::vector<glm::vec3> auxVertices, std::vector<glm::vec2> auxTextures, std::vector<baseNormal> normal_vertex);
        std::vector<std::string> split(std::string const& original, char separator);
};
#endif