#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace tema2Object3D
{
    Mesh* CreateRectangle(const std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill);
    Mesh* CreateCircle(std::string name, float radius, float nrOfTr, glm::vec3 color);
}