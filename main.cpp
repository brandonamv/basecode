
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
//clase obj con opcion cargar, antialiasing blending, multisampling


string openFile() {
    char const* lTheOpenFileName;
    char const* lFilterPatterns[2] = { "*.obj", "*.mtl" };

    lTheOpenFileName = tinyfd_openFileDialog(
        "Seleccione objeto a cargar",
        "",
        2,
        lFilterPatterns,
        NULL,
        0);

    if (!lTheOpenFileName)
    {
        tinyfd_messageBox(
            "Error",
            "Open file name is NULL",
            "ok",
            "error",
            1);
        return "";
    }
    return lTheOpenFileName;
}



// The MAIN function, from here we start the application and run the game loop
int main(){

    
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
    //GLfloat cube_vertices[] =
    //{
    //    // back
    //    -0.5,0.5,-0.5,  0.0f,  0.0f, -1.0f,1,0,0, //v3
    //   0.5,0.5,-0.5, 0.0f,  0.0f, -1.0f, 1,0,0, //v1
    //    0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0, //v2
    //   -0.5,0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0,   //v3
    //    0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0, //v2
    //   -0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0, //v4
    //   // front 
    //  -0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,  //v5
    //  -0.5,-0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0, //v6
    //   0.5,-0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0, //v7
    //  -0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,  //v5
    //   0.5,-0.5,0.5, 0.0f,  0.0f,  1.0f,0,1,0,  //v7
    //   0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,  //v8
    //   // left
    //   -0.5,0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1, //v3
    //   -0.5,-0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1,//v4
    //   -0.5,-0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1, //v6
    //   -0.5,0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1, //v3
    //   -0.5,-0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1, //v6
    //   -0.5,0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1,  //v5
    //   // right
    //    0.5,0.5,0.5, 1.0f,  0.0f,  0.0f, 1,0,1,  //v8
    //    0.5,-0.5,0.5, 1.0f,  0.0f,  0.0f,1,0,1,  //v7
    //    0.5,-0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1, //v2
    //    0.5,0.5,0.5, 1.0f,  0.0f,  0.0f,1,0,1,   //v8
    //    0.5,-0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1, //v2
    //    0.5,0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1,  //v1
    //    // bottom
    //   -0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,
    //   -0.5,-0.5,-0.5, 0.0f, -1.0f,  0.0f,1,1,0,
    //    0.5,-0.5,-0.5,0.0f, -1.0f,  0.0f,1,1,0,
    //   -0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,
    //    0.5,-0.5,-0.5, 0.0f, -1.0f,  0.0f,1,1,0,
    //    0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,
    //    // top
    //    -0.5,0.5,-0.5, 0.0f,  1.0f,  0.0f, 1,1,1, //v3
    //    -0.5,0.5,0.5, 0.0f,  1.0f,  0.0f, 1,1,1,  //v5
    //     0.5,0.5,0.5, 0.0f,  1.0f,  0.0f, 1,1,1,  //v8
    //    -0.5,0.5,-0.5,  0.0f,  1.0f,  0.0f, 1,1,1,//v3
    //     0.5,0.5,0.5, 0.0f,  1.0f,  0.0f,1,1,1,   //v8
    //     0.5,0.5,-0.5, 0.0f,  1.0f,  0.0f,1,1,1,  //v1
    //};

    
    // Initialize ImGUI
    /*IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");*/
    
    // Variables to be changed in the ImGUI window
    bool drawTriangle = true;
    float size = 1.0f;
    float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };

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
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// Tell OpenGL a new frame is about to begin
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        //ImGui::NewFrame();

        //// ImGUI window creation
        //ImGui::Begin("My name is window, ImGUI window");
        //// Text that appears in the window
        //ImGui::Text("Hello there adventurer!");
        //// Checkbox that appears in the window
        //ImGui::Checkbox("Draw Triangle", &drawTriangle);
        //// Slider that appears in the window
        //ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
        //// Fancy color editor that appears in the window
        //ImGui::ColorEdit4("Color", color);
        //// Ends the window
        //ImGui::End();

        // Create camera transformations
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(60.0f * 3.14159f / 180.0f, float(w)/float(h), 0.1f, 100.0f);
        

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        

        if (!objects.empty())
        {
            for (const auto &x:objects)
            {
                x->draw(objectColorLoc,modelLoc);
            }
        }

        // Renders the ImGUI elements
        /*ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Deletes all ImGUI instances
    /*ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();*/
    // Delete all the objects we've created
    glDeleteProgram(basic_shader.Program);
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
    if (key==GLFW_KEY_O && action==GLFW_PRESS)
    {
        objects.push_back(new obj(openFile()));
    }
}

void do_movement(GLfloat delta)
{
    // Camera controls
    /*if (keys[GLFW_KEY_W])
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
    }*/
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

