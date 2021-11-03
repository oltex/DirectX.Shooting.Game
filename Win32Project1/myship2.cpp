#include "myship2.h"
CMyShip2::CMyShip2(){
}
CMyShip2::~CMyShip2(){
}
void CMyShip2::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}
void CMyShip2::Draw(int m_x, int m_y, bool isLeft, LPDIRECTDRAWSURFACE7 lpSurface){
	CGObject::Draw(m_x, m_y, isLeft, lpSurface);
}