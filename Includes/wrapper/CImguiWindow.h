#include "imgui.h"
#include <d3d9.h>
#include <string>
#include <map>

struct ImageInfo
{
	std::wstring m_strPath;
	INT m_nW;
	INT m_nH;
	LPDIRECT3DTEXTURE9 m_pTexture;
};

class CImguiWindow
{
public:
	CImguiWindow(INT nWidth, INT nHeight);
	virtual ~CImguiWindow();

	int WinRun();

	ImageInfo* GetImageInfo(std::wstring strPath);

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
	std::map<std::wstring, ImageInfo*> m_mapTexture;

	void CleanupDevice();
	BOOL InitializeWin();
	BOOL InitializeD3D();
	BOOL InitializeResource();
	void Render();
	void DrawImGuiContent();
	
};