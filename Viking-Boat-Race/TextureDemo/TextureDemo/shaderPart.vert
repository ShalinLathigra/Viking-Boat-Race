#version 130

// Vertex buffer
in vec2 vertex;
in vec2 dir;
in float t;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 x;
uniform float time;
uniform float intensity;	//speed, duration

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out float toBlack;


void main()
{
	vec4 ppos;
	float acttime;
	float speed = intensity;

	//float gravity = -9.8;

	acttime = mod(time + t*5, .5);
	//acttime = mod(time,10);

	//ppos = vec4(vertex.x+dir.x*acttime*speed , vertex.y+dir.y*acttime*speed + 0.5*gravity*acttime*acttime, 0.0, 1.0);

	ppos = vec4(vertex.x+dir.x*acttime*speed , vertex.y+dir.y*acttime*speed, 0.0, 1.0);
    gl_Position = x*ppos;
    
    color_interp = vec4(uv,0.5, 1.0);
    uv_interp = uv;
	//toBlack = 0;

	toBlack = (.5 - acttime) / .5;
}