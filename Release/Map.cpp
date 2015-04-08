#include "Map.h"

agl::Map::Map(char *file, std::vector<GLuint> texlist) {
	loadFromFile(file, texlist);
}

std::vector<std::string> agl::Map::explode(std::string str, std::string params) {
	std::vector<std::string> list;

	int currentOffset = 0;
	while(true) {
		int distFromStart = -1;

		for(int i = 0; i < params.length(); i++) {
			int pos = str.find(params[i], currentOffset);
			if((pos < distFromStart || distFromStart == -1) && pos != std::string::npos)
				distFromStart = pos;
		}

		if(distFromStart == -1) break;

		list.push_back(str.substr(currentOffset, distFromStart-currentOffset));
		currentOffset = distFromStart+1;
	}

	return list;
}

int agl::Map::loadFromFile(char *file, std::vector<GLuint> texlist) {
	FILE *fp;
	if((fp = fopen(file, "rb")) == NULL) return -1;

	texs = texlist;
	vertices.resize(texlist.size());
	normals.resize(texlist.size());
	textices.resize(texlist.size());

	char buffer[1024];
	
	while(fgets(buffer, 1024, fp) != NULL) {
		if(buffer[0] != '#') {
			auto parameters = explode(buffer, ",;");
			
			if(parameters.size() != 0) {
				if(parameters[0][0] == 'f') {
					glm::vec3 v[] = {
						glm::vec3(atof(parameters[2].c_str()),atof(parameters[3].c_str()),atof(parameters[4].c_str())),
						glm::vec3(atof(parameters[2].c_str()),atof(parameters[3].c_str()),atof(parameters[7].c_str())),
						glm::vec3(atof(parameters[5].c_str()),atof(parameters[6].c_str()),atof(parameters[7].c_str())),
						glm::vec3(atof(parameters[5].c_str()),atof(parameters[6].c_str()),atof(parameters[4].c_str()))
					};

					glm::vec2 t[] = {
						glm::vec2(0.0,0.0),
						glm::vec2(0.0,16.0),
						glm::vec2(16.0,16.0),
						glm::vec2(16.0,0.0)
					};

					glm::vec3 n = glm::normalize(-glm::cross(v[0]-v[1],v[2]-v[1]));

					int index = atoi(parameters[1].c_str());


				}
			
				/*for(int i = 0; i < 3; i++)
					vertices[index].push_back(v1[i]);
				for(int i = 0; i < 3; i++)
					normals[index].push_back(norm[i]);
				textices[index].push_back(0.0);
				textices[index].push_back(0.0);
			
				vertices[index].push_back(v1[0]);
				vertices[index].push_back(v1[1]);
				vertices[index].push_back(v2[2]);
				for(int i = 0; i < 3; i++)
					normals[index].push_back(norm[i]);
				textices[index].push_back(0.0);
				textices[index].push_back(1.0);
			
				for(int i = 0; i < 3; i++)
					vertices[index].push_back(v2[i]);
				for(int i = 0; i < 3; i++)
					normals[index].push_back(norm[i]);
				textices[index].push_back(1.0);
				textices[index].push_back(1.0);
			
				for(int i = 0; i < 3; i++)
					vertices[index].push_back(v2[i]);
				for(int i = 0; i < 3; i++)
					normals[index].push_back(norm[i]);
				textices[index].push_back(1.0);
				textices[index].push_back(1.0);
			
				vertices[index].push_back(v2[0]);
				vertices[index].push_back(v2[1]);
				vertices[index].push_back(v1[2]);
				for(int i = 0; i < 3; i++)
					normals[index].push_back(norm[i]);
				textices[index].push_back(1.0);
				textices[index].push_back(0.0);
			
				for(int i = 0; i < 3; i++)
					vertices[index].push_back(v1[i]);
				for(int i = 0; i < 3; i++)
					normals[index].push_back(norm[i]);
				textices[index].push_back(0.0);
				textices[index].push_back(0.0);*/
			}
		}
	}

	/*GLuint vbuf, nbuf, tbuf;

	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, (vertices[0].size())*sizeof(float), &vertices[0][0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &nbuf);
	glBindBuffer(GL_ARRAY_BUFFER, nbuf);
	glBufferData(GL_ARRAY_BUFFER, (normals[0].size())*sizeof(float), &normals[0][0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &tbuf);
	glBindBuffer(GL_ARRAY_BUFFER, tbuf);
	glBufferData(GL_ARRAY_BUFFER, (textices[0].size())*sizeof(float), &textices[0][0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint vao;
	
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

	vaos.push_back(vao);*/

	fclose(fp);
	return 0;
}

void agl::Map::render() {
	/*printf("%i\n", vertices[0].size()/3);
	glBindVertexArray(vaos[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices[0].size()/3);
	glBindVertexArray(0);*/
}

agl::Map::~Map() {

}