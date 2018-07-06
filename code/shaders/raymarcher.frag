#version 330 core


struct Raymarcher
{
	int   iterations;
	float epsilon;
};

struct Camera
{
	mat4x3 frame;
	float  yfov;
	float  aspect;
	float  planes[2];
};


struct Light
{
	mat4x3 frame;
	float  power;
	vec3   color;
};


uniform Raymarcher raymarcher;
uniform Camera     camera;
uniform vec3       color_ambient = vec3(0, 0, 0);

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


vec3 phong(vec3 point, vec3 normal, vec2 uv)
{
	mat4x3 light_frame = camera.frame;
	float  light_power = 10.0f;
	vec3   light_color = vec3(1, 1, 1);
	float  light_dist = length(light_frame[3] - point);
	
	
	vec3  N = normal;                                // Direction normal to the contact point
	vec3  V = normalize(camera.frame[3] - point);    // Direction from the contact point to the camera
	vec3  L = normalize(light_frame[3] - point);     // Direction from the contact point to the light
	vec3  H = normalize(L + V);                      // Bisector between the view direction and the light direction
	float I = light_power / (light_dist*light_dist); // Intensity of the light at the contact point (quadratic falloff)
	
	
	vec3 color_diffuse = vec3(0);
	{
		float NdotL   = dot(N, L);
		color_diffuse = light_color * I * max(0, NdotL);
		// color_diffuse = vec3(0);
	}
	
	
	vec3 color_specular = vec3(0);
	{
		float shininess = 8.0f;
		float NdotH     = dot(N, H);
		color_specular  = light_color * I * pow(max(0, NdotH), shininess);
		// color_specular = vec3(0);
	}
	
	
	vec3 color = color_ambient + color_diffuse + color_specular;
	return color;
}


void
main()
{
	float h = 2 * tan(camera.yfov*0.5f);
	float w = h * camera.aspect;
	
	vec3 ray_pos = camera.frame[3];
	vec3 ray_dir = normalize(w * camera.frame[0] * (uv.x-0.5f) +
	                         h * camera.frame[1] * (uv.y-0.5f) -
	                             camera.frame[2]);
	
	float total_distance = 0.0f;
	bool  did_hit        = false;
	for(int i = 0; i < raymarcher.iterations; ++i)
	{
		float closest_distance = scene(ray_pos);
		total_distance += closest_distance;
		ray_pos        += closest_distance * ray_dir;
		
		if(camera.planes[1] < total_distance)
		{
			break;
		}
		
		if(closest_distance < raymarcher.epsilon)
		{
			did_hit = true;
			break;
		}
	}
	
	
	result = vec4(0, 0, 0, 0);
	if(did_hit)
	{
		float e = raymarcher.epsilon;
		float nx = scene(ray_pos + vec3(e, 0, 0)) - scene(ray_pos - vec3(e, 0, 0));
		float ny = scene(ray_pos + vec3(0, e, 0)) - scene(ray_pos - vec3(0, e, 0));
		float nz = scene(ray_pos + vec3(0, 0, e)) - scene(ray_pos - vec3(0, 0, e));
		
		vec3 point  = ray_pos;
		vec3 normal = normalize(vec3(nx, ny, nz));
		vec2 uv     = vec2(0, 0);
		
		vec3 color = phong(point, normal, uv);
		color      = clamp(color, 0.0f, 1.0f);
		result = vec4(color, 1);
	}
	// else
	// {
	// 	discard;
	// }
}
