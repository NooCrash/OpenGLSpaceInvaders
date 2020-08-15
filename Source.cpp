#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"

// binds the VAO and the VBO with the vertices
void bind(unsigned int* VAO, unsigned int* VBO, float *vertices, int verticesSize);

void move(unsigned int startPos, bool increase);

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

float vertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f	// top left 
};

unsigned short offset = 8;

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};


int main() {
	// init the GLFW
	glfwInit();

	// configure the GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Check if glad could be loaded
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// sets the portion which we will draw on 
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	Shader shader = Shader(".\\VertexShader.vs", ".\\FragmentShader.fs");

	unsigned int VAO, VBO, EBO;
	
	// generate the VAO, VBO and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	// location, size, type, normalized, stride = byte offset between consecutive vertex attributes, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	// location, size, type, normalized, stride = byte offset between consecutive vertex attributes, offset = starting pos
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	// location, size(x,y,z), type, normalized, stride = byte offset between consecutive vertex attributes, offset = starting pos
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int texture1, texture2;

	// create the texture
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture warpping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// load the image properties into these variables
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		// generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load the texture: awesomeface.png" << std::endl;
	}	

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture warpping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// load the image properties into these variables
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		// generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load the texture: awesomeface.png" << std::endl;
	}

	

	

	/*float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/

	// render loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// tells the gpu to use this shader program
		shader.use();
		glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
		shader.setInt("texture2", 1);

		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		glBindVertexArray(VAO);

		//glBindVertexArray(0);

		//bind(&VAOs[0], &VBOs[0], first_triangle, first_triangle_size);
		//glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//// tells the gpu to use this shader program
		//shader.use();
		//shader.setFloat("offset", 0.1f);

		//bind(&VAOs[1], &VBOs[1], second_triangle, second_triangle_size);
		//glBindVertexArray(VAOs[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		// Check if the escape key was pressed, close the window if so
		processInput(window);
		
		// swaps the color buffer that is used to render to during iteration and show it as 
		// output on the screen
		glfwSwapBuffers(window);

		// checks if any events are triggered(keyboard input / mouse movement)
		// updates the window state and calls corresponding functions
		glfwPollEvents();
	}

	// clean up the glfw stuff
	glfwTerminate();
	return 0;
}

void bind(unsigned int* VAO, unsigned int* VBO, float* vertices, int verticesSize) {
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	// location, size, type, normalized, stride = byte offset between consecutive vertex attributes, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// location, size, type, normalized, stride = byte offset between consecutive vertex attributes, offset = starting pos
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	unsigned short xStartPos = 0, yStartPos = 1;
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		move(yStartPos, true);
	}
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		move(xStartPos, false);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		move(yStartPos, false);
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		move(xStartPos, true);
	}	
}

void move(unsigned int startPos, bool increase) {
	if (startPos == -1) {
		return;
	}

	float move_offset = 0.001;
	unsigned short verticesLength = sizeof(vertices) / sizeof(vertices[0]);

	unsigned short index = startPos;
	while (index < verticesLength) {
		if (increase) {
			vertices[index] += move_offset;
		}
		else {
			vertices[index] -= move_offset;
		}
		index += offset;
	}
}