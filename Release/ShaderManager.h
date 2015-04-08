#ifndef SHADERMANH
#define SHADERMANH

#define SFML_STATIC
#define GLEW_STATIC

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <String>
#include <map>
#include <stdarg.h>

namespace agl {
	class ShaderProgram {
		GLuint program;
		char *err;

		std::map<std::string, GLint> uniformMap;
		char* readFile(char*);
	public:
		ShaderProgram() { program = 0; };

		int initProgram(int, ...);
		
		void StartProgram();
		void EndProgram();

		GLint getVariable(char *varname);

		char* getLastError();
		GLuint getProgram();

		~ShaderProgram();
	};
}

#endif