#include "lab_m1/Lab5/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

bool Tema2::SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b)
{
    glm::vec3 cp1 = glm::cross(b - a, p1 - a);
    glm::vec3 cp2 = glm::cross(b - a, p2 - a);

    return (glm::dot(cp1, cp2) >= 0);
}

bool Tema2::InsideTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P)
{
    return SameSide(P, A, B, C) && SameSide(P, B, A, C) && SameSide(P, C, A, B);
}

bool Tema2::InsideTrack(glm::vec3 P)
{
    for (int i = 0; i < trackIndices.size() - 2; i += 3) {
        // cout << minimalTrackIndices.at(i) << " " << minimalTrackIndices.at(i + 1) << " " << minimalTrackIndices.at(i + 2) << endl;
        if (InsideTriangle(4.f * trackVertices.at(trackIndices.at(i)).position,
                4.f * trackVertices.at(trackIndices.at(i + 1)).position,
                4.f * trackVertices.at(trackIndices.at(i + 2)).position,
                P)) 
        {
            return true;
        }
    }
    return false;
}

Mesh* Tema2::CreateParaDrept(const char* name, glm::vec3 color)
{
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-0.5, -0.5,  0.5), color, glm::vec3(0.2, 0.8, 0.2)),
        VertexFormat(glm::vec3(0.5, -0.5,  0.5), color, glm::vec3(0.9, 0.4, 0.2)),
        VertexFormat(glm::vec3(-0.5,  0.5,  0.5), color, glm::vec3(0.7, 0.7, 0.1)),
        VertexFormat(glm::vec3(0.5,  0.5,  0.5), color, glm::vec3(0.7, 0.3, 0.7)),
        VertexFormat(glm::vec3(-0.5, -0.5, -1.5), color, glm::vec3(0.3, 0.5, 0.4)),
        VertexFormat(glm::vec3(0.5, -0.5, -1.5), color, glm::vec3(0.5, 0.2, 0.9)),
        VertexFormat(glm::vec3(-0.5,  0.5, -1.5), color, glm::vec3(0.7, 0.0, 0.7)),
        VertexFormat(glm::vec3(0.5,  0.5, -1.5), color, glm::vec3(0.1, 0.5, 0.8)),
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2,
        2, 3, 7,        2, 7, 6,
        1, 7, 3,        1, 5, 7,
        6, 7, 4,        7, 5, 4,
        0, 4, 1,        1, 4, 5,
        2, 6, 4,        0, 2, 4,
    };

    return CreateMesh(name, vertices, indices);
}


void Tema2::Init()
{
    right = 10.f;
    left = .01f;
    bottom = .01f;
    top = 10.f;
    time = 0;
    carMatrix = glm::mat4(1);
    angle = 0;
    acceleration = 0;
    traffic_index_1 = 0;
    traffic_index_2 = 250;

    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    cameraMiniMap = new implemented::Camera();
    cameraPlayer = new implemented::Camera();
    cameraPlayer->Set(glm::vec3(0, 2.4f, -20.f), glm::vec3(0, 1, -23.5f), glm::vec3(0, 1, 0));
    glm::vec3 pos = cameraPlayer->GetTargetPosition();
    cameraMiniMap->Set(pos + glm::vec3(0, 30, 0), pos, glm::vec3(0, 0, 1));


        // GRASS
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-10, 0,  -10), glm::vec3(0, 0.601, 0.089), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(10, 0,  -10), glm::vec3(0, 0.601, 0.089), glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-10,  0,  10), glm::vec3(0, 0.601, 0.089), glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3(10,  0,  10), glm::vec3(0, 0.601, 0.089), glm::vec3(0.7, 0.3, 0.7))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2
        };

        CreateMesh("grass", vertices, indices);
    }

    glm::vec3 BLACK = glm::vec3(0.164, 0.1601, 0.1328);

    vector<VertexFormat> originalVertices
    {
        VertexFormat(glm::vec3(10, 0,  -5), BLACK),            // A
        VertexFormat(glm::vec3(0, 0,  -5), BLACK),             // B
        VertexFormat(glm::vec3(-10,  0,  -5), BLACK),          // C
        VertexFormat(glm::vec3(-13.5,  0,  -2.76), BLACK),      // D
        VertexFormat(glm::vec3(-15, 0,  0), BLACK),             // E
        VertexFormat(glm::vec3(-20, 0,  1.15), BLACK),             // F
        VertexFormat(glm::vec3(-25,  0,  5), BLACK),          // G
        VertexFormat(glm::vec3(-23.3,  0,  9.2), BLACK),      // H
        VertexFormat(glm::vec3(-17.27, 0,  11.8), BLACK),            // I
        VertexFormat(glm::vec3(-13.13, 0,  12.15), BLACK),             // J
        VertexFormat(glm::vec3(-9.18,  0,  12.1), BLACK),          // K
        VertexFormat(glm::vec3(-7.1,  0,  8.3), BLACK),      // L
        VertexFormat(glm::vec3(-11, 0,  7.4), BLACK),            // M
        VertexFormat(glm::vec3(-16.7, 0,  6.6), BLACK),             // N
        VertexFormat(glm::vec3(-14,  0,  4), BLACK),          // O
        VertexFormat(glm::vec3(-9.9,  0,  2.1), BLACK),      // P
        VertexFormat(glm::vec3(-3.4, 0,  1.8), BLACK),            // Q
        VertexFormat(glm::vec3(1.8, 0,  6.25), BLACK),             // R
        VertexFormat(glm::vec3(5.79,  0,  10.5), BLACK),          // S
        VertexFormat(glm::vec3(10,  0,  13.6), BLACK),      // T
        VertexFormat(glm::vec3(15.1, 0,  12.6), BLACK),            // U
        VertexFormat(glm::vec3(17.8, 0,  9.1), BLACK),             // V
        VertexFormat(glm::vec3(20,  0,  5), BLACK),          // W
        VertexFormat(glm::vec3(24.3,  0,  0.7), BLACK),      // X
        VertexFormat(glm::vec3(27.1, 0,  -5), BLACK),            // Y
        VertexFormat(glm::vec3(18.2, 0,  -5), BLACK),             // Z
    };

    /*
        // MINIMAL TRACK 
    {
        for (int i = 0; i < originalVertices.size() - 1; i++) {
            glm::vec3 P1 = originalVertices.at(i).position;
            glm::vec3 P2 = originalVertices.at(i + 1).position;
            glm::vec3 D = P2 - P1;
            glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
            glm::vec3 R = P1 + glm::vec3(1.5 * P.x, 1.5 * P.y, 1.5 * P.z);
            minimalTrackVertices.push_back(VertexFormat(R, BLACK));
            glm::vec3 B = P1 - glm::vec3(1.5 * P.x, 1.5 * P.y, 1.5 * P.z);
            minimalTrackVertices.push_back(VertexFormat(B, BLACK));
        }

        // BETWEEN LAST AND FIRST SEGEMENT
        glm::vec3 P1 = originalVertices.at(originalVertices.size() - 1).position;
        glm::vec3 P2 = originalVertices.at(0).position;
        glm::vec3 D = P2 - P1;
        glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
        glm::vec3 R = P1 + glm::vec3(1.5 * P.x, 1.5 * P.y, 1.5 * P.z);
        minimalTrackVertices.push_back(VertexFormat(R, BLACK));
        glm::vec3 B = P1 - glm::vec3(1.5 * P.x, 1.5 * P.y, 1.5 * P.z);
        minimalTrackVertices.push_back(VertexFormat(B, BLACK));

        int i = 0, j = 1, k = 3;
        while (k < minimalTrackVertices.size()) {
            minimalTrackIndices.push_back(i);
            minimalTrackIndices.push_back(j);
            minimalTrackIndices.push_back(k);
            k--;
            j += 2;

            minimalTrackIndices.push_back(i);
            minimalTrackIndices.push_back(j);
            minimalTrackIndices.push_back(k);
            i += 2;
            k += 3;
        }

        // BETWEEN LAST AND FIRST SEGEMENT
        minimalTrackIndices.push_back(minimalTrackVertices.size() - 2);
        minimalTrackIndices.push_back(minimalTrackVertices.size() - 1);
        minimalTrackIndices.push_back(1);

        minimalTrackIndices.push_back(minimalTrackVertices.size() - 2);
        minimalTrackIndices.push_back(1);
        minimalTrackIndices.push_back(0);
    }
    */

        // TRACK AND TREES
    vector<VertexFormat> vertices;
    {
        {
            int size = originalVertices.size();
            for (int i = 0; i < size - 1; i++) {
                vertices.push_back(originalVertices[i]);
                double intervalX = (originalVertices[i + 1].position.x - originalVertices[i].position.x) / 50;
                double intervalZ = (originalVertices[i + 1].position.z - originalVertices[i].position.z) / 50;
                for (int k = 1; k < 50; k++) {
                    vertices.push_back(VertexFormat(glm::vec3(
                        originalVertices[i].position.x + k * intervalX,
                            0,
                        originalVertices[i].position.z + k * intervalZ),
                        BLACK));
                }
            }
            // BETWEEN LAST AND FIRST SEGEMENT
            vertices.push_back(originalVertices[originalVertices.size() - 1]);
            double intervalX = (originalVertices[0].position.x - originalVertices[originalVertices.size() - 1].position.x) / 50;
            double intervalZ = (originalVertices[0].position.z - originalVertices[originalVertices.size() - 1].position.z) / 50;
            for (int k = 1; k < 50; k++) {
                vertices.push_back(VertexFormat(glm::vec3(
                    originalVertices[originalVertices.size() - 1].position.x + k * intervalX,
                    0,
                    originalVertices[originalVertices.size() - 1].position.z + k * intervalZ),
                    BLACK));
            }
        }

        for (int i = 0; i < vertices.size() - 1; i++) {
            glm::vec3 P1 = vertices.at(i).position;
            glm::vec3 P2 = vertices.at(i + 1).position;
            glm::vec3 D = P2 - P1;
            glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0 ,1, 0)));
            glm::vec3 R = P1 + glm::vec3(1.5 * P.x, 1.5 * P.y, 1.5 * P.z);
            trackVertices.push_back(VertexFormat(R, BLACK));
            glm::vec3 B = P1 - glm::vec3(1.5 * P.x, 1.5 * P.y, 1.5 * P.z);
            trackVertices.push_back(VertexFormat(B, BLACK));
        }

        // BETWEEN LAST AND FIRST SEGEMENT
        glm::vec3 P1 = vertices.at(vertices.size() - 1).position;
        glm::vec3 P2 = vertices.at(0).position;
        glm::vec3 D = P2 - P1;
        glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
        glm::vec3 R = P1 + glm::vec3(1.5 * P.x, 1.5 * P.y, 1.5 * P.z);
        trackVertices.push_back(VertexFormat(R, BLACK));
        glm::vec3 B = P1 - glm::vec3(1.5 * P.x, 1.5 * P.y, 1.5 * P.z);
        trackVertices.push_back(VertexFormat(B, BLACK));

        int i = 0, j = 1, k = 3;
        while (k < trackVertices.size()) {
            trackIndices.push_back(i);
            trackIndices.push_back(j);
            trackIndices.push_back(k);
            k--;
            j += 2;
            
            trackIndices.push_back(i);
            trackIndices.push_back(j);
            trackIndices.push_back(k);
            i += 2;
            k += 3;
        }

        // BETWEEN LAST AND FIRST SEGEMENT
        trackIndices.push_back(trackVertices.size() - 2);
        trackIndices.push_back(trackVertices.size() - 1);
        trackIndices.push_back(1);

        trackIndices.push_back(trackVertices.size() - 2);
        trackIndices.push_back(1);
        trackIndices.push_back(0);

        CreateMesh("track", trackVertices, trackIndices);

        // TREES
        {
            int size = originalVertices.size();
            for (int i = 0; i < vertices.size() - 1; i += 80) {
                glm::vec3 P1 = vertices.at(i).position;
                glm::vec3 P2 = vertices.at(i + 1).position;
                glm::vec3 D = P2 - P1;
                glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
                glm::vec3 R = P1 + glm::vec3(2 * P.x, 2 * P.y, 2 * P.z);
                treeVertices.push_back(VertexFormat(R, BLACK));
                glm::vec3 B = P1 - glm::vec3(2 * P.x, 2 * P.y, 2 * P.z);
                treeVertices.push_back(VertexFormat(B, BLACK));
            }
            treeVertices.erase(treeVertices.begin() + 15);
            treeVertices.erase(treeVertices.begin() + 15);
            treeVertices.erase(treeVertices.begin() + 18);
            treeVertices.erase(treeVertices.end() - 3);
        }
    }

        // TRAFFIC
    {
        for (int i = 0; i < vertices.size() - 1; i++) {
            glm::vec3 P1 = vertices.at(i).position;
            glm::vec3 P2 = vertices.at(i + 1).position;
            glm::vec3 D = P2 - P1;
            glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
            glm::vec3 R = P1 + 0.7f * glm::vec3(P.x, P.y, P.z);
            traffic_route_1.push_back(VertexFormat(R, BLACK));
            glm::vec3 B = P1 - 0.7f * glm::vec3(P.x, P.y, P.z);
            traffic_route_2.push_back(VertexFormat(B, BLACK));
        }

        // BETWEEN LAST AND FIRST SEGEMENT
        glm::vec3 P1 = vertices.at(vertices.size() - 1).position;
        glm::vec3 P2 = vertices.at(0).position;
        glm::vec3 D = P2 - P1;
        glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
        glm::vec3 R = P1 + 0.7f * glm::vec3(P.x, P.y, P.z);
        traffic_route_1.push_back(VertexFormat(R, BLACK));
        glm::vec3 B = P1 - 0.7f * glm::vec3(P.x, P.y, P.z);
        traffic_route_2.push_back(VertexFormat(B, BLACK));

    }

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab6", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab6", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        CreateParaDrept("trunk", glm::vec3(0.394, 0.2617, 0.1289));
        CreateParaDrept("vehicle", glm::vec3(0.25, 0.875, 0.8125));
        CreateParaDrept("traffic1", glm::vec3(0.1, 0.875, 0.8125));
        CreateParaDrept("traffic2", glm::vec3(0.8, 0.3, 0.8125));
        CreateParaDrept("treeBush", glm::vec3(0, 0.875, 0.0925));
    }
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderScene(){
    {
        for (int i = -200; i < 200; i += 20)
            for (int j = -200; j < 200; j += 20) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 0.1f, j));
                RenderMesh(meshes["grass"], shaders["LabShader"], modelMatrix);
            }
    }

    {
        carMatrix = glm::translate(glm::mat4(1), cameraPlayer->GetTargetPosition());
        carMatrix = glm::rotate(carMatrix, angle, glm::vec3(0, 0.25f, 0));
        RenderMesh(meshes["vehicle"], shaders["VertexColor"], carMatrix);
    }

        // TRAFFIC
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        if (traffic_index_1 >= traffic_route_1.size())
            traffic_index_1 = 0;
        glm::vec3 P1 = traffic_route_1[traffic_index_1].position;
        glm::vec3 P2;
        if (traffic_index_1 == traffic_route_1.size() - 1)
            P2 = traffic_route_1[0].position;
        else
            P2 = traffic_route_1[traffic_index_1 + 1].position;
        glm::vec3 D = P2 - P1;
        float thisAngle = atan2(D.x, D.z);

        modelMatrix = glm::translate(modelMatrix, P1 * 4.f + glm::vec3(0, 0.7f, 0));
        modelMatrix = glm::rotate(modelMatrix, thisAngle, glm::vec3(0, 1, 0));
        RenderMesh(meshes["traffic1"], shaders["LabShader"], modelMatrix);

        modelMatrix = glm::mat4(1);
        if (traffic_index_2 >= traffic_route_2.size())
            traffic_index_2 = 0;
        P1 = traffic_route_2[traffic_index_2].position;
        if (traffic_index_2 == traffic_route_2.size() - 1)
            P2 = traffic_route_2[0].position;
        else
            P2 = traffic_route_2[traffic_index_2 + 1].position;
        D = P2 - P1;
        thisAngle = atan2(D.x, D.z);

        modelMatrix = glm::translate(modelMatrix, P1 * 4.f + glm::vec3(0, 0.7f, 0));
        modelMatrix = glm::rotate(modelMatrix, thisAngle, glm::vec3(0, 1, 0));
        RenderMesh(meshes["traffic2"], shaders["LabShader"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4.f, 0, 4.f));
        RenderMesh(meshes["track"], shaders["LabShader"], modelMatrix);
    }

    for (int i = 0; i < treeVertices.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(treeVertices[i].position.x * 4,
            treeVertices[i].position.y * 4 + 1,
            treeVertices[i].position.z * 4));
        glm::mat4 bushMatrix = modelMatrix;
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 6, 0.5));
        RenderMesh(meshes["trunk"], shaders["LabShader"], modelMatrix);
        bushMatrix = glm::translate(bushMatrix, glm::vec3(0, 3, 0.5));
        bushMatrix = glm::scale(bushMatrix, glm::vec3(4, 4, 2));
        RenderMesh(meshes["treeBush"], shaders["LabShader"], bushMatrix);
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    traffic_index_1++;
    traffic_index_2++;
    camera = cameraPlayer;
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);
    RenderScene();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    camera = cameraMiniMap;
    projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    RenderScene();
}

void Tema2::FrameEnd()
{
    // DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int location = glGetUniformLocation(shader->GetProgramID(), "Model");

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    int projLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glm::vec3 carPos = cameraPlayer->GetTargetPosition();
    int carPosLocation = glGetUniformLocation(shader->GetProgramID(), "CarPos");
    glUniform3fv(carPosLocation, 1, glm::value_ptr(carPos));


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
        time += deltaTime;
        float speed = 15.0f;
        float sensivityOY = 1.6f;

        float colision_radius = 1.f;
        glm::vec3 player_pos = cameraPlayer->GetTargetPosition();
        player_pos.y = 0;
        glm::vec3 traffic_pos_1 = traffic_route_1[traffic_index_1].position * 4.f;
        traffic_pos_1.y = 0;
        glm::vec3 traffic_pos_2 = traffic_route_2[traffic_index_2].position * 4.f;
        traffic_pos_2.y = 0;

        if (length(player_pos - traffic_pos_1) <= 1.5 * colision_radius ||
            length(player_pos - traffic_pos_2) <= 1.5 * colision_radius)
        {
            acceleration = 0;
        }
        else 
        {
            if (window->KeyHold(GLFW_KEY_W))
            {
                glm::vec3 position = cameraPlayer->GetNextPosOfTarget(deltaTime * acceleration + 1);
                if (InsideTrack(position)) {
                    acceleration = std::min((double)30, acceleration + 0.5);
                    cameraPlayer->MoveForward(deltaTime * acceleration + 0.1);
                    glm::vec3 pos = cameraPlayer->GetTargetPosition();
                    cameraMiniMap->Set(pos + glm::vec3(0, 30, 0), pos, glm::vec3(0, 0, 1));
                }
                else acceleration = 0;
            }
            if (window->KeyHold(GLFW_KEY_A))
            {
                cameraPlayer->RotateThirdPerson_OY(sensivityOY * deltaTime);
                angle += sensivityOY * deltaTime;
            }
            if (window->KeyHold(GLFW_KEY_S))
            {
                acceleration = std::max((double)-25, acceleration - 0.5);
                glm::vec3 position = cameraPlayer->GetNextPosOfTarget(-deltaTime * acceleration - 1);
                if (InsideTrack(position)) {
                    cameraPlayer->MoveForward(deltaTime * acceleration - 0.1);
                    glm::vec3 pos = cameraPlayer->GetTargetPosition();
                    cameraMiniMap->Set(pos + glm::vec3(0, 30, 0), pos, glm::vec3(0, 0, 1));
                }
                else acceleration = 0;
            }
            else if (!window->KeyHold(GLFW_KEY_W)) {
                acceleration = max((double)0, acceleration - 0.1);
                if (acceleration > 0) {
                    glm::vec3 position = cameraPlayer->GetNextPosOfTarget(deltaTime * acceleration);
                    if (InsideTrack(position)) {
                        cameraPlayer->MoveForward(deltaTime * acceleration);
                        glm::vec3 pos = cameraPlayer->GetTargetPosition();
                        cameraMiniMap->Set(pos + glm::vec3(0, 30, 0), pos, glm::vec3(0, 0, 1));
                    }
                    else acceleration = 0;
                }
            }
            if (window->KeyHold(GLFW_KEY_D))
            {
                cameraPlayer->RotateThirdPerson_OY(sensivityOY * -deltaTime);
                angle -= sensivityOY * deltaTime;
            }

            if (window->KeyHold(GLFW_KEY_W))
            {
                bottom -= deltaTime;
                top += deltaTime;
            }
            if (window->KeyHold(GLFW_KEY_S))
            {
                bottom += deltaTime;
                top -= deltaTime;
            }
            if (window->KeyHold(GLFW_KEY_A))
            {
                left -= deltaTime;
                right += deltaTime;
            }
            if (window->KeyHold(GLFW_KEY_D))
            {
                left += deltaTime;
                right -= deltaTime;
            }
        }
}


void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}