// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <Windows.h>
#include "MyWin.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	CMyWin* pWin = new CMyWin(400,800);

	if(pWin)
	{
		int nret = pWin->WinRun();

		delete pWin;

		return nret;
	}
	else
		return 0;
}
