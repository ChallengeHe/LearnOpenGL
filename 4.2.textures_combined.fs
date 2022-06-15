#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

// ÎÆÀí²ÉÑùÆ÷
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // texture1: 80%, texture2: 20%
    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}