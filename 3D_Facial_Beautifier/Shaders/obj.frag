#version 330 core

out vec4 color;

varying vec3 finalcolor;

void main()
{    
	//color = vec4(1.0f);
	color = vec4(vec3(finalcolor), 1.0f);
    //color = vec4(texture(texture_diffuse1, TexCoords));
}