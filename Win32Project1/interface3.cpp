#include "interface3.h"
CInterface3::CInterface3(){
}
CInterface3::~CInterface3(){
}
void CInterface3::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}
void CInterface3::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int hp){
	for (int i = 0; i < hp; i++){
		m_pSprite->Drawing(false, 0, 97 + 13 * i, 77, lpSurface, true);
	}
}