#ifndef _interface3_h_
#define _interface3_h_
#include "gobject.h"
class CInterface3 : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	CInterface3();
	~CInterface3();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int hp);
};
#endif