#ifndef _myship2_h_
#define _myship2_h_
#include "gobject.h"
enum state{};
class CMyShip2 : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	CMyShip2();
	~CMyShip2();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(int m_x, int m_y, bool isLeft, LPDIRECTDRAWSURFACE7 lpSurface);
};
#endif