#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

#include <subobj.h>

class obj
{
	public:
		obj(string fileName);
		~obj();
        
        float* getDifuse_color() {
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
        struct mat
        {
            string name;
            string map_ka;
            glm::vec3 kd,ka,ks;
            float ns, d;
        };
        struct face
        {
            mat material;
            vector<int> vertex;
            glm::vec3 normal;
        };

        struct baseNormal {
            glm::vec3 normal={.0f,.0f,.0f};
            float adjacents=0.0f;
        };

        struct mesh {
            subObj* meshs;
            //variables para dibujar objetos
            vector <GLfloat> obj_data;
            mat material;
        };

        void draw(Shader basic_shader);

        void rotateObj(float x, float y);
        void traslateObj(float x, float y,float z);

	private:
        // Variables para los shaders
        float x_angle = 0.0f;
        float y_angle = 0.0f;
        glm::vec3 tras_vector = { 0.0f,0.0f,-2.0f };
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
        vector <face> vector_faces;
        vector <mat> vector_materials;

        

        //variables de control de objetos
        glm::vec3 max, min;
        glm::vec3 scale = { 1.0f,1.0f,1.0f };
        bool selected;
        glm::mat4 model;

        //calculo de normales de no tener
        vector<baseNormal> calcNormal;

        vector<mesh> vector_mesh;

        //bounding box
        GLuint boundingVBO, boundingVAO;


        

        void loadObj(string filename);

        void loadVertex(string const& temp);

        vector<string> split(string const& original, char separator);

        glm::vec3 getVertex(string temp);

        void setVertexNormal(face temp, int i);
};

inline obj::obj(string fileName)
{
    string aux = fileName;
    int n = aux.size() - 1;
    aux[n] = 'l';
    aux[n - 1] = 't';
    aux[n - 2] = 'm';
    ifstream MyReadFile(aux);
    if (MyReadFile.is_open())
    {
        vector<string> temp;
        mat tempMaterial;
        while (getline(MyReadFile, aux)) {
            if (aux.size() < 1)continue;
            temp = split(aux, ' ');
            if (temp[0][0] == 'K')
            {
                if (temp[0][1] == 'a')
                    vector_materials.back().ka = { stof(temp[1]),stof(temp[2]),stof(temp[3]) };
                else if (temp[0][1] == 'd')
                    vector_materials.back().kd = { stof(temp[1]),stof(temp[2]),stof(temp[3]) };
                else if (temp[0][1] == 's')
                    vector_materials.back().ks = { stof(temp[1]),stof(temp[2]),stof(temp[3]) };
            }
            else if (temp[0][0] == 'N')
                vector_materials.back().ns = stof(temp[1].data());
            else if (temp[0][0] == 'd')
                vector_materials.back().d = stof(temp[1].data());
            else if (temp[0][0] == 'm')
                vector_materials.back().map_ka = temp[1];
            else if (strcmp(temp[0].data(), "newmtl") == 0)
            {
                tempMaterial.name = temp[1];
                vector_materials.push_back(tempMaterial);
            }
        }

    }
    else {
        cout << "No se encontro el archivo .mtl";
    }
    loadObj(fileName);
    if (temp_normals.empty())
    {
        for (int i = 0; i < vector_faces.size(); i++)
        {
            setVertexNormal(vector_faces[i], 0);
            setVertexNormal(vector_faces[i], 1);
            setVertexNormal(vector_faces[i], 2);
            if (vector_faces[i].vertex.size() > 3)
            {
                setVertexNormal(vector_faces[i], 0);
                setVertexNormal(vector_faces[i], 2);
                setVertexNormal(vector_faces[i], 3);
            }
        }
    }
    float dx, dy, dz, df;
    dx = abs(max.x - min.x);
    dy = abs(max.y - min.y);
    dz = abs(max.z - min.z);
    if (dx > dy > dz)
    {
        df = dx;
    }
    else
    {
        if (dy > dz)
        {
            df = dy;
        }
        else
        {
            df = dz;
        }
    }
    GLfloat bounding[] = {
        min.x,max.y,max.z,
        max.x,max.y,max.z,
        min.x,min.y,max.z,
        max.x,min.y,max.z, 
        max.x,min.y,max.z, 
        max.x,max.y,max.z, 
        min.x,min.y,max.z, 
        min.x,max.y,max.z,//cuadrado frontal
        max.x,max.y,min.z, 
        min.x,max.y,min.z,
        max.x,max.y,min.z, 
        max.x,min.y,min.z, 
        min.x,min.y,min.z, 
        max.x,min.y,min.z,
        min.x,min.y,min.z, 
        min.x,max.y,min.z, //cuadrado trasero
        max.x,max.y,max.z, 
        max.x,max.y,min.z, 
        max.x,min.y,max.z, 
        max.x,min.y,min.z, 
        min.x,max.y,max.z, 
        min.x,max.y,min.z, 
        min.x,min.y,max.z,
        min.x,min.y,min.z
    };
    glGenVertexArrays(1, &boundingVAO);
    // 1. bind Vertex Array Object
    glBindVertexArray(boundingVAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glGenBuffers(1, &boundingVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boundingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 72, bounding, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    scale_vector.x = 1.0f / df;
    scale_vector.y = 1.0f / df;
    scale_vector.z = 1.0f / df;
    max = max / df;
    min = min / df;

    
    vector<string> base = split(fileName, '\\');
    string image;
    base.pop_back();

    for (int i = 0; i < vector_mesh.size(); i++)
    {
        for (int i = 0; i < base.size(); i++)
        {
            image.append(base[i]);
            image.append("\\");
        }
        image.append(vector_mesh[i].material.map_ka);
        vector_mesh[i].meshs = new subObj(vector_mesh[i].obj_data.data(), sizeof(GLfloat*) * vector_mesh[i].obj_data.size(), image);
        image.clear();
    }

}

inline obj::~obj()
{
    for (int i = 0; i < vector_mesh.size(); i++)
    {
        vector_mesh[i].meshs->~subObj();
    }
    vector_mesh.clear();
    temp_normals.clear();
    temp_vertices.clear();
    temp_textures.clear();
    glDeleteVertexArrays(1, &boundingVAO);
    glDeleteBuffers(1, &boundingVBO);
}

inline void obj::draw(Shader basic_shader)
{
    // Use cooresponding shader when setting uniforms/drawing objects
    /*uniform vec4 Ka, Kd, Ks;
    uniform float alfa, shininess;*/
    GLint ambientColorLoc = glGetUniformLocation(basic_shader.Program, "Ka");
    GLint difuseColorLoc = glGetUniformLocation(basic_shader.Program, "Kd");
    GLint specularColorLoc = glGetUniformLocation(basic_shader.Program, "Ks");
    GLint shininessLoc = glGetUniformLocation(basic_shader.Program, "shininess");
    GLint modelLoc = glGetUniformLocation(basic_shader.Program, "model");

    
    model =
        glm::translate(glm::mat4(1.0f), tras_vector) *
        glm::rotate(glm::mat4(1.0f), y_angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), x_angle, glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), scale_vector * scale);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < vector_mesh.size(); i++)
    {
        if (vector_materials.empty())
        {
            glUniform3f(ambientColorLoc, ambient_color[0], ambient_color[1], ambient_color[2]);
            glUniform3f(difuseColorLoc, difuse_color[0], difuse_color[1], difuse_color[2]);
            glUniform3f(specularColorLoc, specular_color[0], difuse_color[1], difuse_color[2]);
            glUniform1f(shininessLoc, shininess);
        }
        else
        {
            glUniform3f(ambientColorLoc, vector_mesh[i].material.ka.x, vector_mesh[i].material.ka.y, vector_mesh[i].material.ka.z);
            glUniform3f(difuseColorLoc, vector_mesh[i].material.kd.x, vector_mesh[i].material.kd.y, vector_mesh[i].material.kd.z);
            glUniform3f(specularColorLoc, vector_mesh[i].material.ks.x, vector_mesh[i].material.ks.y, vector_mesh[i].material.ks.z);
            glUniform1f(shininessLoc, vector_mesh[i].material.ns);
        }
        vector_mesh[i].meshs->draw();
    }
    if (selected)
    {
        glUniform3f(difuseColorLoc, bounding_color[0], bounding_color[1], bounding_color[2]);
        glBindVertexArray(boundingVAO);
        glDrawArrays(GL_LINES, 0, 72);
    }

}

inline bool obj::onClick(int x, int y)
{

    cout << x << " " << y << endl;
    return false;
}



inline void obj::loadObj(string filename)
{
    string myText;
    ifstream MyReadFile(filename);
    vector<string> temp;
    vector<string> aux;

    size_t found;
    glm::vec3 vertex;
    glm::vec3 normal;
    glm::vec2 textura;
    glm::vec3 a, b, c, d, v1, v2, n, nt;

    face tempFace;
    mat tempMaterial;
    baseNormal tempCalcNormal;
    mesh tempMesh;

    int vertice;
    while (getline(MyReadFile, myText)) {
        if (myText.size() < 2)
            continue;
        if (myText[0] != 'f' && myText[0] != 'v' && myText[1] != 'n' && myText[0] != 'u')
            continue;
        found = myText.find("  ");
        if (found != string::npos)
            myText.erase(myText.begin() + found);
        if (myText[myText.size() - 1] == ' ')
            myText.erase(myText.begin() + myText.size() - 1);
        temp = split(myText, ' ');
        if (strcmp(temp[0].data(), "usemtl") == 0)
        {
            if (vector_materials.empty()) continue;
            tempMaterial.name = temp[1];
            for (int i = 0; i < vector_materials.size(); i++)
            {
                if (strcmp(vector_materials[i].name.data(), temp[1].data()) != 0) continue;
                tempMaterial = vector_materials[i];
            }
            tempMesh.material = tempMaterial;
            vector_mesh.push_back(tempMesh);

        }
        else if (strcmp(temp[0].data(), "v") == 0)
        {
            vertex.x = stof(temp[1]);
            vertex.y = stof(temp[2]);
            vertex.z = stof(temp[3]);
            if (vertex.x < min.x)min.x = vertex.x;
            if (vertex.x > max.x)max.x = vertex.x;
            if (vertex.y < min.y)min.y = vertex.y;
            if (vertex.y > max.y)max.y = vertex.y;
            if (vertex.z < min.z)min.z = vertex.z;
            if (vertex.z > max.z)max.z = vertex.z;
            temp_vertices.push_back(vertex);
            calcNormal.push_back(tempCalcNormal);

        }
        else if (strcmp(temp[0].data(), "vn") == 0)
        {
            normal.x = stof(temp[1]);
            normal.y = stof(temp[2]);
            normal.z = stof(temp[3]);
            temp_normals.push_back(normal);
        }
        else if (strcmp(temp[0].data(), "vt") == 0)
        {
            textura.x = stof(temp[1]);
            textura.y = stof(temp[2]);
            temp_textures.push_back(textura);
        }
        else if (strcmp(temp[0].data(), "f") == 0)
        {
            a = getVertex(temp[1]);
            b = getVertex(temp[2]);
            c = getVertex(temp[3]);
            n = glm::cross((a - b), (a - c));
            if (!temp_normals.empty())
            {
                loadVertex(temp[1]);
                loadVertex(temp[2]);
                loadVertex(temp[3]);
                if (temp.size() < 5)continue;
                loadVertex(temp[1]);
                loadVertex(temp[3]);
                loadVertex(temp[4]);
            }
            else
            {
                tempFace.vertex.clear();
                for (int i = 1; i < temp.size(); i++)
                {
                    aux = split(temp[i], '/');
                    vertice = stoi(aux[0]);
                    vertice < 0 ? vertice = temp_vertices.size() + vertice : vertice--;
                    tempFace.vertex.push_back(vertice);
                    nt = calcNormal[vertice].normal * calcNormal[vertice].adjacents;
                    nt = nt + n;
                    calcNormal[vertice].adjacents++;
                    calcNormal[vertice].normal = nt / calcNormal[vertice].adjacents;
                }
                tempFace.normal = n;
                if (!vector_materials.empty())tempFace.material = tempMaterial;
                vector_faces.push_back(tempFace);

            }
        }
    }
}

inline void obj::loadVertex(string const& temp)
{
    vector<string> aux;
    int vertice, normal, textures;
    aux = split(temp, '/');
    vertice = stoi(aux[0]);
    normal = stoi(aux[2]);
    vertice < 0 ? vertice = temp_vertices.size() + vertice : vertice--;
    normal < 0 ? normal = temp_normals.size() + normal : normal--;
    vector_mesh.back().obj_data.push_back(temp_vertices[vertice].x);
    vector_mesh.back().obj_data.push_back(temp_vertices[vertice].y);
    vector_mesh.back().obj_data.push_back(temp_vertices[vertice].z);

    vector_mesh.back().obj_data.push_back(temp_normals[normal].x);
    vector_mesh.back().obj_data.push_back(temp_normals[normal].y);
    vector_mesh.back().obj_data.push_back(temp_normals[normal].z);


    if (temp_textures.empty())
    {
        cout << "sin texturas";
    }
    else {
        textures = stoi(aux[1]);
        textures < 0 ? textures = temp_normals.size() + textures : textures--;
        vector_mesh.back().obj_data.push_back(temp_textures[textures].x);
        vector_mesh.back().obj_data.push_back(temp_textures[textures].y);
    }


}

inline vector<string> obj::split(string const& original, char separator) {
    vector<string> results;
    string::const_iterator start = original.begin();
    string::const_iterator end = original.end();
    string::const_iterator next = find(start, end, separator);
    while (next != end) {
        results.push_back(string(start, next));
        start = next + 1;
        next = find(start, end, separator);
    }
    results.push_back(string(start, next));
    return results;
}

inline glm::vec3 obj::getVertex(string temp)
{
    vector<string> aux;
    int vertice;
    aux = split(temp, '/');
    vertice = stoi(aux[0]);
    vertice < 0 ? vertice = temp_vertices.size() + vertice : vertice--;
    return { temp_vertices[vertice] };
}

inline void obj::setVertexNormal(face temp, int i) {
    vector_mesh[i].obj_data.push_back(temp_vertices[temp.vertex[i]].x);
    vector_mesh[i].obj_data.push_back(temp_vertices[temp.vertex[i]].y);
    vector_mesh[i].obj_data.push_back(temp_vertices[temp.vertex[i]].z);

    vector_mesh[i].obj_data.push_back(calcNormal[temp.vertex[i]].normal.x);
    vector_mesh[i].obj_data.push_back(calcNormal[temp.vertex[i]].normal.y);
    vector_mesh[i].obj_data.push_back(calcNormal[temp.vertex[i]].normal.z);

}

inline void obj::rotateObj(float x, float y)
{
    x_angle += x;
    y_angle += y;
}

inline void obj::traslateObj(float x, float y, float z) {
    tras_vector.x += x;
    tras_vector.y += y;
    tras_vector.z += z;
}