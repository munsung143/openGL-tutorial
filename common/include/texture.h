#pragma once

#include <glad/glad.h>



class Texture
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Texture(const char* imagePath, int rgb);

    // use/activate the shader
    void activeAndBind(GLenum unit);
};