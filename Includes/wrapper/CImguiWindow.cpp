#include <Windows.h>
#include <Windowsx.h>
#include "CImguiWindow.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#pragma	comment(lib, "D3dx9.lib")
#pragma	comment(lib, "D3d9.lib")

#include <string>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define SAFE_RELEASE(p) { if (p)  { p->Release(); p = NULL; } }

CImguiWindow::CImguiWindow(INT nWidth, INT nHeight) :
m_nWindowWidth(nWidth)
, m_nWindowHeight(nHeight)
, m_hWnd(NULL)
, m_pD3D9(NULL)
, m_pD3dDevice9(NULL)
, m_nBufferW(0)
, m_nBufferH(0)
{
	// initialize window
	if(!InitializeWin())
		return;

	// initialize d3d9
	if(!InitializeD3D())
		CleanupDevice();
}

CImguiWindow::~CImguiWindow()
{
	// cleanup
	CleanupDevice();
}

void CImguiWindow::DrawCustomContent()
{
	bool bshow = true;

	// draw customized win here
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::Begin("Tools", &bshow, window_flags);                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("some tools: ");               // Display some text (you can use a format strings too)

		ImGui::Columns(3, "horizontalLayout", false);
		ImGui::Button("first"); 
		ImGui::NextColumn();
		ImGui::Button("second");
		ImGui::NextColumn();
		ImGui::Button("third");
		ImGui::Columns(1);

		ImGui::Separator();

		// a column area
		ImGui::Columns(2, "word-wrapping", false);
		ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
		ImGui::TextWrapped("Hello Left");
		ImGui::NextColumn();
		ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
		ImGui::TextWrapped("Hello Right");
		ImGui::Columns(1);

		ImGui::Separator();

		static char buf1[64] = ""; ImGui::InputText("", buf1, 64); ImGui::SameLine();
		ImGui::Button("Submit");
		ImGui::Text("input data : %s", buf1);

		ImGui::Separator();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}
}

void CImguiWindow::DrawImGuiContent()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(m_nBufferW,m_nBufferH), ImGuiCond_Always);

	DrawCustomContent();

	ImGui::EndFrame();
	// end of imgui drawing
}

void CImguiWindow::Render()
{
	if(!m_pD3dDevice9)
		return;

	DrawImGuiContent();

	// draw your own content here.
	m_pD3dDevice9->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA( 255, 255, 255, 0 ), 1.0f, 0 );

	m_pD3dDevice9->BeginScene();

	// output igui
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	m_pD3dDevice9->EndScene();

	m_pD3dDevice9->Present( NULL, NULL, NULL, NULL );
}

int CImguiWindow::WinRun()
{
	MSG msg = {0};
	
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			Render();
		}
	}

	return (int) msg.wParam;
}

void CImguiWindow::CleanupDevice()
{
	// imgui destructor
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SAFE_RELEASE(m_pD3dDevice9);
	SAFE_RELEASE(m_pD3D9);
}

BOOL CImguiWindow::InitializeWin()
{
	if(!m_nWindowWidth || !m_nWindowHeight)
		return FALSE;

	// register winclass
	WNDCLASSEXW wndclass;

	wndclass.cbSize			= sizeof(wndclass);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc	= WindowProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= L"ImguiWin";
	wndclass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassExW(&wndclass);

	// create win
	m_hWnd = CreateWindowExW( WS_EX_APPWINDOW, L"ImguiWin", L"ImguiWin", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_nWindowWidth, m_nWindowHeight,
		NULL, NULL, NULL, NULL);

	// create a borderless window
// 	m_hWnd = CreateWindowExW(  WS_EX_APPWINDOW, L"ImguiWin", L"ImguiWin", 
// 		WS_POPUP, 
// 		CW_USEDEFAULT, CW_USEDEFAULT,
// 		m_nWindowWidth, m_nWindowHeight,
// 		NULL, NULL, NULL, NULL);

	if(!m_hWnd)
		return FALSE;

	// store wnidow as userdata
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

BOOL CImguiWindow::InitializeD3D()
{
	if(!m_hWnd)
		return FALSE;

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nBufferW = rcClient.right - rcClient.left;
	m_nBufferH = rcClient.bottom - rcClient.top;

	do 
	{
		m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if(!m_pD3D9)
			break;

		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof( d3dpp ) );
		d3dpp.Windowed = TRUE;
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferCount	= 1;
		d3dpp.BackBufferFormat	= D3DFMT_A8R8G8B8;
		d3dpp.Flags				= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		d3dpp.MultiSampleType	= D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality= 0;
		d3dpp.hDeviceWindow = m_hWnd;
		d3dpp.BackBufferWidth = m_nBufferW;
		d3dpp.BackBufferHeight = m_nBufferH;

		if(FAILED(m_pD3D9->CreateDevice( 0, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3dDevice9 )))
			break;

		// init imgui
		if(m_pD3dDevice9)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
			ImGui_ImplWin32_Init(m_hWnd);
			ImGui_ImplDX9_Init(m_pD3dDevice9);

			ImGui::StyleColorsLight();
		}

		return TRUE;

	} while (FALSE);

	return FALSE;
}

LRESULT CALLBACK CImguiWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	CImguiWindow* pObj = NULL;
	pObj = (CImguiWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	//Add these two static variables
	static int xClick;
	static int yClick;
	static bool bLButton;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		//Restrict mouse input to current window
		SetCapture(hWnd);

		//Get the click position
		xClick = LOWORD(lParam);
		yClick = HIWORD(lParam);
		bLButton = true;
		break;

	case WM_LBUTTONUP:
		//Window no longer requires all mouse input
		ReleaseCapture();
		bLButton = false;
		break;

	case WM_MOUSEMOVE:
		{
			if (bLButton && GetCapture() == hWnd)  //Check if this window has mouse input
			{
				//Get the window's screen coordinates
				RECT rcWindow;
				GetWindowRect(hWnd,&rcWindow);

				//Get the current mouse coordinates
				int xMouse = GET_X_LPARAM(lParam);
				int yMouse = GET_Y_LPARAM(lParam);

				//Calculate the new window coordinates
				int xWindow = rcWindow.left + xMouse - xClick;
				int yWindow = rcWindow.top + yMouse - yClick;
				
				//Set the window's new screen position (don't resize or change z-order)
				SetWindowPos(hWnd,NULL,xWindow,yWindow,0,0,SWP_NOSIZE|SWP_NOZORDER);
			}
			break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}