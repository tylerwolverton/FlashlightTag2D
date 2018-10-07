#version 330 core
in vec2 texCoords;
out vec4 color;

#define MAX_NUM_LIGHTS 10

uniform sampler2D image;
uniform int timeElapsed;
uniform vec3[MAX_NUM_LIGHTS] flashingLightSrc;
uniform vec3[MAX_NUM_LIGHTS] flashingLightColor;

void main()
{    
    color = texture(image, texCoords);

	for(int i = 0; i < MAX_NUM_LIGHTS; i++)
	{
		// Check if this is the special last light
		if(flashingLightSrc[i].x == -901.0
			&& flashingLightSrc[i].y == -901.0 
			&& flashingLightSrc[i].z == -901.0)
		{
			break;
		}

		float dist = distance(gl_FragCoord.xy, flashingLightSrc[i].xy);
		if(dist < flashingLightSrc[i].z)
		{
			color.xyz = clamp(sin(timeElapsed), 0.7, 1) *  flashingLightColor[i];
		}
	}
}  