#define YGL_OPENGL 1
#include "yocto/yocto_gl.h"
#include "yocto/ext/imgui/imgui.cpp"
#include "yocto/ext/imgui/imgui_draw.cpp"
#include "yocto/ext/imgui/imgui_impl_glfw_gl3.cpp"


static
ygl::gl_program
load_gl_program()
{
	std::vector<unsigned char> vert_shader_code = ygl::load_binary("shaders/shader.vert");
	std::vector<unsigned char> frag_shader_code = ygl::load_binary("shaders/shader.frag");
	ygl::gl_program gl_program = ygl::make_program(std::string(vert_shader_code.begin(), vert_shader_code.end()),
	                                               std::string(frag_shader_code.begin(), frag_shader_code.end()));
	return gl_program;
}


static
void
handle_camera_navigation(ygl::gl_window* window, ygl::camera* camera)
{
	static ygl::vec2f mouse_last   = ygl::zero2f;
	ygl::vec2f        mouse_pos    = get_mouse_posf(window);
	int               mouse_button = get_mouse_button(window);
	
	// handle mouse and keyboard for navigation
	if(mouse_button && !ygl::get_widget_active(window))
	{
		auto dolly  = 0.0f;
		auto pan    = ygl::zero2f;
		auto rotate = ygl::zero2f;
		switch(mouse_button)
		{
			case  1: rotate = (mouse_pos   - mouse_last)   / 100.0f; break;
			case  2: dolly  = (mouse_pos.x - mouse_last.x) / 100.0f; break;
			case  3: pan    = (mouse_pos   - mouse_last)   / 100.0f; break;
			default: break;
		}
		ygl::camera_fps(camera->frame, {0, 0, 0}, rotate);
	}
	
	// handle keyboard for navigation
	if(!get_widget_active(window))
	{
		ygl::vec3f transl = ygl::zero3f;
		if(ygl::get_key(window, 'w')) transl.z += +0.1f;
		if(ygl::get_key(window, 'a')) transl.x += -0.1f;
		if(ygl::get_key(window, 's')) transl.z += -0.1f;
		if(ygl::get_key(window, 'd')) transl.x += +0.1f;
		if(ygl::get_key(window, 'e')) transl.y += +0.1f;
		if(ygl::get_key(window, 'q')) transl.y += -0.1f;
		
		if(transl != ygl::zero3f)
		{
			ygl::camera_fps(camera->frame, transl, {0, 0});
		}
	}
	
	// record mouse position
	mouse_last = mouse_pos;
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
	int uniform_location_camera_focus        = ygl::get_program_uniform_location(gl_program, "camera_focus");
	int uniform_location_camera_planes       = ygl::get_program_uniform_location(gl_program, "camera_planes");
	
	int   raymarch_iterations = 100;
	float raymarch_epsilon    = 0.0001f;
	ygl::camera camera = ygl::camera();
	camera.name  = "camera";
	camera.frame = {
		{1.0f,  0.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  0.0f,  1.0f},
		{0.0f,  0.0f, -5.0f}
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
				ygl::unbind_program(gl_program);
				ygl::clear_program(gl_program);
				gl_program = load_gl_program();
				ygl::bind_program(gl_program);
			}
		}
		
		// Update camera
		{
			handle_camera_navigation(window, &camera);
			
			ygl::vec2i framebufer_size = get_framebuffer_size(window);
			camera.aspect = (float)framebufer_size.x / (float)framebufer_size.y;
		}
		
		ygl::set_program_uniform(gl_program, uniform_location_raymarch_iterations, raymarch_iterations);
		ygl::set_program_uniform(gl_program, uniform_location_raymarch_epsilon,    raymarch_epsilon);
		ygl::set_program_uniform(gl_program, uniform_location_camera_frame,        camera.frame);
		ygl::set_program_uniform(gl_program, uniform_location_camera_yfov,         camera.yfov);
		ygl::set_program_uniform(gl_program, uniform_location_camera_aspect,       camera.aspect);
		ygl::set_program_uniform(gl_program, uniform_location_camera_focus,        camera.focus);
		ygl::set_program_uniform(gl_program, uniform_location_camera_planes,       camera_planes, 1, 2);
		
		ygl::gl_clear_buffers({0.0f, 1.0f, 0.0f, 1.0f});
		
		// Draw scene - everything is done in the fragment shader
		glBindVertexArray(gl_program.vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		
		// Draw UI
		if(begin_widgets(window, "sdf"))
		{
			draw_value_widget(window, "camera", camera.frame, 0.0f, 0.0f, 0.0f);
			draw_value_widget(window, "iterations", raymarch_iterations, 0, 200, 1);
			draw_value_widget(window, "epsilon", raymarch_epsilon, 0.001f, 0.1f, 0.001f);
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
