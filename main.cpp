
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

    srand(time(NULL));
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto B CG2 - UCV", glfwGetPrimaryMonitor(),  nullptr);
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
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glPolygonOffset(1.0, 1.0);

    // Build and compile our shader program
    Shader basic_shader("color_shader.vs", "color_shader.frag");
    Shader particle_shader("particle_shader.vs", "particle_shader.frag");
    std::vector<ParticleGenerator*> particle_system;
    // Initialize ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // Variables to be changed in the ImGUI window
    static bool menu_general;
    static bool opc_fps;
    static bool opc_antialliasing;
    static bool opc_gravity;
    static bool opc_blending;
    static bool opc_deph_sort;
    float opc_background_color[3] = { 0.0f, 1.0f, 1.0f };
    float opc_camera_speed = 5.0f;
    float opc_anim_speed = 1.0f;

    //menu creacion particulas
    static bool menu_particle;

    float sx,sy,sz;
    float* objColor = nullptr, * boundingColor;


    basic_shader.Use();
    // Get the uniform locations
    GLint viewLoc = glGetUniformLocation(basic_shader.Program, "view");
    GLint projLoc = glGetUniformLocation(basic_shader.Program, "projection");
    cam = new camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    
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
        glClearColor(opc_background_color[0], opc_background_color[1], opc_background_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// Tell OpenGL a new frame is about to begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //// ImGUI window creation
        ImGui::Begin("General Menu", &menu_general, ImGuiWindowFlags_MenuBar);
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
                if (ImGui::MenuItem("Save", "Ctrl+S")) { 
                
                }
                if (ImGui::MenuItem("Load", "Ctrl+W")) {  }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        //// Text that appears in the window
        ImGui::Text("Bacground Color");
        ImGui::ColorEdit3("col_bg", opc_background_color);

        ImGui::Checkbox("Antialliasing", &opc_antialliasing);
        if (opc_antialliasing)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else {
            glDisable(GL_MULTISAMPLE);
        }
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
        }
        
        ImGui::Checkbox("FPS", &opc_fps);
        if (opc_fps)
        {
            
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            float samples[3000]{};
            for (int n = 0; n < 3000; n++)
                samples[n] = ImGui::GetIO().Framerate;
            ImGui::PlotHistogram("Samples", samples, 3000, 0, NULL, 0.0f, 100.0f, ImVec2(300, 50));
        }
        ImGui::Separator();
        ImGui::Text("Camera Speed");
        ImGui::SliderFloat("s_cam", &opc_camera_speed, 0.1f, 30.0f);
        cam->setcameraSpeed(opc_camera_speed,deltaTime);
        ImGui::Separator();
        ImGui::Checkbox("Gravity", &opc_gravity);
        ImGui::Checkbox("Blending", &opc_blending);
        if (opc_blending)
        {
            glEnable(GL_BLEND);
            ImGui::Checkbox("Deph Sort", &opc_deph_sort);
            if (opc_deph_sort)
            {
                glEnable(GL_DEPTH_TEST);

            }
            else {
                glDisable(GL_DEPTH_TEST);
            }
        }
        else {
            glDisable(GL_BLEND);
        }

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            particle_system.push_back(new ParticleGenerator());

        if (!particle_system.empty())
        {
            ImGui::Separator();
            ImGui::Text("Animation Speed");
            ImGui::SliderFloat("s_ptcle", &opc_anim_speed, 0.1f, 10.0f);

        }


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
        glm::mat4 view = cam->getView();
        glm::mat4 projection = glm::perspective(60.0f * 3.14159f / 180.0f, float(w) / float(h), 0.01f, 100.0f);
       
        
        
        basic_shader.Use();
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
        
        particle_shader.Use();
        if (!particle_system.empty())
        {
            for (const auto& x : particle_system)
            {
                x->Update(deltaTime, 100, glm::vec3(1.0f, 1.0f, 1.0f));
                x->Draw(particle_shader,view, projection);
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
            actual->traslateObj(cam->getcameraFront().x, cam->getcameraFront().y, cam->getcameraFront().z);
        }
        
    }
    if (key == GLFW_KEY_G && action == GLFW_RELEASE)
        cam->setGodMode(false);
    if (key == GLFW_KEY_P && action == GLFW_RELEASE)
    {
        if (!pause)
        {
            pause = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            pause = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

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
    GLfloat dx = xpos - xini;
    GLfloat dy = ypos - yini;
    if (pause)
    {
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
    }
    else {

        if (dx != 0)
        {
            dx > 0 ? cam->move_right() : cam->move_left();
        }
        if (dy!=0)
        {
            dy < 0 ? cam->move_up() : cam->move_down();
        }
    }
    xini = xpos;
    yini = ypos;
}

