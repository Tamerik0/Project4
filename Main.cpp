#include "ShaderProgram.hpp"
#include "Shader.hpp"
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float aspect = 1;
sf::Clock clock_;
uint32_t VBO;
uint32_t VAO;
uint32_t EBO;
float vertices[]{
    -0.5, -0.5, -0.5,   1,0,0,
    0.5, -0.5, -0.5,   0,1,0,
    0.5, -0.5, 0.5,   0,0,1,
    -0.5, -0.5, 0.5,  1,1,0,
    -0.5, 0.5, -0.5,   1,0,1,
    0.5, 0.5, -0.5,   0,1,1,
    0.5, 0.5, 0.5,   1,1,1,
    -0.5, 0.5, 0.5,   0,0,0
    
};
uint32_t indices[]{
    0,1,2,3,
    0,1,5,4,
    1,2,6,5,
    4,5,6,7,
    0,4,7,3,
    3,7,6,2
};

void InitOpenGL() {
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
    else {
        std::cout << "GLEW initialized"<<std::endl;
    }

    glEnable(GL_DEPTH_TEST);

}
void CreateCubeVAO () {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
int main()
{
    sf::ContextSettings windowSettings;
    windowSettings.depthBits = 24;
    windowSettings.stencilBits = 8;
    windowSettings.antialiasingLevel = 24;

    

    sf::RenderWindow window(sf::VideoMode(800, 600), "Window", sf::Style::Default, windowSettings);
    InitOpenGL();
    CreateCubeVAO();
    
    Shader vShader("Shaders/vertex.glsl", GL_VERTEX_SHADER);
    Shader fShader("Shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    ShaderProgram shaderProgram{vShader,fShader};

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sf::Vector2u windowSize = window.getSize();
        if ((double)windowSize.x / windowSize.y > aspect) {
            glViewport((windowSize.x - windowSize.y * aspect) / 2, 0, windowSize.y * aspect, windowSize.y);

        }
        else {
            glViewport(0, (windowSize.y - windowSize.x / aspect) / 2, windowSize.x, windowSize.x / aspect);
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(clock_.getElapsedTime().asMilliseconds() * 0.1, 0.6, 0.4, 0.5);
        /*glBegin(GL_QUADS);
        glColor3f(1, 0, 1);
        glVertex3f(-0.5, -0.5, -0.5);
        glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(0.5, -0.5, -0.5);

        glColor3f(0, 1, 1);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, -0.5);

        glColor3f(1, 1, 0);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(-0.5, -0.5, 0.5);

        glColor3f(0, 0, 1);
        glVertex3f(-0.5, -0.5, -0.5);
        glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(-0.5, 0.5, -0.5);

        glColor3f(1, 0, 0);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(-0.5, 0.5, -0.5);

        glColor3f(0, 1, 0);
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(-0.5, -0.5, -0.5);


        glEnd();*/
        glBindVertexArray(VAO);
        glm::mat4 r(1.0f);
        r = glm::rotate(r, (float)clock_.getElapsedTime().asMilliseconds() * 0.002f, glm::vec3(0.6, 0.4, 0.5));
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(r));
        shaderProgram.use();
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
        window.display();
    }

    return 0;
}