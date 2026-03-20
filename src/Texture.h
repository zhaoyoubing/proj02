#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>


class Texture
{
public:
    GLuint id;
    int width, height;
    std::string type;

	Texture(std::string path);
	Texture(std::string path, int w, int h);
	~Texture();
	void bindTexture();

	void generateTexture(std::string path);

	
};

#endif