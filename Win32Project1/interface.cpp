#include "interface.h"
CInterface::CInterface(){
}
CInterface::~CInterface(){
}
void CInterface::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}
void CInterface::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int bullet, int bomb){
	m_pSprite->Drawing(false, bullet / 100, 115, 45, lpSurface, true);
	m_pSprite->Drawing(false, bullet % 100 / 10, 135, 45, lpSurface, true);
	m_pSprite->Drawing(false, bullet % 100 % 10, 155, 45, lpSurface, true);

	m_pSprite->Drawing(false, bomb % 100 / 10, 200, 45, lpSurface, true);
	m_pSprite->Drawing(false, bomb % 100 % 10, 220, 45, lpSurface, true);
}