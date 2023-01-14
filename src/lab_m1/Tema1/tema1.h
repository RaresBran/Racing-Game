#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill);

        Mesh* CreateCircle(const std::string& name, int radius, glm::vec3 color);

        Mesh* CreateTriangle(const std::string& name, glm::vec3 color);

        void Init() override;

    private:
        void FrameStart() override;
        void RenderDuck(float bodyX, float bodyY);
        void RenderGUI();
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float bodyAngle;
        float wingRotation;
        float bodyX, bodyY;
        int count;
        glm::mat3 bodyMatrix;
        float timeElapsed;
        int round;
        int lives;
        int bullets;
        float deltaTime;
        bool birdDED;
        int score;
        bool birdEscaping;
        float speed_multiplier;

        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
