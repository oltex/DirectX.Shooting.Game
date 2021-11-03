#include "interface2.h"
CInterface2::CInterface2(){
}
CInterface2::~CInterface2(){
}
void CInterface2::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}
void CInterface2::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int weapon){
	m_pSprite->Drawing(false, weapon, 48, 45, lpSurface, true);
}