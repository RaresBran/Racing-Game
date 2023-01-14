#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        Mesh* CreateParaDrept(const char* name, glm::vec3 color);
        void RenderScene();
        bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b);
        bool InsideTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P);
        bool InsideTrack(glm::vec3 P);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera* camera;
        implemented::Camera* cameraMiniMap;
        implemented::Camera* cameraPlayer;
        glm::mat4 projectionMatrix;
        glm::mat4 viewportProjectionMatrix;
        ViewportArea miniViewportArea;

        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;

        // TODO(student): If you need any other class variables, define them here.
        float time;
        glm::mat4 carMatrix;
        float angle;
        double acceleration;
        std::vector<VertexFormat> minimalTrackVertices;
        std::vector<unsigned int> minimalTrackIndices;
        std::vector<VertexFormat> treeVertices;
        std::vector<VertexFormat> trackVertices;
        std::vector<unsigned int> trackIndices;
        std::vector<VertexFormat> traffic_route_1;
        std::vector<VertexFormat> traffic_route_2;
        int traffic_index_1, traffic_index_2;
    };
}   // namespace m1