//STD
#include <iostream>
#include <cstdlib>
#include <ctime>

//GLAD
#include <glad/glad.h>

//GLM
#include "glm/ext/vector_float3.hpp"
#include <glm/gtc/type_ptr.hpp> //Access to the value_ptr

//ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//LEARNOPENGL
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>

//irrKlang
#include <irrKlang.h>

//GENERAL
#include "main.h"
#include "Shuttle.h"
#include "Ship.h"
#include "Laser.h"
#include "Section31.h"
#include "Collider.h"

using namespace std;
using namespace glm;
using namespace irrklang;

//Window
int windowWidth;
int windowHeight;

//VAO vertex attribute positions in correspondence to vertex attribute type
enum VAO_IDs { Triangles, Indices, Colours, Textures, NumVAOs = 2 };
//VAOs
GLuint VAOs[NumVAOs];

//Buffer types
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
//Buffer objects
GLuint Buffers[NumBuffers];

//Transformations
//Relative position within world space
vec3 cameraPosition = vec3(0.0f, -20.0f, 150.0f);
//The direction of travel
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
//Up position within world space
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
//Players ship position and rotation
vec3 shipPosition;
mat4 playerRoation;

//Camera sidways rotation
float cameraYaw = -90.0f;
//Camera vertical rotation
float cameraPitch = 0.0f;
//Determines if first entry of mouse into window
bool mouseFirstEntry = true;
//Positions of camera from given last frame
float cameraLastXPos = 800.0f / 2.0f;
float cameraLastYPos = 600.0f / 2.0f;

//Model-View-Projection Matrix
mat4 mvp;
mat4 model;
mat4 view;
mat4 projection;

//Time
//Time change
float deltaTime = 0.0f;
//Last value of time change
float lastFrame = 0.0f;
//Consecutive value of deltaTime
float currentTime = 0.0f;
//Time Remaining
int timeRemaining = 60;

//Shuttles
vector<Shuttle*> shuttles;
float shuttleTime;

//Ships
vector<Ship*> ships;
float shipTime;

//Lasers
vector<Laser*> lasers;
float shotCooldown;

//Skybox
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
GLuint skyboxVAO, skyboxVBO;

//Colliders
Collider stationCollider(vec3(-16.0f, -34.0f, -16.0f), vec3(16.0f, 11.0f, 16.0f));
Collider drydock_A_Collider(vec3(13.0f, -6.0f, -2.0f), vec3(17.0f, -3.0f, 3.0f));
Collider drydock_B_Collider(vec3(23.0f, -6.0f, -2.0f), vec3(27.0f, -3.0f, 3.0f));

//Section 31
vector<Section31*> section31;
int section31remaining;

//Game State Booleans
bool inHyperspace;

//Hyperspace
vector<vec3> starPositions;

//Sound Engine
ISoundEngine* engine;

int main()
{
    //Initialisation of GLFW
    glfwInit();
    //Initialisation of 'GLFWwindow' object
    windowWidth = 1280;
    windowHeight = 720;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Section 31 Takeover", NULL, NULL);

    //Checks if window has been successfully instantiated
    if (window == NULL)
    {
        cout << "GLFW Window did not instantiate\n";
        glfwTerminate();
        return -1;
    }

    engine = createIrrKlangDevice();

    if (!engine) {
        cout << "Failed to initialise sound engine\n";
        return -1;
    }

    //Sets cursor to automatically bind to window & hides cursor pointer
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //Binds OpenGL to window
    glfwMakeContextCurrent(window);

    //Initialisation of GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "GLAD failed to initialise\n";
        return -1;
    }
    
    //Loading of shaders
    Shader Shaders("shaders/vertexShader.vert", "shaders/fragmentShader.frag");
    
    //Object Models
    cout << "Loading Station Model" << endl;
    Model Station("media/Station/Station.obj");
    cout << "Loading Drydock Model" << endl;
    Model Drydock("media/Drydock/Drydock.obj");
    cout << "Loading Discovery Model" << endl;
    Model Discovery("media/Ships/Discovery/Discovery.obj");
    cout << "Loading Shuttle Model" << endl;
    Model ShuttleModel("media/Shuttle/Shuttle.obj");
    cout << "Loading Section31 Model" << endl;
    Model Section31Model("media/Section31/Section31.obj");

    //Effect Models
    cout << "Loading Laser Effect" << endl;
    Model Laser("media/Laser/laser.fbx");
    cout << "Loading Shield Effect" << endl;
    Model Shield("media/Shield/shield.fbx");
    cout << "Loading Explosion Effect" << endl;
    Model Explosion("media/Explosion/explosion.fbx");
    cout << "Loading Hyperspace Star Effect" << endl;
    Model HyperspaceStar("media/Hyperspace/hyperspaceStar.fbx");

    //UI Models
    cout << "Loading Time UI" << endl;
    Model UITime("media/UI/time.fbx");
    cout << "Loading Ships UI" << endl;
    Model UIShips("media/UI/ships.fbx");
    cout << "Loading Mission UI" << endl;
    Model UIMission("media/UI/mission.fbx");
    cout << "Loading Fail UI" << endl;
    Model UIFail("media/UI/fail.fbx");
    cout << "Loading Complete UI" << endl;
    Model UIComplete("media/UI/complete.fbx");

    //Numbers
    cout << "Loading Number 0" << endl;
    Model UI0("media/UI/Numbers/0.fbx");
    cout << "Loading Number 1" << endl;
    Model UI1("media/UI/Numbers/1.fbx");
    cout << "Loading Number 2" << endl;
    Model UI2("media/UI/Numbers/2.fbx");
    cout << "Loading Number 3" << endl;
    Model UI3("media/UI/Numbers/3.fbx");
    cout << "Loading Number 4" << endl;
    Model UI4("media/UI/Numbers/4.fbx");
    cout << "Loading Number 5" << endl;
    Model UI5("media/UI/Numbers/5.fbx");
    cout << "Loading Number 6" << endl;
    Model UI6("media/UI/Numbers/6.fbx");
    cout << "Loading Number 7" << endl;
    Model UI7("media/UI/Numbers/7.fbx");
    cout << "Loading Number 8" << endl;
    Model UI8("media/UI/Numbers/8.fbx");
    cout << "Loading Number 9" << endl;
    Model UI9("media/UI/Numbers/9.fbx");

    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
    std::vector<std::string> faces{
    "media/Skybox/right.jpg",
    "media/Skybox/left.jpg",
    "media/Skybox/top.jpg",
    "media/Skybox/bottom.jpg",
    "media/Skybox/front.jpg",
    "media/Skybox/back.jpg"
    };
    GLuint cubemapTexture = LoadCubemap(faces);
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    Shaders.use();

    //Sets the viewport size within the window to match the window size of 1280x720
    glViewport(0, 0, 1280, 720);

    //Sets the framebuffer_size_callback() function as the callback for the window resizing event
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Projection matrix
    projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 200.0f);

    //Initialize Section 31 Ships
    section31.push_back(new Section31(vec3(20.0f, 0.0f, 20.0f), mat4(1.0f)));
    section31.push_back(new Section31(vec3(-20.0f, 0.0f, 20.0f), mat4(1.0f)));
    section31.push_back(new Section31(vec3(20.0f, 0.0f, -20.0f), mat4(1.0f)));
    section31.push_back(new Section31(vec3(-20.0f, 0.0f, -20.0f), mat4(1.0f)));
    section31remaining = section31.size();

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    //Initialize Hyperspace
    inHyperspace = true;
    while (starPositions.size() < 300) {
        starPositions.push_back(vec3((rand() % 201) - 100.0f, (rand() % 201) - 100.0f, (rand() % 201) - 200.0f));
    }

    //Start background ambiance
    engine->play2D("media/Sounds/background.wav", true);

    //Render loop
    while (glfwWindowShouldClose(window) == false)
    {
        //Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Colour to display on cleared window
        glClear(GL_COLOR_BUFFER_BIT); //Clears the colour buffer
        glClear(GL_DEPTH_BUFFER_BIT); //Might need

        //Delta Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (deltaTime > 0.1f) {
            deltaTime = 0.1f;
        }

        //Hyperspace (Game start and completion)
        if (inHyperspace) {
            view = mat4(1.0f);

            //Render Scene
            Shaders.use();
            Shaders.setMat4("view", view);
            Shaders.setMat4("projection", projection);

            //Input
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                inHyperspace = false;

                //Reset Time
                timeRemaining = 60;

                //Reset Section 31 Ships
                for (Section31* ship : section31) {
                    delete ship;
                }
                section31.clear();

                section31.push_back(new Section31(vec3(20.0f, 0.0f, 20.0f), mat4(1.0f)));
                section31.push_back(new Section31(vec3(-20.0f, 0.0f, 20.0f), mat4(1.0f)));
                section31.push_back(new Section31(vec3(20.0f, 0.0f, -20.0f), mat4(1.0f)));
                section31.push_back(new Section31(vec3(-20.0f, 0.0f, -20.0f), mat4(1.0f)));
                section31remaining = section31.size();

                //Reset Position
                cameraPosition = vec3(0.0f, -20.0f, 150.0f);
                cameraFront = vec3(0.0f, 0.0f, -1.0f);
                cameraUp = vec3(0.0f, 1.0f, 0.0f);
            }

            //Draw Hyperspace
            for (vec3& position : starPositions) {
                if (position.z > 0) {
                    position.z = -200.0f;
                }
                else {
                    position.z += deltaTime * 175.0f;
                }

                model = mat4(1.0f);
                model = scale(model, vec3(1.0f, 1.0f, 5.0f));
                model = translate(model, position);
                SetMatrices(Shaders);
                HyperspaceStar.Draw(Shaders);
            }

            //Draw UI
            model = mat4(1.0f);
            model = scale(model, vec3(1.920f, 1.080f, 1.0f));
            model = translate(model, vec3(0.0f, 0.0f, -5.0f));
            SetMatrices(Shaders);

            glDisable(GL_DEPTH_TEST);
            //Failed
            if (timeRemaining <= 0) {
                UIFail.Draw(Shaders);
            }
            //Completed
            else if (section31remaining <= 0) {
                UIComplete.Draw(Shaders);
            }
            //Starting
            else {
                UIMission.Draw(Shaders);
            }
            glEnable(GL_DEPTH_TEST);

            //Refreshing
            glfwSwapBuffers(window); //Swaps the colour buffer
            glfwPollEvents(); //Queries all GLFW events

            //Back to start of while loop
            continue;
        }


        // Render the skybox
        glDepthFunc(GL_LEQUAL); // Change depth function for skybox
        skyboxShader.use();
        mat4 viewMatrix = mat4(mat3(view)); // Remove translation for skybox
        skyboxShader.setMat4("view", viewMatrix);
        skyboxShader.setMat4("projection", projection);

        // Bind the cubemap
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Restore depth function

        //Render Scene
        Shaders.use();
        Shaders.setMat4("view", view);
        Shaders.setMat4("projection", projection);

        //Time Remaining Calculations
        currentTime += deltaTime;
        if (currentTime >= 1.0f) {
            timeRemaining--;
            currentTime -= 1.0f;
        }

        if (timeRemaining <= 0) {
            inHyperspace = true;
            engine->play2D("media/Sounds/fail.wav");
        }

        //Input
        ProcessUserInput(window);
        ProcessMouseInput(window, deltaTime);

        glEnable(GL_CULL_FACE); //Discards all back-facing triangles
        glEnable(GL_DEPTH_TEST);

        //Transformations & Drawing
        //Viewer orientation
        view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); //Sets the position of the viewer, the movement direction in relation to it & the world up direction

        //Station
        model = mat4(1.0f);
        model = rotate(model, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        model = scale(model, vec3(0.1f, 0.1f, 0.1f));
        SetMatrices(Shaders);
        Station.Draw(Shaders);

        //Drydock A
        model = mat4(1.0f);
        model = translate(model, vec3(15.0f, -4.4f, 0.0f));
        model = scale(model, vec3(0.015f, 0.015f, 0.015f));
        SetMatrices(Shaders);
        Drydock.Draw(Shaders);

        //Drydock B
        model = mat4(1.0f);
        model = translate(model, vec3(25.0f, -4.4f, 0.0f));
        model = scale(model, vec3(0.015f, 0.015f, 0.015f));
        SetMatrices(Shaders);
        Drydock.Draw(Shaders);

        //
        //Ships
        // 

        //Players Ship
        //cout << "X: " << cameraPosition.x << " Y: " << cameraPosition.y << " Z: " << cameraPosition.z << endl;
        model = mat4(1.0f);
        model = translate(model, shipPosition);
        model *= playerRoation;
        model = scale(model, vec3(0.01f, 0.01f, 0.01f));
        SetMatrices(Shaders);
        Discovery.Draw(Shaders);

        //Drydocked Ship
        model = mat4(1.0f);
        model = translate(model, vec3(25.0f, -5.0f, -0.5f));
        model = scale(model, vec3(0.01f, 0.01f, 0.01f));
        SetMatrices(Shaders);
        Discovery.Draw(Shaders);

        //
        //Generated Ships
        //
        
        //Spawn
        shipTime += deltaTime;
        if (shipTime > 1.0f) {
            shipTime = 0.0f;

            int spawnShip = std::rand() % 101;
            if (spawnShip < 10) {
                if (spawnShip < 5) {
                    ships.push_back(new Ship(0.1f, true));
                }
                else {
                    ships.push_back(new Ship(0.2f, false));
                }
            }
        }

        //Move and Display
        for (int i = 0; i < ships.size(); i++) {
            if (ships[i] != nullptr) {
                vec3 position = ships[i]->UpdatePosition(deltaTime);
                if (position == vec3(0.0f, 0.0f, 0.0f)) {
                    delete ships[i];
                    ships[i] = nullptr;
                }
                else {
                    model = mat4(1.0f);
                    model = translate(model, position);
                    model = scale(model, vec3(0.01f, 0.01f, 0.01f));
                    model = rotate(model, radians(ships[i]->GetRotation()), vec3(0.0f, 1.0f, 0.0f));
                    SetMatrices(Shaders);
                    Discovery.Draw(Shaders);
                }
            }
        }

        //
        //Shuttles
        //

        //Spawn
        shuttleTime += deltaTime;
        if (shuttleTime > 0.5f) {
            shuttleTime = 0.0f;

            int spawnShuttle = std::rand() % 101;
            if (spawnShuttle < 40) {
                if (spawnShuttle < 20) {
                    shuttles.push_back(new Shuttle(0.5f, 1));
                }
                else {
                    shuttles.push_back(new Shuttle(0.5f, 2));
                }
            }
            else if (spawnShuttle < 80) {
                shuttles.push_back(new Shuttle(0.5f, 3));
            }
        }

        //Move and Display
        for (int i = 0; i < shuttles.size(); i++) {
            if (shuttles[i] != nullptr) {
                vec3 position = shuttles[i]->UpdatePosition(deltaTime);
                if (position == vec3(0.0f, 0.0f, 0.0f)) {
                    delete shuttles[i];
                    shuttles[i] = nullptr;
                }
                else {
                    model = mat4(1.0f);
                    model = translate(model, position);
                    model = scale(model, vec3(0.01f, 0.01f, 0.01f));
                    SetMatrices(Shaders);
                    ShuttleModel.Draw(Shaders);
                }
            }
        }

        //
        //Lasers
        //

        //Move and Display
        for (int i = 0; i < lasers.size(); i++) {
            if (lasers[i] != nullptr) {
                if (lasers[i]->hit) {
                    if (lasers[i]->RemoveShield(deltaTime)) {
                        delete lasers[i];
                        lasers[i] = nullptr;
                    }
                    else {
                        model = mat4(1.0f);
                        model = translate(model, lasers[i]->GetPosition());
                        model *= lasers[i]->GetRotation();
                        model = scale(model, vec3(0.25f, 0.25f, 0.25f));
                        SetMatrices(Shaders);
                        Shield.Draw(Shaders);
                    }
                }
                else {
                    vec3 position = lasers[i]->UpdatePosition(deltaTime);
                    if (position == vec3(0.0f, 0.0f, 0.0f)) {
                        delete lasers[i];
                        lasers[i] = nullptr;
                    }
                    else {
                        //Check collisions
                        //With Station and Drydocks
                        if (lasers[i]->collider.Intersects(stationCollider)) {
                            lasers[i]->hit = true;
                            engine->play2D("media/Sounds/shield.wav");
                        }
                        else if (lasers[i]->collider.Intersects(drydock_A_Collider)) {
                            lasers[i]->hit = true;
                            engine->play2D("media/Sounds/shield.wav");
                        }
                        else if (lasers[i]->collider.Intersects(drydock_B_Collider)) {
                            lasers[i]->hit = true;
                            engine->play2D("media/Sounds/shield.wav");
                        }
                        else {
                            //Check Section 31 Colliders
                            bool section31hit = false;

                            for (int j = 0; j < section31.size(); j++) {
                                if (lasers[i] != nullptr && section31[j] != nullptr) {
                                    if (lasers[i]->collider.Intersects(section31[j]->collider)) {
                                        section31hit = true;
                                        section31[j]->health--;

                                        if (section31[j]->health <= 0) {
                                            engine->play2D("media/Sounds/explosion.wav");
                                            delete lasers[i];
                                            lasers[i] = nullptr;
                                        }
                                        else {
                                            lasers[i]->hit = true;
                                            engine->play2D("media/Sounds/shield.wav");
                                        }
                                    }
                                }
                            }

                            if (!section31hit) {
                                model = mat4(1.0f);
                                model = translate(model, position);
                                model *= lasers[i]->GetRotation();
                                model = scale(model, vec3(0.05f, 0.05f, 0.3f));
                                SetMatrices(Shaders);
                                Laser.Draw(Shaders);
                            }
                        }
                    }
                }
            }
        }

        //
        //Section 31
        //

        //Display
        for (int i = 0; i < section31.size(); i++) {
            if (section31[i] != nullptr) {
                vec3 position = section31[i]->GetPosition();
                mat4 rotation = section31[i]->GetRotation();

                if (section31[i]->health <= 0) {
                    bool isDone = section31[i]->ExplosionOver(deltaTime);
                    if (isDone) {
                        delete section31[i];
                        section31[i] = nullptr;
                        section31remaining--;
                    }
                    else {
                        model = mat4(1.0f);
                        model = translate(model, position);
                        model *= rotation;
                        model = scale(model, vec3(2.0f, 2.0f, 2.0f));
                        SetMatrices(Shaders);
                        Explosion.Draw(Shaders);
                    }
                }
                else {
                    model = mat4(1.0f);
                    model = translate(model, position);
                    model *= rotation;
                    model = scale(model, vec3(0.03f, 0.03f, 0.03f));
                    SetMatrices(Shaders);
                    Section31Model.Draw(Shaders);
                }
            }
        }

        //
        //UI
        //

        //Time Header
        model = mat4(1.0f);
        model = translate(model, vec3(0.0f, 30.0f, 0.0f));
        model *= playerRoation;
        model = scale(model, vec3(6.0f, 2.0f, 2.0f));
        SetMatrices(Shaders);
        UITime.Draw(Shaders);

        //Time Numbers
        mat4 numberBaseTransform = model;
        numberBaseTransform = scale(numberBaseTransform, vec3(1.333f / 6.0f, 1.0f, 1.0f));

        //First Digit
        int digit = (timeRemaining / 10) % 10;

        model = numberBaseTransform;
        model = translate(model, vec3(-1.0f, -2.0f, 0.0f));
        SetMatrices(Shaders);
        
        switch (digit) {
            case 0:
                UI0.Draw(Shaders);
                break;
            case 1:
                UI1.Draw(Shaders);
                break;
            case 2:
                UI2.Draw(Shaders);
                break;
            case 3:
                UI3.Draw(Shaders);
                break;
            case 4:
                UI4.Draw(Shaders);
                break;
            case 5:
                UI5.Draw(Shaders);
                break;
            case 6:
                UI6.Draw(Shaders);
                break;
            case 7:
                UI7.Draw(Shaders);
                break;
            case 8:
                UI8.Draw(Shaders);
                break;
            case 9:
                UI9.Draw(Shaders);
                break;
        }

        //Second Digit
        digit = timeRemaining % 10;

        model = numberBaseTransform;
        model = translate(model, vec3(1.0f, -2.0f, 0.0f));
        SetMatrices(Shaders);
        
        switch (digit) {
            case 0:
                UI0.Draw(Shaders);
                break;
            case 1:
                UI1.Draw(Shaders);
                break;
            case 2:
                UI2.Draw(Shaders);
                break;
            case 3:
                UI3.Draw(Shaders);
                break;
            case 4:
                UI4.Draw(Shaders);
                break;
            case 5:
                UI5.Draw(Shaders);
                break;
            case 6:
                UI6.Draw(Shaders);
                break;
            case 7:
                UI7.Draw(Shaders);
                break;
            case 8:
                UI8.Draw(Shaders);
                break;
            case 9:
                UI9.Draw(Shaders);
                break;
        }

        //Remaining Section31 Ships
        model = mat4(1.0f);
        model = translate(model, vec3(0.0f, 20.0f, 0.0f));
        model *= playerRoation;
        model = scale(model, vec3(6.0f, 2.0f, 2.0f));
        SetMatrices(Shaders);
        UIShips.Draw(Shaders);

        //Amount
        numberBaseTransform = model;
        numberBaseTransform = scale(numberBaseTransform, vec3(1.333f / 6.0f, 1.0f, 1.0f));
        model = numberBaseTransform;
        model = translate(model, vec3(0.0f, -2.0f, 0.0f));
        SetMatrices(Shaders);

        switch (section31remaining) {
        case 0:
            UI0.Draw(Shaders);
            inHyperspace = true;
            engine->play2D("media/Sounds/complete.wav");
            break;
        case 1:
            UI1.Draw(Shaders);
            break;
        case 2:
            UI2.Draw(Shaders);
            break;
        case 3:
            UI3.Draw(Shaders);
            break;
        case 4:
            UI4.Draw(Shaders);
            break;
        case 5:
            UI5.Draw(Shaders);
            break;
        case 6:
            UI6.Draw(Shaders);
            break;
        case 7:
            UI7.Draw(Shaders);
            break;
        case 8:
            UI8.Draw(Shaders);
            break;
        case 9:
            UI9.Draw(Shaders);
            break;
        }

        //Refreshing
        glfwSwapBuffers(window); //Swaps the colour buffer
        glfwPollEvents(); //Queries all GLFW events
    }

    //Safely terminates GLFW
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Resizes window based on contemporary width & height values
    glViewport(0, 0, width, height);
}

void ProcessMouseInput(GLFWwindow* window, float deltaTime)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;

    float xOffset = (float)xpos - centerX;
    float yOffset = centerY - (float)ypos;

    const float deadZone = 10.0f;
    if (!(fabs(xOffset) < deadZone && fabs(yOffset) < deadZone))
    {
        const float sensitivity = 0.001f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        cameraYaw += xOffset;
        cameraPitch += yOffset;

        const float maxPitch = 75.0f;
        const float minPitch = -75.0f;
        if (cameraPitch > maxPitch) {
            cameraPitch = maxPitch;
        }
        else if (cameraPitch < minPitch) {
            cameraPitch = minPitch;
        }

        vec3 direction;
        direction.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
        direction.y = sin(radians(cameraPitch));
        direction.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
        cameraFront = normalize(direction);
    }

    //Ship Position
    shipPosition = cameraPosition + cameraFront * 3.0f - cameraUp * 0.5f;

    //Ship Rotation
    vec3 right = normalize(cross(cameraFront, cameraUp));
    vec3 adjustedUp = normalize(cross(right, cameraFront));
    playerRoation = mat4(1.0f);
    playerRoation[0] = vec4(right, 0.0f);
    playerRoation[1] = vec4(adjustedUp, 0.0f);
    playerRoation[2] = vec4(-cameraFront, 0.0f);

    //Check if the player can shoot
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        shotCooldown += deltaTime;
        if (shotCooldown >= 0.5f) {
            shotCooldown = 0.0f;
            lasers.push_back(new Laser(20.0f, shipPosition, -playerRoation));
            engine->play2D("media/Sounds/laser.wav");
        }
    }
}

void ProcessUserInput(GLFWwindow* WindowIn)
{
    //Closes window on 'exit' key press
    if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(WindowIn, true);
    }

    //Extent to which to move in one instance
    const float movementSpeed = 10.0f * deltaTime;
    const float rollSpeed = 45.0f * deltaTime;

    //WASD controls
    if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= movementSpeed * cameraFront;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }

    //Up and Down
    if (glfwGetKey(WindowIn, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPosition += movementSpeed * cameraUp;
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraPosition -= movementSpeed * cameraUp;
    }

    //Rolling (Works on its own but does not work with camera movement)
    /*if (glfwGetKey(WindowIn, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cameraUp = vec3(glm::rotate(mat4(1.0f), radians(-rollSpeed), cameraFront) * vec4(cameraUp, 1.0f));
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_E) == GLFW_PRESS)
    {
        cameraUp = vec3(glm::rotate(mat4(1.0f), radians(rollSpeed), cameraFront) * vec4(cameraUp, 1.0f));
    }*/
}

void SetMatrices(Shader& ShaderProgramIn)
{
    mvp = projection * view * model; //Setting of MVP
    ShaderProgramIn.setMat4("mvpIn", mvp); //Setting of uniform with Shader class
}

GLuint LoadCubemap(std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to load cubemap texture at " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}