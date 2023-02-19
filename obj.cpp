#include "obj.h"
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>
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
        mat tempMaterial;
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
    if (temp_normals.empty())
    {
        for (int y = 0; y < vector_mesh.size(); y++)
        {
            for (int i = 0; i < vector_mesh[y].vector_faces.size(); i++)
            {
                setVertexNormal(vector_mesh[y].vector_faces[i], 0, y);
                setVertexNormal(vector_mesh[y].vector_faces[i], 1, y);
                setVertexNormal(vector_mesh[y].vector_faces[i], 2, y);
                if (vector_mesh[y].vector_faces[i].vertex.size() > 3)
                {
                    setVertexNormal(vector_mesh[y].vector_faces[i], 0, y);
                    setVertexNormal(vector_mesh[y].vector_faces[i], 2, y);
                    setVertexNormal(vector_mesh[y].vector_faces[i], 3, y);
                }
            }
        }

    }
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
        image.append(vector_mesh[i].material.map_ka);
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
    /*uniform vec4 Ka, Kd, Ks;
    uniform float alfa, shininess;*/
    GLint ambientColorLoc = glGetUniformLocation(basic_shader.Program, "Ka");
    GLint difuseColorLoc = glGetUniformLocation(basic_shader.Program, "Kd");
    GLint specularColorLoc = glGetUniformLocation(basic_shader.Program, "Ks");
    GLint shininessLoc = glGetUniformLocation(basic_shader.Program, "shininess");
    GLint modelLoc = glGetUniformLocation(basic_shader.Program, "model");
    GLint boundingLoc = glGetUniformLocation(basic_shader.Program, "bounding");
    GLint texturizedLoc = glGetUniformLocation(basic_shader.Program, "texturized");
    glm::mat4 ident = glm::mat4(1.0f);
    glm::mat4 center = glm::translate(ident, center_vector);
    glm::mat4 scaling = glm::scale(ident, scale_vector * scale);
    glm::mat4 traslation = glm::translate(ident, tras_vector);
    model = traslation * rotation * scaling * center;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(boundingLoc, false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < vector_mesh.size(); i++)
    {

        glUniform3f(ambientColorLoc, vector_mesh[i].material.ka[0], vector_mesh[i].material.ka[1], vector_mesh[i].material.ka[2]);
        glUniform3f(difuseColorLoc, vector_mesh[i].material.kd[0], vector_mesh[i].material.kd[1], vector_mesh[i].material.kd[2]);
        glUniform3f(specularColorLoc, vector_mesh[i].material.ks[0], vector_mesh[i].material.ks[1], vector_mesh[i].material.ks[2]);
        glUniform1f(shininessLoc, vector_mesh[i].material.ns);

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

    int vertice, texture;
    while (getline(MyReadFile, myText)) {
        if (myText.size() < 2)
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
            vector_mesh.back().material = tempMaterial;
        }
        else if (strcmp(temp[0].data(), "o") == 0 || strcmp(temp[0].data(), "g") == 0)
        {
            if (vector_mesh.empty())
            {
                tempMesh.nombre = temp[1];
                vector_mesh.push_back(tempMesh);
            }
            else
            {
                if (strcmp(vector_mesh.back().nombre.data(), temp[1].data()) != 0)
                {
                    tempMesh.nombre = temp[1];
                    vector_mesh.push_back(tempMesh);
                }
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
                    if (!temp_textures.empty())
                    {
                        aux = split(temp[i], '/');
                        texture = stoi(aux[1]);
                        texture < 0 ? texture = temp_vertices.size() + texture : texture--;
                        tempFace.texture.push_back(texture);
                    }
                }
                tempFace.normal = n;
                if (!vector_materials.empty()) {
                    tempFace.material = tempMaterial;
                    vector_mesh.back().material = tempMaterial;
                }
                vector_mesh.back().vector_faces.push_back(tempFace);
            }
        }
    }
}

void obj::loadVertex(string const& temp)
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
        vector_mesh.back().obj_data.push_back(-0.1f);
        vector_mesh.back().obj_data.push_back(-0.1f);
    }
    else {
        textures = stoi(aux[1]);
        textures < 0 ? textures = temp_normals.size() + textures : textures--;
        vector_mesh.back().obj_data.push_back(temp_textures[textures].x);
        vector_mesh.back().obj_data.push_back(temp_textures[textures].y);
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

glm::vec3 obj::getVertex(string temp)
{
    vector<string> aux;
    int vertice;
    aux = split(temp, '/');
    vertice = stoi(aux[0]);
    vertice < 0 ? vertice = temp_vertices.size() + vertice : vertice--;
    return { temp_vertices[vertice] };
}

void obj::setVertexNormal(face temp, int i, int v) {
    vector_mesh[v].obj_data.push_back(temp_vertices[temp.vertex[i]].x);
    vector_mesh[v].obj_data.push_back(temp_vertices[temp.vertex[i]].y);
    vector_mesh[v].obj_data.push_back(temp_vertices[temp.vertex[i]].z);

    vector_mesh[v].obj_data.push_back(calcNormal[temp.vertex[i]].normal.x);
    vector_mesh[v].obj_data.push_back(calcNormal[temp.vertex[i]].normal.y);
    vector_mesh[v].obj_data.push_back(calcNormal[temp.vertex[i]].normal.z);

    if (vector_materials.empty())
    {
        vector_mesh[v].obj_data.push_back(-0.1f);
        vector_mesh[v].obj_data.push_back(-0.1f);
    }
    else
    {
        if (!temp_textures.empty())
        {
            vector_mesh[v].obj_data.push_back(temp_textures[temp.texture[i]].x);
            vector_mesh[v].obj_data.push_back(temp_textures[temp.texture[i]].y);
        }

    }

}

void obj::rotateObj(float x, float y)
{
    x_angle += x;
    y_angle += y; 

    
    glm::vec3 RIGHT= glm::cross(FRONT, UP);
    rotation = glm::rotate(y_angle,UP)
        *glm::rotate(x_angle, RIGHT);
    FRONT = glm::mat3(rotation) * FRONT;
}

void obj::traslateObj(float x, float y, float z) {
    tras_vector.x += x;
    tras_vector.y += y;
    tras_vector.z += z;
}