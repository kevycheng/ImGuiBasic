#include "CImguiWindow.h"

class CMyWin : public CImguiWindow
{
public:
	CMyWin(INT nWidth, INT nHeight);
	virtual ~CMyWin();

	void DrawCustomContent();
protected:
private:
};