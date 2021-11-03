#include "mymissile.h"
CMyMissile::CMyMissile(){
}
CMyMissile::~CMyMissile(){
}
void CMyMissile::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval, int s_x, int s_y, bool bIsLeft, int bulletType){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_xSpeed = s_x;
	m_ySpeed = s_y;
	m_bIsLeft = bIsLeft;
	m_bulletType = bulletType;
	m_bombTimer = 0;
	if (m_bIsLeft == true){
		m_xSpeed = -m_xSpeed;
	}
	switch (m_bulletType) {
	case BULLET:
		m_damage = 5;
		SetSprite(0, 0);
		SetRect(3, 3, 3, 3);
		break;
	case BULLET2:
		m_damage = 5;
		switch (m_ySpeed){
		case 10:
			SetSprite(15, 15);
			break;
		case 7:
			SetSprite(14, 14);
			break;
		case 5:
			SetSprite(13, 13);
			break;
		case 0:
			SetSprite(9, 9);
			break;
		case -5:
			SetSprite(10, 10);
			break;
		case -7:
			SetSprite(11, 11);
			break;
		case -10:
			SetSprite(12, 12);
			break;
		}
		SetRect(5, 5, 5, 5);
		break;
	case BOMB:
		m_damage = 10;
		SetSprite(1, 8);
		m_jump = s_y;
		SetRect(5, 5, 5, 5);
		break;
	}
}
void CMyMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y){
	CGObject::Draw(m_x - (x - p_x), m_y - (y - p_y), m_bIsLeft, lpSurface);
}
void CMyMissile::Handle(CBlock* block, int max_block, int x, int y){
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval)){
		switch (m_bulletType) {
		case BULLET:
			m_x += m_xSpeed;
			m_y += m_ySpeed;
			SetRect(3, 3, 3, 3);
			break;
		case BULLET2:
			m_x += m_xSpeed;
			m_y += m_ySpeed;
			SetRect(5, 5, 5, 5);
			break;
		case BOMB:
			m_x += m_xSpeed;
			m_ySpeed += m_gravity;
			m_y += m_ySpeed;
			SetRect(5, 5, 5, 5);
			m_bombTimer++;
			break;
		}


		for (int i = 0; i < max_block; i++){
			if (block[i].GetCollider(m_x, m_y, m_rect.bottom) != -1){
				switch (m_bulletType){
				case BULLET:
					if (block[i].GetPasssBullet() == false){
						m_bIsLive = false;
					}
					break;
				case BULLET2:
					if (block[i].GetPasssBullet() == false){
						m_bIsLive = false;
					}
					break;
				case BOMB:
					if (m_ySpeed > 0){
						if (m_xSpeed > 0){
							m_xSpeed--;
						}
						if (m_xSpeed < 0){
							m_xSpeed++;
						}
						if (m_xSpeed == 0){
							SetSprite(m_nCurrentFrame, m_nCurrentFrame);
						}
						m_ySpeed = 0;
						m_ySpeed = m_jump * 2 / 2;
						m_jump /= 2;
						m_y = block[i].GetCollider(m_x, m_y, m_rect.bottom);
					}
					break;
				}
			}
		}
		if (abs(x - m_x) > 320 || abs(y - m_y) > 300){
			m_bIsLive = false;
		}
	}
}
int CMyMissile::GetBulletType(){
	return m_bulletType;
}
int CMyMissile::GetBombTimer(){
	return m_bombTimer;
}
int CMyMissile::GetDamage(){
	return m_damage;
}