#include "DebugRect.h"

GLuint agl::DebugRect::vbuf;
GLuint agl::DebugRect::tbuf;
GLuint agl::DebugRect::vao;
GLuint agl::DebugRect::sample;
GLint agl::DebugRect::uDepthMap;
float *agl::DebugRect::vdata;
float *agl::DebugRect::tdata;
agl::ShaderProgram agl::DebugRect::sp;

void agl::DebugRect::initShader() {
	agl::DebugRect::vdata = new float[12];
	agl::DebugRect::vdata[0] = 0.5;
	agl::DebugRect::vdata[1] = 0.5;
	agl::DebugRect::vdata[2] = 1.0;
	agl::DebugRect::vdata[3] = 0.5;
	agl::DebugRect::vdata[4] = 1.0;
	agl::DebugRect::vdata[5] = 1.0;
	agl::DebugRect::vdata[6] = 1.0;
	agl::DebugRect::vdata[7] = 1.0;
	agl::DebugRect::vdata[8] = 0.5;
	agl::DebugRect::vdata[9] = 1.0;
	agl::DebugRect::vdata[10] = 0.5;
	agl::DebugRect::vdata[11] = 0.5;

	agl::DebugRect::tdata = new float[12];
	agl::DebugRect::tdata[0] = 0.0;
	agl::DebugRect::tdata[1] = 0.0;
	agl::DebugRect::tdata[2] = 1.0;
	agl::DebugRect::tdata[3] = 0.0;
	agl::DebugRect::tdata[4] = 1.0;
	agl::DebugRect::tdata[5] = 1.0;
	agl::DebugRect::tdata[6] = 1.0;
	agl::DebugRect::tdata[7] = 1.0;
	agl::DebugRect::tdata[8] = 0.0;
	agl::DebugRect::tdata[9] = 1.0;
	agl::DebugRect::tdata[10] = 0.0;
	agl::DebugRect::tdata[11] = 0.0;

	agl::DebugRect::sp = agl::ShaderProgram();
	int err = agl::DebugRect::sp.initProgram(2, "dvert.vert", GL_VERTEX_SHADER,
												"dfrag.frag", GL_FRAGMENT_SHADER);

	if(err == -1) {
		printf("%s\n", agl::DebugRect::sp.getLastError());
		getchar();
		return;
	}
	
	uDepthMap = sp.getVariable("depthMap");
	glGenSamplers(1, &sample);
	glSamplerParameteri(sample, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sample, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sample, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sample, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenBuffers(1, &agl::DebugRect::vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, agl::DebugRect::vbuf);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), agl::DebugRect::vdata, GL_STATIC_DRAW);
	
	glGenBuffers(1, &agl::DebugRect::tbuf);
	glBindBuffer(GL_ARRAY_BUFFER, agl::DebugRect::tbuf);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), agl::DebugRect::tdata, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &agl::DebugRect::vao);
	glBindVertexArray(agl::DebugRect::vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, agl::DebugRect::vbuf);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, agl::DebugRect::tbuf);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);
}

void agl::DebugRect::render() {
	agl::DebugRect::sp.StartProgram();
	
	glActiveTexture(GL_TEXTURE0);
	glBindSampler(0, sample); 

	glBindVertexArray(agl::DebugRect::vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	agl::DebugRect::sp.EndProgram();
}