#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/tema2Camera.h"



#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stack>
//#define N 10 // map size

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

        //void BacktrackingGeneratorMap();
        

        ////

        void Maze(std::vector<std::vector<int>>& map);
        //void showMaze(std::vector<std::vector<int>>& map);
        void MazeR(std::vector<std::vector<int>>& map, int i, int j);
        /*int countVisitedNeighbor(std::vector<std::vector<int>>& map, int i, int j);
        void shuffle(int a[], int n);*/
        void Swap(int& a, int& b);

        ////



    protected:
        implemented::Cam* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix;
        bool renderCameraTarget;
        //float mapMatrix[N][N];

        float left, right, top, bottom, zFar, zNear, fov;
        bool projectionPerspective;

        float posXplayer;
        float posYplayer;
        float posZplayer;

        float widthLegPlayer = 0.15f;
        float heightLegPlayer = 0.4f;

        float widthBodyPlayer = 0.35f;
        float heightBodyPlayer = 0.6f;

        float widthArmPlayer = 0.1f;
        float heightArmPlayer = 0.4f;

        float widthHandPlayer = 0.1f;
        float heightHandPlayer = 0.1f;

        float widthHeadPlayer = 0.25f;
        float heightHeadPlayer = 0.25f;

        float widthHaloPlayer = 0.3f;
        float heightHaloPlayer = 0.01f;

        float widthHairRightLeftPlayer = 0.05f;
        float heightHairRightLeftPlayer = 0.2f;

        float heightPosEyes = heightLegPlayer + heightBodyPlayer + heightHeadPlayer / 2;

        bool thirdPerson = true;
        float angle = M_PI_2;


        float posX_pr;
        float posZ_pr;
        float distanceMovingPr;
        float projectileMoving;
        float angle_pr;
        float prSpeed;
        float prFireRate;
        float distPlayerProjectil;

        float distCam = 1.5f + widthHeadPlayer / 2 + 0.001f;

        std::vector<std::vector<int>> map;
        int height = 25;
        int width = 25;
    };
}   // namespace m1
