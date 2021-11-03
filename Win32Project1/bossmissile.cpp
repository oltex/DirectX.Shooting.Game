#include "bossmissile.h"
CBossMissile::CBossMissile(){
}
CBossMissile::~CBossMissile(){
}
void CBossMissile::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int s_x, int s_y, bool bIsLeft, int bulletType){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_xSpeed = s_x;
	m_ySpeed = s_y;
	m_bIsLeft = bIsLeft;
	m_bulletType = bulletType;
	switch (m_bulletType) {
	case BOSSBULLET1:
		m_damage = 2;
		SetSprite(0, 3);
		SetRect(5, 5, 5, 5);
		break;
	case BOSSBULLET2:
		m_damage = 1;
		SetSprite(4, 15);
		SetRect(3, 3, 3, 3);
		break;
	}
}
void CBossMissile::Handle(CBlock* block, int max_block, int x, int y){
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval)){
		switch (m_bulletType) {
		case BOSSBULLET1:
			m_x += m_xSpeed;
			m_y += m_ySpeed;
			SetRect(5, 5, 5, 5);
			break;
		case BOSSBULLET2:
			m_x += m_xSpeed;
			m_y += m_ySpeed;
			SetRect(3, 3, 3, 3);
			break;
		}
		for (int i = 0; i < max_block; i++) {
			if (block[i].GetCollider(m_x, m_y, m_rect.bottom) != -1) {
				if (block[i].GetPasssBullet() == false) {
					m_bIsLive = false;
				}
			}
		}
		if (abs(x - m_x) > 640 || abs(y - m_y) > 300) {
			m_bIsLive = false;
		}
	}
}
void CBossMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y){
	CGObject::Draw(m_x - (x - p_x), m_y - (y - p_y), m_bIsLeft, lpSurface);
}
int CBossMissile::GetDamage(){
	return m_damage;
}