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

enum UIStyle{
	US_LIGHT = 0,
	US_CLASSIC = 1,
	US_DARK
};

struct WinStyle
{
	UIStyle style;
	D3DCOLOR cBackground;
};

class CImguiWindow
{
public:
	CImguiWindow(INT nWidth, INT nHeight, std::wstring strTitle);
	virtual ~CImguiWindow();

	int WinRun();

	ImageInfo* GetImageInfo(std::wstring strPath);
	ImageInfo* GetImageInfoA(std::string strPath);

	HWND GetMainWnd();

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
	D3DPRESENT_PARAMETERS m_d3dp;
	WinStyle m_uStyle;

	void CleanupDevice();
	BOOL InitializeWin(std::wstring strTitle);
	BOOL InitializeD3D();
	bool ResetD3D9(HWND hWnd, int nWidth, int nHeight);
	BOOL InitializeResource();
	void Render();
	void DrawImGuiContent();
	
	std::string& to_string(std::string& dest, std::wstring const & src);
	std::wstring& to_wstring(std::wstring& dest, std::string const & src);
};