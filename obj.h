#pragma once


#include <subobj.h>

#include <Shader.h>

//using namespace std;

struct mat
{
    string name;
    string map_ka;
    float kd[3] = {.7f,.7f,.7f}, ka[3] = {.0f,.0f,.0f}, ks[3] = {.7f,.7f,.7f};
    float ns=20.0, d=1.0;
};
struct face
{
    mat material;
    vector<int> vertex;
    vector<int> texture;
    glm::vec3 normal;
};

struct baseNormal {
    glm::vec3 normal = { .0f,.0f,.0f };
    float adjacents = 0.0f;
};

struct mesh {
    string nombre;
    subObj* meshs;
    //variables para dibujar objetos
    vector <GLfloat> obj_data;
    mat material;
    vector <face> vector_faces;
};

class obj
{
	public:
		obj(string fileName);
		~obj();
        
        /*float* getDifuse_color() {
            return difuse_color;
        }
        void setDifuse_color(float* color) {
            difuse_color[0] = color[0];
            difuse_color[1] = color[1];
            difuse_color[2] = color[2];
        }
        float* getSpecular_color() {
            return specular_color;
        }
        void setSpecular_color(float* color) {
            specular_color[0] = color[0];
            specular_color[1] = color[1];
            specular_color[2] = color[2];
        }
        float* getAmbient_color() {
            return ambient_color;
        }
        void setAmbient_color(float* color) {
            ambient_color[0] = color[0];
            ambient_color[1] = color[1];
            ambient_color[2] = color[2];
        }
        float getShininess() {
            return shininess;
        }
        void setShininess(float color) {
            shininess = color;
        }*/
        vector<mesh> getMesh() {
            return vector_mesh;
        }
        void setMesh(vector<mesh> sub) {
            vector_mesh=sub;
        }


        float* getBouningColor() {
            return bounding_color;
        }
        void setBouningColor(float* color) {
            bounding_color[0] = color[0];
            bounding_color[1] = color[1];
            bounding_color[2] = color[2];
        }

        glm::vec3 getTam() {
            return scale;
        }
        void setTam(float sx,float sy,float sz) {
            scale = {sx,sy,sz};
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
        float x_angle = 0.0f;
        float y_angle = 0.0f;
        glm::vec3 tras_vector = { 0.0f,0.0f,-2.0f };
        glm::vec3 center_vector = { 0.0f,0.0f,0.0f };
        glm::vec3 scale_vector = { 1.0f,1.0f,1.0f };
        //colores
        float difuse_color[4] = {.7f,.7f,.7f,1.0f};
        float specular_color[4] = { .7f,.7f,.7f,1.0f };
        float ambient_color[4] = { .0f,.0f,.0f,1.0f };
        float shininess = 20.0;
        float bounding_color[4] = { 0.0f,0.0f,1.0f,1.0f };
        // Variables de datos temporales de objetos
        vector <glm::vec3> temp_vertices;
        vector <glm::vec3> temp_normals;
        vector <glm::vec2> temp_textures;
        vector <mat> vector_materials;

        string file_path;

        //variables de control de objetos
        glm::vec3 max = { -INFINITY,-INFINITY ,-INFINITY }, min = { INFINITY,INFINITY,INFINITY };
        glm::vec3 scale = { 1.0f,1.0f,1.0f };
        bool selected;
        glm::mat4 model;
        glm::mat4 rotation = glm::mat4(1.0f);
        glm::vec3 FRONT = { 0,0,1 }, UP = { 0,1,0 };
        bool texture;

        //calculo de normales de no tener
        vector<baseNormal> calcNormal;

        vector<mesh> vector_mesh;

        //bounding box
        GLuint boundingVBO, boundingVAO;


        

        void loadObj(string filename);

        void loadVertex(string const& temp);

        vector<string> split(string const& original, char separator);

        glm::vec3 getVertex(string temp);

        void setVertexNormal(face temp, int i, int v);
};
