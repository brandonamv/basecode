
#include <main.h>

// GLEW
// #ifdef GLEW_STATIC
// #  define GLEWAPI extern
// #else
// #  ifdef GLEW_BUILD
// #    define GLEWAPI extern __declspec(dllexport)
// #  else
// #    define GLEWAPI extern __declspec(dllimport)
// #  endif
// #endif

vector<GLfloat> objVertex;

//funcion para guardar el contenido del archivo en un vector de text
vector<string> split(string s, string del) {
    int i = 0;
    int start = 0;
    int end = s.find(del);
    vector<string> temp;
    while (end != -1) {
        temp.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    temp.push_back(s.substr(start, end - start));
    return temp;
}

void loadObj(){

    string myText;
    ifstream MyReadFile("C:/Users/Marielsy/source/repos/basecode/obj/Sting-Sword-lowpoly.obj");
    vector<string> temp;
    vector<string> aux;
    text.clear();
    while (getline(MyReadFile, myText)) {
        // Output the text from the file
        temp=split(myText, " ");
        if (temp[0].compare("v")==0) {
            glm::vec3 vertex;
            vertex.x = stof(temp[1]);
            vertex.y = stof(temp[2]);
            vertex.z = stof(temp[3]);
            temp_vertices.push_back(vertex);
        }else if (temp[0].compare("vn") == 0){
            glm::vec3 normal;
            normal.x = stof(temp[1]);
            normal.y = stof(temp[2]);
            normal.z = stof(temp[3]);
            temp_normals.push_back(normal);
        }
        else if (temp[0].compare("vt") == 0) {
            glm::vec2 uv;
            uv.x = stof(temp[1]);
            uv.y = stof(temp[2]);
            temp_uvs.push_back(uv);
        }else if (temp[0].compare("f") == 0) {
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

// The MAIN function, from here we start the application and run the game loop
int main(){
    loadObj();
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW, (GLSL version required = 3.0)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto 2 CG1 - UCV", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glDisable(GL_CULL_FACE);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader basic_shader("color_shader.vs", "color_shader.frag");


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat cube_vertices[] =
    {
        // back
        0.5,0.5,-0.5, 0.0f,  0.0f, -1.0f, 1,0,0, //v1
        0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0, //v2
       -0.5,0.5,-0.5,  0.0f,  0.0f, -1.0f,1,0,0, //v3
       -0.5,0.5,-0.5,0.0f,  0.0f, -1.0f,1,0,0,   //v3
        0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0, //v2
       -0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0, //v4

       // front 
      -0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,
      -0.5,-0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,
       0.5,-0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,
      -0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,
       0.5,-0.5,0.5, 0.0f,  0.0f,  1.0f,0,1,0,
       0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,

       // left
       -0.5,0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,-0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,-0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,-0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1,



       // right
        0.5,0.5,0.5, 1.0f,  0.0f,  0.0f, 1,0,1,
        0.5,-0.5,0.5, 1.0f,  0.0f,  0.0f,1,0,1,
        0.5,-0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1,
        0.5,0.5,0.5, 1.0f,  0.0f,  0.0f,1,0,1,
        0.5,-0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1,
        0.5,0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1,


        // bottom
       -0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,
       -0.5,-0.5,-0.5, 0.0f, -1.0f,  0.0f,1,1,0,
        0.5,-0.5,-0.5,0.0f, -1.0f,  0.0f,1,1,0,
       -0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,
        0.5,-0.5,-0.5, 0.0f, -1.0f,  0.0f,1,1,0,
        0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,


        // top
        -0.5,0.5,-0.5, 0.0f,  1.0f,  0.0f, 1,1,1,
        -0.5,0.5,0.5, 0.0f,  1.0f,  0.0f, 1,1,1,
         0.5,0.5,0.5, 0.0f,  1.0f,  0.0f, 1,1,1,
        -0.5,0.5,-0.5,  0.0f,  1.0f,  0.0f, 1,1,1,
         0.5,0.5,0.5, 0.0f,  1.0f,  0.0f,1,1,1,
         0.5,0.5,-0.5, 0.0f,  1.0f,  0.0f,1,1,1,
    };
    GLfloat* thearray=objVertex.data();
    My3DObject cube(thearray, objVertex.size());
    //glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);


    // Game loop
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    basic_shader.Use();
    // Get the uniform locations
    GLint objectColorLoc = glGetUniformLocation(basic_shader.Program, "objectColor");
    GLint modelLoc = glGetUniformLocation(basic_shader.Program, "model");
    GLint viewLoc = glGetUniformLocation(basic_shader.Program, "view");
    GLint projLoc = glGetUniformLocation(basic_shader.Program, "projection");

    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        // Define the viewport dimensions
        glViewport(0, 0, w, h);

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement(deltaTime);

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        static float x_angle = 0.0f;
        x_angle += deltaTime;

        // Create camera transformations
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(60.0f * 3.14159f / 180.0f, float(w)/float(h), 0.1f, 100.0f);
        glm::mat4 model =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zDist)) *
            glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), x_angle, glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        // Use cooresponding shader when setting uniforms/drawing objects
        glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        cube.draw();

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement(GLfloat delta)
{
    // Camera controls
    if (keys[GLFW_KEY_W])
    {
        zDist += delta;
    }
    if (keys[GLFW_KEY_S])
    {
        zDist -= delta;
    }
    if (keys[GLFW_KEY_A])
    {
        angle -= delta;
    }
    if (keys[GLFW_KEY_D])
    {
        angle += delta;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    // Reversed since y-coordinates go from bottom to left
    GLfloat yoffset = lastY - ypos;  
    //glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

    lastX = WIDTH / 2;// xpos;
    lastY = HEIGHT / 2; // ypos;

}

