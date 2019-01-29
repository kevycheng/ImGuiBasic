#include "CImguiWindow.h"

class CMyWin : public CImguiWindow
{
public:
	CMyWin(INT nWidth, INT nHeight, std::wstring strTitle);
	virtual ~CMyWin();

	void DrawCustomContent();
protected:
private:
};