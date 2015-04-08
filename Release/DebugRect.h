#ifndef DEBUGRECTH
#define DEBUGRECTH

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
#include "Texture.h"
#include "ShaderManager.h"

namespace agl {
	class DebugRect {
		static GLuint vbuf, tbuf, vao, sample;
		static GLint uDepthMap;

		static float *vdata;
		static float *tdata;

		static agl::ShaderProgram sp;
		static agl::Texture tx;

	public:
		static void initShader();
		static void render();
		static void destroy();
	};
}

#endif