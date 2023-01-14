#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema1/transform2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

Mesh* Tema1::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* Tema1::CreateCircle(
    const std::string& name,
    int radius,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    const unsigned int fragments = 360;

    float increment = 2.0f * M_PI / fragments;
    unsigned int i = 0;
    indices.clear();
    vertices.clear();
    for (float currAngle = 0.0f; currAngle <= 2.0f * M_PI; currAngle += increment) {
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(currAngle), radius * sin(currAngle), 0), color));
        indices.push_back(i);
        i++;
    }

    Mesh* circle = new Mesh(name);
    circle->InitFromData(vertices, indices);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    return circle;
}

Mesh* Tema1::CreateTriangle(
    const std::string& name,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    //triangle (body, wings)
    //color = glm::vec3(0.5f, 0.35f, 0.05f);
    vertices =
    {
        VertexFormat(glm::vec3(-40, -50, 0), color),
        VertexFormat(glm::vec3(40, -50, 0), color),
        VertexFormat(glm::vec3(0, 100, 0), color)
    };

    Mesh* triangle = new Mesh(name);
    indices = { 0, 1, 2 };
    triangle->InitFromData(vertices, indices);
    return triangle;
}

void Tema1::Init()
{
    srand(static_cast <unsigned> (time(0)));
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    translateX = 0;
    translateY = 0;
    scaleX = 1;
    scaleY = 1;
    timeElapsed = 0;
    round = 0;
    lives = 3;
    bullets = 3;
    count = 0;
    deltaTime = 0;
    birdDED = false;
    score = 0;
    bodyAngle = 0;
    birdEscaping = false;
    speed_multiplier = 1;

    Mesh* circle = CreateCircle("head", 25, glm::vec3(0.05f, 0.2156f, 0.05f));
    AddMeshToList(circle);

    Mesh* body = CreateTriangle("body", glm::vec3(0.5f, 0.35f, 0.05f));
    AddMeshToList(body);
    Mesh* wing1 = CreateTriangle("wing1", glm::vec3(0.36f, 0.25f, 0.2f));
    AddMeshToList(wing1);
    Mesh* wing2 = CreateTriangle("wing2", glm::vec3(0.36f, 0.25f, 0.2f));
    AddMeshToList(wing2);
    Mesh* beak = CreateTriangle("beak", glm::vec3(1, 1, 0));
    AddMeshToList(beak);

    Mesh* bullet1 = CreateRectangle("rect1", glm::vec3(0, 0, 0), 20, 30, glm::vec3(0, 0, 1), false);
    AddMeshToList(bullet1);
    Mesh* bullet2 = CreateRectangle("rect2", glm::vec3(0, 0, 0), 20, 30, glm::vec3(0, 0, 1), false);
    AddMeshToList(bullet2);
    Mesh* bullet3 = CreateRectangle("rect3", glm::vec3(0, 0, 0), 20, 30, glm::vec3(0, 0, 1), false);
    AddMeshToList(bullet3);

    Mesh* box = CreateRectangle("box", glm::vec3(0, 0, 0), 150, 210, glm::vec3(0, 0, 1), false);
    AddMeshToList(box);

    Mesh* scoreBar = CreateRectangle("scoreBar", glm::vec3(0, 0, 0), 120, 20, glm::vec3(0, 0, 1), false);
    AddMeshToList(scoreBar);

    Mesh* life1 = CreateCircle("life1", 10, glm::vec3(1, 0, 0));
    AddMeshToList(life1);
    Mesh* life2 = CreateCircle("life2", 10, glm::vec3(1, 0, 0));
    AddMeshToList(life2);
    Mesh* life3 = CreateCircle("life3", 10, glm::vec3(1, 0, 0));
    AddMeshToList(life3);
}


void Tema1::FrameStart()
{
    glClearColor(0.529, 0.807, 0.921, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::RenderDuck(float bodyX, float bodyY)
{
    bodyMatrix = glm::mat3(1);
    bodyMatrix *= transform2D::Translate(bodyX, bodyY) *
                    transform2D::Rotate(bodyAngle) *
                    transform2D::Rotate(-M_PI / 2);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], bodyMatrix);
    // glm::mat3 boxMatrix = bodyMatrix * transform2D::Translate(-75, -50);
    // RenderMesh2D(meshes["box"], shaders["VertexColor"], boxMatrix);

    float step = M_PI / 180;

    if (count % 22 == 0) {
        wingRotation = 0;
    }
    else {
        wingRotation += step;
    }
    glm::mat3 wing1Matrix = bodyMatrix * transform2D::Translate(27, 20) *
        transform2D::Rotate(-1.57079633f - wingRotation) * transform2D::Scale(0.5, 0.5);
    RenderMesh2D(meshes["wing1"], shaders["VertexColor"], wing1Matrix);

    glm::mat3 wing2Matrix = bodyMatrix * transform2D::Translate(-27, 20) *
        transform2D::Rotate(1.57079633f + wingRotation) * transform2D::Scale(0.5, 0.5);
    RenderMesh2D(meshes["wing2"], shaders["VertexColor"], wing2Matrix);

    glm::mat3 headMatrix = bodyMatrix * transform2D::Translate(0, 110);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], headMatrix);

    glm::mat3 beakMatrix = headMatrix * transform2D::Translate(0, 30) *
        transform2D::Scale(0.2, 0.2);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], beakMatrix);
}

void Tema1::RenderGUI()
{
    glm::ivec2 resolution = window->GetResolution();
    glm::mat3 matrix;

    Mesh* grass = CreateRectangle("grass", glm::vec3(0, 0, 0), resolution.x, 130, glm::vec3(0, 1, 0), true);
    AddMeshToList(grass);
    matrix = glm::mat3(1);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], matrix);

    if (score < 13) {
        Mesh* currentScore = CreateRectangle("currentScore", glm::vec3(0, 0, 0), (score % 13) * 9.91 , 18, glm::vec3(0, 1, 0), true);
        AddMeshToList(currentScore);
    }

    matrix = glm::mat3(1);
    matrix *= transform2D::Translate(40, resolution.y - 150);
    RenderMesh2D(meshes["scoreBar"], shaders["VertexColor"], matrix);
    RenderMesh2D(meshes["currentScore"], shaders["VertexColor"], matrix *
                                transform2D::Translate(1, 0));

    matrix = glm::mat3(1);
    matrix *= transform2D::Translate(40, resolution.y - 100);
    int b = bullets;
    if (b > 0) {
        RenderMesh2D(meshes["rect1"], shaders["VertexColor"], matrix);
        b--;

        if (b > 0) {
            matrix *= transform2D::Translate(50, 0);
            RenderMesh2D(meshes["rect2"], shaders["VertexColor"], matrix);
            b--;
        }

        if (b > 0) {
            matrix *= transform2D::Translate(50, 0);
            RenderMesh2D(meshes["rect3"], shaders["VertexColor"], matrix);
            b--;
        }
    }

    int l = lives;
    if (l > 0) {
        matrix = glm::mat3(1);
        matrix *= transform2D::Translate(50, resolution.y - 50);

        RenderMesh2D(meshes["life1"], shaders["VertexColor"], matrix);
        l--;

        if (l > 0) {
            matrix *= transform2D::Translate(50, 0);
            RenderMesh2D(meshes["life2"], shaders["VertexColor"], matrix);
            l--;
        }

        if (l > 0) {
            matrix *= transform2D::Translate(50, 0);
            RenderMesh2D(meshes["life3"], shaders["VertexColor"], matrix);
            l--;
        }
    }
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    deltaTime += deltaTimeSeconds;

    if (birdDED)
    {
        if (bodyY > 0)
        {
            bodyY -= 8;
            bodyAngle = 3 * M_PI / 2;
            RenderGUI();
            RenderDuck(bodyX, bodyY);
        }
        else 
        {
            bullets = 3;
            birdDED = false;
            timeElapsed = 0;
            score++;
        }
    }
    else if (bullets <= 0)
    {
        birdEscaping = true;
        if (bodyY < resolution.y + 50) {
            bodyY += 8;
            bodyAngle = M_PI / 2;
            count++;
            RenderDuck(bodyX, bodyY);
            RenderGUI();
        }
        else {
            lives--;
            timeElapsed = 0;
            bullets = 3;
        }
    }
    else if (lives <= 0)
    {
        window->Close();
    }
    else 
    {

        if (timeElapsed == 0) {
            round++;
            birdDED = false;
            birdEscaping = false;
            float speed = speed_multiplier * (5 + (int)(deltaTimeSeconds * resolution.x) % 5);

            if (round % 5 == 0) {
                speed_multiplier += 0.2;
            }
            bodyX = 30 + rand() % (resolution.x - 30);
            bodyY = 50;

            float HI, LO;
            if (rand() % 2) {
                HI = M_PI / 3;
                LO = M_PI / 6;
            }
            else {
                HI = 5 * M_PI / 6;
                LO = 2 * M_PI / 3;
            }
            bodyAngle = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
            translateX = speed;
            translateY = abs(tan(bodyAngle) * translateX);

            RenderDuck(bodyX, bodyY);

            timeElapsed += deltaTimeSeconds;
        }
        else if (timeElapsed < 10) {
            RenderGUI();

            if (bodyX < resolution.x - 10 && bodyY < resolution.y - 30 && bodyX > 10 && bodyY > 10) {
                bodyX += translateX;
                bodyY += translateY;
            }
            else {
                if (bodyX >= resolution.x - 10) {
                    translateX = -translateX;
                    bodyX = resolution.x - 11;
                }
                else if (bodyY >= resolution.y - 30) {
                    translateY = -translateY;
                    bodyY = resolution.y - 31;
                }
                else if (bodyX <= 10) {
                    bodyX = 11;
                    translateX = -translateX;
                }
                else if (bodyY <= 10) {
                    bodyY = 11;
                    translateY = -translateY;
                }
            }
            bodyAngle = atan2(translateY, translateX);

            RenderDuck(bodyX, bodyY);

            count++;
            timeElapsed += deltaTimeSeconds;
        }
        else {
            birdEscaping = true;
            if (bodyY < resolution.y + 50) {
                bodyY += 8;
                bodyAngle = M_PI / 2;
                count++;
                RenderDuck(bodyX, bodyY);
                RenderGUI();
            }
            else {
                lives--;
                timeElapsed = 0;
            }
        }
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // cout << mouseX << " " << window->GetResolution().y - mouseY << endl;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    glm::vec2 leftBottomCorner = glm::vec2(bodyX - 80, bodyY - 50);
    glm::vec2 rightTopCorner = glm::vec2(bodyX + 80, bodyY + 100);

    mouseY = window->GetResolution().y - mouseY;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
    {
        if (mouseX > leftBottomCorner.x && mouseX < rightTopCorner.x &&
            mouseY > leftBottomCorner.y && mouseY < rightTopCorner.y &&
            birdEscaping == false)
        {
            birdDED = true;
        }
        else {
            bullets--;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
