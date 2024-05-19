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

class SpaceShooterGame : public Window, public InputListener
{
public:
	SpaceShooterGame();
	void update();
	~SpaceShooterGame();

	void updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_materials);
	void updateCamera();
	void updateThirdPersonCamera();
	void updateSkybox();
	void updateLight();
	void updateSpaceship();

	void render();

	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials);

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

	//textures
	TexturePtr m_texture_skybox;  //skybox
	TexturePtr m_space_ship_texture;
	TexturePtr m_asteroid_texture;

	//meshes
	MeshPtr m_skybox_mesh;  //skybox
	MeshPtr m_space_ship_mesh;
	MeshPtr m_asteroid_mesh;

	//materials
	MaterialPtr m_base_material;  //base
	MaterialPtr m_space_ship_material;
	MaterialPtr m_asteroid_material;
	MaterialPtr m_skybox_material;  //skybox

	std::vector<MaterialPtr> m_materials;

	//elapsed time calculation
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;
	float m_time = .0f;

	//rotation coefficients
	//float m_rotation_x = .0f;
	//float m_rotation_y = .0f;

	//light
	//float m_light_rotation_y = .0f;
	Vector4D m_light_position;
	Matrix4x4 m_light_rotation_matrix;

	//mouse button flags
	bool is_pressed_left = false;
	bool is_pressed_right = false;

	//mouse move
	float m_delta_mouse_x = .0f;
	float m_delta_mouse_y = .0f;

	//cam
	float m_forward = .0f;
	float m_horizontal_move_coefficient = .0f;  //right is positive due to the LH coord sys
	Vector3D m_camera_rotation;
	Vector3D m_camera_position;
	Vector3D m_spaceship_position;
	Vector3D m_spaceship_rotation;
	float m_camera_distance = 15.0f;
	float m_spaceship_speed = 150.0f;

	//world-view-porjection matrices
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_projection_cam;

	//state flags
	bool m_resize_state = false;
	bool m_fullscreen_flag = false;
};