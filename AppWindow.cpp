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
	Vector4D m_light_position = Vector4D(0,1,0,0);
	float m_light_radius = 4.0f;
	float m_time = .0f;


};

AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	updateCamera();
	updateLight();
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

void AppWindow::updateModel(Vector3D position, const MaterialPtr& material)
{
	constant model_constant;
	Matrix4x4 m_light_rotation_matrix;

	m_light_rotation_matrix.setIdentityMatrix();
	m_light_rotation_matrix.setRotationYMatrix(m_light_rotation_y);

	model_constant.m_world.setIdentityMatrix();
	model_constant.m_world.setTranslationMatrix(position);
	model_constant.m_view = m_view_cam;
	model_constant.m_projection = m_projection_cam;
	model_constant.m_cam_position = m_world_cam.getTranslation();

	model_constant.m_light_position = m_light_position;
	model_constant.m_light_radius = m_light_radius;

	model_constant.m_light_direction = m_light_rotation_matrix.getZDirection();
	model_constant.m_time = m_time;

	material->setData(&model_constant, sizeof(constant));
}

void AppWindow::updateSkybox()
{
	constant skybox_constant;

	skybox_constant.m_world.setIdentityMatrix();
	skybox_constant.m_world.setScaleMatrix(Vector3D(100.0f, 100.0f, 100.0f));
	skybox_constant.m_world.setTranslationMatrix(m_world_cam.getTranslation());
	skybox_constant.m_view = m_view_cam;
	skybox_constant.m_projection = m_projection_cam;

	m_skybox_material->setData(&skybox_constant, sizeof(constant));
}

void AppWindow::updateLight()
{
	m_light_rotation_y += 1.57f * m_delta_time;  // pi / 4 * delta_t  (can be modified to change the speed of the animation)

	float distance_from_origin = 3.0f;
	m_light_position = Vector4D(cos(m_light_rotation_y) * distance_from_origin, 2.0f, sin(m_light_rotation_y) * distance_from_origin, 2.0f);
}

void AppWindow::render()
{
	/* clearing the render target */
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, .25f, .25f, .25f, 1);  //0.125f, 0.025f, 0.125f

	/* set viewport of render target (which to draw) */
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	/* constant buffer updates */
	// flag update
	update();

	//rendering the model
	updateModel(Vector3D(0, 0, 0), m_brick_material);
	drawMesh(m_skybox_mesh, m_brick_material);

	//rendering the model
	updateModel(Vector3D(0, 0, 5), m_world_material);
	drawMesh(m_skybox_mesh, m_world_material);

	//rendering the skybox mesh which is a sphere
	drawMesh(m_skybox_mesh, m_skybox_material);

	m_swap_chain->present(false);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;

	m_time += m_delta_time;
}

void AppWindow::drawMesh(const MeshPtr& mesh, const MaterialPtr material)
{
	//set constant buffer
	GraphicsEngine::get()->setMaterial(material);
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

	m_resize_state = true;
	InputSystem::get()->showCursor(false);

	// get texture from file
	m_brick_texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_world_texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\world_morning.jpg");
	m_texture_skybox = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars.jpg");  //skybox

	// get 3d model from file
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"C:\\Users\\emrer\\OneDrive\\Meshes\\scene.obj");
	m_skybox_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"C:\\Users\\emrer\\OneDrive\\Meshes\\sphere.obj");  //skybox

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//cam
	m_world_cam.setTranslationMatrix(Vector3D(0, 0, -1));

	/* MATERIAL */
	m_brick_material = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"PointLightPixelShader.hlsl");
	m_brick_material->addTexture(m_brick_texture);
	m_brick_material->setCullMode(CULL_MODE_BACK);

	m_world_material = GraphicsEngine::get()->createMaterial(m_brick_material);
	m_world_material->addTexture(m_world_texture);
	m_world_material->setCullMode(CULL_MODE_BACK);
	
	m_skybox_material = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SkyboxShader.hlsl");
	m_skybox_material->addTexture(m_texture_skybox);
	m_skybox_material->setCullMode(CULL_MODE_FRONT);

	m_world_cam.setTranslationMatrix(Vector3D(0, 0, -3));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullscreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right, rc.bottom);
	this->render();
}

void AppWindow::onKeyDown(int key)
{
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
	else if (key == VK_LSHIFT)
	{
		m_forward *= 4.0f;
		m_horizontal_move_coefficient *= 4.0f;
	}
	else if (key == VK_ESCAPE)
	{
		m_is_running = false;
	}
	else if (key == VK_UP)
	{
		m_light_radius += 4.0f * m_delta_time;
	}
	else if (key == VK_DOWN)
	{
		m_light_radius -= 4.0f * m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = .0f;
	m_horizontal_move_coefficient = .0f;

	if (key == 'P')
	{
		m_resize_state = (m_resize_state) ? false : true;
		InputSystem::get()->showCursor(!m_resize_state);
	}
	else if (key == VK_F11)
	{
		m_fullscreen_flag = (m_fullscreen_flag) ? false : true;
		
		RECT screen_size = this->getScreenSize();

		m_swap_chain->setFullscreen(m_fullscreen_flag, screen_size.right, screen_size.bottom);
	}
}

void AppWindow::onMouseMove(const Point& mouse_position)
{
	if (!m_resize_state) return;
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