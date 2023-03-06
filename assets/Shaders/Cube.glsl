#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 fUv;

uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    fUv = aUv;
    gl_Position = u_proj * u_view * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core

in vec2 fUv;

out vec4 FragColor;

uniform sampler2D samp;

void main()
{
    vec4 samp = texture(samp, fUv);
    FragColor = samp;
}
