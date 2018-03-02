// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

out vec4 FragColor;

uniform sampler2D onetex;

void main()
{
	vec4 color = texture2D(onetex, uv_interp);
	FragColor = vec4(color.r,color.g,color.b,color.a);
    if(FragColor.a < 0.05)
	{
		discard;
//		gl_FragDepth = 10000;
//		FragColor = vec4(color.r,color.g,color.b,0.5);
	}
//	 FragColor = color_interp;
}