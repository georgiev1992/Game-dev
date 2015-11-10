#version 400

// Passed from the vertex shader
in vec2 uv;

// Passed from outside
uniform float time;
uniform sampler2D diffuse_map;

void main() 
{
	// wavering
	vec2 pos = uv;
    pos.x = pos.x + 0.05*(sin(time/10.0+8.0*pos.y));

	vec4 pixel = texture(diffuse_map, pos);
    
    gl_FragColor = pixel;
}