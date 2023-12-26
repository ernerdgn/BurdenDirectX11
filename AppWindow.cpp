#include "AppWindow.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"

#include <Windows.h>
#include <iostream>

struct vertex
{
	Vector3D pos;
	Vector2D texcoord;
};

__declspec(align(16))  //16 bytes alignation
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	Vector4D m_light_direction;
	Vector4D m_cam_position;
};

AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	updateCamera();
	updateModel();
	updateSkybox();
}

AppWindow::~AppWindow()
{
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentityMatrix();

	temp.setIdentityMatrix();
	temp.setRotationXMatrix(m_rotation_x);
	world_cam *= temp;

	temp.setIdentityMatrix();
	temp.setRotationYMatrix(m_rotation_y);
	world_cam *= temp;

	Vector3D new_position = m_world_cam.getTranslation() + (world_cam.getZDirection() * (m_forward * .1f) + world_cam.getXDirection() * (m_horizontal_move_coefficient * .1f));

	world_cam.setTranslationMatrix(new_position);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	m_projection_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), .1f, 100.0f);
}

void AppWindow::updateModel()
{
	constant model_constant;
	Matrix4x4 m_light_rotation_matrix;

	m_light_rotation_matrix.setIdentityMatrix();
	m_light_rotation_matrix.setRotationYMatrix(m_light_rotation_y);

	m_light_rotation_y += .785f * m_delta_time;  // pi / 4 * delta_t

	model_constant.m_world.setIdentityMatrix();
	model_constant.m_view = m_view_cam;
	model_constant.m_projection = m_projection_cam;
	model_constant.m_cam_position = m_world_cam.getTranslation();
	model_constant.m_light_direction = m_light_rotation_matrix.getZDirection();

	m_constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &model_constant);  //constant buffer updating
}

void AppWindow::updateSkybox()
{
	constant skybox_constant;

	skybox_constant.m_world.setIdentityMatrix();
	skybox_constant.m_world.setScaleMatrix(Vector3D(100.0f, 100.0f, 100.0f));
	skybox_constant.m_world.setTranslationMatrix(m_world_cam.getTranslation());
	skybox_constant.m_view = m_view_cam;
	skybox_constant.m_projection = m_projection_cam;

	m_skybox_constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &skybox_constant);  //constant buffer updating
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vertex_shader, const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer, const TexturePtr& texture)
{
	//set constant buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vertex_shader, constant_buffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(pixel_shader, constant_buffer);

	//implement the prepared shaders to graphic pipeline to be able to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vertex_shader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(pixel_shader);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(pixel_shader, texture);

	//set the vertices of the object to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());

	//set the indices of the object
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	/* drawing */
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	// get texture from file
	m_texture1 = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\whitewall.png");
	m_texture_skybox = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");

	// get 3d model from file
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"C:\\Users\\emrer\\OneDrive\\Meshes\\statue.obj");
	m_skybox_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"C:\\Users\\emrer\\OneDrive\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//cam
	m_world_cam.setTranslationMatrix(Vector3D(0, 0, -1));

	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	/* VERTEX SHADER */
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	/* PIXEL SHADER */
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	/* SKYBOX SHADER */
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyboxShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_skybox_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);  //is a pixelShaderPtr
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	/* CONSTANT BUFFER */
	constant const_obj;

	m_constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&const_obj, sizeof(constant));  //constant buffer loading
	m_skybox_constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&const_obj, sizeof(constant));  //skybox const buff
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	/* clearing the render target */
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, .25f, .25f, .25f, 1);  //0.125f, 0.025f, 0.125f

	/* set viewport of render target (which to draw) */
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	/* constant buffer updates */
	// flag update
	update();

	//rendering the model
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(false);
	drawMesh(m_mesh, m_vertex_shader, m_pixel_shader, m_constant_buffer, m_texture1);

	//rendering the skybox mesh which is a sphere
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(true);
	drawMesh(m_skybox_mesh, m_vertex_shader, m_skybox_shader, m_skybox_constant_buffer, m_texture_skybox);

	m_swap_chain->present(false);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
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
		m_horizontal_move_coefficient = 1.0;
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
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	m_rotation_x += (mouse_position.m_y - (height / 2)) * m_delta_time * .1f;
	m_rotation_y += (mouse_position.m_x - (width / 2)) * m_delta_time * .1f;

	InputSystem::get()->setCursorPosition(Point(width / 2, height / 2));
}

void AppWindow::onLeftMouseButtonDown(const Point& mouse_position)
{
	is_pressed_left = true;
}

void AppWindow::onLeftMouseButtonUp(const Point& mouse_position)
{
	is_pressed_left = false;
}

void AppWindow::onRightMouseButtonDown(const Point& mouse_position)
{
	is_pressed_right = true;
}

void AppWindow::onRightMouseButtonUp(const Point& mouse_position)
{
	is_pressed_right = false;
}