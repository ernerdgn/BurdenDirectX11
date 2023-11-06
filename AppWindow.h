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

	//inheritence from window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	//inheritence from inputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_position) override;
	virtual void onLeftMouseButtonDown(const Point& mouse_position) override;
	virtual void onLeftMouseButtonUp(const Point& mouse_position) override;
	virtual void onRightMouseButtonDown(const Point& mouse_position) override;
	virtual void onRightMouseButtonUp(const Point& mouse_position) override;

private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vertex_buffer;
	VertexShader* m_vertex_shader;
	PixelShader* m_pixel_shader;
	ConstantBuffer* m_constant_buffer;
	IndexBuffer* m_index_buffer;

	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;

	float m_rotation_x = .0f;
	float m_rotation_y = .0f;

	bool is_pressed_left = false;
	bool is_pressed_right = false;

	float m_scale_with_tick = 1;

	//cam
	float m_forward = .0f;
	float m_horizontal_move_coefficient = .0f;  //right is positive due to the LH coord sys

	Matrix4x4 m_world_cam;
};