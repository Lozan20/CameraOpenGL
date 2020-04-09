// Nagłówki

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/System/Time.hpp>

#define PI 3.1415

// Kody shaderów
const GLchar* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 Color;

void main(){
Color = color;
gl_Position = proj * view * model * vec4(position, 1.0);
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec3 Color;
out vec4 outColor;
void main()
{
outColor = vec4(Color, 1.0);
}
)glsl";

class Shader
{
private:
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLint posAttrib;
    GLint colAttrib;
public:
    Shader()
    {
        compileShaders(vertexShader, fragmentShader, shaderProgram);
        setPosAttrib(posAttrib, shaderProgram);
        setColAtrrib(colAttrib, shaderProgram);
    }
    ~Shader()
    {
        glDeleteProgram(shaderProgram);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    }
    void compileShaders(GLuint& vertexShader, GLuint& fragmentShader, GLuint& shaderProgram)
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
    GLint setPosAttrib(GLint& posAttrib, GLuint& shaderProgram)
    {
        posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        return posAttrib;
    }
    GLint setColAtrrib(GLint& colAttrib, GLuint& shaderProgram)
    {
        colAttrib = glGetAttribLocation(shaderProgram, "color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
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
    double yaw;  //obrót względem osi Y
    double pitch;   //obrót względem osi X
    int lastX;
    int lastY;
    bool firstMouse;
    float camera_rotation = 0;

public:
    Camera(GLuint &shaderProgram)
    {
        yaw = -90;
        pitch = 0;
        firstMouse = true;
        cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, float(PI / 180 * 45), glm::vec3(1.0f, 0.0f, 0.0f)); // rotacja modelu o 45 stopni w osi X
        uniTrans = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));
        uniView = glGetUniformLocation(shaderProgram, "view");
        proj = glm::perspective(float(PI / 180 * 45), 800.0f / 600.0f, 0.06f, 100.0f); // fov,proporcje ekranu, bliska plaszczyzna, daleka plaszczyzna
        uniProj = glGetUniformLocation(shaderProgram, "proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
    }
    ~Camera() {}

    void setCameraKeyboard( float time)
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
    void setCameraMouse( float time, sf::Window& window)
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

        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));

    }
};
class Okno
{
private:
    sf::ContextSettings settings;
    sf::Window *window;

public:
    Okno()
    {
        settings.depthBits = 24;
        settings.stencilBits = 8;
        window = new sf::Window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
        (*window).setMouseCursorGrabbed(true);
        (*window).setMouseCursorVisible(false);
    }
    sf::Window* getWindow()
    {
        return window;
    }
};

void kostka(GLint vbo_buffer)
{
    GLfloat vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f


    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6*36, vertices, GL_STATIC_DRAW);
}
int main()
{

    Okno window;
    GLuint vao;
    GLuint vbo;
    glewExperimental = GL_TRUE;
    glewInit();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    kostka(vbo);

    sf::Clock clock;
    sf::Time time;
    Shader shader;
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
        glDrawArrays(GL_TRIANGLES, 0, 36);

        (*window.getWindow()).display();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    (*window.getWindow()).close();
    return 0;
}