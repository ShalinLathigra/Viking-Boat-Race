// Source code of vertex shader
#version 130

// Vertex buffer
in vec2 vertex;
in vec3 color;
in vec2 uv;

// Uniform (global) buffer
uniform mat4 x;

// Attributes forwarded to the fragment shader
out vec4 color_interp;
out vec2 uv_interp;

uniform int frameindex;
uniform int currentAnim;
uniform float spriteHeight;
uniform float spriteWidth;
uniform vec2 position;
uniform int direction;


void main()
{
	vec4 t;
	t = vec4(vertex, 0.0, 1.0);
    gl_Position = x*t;
	
    color_interp = vec4(color, 1.0);

	float tw = 1.0f/6.0f ; //float(spriteWidth) / texWidth; 
	float th = 1.0f/4.0f ; //float(spriteHeight) / texHeight;
	int numPerRow = 6; //texWidth / spriteWidth;

	float tx = float(frameindex % numPerRow) * tw;
	float ty = float(frameindex / numPerRow ) * th + th * (currentAnim - 1);
	if (currentAnim > 4){
		ty = float(frameindex / numPerRow ) * th + th * ((currentAnim - 4) - 1);
	}

	// adjust vertex position according to sprite width and height and sprite position
    gl_Position.x = (gl_Position.x * spriteWidth)  / spriteWidth  + position.x;
    gl_Position.y = (gl_Position.y * spriteHeight) / spriteHeight + position.y;

    // (texCoordX  * spriteWidth / textureWidth) + texSourceX

	uv_interp.y = (uv.y * th) + ty ; 

	uv_interp.x = (uv.x *  tw) * direction + tx;
}