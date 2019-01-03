#include "imgui.h"
#include <d3d9.h>

class CImguiWindow
{
public:
	CImguiWindow(INT nWidth, INT nHeight);
	virtual ~CImguiWindow();

	int WinRun();

protected:
	
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void DrawCustomContent();

private:
	HWND m_hWnd;
	INT  m_nWindowWidth;
	INT  m_nWindowHeight;
	INT  m_nBufferW;
	INT  m_nBufferH;
	IDirect3D9* m_pD3D9;
	IDirect3DDevice9* m_pD3dDevice9;

	void CleanupDevice();
	BOOL InitializeWin();
	BOOL InitializeD3D();
	void Render();
	void DrawImGuiContent();
};