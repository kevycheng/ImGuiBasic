#include "MyWin.h"

CMyWin::CMyWin(INT nWidth, INT nHeight) : CImguiWindow(nWidth, nHeight) 
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