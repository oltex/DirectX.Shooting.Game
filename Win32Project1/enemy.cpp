#include "enemy.h"
CEnemy::CEnemy(){
}
CEnemy::~CEnemy(){
}
void CEnemy::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int enemyType){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_state = NULL;
	m_bIsLeft = false;
	m_xSpeed = 0;
	m_ySpeed = 0;
	SetRect(20, 10, 10, 10);
	m_enemyType = enemyType;
	switch (enemyType){
	case SOLDIER:
		m_hp = 15;
		break;
	case TANK:
		m_hp = 50;
		break;
	case HELICOPTER:
		m_hp = 50;
		m_nowSprite1 = 94;
		m_nowSprite2 = 99;
		break;
	}
	m_bIsActive = false;
}
void CEnemy::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y){
	CGObject::Draw(m_x - (x - p_x), m_y - (y - p_y), m_bIsLeft, lpSurface);
}
void CEnemy::Handle(CBlock* block, int max_block, int x){
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval)){
		if (m_x - x < 320){
			m_bIsActive = true;
		}
		if (m_x - x < -640){
			m_bIsLive = false;
		}
		switch (m_enemyType){
		case SOLDIER:
			m_x - x > 0 ? m_bIsLeft = true : m_bIsLeft = false;
			SetSprite(0, 11);
			if (m_spriteType == 0) {
				m_xSpeed = 0;
				if (m_bIsActive == true) {
					int m_state = rand() % 50;
					switch (m_state) {
					case MOVE:
						if (abs(x - m_x) > 50) {
							m_x - x > 0 ? m_xSpeed = -2 : m_xSpeed = 2;
							SetSprite(12, 23, 1);
						}
						break;
					case THROW:
						if (abs(x - m_x) > 80 && abs(x - m_x) < 160) {
							SetSprite(24, 37, 1);
						}
						break;
					case ROLL:
						if (abs(x - m_x) > 100 && abs(x - m_x) < 200) {
							SetSprite(38, 47, 1);
						}
						break;
					case CUT:
						if (abs(x - m_x) < 50) {
							SetSprite(48, 59, 1);
							break;
						}
					case CUTJUMP:
						if (abs(x - m_x) < 120 && abs(x - m_x) > 80) {
							SetSprite(60, 72, 2);
							m_x - x > 0 ? m_xSpeed = -3 : m_xSpeed = 3;
							m_ySpeed = -10;
						}
						break;
					}
				}
			}
			m_ySpeed += m_gravity;
			m_x += m_xSpeed;
			m_y += m_ySpeed;
			SetRect(20, 10, 10, 10);
			for (int i = 0; i < max_block; i++) {
				if (m_ySpeed > 0 && block[i].GetCollider(m_x, m_y, m_rect.bottom) != -1) {
					if (m_nCurrentFrame == 72){
						m_spriteType = 0;
					}
					if (m_min_nCurrentFrame == 60) {
						m_xSpeed = 0;
					}
					m_ySpeed = 0;
					m_y = block[i].GetCollider(m_x, m_y, m_rect.bottom);
				}
			}
			break;
		case TANK:
			m_bIsLeft = true;
			SetSprite(73, 76);
			if (m_spriteType == 0) {
				m_xSpeed = 0;
				if (m_bIsActive == true && m_x - x > 0) {
					int m_state = rand() % 150;
					switch (m_state) {
					case MOVE:
						if (m_x - x > 200) {
							m_xSpeed = -2;
							SetSprite(77, 82, 1);
							break;
						}
					case SHOT:
						SetSprite(83, 93, 1);
						break;
					}
				}
			}
			m_ySpeed += m_gravity;
			m_x += m_xSpeed;
			m_y += m_ySpeed;
			SetRect(20, 15, 15, 20);
			for (int i = 0; i < max_block; i++) {
				if (m_ySpeed > 0 && block[i].GetCollider(m_x, m_y, m_rect.bottom) != -1) {
					m_ySpeed = 0;
					m_y = block[i].GetCollider(m_x, m_y, m_rect.bottom);
				}
			}
			break;
		case HELICOPTER:
			m_x - x > 0 ? m_bIsLeft = true : m_bIsLeft = false;
			SetSprite(m_nowSprite1, m_nowSprite2);
			if (m_spriteType == 0) {
				m_xSpeed = 0;
				if (m_bIsActive == true) {
					int m_state = rand() % 7;
					switch (m_state) {
					case MOVE:
						if (abs(x - m_x) >= 100) {
							m_x - x > 0 ? m_xSpeed = -3 : m_xSpeed = 3;
							SetSprite(100, 105, 1);
							m_nowSprite1 = 100;
							m_nowSprite2 = 105;
						}
						break;
					case MOVE2:
						if (abs(x - m_x) < 100 && abs(x - m_x) >= 13){
							m_x - x > 0 ? m_xSpeed = -2 : m_xSpeed = 2;
							SetSprite(106, 111, 1);
							m_nowSprite1 = 106;
							m_nowSprite2 = 111;
						}
						break;
					case SHOT:
						if (abs(x - m_x) < 13){
							SetSprite(94, 99, 1);
							m_nowSprite1 = 94;
							m_nowSprite2 = 99;
						}
						break;
					}
				}
			}
			m_x += m_xSpeed;
			SetRect(20, 15, 15, 20);
			break;
		}
		if (m_y > 440){
			m_hp = 0;
		}
	}
}
bool CEnemy::GetbIsLeft() {
	return m_bIsLeft;
}
int CEnemy::GetHp(){
	return m_hp;
}
void CEnemy::SetHp(int hp){
	m_hp -= hp;
	if (m_hp < 0){
		m_hp = 0;
	}
}
int CEnemy::GetEnemyType(){
	return m_enemyType;
}