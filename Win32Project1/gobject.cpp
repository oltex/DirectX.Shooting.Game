#include "gobject.h"
CGObject::CGObject(){
	m_bIsLive = false;
}
CGObject::~CGObject(){
}
void CGObject::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval){
	m_pSprite = pSprite;
	m_nCurrentFrame = CurrentFrame;
	m_x = x;
	m_y = y;
	m_nFrameInterval = FrameInterval;
	m_pTimer = timer;
	m_nLastFrameTime = m_pTimer->time();
	m_bIsLive = true;
}
bool CGObject::IsLive(){
	return m_bIsLive;
}
void CGObject::Kill(){
	m_bIsLive = false;
}
void CGObject::Draw(int x, int y, bool isLeft, LPDIRECTDRAWSURFACE7 lpSurface){
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval)){
		if (m_nCurrentFrame == m_max_nCurrentFrame && m_spriteType == 2){
			m_nCurrentFrame--;
		}
		if (m_nCurrentFrame >= m_max_nCurrentFrame || m_nCurrentFrame < m_min_nCurrentFrame){
			m_nCurrentFrame = m_min_nCurrentFrame - 1;
		}
		m_nCurrentFrame++;
		if (m_nCurrentFrame == m_max_nCurrentFrame && m_spriteType == 1)
			m_spriteType = 0;
	}
	m_pSprite->Drawing(isLeft, m_nCurrentFrame, x, y, lpSurface);
}
void CGObject::SetSprite(int min_nCurrentFrame, int max_nCurrentFrame){
	if (m_spriteType == 0){
		m_min_nCurrentFrame = min_nCurrentFrame;
		m_max_nCurrentFrame = max_nCurrentFrame;
	}
}
void CGObject::SetSprite(int min_nCurrentFrame, int max_nCurrentFrame, int SpriteType){
	m_nCurrentFrame = min_nCurrentFrame;
	m_min_nCurrentFrame = min_nCurrentFrame;
	m_max_nCurrentFrame = max_nCurrentFrame;
	m_spriteType = SpriteType;
}
void CGObject::SetRect(int bottom, int left, int right, int top){
	m_rect.bottom = m_y + bottom;
	m_rect.left = m_x - left;
	m_rect.right = m_x + right;
	m_rect.top = m_y - top;
}
int CGObject::GetX(){
	return m_x;
}
int CGObject::GetY(){
	return m_y;
}
int CGObject::GetCurrentFrame(){
	return m_nCurrentFrame;
}
RECT CGObject::GetRect(){
	return m_rect;
}