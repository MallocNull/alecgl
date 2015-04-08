#include "MatrixStack.h"

agl::MatrixStack::MatrixStack(bool loadWithIdentity) {
	if(loadWithIdentity)
		loadIdentity();
}

void agl::MatrixStack::multTopByMatrix(glm::mat4 mat) {
	glm::mat4 top = stack.top();
	stack.pop();
	stack.push(mat * top);
}

void agl::MatrixStack::push() {
	stack.push(stack.top());
}

void agl::MatrixStack::pop() {
	if(!stack.empty())
		stack.pop();
	else
		printf("no");
}

void agl::MatrixStack::translate(float x, float y, float z) {
	multTopByMatrix(glm::translate(glm::mat4(1.0), glm::vec3(x,y,z)));
}

void agl::MatrixStack::translate(glm::vec3 vec) {
	multTopByMatrix(glm::translate(glm::mat4(1.0), vec));
}

void agl::MatrixStack::scale(float x, float y, float z) {
	multTopByMatrix(glm::scale(glm::mat4(1.0), glm::vec3(x,y,z)));
}

void agl::MatrixStack::scale(glm::vec3 vec) {
	multTopByMatrix(glm::scale(glm::mat4(1.0), vec));
}

void agl::MatrixStack::rotatex(float ang) {
	multTopByMatrix(glm::rotate(glm::mat4(1.0), ang, glm::vec3(1,0,0)));
}

void agl::MatrixStack::rotatey(float ang) {
	multTopByMatrix(glm::rotate(glm::mat4(1.0), ang, glm::vec3(0,1,0)));
}

void agl::MatrixStack::rotatez(float ang) {
	multTopByMatrix(glm::rotate(glm::mat4(1.0), ang, glm::vec3(0,0,1)));
}

void agl::MatrixStack::loadIdentity() {
	stack.push(glm::mat4(1.0));
}

glm::mat4 agl::MatrixStack::top() {
	if(!stack.empty())
		return stack.top();
}

agl::MatrixStack::~MatrixStack() {
}