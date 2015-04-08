#include "Mesh.h"

agl::Mesh::Mesh(char *filename) {
	loadFromFile(filename);
}

int agl::Mesh::loadFromFile(char *filename) {
	char line[1024], *giblet;
	int bytesRead, count = 0, type = 0;
	std::string aids;
	FILE *fp = fopen(filename, "rb");
	glm::vec3 tmpv;
	Index tmpix[3];

	while(!feof(fp)) {
		fgets(line, 1024, fp);
		
		if(line[0] == 'v' && line[1] == ' ') {
			sscanf(line, "%*s %f %f %f", &tmpv.x, &tmpv.y, &tmpv.z);
			vertices.push_back(tmpv);
		} else if(line[0] == 'v' && line[1] == 'n') {
			sscanf(line, "%*s %f %f %f", &tmpv.x, &tmpv.y, &tmpv.z);
			normals.push_back(tmpv);
		} else if(line[0] == 'v' && line[1] == 't') {
			sscanf(line, "%*s %f %f", &tmpv.x, &tmpv.y);
			textices.push_back(glm::vec2(tmpv));
		} else if(line[0] == 'f' && line[1] == ' ') {
			sscanf(line, "%*s %i/%i/%i %i/%i/%i %i/%i/%i", &tmpix[0].vertexIndex, &tmpix[0].texIndex, &tmpix[0].normalIndex, &tmpix[1].vertexIndex, &tmpix[1].texIndex, &tmpix[1].normalIndex, &tmpix[2].vertexIndex, &tmpix[2].texIndex, &tmpix[2].normalIndex);
			indices.push_back(tmpix[0]);
			indices.push_back(tmpix[1]);
			indices.push_back(tmpix[2]);
		} else continue;
	}

	fclose(fp);
	
	vertexData = new float[indices.size()*3];
	normalData = new float[indices.size()*3];
	texData = new float[indices.size()*2];

	for(int i = 0; i < indices.size(); i++) {
		vertexData[i*3] = vertices[indices[i].vertexIndex - 1].x;
		vertexData[i*3+1] = vertices[indices[i].vertexIndex - 1].y;
		vertexData[i*3+2] = vertices[indices[i].vertexIndex - 1].z;
		
		normalData[i*3] = normals[indices[i].normalIndex - 1].x;
		normalData[i*3+1] = normals[indices[i].normalIndex - 1].y;
		normalData[i*3+2] = normals[indices[i].normalIndex - 1].z;
		
		texData[i*2] = textices[indices[i].texIndex - 1].x;
		texData[i*2+1] = textices[indices[i].texIndex - 1].y;
	}
	
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, (indices.size()*3)*sizeof(float), vertexData, GL_STATIC_DRAW);
	
	glGenBuffers(1, &nbuf);
	glBindBuffer(GL_ARRAY_BUFFER, nbuf);
	glBufferData(GL_ARRAY_BUFFER, (indices.size()*3)*sizeof(float), normalData, GL_STATIC_DRAW);
	
	glGenBuffers(1, &tbuf);
	glBindBuffer(GL_ARRAY_BUFFER, tbuf);
	glBufferData(GL_ARRAY_BUFFER, (indices.size()*2)*sizeof(float), texData, GL_STATIC_DRAW);

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

	rnum = indices.size();

	return 0;
}

void agl::Mesh::Render() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, rnum);
	glBindVertexArray(0);
}

void agl::Mesh::test() {
	printf("VERTICES:\n");
	for(int i = 0; i < rnum*3; i+=3)
		printf("(%f, %f, %f)\n", vertexData[i], vertexData[i+1], vertexData[i+2]);
	printf("\nNORMALS:\n");
	for(int i = 0; i < rnum*3; i+=3)
		printf("(%f, %f, %f)\n", normalData[i], normalData[i+1], normalData[i+2]);
	printf("\nTEXTICES:\n");
	for(int i = 0; i < rnum*2; i+=2)
		printf("(%f, %f)\n", texData[i], texData[i+1]);
}

agl::Mesh::~Mesh() {
}