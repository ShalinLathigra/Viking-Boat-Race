#version 130

// Vertex bufferll
in vec2 vertex;
in vec2 dir;
in float t;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 x;
uniform float time;
uniform float speed;	//speed

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out vec4 colour;
out float alpha;

void main()
{
	vec4 ppos;
	float gravity = 0;
	float maxTime = 1.5;
	float acttime = mod(time + t * 10, maxTime);

	ppos = vec4(vertex.x+dir.x*acttime*speed, vertex.y+dir.y*acttime*speed + 0.5*gravity*acttime*acttime + 1.5f, 0.0, 1);
	
	gl_Position = x*ppos;
    
    color_interp = vec4(uv,0.5, 1.0);
    uv_interp = uv;

	float numShifts = 7.0;
	float timeToShift = maxTime / numShifts;
	
	vec4 white = vec4(1, 1, 1, 1);
	vec4 red = vec4(1, 0, 0, 1);
	vec4 yellow = vec4(1, 1, 0, 1);
	vec4 green = vec4(0, 1, 0, 1);
	vec4 blue = vec4(0, 0, 1, 1);
	vec4 violet = vec4(1, 0, 1, 1);
	vec4 black = vec4(0, 0, 0, 1);
	
	if (acttime < .5 * timeToShift)
	{
		colour = white;
		alpha = 1;
	} 
	else if (acttime < 2 * timeToShift){
		colour = white * (1 - (acttime - 1 * timeToShift) / timeToShift) + red * ((acttime - 1 * timeToShift) / timeToShift);
		alpha = .9;
	}
	else if (acttime < 3 * timeToShift)
	{
		colour = red * (1 - (acttime - 2 * timeToShift) / timeToShift) + yellow * ((acttime - 2 * timeToShift) / timeToShift);
		alpha = .8;
	}
	else if (acttime < 4 * timeToShift)
	{
		colour = yellow * (1 - (acttime-3.0*timeToShift)/ (timeToShift)) + green * ((acttime-3.0*timeToShift) / (timeToShift));
		alpha = .7;
	}
	else if (acttime < 5 * timeToShift)
	{
		colour = green * (1 - (acttime-4.0*timeToShift)/ (timeToShift)) + blue * ((acttime-4.0*timeToShift) / (timeToShift));
		alpha = .6;
	}
	else if (acttime < 6 * timeToShift)
	{
		colour = blue * (1 - (acttime-5.0*timeToShift)/ (timeToShift)) + violet * ((acttime-5.0*timeToShift) / (timeToShift));
		alpha = .5;
	}
	else
	{
		colour = violet * (1 - (acttime-6.0*timeToShift)/ (timeToShift));
		alpha = .4;
	}
}