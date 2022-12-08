
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
    float *objColor,*boundingColor;

    // Game loop
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    basic_shader.Use();
    // Get the uniform locations
    GLint viewLoc = glGetUniformLocation(basic_shader.Program, "view");
    GLint projLoc = glGetUniformLocation(basic_shader.Program, "projection");
    //variables para activar el tipo de luz
    GLint ambientLoc = glGetUniformLocation(basic_shader.Program, "ambient");
    GLint lambertLoc = glGetUniformLocation(basic_shader.Program, "lambert");
    GLint phongLoc = glGetUniformLocation(basic_shader.Program, "phong");
    //variable para el color de la luz
    GLint ambientColorLoc= glGetUniformLocation(basic_shader.Program, "ambient_color");
    GLint lambertColorLoc = glGetUniformLocation(basic_shader.Program, "lambert_color");
    GLint phongColorLoc = glGetUniformLocation(basic_shader.Program, "phong_color");
    GLint optionLoc = glGetUniformLocation(basic_shader.Program, "option");
    //booleanos controladores
    bool ambient = false;
    bool lambert = false;
    bool phong = false;

    int option=0;
    float ambientColor[3] = { 1.0f,1.0f,1.0f };
    float lambertColor[3] = { 1.0f,1.0f,1.0f };
    float phongColor[3] = { 1.0f,1.0f,1.0f };
    cam = new camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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
            float samples[3000];
            for (int n = 0; n < 3000; n++)
                samples[n] = ImGui::GetIO().Framerate;
            ImGui::PlotHistogram("Samples", samples, 3000, 0, NULL, 0.0f, 100.0f, ImVec2(300, 50));
        }
        
        if (actual)
        {
            //// Slider that appears in the window
            sx = actual->getTam().x;
            sy = actual->getTam().y;
            sz = actual->getTam().z;
            ImGui::SliderFloat("Size x", &sx, 0.1f, 30.0f);
            ImGui::SliderFloat("Size y", &sy, 0.1f, 30.0f);
            ImGui::SliderFloat("Size z", &sz, 0.1f, 30.0f);
            actual->setTam(sx, sy, sz);

            //// Fancy color editor that appears in the window
            boundingColor = actual->getBouningColor();
            ImGui::ColorEdit3("BoundingBox Color", boundingColor);
            actual->setBouningColor(boundingColor);

            //// Fancy color editor that appears in the window
            objColor = actual->getDifuse_color();
            ImGui::ColorEdit3("Triangle Difuse Color", objColor);
            actual->setDifuse_color(objColor);
        }
        
        
        //// Ends the window
        ImGui::End();
        ImGui::Begin("Lights Menu", &my_tool_active, ImGuiWindowFlags_MenuBar);
        // Generate samples and plot them
        const char* items[] = { "Ambiental", "Lambert", "Phong" };
        static const char* current_item = NULL;
        const char* options[] = { "Only Object", "Mix", "Only Scene" };
        static const char* current_option = NULL;

        if (ImGui::BeginCombo("Light Type", current_item)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(items[n], is_selected)) 
                    current_item = items[n];
                if (current_item == items[n])
                {
                    if (n==0)
                    {
                        ambient = true;
                        lambert = false;
                        phong = false;
                    }
                    else if (n==1)
                    {
                        ambient = false;
                        lambert = true;
                        phong=false;
                    }
                    else if (n==2)
                    {
                        ambient = false;
                        lambert = false;
                        phong = true;
                    }
                    
                }
                if (is_selected) 
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                    
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Mix Option", current_option)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(options); n++)
            {
                bool is_selected = (current_option == options[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(options[n], is_selected))
                    current_option = options[n];
                if (current_option == options[n])
                {
                    option = n;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

            }
            ImGui::EndCombo();
        }
        if (ambient)
        {
            
            if (option!=0)
            {
                ImGui::ColorEdit3("Ambient Color", ambientColor);
                glUniform3f(ambientColorLoc, ambientColor[0], ambientColor[1], ambientColor[2]);
            }
        }
        if (lambert)
        {
            if (option != 0)
            {
                ImGui::ColorEdit3("Ambient Color", ambientColor);
                glUniform3f(ambientColorLoc, ambientColor[0], ambientColor[1], ambientColor[2]);
                ImGui::ColorEdit3("Difuse Color", lambertColor);
                glUniform3f(lambertColorLoc, lambertColor[0], lambertColor[1], lambertColor[2]);
            }
        }
        if (phong)
        {
            if (option != 0)
            {
                ImGui::ColorEdit3("Ambient Color", ambientColor);
                glUniform3f(ambientColorLoc, ambientColor[0], ambientColor[1], ambientColor[2]);
                ImGui::ColorEdit3("Difuse Color", lambertColor);
                glUniform3f(lambertColorLoc, lambertColor[0], lambertColor[1], lambertColor[2]);
                ImGui::ColorEdit3("Specular Color", phongColor);
                glUniform3f(phongColorLoc, phongColor[0], phongColor[1], phongColor[2]);
            }
            
        }
        glUniform1i(optionLoc, option);
        glUniform1i(lambertLoc, lambert);
        glUniform1i(ambientLoc, ambient);
        glUniform1i(phongLoc, phong);
        
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
        cout << mouse.x << " " << mouse.y<<endl;
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
}

void do_movement(GLfloat delta)
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        cam->move_front();
    if (keys[GLFW_KEY_S])
        cam->move_back();
    if (keys[GLFW_KEY_D])
        cam->move_right();
    if (keys[GLFW_KEY_A])
        cam->move_left();
    if (keys[GLFW_KEY_Q])
        cam->move_up();
    if (keys[GLFW_KEY_E])
        cam->move_down();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = WIDTH/2;
        lastY = HEIGHT/2;
    
        GLfloat xoffset = xpos - lastX;
        // Reversed since y-coordinates go from bottom to left
        GLfloat yoffset = lastY - ypos;  
        //glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);
        GLfloat dx = xoffset - xini;
        GLfloat dy = yoffset - yini;
        
        if (actual)
        {
            if (rotation)
                actual->rotateObj(dy/10, dx/10);
            if (moving)
                actual->traslateObj(dx/100, dy/100, 0);
                
        }
        xini = xoffset;
        yini = yoffset;
    }

}

