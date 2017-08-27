#version 330 core
out vec4 FragColor;
  
#in vec3 ourColor;
#in vec2 TexCoord;

#uniform sampler2D ourTexture;

void main()
{
    FragColor = (.2f, .3f, .5f, 1.0f);
	#FragColor = texture(ourTexture, TexCoord);
}