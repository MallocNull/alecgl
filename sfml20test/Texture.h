#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdarg.h>

namespace agl {
	class Texture {
		std::vector<sf::Image*> img;
		std::vector<GLuint> texids;
		sf::Clock *clk;
		int index;
		float fps;
	public:
		Texture() {};
		Texture(char*);
		Texture(float, int, ...);
		int addImageFromFile(char*);
		int loadFromFile(char*);
		int loadFromFile(float, int, ...);
		void bindTexture();
		void unbindTexture();
		GLuint getTextureId();
		~Texture();
	};
};