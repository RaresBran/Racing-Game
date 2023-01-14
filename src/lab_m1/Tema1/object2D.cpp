#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateTriangle(
    const std::string &name,
    glm::vec3 tip,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = tip;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, length), color),
        VertexFormat(corner + glm::vec3(0, length, length), color)
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    /*
    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }
    */

    triangle->InitFromData(vertices, indices);
    return triangle;
}
