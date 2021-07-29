#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;			

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform mat4 u_Model;
			
out vec2 v_TexCoord;
			
void main()
{	
	v_TexCoord = vec2(a_TexCoord.x, a_TexCoord.y);				
	gl_Position = u_ViewProjection *  u_Transform  * vec4(a_Position, 1.0);				
}	

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;						
uniform vec3 u_Color;

in vec2 v_TexCoord;

void main()
{
				
	color = texture(u_Texture, v_TexCoord) * vec4(u_Color, 1.0);			
}