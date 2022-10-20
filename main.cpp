
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
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto 2 CG1 - UCV", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouseClick);
    glDisable(GL_CULL_FACE);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader basic_shader("color_shader.vs", "color_shader.frag");


    // Initialize ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // Variables to be changed in the ImGUI window
    static bool my_tool_active;
    static bool filledTriangle;
    static bool vertexPoint;
    static bool lineTriangle;
    static bool fpsShow;
    static bool zBuffer;
    static bool backFaceCulling;
    static bool antialliasing;
    float size;
    float pointSize;
    float *objColor,*lineColor,*pointColor;

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
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// Tell OpenGL a new frame is about to begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //// ImGUI window creation
        ImGui::Begin("Menu", &my_tool_active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) {
                    string objeto = openFile();
                    if (!objeto.empty())
                    {
                        objects.push_back(new obj(objeto));
                        actual = objects.back();
                    }
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                if (ImGui::MenuItem("Load", "Ctrl+W")) {  }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        //// Text that appears in the window
        ImGui::ColorEdit4("BackGround Color", backgroundColor);
        if (!objects.empty())
        {
            if (ImGui::Button("Clear Scene")) {
                objects.clear();
                actual = nullptr;
            }
            ImGui::Checkbox("Back Face Culling", &backFaceCulling);
            if (backFaceCulling)
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                glFrontFace(GL_CW);
            }
            else {
                glDisable(GL_CULL_FACE);
            }
            ImGui::Checkbox("Antialliasing", &antialliasing);
            if (antialliasing)
            {
                glEnable(GL_MULTISAMPLE);
            }
            else {
                glDisable(GL_MULTISAMPLE);
            }
        }
        
        ImGui::Checkbox("FPS", &fpsShow);
        if (fpsShow)
        {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            float samples[100];
            for (int n = 0; n < 100; n++)
                samples[n] = sinf(0.2f + ImGui::GetIO().Framerate * 1.5f);
            ImGui::PlotLines("Samples", samples, 100);
        }
        
        if (actual)
        {
            //// Slider that appears in the window
            size = actual->getTam();
            ImGui::SliderFloat("Size", &size, 0.1f, 10.0f);
            actual->setTam(size);
            //// Checkbox that appears in the window
            filledTriangle = actual->getFilled();
            ImGui::Checkbox("Filled Triangles", &filledTriangle);
            actual->setFilled(filledTriangle);
            if (filledTriangle)
            {
                //// Fancy color editor that appears in the window
                objColor = actual->getFillColor();
                ImGui::ColorEdit3("Triangle Fill Color", objColor);
                actual->setFillColor(objColor);
            }
            lineTriangle = actual->getLined();
            ImGui::Checkbox("Triangle Lines", &lineTriangle);
            actual->setLined(lineTriangle);
            if (lineTriangle)
            {
                //// Fancy color editor that appears in the window
                lineColor = actual->getLineColor();
                ImGui::ColorEdit3("Triangle Line Color", lineColor);
                actual->setLineColor(lineColor);
            }
            vertexPoint = actual->getPointed();
            ImGui::Checkbox("Vertex Point", &vertexPoint);
            actual->setPointed(vertexPoint);
            if (vertexPoint)
            {
                //// Fancy color editor that appears in the window
                pointColor = actual->getPointColor();
                ImGui::ColorEdit3("Vertex Point Color", pointColor);
                actual->setPointColor(pointColor);

                pointSize = actual->getPointSize();
                ImGui::SliderFloat("Point Size", &pointSize, 0.1f, 10.0f);
                actual->setPointSize(pointSize);
            }
        }
        
        
        //// Ends the window
        ImGui::End();

        // Create camera transformations
        glm::mat4 view = glm::lookAt(eyePos, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(60.0f * 3.14159f / 180.0f, float(w)/float(h), 0.01f, 100.0f);
        

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
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Deletes all ImGUI instances
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // Delete all the objects we've created
    glDeleteProgram(basic_shader.Program);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void mouseClick(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    GLfloat xoffset = x - lastX;
    // Reversed since y-coordinates go from bottom to left
    GLfloat yoffset = lastY - y;
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i]->onClick(xoffset,yoffset);
    }
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
        string objeto = openFile();
        if (!objeto.empty())
        {
            objects.push_back(new obj(objeto));
            actual = objects.back();
        }
        
    }
}

void do_movement(GLfloat delta)
{
    // Camera controls
    if (keys[GLFW_KEY_UP])
    {
        trasScene.z += delta;
    }
    if (keys[GLFW_KEY_DOWN])
    {
        trasScene.z -= delta;
    }
    /*if (keys[GLFW_KEY_RIGHT])
    {
        trasScene.x -= delta;
    }
    if (keys[GLFW_KEY_LEFT])
    {
        trasScene.x += delta;
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

