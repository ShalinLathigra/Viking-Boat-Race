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
uniform int isTrail;

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;
out vec4 colour;

void main()
{
	vec4 ppos;
	float maxTime = 1.5;
	float acttime = mod(time + t * 10, maxTime);

	ppos = vec4(vertex.x+dir.x*acttime*speed, vertex.y+dir.y*acttime*speed, 0.0, 1);
	
	gl_Position = x*ppos;
    
    color_interp = vec4(uv,0.5, 1.0);
    uv_interp = uv;

	float numShifts = 3.0;
	float timeToShift = maxTime / numShifts;
	
	//vec4 red = vec4(1, 0, 0, 1);
	//vec4 yellow = vec4(1, 1, 0, .75);
	//vec4 green = vec4(0, 1, 0, .5);
	//vec4 blue = vec4(0, 0, 1, .3);
	//vec4 violet = vec4(1, 0, 1, .2);
	
	vec4 white = vec4(.9, .9, .9, 1);
	vec4 lGray = vec4(.7, .7, .7, 1);
	vec4 black = vec4(0, 0, 0, 1);
	
	if (isTrail == 1)
	{
		if (acttime <  timeToShift)
		{
			colour = white;
		}
		else if (acttime < 2 * timeToShift)
		{
			colour = white * (1 - (acttime - 1.0 * timeToShift) / timeToShift) + lGray * ((acttime - 1.0 * timeToShift) / timeToShift);
		}
		else if (acttime < 3 * timeToShift)
		{
			colour = lGray * (1 - (acttime - 2.0 * timeToShift) / timeToShift) + black * ((acttime - 2.0 * timeToShift) / timeToShift);
		}
	} 
	else 
	{
		colour = vec4(.9,.9,1,1);
	}
}