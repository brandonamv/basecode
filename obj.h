#pragma once

class obj
{
	public:
		obj(string fileName);
		~obj();
        void loadObj(string fileName) {

            string myText;
            string archivo;
            archivo = openFile();
            ifstream MyReadFile(archivo);
            vector<string> temp;
            vector<string> aux;
            text.clear();
            while (getline(MyReadFile, myText)) {
                // Output the text from the file
                temp = split(myText, " ");
                if (temp[0].compare("v") == 0) {
                    glm::vec3 vertex;
                    vertex.x = stof(temp[1]);
                    vertex.y = stof(temp[2]);
                    vertex.z = stof(temp[3]);
                    temp_vertices.push_back(vertex);
                }
                else if (temp[0].compare("vn") == 0) {
                    glm::vec3 normal;
                    normal.x = stof(temp[1]);
                    normal.y = stof(temp[2]);
                    normal.z = stof(temp[3]);
                    temp_normals.push_back(normal);
                }
                else if (temp[0].compare("f") == 0) {
                    nFaces++;
                    aux = split(temp[1], "/");
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].x);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].y);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].z);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].x);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].y);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].z);
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].x));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].y));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].z));
                    aux = split(temp[2], "/");
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].x);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].y);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].z);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].x);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].y);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].z);
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].x));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].y));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].z));
                    aux = split(temp[3], "/");
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].x);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].y);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].z);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].x);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].y);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].z);
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].x));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].y));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].z));
                    aux = split(temp[1], "/");
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].x);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].y);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].z);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].x);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].y);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].z);
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].x));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].y));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].z));
                    aux = split(temp[3], "/");
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].x);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].y);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].z);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].x);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].y);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].z);
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].x));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].y));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].z));
                    aux = split(temp[4], "/");
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].x);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].y);
                    objVertex.push_back(temp_vertices[stoi(aux[0]) - 1].z);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].x);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].y);
                    objVertex.push_back(temp_normals[stoi(aux[2]) - 1].z);
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].x));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].y));
                    objVertex.push_back(abs(temp_normals[stoi(aux[2]) - 1].z));
                }
            }
        }
	private:
        
};

obj::obj(string fileName)
{
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    GLfloat* thearray = objVertex.data();
    subObj cube(thearray, sizeof(thearray) * objVertex.size());
}

obj::~obj()
{
}