#ifndef _interface2_h_
#define _interface2_h_
#include "gobject.h"
class CInterface2 : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	CInterface2();
	~CInterface2();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int weapon);
};
#endif