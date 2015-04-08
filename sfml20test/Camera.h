#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdarg.h>
#include "Common.h"

namespace agl {
	class Camera {
		glm::mat4 camToWorld;
		glm::vec3 lookVector;

		glm::vec3 position;
		glm::vec3 rotation;

		bool matrixToBeUpdated;

		void updateMatrix();

		float degToRad(float deg) {return deg*(M_PI/180);}
		float radToDeg(float rad) {return rad*(180/M_PI);}
	public:
		Camera();
		Camera(glm::vec3, glm::vec3=glm::vec3(0.0));
		Camera(float, float, float, float=0.0, float=0.0, float=0.0);

		void setCamera(glm::vec3, glm::vec3=glm::vec3(0.0));
		void setCamera(float, float, float, float=0.0, float=0.0, float=0.0);

		void setPosition(glm::vec3);
		void setPosition(float, float, float);
		
		void setRotation(glm::vec3);
		void setRotation(float, float, float);
		
		void rotateCamera(glm::vec3);
		void rotateCamera(float, float, float);

		void moveCamera(glm::vec3);
		void moveCamera(float, float, float);
		
		void move(float);	// positive is forward
		void strafe(float);	// positive is right

		void updateCamera();

		glm::mat4 getMatrix();
		glm::vec3 getLookVector();
		glm::vec3 getPosition();
		glm::vec3 getRotation();

		~Camera();
	};
};