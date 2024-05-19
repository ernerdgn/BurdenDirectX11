#include "SpaceShooterGame.h"
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
	Vector4D m_light_position = Vector4D(0, 1, 0, 0);
	float m_light_radius = 4.0f;
	float m_time = .0f;


};

SpaceShooterGame::SpaceShooterGame()
{
}

void SpaceShooterGame::update()
{
	updateLight();
	updateSkybox();
	updateSpaceship();
	updateThirdPersonCamera();
}

SpaceShooterGame::~SpaceShooterGame()
{
}

void SpaceShooterGame::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentityMatrix();

	m_camera_rotation.m_x += m_delta_mouse_y * m_delta_time * .1f;
	m_camera_rotation.m_y += m_delta_mouse_x * m_delta_time * .1f;

	temp.setIdentityMatrix();
	temp.setRotationXMatrix(m_camera_rotation.m_x);
	world_cam *= temp;

	temp.setIdentityMatrix();
	temp.setRotationYMatrix(m_camera_rotation.m_y);
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

void SpaceShooterGame::updateThirdPersonCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentityMatrix();

	m_camera_rotation.m_x += m_delta_mouse_y * m_delta_time * .1f;
	m_camera_rotation.m_y += m_delta_mouse_x * m_delta_time * .1f;

	if (m_camera_rotation.m_x >= 1.57f) m_camera_rotation.m_x = 1.57f;
	if (m_camera_rotation.m_x <= -1.57f) m_camera_rotation.m_x = -1.57f;

	temp.setIdentityMatrix();
	temp.setRotationXMatrix(m_camera_rotation.m_x);
	world_cam *= temp;

	temp.setIdentityMatrix();
	temp.setRotationYMatrix(m_camera_rotation.m_y);
	world_cam *= temp;

	m_camera_position = m_spaceship_position;

	Vector3D new_position = m_camera_position + (world_cam.getZDirection() * (-m_camera_distance));
	new_position = new_position + (world_cam.getYDirection() * (5.0f));

	world_cam.setTranslationMatrix(new_position);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	m_projection_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), .1f, 100.0f);
}

void SpaceShooterGame::updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials)
{
	constant model_constant;

	Matrix4x4 temp;
	model_constant.m_world.setIdentityMatrix();
	
	temp.setIdentityMatrix();
	temp.setScaleMatrix(scale);
	model_constant.m_world *= temp;

	temp.setIdentityMatrix();
	temp.setRotationXMatrix(rotation.m_x);
	model_constant.m_world *= temp;

	temp.setIdentityMatrix();
	temp.setRotationYMatrix(rotation.m_y);
	model_constant.m_world *= temp;

	temp.setIdentityMatrix();
	temp.setRotationZMatrix(rotation.m_z);
	model_constant.m_world *= temp;

	temp.setIdentityMatrix();
	temp.setTranslationMatrix(position);
	model_constant.m_world *= temp;

	model_constant.m_view = m_view_cam;
	model_constant.m_projection = m_projection_cam;
	model_constant.m_cam_position = m_world_cam.getTranslation();

	model_constant.m_light_position = m_light_position;
	model_constant.m_light_radius = .0f;

	model_constant.m_light_direction = m_light_rotation_matrix.getZDirection();
	model_constant.m_time = m_time;

	for (size_t material = 0; material < list_materials.size(); material++)
	{
		list_materials[material]->setData(&model_constant, sizeof(constant));
	}
}

void SpaceShooterGame::updateSkybox()
{
	constant skybox_constant;

	skybox_constant.m_world.setIdentityMatrix();
	skybox_constant.m_world.setScaleMatrix(Vector3D(100.0f, 100.0f, 100.0f));
	skybox_constant.m_world.setTranslationMatrix(m_world_cam.getTranslation());
	skybox_constant.m_view = m_view_cam;
	skybox_constant.m_projection = m_projection_cam;

	m_skybox_material->setData(&skybox_constant, sizeof(constant));
}

void SpaceShooterGame::updateLight()
{
	Matrix4x4 temp;
	m_light_rotation_matrix.setIdentityMatrix();

	temp.setIdentityMatrix();
	temp.setRotationXMatrix(-.707f);
	m_light_rotation_matrix *= temp;

	temp.setIdentityMatrix();
	temp.setRotationYMatrix(.707f);
	m_light_rotation_matrix *= temp;
}

void SpaceShooterGame::updateSpaceship()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentityMatrix();

	m_spaceship_rotation.m_x += m_delta_mouse_y * m_delta_time * .1f;
	m_spaceship_rotation.m_y += m_delta_mouse_x * m_delta_time * .1f;

	if (m_spaceship_rotation.m_x >= 1.57f) m_spaceship_rotation.m_x = 1.57f;
	if (m_spaceship_rotation.m_x <= -1.57f) m_spaceship_rotation.m_x = -1.57f;

	temp.setIdentityMatrix();
	temp.setRotationXMatrix(m_spaceship_rotation.m_x);
	world_cam *= temp;

	temp.setIdentityMatrix();
	temp.setRotationYMatrix(m_spaceship_rotation.m_y);
	world_cam *= temp;

	m_spaceship_position = m_spaceship_position + (world_cam.getZDirection() * (m_forward) * 150.0f * m_delta_time);

	
}

void SpaceShooterGame::render()
{
	/* clearing the render target */
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, .25f, .25f, .25f, 1);  //0.125f, 0.025f, 0.125f

	/* set viewport of render target (which to draw) */
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	/* RENDERING MATERIALS */
	//spaceship
	m_materials.clear();
	m_materials.push_back(m_space_ship_material);
	updateModel(m_spaceship_position, m_spaceship_rotation, Vector3D(1, 1, 1), m_materials);
	drawMesh(m_space_ship_mesh, m_materials);

	//asteroid
	m_materials.clear();
	m_materials.push_back(m_asteroid_material);
	updateModel(Vector3D(0, 15, 15), Vector3D(0, 0, 0), Vector3D(1, 1, 1), m_materials);
	drawMesh(m_asteroid_mesh, m_materials);

	//skybox
	m_materials.clear();
	m_materials.push_back(m_skybox_material);

	drawMesh(m_skybox_mesh, m_materials);  //rendering the skybox mesh which is a sphere obj
	/* - - - */

	m_swap_chain->present(false);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;

	m_time += m_delta_time;
}

void SpaceShooterGame::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
{
	//set the vertices of the object to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());

	//set the indices of the object
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	for (size_t material = 0; material < mesh->getNumMaterialSlots(); material++)
	{
		if (material >= list_materials.size()) break;

		MaterialSlot mat_ = mesh->getMaterialSlot(material);

		//set constant buffer
		GraphicsEngine::get()->setMaterial(list_materials[material]);

		/* drawing */
		//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat_.num_indices, 0, mat_.start_index);
	}
}

void SpaceShooterGame::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);

	m_resize_state = true;
	InputSystem::get()->showCursor(false);

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	/* TEXTURES */
	m_texture_skybox = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars.jpg");  //skybox
	m_space_ship_texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\spaceship-dif3.jpg");
	m_asteroid_texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\asteroid.jpg");

	/* MODELS - OBJECTS */
	m_skybox_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"C:\\Users\\emrer\\OneDrive\\Meshes\\sphere.obj");  //skybox
	m_space_ship_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"C:\\Users\\emrer\\OneDrive\\Meshes\\spaceship.obj");
	m_asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"C:\\Users\\emrer\\OneDrive\\Meshes\\asteroid.obj");



	/* MATERIALS */
	//base material to load light shaders
	m_base_material = GraphicsEngine::get()->createMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	m_base_material->setCullMode(CULL_MODE_BACK);

	//spaceship
	m_space_ship_material = GraphicsEngine::get()->createMaterial(m_base_material);
	m_space_ship_material->addTexture(m_space_ship_texture);
	m_space_ship_material->setCullMode(CULL_MODE_BACK);

	//asteroid
	m_asteroid_material = GraphicsEngine::get()->createMaterial(m_base_material);
	m_asteroid_material->addTexture(m_asteroid_texture);
	m_asteroid_material->setCullMode(CULL_MODE_BACK);

	//skybox
	m_skybox_material = GraphicsEngine::get()->createMaterial(L"SkyboxVertexShader.hlsl", L"SkyboxPixelShader.hlsl");
	m_skybox_material->addTexture(m_texture_skybox);
	m_skybox_material->setCullMode(CULL_MODE_FRONT);

	m_world_cam.setTranslationMatrix(Vector3D(0, 0, -3));

	m_materials.reserve(32);
}

void SpaceShooterGame::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	//update flag
	this->update();

	this->render();
}

void SpaceShooterGame::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullscreen(false, 1, 1);
}

void SpaceShooterGame::onFocus()
{
	InputSystem::get()->addListener(this);
}

void SpaceShooterGame::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void SpaceShooterGame::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);
	this->render();
}

void SpaceShooterGame::onKeyDown(int key)
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
	//else if (key == VK_UP)
	//{
	//	m_light_radius += 4.0f * m_delta_time;
	//}
	//else if (key == VK_DOWN)
	//{
	//	m_light_radius -= 4.0f * m_delta_time;
	//}
	//else if (key == VK_CONTROL)
	//{
	//	return;
	//}
	//else if (key == VK_SPACE)
	//{
	//	retrun;
	//}
}

void SpaceShooterGame::onKeyUp(int key)
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

void SpaceShooterGame::onMouseMove(const Point& mouse_position)
{
	if (!m_resize_state) return;

	RECT window_size = this->getClientWindowRect();

	int width = window_size.right - window_size.left;
	int height = window_size.bottom - window_size.top;

	m_delta_mouse_x = (int)(mouse_position.m_x - (window_size.left + (width / 2)));
	m_delta_mouse_y = (int)(mouse_position.m_y - (window_size.top + (height / 2)));

	InputSystem::get()->setCursorPosition(Point(window_size.left + (int)width / 2, window_size.top + (int)height / 2));
}

void SpaceShooterGame::onLeftMouseButtonDown(const Point& mouse_position)
{
	is_pressed_left = true;
}

void SpaceShooterGame::onLeftMouseButtonUp(const Point& mouse_position)
{
	is_pressed_left = false;
}

void SpaceShooterGame::onRightMouseButtonDown(const Point& mouse_position)
{
	is_pressed_right = true;
}

void SpaceShooterGame::onRightMouseButtonUp(const Point& mouse_position)
{
	is_pressed_right = false;
}