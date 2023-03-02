#include "subobj.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;
subObj::subObj(GLfloat* data, int size, string image) {
    this->size = size / sizeof(data);
    // First, set the container's VAO (and VBO)
    glGenVertexArrays(1, &containerVAO);
    glBindVertexArray(containerVAO);

    glGenBuffers(1, &VBO);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    if (image.compare(""))
    {
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        unsigned char* datas = stbi_load(image.data(), &width, &height, &nrChannels, 0);
        if (datas)
        {
            texturized = true;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, datas);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            texturized = false;
            cout << "Failed to load texture " << image << endl;
        }
        stbi_image_free(datas);
    }
    else
    {
        texturized = false;
    }
}

subObj::~subObj() {
    // Delete all the objects we've created
    glDeleteVertexArrays(1, &containerVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
}
bool subObj::getTexture() {
    return texturized;
}
void subObj::draw() {

    glBindTexture(GL_TEXTURE_2D, texture);
    bind();
    glDrawArrays(GL_TRIANGLES, 0, size / sizeof(GLfloat*));
}