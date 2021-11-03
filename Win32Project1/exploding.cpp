#include "exploding.h"
CExploding::CExploding(){
}
CExploding::~CExploding(){
}
void CExploding::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval, int min_nCurrentFrame, int max_nCurrentFrame, int bIsLeft, int spriteType){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_bIsLeft = bIsLeft;
	SetSprite(min_nCurrentFrame, max_nCurrentFrame, spriteType);
}
void CExploding::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x,int p_x,int y, int p_y){
	CGObject::Draw(m_x - (x - p_x), m_y - (y - p_y), m_bIsLeft, lpSurface);
}
void CExploding::Handle(){
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval)){
		if (m_spriteType == 0){
			m_bIsLive = false;
		}
	}
}