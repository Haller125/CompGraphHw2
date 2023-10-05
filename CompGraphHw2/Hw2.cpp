#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float pi = 3.14159265;

GLuint vbo, cbo, nbo;
GLuint colloc, matloc, normloc;
glm::mat4 transform;
std::vector<float> vertices;
std::vector<float> normals;
std::vector<float> colors;

void drawSphere(glm::vec3 position = glm::vec3(0.5f, 0.1f, 0.0f), float r = 0.25f, size_t N = 50){
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N / 2; ++j)
		{
			float x = position.x + r * cosf(i * 2 * pi / N) * sinf(j * 2 * pi / N);
			float y = position.y + r * sinf(i * 2 * pi / N) * sinf(j * 2 * pi / N);
			float z = position.z + r * cosf(j * 2 * pi / N);

			float x1 = position.x + r * cosf((i + 1) * 2 * pi / N) * sinf(j * 2 * pi / N);
			float y1 = position.y + r * sinf((i + 1) * 2 * pi / N) * sinf(j * 2 * pi / N);
			float z1 = position.z + r * cosf(j * 2 * pi / N);

			float x2 = position.x + r * cosf((i) * 2 * pi / N) * sinf((j + 1) * 2 * pi / N);
			float y2 = position.y + r * sinf((i) * 2 * pi / N) * sinf((j + 1) * 2 * pi / N);
			float z2 = position.z + r * cosf((j + 1) * 2 * pi / N);

			float x3 = position.x + r * cosf((i + 1) * 2 * pi / N) * sinf((j + 1) * 2 * pi / N);
			float y3 = position.y + r * sinf((i + 1) * 2 * pi / N) * sinf((j + 1) * 2 * pi / N);
			float z3 = position.z + r * cosf((j + 1) * 2 * pi / N);

			vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
			vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(z1);
			vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(z2);
			vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(z1);
			vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(z2);
			vertices.push_back(x3); vertices.push_back(y3); vertices.push_back(z3);

			glm::vec3 tangent1 = glm::normalize(glm::vec3(x1, y1, z1) - glm::vec3(x, y, z));
			glm::vec3 tangent2 = glm::normalize(glm::vec3(x2, y2, z2) - glm::vec3(x, y, z));

			glm::vec3 normal = -glm::cross(tangent1, tangent2);


			for (auto _j = 0; _j < 6; _j++)
			{
				colors.push_back(1);
				colors.push_back(0);
				colors.push_back(0);
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
			}
		}
	}
}

void drawCube(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float s = 0.25f, float angleX = 45.0f, float angleY = 45.0f) {
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMatrix = rotationY * rotationX;

	glm::vec3 corners[8];

	for (int i = 0; i < 8; ++i) {
		glm::vec3 offset = glm::vec3((i & 1 ? s : -s) / 2, (i & 2 ? s : -s) / 2, (i & 4 ? s : -s) / 2);
		corners[i] = position + glm::vec3(rotationMatrix * glm::vec4(offset, 1.0f));
	}
	int faces[12][3] = {
		{3, 7, 6}, {3, 6, 2}, 
		{0, 1, 2}, {2, 3, 1}, 
		{7, 6, 4}, {7, 5, 4}, 
		{4, 0, 2}, {4, 2, 6}, 
		{4, 5, 0}, {0, 5, 1},
		{1, 3, 7}, {1, 7, 5}
	};
	for (int i = 0; i < 12; i++) {
		glm::vec3 triangle[3] = { corners[faces[i][0]], corners[faces[i][1]], corners[faces[i][2]] };

		// Compute the normal for the triangle.
		glm::vec3 normal = glm::normalize(glm::cross(triangle[2] - triangle[1], triangle[2] - triangle[0]));

		for (int j = 0; j < 3; j++) {
			vertices.push_back(triangle[j].x);
			vertices.push_back(triangle[j].y);
			vertices.push_back(triangle[j].z);

			colors.push_back(0);
			colors.push_back(1);  
			colors.push_back(0);  

			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
		}
	}
}

void drawCylinder(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float radius = 0.2f, float height = 0.4f, float angleX = 70.0f, int segments = 64) {
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));

	for (int i = 0; i < segments; i++) {
		float theta1 = float(i) / segments * 2.0f * pi;
		float theta2 = float(i + 1) / segments * 2.0f * pi;


		glm::vec3 sideNormal1(cos(theta1), 0.0f, sin(theta1));
		glm::vec3 sideNormal2(cos(theta2), 0.0f, sin(theta2));


		// Top circle
		glm::vec3 top_center = position + glm::vec3(0, height / 2, 0);
		glm::vec3 top1 = top_center + glm::vec3(radius * cos(theta1), 0, radius * sin(theta1));
		glm::vec3 top2 = top_center + glm::vec3(radius * cos(theta2), 0, radius * sin(theta2));

		// Apply rotation
		top1 = glm::vec3(rotationX * glm::vec4(top1, 1.0f));
		top2 = glm::vec3(rotationX * glm::vec4(top2, 1.0f));
		top_center = glm::vec3(rotationX * glm::vec4(top_center, 1.0f));

		// Bottom circle
		glm::vec3 bottom_center = position - glm::vec3(0, height / 2, 0);
		glm::vec3 bottom1 = bottom_center + glm::vec3(radius * cos(theta1), 0, radius * sin(theta1));
		glm::vec3 bottom2 = bottom_center + glm::vec3(radius * cos(theta2), 0, radius * sin(theta2));

		// Apply rotation
		bottom1 = glm::vec3(rotationX * glm::vec4(bottom1, 1.0f));
		bottom2 = glm::vec3(rotationX * glm::vec4(bottom2, 1.0f));
		bottom_center = glm::vec3(rotationX * glm::vec4(bottom_center, 1.0f));

		// Apply rotation to vertices and normals
		top1 = glm::vec3(rotationX * glm::vec4(top1, 1.0f));
		top2 = glm::vec3(rotationX * glm::vec4(top2, 1.0f));
		top_center = glm::vec3(rotationX * glm::vec4(top_center, 1.0f));

		bottom1 = glm::vec3(rotationX * glm::vec4(bottom1, 1.0f));
		bottom2 = glm::vec3(rotationX * glm::vec4(bottom2, 1.0f));
		bottom_center = glm::vec3(rotationX * glm::vec4(bottom_center, 1.0f));

		sideNormal1 = glm::vec3(rotationX * glm::vec4(sideNormal1, 0.0f));
		sideNormal2 = glm::vec3(rotationX * glm::vec4(sideNormal2, 0.0f));

		for (int j = 0; j < 3; j++) {
			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);
		}

		for (int j = 0; j < 3; j++) {
			normals.push_back(0.0f);
			normals.push_back(-1.0f);
			normals.push_back(0.0f);
		}

		for (int j = 0; j < 3; j++) {
			colors.push_back(1.0f);  
			colors.push_back(0.0f);  
			colors.push_back(0.0f);  
		}

		for (int j = 0; j < 3; j++) {
			colors.push_back(1.0f);  
			colors.push_back(0.0f);  
			colors.push_back(0.0f);  
		}
		
		for (int j = 0; j < 6; j++) {
			colors.push_back(1.0f);  
			colors.push_back(0.0f);  
			colors.push_back(0.0f);  
		}

		// Side quad normals
		normals.push_back(sideNormal1.x); normals.push_back(sideNormal1.y); normals.push_back(sideNormal1.z);
		normals.push_back(sideNormal1.x); normals.push_back(sideNormal1.y); normals.push_back(sideNormal1.z);
		normals.push_back(sideNormal2.x); normals.push_back(sideNormal2.y); normals.push_back(sideNormal2.z);

		normals.push_back(sideNormal1.x); normals.push_back(sideNormal1.y); normals.push_back(sideNormal1.z);
		normals.push_back(sideNormal2.x); normals.push_back(sideNormal2.y); normals.push_back(sideNormal2.z);
		normals.push_back(sideNormal2.x); normals.push_back(sideNormal2.y); normals.push_back(sideNormal2.z);

		// Vertices for the top triangle
		vertices.push_back(top_center.x); vertices.push_back(top_center.y); vertices.push_back(top_center.z);
		vertices.push_back(top1.x); vertices.push_back(top1.y); vertices.push_back(top1.z);
		vertices.push_back(top2.x); vertices.push_back(top2.y); vertices.push_back(top2.z);

		// Vertices for the bottom triangle
		vertices.push_back(bottom_center.x); vertices.push_back(bottom_center.y); vertices.push_back(bottom_center.z);
		vertices.push_back(bottom1.x); vertices.push_back(bottom1.y); vertices.push_back(bottom1.z);
		vertices.push_back(bottom2.x); vertices.push_back(bottom2.y); vertices.push_back(bottom2.z);

		// Side quads
		vertices.push_back(top1.x); vertices.push_back(top1.y); vertices.push_back(top1.z);
		vertices.push_back(bottom1.x); vertices.push_back(bottom1.y); vertices.push_back(bottom1.z);
		vertices.push_back(bottom2.x); vertices.push_back(bottom2.y); vertices.push_back(bottom2.z);

		vertices.push_back(top1.x); vertices.push_back(top1.y); vertices.push_back(top1.z);
		vertices.push_back(bottom2.x); vertices.push_back(bottom2.y); vertices.push_back(bottom2.z);
		vertices.push_back(top2.x); vertices.push_back(top2.y); vertices.push_back(top2.z);

		// You can compute colors, normals and other vertex attributes similarly
	}
}


void mydisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

	glutSwapBuffers();
}

void myidle()
{
	glm::vec3 center = glm::vec3(0.3, 0.5, 0);
	glUniformMatrix4fv(matloc, 1, GL_FALSE, glm::value_ptr<float>(transform));
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(720, 720);
	glutInitWindowPosition(100, 200);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);

	glewInit();
	glEnable(GL_DEPTH_TEST);


	drawSphere(glm::vec3(0.65f, 0.2f, 0.0f), 0.2f);
	drawCube(glm::vec3(-0.65f, 0.2f, 0.0f), 0.25f);   
	drawCylinder();


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
		vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]),
		colors.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]),
		normals.data(), GL_STATIC_DRAW);

	GLuint shp = glCreateProgram();
	GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vshcode = ""
		"#version 150 \n"
		"attribute vec3 pos;"
		"attribute vec3 color;"
		"attribute vec3 normal;"
		"varying vec3 vcolor;"
		"varying vec3 vnormal;"
		"varying vec3 vpos;"
		"uniform mat4 transform;"
		"void main() {"
		"vcolor = color;"
		"vnormal = normal;"
		"vpos = pos;"
		"gl_Position = transform*vec4(pos,1);}";
	GLint vshlen = strlen(vshcode);
	glShaderSource(vsh, 1, &vshcode, &vshlen);
	glCompileShader(vsh);
	glAttachShader(shp, vsh);

	GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fshcode = ""
		"#version 150 \n"
		"varying vec3 vcolor;"
		"varying vec3 vnormal;"
		"varying vec3 vpos;"
		"void main() {"
		"vec3 lightpos = vec3(-3,-3,-3);"
		"vec3 camdir = vec3(0,0,-1);"
		"vec3 ambientColor = vec3(0.15, 0.15, 0.05);"
		"float lighting = max(dot(normalize(vnormal), normalize(lightpos - vpos)), 0);"
		"vec3 r = reflect(-normalize(lightpos - vpos), normalize(vnormal));"
		"float specularity = max(pow(dot(camdir, r), 30), 0);"
		"gl_FragColor = vec4(ambientColor + lighting*vcolor + specularity*vec3(1), 1);}";
	GLint fshlen = strlen(fshcode);
	glShaderSource(fsh, 1, &fshcode, &fshlen);
	glCompileShader(fsh);

	GLint isCompiled = 0;
	glGetShaderiv(fsh, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fsh, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fsh, maxLength, &maxLength, &errorLog[0]);

		printf("%s", errorLog.data());
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(fsh); // Don't leak the shader.
		return 0;
	}

	glAttachShader(shp, fsh);

	glLinkProgram(shp);
	glValidateProgram(shp);
	glUseProgram(shp);

	//colloc = glGetUniformLocation(shp, "color");

	GLuint posloc = glGetAttribLocation(shp, "pos");
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(posloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posloc);

	GLuint colorloc = glGetAttribLocation(shp, "color");
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glVertexAttribPointer(colorloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorloc);

	normloc = glGetAttribLocation(shp, "normal");
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(normloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normloc);

	transform = glm::mat4(1);
	matloc = glGetUniformLocation(shp, "transform");

	glClearColor(1, 1, 0, 1);
	glutDisplayFunc(mydisplay);
	glutIdleFunc(myidle);
	//glutTimerFunc(100, mytimer, 0);
	glutMainLoop();

	glDisableVertexAttribArray(posloc);
	glDisableVertexAttribArray(colorloc);
	glDisableVertexAttribArray(normloc);

	return 0;
}