#include "enemymissile.h"
CEnemyMissile::CEnemyMissile(){
}
CEnemyMissile::~CEnemyMissile(){
}
void CEnemyMissile::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int s_x, int s_y, bool bIsLeft, int bulletType){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_xSpeed = s_x;
	m_ySpeed = s_y;
	m_bIsLeft = bIsLeft;
	m_bulletType = bulletType;
	if (m_bIsLeft == true) {
		m_xSpeed = -m_xSpeed;
	}
	switch (m_bulletType) {
	case THROWBOMB:
		m_damage = 1;
		SetSprite(1, 15);
		SetRect(5, 5, 5, 5);
		break;
	case ROLLBOMB:
		m_damage = 1;
		SetSprite(16, 23);
		SetRect(5, 5, 5, 5);
		break;
	case TANKBULLET:
		m_damage = 2;
		SetSprite(24, 27);
		SetRect(12, 5, 5, 12);
		break;
	case HELICOPTERBULLET:
		m_damage = 1;
		SetSprite(28, 29);
		SetRect(2, 2, 2, 2);
		break;
	}
}
void CEnemyMissile::Handle(CBlock* block, int max_block, int x, int y){
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval)){
		switch (m_bulletType) {
		case THROWBOMB:
			m_x += m_xSpeed;
			m_ySpeed += m_gravity;
			m_y += m_ySpeed;
			SetRect(5, 5, 5, 5);
			break;
		case ROLLBOMB:
			m_x += m_xSpeed;
			m_ySpeed += m_gravity;
			m_y += m_ySpeed;
			SetRect(5, 5, 5, 5);
			break;
		case TANKBULLET:
			m_x += m_xSpeed;
			m_y += m_ySpeed;
			SetRect(12, 5, 5, 12);
			break;
		case HELICOPTERBULLET:
			m_x += m_xSpeed;
			m_y += m_ySpeed;
			SetRect(2, 2, 2, 2);
			break;
		}

		for (int i = 0; i < max_block; i++) {
			if (block[i].GetCollider(m_x, m_y, m_rect.bottom) != -1) {
				switch (m_bulletType) {
				case THROWBOMB:
				case TANKBULLET:
				case HELICOPTERBULLET:
					if (block[i].GetPasssBullet() == false) {
						m_bIsLive = false;
					}
					break;
				case ROLLBOMB:
						m_ySpeed = 0;
						m_y = block[i].GetCollider(m_x, m_y, m_rect.bottom);
					break;
				}
			}
		}
		if (abs(x - m_x) > 320 || abs(y - m_y) > 300) {
			m_bIsLive = false;
		}
	}
}
void CEnemyMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y){
	CGObject::Draw(m_x - (x - p_x), m_y - (y - p_y), m_bIsLeft, lpSurface);
}
int CEnemyMissile::GetDamage(){
	return m_damage;
}