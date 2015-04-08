#include "NoiseReader.h"

agl::NoiseReader::NoiseReader(char *filename) {
	loadFromFile(filename);
}

float agl::NoiseReader::avgPixel(sf::Color c) {
	return (c.r+c.g+c.b)/3;
}

int agl::NoiseReader::loadFromFile(char *filename) {
	sf::Image tmp;
	tmp.loadFromFile(filename);

	rnum = ((tmp.getSize().x-1)*2)*(tmp.getSize().y-1);
	
	vertexData	= new float[rnum*3*3];
	normalData	= new float[rnum*3*3];
	texData		= new float[rnum*3*2];

	printf("%i %i\n", tmp.getSize().x, tmp.getSize().y);

	glm::vec3 *tmpv = new glm::vec3[3],
			   tmpn;
	glm::vec2 *tmpt = new glm::vec2[3];

	const float wmult = 1, hmult = .15;

	int i = 0;
	int ti = 0;
	for(int y = 0; y < tmp.getSize().y-1; y++) {
		for(int x = 0; x < tmp.getSize().x-1; x++) {
			float h[] = {avgPixel(tmp.getPixel(x+1,y)), avgPixel(tmp.getPixel(x,y)), avgPixel(tmp.getPixel(x,y+1))};

			tmpv[0] = glm::vec3((x+1)*wmult,h[0]*hmult,y*wmult);
			tmpv[1] = glm::vec3(x*wmult,h[1]*hmult,y*wmult);
			tmpv[2] = glm::vec3(x*wmult,h[2]*hmult,(y+1)*wmult);
			
			int a = 2;
			float b = 1.0f/float(a);
			tmpt[0] = glm::vec2(b+b*(x%a), b+b*(y%a));
			tmpt[1] = glm::vec2(b*(x%a), b+b*(y%a));
			tmpt[2] = glm::vec2(b*(x%a), b*(y%a));

			tmpn = -glm::cross(tmpv[0]-tmpv[1],tmpv[2]-tmpv[1]);
			
			for(int w = 0; w < 3; w++) {
				vertexData[i] = tmpv[w].x;
				normalData[i] = tmpn.x;
				texData[ti]    = tmpt[w].x;
				i++; ti++;
			
				vertexData[i] = tmpv[w].y;
				normalData[i] = tmpn.y;
				texData[ti]    = tmpt[w].y;
				i++; ti++;
			
				vertexData[i] = tmpv[w].z;
				normalData[i] = tmpn.z;
				i++;
			}
			
			
			h[0] = avgPixel(tmp.getPixel(x+1,y));
			h[1] = avgPixel(tmp.getPixel(x,y+1));
			h[2] = avgPixel(tmp.getPixel(x+1,y+1));

			tmpv[0] = glm::vec3((x+1)*wmult,h[0]*hmult,y*wmult);
			tmpv[1] = glm::vec3(x*wmult,h[1]*hmult,(y+1)*wmult);
			tmpv[2] = glm::vec3((x+1)*wmult,h[2]*hmult,(y+1)*wmult);
			
			tmpt[0] = glm::vec2(b+b*(x%a), b+b*(y%a));
			tmpt[1] = glm::vec2(b*(x%a), b*(y%a));
			tmpt[2] = glm::vec2(b+b*(x%a), b*(y%a));

			tmpn = glm::cross(tmpv[0]-tmpv[2],tmpv[1]-tmpv[2]);
			
			for(int w = 0; w < 3; w++) {
				vertexData[i] = tmpv[w].x;
				normalData[i] = tmpn.x;
				texData[ti]    = tmpt[w].x;
				i++; ti++;
			
				vertexData[i] = tmpv[w].y;
				normalData[i] = tmpn.y;
				texData[ti]    = tmpt[w].y;
				i++; ti++;
			
				vertexData[i] = tmpv[w].z;
				normalData[i] = tmpn.z;
				i++;
			}
		}
	}

	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, (rnum*3*3)*sizeof(float), vertexData, GL_STATIC_DRAW);
	
	glGenBuffers(1, &nbuf);
	glBindBuffer(GL_ARRAY_BUFFER, nbuf);
	glBufferData(GL_ARRAY_BUFFER, (rnum*3*3)*sizeof(float), normalData, GL_STATIC_DRAW);
	
	glGenBuffers(1, &tbuf);
	glBindBuffer(GL_ARRAY_BUFFER, tbuf);
	glBufferData(GL_ARRAY_BUFFER, (rnum*3*2)*sizeof(float), texData, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbuf);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, nbuf);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, tbuf);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);

	return 0;
}

void agl::NoiseReader::Render() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, rnum*3);
	glBindVertexArray(0);
}

agl::NoiseReader::~NoiseReader() {
}