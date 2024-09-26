#pragma once
#include "Libs.h"
namespace Prim
{
    vector<Vertex> cubeVertices = {
        // Positions                            // Normal                     // Texture Coords
        // Back face
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)), // Bottom-left
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)), // top-right
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f)), // bottom-right         
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)), // top-right
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)), // bottom-left
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f)), // top-left
        // Front face
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f)), // bottom-left
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f)), // bottom-right
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f)), // top-right
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f)), // top-right
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f)), // top-left
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f)), // bottom-left
        // Left face
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)), // top-right
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)), // top-left
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)), // bottom-left
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)), // bottom-left
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)), // bottom-right
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)), // top-right
        // Right face
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)), // top-left
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)), // bottom-right
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)), // top-right         
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)), // bottom-right
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)), // top-left
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)), // bottom-left     
        // Bottom face
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)), // top-right
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)), // top-left
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)), // bottom-left
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)), // bottom-left
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)), // bottom-right
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)), // top-right
        // Top face
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)), // top-left
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)), // bottom-right
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f)), // top-right     
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)), // bottom-right
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)), // top-left
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f))  // bottom-left
    };

    vector<Vertex> planeVertices = {
        // Positions            // Texture Coords (note we set these higher than 1 that together with GL_REPEAT (as texture wrapping mode) will cause the floor texture to repeat)
        Vertex(glm::vec3( 5.0f, -0.5f,  5.0f),  glm::vec2(6.0f, 0.0f)),
        Vertex(glm::vec3(-5.0f, -0.5f, -5.0f),  glm::vec2(0.0f, 6.0f)),
        Vertex(glm::vec3(-5.0f, -0.5f,  5.0f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3( 5.0f, -0.5f,  5.0f),  glm::vec2(6.0f, 0.0f)),
        Vertex(glm::vec3( 5.0f, -0.5f, -5.0f),  glm::vec2(6.0f, 6.0f)),
        Vertex(glm::vec3(-5.0f, -0.5f, -5.0f),  glm::vec2(0.0f, 6.0f))
    };

    vector<Vertex> transparentVertices = {
        // Positions         // Texture Coords (swapped y coordinates because texture is flipped upside down)
        Vertex(glm::vec3(0.0f,  0.5f,  0.0f),  glm::vec2(1.0f,  1.0f)),
        Vertex(glm::vec3(0.0f, -0.5f,  0.0f),  glm::vec2(1.0f,  0.0f)),
        Vertex(glm::vec3(1.0f, -0.5f,  0.0f),  glm::vec2(0.0f,  0.0f)),

        Vertex(glm::vec3(0.0f,  0.5f,  0.0f),  glm::vec2(1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f, -0.5f,  0.0f),  glm::vec2(0.0f,  0.0f)),
        Vertex(glm::vec3(1.0f,  0.5f,  0.0f),  glm::vec2(0.0f,  1.0f))
    };

    vector<Vertex> quadVertices =
    {
        Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(1.0f, 0.0f)),

        Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(1.0f, 1.0f))
    };

    vector<Vertex> smallQuadVertices =
    {
        Vertex(glm::vec3(-0.3f,  1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.3f,  0.7f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.3f,  0.7f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(1.0f, 0.0f)),

        Vertex(glm::vec3(-0.3f,  1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.3f,  0.7f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.3f,  1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f),  glm::vec2(1.0f, 1.0f))
    };

    vector<Vertex> skyboxVertices =
    {
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f, -1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f,  1.0f),  glm::vec2(0.0f, 0.0f))
    };

    vector<Vertex> pointsVertices =
    {
        Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)), // 左上方
        Vertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f)),  // 右上方
        Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f)),  // 右下方
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3( 1.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f)) // 左下方
    };

    vector<Vertex> smallScreenVertices =
    {
        Vertex(glm::vec3(-0.05f,  0.05f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-0.05f,  0.05f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.05f, -0.05f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.05f,  0.05f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f))
    };

    vector<Vertex> floorVertices =
    {
        // Positions                           // Normals                   // Texture Coords
        Vertex(glm::vec3( 8.0f, -0.5f,  8.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(5.0f, 0.0f)),
        Vertex(glm::vec3(-8.0f, -0.5f,  8.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-8.0f, -0.5f, -8.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 5.0f)),

        Vertex(glm::vec3( 8.0f, -0.5f,  8.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(5.0f, 0.0f)),
        Vertex(glm::vec3(-8.0f, -0.5f, -8.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 5.0f)),
        Vertex(glm::vec3( 8.0f, -0.5f, -8.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(5.0f, 5.0f))
    };

    vector<Vertex> hugePlaneVertices =
    {
         // Positions                             // Normals                   // Texture Coords
         Vertex(glm::vec3( 25.0f, -0.5f,  25.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(25.0f,  0.0f)),
         Vertex(glm::vec3(-25.0f, -0.5f, -25.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2( 0.0f, 25.0f)),
         Vertex(glm::vec3(-25.0f, -0.5f,  25.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2( 0.0f,  0.0f)),

         Vertex(glm::vec3( 25.0f, -0.5f,  25.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(25.0f,  0.0f)),
         Vertex(glm::vec3( 25.0f, -0.5f, -25.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(25.0f, 25.0f)),
         Vertex(glm::vec3(-25.0f, -0.5f, -25.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2( 0.0f, 25.0f))
    };
}
