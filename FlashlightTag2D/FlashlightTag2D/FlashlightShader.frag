#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D image;
uniform vec2 lightDir;
uniform vec2 lightPos;

void main()
{    
    color = texture(image, texCoords);

	//Distance of the current pixel from the light position
	float distSpot = distance(gl_FragCoord.xy, lightPos.xy);
    
	float angleDiff = degrees(acos(dot(normalize(gl_FragCoord.xy - lightPos.xy), lightDir.xy)));
	if(distSpot < 150.0 && angleDiff < 30 && angleDiff > -30)
	{
		color = color + vec4(0.75, 0.75, 0.75, 0.0);
	}
}  