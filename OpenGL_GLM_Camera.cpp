#pragma once

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <vector>
#include <cstdio>

#define PI 3.1415
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define CUBE_SIZE 288

// Kody shaderów
const GLchar* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 color;
in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 Color;

void main(){
TexCoord=aTexCoord;
Color = color;
gl_Position = proj * view * model * vec4(position, 1.0);
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec3 Color;
out vec4 outColor;
in vec2 TexCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
outColor = mix(texture(texture1,TexCoord),texture(texture2,TexCoord),0.5);
//outColor = texture(texture1,TexCoord);
//outColor = texture(texture2,TexCoord);
}
)glsl";
class Cube {

	GLfloat vertices[CUBE_SIZE];

public:
	Cube(GLuint vbo)
	{
		initializeVertices();
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 36, vertices, GL_STATIC_DRAW);
	}
	~Cube()
	{

	}
	GLfloat getVertices()
	{
		return *vertices;
	}
	void initializeVertices()
	{
		GLfloat vertices_[CUBE_SIZE] = {
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f,0.0f,
0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,1.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f,1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f,0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f,0.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,1.0f,
0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f,0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,1.0f,0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,1.0f,1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,1.0f,1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f,1.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f,0.0f,

0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f,0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,1.0f,0.0f,
0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,1.0f,1.0f,
0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,1.0f,1.0f,
0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f,1.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f,0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,
0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,1.0f,0.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f,1.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f,1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f,1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,1.0f,0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f,1.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f,1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f,1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,0.0f,0.0f


		};
		for (int i = 0; i < CUBE_SIZE; i++)
		{
			this->vertices[i] = vertices_[i];
		}


	}

};
class Texture
{

	std::string name;
	unsigned int texture;
	int width, height, nrChannels;
	unsigned char* data;
public:
	static int licznik;
	Texture(std::string fileName, GLuint* shaderProgram)
	{
		licznik++;
		name = "texture" + std::to_string(licznik);

		//Jak do wszyskich obiektów w OpenGL do tekstury można odwołac się poprzez identyfikator ID// texture 1
		glGenTextures(1, &texture);                              // Generuje tekstury        
		glBindTexture(GL_TEXTURE_2D, texture);     //Ustawienie tekstury jako bieżącej (powiązanie)        
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps

		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		glUniform1i(glGetUniformLocation(*shaderProgram, name.c_str()), licznik - 1);

	}
	~Texture()
	{
		stbi_image_free(data);
	}
	int getID()
	{
		return this->texture;
	}
};

int Texture::licznik = 0;

class Shader
{
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	GLint posAttrib;
	GLint colAttrib;
	GLint texAttrib;
public:
	Shader()
	{
		compileShaders();
		setPosAttrib();
		setColAtrrib();
		setTexAttrib();
	}
	~Shader()
	{
		glDeleteProgram(shaderProgram);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}
	void compileShaders()
	{

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		GLint vertexSucces;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSucces);
		if (vertexSucces);// std::cout << "Vertex shader compilation succeded\n";
		else
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		GLint fragmentSucces;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSucces);
		if (fragmentSucces);// std::cout << "Fragment shader compilation succeded\n";
		else
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		}
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindFragDataLocation(shaderProgram, 0, "outColor");
		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);
	}
	void setTexAttrib()
	{
		this->texAttrib = glGetAttribLocation(this->shaderProgram, "aTexCoord");
		glEnableVertexAttribArray(this->texAttrib);
		glVertexAttribPointer(this->texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	}
	GLint setPosAttrib()
	{
		this->posAttrib = glGetAttribLocation(this->shaderProgram, "position");
		glEnableVertexAttribArray(this->posAttrib);
		glVertexAttribPointer(this->posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		return posAttrib;
	}
	GLint setColAtrrib()
	{
		this->colAttrib = glGetAttribLocation(this->shaderProgram, "color");
		glEnableVertexAttribArray(this->colAttrib);
		glVertexAttribPointer(this->colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		return colAttrib;
	}
	GLuint* getShaderProgram()
	{
		return &shaderProgram;
	}
	GLuint* getVertexShader()
	{
		return &vertexShader;
	}
	GLuint* getFragmentShader()
	{
		return &fragmentShader;
	}
};
class Camera
{
private:
	glm::vec3 cameraPos;   // poczatkowe wartosci do macierzy widoku
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::mat4 model;
	GLint uniTrans;
	GLint uniView;
	glm::mat4 proj;
	GLint uniProj;
	double yaw = -90;  //obrót względem osi Y
	double pitch = 0;   //obrót względem osi X
	int lastX;
	int lastY;
	bool firstMouse;
	float camera_rotation = 0;

public:
	Camera(GLuint& shaderProgram)
	{
		firstMouse = true;
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::mat4(1.0f);
		model = glm::rotate(model, float(PI / 180 * 0), glm::vec3(1.0f, 0.0f, 0.0f)); // rotacja modelu o 45 stopni w osi X
		uniTrans = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));
		uniView = glGetUniformLocation(shaderProgram, "view");
		proj = glm::perspective(float(PI / 180 * 45), 800.0f / 600.0f, 0.06f, 100.0f); // fov,proporcje ekranu, bliska plaszczyzna, daleka plaszczyzna
		uniProj = glGetUniformLocation(shaderProgram, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	}
	~Camera()
	{

	}

	void setCameraKeyboard(float time)
	{
		float cameraSpeed = 0.000002f * time; //jak szybko ma sie poruszac kamerka

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			cameraPos += cameraSpeed * cameraFront;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			cameraPos -= cameraSpeed * cameraFront;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view)); //zaktualizowanie widoku 
	}
	void setCameraMouse(float time, sf::Window& window)
	{
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);

		sf::Vector2i position;
		bool relokacja = false;


		if (localPosition.x <= 0)
		{
			position.x = window.getSize().x;
			position.y = localPosition.y;
			relokacja = true;
		}
		if (localPosition.x >= window.getSize().x - 1)
		{
			position.x = 0;
			position.y = localPosition.y;
			relokacja = true;
		}
		if (localPosition.y <= 0)
		{
			position.x = localPosition.x;
			position.y = window.getSize().y;
			relokacja = true;
		}
		if (localPosition.y >= window.getSize().y - 1)
		{
			position.x = localPosition.x;
			position.y = 0;
			relokacja = true;
		}

		if (relokacja)
		{
			sf::Mouse::setPosition(position, window);
			firstMouse = true;
		}
		localPosition = sf::Mouse::getPosition(window);

		if (firstMouse)
		{
			lastX = localPosition.x;
			lastY = localPosition.y;
			firstMouse = false;
		}

		double offsetX = localPosition.x - lastX;
		double offsetY = localPosition.y - lastY;
		lastX = localPosition.x;
		lastY = localPosition.y;

		double czulosc = 0.8f;
		double cameraSpeed = 0.00002f * time;

		offsetX *= czulosc;
		offsetY *= czulosc;

		yaw += offsetX * cameraSpeed;
		pitch -= offsetY * cameraSpeed;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;



		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);

		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	}
};
class Okno
{
private:
	sf::ContextSettings settings;
	sf::Window* window;

public:
	Okno()
	{
		settings.depthBits = 24;
		settings.stencilBits = 8;
		window = new sf::Window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
		(*window).setMouseCursorGrabbed(true);
		(*window).setMouseCursorVisible(false);
	}
	~Okno()
	{
		(*window).close();
	}
	sf::Window* getWindow()
	{
		return window;
	}
};

int main()
{
	sf::Clock clock;
	sf::Time time;
	Okno window;
	glewExperimental = GL_TRUE;
	glewInit();
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	Cube kostka(vbo);
	Shader shader;


	Texture tekstura("wood.bmp", shader.getShaderProgram());
	Texture tekstura1("dogs.jpg", shader.getShaderProgram());

	Camera kamera(*shader.getShaderProgram());

	glEnable(GL_DEPTH_TEST);
	bool running = true;
	int counter = 0;
	while (running) {
		time = clock.getElapsedTime();
		clock.restart();
		(*window.getWindow()).setFramerateLimit(60);
		counter++;
		float fps = 1000000 / time.asMicroseconds();
		if (counter > fps)
		{
			(*window.getWindow()).setTitle(std::to_string(fps));
		}

		sf::Event windowEvent;
		while ((*window.getWindow()).pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				running = false;
				break;
			case (sf::Event::MouseMoved):
			{
				kamera.setCameraMouse(time.asMicroseconds(), (*window.getWindow()));
				break;
			}
			}

		}
		kamera.setCameraKeyboard(time.asMicroseconds());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tekstura.getID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tekstura1.getID());


		glDrawArrays(GL_TRIANGLES, 0, 36);

		(*window.getWindow()).display();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return 0;
}
