#ifndef MATSTACKH
#define MATSTACKH

#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace agl {
	class MatrixStack {
		std::stack<glm::mat4> stack;

		void multTopByMatrix(glm::mat4);
	public:
		MatrixStack(bool=true);

		void loadIdentity();

		void push();
		void pop();
		
		void translate(float, float, float);
		void translate(glm::vec3);
		
		void rotatex(float);
		void rotatey(float);
		void rotatez(float);
		
		void scale(float, float, float);
		void scale(glm::vec3);

		glm::mat4 top();

		~MatrixStack();
	};
};

#endif