#define SFML_STATIC
#define GLEW_STATIC
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderManager.h"
#define PI 3.14159

double degToRad(double deg) {return deg*(PI/180.f);}
double radToDeg(double rad) {return rad*(180.f/PI);}

glm::mat4 calcCameraMatrix(glm::vec3 &camTarget, glm::vec3 &camSphere);

int main() {
	sf::RenderWindow window(sf::VideoMode(200, 200), "opengl or something", sf::Style::Default, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);

	if(glewInit() != GLEW_OK) return -1;

	agl::ShaderProgram shader;
	int err = shader.initProgram(2,	"vertex.vert"	, GL_VERTEX_SHADER,
									"fragment.frag"	, GL_FRAGMENT_SHADER);

	if(err == -1) {
		printf("%s\n", shader.getLastError());
		getchar();
		return -1;
	}

	GLuint	sp				= shader.getProgram();

	GLint	camToClip		= glGetUniformLocation(sp, "camToClip"),
			worldToCam		= glGetUniformLocation(sp, "worldToCam"),
			modelToWorld	= glGetUniformLocation(sp, "modelToWorld");

	glm::mat4 frustum = glm::perspective<float>(90.0f, window.getSize().x/window.getSize().y, 1.0, 1000.0);
	
	//glm::vec3 camTarget(-38.f, -104.f, -94.f);
	glm::vec3 camTarget(0, .4f, 0);
	glm::vec3 camSphereCoords(67.5f, -46.0f, 150.0f);

	GLuint vao, vbuf, vabuf;

	const float vertexData[] = {
		 +1.0f, +1.0f, +1.0f, // 0
		 +1.0f, +1.0f, -1.0f, // 1 
		 +1.0f, -1.0f, +1.0f, // 2
		 +1.0f, -1.0f, -1.0f, // 3
		 -1.0f, +1.0f, +1.0f, // 4
		 -1.0f, +1.0f, -1.0f, // 5
		 -1.0f, -1.0f, +1.0f, // 6
		 -1.0f, -1.0f, -1.0f  // 7
		 
		 +0.0f, +0.0f, +0.0f,
		 +0.0f, +0.0f, +1.0f,
		 +0.0f, +1.0f, +0.0f,
		 +0.0f, +1.0f, +1.0f,
		 +1.0f, +0.0f, +0.0f,
		 +1.0f, +0.0f, +1.0f,
		 +1.0f, +1.0f, +0.0f,
		 +1.0f, +1.0f, +1.0f
	};

	const GLshort indexData[] = {
		// front
		4, 0, 6,
		0, 2, 6,

		// right
		0, 1, 2,
		1, 3, 2,

		// back
		1, 5, 3,
		5, 7, 3,

		// left
		5, 4, 7,
		4, 6, 7,

		// top
		5, 1, 4,
		1, 0, 4,

		// bottom
		3, 7, 2,
		7, 6, 2
	};
	
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &vabuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vabuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		size_t cpos = sizeof(float)*24; // 3*8

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)cpos);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vabuf);
	glBindVertexArray(0);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);
	
	sf::Clock clk;
	
	shader.StartProgram();
	glUniformMatrix4fv(camToClip, 1, GL_FALSE, &frustum[0][0]);
	shader.EndProgram();

    while (window.isOpen()) {
		glClearColor(0.0,0.0,0.0,0.0);
		glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.StartProgram();
		glBindVertexArray(vao);
		
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(50.f, 50.f, 50.f));
		printf("camtarget %f %f %f\n", camTarget.x, camTarget.y, camTarget.z);
		glUniformMatrix4fv(worldToCam, 1, GL_FALSE, &(calcCameraMatrix(camTarget, camSphereCoords))[0][0]);
		glUniformMatrix4fv(modelToWorld, 1, GL_FALSE, &model[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
		shader.EndProgram();

        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
			else if(event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
				frustum = glm::perspective<float>(90.0f, float(event.size.width)/float(event.size.height), 0.5, 100.0);
				shader.StartProgram();
				glUniformMatrix4fv(camToClip, 1, GL_FALSE, &frustum[0][0]);
				shader.EndProgram();
			}
        }
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) camTarget.x -= .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) camTarget.x += .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) camTarget.z -= .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) camTarget.z += .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) camTarget.y -= .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) camTarget.y += .1f;
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)) camSphereCoords.y -= .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) camSphereCoords.y += .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) camSphereCoords.x -= .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) camSphereCoords.x += .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::U)) camSphereCoords.z -= .1f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)) camSphereCoords.z += .1f;
    }

    return 0;
}

glm::mat4 calcCameraMatrix(glm::vec3 &camTarget, glm::vec3 &camSphere) {
	float phi = degToRad(camSphere.x);
	float theta = degToRad(camSphere.y + 90.f);
	float radius = camSphere.z;

	glm::vec3 dirToCam(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));
	glm::vec3 camPt = (dirToCam * radius) + camTarget;	
	printf("%f %f %f\n", camPt.x, camPt.y, camPt.z);
	
	glm::vec3 lookDir = glm::normalize(camTarget - camPt);
	glm::vec3 upDir = glm::normalize(glm::vec3(0.0f, 1.0, 0.0f));

	glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
	glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

	glm::mat4 rotMat(1.0f);
	rotMat[0] = glm::vec4(rightDir, 0.0f);
	rotMat[1] = glm::vec4(perpUpDir, 0.0f);
	rotMat[2] = glm::vec4(-lookDir, 0.0f);

	rotMat = glm::transpose(rotMat);

	glm::mat4 transMat(1.0f);
	transMat[3] = glm::vec4(-camPt, 1.0f);

	return rotMat * transMat;
}