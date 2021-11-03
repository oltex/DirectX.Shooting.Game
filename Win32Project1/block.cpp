#include "block.h"
CBlock::CBlock(){
}
CBlock::~CBlock(){
}
void CBlock::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int bottom, int left, int right, int top, int startY, int endY, bool passBullet){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_rect.bottom = bottom;
	m_rect.left = left;
	m_rect.right = right;
	m_rect.top = top;
	m_startY = startY;
	m_endY = endY;
	m_passBullet = passBullet;
}
int CBlock::GetCollider(int x, int y, int b_y){
	if (x >= m_rect.left && x < m_rect.right && b_y > m_rect.top && b_y < m_rect.bottom){
		if (m_startY != NULL){
			if (b_y > m_startY - (int)((float)(x - m_rect.left) / (float)(m_rect.right - m_rect.left) * (float)(m_startY - m_endY))){
				return m_startY - (b_y -  y) - (int)((float)(x - m_rect.left) / (float)(m_rect.right - m_rect.left) * (float)(m_startY - m_endY));
			}
		}
		else{
			return m_rect.top - (b_y - y);
		}
	}
	return -1;
}
bool CBlock::GetPasssBullet(){
	return m_passBullet;
}