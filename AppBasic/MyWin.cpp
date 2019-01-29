#include "MyWin.h"

CMyWin::CMyWin(INT nWidth, INT nHeight, std::wstring strTitle) : CImguiWindow(nWidth, nHeight, strTitle) 
{

}

CMyWin::~CMyWin()
{
}

void CMyWin::DrawCustomContent()
{
	// call base class for default testing.
	CImguiWindow::DrawCustomContent();

	// TODO: implement this function
	// draw customized win here
// 	{
// 		ImGui::Begin("Tools"); 
// 		ImGui::End();
// 	}
}