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


#define PI 3.14159265
#define MAX_DISTANCE 100
#define VANISHING_RATEO 0.8

//------------------- signed distance functions --------------------------------
#define FLOOR(p) p.y

float sphere(const vec3 point, const float radius)
{
	return length(point) - radius;
}


float box(const vec3 point, const vec3 size)
{
	return length(max(abs(point) - size, 0.0));
}

float cylinder_inf( const vec3 point , const float radius)
{
  return length(point.xz) - radius;
}

float cylinder( const vec3 point , const float height, const float radius)
{
	float dist = length(point.xz) - radius;
	dist = max(dist, abs(point.y) - height);
	return dist;
}

float torus( vec3 point, const float height, const float radius)
{
  vec2 q = vec2(length(point.xz) - radius, point.y);
  return length(q)- height;
}


float scene(vec3 point);

//----------------------------- Ray Marching -----------------------------------
bool ray_cast(inout vec3 ray_pos, const vec3 ray_dir, inout float total_distance)
{
	total_distance = 0.0f;
	bool  did_hit  = false;

	for(int i = 0; total_distance <= MAX_DISTANCE; ++i)
	{
		float closest_distance = scene(ray_pos);
		total_distance += closest_distance;
		ray_pos        += closest_distance*  0.5f * ray_dir;
		
		if(camera_planes[1] < total_distance)
		{
			break;
		}
		
		if(closest_distance < raymarch_epsilon)
		{
			if(total_distance >= camera_planes[0])
			{
				did_hit = true;
			}
			break;
		}
	}
	return did_hit;
}

void
main()
{
	float h = 2.0f * tan(1*0.5f);
	float w =    h * camera_aspect;
	
	vec3 direction = w * camera_frame[0] * (uv.x-0.5f) +
	                 h * camera_frame[1] * (uv.y-0.5f) -
	                 camera_frame[2];
	vec3 ray_dir = normalize(direction);
	vec3 ray_pos = camera_frame[3];
	

	float total_distance = 0.0f;
	bool did_hit = ray_cast(ray_pos, ray_dir, total_distance);
	
	vec3 background = vec3(0.185, 0.19, 0.2)*(ray_dir.y*2+2);
	result.rgb = background;
	if(did_hit)
	{
		float e = raymarch_epsilon;
		vec3 normal = normalize(vec3(
			scene(ray_pos + vec3(e, 0, 0)) - scene(ray_pos - vec3(e, 0, 0)),
			scene(ray_pos + vec3(0, e, 0)) - scene(ray_pos - vec3(0, e, 0)),
			scene(ray_pos + vec3(0, 0, e)) - scene(ray_pos - vec3(0, 0, e))
		));
		
		// Proper lighting
		vec3 sun_dir         = normalize(vec3(0,1,-0.5));
		float color_diffuse  = max(0.1, dot(sun_dir, normal)+0.5);
		//float color_specular = pow(color_diffuse, 32.0f);
		
		// float color_final = max(0.0f, color_diffuse + color_specular);
		float color = clamp(color_diffuse, 0.0f, 1.0f);
		result.rgb = vec3(color, color, color);

		// graceful distance vanishing
		if (total_distance < MAX_DISTANCE && 
			total_distance > MAX_DISTANCE*VANISHING_RATEO)
		{
			float alpha = (total_distance - MAX_DISTANCE*VANISHING_RATEO)/(MAX_DISTANCE*(1-VANISHING_RATEO));
			result.rgb = result.rgb*(1-alpha) + alpha*background;
		}

		// floor color changing
		if (ray_pos.y + 1 <= 0.001f) 
		{
			vec2 tmp = mod(ray_pos.xz, 1.0f);
			if ((tmp.x >= 0.45f && tmp.x <= 0.55f)||
				(tmp.y >= 0.45f && tmp.y <= 0.55f))
			{
				result.rgb *= 0.5f;
			}
			else
			{
				result.rgb *= 0.85f;
			}
		}
	}
}




//------------------------------------------------------------------------------
// SCENE CONSTRUCTION
//------------------------------------------------------------------------------


//---------------------------- combine operations ------------------------------
#define UNION(a,b) min(a,b)
#define INTERSECT(a,b) max(a,b)
#define SUBTRACT(a,b) max(-a,b)

float UNION_SMOOTH( float d1, float d2, float k )
{
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h);
}

float UNION_CHAMFER(float d1, float d2, float radius)
{
	return min(min(d1, d2), (d1 - radius + d2)*sqrt(0.5));
}




//------------------------- space folding operations ---------------------------
vec2 pR(vec2 point, float angle)
{
	return cos(angle)*point + sin(angle)*vec2(point.y, -point.x);
}


vec3 repeat_space(const vec3 point, const vec3 rep_size)
{
	return  mod(point + rep_size/2, rep_size) - rep_size/2;
}


// repeat object using polar coordinates
vec2 repeat_circular(const vec2 p, const float repetitions)
{
	float angle = 2*PI/repetitions;
	float a = atan(p.y, p.x) + angle/2;
	float radius = length(p);
	a = mod(a,angle) - angle/2;
	return vec2(cos(a), sin(a))*radius;
}

// ------------------------------- structures ----------------------------------

// pillars
float 
pillar(vec3 point, float width,
                   float height,
                   float depth)
{
	vec3 lpoint = point;

	float pil_width  = width*25/30;
	float pil_height = height*9/10;
	float pil_depth  = depth*25/30;

	vec3 PIL_BORDER = vec3(pil_width/2,
	                       pil_height/2,
	                       pil_depth/2);

	vec3 FRAME_BORDER = vec3(width/2,
                             (height-pil_height)/2,
                             depth/2);

	lpoint.y          = point.y + FRAME_BORDER.y;
	float dst_pillar  = box(lpoint,   PIL_BORDER);

	lpoint.y           = point.y - height/2 + FRAME_BORDER.y;
	float dst_frame   = box(lpoint,   FRAME_BORDER);

	return UNION_CHAMFER(dst_pillar,
	                     dst_frame,
	                     (width-pil_width)/2);
}

float
arch(vec3 point, float width,
                 float height,
                 float depth,
                 float radius)
{
	vec3 lpoint = point;

	float body_depth = depth*29/30;

	// get distance from main body of arch
	float d_body = box(lpoint, vec3(width/2,
	                               height/2,
	                               body_depth/2));

	float d_bound = box(lpoint,vec3(width/2,
	                               height/2,
	                               depth/2));

	// rotate space of 90 degree with respect of x
	lpoint.y = point.z;
	lpoint.z = point.y + height/2;
	lpoint.x = point.x + width/2;

	lpoint = repeat_space(lpoint, vec3(width,0,0));

	// produce frame and arch
	float d_frame = cylinder_inf(lpoint, radius);
	float d_arch  = cylinder_inf(lpoint, radius - radius/20);

	return SUBTRACT(
	                d_arch,
	                INTERSECT(
	                          d_bound,
	                          UNION(d_frame, d_body)
	                         )
	               );
}





float column(const vec3 point, const float width, const float height)
{
	vec3 COLUMN_BASE = vec3(width/2,   // base width
	                        height/65, // base height
	                        width/2);  // base depth

	vec2 COLUMN_SHAFT = vec2(width/3,             // shaft radius
	                         height/2 - COLUMN_BASE.y); // shaft height

	vec3 lpoint = point;
	// get distance from column shaft
	float d_shaft = cylinder(lpoint,
	                         COLUMN_SHAFT.y,
	                         COLUMN_SHAFT.x);

	// move space to actual column base
	lpoint.y += COLUMN_SHAFT.y;
	float d_base_body = box(lpoint, COLUMN_BASE);

	// move space to the column top
	lpoint = point;
	lpoint.y -= COLUMN_SHAFT.y;
	float d_top  = box(lpoint, COLUMN_BASE);

	return UNION(UNION_CHAMFER(d_base_body, d_shaft, width/6), d_top);
}


float arch_complex(const vec3 point, float width,
                                     float height,
                                     float depth)
{
	vec3 lpoint = point;

	float pillar_width  = depth*0.8;
	float pillar_height = height*0.6;

	float arch_width  = width;
	float arch_height = height - pillar_height;
	float arch_depth  = depth*0.8;
	float arch_radius = width*0.4;

	lpoint.y = point.y + height/2 - pillar_height/2;
	lpoint.z = point.z + depth/2  - arch_depth/2;

	// compute distance from pillar
	float d_pillar = pillar(lpoint, pillar_width,
	                                pillar_height,
	                                pillar_width);

	lpoint.y = point.y - height/2 + arch_height/2;

	// get distance of arch
	float d_arch = arch(lpoint, arch_width,
	                            arch_height,
	                            arch_depth,
	                            arch_radius);

	lpoint = point;
	lpoint.z = point.z - arch_depth/2;
	float d_col = column(lpoint, depth - arch_depth, height);
	// return union between column and arch
	return UNION(
	             UNION(d_arch,
	                   d_pillar),
	             d_col
	             );
}


vec2 bend( vec2 p )
{
	float a = -0.1*p.x;

    float c = cos(a);
    float s = sin(a);

    mat2  m = mat2(c,-s,s,c);
    vec2  q = m*p;
    return q;
}

float unit(vec3 point, float width,
                       float height,
                       float depth)
{
	vec3 lpoint = point;
	lpoint.y += 0.1;
	float d_arch = arch_complex(lpoint, width, height*0.8, depth);
	
	lpoint.y = lpoint.y - 0.5;
	float d_floor1 = box(lpoint, vec3 (width/2, height*0.1, depth/2));
	lpoint.y = point.y + 0.6;
	float d_floor2 = box(lpoint, vec3 (width/2, height*0.1, depth/2));


	return UNION(UNION(d_arch, d_floor1),d_floor2);
}


float scene(vec3 point)
{
	// move space down (because camera has position 0)
	point.y += 1;

	// get floor distance
	float d_floor = FLOOR(point);

	// get distance from scene objects
	point.y -= 0.5f;

	point.zx = repeat_circular(point.zx, 57);
	point = repeat_space(point, vec3(0,1,0));

	point.z -= 7;
	point.xz = bend(point.xz);
	point.z -= 1;

	float d_obj  = box(point, vec3(0.4, 0.5, 0.20));
	if (d_obj <= raymarch_epsilon)
	{
		d_obj = unit(point, 0.8, 1, 0.40);
	}

	return UNION(d_floor, d_obj);
}

