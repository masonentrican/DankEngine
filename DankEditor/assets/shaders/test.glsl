#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = u_ViewProjection * model * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform Material material;


void main()
{    

    vec3 ambient = material.ambient;
    vec3 diffuse = material.diffuse;
    vec3 specular = material.specular;
    vec3 objectColor = vec3(1.0, 0.5, 0.31);
    vec3 result = diffuse;
    FragColor = vec4(result, 1.0);
   //FragColor = texture(texture_diffuse1, TexCoords);
    
}