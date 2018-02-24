#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D image;
//uniform vec3 spriteColor;
uniform vec3[3] lightSrc;
uniform vec2[3] lightDir;
uniform vec2[3] lightPos;

void main()
{    
    color = texture(image, texCoords);

	bool illuminated;
	for(int i = 0; i < 3; i++)
	{
		//Distance of the current pixel from the light position
		float distSpot = distance(gl_FragCoord.xy, lightPos[i].xy);
    
		float angleDiff = degrees(acos(dot(normalize(gl_FragCoord.xy - lightPos[i].xy), lightDir[i].xy)));
		if(distSpot < 150.0 && angleDiff < 30 && angleDiff > -30)
		{
			color = color + vec4(0.0, 0.0, 0.75, 0.0);
		}

		//color = color * (1.0 - dist / lightSrc[i].z);
		if(!illuminated)
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
		color = vec4(0.0);
	}
}  