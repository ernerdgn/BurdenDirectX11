#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"

//struct vec3
//{
//	float x, y, z;
//};

struct vertex
{
	Vector3D pos;
	Vector3D pos1;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))  //16/32 bytes alignation
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

void AppWindow::updateQuadPos()
{
	//m_angle += 1.5707f * m_delta_time;  //(pi / 2) * delta_time 
	constant const_obj;
	const_obj.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;

	if (m_delta_pos > 1.0f) m_delta_pos = 0;

	Matrix4x4 temp;

	m_delta_scale += m_delta_time / .15f;

	//const_obj.m_world.setTranslationMatrix(Vector3D(0,0,0));
	//const_obj.m_world.setTranslationMatrix(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_pos));
	const_obj.m_world.setScaleMatrix(Vector3D::lerp(Vector3D(.5, .5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale)+1.0f)/2.0f));
	temp.setTranslationMatrix(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));
	
	const_obj.m_world *= temp;

	const_obj.m_view.setIdentityMatrix();
	const_obj.m_projection.setOrthogonalProjectionMatrix(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);

	m_constant_buffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &const_obj);  //constant buffer updating
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//vertex list[] = {  //for triangle list
	//	{-.5f, -.5f, .0f},  //pos1
	//	{.0f, .5f, .0f},  //pos2
	//	{.5f, -.5f, .0f},  //pos3
	//};

	vertex list[] = {  //for triangle strip
			//(vec3)pos_1,		(vec3)pos1_1,			(vec3)color_1			(vec3)color1_1
			//(vec3)pos_2,		(vec3)pos1_2,			(vec3)color_2			(vec3)color1_2
			//(vec3)pos_3,		(vec3)pos1_3,			(vec3)color_3			(vec3)color1_3
			//(vec3)pos_4,		(vec3)pos1_4,			(vec3)color_4			(vec3)color1_4
			{Vector3D(-.5f, -.5f, .0f),	Vector3D(-.32f, -.11f, .0f),	Vector3D(0, 0, 1),		Vector3D(1, 1, 0)},
			{Vector3D(-.5f, .5f, .0f),	Vector3D(-.11f, .78f, .0f),		Vector3D(0, 1, 0),		Vector3D(1, 0, 1)},
			{Vector3D(.5f, -.5f, .0f),	Vector3D(.75f, -.73f, .0f),		Vector3D(1, 0, 0),		Vector3D(1, 1, 0)},
			{Vector3D(.5f, .5f, .0f),	Vector3D(.88f, .77f, .0f),		Vector3D(0, 1, 1),		Vector3D(1, 0, 0)}
	};

	m_vertex_buffer = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);
	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;


	/* VERTEX SHADER */
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	
	m_vertex_shader = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vertex_buffer->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	
	GraphicsEngine::get()->releaseCompiledShader();

	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);

	/* PIXEL SHADER */
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	m_pixel_shader = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	/* CONSTANT BUFFER */
	constant const_obj;
	const_obj.m_time = 0;

	m_constant_buffer = GraphicsEngine::get()->createConstantBuffer();

	m_constant_buffer->load(&const_obj, sizeof(constant));  //constant buffer loading
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	/*clearing the render target*/
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0.125f, 0.025f, 0.125f, 0.075f);

	/*set viewport of render target (which to draw)*/
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	//GraphicsEngine::get()->setShaders();

	/* constant buffer updates */
	//const_obj.m_angle = ::GetTickCount();  //get the time(ms) elapsed since the sys has started (winAPI)
	// flag
	updateQuadPos();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vertex_shader, m_constant_buffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_pixel_shader, m_constant_buffer);

	/*implement the prepared shaders to graphic pipeline to be able to draw*/
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertex_buffer);

	/* drawing */
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vertex_buffer->getSizeVertexList(), 0);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vertex_buffer->getSizeVertexList(), 0);

	m_swap_chain->present(false);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vertex_buffer->release();
	m_swap_chain->release();
	m_vertex_shader->release();
	m_pixel_shader->release();
	GraphicsEngine::get()->release();
}
