#include "Texture.h"

agl::Texture::Texture(char *file) {
	loadFromFile(file);
}

int agl::Texture::loadFromFile(char *file) {
	fps = 1;
	index = 0;
	clk = new sf::Clock();

	img.push_back(new sf::Image());
	if(img[img.size()-1]->loadFromFile(file) == false) return -1;
	img[img.size()-1]->flipVertically();
	
	texids.push_back(0);
	glGenTextures(1, &texids[texids.size()-1]);
	glBindTexture(GL_TEXTURE_2D, texids[texids.size()-1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[img.size()-1]->getSize().x, 
			img[img.size()-1]->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			img[img.size()-1]->getPixelsPtr());
		
	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

agl::Texture::Texture(float ifps, int imageCount, ...) {
	fps = ifps;
	index = 0;
	clk = new sf::Clock();

	va_list args;
	va_start(args, imageCount);
	for(int i = 0; i < imageCount; i++) {
		img.push_back(new sf::Image());
		img[img.size()-1]->loadFromFile(va_arg(args, char*));
		img[img.size()-1]->flipVertically();

		texids.push_back(0);
		glGenTextures(1, &texids[texids.size()-1]);
		glBindTexture(GL_TEXTURE_2D, texids[texids.size()-1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[img.size()-1]->getSize().x, 
				img[img.size()-1]->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				img[img.size()-1]->getPixelsPtr());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

GLuint agl::Texture::getTextureId() {
	return texids[0];
}

int agl::Texture::addImageFromFile(char *file) {
	img.push_back(new sf::Image());
	if(img[img.size()-1]->loadFromFile(file) == false) return -1;
	img[img.size()-1]->flipVertically();
	
	texids.push_back(0);
	glGenTextures(1, &texids[texids.size()-1]);
	glBindTexture(GL_TEXTURE_2D, texids[texids.size()-1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[img.size()-1]->getSize().x, 
			img[img.size()-1]->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			img[img.size()-1]->getPixelsPtr());
		
	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

int agl::Texture::loadFromFile(float ifps, int imageCount, ...) {
	fps = ifps;
	index = 0;
	clk = new sf::Clock();

	va_list args;
	va_start(args, imageCount);
	for(int i = 0; i < imageCount; i++) {
		img.push_back(new sf::Image());
		img[img.size()-1]->loadFromFile(va_arg(args, char*));
		img[img.size()-1]->flipVertically();

		texids.push_back(0);
		glGenTextures(1, &texids[texids.size()-1]);
		glBindTexture(GL_TEXTURE_2D, texids[texids.size()-1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[img.size()-1]->getSize().x, 
				img[img.size()-1]->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				img[img.size()-1]->getPixelsPtr());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void agl::Texture::bindTexture() {
	if(clk->getElapsedTime().asSeconds() >= 1.f/fps) {
		index++;
		clk->restart();
	}

	glBindTexture(GL_TEXTURE_2D, texids[index%img.size()]);
}

void agl::Texture::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

agl::Texture::~Texture() {
}