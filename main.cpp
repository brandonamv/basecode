
#include <main.h>
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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

string saveFile(string fileText)
{
    char const* lTheSaveFileName;
    char const* lFilterPatterns[2] = { "*.txt", "*.text" };

    lTheSaveFileName = tinyfd_saveFileDialog(
        "let us save this password",
        "particle.txt",
        2,
        lFilterPatterns,
        NULL);

    if (!lTheSaveFileName)
    {
        tinyfd_messageBox(
            "Error",
            "Save file name is NULL",
            "ok",
            "error",
            1);
        return "";
    }
    lIn = fopen(lTheSaveFileName, "w");
    if (!lIn)
    {
        tinyfd_messageBox(
            "Error",
            "Can not open this file in write mode",
            "ok",
            "error",
            1);
        return "";
    }
    fputs(fileText.data(), lIn);
    fclose(lIn);
    return lTheSaveFileName;
}

// The MAIN function, from here we start the application and run the game loop
int main(){

    srand(time(0));
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto B CG2 - UCV", /*glfwGetPrimaryMonitor()*/NULL,  nullptr);
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
    Shader basic_shader("color_shader.vs", "", "color_shader.frag");
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
    static bool opc_play=true;
    static bool opc_blending;
    static bool opc_deph_sort;
    float opc_background_color[3] = { 0.0f, 1.0f, 1.0f };
    float opc_camera_speed = 5.0f;
    float opc_anim_speed = 1.0f;

    //menu creacion particulas
    static bool menu_particle;
    float min_box[3] = { .0f,.0f,.0f };
    float max_box[3] = { .0f,.0f,.0f };
    float color_ini[4] = { .0f,.0f,.0f, 1.0f };
    float color_ini_variance[4] = { .0f,.0f,.0f,.0f };
    float color_fin[4] = { .0f,.0f,.0f, .0f };
    float color_fin_variance[4] = { .0f,.0f,.0f,.0f };
    int particles_generation = 1;
    int particles_generation_variance = 1;
    float particles_life = 1.0f;
    float particles_life_variance = 1.0f;
    vec3 particles_direction(.0f,.0f,.0f);
    float particles_direction_variance[3] = { .0f,.0f, .0f };
    float particles_speed = .01f;
    float particles_speed_variance = .01f;
    static bool particles_texture = false;
    static bool particles_point = true;
    int particles_max = 1;
    float particles_size = .1f;
    float particles_size_variance = 3.0f;
    float particles_scale_ini = .1f;
    float particles_scale_ini_variance = 3.0f;
    float particles_scale_fin = .1f;
    float particles_scale_fin_variance = 3.0f;
    float particle_mass=1.0f;
    float particle_mass_variance = 1.0f;

    
    float sx,sy,sz;
    float* objColor = nullptr, * boundingColor;


    basic_shader.Use();
    // Get the uniform locations
    GLint viewLoc = glGetUniformLocation(basic_shader.Program, "view");
    GLint projLoc = glGetUniformLocation(basic_shader.Program, "projection");
    cam = new camera(glm::vec3(0.0f, 1.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    
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
        ImGui::Checkbox("Blending", &opc_blending);/*
        ImGui::gizmo3D("guizmo", light);*/
        if (opc_blending)
        {
            glEnable(GL_BLEND);
            const char* items[] = { "GL_ZERO", "GL_ONE", "GL_SRC_COLOR", "GL_ONE_MINUS_SRC_COLOR", "GL_DST_COLOR", "GL_ONE_MINUS_DST_COLOR", "GL_SRC_ALPHA", "GL_ONE_MINUS_SRC_ALPHA", "GL_DST_ALPHA", "GL_ONE_MINUS_DST_ALPHA", "GL_CONSTANT_COLOR", "GL_ONE_MINUS_CONSTANT_COLOR", "GL_CONSTANT_ALPHA", "GL_ONE_MINUS_CONSTANT_ALPHA" };
            static const char* current_item = "GL_ONE";
            static const char* current_item1 = "GL_ONE";
            GLenum blend1[] = { GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA };
            int b1=1, b2=1;
            ImGui::Text("Blend Function 1");
            if (ImGui::BeginCombo("bFunc1", current_item)) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(items[n], is_selected))
                        current_item = items[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                    
                }
                ImGui::EndCombo();
            }
            ImGui::Text("Blend Function 2");
            if (ImGui::BeginCombo("bFunc2", current_item1)) // The second parameter is the label previewed before opening the combo.
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    bool is_selected = (current_item1 == items[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(items[n], is_selected))
                        current_item1 = items[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                    
                }
                ImGui::EndCombo();
            }
            for (int i = 0; i < IM_ARRAYSIZE(items); i++)
            {
                if (current_item == items[i])
                    b1 = i;
                if (current_item1 == items[i])
                    b2 = i;
            }
            glBlendFunc(blend1[b1],blend1[b2]);
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

        if (ImGui::Button("New Particles"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            particle_system.push_back(new ParticleGenerator());
            particle_actual = particle_system.back();
        }

        if (!particle_system.empty())
        {
            ImGui::Separator();
            ImGui::Text("Animation Speed");
            ImGui::SliderFloat("s_ptcle", &opc_anim_speed, 0.1f, 10.0f);
            ImGui::Checkbox("Gravity", &opc_gravity);
            ImGui::Checkbox("Play/Stop Animation", &opc_play);
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

        if (!particle_system.empty())
        {
            particles_max = particle_actual->getMaxParticles();
            particles_generation = particle_actual->getNewParticles();
            particles_generation_variance = particle_actual->getNewParticlesVariance();
            particles_life = particle_actual->getLifeTime();
            particles_life_variance = particle_actual->getLifeTimeVar();
            particles_speed = particle_actual->getSpeed();
            particles_speed_variance = particle_actual->getSpeedVar();
            particles_size = particle_actual->getSize();
            particles_size_variance = particle_actual->getSizeVar();
            particles_scale_ini = particle_actual->getScaleIni();
            particles_scale_ini_variance = particle_actual->getScaleIniVar();
            particles_scale_fin = particle_actual->getScaleFin();
            particles_scale_fin_variance = particle_actual->getScaleFinVar();
            color_ini[0] = particle_actual->getInitColor()[0];
            color_ini[1] = particle_actual->getInitColor()[1];
            color_ini[2] = particle_actual->getInitColor()[2];
            color_ini[3] = particle_actual->getInitColor()[3];
            color_ini_variance[0] = particle_actual->getInitColorVariance()[0];
            color_ini_variance[1] = particle_actual->getInitColorVariance()[1];
            color_ini_variance[2] = particle_actual->getInitColorVariance()[2];
            color_ini_variance[3] = particle_actual->getInitColorVariance()[3];
            color_fin[0] = particle_actual->getFinColor()[0];
            color_fin[1] = particle_actual->getFinColor()[1];
            color_fin[2] = particle_actual->getFinColor()[2];
            color_fin[3] = particle_actual->getFinColor()[3];
            color_fin_variance[0] = particle_actual->getFinColorVariance()[0];
            color_fin_variance[1] = particle_actual->getFinColorVariance()[1];
            color_fin_variance[2] = particle_actual->getFinColorVariance()[2];
            color_fin_variance[3] = particle_actual->getFinColorVariance()[3];
            min_box[0] = particle_actual->getMin()[0];
            min_box[1] = particle_actual->getMin()[1];
            min_box[2] = particle_actual->getMin()[2];
            max_box[0] = particle_actual->getMax()[0];
            max_box[1] = particle_actual->getMax()[1];
            max_box[2] = particle_actual->getMax()[2];
            particles_direction[0] = particle_actual->getDirection()[0];
            particles_direction[1] = particle_actual->getDirection()[1];
            particles_direction[2] = particle_actual->getDirection()[2];
            particles_direction_variance[0] = particle_actual->getDirectionVar()[0];
            particles_direction_variance[1] = particle_actual->getDirectionVar()[1];
            particles_direction_variance[2] = particle_actual->getDirectionVar()[2];
            particle_mass = particle_actual->getMass();
            particle_mass_variance = particle_actual->getMassVar();
            particles_point = particle_actual->getOptPoint();
            
            ImGui::Begin("Particle Menu", &menu_particle, ImGuiWindowFlags_MenuBar);
            ImGui::InputFloat3("Spawn Init", min_box);
            ImGui::InputFloat3("Spawn Fin", max_box);
            ImGui::Checkbox("Point", &particles_point);
            if (particles_point)
            {
                ImGui::SliderFloat("Size", &particles_size, 1.0f, 5.0f);
                ImGui::SliderFloat("Size Var", &particles_size_variance, 1.0f, 5.0f);
            }
            else
            {
                if (!particle_actual->getTexture())
                {
                    if (ImGui::Button("Add Texture"))
                    {
                        
                        string img = openFile();
                        if (img != "")
                        {
                            int width, height, nrChannels;
                            stbi_set_flip_vertically_on_load(true);
                            unsigned char* data = stbi_load(img.data(), &width, &height, &nrChannels, 0);
                            if (data)
                            {
                                particle_actual->setTexture(data, width, height, true, img);
                            }
                            else
                            {
                                std::cout << "Failed to load texture" << std::endl;
                            }
                            stbi_image_free(data);
                        }
                    }
                }
                else {
                    if (ImGui::Button("Delete Texture"))
                    {
                        
                        particle_actual->setTexture(nullptr, 0, 0, false,"");
                    }
                }
                ImGui::SliderFloat("Size", &particles_size, 1.0f, 3.0f);
                ImGui::SliderFloat("Size Var", &particles_size_variance, 1.0f, 3.0f);
            }
            if (opc_gravity)
            {
                ImGui::InputFloat("Mass", &particle_mass, .1f, 1.0f);
                ImGui::InputFloat("Mass Variance", &particle_mass_variance, .1f, 1.0f);
            }
            ImGui::SliderInt("Max Particles", &particles_max, 1, 10000);
            ImGui::SliderInt("Particles/sec", &particles_generation, 1, 1000);
            ImGui::SliderInt("Particles/sec Var", &particles_generation_variance, 1, 100);
            ImGui::SliderFloat("LifeTime", &particles_life, 0.1f, 10.0f, "%.2f secs");
            ImGui::SliderFloat("LifeTime Var", &particles_life_variance, 0.1f, 3.0f, "%.2f secs");
            ImGui::SliderFloat("Speed", &particles_speed, 0.01f, 100.0f);
            ImGui::SliderFloat("Speed Var", &particles_speed_variance, 0.01f, 30.0f);
            ImGui::SliderFloat("Scale Init", &particles_scale_ini, 0.1f, 10.0f);
            ImGui::SliderFloat("Scale Init Var", &particles_scale_ini_variance, .1f, 3.0f);
            ImGui::SliderFloat("Scale Fin", &particles_scale_fin, 0.1f, 10.0f);
            ImGui::SliderFloat("Scale Fin Var", &particles_scale_fin_variance, .1f, 3.0f);
            ImGui::ColorEdit4("InitColor", color_ini);
            ImGui::SliderFloat4("Init Var", color_ini_variance, .0f, 1.0f);
            ImGui::ColorEdit4("FinalColor", color_fin);
            ImGui::SliderFloat4("Final Var", color_fin_variance, .0f, 1.0f);
            ImGui::SliderFloat3("Direction Var", particles_direction_variance, .0f, .5f);
            ImGui::gizmo3D("Direction", particles_direction);
            particle_actual->setMaxParticles(particles_max);
            particle_actual->setNewParticles(particles_generation);
            particle_actual->setNewParticlesVariance(particles_generation_variance);
            particle_actual->setLifeTime(particles_life);
            particle_actual->setLifeTimeVar(particles_life_variance);
            particle_actual->setSpeed(particles_speed);
            particle_actual->setSpeedVar(particles_speed_variance);
            particle_actual->setSize(particles_size);
            particle_actual->setSizeVar(particles_size_variance);
            particle_actual->setScale(particles_scale_ini, particles_scale_fin);
            particle_actual->setScaleVar(particles_scale_ini_variance, particles_scale_fin_variance);
            particle_actual->setColor(
                glm::vec4(color_ini[0], color_ini[1], color_ini[2], color_ini[3]),
                glm::vec4(color_fin[0], color_fin[1], color_fin[2], color_fin[3])
            );
            particle_actual->setRGBAvariance(
                glm::vec4(color_ini_variance[0], color_ini_variance[1], color_ini_variance[2], color_ini_variance[3]),
                glm::vec4(color_fin_variance[0], color_fin_variance[1], color_fin_variance[2], color_fin_variance[3])
            );
            particle_actual->setMinMax(
                glm::vec3(min_box[0], min_box[1], min_box[2]),
                glm::vec3(max_box[0], max_box[1], max_box[2])
            );
            particle_actual->setDirection(
                glm::vec3(particles_direction[0], particles_direction[1], particles_direction[2])
            );
            particle_actual->setDirectionVar(
                glm::vec3(particles_direction_variance[0], particles_direction_variance[1], particles_direction_variance[2])
            );
            particle_actual->setMass(particle_mass);
            particle_actual->setMassVar(particle_mass_variance);
            particle_actual->setOptPoint(particles_point);
            if (ImGui::Button("Save"))
            {
                string tempName;
                size_t numero;
                
                tempName=saveFile(particle_actual->save());
                numero=tempName.find_last_of("\\");
                
                if (particle_actual->getTexture())
                {
                    int width, height, nrChannels;
                    stbi_set_flip_vertically_on_load(true);
                    string imgName = particle_actual->getTextureData();
                    unsigned char* data = stbi_load(imgName.data(), &width, &height, &nrChannels, 0);
                    if (data)
                    {
                        if (imgName[imgName.find_last_of(".") + 1] == 'j')
                        {
                            stbi_write_jpg(string(tempName.substr(0, tempName.find_last_of(".")) + imgName.substr(imgName.find_last_of("."), imgName.size())).data(), width, height, nrChannels, data, 100);
                        }
                        else if (imgName[imgName.find_last_of(".") + 1] == 'p')
                        {
                            stbi_write_png(string(tempName.substr(0, tempName.find_last_of(".")) + imgName.substr(imgName.find_last_of("."), imgName.size())).data(), width, height, nrChannels, data, width* nrChannels);
                        }
                        else {
                            stbi_write_bmp(string(tempName.substr(0, tempName.find_last_of(".")) + imgName.substr(imgName.find_last_of("."), imgName.size())).data(), width, height, nrChannels, data);
                        }
                        
                    }
                    else
                    {
                        std::cout << "Failed to load texture" << std::endl;
                    }
                    stbi_image_free(data);
                }
                

            }
            if (ImGui::Button("Load"))
            {
                string temp = openFile();
                if (temp!="")
                {
                    particle_actual->load(temp);
                    if (particle_actual->getTexture())
                    {
                        int width, height, nrChannels;
                        stbi_set_flip_vertically_on_load(true);
                        string imgName = particle_actual->getTextureData();
                        unsigned char* data = stbi_load(imgName.data(), &width, &height, &nrChannels, 0);
                        if (data)
                        {
                            
                            particle_actual->setTexture(data, width, height, true, imgName);
                        }
                        else
                        {
                            std::cout << "Failed to load texture" << std::endl;
                        }
                        stbi_image_free(data);
                    }
                }
                
            }
            ImGui::End();
            
        }

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
        
        if (!particle_system.empty())
        {
            
            for (const auto& x : particle_system)
            {
                x->setAnimSpeed(opc_anim_speed);
                x->setGravity(opc_gravity);
                x->setOptBlending(opc_blending);
                if (opc_play)x->Update(deltaTime);
                x->Draw(view, projection);
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

