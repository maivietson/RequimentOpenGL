#version 120

uniform sampler2D texture;

varying vec2 TexCoord;

void main()
{
	gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}