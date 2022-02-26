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
float aspect = 16/9;
sf::Clock clock_;
VAO *vao;
VAO *fractal;
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
    glPointSize(5);

}
void CreateCubeVAO () {
    vao = new VAO(new VBO({ VBO::VertexAttrib(0,GL_FLOAT,3),VBO::VertexAttrib(1,GL_FLOAT,3) }, GL_QUADS, GL_STREAM_DRAW, &vertices, 8), new EBO(&indices, 24, GL_STREAM_DRAW));
}
std::vector<double>* a;
std::vector<glm::vec3> fractalvertices;
void drawShape(int id, double x, double y, double z, double scale) {
    if (scale < 0.01) {
        //arr[int(x)][int(y)] = true;
        fractalvertices.push_back(glm::vec3(x, y, z));
        fractalvertices.push_back(glm::vec3(x, y, z));
        return;
    }
    for (int i = 0; i < a[id].size(); i += 5) {
        drawShape(a[id][i], x + a[id][i + 1] * scale, y + a[id][i + 2] * scale, z + a[id][i + 3] * scale, scale * a[id][i + 4]);
    }
}
void CreateFractalVAO() {
    
    int n;
    std::cin >> n;
    a = new std::vector<double>[n];
    for (int i = 0; i < n; i++) {
        int t;
        std::cin >> t;
        for (int j = 0; j < t * 5; j++) {
            double b;
            std::cin >> b;
            a[i].push_back(b);
        }
    }
    
    
    drawShape(0, 0, 0, 0, 1);
    glm::vec3* arr = fractalvertices.data();
    fractal = new VAO(new VBO({ VBO::VertexAttrib(0,GL_FLOAT,3),VBO::VertexAttrib(1,GL_FLOAT,3) }, GL_POINTS, GL_STATIC_DRAW, arr, fractalvertices.size()/2), nullptr);
    
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
    CreateFractalVAO();
    
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
        
        float fov=90;
        glm::mat4 r(1.0f);
        r = glm::perspective(glm::radians(fov)/aspect, aspect, 0.0001f, 10.0f)*glm::translate(r, glm::vec3(-0.5, -0.5, -2))*glm::rotate(r, (float)clock_.getElapsedTime().asMilliseconds() * 0.002f, glm::vec3(0.6, 0.4, 0.5));
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(r));
        shaderProgram.use();
        //vao->draw();
        fractal->draw();
        window.display();
    }

    return 0;
}