
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
    float* objColor = nullptr, * boundingColor;

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
    //light 1
    GLint light1DirLoc = glGetUniformLocation(basic_shader.Program, "light1.position");
    GLint light1IntensityLoc = glGetUniformLocation(basic_shader.Program, "light1.intensity");
    GLint light1TypeLoc = glGetUniformLocation(basic_shader.Program, "light1.type");
    GLint light1DifuseLoc = glGetUniformLocation(basic_shader.Program, "light1.diffuse");
    GLint light1SpecularLoc = glGetUniformLocation(basic_shader.Program, "light1.specular");
    GLint light1ActiveLoc = glGetUniformLocation(basic_shader.Program, "light1.isActive");
    //light 2
    GLint light2DirLoc = glGetUniformLocation(basic_shader.Program, "light2.position");
    GLint light2IntensityLoc = glGetUniformLocation(basic_shader.Program, "light2.intensity");
    GLint light2TypeLoc = glGetUniformLocation(basic_shader.Program, "light2.type");
    GLint light2DifuseLoc = glGetUniformLocation(basic_shader.Program, "light2.diffuse");
    GLint light2SpecularLoc = glGetUniformLocation(basic_shader.Program, "light2.specular");
    GLint light2ActiveLoc = glGetUniformLocation(basic_shader.Program, "light2.isActive");
    //light 3
    GLint light3DirLoc = glGetUniformLocation(basic_shader.Program, "light3.position");
    GLint light3IntensityLoc = glGetUniformLocation(basic_shader.Program, "light3.intensity");
    GLint light3TypeLoc = glGetUniformLocation(basic_shader.Program, "light3.type");
    GLint light3DifuseLoc = glGetUniformLocation(basic_shader.Program, "light3.diffuse");
    GLint light3SpecularLoc = glGetUniformLocation(basic_shader.Program, "light3.specular");
    GLint light3ActiveLoc = glGetUniformLocation(basic_shader.Program, "light3.isActive");

    int option=0;
    float ambientColor[3] = { 1.0f,1.0f,1.0f };
    float lambertColor[3] = { 1.0f,1.0f,1.0f };
    float phongColor[3] = { 1.0f,1.0f,1.0f };
    cam = new camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    vector<mesh> subObjs;
    //light1
    float light1Dir[3]= { .0f,.0f,.0f };
    float temp1a[3] = { 0.0f,0.0f,.0f };
    float temp1b[3] = { 0.1f,0.1f,.1f };
    float light1Intensity=10;
    int light1Type=0;
    float light1Difuse[3]= { 1.0f,1.0f,1.0f };
    float light1Specular[3]= { 1.0f,1.0f,1.0f };
    bool light1Active = false;
    //light2
    float light2Dir[3] = { .0f,.0f,.0f };
    float temp2a[3] = { 0.0f,0.0f,.0f };
    float temp2b[3] = { 0.1f,0.1f,.1f };
    float light2Intensity = 10;
    int light2Type = 0;
    float light2Difuse[3] = { 1.0f,1.0f,1.0f };
    float light2Specular[3] = { 1.0f,1.0f,1.0f };
    bool light2Active = false;
    //light 3
    float light3Dir[3] = { .0f,.0f,.0f };
    float temp3a[3] = { 0.0f,0.0f,.0f };
    float temp3b[3] = { 0.1f,0.1f,.1f };
    float light3Intensity = 10;
    int light3Type = 0;
    float light3Difuse[3] = { 1.0f,1.0f,1.0f };
    float light3Specular[3] = { 1.0f,1.0f,1.0f };
    bool light3Active = false;
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

            //// Fancy color editor that appears in the window
            boundingColor = actual->getBouningColor();
            ImGui::ColorEdit3("BoundingBox Color", boundingColor);
            actual->setBouningColor(boundingColor);

            subObjs = actual->getMesh();
            for (int i = 0; i < subObjs.size(); i++)
            {
                ImGui::Separator();
                ImGui::Text(subObjs[i].nombre.data());
                string id = to_string(i);
                ImGui::ColorEdit3(id.append(" Ambient Color").data(), subObjs[i].material.ka);
                id = to_string(i);
                ImGui::ColorEdit3(id.append(" Difuse Color").data(), subObjs[i].material.kd);
                id = to_string(i);
                ImGui::ColorEdit3(id.append(" Specular Color").data(), subObjs[i].material.ks);
                id = to_string(i);
                ImGui::SliderFloat(id.append(" Shininess").data(), &subObjs[i].material.ns, .0f, 100.0f);
                /*ImGui::ColorEdit3("Difuse Color", subObjs[i].material.kd);
                ImGui::ColorEdit3("Specular Color", subObjs[i].material.ks);
                ImGui::SliderFloat("Shininess", &subObjs[i].material.ns,.0f,100.0f);*/
            }
            actual->setMesh(subObjs);

        }
        
        
        //// Ends the window
        ImGui::End();

        //menu luces
        ImGui::Begin("Lights Menu", &my_tool_active, ImGuiWindowFlags_MenuBar);
        // Generate samples and plot them
        const char* items[] = { "Ambiental", "Lambert", "Phong" };
        static const char* current_item = NULL;
        const char* options[] = { "Only Object", "Mix", "Only Scene" };
        static const char* current_option = NULL;
        ImGui::Separator();
        if (!light1Active)
        {
            if (ImGui::Button("Add Light 1")) {
                light1Active = true;
            }
        }
        else
        {
            const char* type1[] = { "PointLight", "DirectionalLight"};
            static const char* current_type1 = NULL;
            if (ImGui::BeginCombo("Light 1 Type", current_type1)) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < IM_ARRAYSIZE(type1); n++)
                {
                    bool is_selected = (current_type1 == type1[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(type1[n], is_selected))
                        current_type1 = type1[n];
                    if (current_type1 == type1[n])
                    {
                        light1Type = n+1;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

                }
                ImGui::EndCombo();
            }
            
            if (light1Type ==2)
            {
                
                ImGui::SliderFloat("Light 1 xa", &temp1a[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 1 ya", &temp1a[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 1 za", &temp1a[2], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 1 xb", &temp1b[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 1 yb", &temp1b[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 1 zb", &temp1b[2], -30.0f, 30.0f);
                glm::vec3 res=glm::normalize(glm::vec3(temp1a[0], temp1a[1], temp1a[2]) - glm::vec3(temp1b[0], temp1b[1], temp1b[2]));
                if (isnan(res.x)|| isnan(res.y)|| isnan(res.z))
                {
                    res.x = 0.1f;
                    res.y = 0.1f;
                    res.z = 0.1f;
                }
                light1Dir[0] = res.x;
                light1Dir[1] = res.y;
                light1Dir[2] = res.z;
            }
            else if(light1Type==1)
            {
                ImGui::SliderFloat("Light 1 x", &light1Dir[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 1 y", &light1Dir[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 1 z", &light1Dir[2], -30.0f, 30.0f);
            }
            ImGui::SliderFloat("Light 1 Intensity", &light1Intensity, 0.0f, 100.0f);
            ImGui::ColorEdit3("Light 1 Difuse", light1Difuse);
            ImGui::ColorEdit3("Light 1 specular", light1Specular);

            if (ImGui::Button("Delete Light 1")) {
                light1Active = false;
            }
        }
        //light 1
        glUniform3f(light1DirLoc, light1Dir[0], light1Dir[1], light1Dir[2]);
        glUniform1f(light1IntensityLoc, light1Intensity);
        glUniform1i(light1TypeLoc, light1Type);
        glUniform3f(light1DifuseLoc, light1Difuse[0], light1Difuse[1], light1Difuse[2]);
        glUniform3f(light1SpecularLoc, light1Specular[0], light1Specular[1], light1Specular[2]);
        glUniform1i(light1ActiveLoc, light1Active);
        ImGui::Separator();
        if (!light2Active)
        {
            if (ImGui::Button("Add Light 2")) {
                light2Active = true;
            }
        }
        else
        {
            const char* type2[] = { "PointLight", "DirectionalLight" };
            static const char* current_type2 = NULL;
            if (ImGui::BeginCombo("Light 2 Type", current_type2)) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < IM_ARRAYSIZE(type2); n++)
                {
                    bool is_selected = (current_type2 == type2[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(type2[n], is_selected))
                        current_type2 = type2[n];
                    if (current_type2 == type2[n])
                    {
                        light2Type = n + 1;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

                }
                ImGui::EndCombo();
            }
            if (light2Type == 2)
            {

                ImGui::SliderFloat("Light 2 xa", &temp2a[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 2 ya", &temp2a[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 2 za", &temp2a[2], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 2 xb", &temp2b[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 2 yb", &temp2b[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 2 zb", &temp2b[2], -30.0f, 30.0f);
                glm::vec3 res = glm::normalize(glm::vec3(temp2a[0], temp2a[1], temp2a[2]) - glm::vec3(temp2b[0], temp2b[1], temp2b[2]));
                if (isnan(res.x) || isnan(res.y) || isnan(res.z))
                {
                    res.x = 0.1f;
                    res.y = 0.1f;
                    res.z = 0.1f;
                }
                light2Dir[0] = res.x;
                light2Dir[1] = res.y;
                light2Dir[2] = res.z;
            }
            else if (light2Type == 1)
            {
                ImGui::SliderFloat("Light 2 x", &light2Dir[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 2 y", &light2Dir[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 2 z", &light2Dir[2], -30.0f, 30.0f);
            }
            ImGui::SliderFloat("Light 2 Intensity", &light2Intensity, 0.0f, 100.0f);
            ImGui::ColorEdit3("Light 2 Difuse", light2Difuse);
            ImGui::ColorEdit3("Light 2 specular", light2Specular);

            if (ImGui::Button("Delete Light 2")) {
                light2Active = false;
            }
        }
        glUniform3f(light2DirLoc, light2Dir[0], light2Dir[1], light2Dir[2]);
        glUniform1f(light2IntensityLoc, light2Intensity);
        glUniform1i(light2TypeLoc, light2Type);
        glUniform3f(light2DifuseLoc, light2Difuse[0], light2Difuse[1], light2Difuse[2]);
        glUniform3f(light2SpecularLoc, light2Specular[0], light2Specular[1], light2Specular[2]);
        glUniform1i(light2ActiveLoc, light2Active);
        ImGui::Separator();
        if (!light3Active)
        {
            if (ImGui::Button("Add Light 2")) {
                light3Active = true;
            }
        }
        else
        {
            const char* type3[] = { "PointLight", "DirectionalLight" };
            static const char* current_type3 = NULL;
            if (ImGui::BeginCombo("Light 3 Type", current_type3)) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < IM_ARRAYSIZE(type3); n++)
                {
                    bool is_selected = (current_type3 == type3[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(type3[n], is_selected))
                        current_type3 = type3[n];
                    if (current_type3 == type3[n])
                    {
                        light3Type = n + 1;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

                }
                ImGui::EndCombo();
            }
            if (light3Type == 2)
            {

                ImGui::SliderFloat("Light 3 xa", &temp3a[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 3 ya", &temp3a[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 3 za", &temp3a[2], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 3 xb", &temp3b[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 3 yb", &temp3b[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 3 zb", &temp3b[2], -30.0f, 30.0f);
                glm::vec3 res = glm::normalize(glm::vec3(temp3a[0], temp3a[1], temp3a[2]) - glm::vec3(temp3b[0], temp3b[1], temp3b[2]));
                if (isnan(res.x) || isnan(res.y) || isnan(res.z))
                {
                    res.x = 0.1f;
                    res.y = 0.1f;
                    res.z = 0.1f;
                }
                light3Dir[0] = res.x;
                light3Dir[1] = res.y;
                light3Dir[2] = res.z;
            }
            else if (light3Type == 1)
            {
                ImGui::SliderFloat("Light 3 x", &light3Dir[0], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 3 y", &light3Dir[1], -30.0f, 30.0f);
                ImGui::SliderFloat("Light 3 z", &light3Dir[2], -30.0f, 30.0f);
            }
            ImGui::SliderFloat("Light 3 Intensity", &light3Intensity, 0.0f, 100.0f);
            ImGui::ColorEdit3("Light 3 Difuse", light3Difuse);
            ImGui::ColorEdit3("Light 3 specular", light3Specular);

            if (ImGui::Button("Delete Light 3")) {
                light3Active = false;
            }
        }
        glUniform3f(light3DirLoc, light3Dir[0], light3Dir[1], light3Dir[2]);
        glUniform1f(light3IntensityLoc, light3Intensity);
        glUniform1i(light3TypeLoc, light3Type);
        glUniform3f(light3DifuseLoc, light3Difuse[0], light3Difuse[1], light3Difuse[2]);
        glUniform3f(light3SpecularLoc, light3Specular[0], light3Specular[1], light3Specular[2]);
        glUniform1i(light3ActiveLoc, light3Active);
        ImGui::Separator();
        if (ImGui::BeginCombo("Lighting Type", current_item)) // The second parameter is the label previewed before opening the combo.
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
        }/*
        if (ambient)
        {
            */
            if (option!=0)
            {
                ImGui::ColorEdit3("Ambient Color", ambientColor);
                glUniform3f(ambientColorLoc, ambientColor[0], ambientColor[1], ambientColor[2]);
            }
       /* }
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
            
        }*/
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
        lastX = static_cast<GLfloat>(WIDTH) / 2;
        lastY = static_cast<GLfloat>(HEIGHT) / 2;
    
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

