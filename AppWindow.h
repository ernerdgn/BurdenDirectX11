#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "InputListener.h"
#include "Matrix4x4.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	void update();
	~AppWindow();

	void updateModel();
	void updateCamera();
	void updateSkybox();

	void render();

	void drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vertex_shader, const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer, const TexturePtr* texture_list, unsigned int texture_count);

	//inheritence from window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;

	//inheritence from inputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_position) override;
	virtual void onLeftMouseButtonDown(const Point& mouse_position) override;
	virtual void onLeftMouseButtonUp(const Point& mouse_position) override;
	virtual void onRightMouseButtonDown(const Point& mouse_position) override;
	virtual void onRightMouseButtonUp(const Point& mouse_position) override;

private:
	//graphicsEngine components
	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vertex_buffer;
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	PixelShaderPtr m_skybox_shader;
	ConstantBufferPtr m_constant_buffer;
	ConstantBufferPtr m_skybox_constant_buffer;

	//textures
	TexturePtr m_world_morning_tex;
	TexturePtr m_world_night_tex;
	TexturePtr m_world_clouds_tex;
	TexturePtr m_world_specular_tex;
	TexturePtr m_texture_skybox;
	
	//meshes
	MeshPtr m_mesh;
	MeshPtr m_skybox_mesh;

	//elapsed time calculation
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;
	float m_time = .0f;

	//position and scale changes
	float m_delta_pos;
	float m_delta_scale;

	//rotation coefficients
	float m_rotation_x = .0f;
	float m_rotation_y = .0f;

	//light
	float m_light_rotation_y = .0f;

	//mouse button flags
	bool is_pressed_left = false;
	bool is_pressed_right = false;

	//cam
	float m_forward = .0f;
	float m_horizontal_move_coefficient = .0f;  //right is positive due to the LH coord sys

	//world-view-porjection matrices
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_projection_cam;

	//state flags
	bool m_resize_state = false;
	bool m_fullscreen_flag = false;
};