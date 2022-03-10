#include "tema2Object3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* tema2Object3D::CreateRectangle(const std::string name, glm::vec3 corner, float width, float height, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-width / 2, -height / 2, 0), color),
        VertexFormat(corner + glm::vec3(width / 2, -height / 2, 0), color),
        VertexFormat(corner + glm::vec3(width / 2, height / 2, 0), color),
        VertexFormat(corner + glm::vec3(-width / 2, height / 2, 0), color)
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


Mesh* tema2Object3D::CreateCircle(std::string name, float radius, float nrOfTr, glm::vec3 color)
{
    float doublePI = 2.0f * 3.2f;
    int i;
    std::vector<VertexFormat> vertices;
    std::vector<GLuint> indices;

    for (i = 0; i < nrOfTr; i++)
    {
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(i * doublePI / nrOfTr), radius * sin(i * doublePI / nrOfTr), 0), color));

        indices.push_back(i);
        indices.push_back(0);
        indices.push_back(i + 1);
    }

    Mesh* circle = new Mesh(name);
    
    circle->SetDrawMode(GL_TRIANGLES);
    circle->InitFromData(vertices, indices);

    return circle;
}
