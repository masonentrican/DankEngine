#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in float a_TexturesLoaded;
layout (location = 5) in vec3 a_Color;
layout (location = 6) in vec4 a_Model1;
layout (location = 7) in vec4 a_Model2;
layout (location = 8) in vec4 a_Model3;
layout (location = 9) in vec4 a_Model4;

out vec2 TexCoords;
out float v_TexIndex;
out float TexturesLoaded;
out vec3 Normal;
out vec3 FragPos;
out vec3 Color;

//uniform mat4 model;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    mat4 model = mat4(a_Model1, a_Model2, a_Model3, a_Model4);
    TexCoords = a_TexCoords;
    gl_Position = u_ViewProjection * model * vec4(a_Position, 1.0f);
    FragPos = vec3(model * vec4(a_Position, 1.0));
    Normal = mat3(transpose(inverse(model))) * a_Normal;
    //Normal = a_Normal;
    Color = a_Color;
    v_TexIndex = a_TexIndex;
    TexturesLoaded = a_TexturesLoaded;
  
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
in float v_TexIndex;
in float TexturesLoaded;
in vec3 Normal;
in vec3 FragPos;
in vec3 Color;


uniform sampler2D u_Textures[32];
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
    if(int(TexturesLoaded) == 1) {
    //ambient
    //vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 ambient = light.ambient * vec3(texture(u_Textures[int(v_TexIndex + 0.1) - 1], TexCoords));
    //diffuse
    vec3 norm = normalize(Normal);
    
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Textures[int(v_TexIndex + 0.1) - 1], TexCoords));

       //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * texture(u_Textures[int(v_TexIndex + 0.1)], TexCoords).rgb;
    //vec3 specular = light.specular * (spec * material.specular); 

    vec3 result = ambient + diffuse + specular; 
    FragColor = vec4(result, 1.0);
   //FragColor = texture(texture_diffuse1, TexCoords);

    }
    else if(TexturesLoaded == int(0)){
    
    //ambient
    vec3 ambient = light.ambient * Color;

    //diffuse
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * Color);
       //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   
    vec3 specular = light.specular * (spec * vec3(1.0)); 

    vec3 result = ambient + diffuse + specular; 
    FragColor = vec4(result, 1.0);
  
    }
 
   
    
}