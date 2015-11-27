#version 400

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 colour_interp;
in vec3 light_pos;
in vec3 light2_pos;
in vec2 uv_interp;


// Attributes passed with the material file
uniform vec4 outline_colour;
uniform vec4 ambient_colour;
uniform vec4 diffuse_colour;
uniform vec4 specular_colour;
uniform float phong_exponent;
uniform sampler2D diffuse_map;


void main() 
{
    // Blinn–Phong shading

    vec3 N, // Interpolated normal for fragment
	     L, // Light-source direction
		 L2, //2nd Light-source
		 V, // View direction
		 R,
		 R2,
		 H; // Half-way vector

	

	// Compute Lambertian lighting Id
    N = normalize(normal_interp);

	L = (light_pos - position_interp);
	L = normalize(L);

	L2 = (light2_pos - position_interp);
	L2 = normalize(L2);


	float Id = max(dot(N, L), 0.0);
	float Id2 = max(dot(N, L2), 0.0);
	
	// Compute specular term for Blinn–Phong shading
	// Initially: V = (eye_position - position_interp);
	V = - position_interp; // Eye position is (0, 0, 0) in view coordinates
    V = normalize(V);

    H = 0.5*(V + L); // Halfway vector
    H = normalize(H);

	R =  -1*L + 2*(max(dot(L,N) , 0.0))*N ;
	R = normalize(R);

	R2 =  -1*L2 + 2*(max(dot(L2,N) , 0.0))*N ;
	R2 = normalize(R2);

    float spec_angle_cos = max(dot(V, R), 0.0);
	float Is = pow(spec_angle_cos, phong_exponent);

	float spec_angle_cos2 = max(dot(V, R2), 0.0);
	float Is2 = pow(spec_angle_cos2, phong_exponent);

	if(Is2 < 0.05){
		Is2 = 0.05;
	}else{
		Is2 = 0.15;
	}
	if(Is < 0.05){
		Is = 0.05;
	}else{
		Is = 0.15;
	}
	if(Id2 < 0.05){
		Id2 = 0.05;
	}else{
		Id2 = 0.15;
	}
	if(Id < 0.15){
		Id = 0.15;
	}else{
		Id = 0.15;
	}
	    
	vec4 pixel = texture(diffuse_map, uv_interp);

	// Assign light to the fragment

	gl_FragColor = (ambient_colour + Id*diffuse_colour+ Id2*diffuse_colour)*pixel + Is*specular_colour + Is2*specular_colour;


		
	
	// For debug, we can display the different values
	//gl_FragColor = ambient_colour;
	//gl_FragColor = diffuse_colour;
	//gl_FragColor = specular_colour;
	//gl_FragColor = colour_interp;
	//gl_FragColor = vec4(N.xyz, 1.0);
	//gl_FragColor = vec4(L.xyz, 1.0);
	//gl_FragColor = vec4(V.xyz, 1.0);
}