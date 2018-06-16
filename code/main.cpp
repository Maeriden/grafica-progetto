#define YGL_OPENGL 1
#include "yocto/yocto_gl.h"
#include "yocto/ext/imgui/imgui.cpp"
#include "yocto/ext/imgui/imgui_draw.cpp"
#include "yocto/ext/imgui/imgui_impl_glfw_gl3.cpp"


{
}


static
void
handle_camera_navigation(ygl::gl_window* window, ygl::camera* camera)
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
		
		// Right mouse button allows to look up-down
		if(mouse_button == 2)
		{
			// Difference in y position becomes rotation around x axis
			rotate.x = (mouse_pos.y - mouse_last.y) / 100.0f;
		}
		
		
		if(ygl::get_key(window, GLFW_KEY_A)) transl.x += -0.1f;
		if(ygl::get_key(window, GLFW_KEY_D)) transl.x += +0.1f;
		if(ygl::get_key(window, GLFW_KEY_Q)) transl.y += -0.1f;
		if(ygl::get_key(window, GLFW_KEY_E)) transl.y += +0.1f;
		if(ygl::get_key(window, GLFW_KEY_W)) transl.z += -0.1f;
		if(ygl::get_key(window, GLFW_KEY_S)) transl.z += +0.1f;
		
		if(rotate != ygl::zero3f || transl != ygl::zero3f)
		{
			transform_camera(camera->frame, transl, rotate);
		}
	}
	
	// Home key resets camera to initial state
	if(ygl::get_key(window, GLFW_KEY_HOME))
	{
		camera->frame = {
			{1.0f,  0.0f,  0.0f},
			{0.0f,  1.0f,  0.0f},
			{0.0f,  0.0f,  1.0f},
			{0.0f,  0.0f,  5.0f}
		};
	}
	
	// record mouse position
	mouse_last = mouse_pos;
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


void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int
main(int argc, char** argv)
{
	ygl::gl_window* window = ygl::make_window(800, 600, "SDF", NULL);
	ygl::init_widgets(window);
	
	ygl::gl_program gl_program = load_gl_program();
	ygl::bind_program(gl_program);
	
	
	int uniform_location_raymarch_iterations = ygl::get_program_uniform_location(gl_program, "raymarch_iterations");
	int uniform_location_raymarch_epsilon    = ygl::get_program_uniform_location(gl_program, "raymarch_epsilon");
	int uniform_location_camera_frame        = ygl::get_program_uniform_location(gl_program, "camera_frame");
	int uniform_location_camera_yfov         = ygl::get_program_uniform_location(gl_program, "camera_yfov");
	int uniform_location_camera_aspect       = ygl::get_program_uniform_location(gl_program, "camera_aspect");
	int uniform_location_camera_planes       = ygl::get_program_uniform_location(gl_program, "camera_planes");
	
	int   raymarch_iterations = 100;
	float raymarch_epsilon    = 0.0001f;
	ygl::camera camera = ygl::camera();
	camera.name   = "camera";
	camera.aspect = 4.0f / 3.0f;
	camera.frame  = {
		{1.0f,  0.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  0.0f,  1.0f},
		{0.0f,  0.0f,  5.0f}
	};
	
	// FIXME: Using camera.near causes a compilation error on MSVC 2017
	float camera_planes[2] = {0.01f, 1000.0f};
	
	while(ygl::should_close(window) == false)
	{
		ygl::poll_events(window);
	
		if(!get_widget_active(window))
		{
			if(ygl::get_key(window, 'r'))
			{
				try_reload_gl_program(&gl_program);
			}
		}
		
		// Update camera
		{
			// handle_camera_navigation(window, &camera);
			ygl::handle_camera_navigation(window, &camera, true);
			
			// ygl::vec2i framebufer_size = ygl::get_framebuffer_size(window);
			// camera.aspect = (float)framebufer_size.x / (float)framebufer_size.y;
		}
		
		ygl::set_program_uniform(gl_program, uniform_location_raymarch_iterations, raymarch_iterations);
		ygl::set_program_uniform(gl_program, uniform_location_raymarch_epsilon,    raymarch_epsilon);
		ygl::set_program_uniform(gl_program, uniform_location_camera_frame,        camera.frame);
		ygl::set_program_uniform(gl_program, uniform_location_camera_yfov,         camera.yfov);
		ygl::set_program_uniform(gl_program, uniform_location_camera_aspect,       camera.aspect);
		ygl::set_program_uniform(gl_program, uniform_location_camera_planes,       camera_planes, 1, 2);
		
		ygl::gl_clear_buffers({0.0f, 1.0f, 0.0f, 1.0f});
		
		// Draw scene - everything is done in the fragment shader
		glBindVertexArray(gl_program.vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		
#if 0
		Draw UI
		if(ygl::begin_widgets(window, "sdf"))
		{
			draw_value_widget(window, "camera",     camera.frame,        0.0f,   0.0f, 0.0f);
			draw_value_widget(window, "iterations", raymarch_iterations, 0,      200,  1);
			draw_value_widget(window, "epsilon",    raymarch_epsilon,    0.001f, 0.1f, 0.001f);
		}
		end_widgets(window);
#endif
		
		ygl::swap_buffers(window);
	}
	
	clear_window(window);
	
	return 0;
}


#if PLATFORM_WIN32
int
WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}
#endif // PLATFORM_WIN32
