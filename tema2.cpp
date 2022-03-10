#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/tema2Transform3D.h"

#include <vector>
#include <string>
#include <iostream>


using namespace std;

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


void Tema2::Init()
{
    renderCameraTarget = false;

    srand(time(0));
    std::vector<int> row(width);
    for (int i = 0; i < height; ++i)
    {
        map.push_back(row);
    }


    Tema2::Maze(map);

    posYplayer = 0;

    bool ok = false;
    int x, z;

    while (!ok) {
        x = rand() % 5 + 10;
        z = -(rand() % 5 + 10);

        if (map[x][-z] == 0) {
            posXplayer = x;
            posZplayer = z;
            ok = true;
        }
    }

    camera = new implemented::Cam();
    camera->Set(glm::vec3(posXplayer, 1.5f, posZplayer + 1.5f), glm::vec3(posXplayer, 1.5f, posZplayer), glm::vec3(0, 1.5f, 0));
    camera->distanceToTarget = 1.5f;
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("BodyArm");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderBodyArm.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderBodyArm.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Legs");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderLegs.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderLegs.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Hand");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderHand.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderHand.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Halo");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderHalo.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderHalo.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Hair");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderHair.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderHair.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Map");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderMap.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderMap.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("MapFloor");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderMapFloor.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderMapFloor.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Projectile");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderProjectile.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShaderProjectile.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    posX_pr = posXplayer;
    posZ_pr = posZplayer;
    angle_pr = M_PI_2;
    distanceMovingPr = 2.5f;
    projectileMoving = false;
    prSpeed = 0;
    prFireRate = 0;

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

void Tema2::Maze(std::vector<std::vector<int>>& map) 
{

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            map[i][j] = 1;
        }
    }
    Tema2::MazeR(map, 0, 0);
}


void Tema2::MazeR(std::vector<std::vector<int>>& map, int i, int j) 
{
    int direct[][2] = { {0,1}, {0,-1}, {-1,0}, {1,0} };
    int directCount[][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    int visitOrder[] = { 0,1,2,3 };
    int count = 0;

    if (i < 0 || j < 0 || i >= map.size() || j >= map[0].size()) return;

    if (map[i][j] == 0) return;

    for (int k = 0; k < 4; ++k) {
        int ni = i + directCount[k][0];
        int nj = j + directCount[k][1];

        if (ni < 0 || nj < 0 || ni >= map.size() || nj >= map[0].size()) continue;
        if (map[ni][nj] == 0) count++;
    }

    if (count > 1) return;

    map[i][j] = 0;

    for (int i = 0; i < 4; ++i) {
        Tema2::Swap(visitOrder[i], visitOrder[rand() % 4]);
    }

    for (int k = 0; k < 4; ++k) {
        int ni = i + direct[visitOrder[k]][0];
        int nj = j + direct[visitOrder[k]][1];

        Tema2::MazeR(map, ni, nj);
    }
}

void Tema2::Swap(int& a, int& b) 
{
    int c = a;
    a = b;
    b = c;
}

void Tema2::Update(float deltaTimeSeconds)
{

    if (projectileMoving) {
        { // projectile
            modelMatrix = glm::mat4(1);
            modelMatrix *= tema2Transform3D::Translate(posX_pr, posYplayer + heightLegPlayer + heightBodyPlayer + heightHeadPlayer / 2, posZ_pr);
            modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
            modelMatrix *= tema2Transform3D::Scale(widthHeadPlayer / 4, heightHeadPlayer / 4, 0.15f / 4);
            RenderMesh(meshes["box"], shaders["Projectile"], modelMatrix);
        }
    }

    { // leg1
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= tema2Transform3D::Translate(widthLegPlayer / 1.5f, heightLegPlayer / 2, 0);
        modelMatrix *= tema2Transform3D::Scale(widthLegPlayer, heightLegPlayer, widthLegPlayer);
        RenderMesh(meshes["box"], shaders["Legs"], modelMatrix);
    }

    { // leg2
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= tema2Transform3D::Translate(- widthLegPlayer / 1.5f, heightLegPlayer / 2, 0);
        modelMatrix *= tema2Transform3D::Scale(widthLegPlayer, heightLegPlayer, widthLegPlayer);
        RenderMesh(meshes["box"], shaders["Legs"], modelMatrix);
    }

    { // body
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer + heightLegPlayer + heightBodyPlayer / 2, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= tema2Transform3D::Scale(widthBodyPlayer, heightBodyPlayer, widthLegPlayer);
        RenderMesh(meshes["box"], shaders["BodyArm"], modelMatrix);
    }

    { // arm1
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= tema2Transform3D::Translate( widthBodyPlayer / 2 + widthArmPlayer / 2, heightLegPlayer + heightBodyPlayer - heightArmPlayer / 2, 0);
        modelMatrix *= tema2Transform3D::Scale(widthArmPlayer, heightArmPlayer, widthArmPlayer);
        RenderMesh(meshes["box"], shaders["BodyArm"], modelMatrix);
    }

    { // arm2
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= tema2Transform3D::Translate( -widthBodyPlayer / 2 - widthArmPlayer / 2, heightLegPlayer + heightBodyPlayer - heightArmPlayer / 2, 0);
        modelMatrix *= tema2Transform3D::Scale(widthArmPlayer, heightArmPlayer, widthArmPlayer);
        RenderMesh(meshes["box"], shaders["BodyArm"], modelMatrix);
    }

    { // hand1
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= tema2Transform3D::Translate(widthBodyPlayer / 2 + widthHandPlayer / 2, heightLegPlayer + heightBodyPlayer - heightArmPlayer - heightHandPlayer / 2, 0);
        modelMatrix *= tema2Transform3D::Scale(widthHandPlayer, heightHandPlayer, widthHandPlayer);
        RenderMesh(meshes["box"], shaders["Hand"], modelMatrix);
    }
    { // hand2
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= tema2Transform3D::Translate(- widthBodyPlayer / 2 - widthHandPlayer / 2, heightLegPlayer + heightBodyPlayer - heightArmPlayer - heightHandPlayer / 2, 0);
        modelMatrix *= tema2Transform3D::Scale(widthHandPlayer, heightHandPlayer, widthHandPlayer);
        RenderMesh(meshes["box"], shaders["Hand"], modelMatrix);
    }

    { // head
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer + heightLegPlayer + heightBodyPlayer + heightHeadPlayer / 2, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);
        modelMatrix *= tema2Transform3D::Scale(widthHeadPlayer, heightHeadPlayer, 0.15f);
        RenderMesh(meshes["box"], shaders["Hair"], modelMatrix);
    }

    { // halo on head
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(posXplayer, posYplayer + heightLegPlayer + heightBodyPlayer + heightHeadPlayer + heightHaloPlayer / 2 + 0.05f, posZplayer);
        modelMatrix *= tema2Transform3D::RotateOY(angle - M_PI_2);

        modelMatrix *= tema2Transform3D::Scale(widthHaloPlayer, heightHaloPlayer, 0.15f);
        RenderMesh(meshes["box"], shaders["Halo"], modelMatrix);
    }

    { // left Wall
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(0, 0, -13 );
        modelMatrix *= tema2Transform3D::Scale(1, 2, -25);
        RenderMesh(meshes["box"], shaders["Map"], modelMatrix);
    }

    { // right Wall
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2Transform3D::Translate(25, 0, -13);
        modelMatrix *= tema2Transform3D::Scale(1, 2, -25);
        RenderMesh(meshes["box"], shaders["Map"], modelMatrix);
    }


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {

            {
                modelMatrix = glm::mat4(1);
                modelMatrix *= tema2Transform3D::Translate(i, 0, -j);
                modelMatrix *= tema2Transform3D::Scale(1, 0.1f, 1);
                RenderMesh(meshes["box"], shaders["MapFloor"], modelMatrix);
            }

            if (map[i][j] == 1) {
                {
                    modelMatrix = glm::mat4(1);
                    modelMatrix *= tema2Transform3D::Translate(i , 0, -j);
                    modelMatrix *= tema2Transform3D::Scale(1, 2, 1);
                    RenderMesh(meshes["box"], shaders["Map"], modelMatrix);
                }
            }
        }
    }
}

void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Get shader location for uniform mat4 "Model"
    int location1 = glGetUniformLocation(shader->program, "Model");

    // Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Get shader location for uniform mat4 "View"
    int location2 = glGetUniformLocation(shader->program, "View");

    // Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(location2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Get shader location for uniform mat4 "Projection"
    int location3 = glGetUniformLocation(shader->program, "Projection");

    // Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;

    if (window->KeyHold(GLFW_KEY_O)) {
        angle += deltaTime;
        camera->RotateThirdPerson_OY(deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_P)) {
        angle -= deltaTime;
        camera->RotateThirdPerson_OY(-deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        camera->TranslateForward(deltaTime * cameraSpeed);
        posZplayer -= cameraSpeed * deltaTime * sin(angle);
        posXplayer += cameraSpeed * deltaTime * cos(angle);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        camera->TranslateRight(-deltaTime * cameraSpeed);
        posZplayer -= cameraSpeed * deltaTime * cos(angle);
        posXplayer -= cameraSpeed * deltaTime * sin(angle);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->TranslateForward(-deltaTime * cameraSpeed);
        posZplayer += cameraSpeed * deltaTime * sin(angle);
        posXplayer -= cameraSpeed * deltaTime * cos(angle);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        camera->TranslateRight(deltaTime * cameraSpeed);
        posZplayer += cameraSpeed * deltaTime * cos(angle);
        posXplayer += cameraSpeed * deltaTime * sin(angle);
    }


    prFireRate += 5 * deltaTime;
    
    if (prFireRate >= 0.2f && thirdPerson == false) {

        if (!projectileMoving) {
            posX_pr = posXplayer;
            posZ_pr = posZplayer;
            angle_pr = angle;
            prFireRate = 0;
        }
        else {
            posX_pr += sin(angle_pr + M_PI_2) * prSpeed * deltaTime;
            posZ_pr += cos(angle_pr + M_PI_2) * prSpeed * deltaTime;

            distPlayerProjectil = sqrt((posX_pr - posXplayer) * (posX_pr - posXplayer) + (posZ_pr - posZplayer) * (posZ_pr - posZplayer));

            if (distPlayerProjectil > distanceMovingPr) {
                posX_pr = posXplayer;
                posZ_pr = posZplayer;
                angle_pr = angle;
                projectileMoving = false;
                prSpeed = 0;
                prFireRate = 0;
            }
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T) {
        renderCameraTarget = !renderCameraTarget;
    }
    
    if (key == GLFW_KEY_LEFT_CONTROL) {
        
        // change in first person
        if (thirdPerson) {
            glm::vec3 newPos = camera->position;
            newPos[0] = newPos[0] - distCam * sin(angle - M_PI_2);
            newPos[1] = heightPosEyes;
            newPos[2] = newPos[2] - distCam * cos(angle - M_PI_2);

            camera->distanceToTarget = - 0.001f - widthHeadPlayer / 2 ;
            glm::vec3 centerPos = newPos;
            centerPos[0] = centerPos[0] - 0.001f * sin(angle - M_PI_2);
            centerPos[2] = centerPos[2] - 0.001f * cos(angle - M_PI_2);
            camera->Set(newPos, centerPos, glm::vec3(0, 1, 0));
            thirdPerson = false;

        }

        // change in third person
        else {
            glm::vec3 newPos = camera->position;
            newPos[0] = newPos[0] + distCam * sin(angle - M_PI_2);
            newPos[1] = 1.5f;
            newPos[2] = newPos[2] + distCam * cos(angle - M_PI_2);

            glm::vec3 centerPos = newPos;
            centerPos[0] = centerPos[0] - 0.001f * sin(angle - M_PI_2);
            centerPos[2] = centerPos[2] - 0.001f * cos(angle - M_PI_2);
            camera->distanceToTarget = 1.5f;
            camera->Set(newPos, centerPos, glm::vec3(0, 1, 0));
            thirdPerson = true;
        }
    }
}

// for debug
void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOX * deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOX * deltaX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (thirdPerson == false) {
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
            prSpeed += 5;
            prFireRate = 0;
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (thirdPerson == false) {
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
            projectileMoving = true;
        }
    }
}


/**
* Pentru labirint m-am folosit de algoritmul gasit aici https://www.bo-song.com/how-to-generate-a-maze-with-c/
**/