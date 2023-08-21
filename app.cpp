#include <app.h>

void App::Run()
{
    if (!openWindow())
        return;

    setUpInputs();

    running = true;

    setupScene();

    while (running) {
        float currentTime = glfwGetTime();
        if (this->lastFrameTime == -1.f) {
            this->lastFrameTime = currentTime;
        }

        auto deltaTime = currentTime - this->lastFrameTime;
        this->lastFrameTime = currentTime;

        // Render loop
        if (glfwWindowShouldClose(window)) {
            this->running = false;
            continue;
        }

        // Process input
        processInput(this->window);

        // Update scene
        update(deltaTime);

        // Render the scene
        draw();
    }

    glfwTerminate();
}

void App::setupScene() {


    // create the pyramid
    auto& pyr1 = projectObjects.emplace_back(std::make_unique<Pyramid>());
    //pyr1->Transform = glm::translate(pyr1->Transform, glm::vec3(0.0f, 0.0f, 0.0f));
    //pyr1->Transform = glm::scale(pyr1->Transform, glm::vec3(1.f, 1.f, 1.f));
    //pyr1->Transform = glm::rotate(pyr1->Transform, glm::radians(30.0f), glm::vec3(-1.f, 0.f, 0.f));


    auto& plane = projectObjects.emplace_back(std::make_unique<Plane>());

    auto& cylinder = projectObjects.emplace_back(std::make_unique<Cylinder>());
    auto& cylHandle = projectObjects.emplace_back(std::make_unique<Cube>());
    
    auto& smallBox = projectObjects.emplace_back(std::make_unique<SmallBox>());

    // directional - add back in when we build out separate dir light sub class
//    auto& light = projectObjects.emplace_back(std::make_unique<Light>());
    // hieght with vert and horiz rotation
//    light->Transform = glm::translate(light->Transform, glm::vec3(0.0f, 1.5f, 0.0f));
////    light->Transform = glm::translate(light->Transform, glm::vec3(0.0f, 0.5f, .5f));


    // create a point light (above pyramid)
    auto& pointLight = projectObjects.emplace_back(std::make_unique<PointLight>());
    // hieght with vert and horiz rotation
//    pointLight->Transform = glm::translate(pointLight->Transform, glm::vec3(1.0f, 1.0f, -0.3f));
    pointLight->Transform = glm::translate(pointLight->Transform, glm::vec3(0.0f, 1.4f, 0.0f));

    auto* castLight = reinterpret_cast<PointLight*>(pointLight.get());
    //castLight->AmbientColor = { 0.1f, 0.1f, 0.1f }; // change colors here
    //castLight->DiffuseColor = { 0.9f, 1.f, 0.7f };  // make diff brighter than amb
    //castLight->SpecColor = { 0.9f, 1.f, 0.7f };  // make spec brighter than amb

    castLight->AmbientColor = { 0.5f, 0.2f, 1.0f }; // change colors here
    castLight->DiffuseColor = { 0.5f, 0.2f, 1.0f };  // make diff brighter than amb
    castLight->SpecColor = { 0.1f, 0.1f, 0.1f };  // make spec brighter than amb


    // set constants
    castLight->ConstantVar = 1.f; // should always be 1
    castLight->LinearVar = 0.35f; 
    castLight->QuadVar = 0.44f;

      
    // add another point light (above box)
    auto& pointLight2 = projectObjects.emplace_back(std::make_unique<PointLight>());
    // hieght with vert and horiz rotation
//    pointLight2->Transform = glm::translate(pointLight2->Transform, glm::vec3(1.0f, 0.6f, -0.3f));
//    pointLight2->Transform = glm::translate(pointLight2->Transform, glm::vec3(0.0f, 1.4f, 1.8f));
    pointLight2->Transform = glm::translate(pointLight2->Transform, glm::vec3(0.1f, 0.0f, 1.4f));

    auto* castLight2 = reinterpret_cast<PointLight*>(pointLight2.get());
    //castLight2->AmbientColor = { 1.0f, 0.0f, 0.0f };
    //castLight2->DiffuseColor = { 1.0f, 0.f, 0.0f };  
    //castLight2->SpecColor = { 1.0f, 0.f, 0.0f };
    castLight2->AmbientColor = { 0.9f, 0.4f, 0.1f }; // Orangish
    castLight2->DiffuseColor = { 0.9f, 0.4f, 0.1f };
    castLight2->SpecColor = { 0.9f, 0.f, 0.1f };


    // set constants
    castLight2->ConstantVar = 1.f; // should always be 1
    castLight2->LinearVar = 0.1f;
    castLight2->QuadVar = 0.1f;



    // add another point light (near cylinder)
    auto& pointLight3 = projectObjects.emplace_back(std::make_unique<PointLight>());
    // hieght with vert and horiz rotation
    pointLight3->Transform = glm::translate(pointLight3->Transform, glm::vec3(3.0f, 1.0f, -0.3f));
//    pointLight3->Transform = glm::translate(pointLight3->Transform, glm::vec3(2.5f, 0.0f, -0.5f));

    auto* castLight3 = reinterpret_cast<PointLight*>(pointLight3.get());
    // Set constants for attenuation for the third point light
    //castLight3->AmbientColor = { 1.0f, 0.0f, 0.0f };
    //castLight3->DiffuseColor = { 1.0f, 1.f, 1.0f };
    //castLight3->SpecColor = { 1.0f, 1.f, 1.0f };
    castLight3->AmbientColor = { 0.0f, 0.6f, 1.0f }; // Bluish
    castLight3->DiffuseColor = { 0.0f, 0.6f, 1.0f };
    castLight3->SpecColor = { 0.2f, 0.2f, 0.2f };


    // Set constants for attenuation for the third point light
    castLight3->ConstantVar = 1.f; // should always be 1
    castLight3->LinearVar = 0.51f;
    castLight3->QuadVar = 0.51f;


}



void App::update(float deltaTime)
{

    glfwPollEvents();

    handleInput(deltaTime);


    // cycle through each object and update
    for (auto& object : projectObjects) {
        object->Update(deltaTime);
    }

}

void App::draw()
{
    // Clear the color and depth buffers
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get the view and projection matrices from the camera
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();

    // Set up the scene parameters for shaders
    SceneParameters sceneParameters{
        .ProjectionMatrix = projection,
        .ViewMatrix = view,
        .CamPosition = camera.GetCamPosition(),

        // Directional light properties
        .DirLight = {
            .Direction = glm::normalize(glm::vec3{ 0.0f, 0.7f, 0.5f }), // Direction of the light
            .AmbientColor = {0.1f, 0.1f, 0.1f}, // Ambient light color
            .DiffuseColor = {0.9f, 1.f, 0.3f},  // Diffuse light color (brighter than ambient)
            .SpecColor = {0.9f, 1.f, 0.3f}  // Specular light color (brighter than ambient)
        }
    };

    // Process lighting for each object in the scene
    for (auto& model : projectObjects) {
        model.get()->ProcessLighting(sceneParameters);
    }

    // Draw each object in the scene
    for (auto& model : projectObjects) {
        model.get()->Draw(sceneParameters);
    }

    // Swap the front and back buffers to display the rendered scene
    glfwSwapBuffers(window);
}


void App::setUpInputs()
{
    // Set up the keyboard input callback
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Get a pointer to the App instance from the GLFW window user pointer
        auto* a = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

        // Process key events
        switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) {
                // Close the application when ESC key is pressed
                a->running = false;
            }
            break;

        case GLFW_KEY_P:
            if (action == GLFW_PRESS) {
                // Toggle between perspective and orthographic camera views
                a->camera.SetIsPerspective(!a->camera.IsPerspective());
            }
            break;

        default:
            break;
        }
        });

    // Set up the cursor position callback (currently commented out)
    glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double xpos, double ypos) {
        // Get a pointer to the App instance from the GLFW window user pointer
        auto* a = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
        // Call the mouse position callback method
        // a->mousePositionCallBack(xpos, ypos);
        });

    // Set up the scroll callback
    glfwSetScrollCallback(this->window, [](GLFWwindow* window, double xOffset, double yOffset) {
        // Get a pointer to the App instance from the GLFW window user pointer
        auto* a = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

        // Uncomment the following line to control zoom with scroll
        // a->camera.IncrementZoom(yOffset * 2);

        // Change mouse sensitivity/movement speed
        // Note: This part may be temporary and subject to removal
        a->ChangeSpeed();
        });

    // Set up the mouse button callback
    glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods) {
        // Process mouse button events
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS) {
                // Left mouse button pressed
            }
            else {
                // Left mouse button released
            }
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS) {
                // Right mouse button pressed
            }
            else {
                // Right mouse button released
            }
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            if (action == GLFW_PRESS) {
                // Middle mouse button pressed
            }
            else {
                // Middle mouse button released
            }
            break;
        default:
            std::cout << "Unhandled mouse button event\n";
            break;
        }
        });
}


void App::handleInput(float deltaTime)
{
    // Calculate the movement amount based on the move speed and delta time
    auto moveAmount = this->moveSpeed * deltaTime;

    // Handle camera movement based on key inputs
    if (glfwGetKey(this->window, GLFW_KEY_W)) {
        this->camera.MoveCamera(Camera::MoveDirection::FORWARD, moveAmount);
    }

    if (glfwGetKey(this->window, GLFW_KEY_A)) {
        this->camera.MoveCamera(Camera::MoveDirection::LEFT, moveAmount);
    }

    if (glfwGetKey(this->window, GLFW_KEY_S)) {
        this->camera.MoveCamera(Camera::MoveDirection::BACKWARD, moveAmount);
    }

    if (glfwGetKey(this->window, GLFW_KEY_D)) {
        this->camera.MoveCamera(Camera::MoveDirection::RIGHT, moveAmount);
    }

    if (glfwGetKey(this->window, GLFW_KEY_Q)) {
        this->camera.MoveCamera(Camera::MoveDirection::UP, moveAmount);
    }

    if (glfwGetKey(this->window, GLFW_KEY_E)) {
        this->camera.MoveCamera(Camera::MoveDirection::DOWN, moveAmount);
    }

    // Get cursor position
    double xpos, ypos;
    glfwGetCursorPos(this->window, &xpos, &ypos);

    // Call mouse position callback method
    mousePositionCallBack(xpos, ypos);
}


// Adjust the mouse sensitivity/movement speed
// Note: This adjustment is temporary and should be removed after week 4
void App::ChangeSpeed() {
    // Increase the camera look speed up to a certain limit
    if (this->cameraLookSpeed.x <= 0.2f) {
        this->cameraLookSpeed.x += 0.01f;
        this->cameraLookSpeed.y += 0.01f;
    }
    // Reset the camera look speed to default values
    else {
        this->cameraLookSpeed.x = 0.05f;
        this->cameraLookSpeed.y = 0.05f;
    }

    // Output the updated camera look speeds for debugging
    std::cout << "Updated look speed x: " << this->cameraLookSpeed.x << "\n";
    std::cout << "Updated look speed y: " << this->cameraLookSpeed.y << "\n";
}


void App::mousePositionCallBack(double xpos, double ypos)
{
    if (!this->firstMouse) {
        this->lastMousePosition.x = static_cast<float>(xpos);
        this->lastMousePosition.y = static_cast<float>(ypos);
        this->firstMouse = true;
    }

    glm::vec2 moveAmount{
        xpos - this->lastMousePosition.x,
        this->lastMousePosition.y - ypos,
    };

    this->lastMousePosition.x = static_cast<float>(xpos);
    this->lastMousePosition.y = static_cast<float>(ypos);

    // to disable camera - comment this out
    this->camera.RotateBy(moveAmount.x * this->cameraLookSpeed.x, moveAmount.y * this->cameraLookSpeed.y);

}

// Initialize and open the GLFW window
bool App::openWindow() {
    // Initialize GLFW
    glfwInit();

    // Configure GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Set OpenGL major version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); // Set OpenGL minor version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use the core profile

    // Create the GLFW window
    window = glfwCreateWindow(this->width, this->height, this->appTitle.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set the created window as the current OpenGL context
    glfwMakeContextCurrent(window);

    // Associate the window pointer with the user data for callbacks
    glfwSetWindowUserPointer(window, (void*)this);

    // Set the framebuffer size callback to adjust the viewport
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
        // Set the viewport dimensions
        glViewport(0, 0, w, h);

        // Get the pointer to the App instance from the window
        auto a = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

        // Update the camera size
        a->camera.SetSize(w, h);
        });

    // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Set front face winding order
    glFrontFace(GL_CCW);

    // Cull back faces to improve rendering performance
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    return true;
}


// Callback function to handle changes in framebuffer size
//void App::framebuffer_size_callback(GLFWwindow* window, int w, int h) {
//    glViewport(0, 0, w, h);
//    // Update width and height
//    //width = w;
//    //height = h;
//}

// Process user input using GLFW
void App::processInput(GLFWwindow* window) {
    // Example: Close the window when the ESC key is pressed
    //if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //    glfwSetWindowShouldClose(window, true);
}
