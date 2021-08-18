#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    TexCoords = a_TexCoords;
    gl_Position = u_ViewProjection * model * vec4(a_Position, 1.0);
    FragPos = vec3(model * vec4(a_Position, 1.0));
    Normal = mat3(transpose(inverse(model))) * a_Normal;
  
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

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform int diffuseLoaded;
uniform int specularLoaded;
uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{    
    if(diffuseLoaded == 1) {
    
    //ambient
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

    //diffuse
    vec3 norm = normalize(Normal);
    
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

       //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;
    //vec3 specular = light.specular * (spec * material.specular); 

    vec3 result = ambient + diffuse + specular; 
    FragColor = vec4(result, 1.0);
   //FragColor = texture(texture_diffuse1, TexCoords);

    }
    else {
    
    //ambient
    vec3 ambient = light.ambient * material.ambient;

    //diffuse
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
       //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   
    vec3 specular = light.specular * (spec * material.specular); 

    vec3 result = ambient + diffuse + specular; 
    FragColor = vec4(result, 1.0);
   //FragColor = texture(texture_diffuse1, TexCoords);
    }
 
   
    
}