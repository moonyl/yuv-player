#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

//uniform sampler2D ourTexture;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;

void main()
{
	//vec3 yuv;
	//vec3 rgb;
	//yuv.x = texture(texture3, TexCoord).r;
	//yuv.y = texture(texture4, TexCoord).r - 0.5;
	//yuv.z = texture(texture5, TexCoord).r - 0.5;
	//rgb = mat3( 1,       1,         1,
    //          0,       -0.39465,  2.03211,
    //          1.13983, -0.58060,  0) * yuv;   

	mediump float r,g,b,y,u,v;
	y=texture(texture3, TexCoord).r;
	u=texture(texture4, TexCoord).r;
	v=texture(texture5, TexCoord).r;

	y=1.1643*(y-0.0625);
	u=u-0.5;
	v=v-0.5;
	
	r=y+1.5958*v ;
	g=y-0.39173*u-0.81290*v;
	b=y+2.017*u;
	FragColor = vec4(r, g, b, 1.0);
	//FragColor = vec4(u, 0, 0, 1.0);

	//FragColor = vec4(rgb, 1);
    //FragColor = texture(ourTexture, TexCoord);
	//FragColor = texture(texture1, TexCoord);
	//FragColor = texture(texture3, TexCoord);
	//FragColor = mix(texture(ourTexture, TexCoord), texture(texture2, TexCoord), 0.5);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
}
