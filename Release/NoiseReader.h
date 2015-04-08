#ifndef NOISEH
#define NOISEH

#define SFML_STATIC
#define GLEW_STATIC
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

namespace agl {
	class NoiseReader {
		GLuint vao, vbuf, nbuf, tbuf;
		
		int rnum;

		float *vertexData;
		float *normalData;
		float *texData;
		float avgPixel(sf::Color);
	public:
		NoiseReader(){};
		NoiseReader(char *);
		int loadFromFile(char *);
		void Render();
		~NoiseReader();
	};
};

#endif