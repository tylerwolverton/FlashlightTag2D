#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D image;
//uniform vec3 spriteColor;
uniform vec3 lightSrc;
uniform vec2 lightDir;
uniform vec2 lightPos;

void main()
{    
    //vec4(spriteColor, 1.0) * 
    color = texture(image, texCoords);

	//Distance of the current pixel from the light position
    float dist = distance(gl_FragCoord.xy, lightSrc.xy);
    
	float angleDiff = degrees(acos(dot(normalize(lightPos.xy - gl_FragCoord.xy), lightDir.xy)));
	if(angleDiff < 30 && angleDiff > -30)
	{
		color = color + vec4(0.0, 0.0, 0.75, 0.0);
	}
	 
	//Check if this pixel is outside the range
    //if(lightSrc.z < dist)
	//{
		color = color * (1.0 - dist / lightSrc.z);
		//color = vec4(0.0, 0.0, 0.5, 0.0);
    //}
}  