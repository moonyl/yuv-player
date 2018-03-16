
#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <shader.hpp>

std::FILE *infile = nullptr;
enum {
	pixel_w = 256,
	pixel_h = 256
};
uint8_t buf[pixel_w*pixel_h * 3 / 2];
uint8_t *plane[3];

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

																   // Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "YUV-player", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("videoCoordLoading.vs", "colorSpaceConversion.fs");

	if ((infile = std::fopen("seol.yuv", "rb")) == NULL) {
		//if ((infile = std::fopen("seol.yuv", "rb")) == NULL) {
		std::cerr << "cannot open this file\n" << std::endl;
	}

	plane[0] = buf;
	plane[1] = plane[0] + pixel_w * pixel_h;
	plane[2] = plane[1] + pixel_w * pixel_h / 4;

	std::cout << "width height " << pixel_w << " " << pixel_h << std::endl;

	if (std::fread(buf, 1, pixel_w*pixel_h * 3 / 2, infile) != pixel_w * pixel_h * 3 / 2) {
		// Loop  
		std::cerr << "read byte not enough. " << std::endl;
		std::fseek(infile, 0, 0);
		std::fread(buf, 1, pixel_w*pixel_h * 3 / 2, infile);
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // texture coords
		0.5f,  0.5f, 0.0f,     1.0f, 0.0f,		// top right
		0.5f, -0.5f, 0.0f,     1.0f, 1.0f,		// bottom right
		-0.5f, -0.5f, 0.0f,    0.0f, 1.0f,		// bottom left
		-0.5f,  0.5f, 0.0f,    0.0f, 0.0f		// top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int textureY, textureU, textureV;
	glGenTextures(1, &textureY);
	glBindTexture(GL_TEXTURE_2D, textureY);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	// set texture filtering parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w, pixel_h, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &textureU);
	glBindTexture(GL_TEXTURE_2D, textureU);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	
	// set texture filtering parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w / 2, pixel_h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &textureV);
	glBindTexture(GL_TEXTURE_2D, textureV);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	// set texture filtering parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w / 2, pixel_h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUseProgram(programID);

	glUniform1i(glGetUniformLocation(programID, "textureY"), 0);
	glUniform1i(glGetUniformLocation(programID, "textureU"), 1);
	glUniform1i(glGetUniformLocation(programID, "textureV"), 2);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	do {
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureY);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureU);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureV);

		// Use our shader
		glUseProgram(programID);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

