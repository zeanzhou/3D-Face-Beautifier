#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

varying vec4 finalcolor;

void main()
{    
	color = vec4(1.0f);
	//color = vec4(vec3(finalcolor), 1.0f);
    //color = vec4(texture(texture_diffuse1, TexCoords));
}