#define ARRAY_COUNT(a) (sizeof(a) / sizeof(*(a)))
#define RADIANS(deg) ( (deg) * (3.1415f / 180.0f) )


struct
{
  GLenum      type;
  const char* name;
} glsl_type_set[] = {
	GL_INVALID_ENUM,                              "invalid",
	GL_FLOAT,                                     "float",
	GL_FLOAT_VEC2,                                "vec2",
	GL_FLOAT_VEC3,                                "vec3",
	GL_FLOAT_VEC4,                                "vec4",
	GL_DOUBLE,                                    "double",
	GL_DOUBLE_VEC2,                               "dvec2",
	GL_DOUBLE_VEC3,                               "dvec3",
	GL_DOUBLE_VEC4,                               "dvec4",
	GL_INT,                                       "int",
	GL_INT_VEC2,                                  "ivec2",
	GL_INT_VEC3,                                  "ivec3",
	GL_INT_VEC4,                                  "ivec4",
	GL_UNSIGNED_INT,                              "unsigned int",
	GL_UNSIGNED_INT_VEC2,                         "uvec2",
	GL_UNSIGNED_INT_VEC3,                         "uvec3",
	GL_UNSIGNED_INT_VEC4,                         "uvec4",
	GL_BOOL,                                      "bool",
	GL_BOOL_VEC2,                                 "bvec2",
	GL_BOOL_VEC3,                                 "bvec3",
	GL_BOOL_VEC4,                                 "bvec4",
	GL_FLOAT_MAT2,                                "mat2",
	GL_FLOAT_MAT3,                                "mat3",
	GL_FLOAT_MAT4,                                "mat4",
	GL_FLOAT_MAT2x3,                              "mat2x3",
	GL_FLOAT_MAT2x4,                              "mat2x4",
	GL_FLOAT_MAT3x2,                              "mat3x2",
	GL_FLOAT_MAT3x4,                              "mat3x4",
	GL_FLOAT_MAT4x2,                              "mat4x2",
	GL_FLOAT_MAT4x3,                              "mat4x3",
	GL_DOUBLE_MAT2,                               "dmat2",
	GL_DOUBLE_MAT3,                               "dmat3",
	GL_DOUBLE_MAT4,                               "dmat4",
	GL_DOUBLE_MAT2x3,                             "dmat2x3",
	GL_DOUBLE_MAT2x4,                             "dmat2x4",
	GL_DOUBLE_MAT3x2,                             "dmat3x2",
	GL_DOUBLE_MAT3x4,                             "dmat3x4",
	GL_DOUBLE_MAT4x2,                             "dmat4x2",
	GL_DOUBLE_MAT4x3,                             "dmat4x3",
	GL_SAMPLER_1D,                                "sampler1D",
	GL_SAMPLER_2D,                                "sampler2D",
	GL_SAMPLER_3D,                                "sampler3D",
	GL_SAMPLER_CUBE,                              "samplerCube",
	GL_SAMPLER_1D_SHADOW,                         "sampler1DShadow",
	GL_SAMPLER_2D_SHADOW,                         "sampler2DShadow",
	GL_SAMPLER_1D_ARRAY,                          "sampler1DArray",
	GL_SAMPLER_2D_ARRAY,                          "sampler2DArray",
	GL_SAMPLER_1D_ARRAY_SHADOW,                   "sampler1DArrayShadow",
	GL_SAMPLER_2D_ARRAY_SHADOW,                   "sampler2DArrayShadow",
	GL_SAMPLER_2D_MULTISAMPLE,                    "sampler2DMS",
	GL_SAMPLER_2D_MULTISAMPLE_ARRAY,              "sampler2DMSArray",
	GL_SAMPLER_CUBE_SHADOW,                       "samplerCubeShadow",
	GL_SAMPLER_BUFFER,                            "samplerBuffer",
	GL_SAMPLER_2D_RECT,                           "sampler2DRect",
	GL_SAMPLER_2D_RECT_SHADOW,                    "sampler2DRectShadow",
	GL_INT_SAMPLER_1D,                            "isampler1D",
	GL_INT_SAMPLER_2D,                            "isampler2D",
	GL_INT_SAMPLER_3D,                            "isampler3D",
	GL_INT_SAMPLER_CUBE,                          "isamplerCube",
	GL_INT_SAMPLER_1D_ARRAY,                      "isampler1DArray",
	GL_INT_SAMPLER_2D_ARRAY,                      "isampler2DArray",
	GL_INT_SAMPLER_2D_MULTISAMPLE,                "isampler2DMS",
	GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,          "isampler2DMSArray",
	GL_INT_SAMPLER_BUFFER,                        "isamplerBuffer",
	GL_INT_SAMPLER_2D_RECT,                       "isampler2DRect",
	GL_UNSIGNED_INT_SAMPLER_1D,                   "usampler1D",
	GL_UNSIGNED_INT_SAMPLER_2D,                   "usampler2D",
	GL_UNSIGNED_INT_SAMPLER_3D,                   "usampler3D",
	GL_UNSIGNED_INT_SAMPLER_CUBE,                 "usamplerCube",
	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,             "usampler2DArray",
	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,             "usampler2DArray",
	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,       "usampler2DMS",
	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "usampler2DMSArray",
	GL_UNSIGNED_INT_SAMPLER_BUFFER,               "usamplerBuffer",
	GL_UNSIGNED_INT_SAMPLER_2D_RECT,              "usampler2DRect",
	GL_IMAGE_1D,                                  "image1D",
	GL_IMAGE_2D,                                  "image2D",
	GL_IMAGE_3D,                                  "image3D",
	GL_IMAGE_2D_RECT,                             "image2DRect",
	GL_IMAGE_CUBE,                                "imageCube",
	GL_IMAGE_BUFFER,                              "imageBuffer",
	GL_IMAGE_1D_ARRAY,                            "image1DArray",
	GL_IMAGE_2D_ARRAY,                            "image2DArray",
	GL_IMAGE_2D_MULTISAMPLE,                      "image2DMS",
	GL_IMAGE_2D_MULTISAMPLE_ARRAY,                "image2DMSArray",
	GL_INT_IMAGE_1D,                              "iimage1D",
	GL_INT_IMAGE_2D,                              "iimage2D",
	GL_INT_IMAGE_3D,                              "iimage3D",
	GL_INT_IMAGE_2D_RECT,                         "iimage2DRect",
	GL_INT_IMAGE_CUBE,                            "iimageCube",
	GL_INT_IMAGE_BUFFER,                          "iimageBuffer",
	GL_INT_IMAGE_1D_ARRAY,                        "iimage1DArray",
	GL_INT_IMAGE_2D_ARRAY,                        "iimage2DArray",
	GL_INT_IMAGE_2D_MULTISAMPLE,                  "iimage2DMS",
	GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY,            "iimage2DMSArray",
	GL_UNSIGNED_INT_IMAGE_1D,                     "uimage1D",
	GL_UNSIGNED_INT_IMAGE_2D,                     "uimage2D",
	GL_UNSIGNED_INT_IMAGE_3D,                     "uimage3D",
	GL_UNSIGNED_INT_IMAGE_2D_RECT,                "uimage2DRect",
	GL_UNSIGNED_INT_IMAGE_CUBE,                   "uimageCube",
	GL_UNSIGNED_INT_IMAGE_BUFFER,                 "uimageBuffer",
	GL_UNSIGNED_INT_IMAGE_1D_ARRAY,               "uimage1DArray",
	GL_UNSIGNED_INT_IMAGE_2D_ARRAY,               "uimage2DArray",
	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,         "uimage2DMS",
	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,   "uimage2DMSArray",
	GL_UNSIGNED_INT_ATOMIC_COUNTER,               "atomic_uint"
};


static
void
gl_print_uniforms(GLuint gl_program)
{
	GLint uniform_count;
	glGetProgramiv(gl_program, GL_ACTIVE_UNIFORMS, &uniform_count);
	
	for(GLint uniform_id = 0; uniform_id < uniform_count; uniform_id++)
	{
		GLint  size = 0;
		GLenum type = GL_INVALID_ENUM;
		GLchar name[256] = {};
		glGetActiveUniform(gl_program, uniform_id, 255, NULL, &size, &type, name);
		GLint location = glGetUniformLocation(gl_program, name);
		
		for(int type_index = 0; type_index < ARRAY_COUNT(glsl_type_set); ++type_index)
		{
			if(glsl_type_set[type_index].type == type)
			{
				if(size > 1)
					printf("Uniform %d (loc=%d):\t%20s %-20s <count: %d>\n",
					       uniform_id, location, glsl_type_set[type_index].name, name, size);
				else
					printf("Uniform %d (loc=%d):\t%20s %-20s\n",
					       uniform_id, location, glsl_type_set[type_index].name, name);
				break;
			}
		}
	}
}


static
ygl::gl_program
load_gl_program()
{
	ygl::gl_program result = {};
	try
	{
		std::string vert_shader_code = ygl::load_text("shaders/raymarcher.vert");
		std::string frag_shader_code = ygl::load_text("shaders/raymarcher.frag");
		result = ygl::make_program(vert_shader_code, frag_shader_code);
		// gl_print_uniforms(result.pid);
	}
	catch(std::runtime_error& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	return result;
}


static
void
try_reload_gl_program(ygl::gl_program* gl_program)
{
	ygl::gl_program new_gl_program = load_gl_program();
	if(new_gl_program.pid)
	{
		ygl::unbind_program(*gl_program);
		ygl::clear_program(*gl_program);
		*gl_program = new_gl_program;
		ygl::bind_program(*gl_program);
	}
}


static
void
transform_camera(ygl::frame3f& frame, const ygl::vec3f& transl, const ygl::vec3f& rotate)
{
	// Same as yocto except rotation is represented around the corresponding axis: x=pitch, y=yaw, z=roll
	auto y = ygl::vec3f{0, 1, 0};
	auto z = ygl::orthonormalize(frame.z, y);
	auto x = ygl::cross(y, z);

	frame = {ygl::frame_rot(ygl::rotation_frame(ygl::vec3f{1, 0, 0}, rotate.x)) *
	         ygl::frame_rot(frame) *
	         ygl::frame_rot(ygl::rotation_frame(ygl::vec3f{0, 1, 0}, rotate.y)),
	         frame.o + transl.x * x + transl.y * y + transl.z * z};
}


static
void
handle_camera_navigation(ygl::gl_window* window, ygl::frame3f& camera_frame)
{
	static ygl::vec2f mouse_last   = ygl::zero2f;
	
	ygl::vec2f mouse_pos    = get_mouse_posf(window);
	int        mouse_button = get_mouse_button(window);
	
	if(!ygl::get_widget_active(window))
	{
		ygl::vec3f rotate = {};
		ygl::vec3f transl = {};
		
		// Left mouse button allows to look left-right
		if(mouse_button == 1)
		{
			// Difference in x position becomes rotation around y axis
			rotate.y = (mouse_pos.x - mouse_last.x) / 100.0f;
		}
		
		if(ygl::get_key(window, GLFW_KEY_A)) transl.x += -0.1f;
		if(ygl::get_key(window, GLFW_KEY_D)) transl.x += +0.1f;
		if(ygl::get_key(window, GLFW_KEY_Q)) transl.y += -0.1f;
		if(ygl::get_key(window, GLFW_KEY_E)) transl.y += +0.1f;
		if(ygl::get_key(window, GLFW_KEY_W)) transl.z += -0.1f;
		if(ygl::get_key(window, GLFW_KEY_S)) transl.z += +0.1f;
		
		if(rotate != ygl::zero3f ||
		   transl != ygl::zero3f)
		{
			transform_camera(camera_frame, transl, rotate);
		}
	
		// Home key resets camera to initial state
		if(ygl::get_key(window, GLFW_KEY_HOME))
		{
			camera_frame = {
				{1.0f,  0.0f,  0.0f},
				{0.0f,  1.0f,  0.0f},
				{0.0f,  0.0f,  1.0f},
				{0.0f,  0.0f,  0.0f}
			};
		}
	}
	
	// record mouse position
	mouse_last = mouse_pos;
}
