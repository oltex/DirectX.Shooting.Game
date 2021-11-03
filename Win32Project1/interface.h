#ifndef _interface_h_
#define _interface_h_
#include "gobject.h"
class CInterface : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	CInterface();
	~CInterface();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int bullet, int bomb);
};
#endif