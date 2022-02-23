#version 460 core
out vec4 FragColor;  
smooth in vec3 ourColor;
  
void main()
{
    FragColor = vec4(ourColor, 1.0);
}