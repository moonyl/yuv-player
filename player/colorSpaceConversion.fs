#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureY;
uniform sampler2D textureU;
uniform sampler2D textureV;


void main()
{
	mediump float r,g,b,y,u,v;
	y=texture(textureY, TexCoord).r;
	u=texture(textureU, TexCoord).r;
	v=texture(textureV, TexCoord).r;

	y=1.1643*(y-0.0625);
	u=u-0.5;
	v=v-0.5;

	r=y+1.5958*v ;
	g=y-0.39173*u-0.81290*v;
	b=y+2.017*u;

	FragColor = vec4(r, g, b, 1.0);

}
