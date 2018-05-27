#version 330 core

uniform int   raymarch_iterations;
uniform float raymarch_epsilon;

// NOTE: GLSL Uses row-major matrices?
uniform mat4x3 camera_frame;
uniform float  camera_yfov;
uniform float  camera_aspect;
uniform float  camera_focus;
uniform float  camera_planes[2];


in  vec2 uv;
out vec4 result;


float sphere(vec3 point, float radius)
{
	return length(point) - radius;
}


float box(vec3 point, vec3 size)
{
	return length(max(abs(point) - size, 0.0));
}


float scene(vec3 point)
{
	// return box(point, vec3(2.0f, 1.0f, 1.0f));
	return sphere(point, 1.0f);
}


void
main()
{
	float d = camera_focus;
	float h = d * 2.0f * tan(camera_yfov*0.5f);
	float w = d *    h * camera_aspect;
	
	vec3 direction = w * camera_frame[0] * (uv.x-0.5f) +
	                 h * camera_frame[1] * (uv.y-0.5f) +
	                 d * camera_frame[2];
	vec3 ray_dir = normalize(direction);
	vec3 ray_pos = camera_frame[3];
	
	float total_distance = 0.0f;
	bool  did_hit        = false;
	for(int i = 0; i < raymarch_iterations; ++i)
	{
		float closest_distance = scene(ray_pos);
		total_distance += closest_distance;
		ray_pos        += closest_distance * ray_dir;
		
		if(camera_planes[1] < total_distance)
		{
			break;
		}
		
		if(closest_distance < raymarch_epsilon)
		{
			did_hit = true;
			break;
		}
	}
	
	
	result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if(did_hit)
	{
		float e = raymarch_epsilon;
		vec3 normal = normalize(vec3(
			scene(ray_pos + vec3(e, 0, 0)) - scene(ray_pos - vec3(e, 0, 0)),
			scene(ray_pos + vec3(0, e, 0)) - scene(ray_pos - vec3(0, e, 0)),
			scene(ray_pos + vec3(0, 0, e)) - scene(ray_pos - vec3(0, 0, e))
		));
		
		// TODO: Proper lighting
		float color_diffuse  = max(0, dot(-ray_dir, normal));
		float color_specular = pow(color_diffuse, 32.0f);
		
		// float color_final = max(0.0f, color_diffuse + color_specular);
		float color = clamp(color_diffuse, 0.0f, 1.0f);
		result.rgb = vec3(color, color, color);
	}
}
