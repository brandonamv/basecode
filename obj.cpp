#include "obj.h"
#include <glm/gtx/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
using namespace std;
obj::obj(string fileName)
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
        material tempMaterial;
        while (getline(MyReadFile, aux)) {
            if (aux.size() < 1)continue;
            temp = split(aux, ' ');
            if (temp[0][0] == 'K')
            {
                if (temp[0][1] == 'a') {
                    vector_materials.back().ka[0] = stof(temp[1]);
                    vector_materials.back().ka[1] = stof(temp[2]);
                    vector_materials.back().ka[2] = stof(temp[3]);
                }
                else if (temp[0][1] == 'd') {
                    vector_materials.back().kd[0] = stof(temp[1]);
                    vector_materials.back().kd[1] = stof(temp[2]);
                    vector_materials.back().kd[2] = stof(temp[3]);
                }
                else if (temp[0][1] == 'd') {
                    vector_materials.back().ks[0] = stof(temp[1]);
                    vector_materials.back().ks[1] = stof(temp[2]);
                    vector_materials.back().ks[2] = stof(temp[3]);
                }
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
    float dx, dy, dz, df;
    dx = abs(max.x - min.x);
    dy = abs(max.y - min.y);
    dz = abs(max.z - min.z);
    glm::vec3 diff = { dx / 2 ,dy / 2 ,dz / 2 };
    if (-diff.x>min.x)
    {
        center_vector.x += min.x + diff.x;
    }
    if (diff.x < max.x)
    {
        center_vector.x -= max.x - diff.x;
    }
    if (-diff.y > min.y)
    {
        center_vector.y += min.y + diff.y;
    }
    if (diff.y < max.y)
    {
        center_vector.y -= max.y - diff.y;
    }
    if (-diff.z > min.z)
    {
        center_vector.z -= min.z + diff.z;
    }
    if (diff.z < max.z)
    {
        center_vector.z += max.z - diff.z;
    }

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat*) * 72, bounding, GL_STATIC_DRAW);
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
        image.append(vector_mesh[i].mat.map_ka);
        vector_materials.empty() ?
            vector_mesh[i].meshs = new subObj(vector_mesh[i].obj_data.data(), sizeof(GLfloat*) * vector_mesh[i].obj_data.size(), "") :
            vector_mesh[i].meshs = new subObj(vector_mesh[i].obj_data.data(), sizeof(GLfloat*) * vector_mesh[i].obj_data.size(), image);
        image.clear();
    }

}

obj::~obj()
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

void obj::draw(Shader basic_shader)
{
    // Use cooresponding shader when setting uniforms/drawing objects
    GLint ambientColorLoc = glGetUniformLocation(basic_shader.Program, "Ka");
    GLint difuseColorLoc = glGetUniformLocation(basic_shader.Program, "Kd");
    GLint specularColorLoc = glGetUniformLocation(basic_shader.Program, "Ks");
    GLint shininessLoc = glGetUniformLocation(basic_shader.Program, "shininess");
    GLint modelLoc = glGetUniformLocation(basic_shader.Program, "model");
    GLint boundingLoc = glGetUniformLocation(basic_shader.Program, "bounding");
    GLint texturizedLoc = glGetUniformLocation(basic_shader.Program, "texturized");

    glm::mat4 center = glm::translate(ident, center_vector);
    glm::mat4 scaling = glm::scale(ident, scale_vector * scale);
    glm::mat4 traslation = glm::translate(ident, tras_vector);
    model = traslation * rotation * scaling * center;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(boundingLoc, false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < vector_mesh.size(); i++)
    {

        glUniform3f(ambientColorLoc, vector_mesh[i].mat.ka[0], vector_mesh[i].mat.ka[1], vector_mesh[i].mat.ka[2]);
        glUniform3f(difuseColorLoc, vector_mesh[i].mat.kd[0], vector_mesh[i].mat.kd[1], vector_mesh[i].mat.kd[2]);
        glUniform3f(specularColorLoc, vector_mesh[i].mat.ks[0], vector_mesh[i].mat.ks[1], vector_mesh[i].mat.ks[2]);
        glUniform1f(shininessLoc, vector_mesh[i].mat.ns);

        glUniform1i(texturizedLoc, vector_mesh[i].meshs->getTexture());
        vector_mesh[i].meshs->draw();
    }
    if (selected)
    {
        glUniform3f(difuseColorLoc, bounding_color[0], bounding_color[1], bounding_color[2]);
        glUniform1i(boundingLoc, true);
        glBindVertexArray(boundingVAO);
        glDrawArrays(GL_LINES, 0, sizeof(GLfloat*) * 6);
    }

}

bool obj::onClick(int x, int y)
{

    return false;
}



void obj::loadObj(string filename)
{
    FILE* file;
    char linea[128] = {0};

    if (fopen_s(&file, filename.data(), "r") != 0)
    {
        printf("Error al cargar el objeto!\n");
        return;
    }

    file_path = filename;

    vector<glm::vec3> auxVertices;
    vector<glm::vec2> auxTextures;
    vector<glm::vec3> auxNormals;

    vector<baseNormal> normal_vertex;

    while (1)
    {
        if (fscanf(file, "%127s", &linea) == EOF)
            break;

        if (strcmp(linea, "v") == 0) {
            glm::vec3 vertice;
            fscanf_s(file, "%f %f %f\n", &vertice.x, &vertice.y, &vertice.z);
            auxVertices.push_back(vertice);
            normal_vertex.push_back({ vertice,0.0f });
            if (vertice.x > max.x)
                max.x = vertice.x;
            if (vertice.x < min.x)
                min.x = vertice.x;
            if (vertice.y > max.y)
                max.y = vertice.y;
            if (vertice.y < min.y)
                min.y = vertice.y;
            if (vertice.z > max.z)
                max.z = vertice.z;
            if (vertice.z < min.z)
                min.z = vertice.z;
        } 
        else if (strcmp(linea, "o") == 0){
            mesh temp_mesh;
            char name[128] = { 0 };
            fscanf(file, "%127s", &name);
            temp_mesh.nombre = name;
            vector_mesh.push_back(temp_mesh);

        }
        else if (strcmp(linea, "usemtl") == 0)
        {
            string name_material;
            char name[128] = { 0 };
            fscanf(file, "%127s", &name);
            name_material = name;
            if (vector_materials.empty()) continue;
            for (int i = 0; i < vector_materials.size(); i++)
            {
                if (strcmp(vector_materials[i].name.data(), name_material.data()) != 0) continue;
                vector_mesh.back().mat = vector_materials[i];
            }
            
        }


        else if (strcmp(linea, "vt") == 0) {
            glm::vec2 texture;
            fscanf_s(file, "%f %f\n", &texture.x, &texture.y);
            auxTextures.push_back(texture);
        }
        else if (strcmp(linea, "vn") == 0) {
            glm::vec3 normals;
            fscanf_s(file, "%f %f %f\n", &normals.x, &normals.y, &normals.z);
            auxNormals.push_back(normals);
        }
        else if (strcmp(linea, "f") == 0) {
            int vertexIndex[4]{ 0,0,0,0 }, textureIndex[4]{ 0,0,0,0 }, normalIndex[4]{ 0,0,0, 0 };
            int matches;
            if (auxNormals.empty())
            {
                if (auxTextures.empty()) {
                    matches = fscanf_s(file, "%d %d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2], &vertexIndex[3]);

                }
                else {
                    matches = fscanf_s(file, "%d/%d %d/%d %d/%d %d/%d\n", &vertexIndex[0], &textureIndex[0], &vertexIndex[1], &textureIndex[1], &vertexIndex[2], &textureIndex[2], &vertexIndex[3], &textureIndex[3]);

                }
            }
            else
            {
                if (auxTextures.empty()) {
                    matches = fscanf_s(file, "%d//%d %d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2], &vertexIndex[3], &normalIndex[3]);

                }
                else
                {
                    matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &textureIndex[0], &normalIndex[0], &vertexIndex[1], &textureIndex[1], &normalIndex[1], &vertexIndex[2], &textureIndex[2], &normalIndex[2], &vertexIndex[3], &textureIndex[3], &normalIndex[3]);

                }
            }
            if (vertexIndex[0] < 0)
            {
                vertexIndex[0] = auxVertices.size() + vertexIndex[0];
                vertexIndex[1] = auxVertices.size() + vertexIndex[1];
                vertexIndex[2] = auxVertices.size() + vertexIndex[2];
                if (vertexIndex[3] != 0)
                    vertexIndex[3] = auxVertices.size() + vertexIndex[3];
            }
            else
            {
                vertexIndex[0]--;
                vertexIndex[1]--;
                vertexIndex[2]--;
                if (vertexIndex[3] != 0)
                    vertexIndex[3]--;
            }

            if (normalIndex[0] < 0)
            {
                normalIndex[0] = auxNormals.size() + normalIndex[0];
                normalIndex[1] = auxNormals.size() + normalIndex[1];
                normalIndex[2] = auxNormals.size() + normalIndex[2];
                if (normalIndex[3] != 0)
                    normalIndex[3] = auxNormals.size() + normalIndex[3];
            }
            else
            {
                normalIndex[0]--;
                normalIndex[1]--;
                normalIndex[2]--;
                if (normalIndex[3] != 0)
                    normalIndex[3]--;
            }

            if (textureIndex[0] < 0)
            {
                textureIndex[0] = auxNormals.size() + textureIndex[0];
                textureIndex[1] = auxNormals.size() + textureIndex[1];
                textureIndex[2] = auxNormals.size() + textureIndex[2];
                if (textureIndex[3] != 0)
                    textureIndex[3] = auxNormals.size() + textureIndex[3];
            }
            else
            {
                textureIndex[0]--;
                textureIndex[1]--;
                textureIndex[2]--;
                if (textureIndex[3] != 0)
                    textureIndex[3]--;
            }
            if (!auxNormals.empty())
            {
                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[0]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[0]].y);
                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[0]].z);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[0]].x);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[0]].y);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[0]].z);
                vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[0]].x);
                vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[0]].y);

                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[1]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[1]].y);
                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[1]].z);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[1]].x);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[1]].y);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[1]].z);
                vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[1]].x);
                vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[1]].y);

                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[2]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[2]].y);
                vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[2]].z);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[2]].x);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[2]].y);
                vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[2]].z);
                vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[2]].x);
                vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[2]].y);
                if (matches % 4 == 0)
                {
                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[0]].x);
                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[0]].y);
                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[0]].z);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[0]].x);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[0]].y);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[0]].z);
                    vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[0]].x);
                    vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[0]].y);

                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[2]].x);
                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[2]].y);
                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[2]].z);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[2]].x);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[2]].y);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[2]].z);
                    vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[2]].x);
                    vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[2]].y);

                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[3]].x);
                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[3]].y);
                    vector_mesh.back().obj_data.push_back(auxVertices[vertexIndex[3]].z);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[3]].x);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[3]].y);
                    vector_mesh.back().obj_data.push_back(auxNormals[normalIndex[3]].z);
                    vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[3]].x);
                    vector_mesh.back().obj_data.push_back(auxTextures[textureIndex[3]].y);
                }
            }
            else
            {
                glm::vec3 temp;
                glm::vec3 calc_normal = glm::cross((auxVertices[vertexIndex[0]] - auxVertices[vertexIndex[1]]), (auxVertices[vertexIndex[0]] - auxVertices[vertexIndex[2]]));
                temp = normal_vertex[vertexIndex[0]].normal * normal_vertex[vertexIndex[0]].adjacents;
                temp += calc_normal;
                normal_vertex[vertexIndex[0]].adjacents++;
                normal_vertex[vertexIndex[0]].normal = temp / normal_vertex[vertexIndex[0]].adjacents;
                temp = normal_vertex[vertexIndex[1]].normal * normal_vertex[vertexIndex[1]].adjacents;
                temp += calc_normal;
                normal_vertex[vertexIndex[1]].adjacents++;
                normal_vertex[vertexIndex[1]].normal = temp / normal_vertex[vertexIndex[1]].adjacents;
                temp = normal_vertex[vertexIndex[2]].normal * normal_vertex[vertexIndex[2]].adjacents;
                temp += calc_normal;
                normal_vertex[vertexIndex[2]].adjacents++;
                normal_vertex[vertexIndex[2]].normal = temp / normal_vertex[vertexIndex[2]].adjacents;
                if (matches % 4 == 0)
                {
                    calc_normal = glm::cross((auxVertices[vertexIndex[3]] - auxVertices[vertexIndex[1]]), (auxVertices[vertexIndex[3]] - auxVertices[vertexIndex[2]]));
                    temp = normal_vertex[vertexIndex[3]].normal * normal_vertex[vertexIndex[3]].adjacents;
                    temp += calc_normal;
                    normal_vertex[vertexIndex[3]].adjacents++;
                    normal_vertex[vertexIndex[3]].normal = temp / normal_vertex[vertexIndex[3]].adjacents;
                    vector_mesh.back().vector_faces.push_back({ vertexIndex[0],vertexIndex[1],vertexIndex[2], vertexIndex[3]
                        ,textureIndex[0],textureIndex[1],textureIndex[2],textureIndex[3]});
                }
                else
                {
                    vector_mesh.back().vector_faces.push_back({ vertexIndex[0],vertexIndex[1],vertexIndex[2], -1
                        ,textureIndex[0],textureIndex[1],textureIndex[2],-1 });
                }

            }
        }
    }
    if (auxNormals.empty())
    {
        for (int i = 0; i < vector_mesh.back().vector_faces.size(); i++)
        {
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[0]].x);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[0]].y);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[0]].z);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[0]].normal.x);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[0]].normal.y);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[0]].normal.z);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[0]].x);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[0]].y);

            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[1]].x);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[1]].y);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[1]].z);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[1]].normal.x);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[1]].normal.y);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[1]].normal.z);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[1]].x);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[1]].y);

            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[2]].x);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[2]].y);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[2]].z);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[2]].normal.x);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[2]].normal.y);
            vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[2]].normal.z);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[2]].x);
            vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[2]].y);
            if (vector_mesh.back().vector_faces[i].vertex[3] != -1)
            {
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[0]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[0]].y);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[0]].z);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[0]].normal.x);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[0]].normal.y);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[0]].normal.z);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[0]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[0]].y);

                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[2]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[2]].y);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[2]].z);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[2]].normal.x);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[2]].normal.y);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[2]].normal.z);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[2]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[2]].y);

                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[3]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[3]].y);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].vertex[3]].z);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[3]].normal.x);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[3]].normal.y);
                vector_mesh.back().obj_data.push_back(normal_vertex[vector_mesh.back().vector_faces[i].vertex[3]].normal.z);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[3]].x);
                vector_mesh.back().obj_data.push_back(auxVertices[vector_mesh.back().vector_faces[i].texture[3]].y);
            }
        }
    }
    
}

vector<string> obj::split(string const& original, char separator) {
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

void obj::rotateObj(float x, float y)
{
    x_angle += x;
    y_angle += y; 
 
    glm::vec3 EulerAngles(x_angle, y_angle, 0);
    MyQuaternion = glm::quat(EulerAngles);
    rotation = glm::toMat4(MyQuaternion);
}

void obj::traslateObj(float x, float y, float z) {
    tras_vector.x += x;
    tras_vector.y += y;
    tras_vector.z += z;
}