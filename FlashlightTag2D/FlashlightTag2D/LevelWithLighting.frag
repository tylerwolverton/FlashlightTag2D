#version 330 core
in vec2 texCoords;
out vec4 color;

#define MAX_NUM_LIGHTS 20

//uniform int numLights;
uniform sampler2D image;
//uniform vec3 spriteColor;
uniform vec3[MAX_NUM_LIGHTS] lightSrc;
uniform vec2[MAX_NUM_LIGHTS] lightDir;
uniform vec2[MAX_NUM_LIGHTS] lightPos;

void main()
{    
    color = texture(image, texCoords);

	bool illuminated = false;
	for(int i = 0; i < MAX_NUM_LIGHTS; i++)
	{
		// Check if this is the special last light
		if(lightSrc[i].x == -901.0f 
			&& lightSrc[i].y == -901.0f 
			&& lightSrc[i].z == -901.0f)
		{
			break;
		}

		//Distance of the current pixel from the light position
		float distSpot = distance(gl_FragCoord.xy, lightPos[i].xy);
    
		float angleDiff = degrees(acos(dot(normalize(gl_FragCoord.xy - lightPos[i].xy), lightDir[i].xy)));
		if(distSpot < 250.0 && angleDiff < 30 && angleDiff > -30)
		{
			color = color + vec4(0.75, 0.75, 0.75, 0.0);
			illuminated = true;
		}
		else
		{
			float dist = distance(gl_FragCoord.xy, lightSrc[i].xy);
			if(dist < lightSrc[i].z)
			{
				illuminated = true;
			}
		}
	}

	if(!illuminated)
	{
		color = color - vec4(0.5, 0.5, 0.5, 0.0);
	}
}