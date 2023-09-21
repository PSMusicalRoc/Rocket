#ifndef _ROC_SPRITE_COMPONENT_H_
#define _ROC_SPRITE_COMPONENT_H_

#include <string>

#include <ECS/Component.h>

#include <glad/gl.h>

#include "Roc_GL/Shader.hpp"
#include "Roc_GL/Texture.hpp"
#include "Roc_GL/CoordinateSystem.hpp"

struct Sprite : public Component
{
    friend class RenderSpriteSystem;

    std::string _tex_key = "";
    std::string _shader_key = "";

    double offsetX = 0, offsetY = 0;
    double width = 0.0, height = 0.0;

    void SetOutputData(double thisx, double thisy);

    void DestroyComponent() override;

private:
    GLuint VBO;
    GLdouble vertices[30] = {
            /// positions   /// texture positions
            0, 0, 0.0f,     1.0f, 0.0f, // top right
            0, 0, 0.0f,     0.0f, 0.0f, // top left
            0, 0, 0.0f,     0.0f, 1.0f, // bottom left
            0, 0, 0.0f,     1.0f, 0.0f, // top right
            0, 0, 0.0f,     0.0f, 1.0f, // bottom left
            0, 0, 0.0f,     1.0f, 1.0f  // bottom right
        };

};

#endif