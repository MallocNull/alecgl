#define SFML_STATIC
#define GLEW_STATIC
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "AlecGL.h"
#define PI 3.14159

float degToRad(float deg) {return deg*(PI/180);}
float radToDeg(float rad) {return rad*(180/PI);}

int main() {
	sf::RenderWindow window(sf::VideoMode(400, 400), "opengl or something", sf::Style::Default, sf::ContextSettings(32));
	window.setVerticalSyncEnabled(true);

	HWND__ *wnd = window.getSystemHandle();
	ShowWindow(wnd, SW_MAXIMIZE);

	if(glewInit() != GLEW_OK) return -1;

	agl::ShaderProgram shader;
	int err = shader.initProgram(2,	"vertex.vert"	, GL_VERTEX_SHADER,
									"fragment.frag"	, GL_FRAGMENT_SHADER);

	if(err == -1) {
		printf("%s\n", shader.getLastError());
		getchar();
		return -1;
	}

	GLuint	sampid			= 0;

	GLint	modelToCam		= shader.getVariable("modelToCam"),
			camToWorld		= shader.getVariable("camToWorld"),
			worldToClip		= shader.getVariable("worldToClip"),
			utime			= shader.getVariable("time"),
			texcol			= shader.getVariable("texcol"),
			lPos			= shader.getVariable("lightPos"),
			aInt			= shader.getVariable("ambIntensity"),
			lInt			= shader.getVariable("lightIntensity"),
			tScale			= shader.getVariable("texScaleFactor"),
			tOff			= shader.getVariable("texOffsetFactor"),
			flPos			= shader.getVariable("flashLightPos"),
			flDir			= shader.getVariable("flashLightDir"),
			flInt			= shader.getVariable("flashLightIntensity"),
			enFlash			= shader.getVariable("enableFlashlight"),
			loVec			= shader.getVariable("lookVector"),
			loPos			= shader.getVariable("lookPos");

	glm::mat4 frustum = glm::perspective<float>(45.0f, window.getSize().x/window.getSize().y, 0.5, 1000.0);

	sf::Music big;
	if(!big.openFromFile("weed.ogg")) return -1;
	big.setLoop(true);
	big.setVolume(0);

	agl::Mesh	test("cube.obj"),
				sphere("monkey.obj"),
				plane("plane.obj");
	
	agl::DebugRect::initShader();

	//agl::NoiseReader test("noise.png");

	agl::Texture tex(20.f, 1, "head/0000.png");
	char cbuf[255];
	for(int i = 1; i < 19; i++) {
		sprintf(cbuf, "head/00%s%i.png", ((i<10)?"0":""), i);
		tex.addImageFromFile(cbuf);
	}
	
	agl::Texture grass("grass.png");
	agl::Texture wall("wall.png");
	//agl::Texture tex("ball.png");

	std::vector<GLuint> texlist;
	texlist.push_back(grass.getTextureId());
	texlist.push_back(wall.getTextureId());

	agl::Map testmap("level.map", texlist);

	agl::Camera cam(0,0,0); 
	agl::Camera lcam(-10, 0, 3, 0,0,0);
	lcam.updateCamera();

	agl::MatrixStack stack;

	glGenSamplers(1, &sampid);
	glSamplerParameteri(sampid, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampid, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampid, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sampid, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);

	sf::Clock clk;

	glm::mat4 offset(1.0f), model(1.0f);
	glm::vec3 displacement(-0.f, -0.f, 0.f),
			  lookVector(0.0,0.0,0.0),
			  flvec(0.0),
			  soundpos(0.0);
	float rot = 180, vrot = 0;
	
	bool flashlightEnabled = true;
	bool mouseLock = true;

	shader.StartProgram();
	glUniform1i(texcol, 0);
	glUniformMatrix4fv(worldToClip, 1, GL_FALSE, &frustum[0][0]);
	glUniform4f(aInt, 0.15, 0.15, 0.15, 1.0);
	glUniform4f(lInt, 0.9, 0.9, 0.9, 1.0);
	glUniform4f(flInt, 0.9, 0.9, 0.9, 1.0);
	shader.EndProgram();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool switchCam = false;

	big.play();
    while (window.isOpen()) {
		window.setMouseCursorVisible(!mouseLock);
		cam.updateCamera();
		if(mouseLock) sf::Mouse::setPosition(sf::Vector2i(window.getSize().x/2, window.getSize().y/2));

		glClearColor(0.529,0.808,0.922,1.0);
		glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = clk.getElapsedTime().asSeconds();

		if(!switchCam) {
			model = cam.getMatrix();//glm::rotate(glm::mat4(1.0), vrot, glm::vec3(1.0, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0), rot, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(glm::mat4(1.0f), displacement);
			lookVector = cam.getLookVector();//glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0), -rot, glm::vec3(0.0, 1.0, 0.0)) * glm::rotate(glm::mat4(1.0), -vrot, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(0.0,0.0,-1.0,0.0)));
		} else {
			model = lcam.getMatrix();
			lookVector = lcam.getLookVector();
		}
		
		soundpos = glm::vec3(model * glm::vec4(0.0f,0.0f,0.0f,1.0f));
		big.setPosition(soundpos.x, soundpos.y, soundpos.z);
		//printf("%f %f %f\n", soundpos.x, soundpos.y, soundpos.z);
		
		agl::DebugRect::render();

		shader.StartProgram();
		
		glUniform3f(lPos, 5.5*cos(time), 6, 5.5*sin(3*time));
		glUniform3f(loPos, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);//-displacement.x, -displacement.y, -displacement.z);
		glUniform3f(loVec, lookVector.x, lookVector.y, lookVector.z);
		glUniform3f(flPos, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);//-displacement.x, -displacement.y, -displacement.z);
		glUniform3f(flDir, lookVector.x, lookVector.y, lookVector.z);

		glUniform1i(enFlash, flashlightEnabled);

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampid); 

		glUniform1f(utime, time);
		glUniformMatrix4fv(camToWorld, 1, GL_FALSE, &model[0][0]);
		
		//glBindTexture(GL_TEXTURE_2D, carpetid);
		
		glUniform2f(tScale, 1.0, 1.0);
		glUniform2f(tOff, 0.0, 0.0);

		grass.bindTexture();
		
		glUniformMatrix4fv(modelToCam, 1, GL_FALSE, &glm::mat4(1.0)[0][0]);
		testmap.render();

		tex.bindTexture();
		
		stack.push();
		
			stack.rotatex(time*50);
			stack.scale(.5, .5, .5);
			stack.translate(-10, 0, 0);
			
			glUniformMatrix4fv(modelToCam, 1, GL_FALSE, &stack.top()[0][0]);
			sphere.Render();

		stack.pop();
		
		stack.push();
			
			printf("%f, %f\n", cam.getRotation().x, cam.getRotation().y);
			stack.rotatex(cam.getRotation().y);
			stack.rotatey(cam.getRotation().x);
			stack.scale(.1, .1, .1);
			stack.translate(cam.getPosition());
			
			glUniformMatrix4fv(modelToCam, 1, GL_FALSE, &stack.top()[0][0]);
			sphere.Render();

		stack.pop();

		/*offset = glm::translate(glm::mat4(1.0f), glm::vec3(3, 1, 0)); //glm::translate(glm::mat4(1.0f), glm::vec3(4*cos(time), 4.0, 4*sin(time))) * glm::rotate(glm::mat4(1.0), time*60, glm::vec3(0.0, 1.0, 1.0)) * glm::scale(glm::mat4(1.0f), glm::vec3(.5f, .5f, .5f));
		glUniformMatrix4fv(modelToCam, 1, GL_FALSE, &offset[0][0]);
		sphere.Render();*/

		tex.unbindTexture();
		glBindSampler(0, 0);

		shader.EndProgram();

        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
			else if(event.type == sf::Event::KeyPressed) {
				if(event.key.code == sf::Keyboard::Z)
					flashlightEnabled = !flashlightEnabled;
				if(event.key.code == sf::Keyboard::X)
					switchCam = !switchCam;
				if(event.key.code == sf::Keyboard::Escape)
					mouseLock = !mouseLock;
			} else if(event.type == sf::Event::LostFocus)
				mouseLock = false;
			else if(event.type == sf::Event::GainedFocus)
				mouseLock = true;
			else if(event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
				frustum = glm::perspective<float>(50.0f, float(event.size.width)/float(event.size.height), 0.5, 1000.0);
				shader.StartProgram();
				glUniformMatrix4fv(worldToClip, 1, GL_FALSE, &frustum[0][0]);
				shader.EndProgram();
			}
        }

		float step = .15f;
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) step = .5f;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cam.move(step);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cam.move(-step);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cam.strafe(step);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cam.strafe(-step);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) cam.moveCamera(0, step, 0);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) cam.moveCamera(0, -step, 0);
		
		if(mouseLock) {
			float dx = (float(sf::Mouse::getPosition().x) - float(window.getSize().x/2))/5.f;
			float dy = (float(sf::Mouse::getPosition().y) - float(window.getSize().y/2))/5.f;
			if(dx != 0 || dy != 0) {
				cam.rotateCamera(-dx, -dy, 0);
			}
		}

		/*if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) rot -= 1.f; old mouse controls
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) rot += 1.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) vrot -= 1.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) vrot += 1.f;*/
    }

    return 0;
}