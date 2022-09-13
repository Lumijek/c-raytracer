#version 410 core
out vec4 FragColor; 

in vec2 TexCoord;

uniform sampler2D texture1;

uniform float alpha;
void main()
{
    FragColor = texture(texture1, TexCoord);
}