#include "AppWindow.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"

#include <Windows.h>
#include <iostream>
#include <chrono>

struct vertex
{
	Vector3D pos;
	Vector2D texcoord;
};

__declspec(align(16))  //16 bytes alignation
struct constant
{
	Matrix4x4 m_view;
	Matrix4x4 m_world;
	Matrix4x4 m_projection;
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	constant const_obj;
	const_obj.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;

	if (m_delta_pos > 1.0f) m_delta_pos = 0;

	Matrix4x4 temp;

	m_delta_scale += m_delta_time / .5f;

	//const_obj.m_world.setTranslationMatrix(Vector3D(0,0,0));
	//const_obj.m_world.setTranslationMatrix(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_pos));
	//const_obj.m_world.setScaleMatrix(Vector3D::lerp(Vector3D(.5, .5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale)+1.0f)/2.0f));
	//temp.setTranslationMatrix(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));
	
	/* before cam */
	////const_obj.m_world *= temp;
	//const_obj.m_world.setScaleMatrix(Vector3D(m_scale_with_tick, m_scale_with_tick, m_scale_with_tick));
	////const_obj.m_world.setScaleMatrix(Vector3D(1, 1, 1));

	////z-rotation
	//temp.setIdentityMatrix();
	//temp.setRotationZMatrix(.0f);
	////temp.setRotationZMatrix(m_delta_scale);
	//const_obj.m_world *= temp;

	////y-rotation
	//temp.setIdentityMatrix();
	//temp.setRotationYMatrix(m_rotation_y);
	////temp.setRotationYMatrix(m_delta_scale);
	//const_obj.m_world *= temp;

	////x-rotation
	//temp.setIdentityMatrix();
	//temp.setRotationXMatrix(m_rotation_x);
	////temp.setRotationXMatrix(m_delta_scale);
	//const_obj.m_world *= temp;

	const_obj.m_world.setIdentityMatrix();

	Matrix4x4 world_cam;
	world_cam.setIdentityMatrix();

	temp.setIdentityMatrix();
	temp.setRotationXMatrix(m_rotation_x);
	world_cam *= temp;

	temp.setIdentityMatrix();
	temp.setRotationYMatrix(m_rotation_y);
	world_cam *= temp;

	Vector3D new_position = m_world_cam.getTranslation() + (world_cam.getZDirection() * (m_forward * .3f) + world_cam.getXDirection() * (m_horizontal_move_coefficient * .3f));

	world_cam.setTranslationMatrix(new_position);

	m_world_cam = world_cam;

	world_cam.inverse();

	//const_obj.m_view.setIdentityMatrix();
	const_obj.m_view = world_cam;

	//const_obj.m_projection.setOrthogonalProjectionMatrix(
	//	(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
	//	(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
	//	-4.0f,
	//	4.0f
	//);

	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	const_obj.m_projection.setPerspectiveFovLH(1.57079632f, ((float)width / (float)height), .1f, 100.0f);

	m_constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &const_obj);  //constant buffer updating
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	// get texture from file
	//m_wooden_crate_text = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wood.jpg");
	m_wooden_crate_text = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_wood.jpg");

	// get 3d model from file C:\Users\emrer\OneDrive
	//m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\teapot.obj");
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"C:\\Users\\emrer\\OneDrive\\teapot.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	//cam
	m_world_cam.setTranslationMatrix(Vector3D(0, 0, -2));

	Vector3D position_list[] = {
		/* FRONT */
		{Vector3D(-.5f, -.5f, -.5f)},
		{Vector3D(-.5f, .5f, -.5f)},
		{Vector3D(.5f, .5f, -.5f)},
		{Vector3D(.5f, -.5f, -.5f)},

		/* BEHIND */
		{Vector3D(.5f, -.5f, .5f)},
		{Vector3D(.5f, .5f, .5f)},
		{Vector3D(-.5f, .5f, .5f)},
		{Vector3D(-.5f, -.5f, .5f)}
	};

	Vector2D texcoord_list[] = {
		{Vector2D(.0f, .0f)},
		{Vector2D(.0f, 1.0f)},
		{Vector2D(1.0f, .0f)},
		{Vector2D(1.0f, 1.0f)},
	};

	vertex vertex_list[] = {
		/* front */
		{ position_list[0], texcoord_list[1] },
		{ position_list[1], texcoord_list[0] },
		{ position_list[2], texcoord_list[2] },
		{ position_list[3], texcoord_list[3] },

		/* behind */
		{ position_list[4], texcoord_list[1] },
		{ position_list[5], texcoord_list[0] },
		{ position_list[6], texcoord_list[2] },
		{ position_list[7], texcoord_list[3] },

		/* top */
		{ position_list[1], texcoord_list[1] },
		{ position_list[6], texcoord_list[0] },
		{ position_list[5], texcoord_list[2] },
		{ position_list[2], texcoord_list[3] },

		/* bottom */
		{ position_list[7], texcoord_list[1] },
		{ position_list[0], texcoord_list[0] },
		{ position_list[3], texcoord_list[2] },
		{ position_list[4], texcoord_list[3] },

		/* right */
		{ position_list[3], texcoord_list[1] },
		{ position_list[2], texcoord_list[0] },
		{ position_list[5], texcoord_list[2] },
		{ position_list[4], texcoord_list[3] },

		/* left */
		{ position_list[7], texcoord_list[1] },
		{ position_list[6], texcoord_list[0] },
		{ position_list[1], texcoord_list[2] },
		{ position_list[0], texcoord_list[3] },
	};

	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] = {
		/* FRONT */
		0, 1, 2,  // 1st triangle
		2, 3, 0,  // 2nd triangle

		/* BACK */
		4, 5, 6,  // 1st triangle
		6, 7, 4,  // 2nd triangle

		/* TOP */
		8, 9, 10,  // 1st triangle
		10, 11, 8,  // 2nd triangle

		/* BOTTOM */
		12, 13, 14,  // 1st triangle
		14, 15, 12,  // 2nd triangle

		/* RIGHT */
		16, 17, 18,  // 1st triangle
		18, 19, 16,  // 2nd triangle

		/* LEFT */
		20, 21, 22,  // 1st triangle
		22, 23, 20  // 2nd triangle
	};

	UINT size_index_list = ARRAYSIZE(index_list);
	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;


	/* VERTEX SHADER */
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	/* PIXEL SHADER */
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	/* CONSTANT BUFFER */
	constant const_obj;
	const_obj.m_time = 0;

	m_constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&const_obj, sizeof(constant));  //constant buffer loading
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	/*clearing the render target*/
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0.125f, 0.025f, 0.125f, 1);

	/*set viewport of render target (which to draw)*/
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	//GraphicsEngine::get()->setShaders();

	/* constant buffer updates */
	//const_obj.m_angle = ::GetTickCount();  //get the time(ms) elapsed since the sys has started (winAPI)
	// flag
	update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vertex_shader, m_constant_buffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_pixel_shader, m_constant_buffer);

	//implement the prepared shaders to graphic pipeline to be able to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_pixel_shader, m_wooden_crate_text);

	//set the vertices of the object to draw
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vertex_buffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());

	//set the indices of the object
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_index_buffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());

	/* drawing */
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vertex_buffer->getSizeVertexList(), 0);
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vertex_buffer->getSizeVertexList(), 0);
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_index_buffer->getSizeIndexList(), 0, 0);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

	m_swap_chain->present(false);

	
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	//Sleep(1);
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	//if (key == 'W') m_rotation_x += 3.1415f * m_delta_time;
	//else if (key == 'S') m_rotation_x -= 3.1415f * m_delta_time;
	//else if (key == 'D') m_rotation_y -= 3.1415f * m_delta_time;
	//else if (key == 'A') m_rotation_y += 3.1415f * m_delta_time;

	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'D')
	{
		m_horizontal_move_coefficient = 1.0f;
	}
	else if (key == 'A')
	{
		m_horizontal_move_coefficient = -1.0f;
	}
	else if (key == VK_ESCAPE)
	{
		//m_kumbara += 1;
		m_is_running = false;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = .0f;
	m_horizontal_move_coefficient = .0f;
}

void AppWindow::onMouseMove(const Point& mouse_position)
{
	//if (is_pressed_left)
	//{
	//	m_rotation_x -= delta_mouse_position.m_y * m_delta_time;
	//	m_rotation_y -= delta_mouse_position.m_x * m_delta_time;
	//}

	//if (is_pressed_right)
	//{
	//	m_scale_with_tick += delta_mouse_position.m_x * m_delta_time;
	//	if (m_scale_with_tick < 0) m_scale_with_tick = 0;
	//	//std::cout << delta_mouse_position.m_x << std::endl;
	//}

	//m_rotation_x -= delta_mouse_position.m_y * 0.015f;  //m_delta_time
	//m_rotation_y -= delta_mouse_position.m_x * 0.015f;

	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	m_rotation_x += (mouse_position.m_y - (height / 2)) * m_delta_time * .1f;
	m_rotation_y += (mouse_position.m_x - (width / 2)) * m_delta_time * .1f;

	InputSystem::get()->setCursorPosition(Point(width / 2, height / 2));
}

void AppWindow::onLeftMouseButtonDown(const Point& mouse_position)
{
	//m_scale_with_tick = 1.15;
	//std::cout << "left mouse down" << std::endl;
	is_pressed_left = true;
}

void AppWindow::onLeftMouseButtonUp(const Point& mouse_position)
{
	//m_scale_with_tick = 1.0f;
	is_pressed_left = false;
	//std::cout << "left mouse UP" << std::endl;
}

void AppWindow::onRightMouseButtonDown(const Point& mouse_position)
{
	//m_scale_with_tick = .5f;
	//std::cout << "right mouse down" << std::endl;
	is_pressed_right = true;
}

void AppWindow::onRightMouseButtonUp(const Point& mouse_position)
{
	//m_scale_with_tick = 1.0f;
	is_pressed_right = false;
	//std::cout << "right mouse UP" << std::endl;
}
