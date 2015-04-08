#include "Camera.h"

void agl::Camera::updateMatrix() {
	matrixToBeUpdated = true;
}

void agl::Camera::updateCamera() {
	if(matrixToBeUpdated) {
		camToWorld = glm::rotate(glm::mat4(1.0), -rotation.y, glm::vec3(1.0, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0), -rotation.x, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(glm::mat4(1.0f), -position);
		lookVector = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0), rotation.x, glm::vec3(0.0, 1.0, 0.0)) * glm::rotate(glm::mat4(1.0), rotation.y, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(0.0,0.0,-1.0,0.0)));	
	}

	matrixToBeUpdated = false;
}

agl::Camera::Camera() {
	rotation = glm::vec3(0.0);
	position = glm::vec3(0.0);
	updateMatrix();
}

agl::Camera::Camera(glm::vec3 pos, glm::vec3 rot) {
	setCamera(pos, rot);
}

agl::Camera::Camera(float px, float py, float pz, float rx, float ry, float rz) {
	setCamera(px, py, pz, rx, ry, rz);
}

void agl::Camera::setCamera(glm::vec3 pos, glm::vec3 rot) {
	rotation = rot;
	position = pos;
	updateMatrix();
}

void agl::Camera::setCamera(float px, float py, float pz, float rx, float ry, float rz) {
	rotation = glm::vec3(rx, ry, rz);
	position = glm::vec3(px, py, pz);
	updateMatrix();
}

void agl::Camera::setPosition(glm::vec3 pos) {
	position = pos;
	updateMatrix();
}

void agl::Camera::setPosition(float x, float y, float z) {
	position = glm::vec3(x,y,z);
	updateMatrix();
}

void agl::Camera::setRotation(glm::vec3 ang) {
	rotation = ang;
	updateMatrix();
}

void agl::Camera::setRotation(float x, float y, float z) {
	rotation = glm::vec3(x,y,z);
	updateMatrix();
}

void agl::Camera::rotateCamera(glm::vec3 ang) {
	rotation += ang;
	rotation.y = (rotation.y>90)?90:((rotation.y<-90)?-90:rotation.y);
	updateMatrix();
}

void agl::Camera::rotateCamera(float x, float y, float z) {
	rotation += glm::vec3(x,y,z);
	rotation.y = (rotation.y>90)?90:((rotation.y<-90)?-90:rotation.y);
	updateMatrix();
}

void agl::Camera::moveCamera(glm::vec3 pos) {
	position += pos;
	updateMatrix();
}

void agl::Camera::moveCamera(float x, float y, float z) {
	position += glm::vec3(x,y,z);
	updateMatrix();
}

void agl::Camera::move(float dist) {
	position -= dist*glm::vec3(sin(degToRad(rotation.x)), 0, cos(degToRad(rotation.x)));
	updateMatrix();
}

void agl::Camera::strafe(float dist) {
	position -= dist*glm::vec3(sin(degToRad(rotation.x+90)), 0, cos(degToRad(rotation.x+90)));
	updateMatrix();
}

glm::mat4 agl::Camera::getMatrix() {
	return camToWorld;
}

glm::vec3 agl::Camera::getLookVector() {
	return lookVector;
}

glm::vec3 agl::Camera::getPosition() {
	return position;
}

agl::Camera::~Camera() {
}