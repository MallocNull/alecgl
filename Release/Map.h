#ifndef MAPH
#define MAPH

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace agl {
	struct floor {
		glm::vec3 v1;
		glm::vec3 v2;
		GLuint texture;
	};

	struct wall {
		glm::vec3 v1;
		glm::vec3 v2;
		GLuint texture;
	};

	class Map {
		std::vector<floor> floors;
		std::vector<wall> walls;
		std::vector<GLuint> texs;
		
		std::vector<std::vector<float>> vertices;
		std::vector<std::vector<float>> normals;
		std::vector<std::vector<float>> textices;

		std::vector<GLuint> vaos;

		std::vector<std::string> explode(std::string, std::string);
	public:
		Map();
		Map(char*, std::vector<GLuint>);

		int loadFromFile(char*, std::vector<GLuint>);

		void render();

		~Map();
	};
};

#endif