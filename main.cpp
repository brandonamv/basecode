
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
using namespace std;

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glDisable(GL_CULL_FACE);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);

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
    float sx,sy,sz;
    float* objColor = nullptr, * boundingColor;

    // Game loop
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    basic_shader.Use();
    // Get the uniform locations
    GLint viewLoc = glGetUniformLocation(basic_shader.Program, "view");
    GLint projLoc = glGetUniformLocation(basic_shader.Program, "projection");
    GLint l_positionLoc = glGetUniformLocation(basic_shader.Program, "ligth_position");
    GLint l_kaLoc = glGetUniformLocation(basic_shader.Program, "ambient_color");
    GLint l_kdLoc = glGetUniformLocation(basic_shader.Program, "difuse_color");
    GLint l_ksLoc = glGetUniformLocation(basic_shader.Program, "specular_color");
    cam = new camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    float cameraSpeed=1.0f;
    //string bombillo = openFile();

    float ambient_color[3]{ 1.0f,1.0f,1.0f };
    float difuse_color[3]{ 1.0f,1.0f,1.0f };
    float specular_color[3]{ 1.0f,1.0f,1.0f };
    float intensity=5.0f;
    /*if (!bombillo.empty())
    {
        objects.push_back(new obj(bombillo));
        actual = objects.back();
        actual->traslateObj(cam->getcameraFront().x, cam->getcameraFront().y, cam->getcameraFront().z);
        actual->setTam(0.5f, 0.5f, 0.5f);
    }*/
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
                        actual->traslateObj(cam->getcameraFront().x, cam->getcameraFront().y, cam->getcameraFront().z);
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
                for (int i = 0; i < objects.size(); i++)
                {
                    objects[i]->~obj();
                }
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
            ImGui::Checkbox("Z-Buffer", &zBuffer);
            if (zBuffer)
            {
                glEnable(GL_DEPTH_TEST);
            }
            else {
                glDisable(GL_DEPTH_TEST);
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
            float samples[3000]{};
            for (int n = 0; n < 3000; n++)
                samples[n] = ImGui::GetIO().Framerate;
            ImGui::PlotHistogram("Samples", samples, 3000, 0, NULL, 0.0f, 100.0f, ImVec2(300, 50));
        }
        ImGui::Separator();
        ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.1f, 30.0f);
        cam->setcameraSpeed(cameraSpeed,deltaTime);
        if (actual)
        {
            ImGui::Separator();
            //// Slider that appears in the window
            sx = actual->getTam().x;
            sy = actual->getTam().y;
            sz = actual->getTam().z;
            ImGui::SliderFloat("Size x", &sx, 0.1f, 30.0f);
            ImGui::SliderFloat("Size y", &sy, 0.1f, 30.0f);
            ImGui::SliderFloat("Size z", &sz, 0.1f, 30.0f);
            actual->setTam(sx, sy, sz);

        }
        
        
        //// Ends the window
        ImGui::End();

        //menu luces
        ImGui::Begin("Lights Menu", &my_tool_active, ImGuiWindowFlags_MenuBar);
        ImGui::ColorEdit3("Ambient Color", ambient_color);
        ImGui::SliderFloat("Light Intensity", &intensity, 0.0f, 10.0f);
        ImGui::ColorEdit3("Difuse Color", difuse_color);
        ImGui::ColorEdit3("Specular Color", specular_color);
        glUniform3f(l_kaLoc, ambient_color[0], ambient_color[1], ambient_color[2]);
        glUniform3f(l_kdLoc, difuse_color[0], difuse_color[1], difuse_color[2]);
        glUniform3f(l_ksLoc, specular_color[0], specular_color[1], specular_color[2]);
        glUniform3f(l_positionLoc, .0f,.0f,.0f);
        
        ImGui::End();
        // Create camera transformations
        glm::mat4 view = cam->getView();
        glm::mat4 projection = glm::perspective(60.0f * 3.14159f / 180.0f, float(w)/float(h), 0.01f, 100.0f);
        

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        
        if (!objects.empty())
        {
            for (const auto &x:objects)
            {
                x->draw(basic_shader);
                x->setSelect(x == actual);
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
    if (action)
    {
        glm::mat4 view = cam->getView();
        glm::mat4 projection = glm::perspective(60.0f * 3.14159f / 180.0f, float(WIDTH) / float(HEIGHT), 0.01f, 100.0f);
        firstMouse = true;
        xini = xoffset;
        yini = yoffset;
        rotation = button == GLFW_MOUSE_BUTTON_1;
        moving = button == GLFW_MOUSE_BUTTON_2;
        glm::vec4 mouse = projection*view*glm::vec4(x/WIDTH, y/HEIGHT,0,0);
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->onClick(mouse.x, mouse.y);
        }
    }
    else
    {
        firstMouse = false;
        rotation = false;
        moving = false;
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
            actual->setSelect(true);
        }
        
    }
    if (key == GLFW_KEY_G && action == GLFW_RELEASE)
    {
        cam->setGodMode(false);
    }
}

void do_movement(GLfloat delta)
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        cam->move_front();
    if (keys[GLFW_KEY_S])
        cam->move_back();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    lastX = static_cast<GLfloat>(WIDTH) / 2;
    lastY = static_cast<GLfloat>(HEIGHT) / 2;
    double x = WIDTH - xpos;
    double y = HEIGHT - ypos;
    GLfloat xoffset = x - lastX;
    // Reversed since y-coordinates go from bottom to left
    GLfloat yoffset = lastY - y;
    //glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);
    GLfloat dx = xoffset - xini;
    GLfloat dy = yoffset - yini;
    if (firstMouse)
    {
        if (actual)
        {
            if (rotation)
                actual->rotateObj(dy/10, dx/10);
            if (moving)
                actual->traslateObj(dx/100, dy/100, 0);
                
        }
    }
    else {
        if (dx != 0)
        {
            dx > 0 ? cam->move_right() : cam->move_left();
        }
        if (dy!=0)
        {
            dy > 0 ? cam->move_up() : cam->move_down();
        }
    }
    xini = xoffset;
    yini = yoffset;
}

