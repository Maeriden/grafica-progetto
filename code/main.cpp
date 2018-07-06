#define YGL_OPENGL 1
#include "yocto/yocto_gl.h"
#include "yocto/ext/imgui/imgui.cpp"
#include "yocto/ext/imgui/imgui_draw.cpp"
#include "yocto/ext/imgui/imgui_impl_glfw_gl3.cpp"
#include "extra.cpp"


#define DEFAULT_SCREEN_SIZE_X 800
#define DEFAULT_SCREEN_SIZE_Y 600


struct GlobalState
{
	ygl::camera camera;
} global_state;


void
framebuffer_size_callback(GLFWwindow* glfw_window, int width, int height)
{
	ygl::gl_window* gl_window    = (ygl::gl_window*)glfwGetWindowUserPointer(glfw_window);
	GlobalState*    global_state = (GlobalState*)gl_window->user_pointer;
	
	global_state->camera.aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
}


int
main(int argc, char** argv)
{
	ygl::gl_window* window = ygl::make_window(DEFAULT_SCREEN_SIZE_X, DEFAULT_SCREEN_SIZE_Y, "SDF", &global_state);
	glfwSetFramebufferSizeCallback(window->gwin, framebuffer_size_callback);
	ygl::init_widgets(window);
	
	ygl::gl_program gl_program = load_gl_program();
	ygl::bind_program(gl_program);
	
	
	int uniform_location_raymarcher_iterations = ygl::get_program_uniform_location(gl_program, "raymarcher.iterations");
	int uniform_location_raymarcher_epsilon    = ygl::get_program_uniform_location(gl_program, "raymarcher.epsilon");
	int uniform_location_camera_frame          = ygl::get_program_uniform_location(gl_program, "camera.frame");
	int uniform_location_camera_yfov           = ygl::get_program_uniform_location(gl_program, "camera.yfov");
	int uniform_location_camera_aspect         = ygl::get_program_uniform_location(gl_program, "camera.aspect");
	int uniform_location_camera_planes         = ygl::get_program_uniform_location(gl_program, "camera.planes");
	
	
	int   raymarcher_iterations = 100;
	float raymarcher_epsilon    = 0.0001f;
	
	
	ygl::camera* camera = &global_state.camera;
	{
		camera->name   = "camera";
		camera->frame  = {
			{1.0f,  0.0f,  0.0f},
			{0.0f,  1.0f,  0.0f},
			{0.0f,  0.0f,  1.0f},
			{0.0f,  0.0f,  5.0f}
		};
		camera->yfov   = RADIANS(70.0f);
		camera->aspect = 800.0f / 600.0f;
		camera->near = 0.01f;
		camera->far  = 1000.0f;
	}
	float camera_planes[2] = {camera->near, camera->far};
	
	
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
			handle_camera_navigation(window, camera->frame);
		}
		
		ygl::set_program_uniform(gl_program, uniform_location_raymarcher_iterations, raymarcher_iterations);
		ygl::set_program_uniform(gl_program, uniform_location_raymarcher_epsilon,    raymarcher_epsilon);
		ygl::set_program_uniform(gl_program, uniform_location_camera_frame,          camera->frame);
		ygl::set_program_uniform(gl_program, uniform_location_camera_yfov,           camera->yfov);
		ygl::set_program_uniform(gl_program, uniform_location_camera_aspect,         camera->aspect);
		ygl::set_program_uniform(gl_program, uniform_location_camera_planes,         camera_planes, 1, 2);
		
		ygl::gl_clear_buffers({0.0f, 1.0f, 0.0f, 1.0f});
		
		// Draw scene - everything is done in the fragment shader
		glBindVertexArray(gl_program.vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		
		// Draw UI
		if(ygl::begin_widgets(window, "sdf"))
		{
			draw_value_widget(window, "camera",      camera->frame,         0.0f,   0.0f,  0.0f);
			draw_value_widget(window, "camera_yfov", camera->yfov,          0.0f,   3.14f, 0.1f);
			draw_value_widget(window, "iterations",  raymarcher_iterations, 0,      200,   1);
			draw_value_widget(window, "epsilon",     raymarcher_epsilon,    0.001f, 0.1f,  0.001f);
		}
		end_widgets(window);
		
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
