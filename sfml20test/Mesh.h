#ifndef MESHH
#define MESHH

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
	struct Index {
		int vertexIndex, normalIndex, texIndex;
	};

	class Mesh {
		GLuint vao, vbuf, nbuf, tbuf;
		
		float *vertexData;
		float *normalData;
		float *texData;

		int rnum;

		std::vector<glm::vec3>	vertices;
		std::vector<glm::vec3>	normals;
		std::vector<glm::vec2>	textices;
		std::vector<Index>		indices;
	public:
		Mesh(){};
		Mesh(char *);
		int loadFromFile(char *);
		void Render();
		void test();
		~Mesh();
	};
};

#endif