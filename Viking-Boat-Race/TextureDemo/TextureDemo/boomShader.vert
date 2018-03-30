#version 130

// Vertex buffer
in vec2 vertex;
in vec2 dir;
in float t;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 x;
uniform float elapsedTime;
uniform float speed;	//speed

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out vec4 colour;

void main()
{
	vec4 ppos;
	float acttime;
	float gravity = -9.8f * 5.0f;
	float maxTime = 2.0f;

	ppos = vec4(vertex.x+dir.x*elapsedTime*speed , vertex.y+dir.y*elapsedTime*speed + 0.5*gravity*elapsedTime*elapsedTime, 0.0, 1.0);
	
	gl_Position = x*ppos;
    
    color_interp = vec4(uv,0.5, 1.0);
    uv_interp = uv;

	
	float timeToRed = .6;
	vec4 yellow = vec4(1, 1, 0, 1);
	vec4 red = vec4(1, 0, 0, 1);
	vec4 black = vec4(0, 0, 0, 1);
	if (elapsedTime < timeToRed)
	{
		colour = yellow * (1 - elapsedTime / timeToRed) + red * (elapsedTime / timeToRed);
	}
	else if (elapsedTime < 2.0f * maxTime)
	{
		colour = red * (1 - (elapsedTime-timeToRed)/ (maxTime-timeToRed)) + black * ((elapsedTime-timeToRed) / (maxTime-timeToRed));
	} else {
		colour = vec4(0,0,0,0);
	}
}