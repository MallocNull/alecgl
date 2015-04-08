#include "ShaderManager.h"

int agl::ShaderProgram::initProgram(int count, ...) {
	char  **programData	= new char*[count];
	GLuint *shaderType	= new GLuint[count],
		   *shader		= new GLuint[count];
	
	program				= glCreateProgram();
	
	GLint	status,
			length		= 0;

	va_list args;
	va_start(args, count);
	for(int i = 0; i < count; i++) {
		programData[i] = readFile(va_arg(args,char*));
		shaderType[i] = va_arg(args,GLenum);
		
		shader[i] = glCreateShader(shaderType[i]);

		glShaderSource(shader[i], 1, (const GLchar **)&programData[i], NULL);
		glCompileShader(shader[i]);
		
		glGetShaderiv(shader[i], GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			glGetShaderiv(shader[i], GL_INFO_LOG_LENGTH, &length);
			err = new char[length+128];
			char *msg = new char[length];
			glGetShaderInfoLog(shader[i], length, NULL, msg);
			sprintf(err, "Error in %s Shader\n\n%s",	((shaderType[i]==GL_VERTEX_SHADER)?"Vertex":
											 (shaderType[i]==GL_FRAGMENT_SHADER)?"Fragment":
											 (shaderType[i]==GL_GEOMETRY_SHADER)?"Geometry":"Unknown"),
											msg);
			return -1;
		}

		glAttachShader(program, shader[i]);
	}
	va_end(args);

	glLinkProgram(program);
	
	for(int i = 0; i < count; i++)
		glDeleteShader(shader[i]);

	return 0;
}

void agl::ShaderProgram::StartProgram() {
	glUseProgram(program);
}

void agl::ShaderProgram::EndProgram() {
	glUseProgram(0);
}

char* agl::ShaderProgram::getLastError() {
	return err;
}

GLuint agl::ShaderProgram::getProgram() {
	return program;
}

char* agl::ShaderProgram::readFile(char *filename) {
	FILE *fp;
	if((fp = fopen(filename, "rb")) == NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	rewind(fp);

	char *content = (char*)new char[len+1];
	fread(content, len, 1, fp);
	content[len] = 0;

	fclose(fp);

	return content;
}

GLint agl::ShaderProgram::getVariable(char *varname) {
	return glGetUniformLocation(program, varname);
}

agl::ShaderProgram::~ShaderProgram() {
	glDeleteProgram(program);
}