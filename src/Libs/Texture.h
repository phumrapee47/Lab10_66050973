#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
public:
    Texture();
    Texture(const char* fileLoc);
    
    bool LoadTexture();
    bool LoadTextureA(); // สำหรับภาพที่มี alpha channel
    
    void UseTexture();
    void ClearTexture();
    
    ~Texture();

private:
    GLuint textureID;
    int width, height, bitDepth;
    
    const char* fileLocation;
};