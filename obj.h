#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <subobj.h>

GLenum modo = GL_FILL;

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
            mat material;
            vector<int> vertex;
        };

	private:
        // Variables para los shaders
        float x_angle = 0.0f;
        float y_angle = 0.0f;
        glm::vec3 tras_vector = { 0.0f,0.0f,-3.0f };
        glm::vec3 scale_vector = { 1.0f,1.0f,1.0f };
        glm::vec3 object_color={0.7f,0.7f,0.7f };
        
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

        void loadVertex(string const& temp, mat actual);

        vector<string> split(string const& original, char separator);

        glm::vec3 getVertex(string temp);

        void setVertexNormal(glm::vec3 vNormal, string temp, mat actual);

};

inline obj::obj(string fileName)
{
    string aux = fileName;
    int n = aux.size() - 1;
    aux[n] = 'l';
    aux[n - 1] = 't';
    aux[n - 2] = 'm';
    cout << aux;
    ifstream MyReadFile(aux);
    if (MyReadFile.is_open())
    {
        vector<string> temp;
        mat tempMaterial;
        while (getline(MyReadFile, aux)) {
            if (aux.size() < 1)continue;
            if (aux[0] != 'n' && aux[0] != 'K')continue;
            temp = split(aux, ' ');
            if (strcmp(temp[0].data(), "newmtl") == 0)
            {
                tempMaterial.name = temp[1]; 
                materials.push_back(tempMaterial);
            }
            else if(strcmp(temp[0].data(), "Kd") == 0)
            {
                materials.back().color = { stof(temp[1]),stof(temp[2]),stof(temp[3])};
            }
        }
    }
    else {
        cout << "No se encontro el archivo .mtl";
    }
    
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
}

inline obj::~obj()
{
    mesh->~subObj();
    objVertex.clear();
    temp_normals.clear();
    temp_vertices.clear();
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
    glPolygonMode(GL_FRONT_AND_BACK,modo);
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
    glm::vec3 a, b, c, d,v1,v2,n,nt;
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
                loadVertex(temp[1],tempMaterial);
                loadVertex(temp[2], tempMaterial);
                loadVertex(temp[3], tempMaterial);
                if (temp.size() < 5)continue;
                loadVertex(temp[1], tempMaterial);
                loadVertex(temp[3], tempMaterial);
                loadVertex(temp[4], tempMaterial);
            }
            else
            {
                a = getVertex(temp[1]);
                b = getVertex(temp[2]);
                c = getVertex(temp[3]);
                n = glm::cross((a - b), (a - c));
                setVertexNormal(n, temp[1], tempMaterial);
                setVertexNormal(n, temp[2], tempMaterial);
                setVertexNormal(n, temp[3], tempMaterial);
                if (temp.size() > 4) {
                    d = getVertex(temp[4]);
                    setVertexNormal(n, temp[1], tempMaterial);
                    setVertexNormal(n, temp[3], tempMaterial);
                    setVertexNormal(n, temp[4], tempMaterial);
                }
                
                /*tempFace.vertex.clear();
                for (int i = 1; i < temp.size(); i++)
                {
                    aux = split(temp[i], '/');
                    vertice = stoi(aux[0]);
                    vertice < 0 ? vertice = temp_vertices.size() + vertice : vertice--;
                    tempFace.vertex.push_back(vertice);
                }
                if(materials.empty())tempFace.material = tempMaterial;
                faces.push_back(tempFace);*/
            }
        }
    }
}

inline void obj::loadVertex(string const& temp, mat actual)
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
    else
    {
        objVertex.push_back(actual.color.x);
        objVertex.push_back(actual.color.y);
        objVertex.push_back(actual.color.z);
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
    aux = split(temp, '/');
    vertice = stoi(aux[0]);
    vertice < 0 ? vertice = temp_vertices.size() + vertice : vertice--;
    return {temp_vertices[vertice]};
}

inline void obj::setVertexNormal(glm::vec3 vNormal, string temp, mat actual)
{
    aux = split(temp, '/');
    vertice = stoi(aux[0]);
    vertice < 0 ? vertice = temp_vertices.size() + vertice : vertice--;
    objVertex.push_back(temp_vertices[vertice].x);
    objVertex.push_back(temp_vertices[vertice].y);
    objVertex.push_back(temp_vertices[vertice].z);

    objVertex.push_back(vNormal.x);
    objVertex.push_back(vNormal.y);
    objVertex.push_back(vNormal.z);

    if (materials.empty()) {
        objVertex.push_back(object_color.x);
        objVertex.push_back(object_color.y);
        objVertex.push_back(object_color.z);
    }
    else
    {
        objVertex.push_back(actual.color.x);
        objVertex.push_back(actual.color.y);
        objVertex.push_back(actual.color.z);
    }
}
