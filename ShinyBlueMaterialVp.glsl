#version 400

// Attributes passed automatically by OGRE
in vec3 vertex;
in vec3 normal;
in vec4 colour;
in vec2 uv0;

// Attributes passed with the material file
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;
uniform mat4 normal_mat;
uniform vec3 light_position;
uniform vec3 light2_position;

// Attributes forwarded to the fragment shader
out vec3 position_interp;
out vec3 normal_interp;
out vec4 colour_interp;
out vec2 uv_interp;
out vec3 light_pos;
out vec3 light2_pos;


void main()
{
    gl_Position = projection_mat * view_mat * world_mat * vec4(vertex, 1.0);

    position_interp = vec3(view_mat * world_mat * vec4(vertex, 1.0));
	
	normal_interp = vec3(normal_mat * vec4(normal, 0.0));

	colour_interp = colour;

    light_pos = vec3(view_mat * vec4(light_position, 1.0));

	uv_interp = uv0;

	light2_pos = vec3(view_mat * vec4(light2_position, 1.0));
}