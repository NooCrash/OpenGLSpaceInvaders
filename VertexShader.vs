#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0 
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

// output a color to the fragment shader
out vec3 ourColor;
out vec4 pos; 

uniform float offset;

void main(){
		
	gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
	pos = gl_Position;

	// set ourColor to the input color we got from the vertex data
	ourColor = aColor; 
}