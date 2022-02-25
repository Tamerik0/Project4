#include "ShaderProgram.hpp"
#include "Shader.hpp"
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VAO.hpp"
float aspect = 1;
sf::Clock clock_;
VAO *vao;

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
    VBO &vbo = *new VBO();
    vbo.setVertexAttribs({ VBO::VertexAttrib(0,GL_FLOAT,3),VBO::VertexAttrib(1,GL_FLOAT,3) });
    vbo.setDrawMode(GL_QUADS);
    vbo.setUsage(GL_STATIC_DRAW);
    vbo.update(&vertices, 8);
    EBO &ebo= *new EBO();
    ebo.setUsage(GL_STATIC_DRAW);
    ebo.setData(&indices, 24);
    vao = new VAO();
    vao->setVBO(&vbo);
    vao->setEBO(&ebo);
    vao->setup();

}
int main()
{
    sf::ContextSettings windowSettings;
    windowSettings.depthBits = 24;
    windowSettings.stencilBits = 8;
    windowSettings.antialiasingLevel = 16;

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
        
     
        glm::mat4 r(1.0f);
        r = glm::rotate(r, (float)clock_.getElapsedTime().asMilliseconds() * 0.002f, glm::vec3(0.6, 0.4, 0.5));
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(r));
        shaderProgram.use();
        vao->draw();
        window.display();
    }

    return 0;
}