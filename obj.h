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
        void draw(GLint objectColorLoc, GLint modelLoc);
        struct mat
        {
            string name;
            glm::vec3 color;
        };
        struct face
        {
            vector<int> vertex;
            vector<int> normals;
        };

	private:
        // Variables para los shaders
        float x_angle = 0.0f;
        float y_angle = 0.0f;
        glm::vec3 tras_vector = { 0.0f,0.0f,-3.0f };
        glm::vec3 scale_vector = { 1.0f,1.0f,1.0f };
        glm::vec3 object_color={1.0f,1.0f,1.0f };
        
        // Variables de datos de objetos
        vector <string> aux;
        vector <glm::vec3> temp_vertices;
        vector <glm::vec3> temp_normals;
        vector <face> faces;
        vector <mat> materials;
        vector <GLfloat> objVertex;
        glm::vec3 max, min;
        int normal, vertice;
        subObj* mesh;

        void loadObj(string filename);

        void loadVertex(string const& temp);

        vector<string> split(string const& original, char separator);

};

inline obj::obj(string fileName)
{
    loadObj(fileName);
    float dx,dy,dz,df;
    dx = abs(max.x - min.x);
    dy = abs(max.y - min.y);
    dz = abs(max.z - min.z);
    if (dx>dy>dz)
    {
        df = dx;
    }
    else
    {
        if (dy>dz)
        {
            df = dy;
        }
        else
        {
            df = dz;
        }
    }
    scale_vector.x = 1.0f/df;
    scale_vector.y = 1.0f/df;
    scale_vector.z = 1.0f/df;
    GLfloat* thearray = objVertex.data();
    mesh=new subObj(thearray, sizeof(thearray) * objVertex.size());
    /*objVertex.clear();
    temp_normals.clear();
    temp_vertices.clear();*/
}

inline obj::~obj()
{
}

inline void obj::draw(GLint objectColorLoc, GLint modelLoc)
{
    // Use cooresponding shader when setting uniforms/drawing objects
    glUniform3f(objectColorLoc, object_color.x,object_color.y,object_color.z);
    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), tras_vector) *
        glm::rotate(glm::mat4(1.0f), y_angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), x_angle, glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), scale_vector);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mesh->draw();
}



inline void obj::loadObj(string filename)
{
    string myText;
    ifstream MyReadFile(filename);
    vector<string> temp;
    size_t found;
    glm::vec3 vertex;
    glm::vec3 normal;
    face tempFace;
    mat tempMaterial;
    while (getline(MyReadFile, myText)) {
        if (myText.size() < 2)
            continue;
        if (myText[0] != 'f' && myText[0] != 'v' && myText[1] != 'n' && myText[0]!='u')
            continue;
        if (myText[1] == 't')
            continue;
        found = myText.find("  ");
        if (found != string::npos)
            myText.erase(myText.begin() + found);
        if (myText[myText.size() - 1]==' ')
            myText.erase(myText.begin()+myText.size() - 1);
        temp = split(myText, ' ');
        if (strcmp(temp[0].data(), "usemtl") == 0)
        {
            if (materials.empty()) continue;
            tempMaterial.name = temp[1];
            for (int i = 0; i < materials.size(); i++)
            {
                if (strcmp(materials[i].name.data(), temp[1].data()) != 0) continue;
                tempMaterial.color = materials[i].color;
            }
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
        }
        else if (strcmp(temp[0].data(), "vn")==0)
        {
            normal.x = stof(temp[1]);
            normal.y = stof(temp[2]);
            normal.z = stof(temp[3]);
            temp_normals.push_back(normal);
        }
        else if (strcmp(temp[0].data(), "f") == 0)
        {
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
        }
    }
}

inline void obj::loadVertex(string const& temp)
{
    aux = split(temp, '/');
    vertice = stoi(aux[0]);
    normal = stoi(aux[2]);
    vertice < 0 ? vertice = temp_vertices.size() + vertice : vertice--;
    normal < 0 ? normal = temp_normals.size() + normal : normal--;

    objVertex.push_back(temp_vertices[vertice].x);
    objVertex.push_back(temp_vertices[vertice].y);
    objVertex.push_back(temp_vertices[vertice].z);

    objVertex.push_back(temp_normals[normal].x);
    objVertex.push_back(temp_normals[normal].y);
    objVertex.push_back(temp_normals[normal].z);

    if (materials.empty()) {
        objVertex.push_back(object_color.x);
        objVertex.push_back(object_color.y);
        objVertex.push_back(object_color.z);
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