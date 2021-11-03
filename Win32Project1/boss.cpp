#include "boss.h"
CBoss::CBoss(){
}
CBoss::~CBoss(){
}
void CBoss::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_xSpeed = 0;
	m_ySpeed = 0;
	m_hp = 600;
	SetRect(70, 50, 50, 30);
	m_bIsLeft = true;
	m_bIsStanding = false;
	m_bIsHandling = false;
}
void CBoss::Handle(CBlock* block, int max_block, int x){
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval)){
		SetSprite(0, 8);
		if (abs(x - m_x) < 320) {
			m_bIsHandling = true;
		}
		if (m_spriteType == 0) {
			m_xSpeed = 0;
			if (m_bIsHandling == true) {
				if (m_bIsStanding == false){
					int m_state = rand() % 30;
					switch (m_state) {
					case BOSSMOVE:
						if (m_x - x > 0) {
							m_xSpeed = -1;
							SetSprite(26, 42, 1);
						}
						break;
					case BOSSMOVE2:
						if (m_x - x < 0) {
							SetSprite(91, 107, 1);
							m_xSpeed = 1;
						}
						break;
					case BOSSSHOT:
						if (m_x - x > 100) {
							SetSprite(9, 25, 1);
						}
						break;
					case BOSSSTANDINGUP:
						if (abs(m_x - x) <= 100) {
							SetSprite(43, 66, 1);
							m_bIsStanding = true;
						}
						break;
					}
				}
				else{
					int m_state = rand() % 2 + 4;
					switch (m_state) {
					case BOSSSTANDINGMOVE:
						if (m_x - x > 50) {
							m_xSpeed = -1;
							SetSprite(67, 75, 1);
						}
						break;
					case BOSSSTANDINGSMASHY:
						if (m_x - x < 50) {
							SetSprite(75, 90, 1);
							m_bIsStanding = false;
						}
						break;
					}
				}
			}
		}
		m_ySpeed += m_gravity;
		m_x += m_xSpeed;
		m_y += m_ySpeed;
		if (m_x > 4090){
			m_x = 4090;
		}
		SetRect(70, 50, 50, 30);
		for (int i = 0; i < max_block; i++) {
			if (m_ySpeed > 0 && block[i].GetCollider(m_x, m_y, m_rect.bottom) != -1) {
				m_ySpeed = 0;
				m_y = block[i].GetCollider(m_x, m_y, m_rect.bottom);
			}
		}
	}
}
void CBoss::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y){
	CGObject::Draw(m_x - (x - p_x), m_y - (y - p_y), m_bIsLeft, lpSurface);
}

void CBoss::SetHp(int hp){
	m_hp -= hp;
	if (m_hp < 0){
		m_hp = 0;
	}
}
int CBoss::GetHp(){
	return m_hp;
}
bool CBoss::GetbIsLeft(){
	return m_bIsLeft;
}