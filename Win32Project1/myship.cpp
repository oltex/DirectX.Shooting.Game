#include "myship.h"
CMyShip::CMyShip(){
}
CMyShip::~CMyShip(){
}
void CMyShip::Initialize(CMyShip2* pCMyShip2, CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval){
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_pMyShip2 = pCMyShip2;
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	m_bIsLeft = false;
	m_bIsWeapon = false;
	m_ySpeed = 0;
	m_diagonal = 0;
	m_hp = 10;
	m_bullet = 0;
	m_bomb = 10;
	m_lastX = 320;
	SetRect(20, 10, 10, 10);
	SetSprite(0, 11);
}
void CMyShip::Draw(LPDIRECTDRAWSURFACE7 lpSurface,int y, int p_y){
	if (m_bNeedLowerBody == true){
		m_pMyShip2->Draw(p_x, m_y - (y - p_y), m_bIsLeft, lpSurface);
	}
	if (m_bCompulsionDraw == true){
		if (m_bIsUp == true){
			switch (m_diagonal){
			case 1:
				m_pSprite->Drawing(m_bIsLeft, 225, p_x, m_y - (y - p_y), lpSurface, true);
				break;
			case 2:
				m_pSprite->Drawing(m_bIsLeft, 226, p_x, m_y - (y - p_y), lpSurface, true);
				break;
			case -1:
				m_pSprite->Drawing(m_bIsLeft, 241, p_x, m_y - (y - p_y), lpSurface, true);
				break;
			case -2:
				m_pSprite->Drawing(m_bIsLeft, 242, p_x, m_y - (y - p_y), lpSurface, true);
				break;
			}
		}
		if (m_bIsUp == false){
			switch (m_diagonal){
			case 1:
				m_pSprite->Drawing(m_bIsLeft, 228, p_x, m_y - (y - p_y), lpSurface, true);
				break;
			case 2:
				m_pSprite->Drawing(m_bIsLeft, 227, p_x, m_y - (y - p_y), lpSurface, true);
				break;
			case -1:
				m_pSprite->Drawing(m_bIsLeft, 244, p_x, m_y - (y - p_y), lpSurface, true);
				break;
			case -2:
				m_pSprite->Drawing(m_bIsLeft, 243, p_x, m_y - (y - p_y), lpSurface, true);
				break;
			}
		}
	}
	else{
		CGObject::Draw(p_x, m_y - (y - p_y), m_bIsLeft, lpSurface);
	}
}
void CMyShip::Handle(CBlock* block, int max_block){
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval)){
		if (m_bIsLeft == true && m_bIsMove == true){
			if (m_bIsJump == true){
				m_x -= 3;
			}
			else if (m_bIsDown == true && m_bIsJump == false && m_spriteType == 0){
				m_x -= 2;
			}
			else if (m_bIsDown == false){
				m_x -= 3;
			}
		}
		if (m_bIsLeft == false && m_bIsMove == true){
			if (m_bIsJump == true){
				m_x += 3;
			}
			else if (m_bIsDown == true && m_bIsJump == false && m_spriteType == 0){
				m_x += 2;
			}
			else if (m_bIsDown == false){
				m_x += 3;
			}

		}
		if (m_bIsWeapon == true && m_bIsUp == false && m_bIsDown == false){
			if (m_diagonal > 0){
				m_diagonal--;
			}
		}
		if (m_bIsWeapon == true && m_bIsUp == true && m_bIsDown == false){
			if (m_diagonal < 3){
				m_diagonal++;
			}
		}
		if (m_bIsWeapon == true && m_bIsDown == false && m_bIsUp == false && m_bIsJump == true){
			if (m_diagonal < 0){
				m_diagonal++;
			}
		}
		if (m_bIsWeapon == true && m_bIsDown == true && m_bIsUp == false && m_bIsJump == true){
			if (m_diagonal > -3){
				m_diagonal--;
			}
		}
		p_x = m_x;
		if (m_x > m_lastX){
			m_lastX = m_x;
		}
		if (m_lastX < 320){
			m_lastX = 320;
		}
		if (m_lastX > 3520){
			m_lastX++;
		}
		if (m_lastX > 3840){
			m_lastX = 3840;
		}
		if (m_x < m_lastX - 300){
			m_x = m_lastX - 300;
		}
		if (m_x > 4140){
			m_x = 4140;
		}
		if (m_x < m_lastX){
			p_x = 320 + (m_x - m_lastX);
		}
		if (m_x >= m_lastX && m_x <= 3840) {
			p_x = 320;
		}
		if (m_x > 3840) {
			p_x = 320 + (m_x - 3840);
		}
		SetRect(20, 10, 10, 10);
		if (m_bIsDown == true && m_bIsJump == false){
			SetRect(20, 10, 10, 5);
		}
		m_ySpeed += m_gravity;
		m_y += m_ySpeed;
		for (int i = 0; i < max_block; i++){
			if (m_ySpeed > 0 && block[i].GetCollider(m_x, m_y, m_rect.bottom) != -1){
				if (m_bIsJump)
					m_spriteType = 0;
				m_bIsJump = false;
				m_ySpeed = 0;
				m_pMyShip2->m_spriteType = 0;
				m_y = block[i].GetCollider(m_x, m_y, m_rect.bottom);
			}
		}
		if (m_y > 440){
			m_hp = 0;
		}
	}

	if (m_bIsUp == true && m_bIsDown == false && (m_min_nCurrentFrame == 36 || m_min_nCurrentFrame == 68 || m_min_nCurrentFrame == 80 || m_min_nCurrentFrame == 211 || m_min_nCurrentFrame == 217)){
		m_spriteType = 0;
	}
	if (m_bIsDown == true && m_bIsJump == true && (m_min_nCurrentFrame == 211 || m_min_nCurrentFrame == 217)){
		m_spriteType = 0;
	}
	if (m_bIsDown == true && (m_min_nCurrentFrame == 36 || m_min_nCurrentFrame == 120 || m_min_nCurrentFrame == 130)){
		m_spriteType = 0;
	}
	if (m_bIsDown == false && (m_min_nCurrentFrame == 99 || m_min_nCurrentFrame == 125 || m_min_nCurrentFrame == 135)){
		m_spriteType = 0;
	}
	if (m_bIsDown == true && m_bIsMove == true && m_bIsShot == false && m_min_nCurrentFrame == 99){
		m_spriteType = 0;
	}
	if (m_bIsDown == true && m_bIsJump == false){
		m_bNeedLowerBody = false;
	}



	if (m_bIsMove == false && m_bIsUp == false && m_bIsDown == false && m_bIsShot == false && m_bIsWeapon == false){
		SetSprite(0, 11);
	}
	if (m_bIsMove == false && m_bIsUp == false && m_bIsDown == false && m_bIsShot == false && m_bIsWeapon == true){
		switch (m_diagonal){
		case 0:
			SetSprite(140, 151);
			break;
		case 1:
			SetSprite(223, 223);
			break;
		case 2:
			SetSprite(224, 224);
			break;
		}
	}
	if (m_bIsMove == false){
		m_pMyShip2->SetSprite(0, 14);
	}
	if (m_bIsMove == true && m_bIsUp == false && m_bIsDown == false && m_bIsWeapon == false){
		SetSprite(12, 23);
	}
	if (m_bIsMove == true && m_bIsUp == false && m_bIsDown == false && m_bIsWeapon == true){
		switch (m_diagonal){
		case 0:
			SetSprite(152, 163);
			break;
		case 1:
			SetSprite(223, 223);
			break;
		case 2:
			SetSprite(224, 224);
			break;
		}
	}
	if (m_bIsMove == true){
		m_pMyShip2->SetSprite(15, 32);
	}
	if (m_bIsUp == true && m_bIsDown == false && m_bIsWeapon == false){
		SetSprite(24, 35);
	}
	if (m_bIsUp == true && m_bIsDown == false && m_bIsWeapon == true){
		switch (m_diagonal){
		case 1:
			SetSprite(223, 223);
			break;
		case 2:
			SetSprite(224, 224);
			break;
		case 3:
			SetSprite(164, 175);
			break;
		}
	}
	if (m_bIsDown == true && m_bIsMove == false && m_bIsUp == false && m_bIsWeapon == false){
		SetSprite(56, 67);
	}
	if (m_bIsDown == true && m_bIsMove == false && m_bIsUp == false && m_bIsWeapon == true && m_bIsJump == false){
		SetSprite(176, 187);
	}
	if (m_bIsDown == true && m_bIsMove == true && m_bIsUp == false && m_bIsWeapon == false){
		SetSprite(109, 119);
	}
	if (m_bIsDown == true && m_bIsMove == true && m_bIsUp == false && m_bIsWeapon == true && m_bIsJump == false){
		SetSprite(188, 198);
	}
	if (m_bIsDown == true && m_bIsJump == true && m_bIsUp == false && m_bIsWeapon == true){
		switch (m_diagonal){
		case -1:
			SetSprite(239, 239);
			break;
		case -2:
			SetSprite(240, 240);
			break;
		case -3:
			SetSprite(233, 238);
			break;
		}
	}
	if (m_bIsJump == true && m_bIsMove == false && m_bIsUp == false && m_bIsWeapon == false && !(m_min_nCurrentFrame == 68 || m_min_nCurrentFrame == 36 || m_min_nCurrentFrame == 46 || m_min_nCurrentFrame == 92 || m_min_nCurrentFrame == 80 || m_min_nCurrentFrame == 120 || m_min_nCurrentFrame == 125 || m_min_nCurrentFrame == 130 || m_min_nCurrentFrame == 135)){
		SetSprite(68, 79, 2);
	}
	if (m_bIsJump == true && m_bIsMove == false && m_bIsUp == false && m_bIsDown == false && m_bIsWeapon == true && !(m_min_nCurrentFrame == 211 || m_min_nCurrentFrame == 199 || m_min_nCurrentFrame == 207 || m_min_nCurrentFrame == 223 || m_min_nCurrentFrame == 224 || m_min_nCurrentFrame == 239 || m_min_nCurrentFrame == 240)){
		SetSprite(211, 216, 2);
	}
	if (m_bIsJump == true && m_bIsMove == true && m_bIsUp == false && m_bIsWeapon == false && !(m_min_nCurrentFrame == 80 || m_min_nCurrentFrame == 36 || m_min_nCurrentFrame == 46 || m_min_nCurrentFrame == 92 || m_min_nCurrentFrame == 68 || m_min_nCurrentFrame == 120 || m_min_nCurrentFrame == 125 || m_min_nCurrentFrame == 130 || m_min_nCurrentFrame == 135)){
		SetSprite(80, 91, 2);
	}
	if (m_bIsJump == true && m_bIsMove == true && m_bIsUp == false && m_bIsDown == false && m_bIsWeapon == true && !(m_min_nCurrentFrame == 217 || m_min_nCurrentFrame == 199 || m_min_nCurrentFrame == 207 || m_min_nCurrentFrame == 223 || m_min_nCurrentFrame == 224 || m_min_nCurrentFrame == 239 || m_min_nCurrentFrame == 240)){
		SetSprite(217, 222, 2);
	}
	if (m_bIsJump == true && m_bIsMove == true && m_pMyShip2->m_min_nCurrentFrame != 33 && m_pMyShip2->m_min_nCurrentFrame != 45){
		m_pMyShip2->SetSprite(45, 56, 2);
	}
	if (m_bIsJump == true && m_bIsMove == false && m_pMyShip2->m_min_nCurrentFrame != 45 && m_pMyShip2->m_min_nCurrentFrame != 33){
		m_pMyShip2->SetSprite(33, 44, 2);
	}
	if (m_bIsShot == true && m_bIsUp == false && m_bIsDown == false && m_bIsWeapon == false){
		SetSprite(36, 45, 1);
	}
	if (m_bIsShot == true && m_bIsUp == false && m_bIsDown == false && m_bIsWeapon == true){
		SetSprite(199, 202, 1);
	}
	if (m_bIsShot == true && m_bIsUp == true && m_bIsDown == false && m_bIsWeapon == false){
		SetSprite(46, 55, 1);
	}
	if (m_bIsShot == true && m_bIsUp == true && m_bIsDown == false && m_bIsWeapon == true){
		SetSprite(207, 210, 1);
	}
	if (m_bIsShot == true && m_bIsDown == true && m_bIsJump == false && m_bIsWeapon == false){
		SetSprite(99, 108, 1);
	}
	if (m_bIsShot == true && m_bIsDown == true && m_bIsJump == false && m_bIsWeapon == true){
		SetSprite(203, 206, 1);
	}
	if (m_bIsShot == true && m_bIsDown == true && m_bIsJump == true && m_bIsWeapon == false){
		SetSprite(92, 98, 1);
	}
	if (m_bIsShot == true && m_bIsDown == true && m_bIsJump == true && m_bIsWeapon == true){
		SetSprite(229, 232, 1);
	}
	if (m_bIsBomb == true && m_bIsDown == false && m_bIsShot == false && m_bIsCut == false){
		SetSprite(120, 124, 1);
	}
	if (m_bIsBomb == true && m_bIsDown == false && m_bIsShot == false && m_bIsCut == false && m_bIsWeapon == true){
		SetSprite(245, 249, 1);
	}
	if (m_bIsBomb == true && m_bIsDown == true && m_bIsJump == false && m_bIsWeapon == false && m_bIsShot == false && m_bIsCut == false){
		SetSprite(125, 129, 1);
	}
	if (m_bIsBomb == true && m_bIsDown == true && m_bIsJump == false && m_bIsWeapon == true && m_bIsShot == false && m_bIsCut == false){
		SetSprite(250, 254, 1);
	}
	if (m_bIsCut == true && m_bIsDown == false && m_bIsShot == false && m_bIsBomb == false && m_bIsWeapon == false){
		SetSprite(130, 134, 1);
	}
	if (m_bIsCut == true && m_bIsDown == false && m_bIsShot == false && m_bIsBomb == false && m_bIsWeapon == true){
		SetSprite(255, 259, 1);
	}
	if (m_bIsCut == true && m_bIsDown == true && m_bIsJump == false && m_bIsShot == false && m_bIsBomb == false && m_bIsWeapon == false){
		SetSprite(135, 139, 1);
	}
	if (m_bIsCut == true && m_bIsDown == true && m_bIsJump == false && m_bIsShot == false && m_bIsBomb == false && m_bIsWeapon == true){
		SetSprite(260, 264, 1);
	}



	if (m_min_nCurrentFrame == 199 && m_diagonal > 0){
		m_bCompulsionDraw = true;
	}
	if (m_min_nCurrentFrame == 199 && m_diagonal == 3){
		SetSprite(207, 210, 1);
	}
	if (m_min_nCurrentFrame == 207 && m_diagonal < 3 && m_diagonal > 0){
		m_bCompulsionDraw = true;
	}
	if (m_min_nCurrentFrame == 207 && m_diagonal == 0){
		SetSprite(199, 202, 1);
	}
	if (m_min_nCurrentFrame == 199 && m_diagonal < 0){
		m_bCompulsionDraw = true;
	}
	if (m_min_nCurrentFrame == 199 && m_diagonal == -3){
		SetSprite(229, 232, 1);
	}
	if (m_min_nCurrentFrame == 229 && m_diagonal > -3 && m_diagonal < 0){
		m_bCompulsionDraw = true;
	}
}
void CMyShip::Stand(){
	if (!m_bIsLive)
		return;
	m_bIsMove = false;
	m_bIsUp = false;
	m_bIsDown = false;
	m_bIsShot = false;
	m_bIsBomb = false;
	m_bIsCut = false;
	m_bNeedLowerBody = true;
	m_bCompulsionDraw = false;
	m_bIsWeapon = m_bullet > 0 ? true : false;
}
void CMyShip::Left(){
	if (!m_bIsLive)
		return;
	m_bIsLeft = true;
	m_bIsMove = true;
}
void CMyShip::Right(){
	if (!m_bIsLive)
		return;
	m_bIsLeft = false;
	m_bIsMove = true;
}
void CMyShip::Up(){
	if (!m_bIsLive)
		return;
	m_bIsUp = true;
}
void CMyShip::Down(){
	if (!m_bIsLive)
		return;
	m_bIsDown = true;
}
void CMyShip::Jump(){
	if (!m_bIsLive)
		return;
	if (m_bIsJump == false){
		m_bIsJump = true;
		m_ySpeed = m_jump;
	}
}
void CMyShip::Shot(){
	if (!m_bIsLive)
		return;
	m_bIsShot = true;
}
void CMyShip::Bomb(){
	if (!m_bIsLive)
		return;
	m_bIsBomb = true;
}
void CMyShip::Cut(){
	if (!m_bIsLive)
		return;
	m_bIsCut = true;
}
void CMyShip::SetWeapon(){
	if (!m_bIsLive)
		return;
	m_bIsWeapon = !m_bIsWeapon;
}
int CMyShip::GetPX() {
	return p_x;
}
bool CMyShip::GetbIsLeft(){
	return m_bIsLeft;
}
int CMyShip::GetDiagonal(){
	return m_diagonal;
}
int CMyShip::GetHp(){
	return m_hp;
}
int CMyShip::GetBullet(){
	return m_bullet;
}
int CMyShip::GetBomb(){
	return m_bomb;
}
bool CMyShip::GetWeapon(){
	return m_bIsWeapon;
}

void CMyShip::SetHp(int hp){
	m_hp -= hp;
	if (m_hp < 0){
		m_hp = 0;
	}
	if (m_bIsHpMax == true){
		m_hp = 10;
	}
}
void CMyShip::Kill(){
	m_pMyShip2->Kill();
	CGObject::Kill();
}
void CMyShip::SetBullet(int bullet){
	m_bullet += bullet;
	if(m_bullet < 0){
		m_bullet = 0;
	}
	if (m_bullet > 999){
		m_bullet = 999;
	}
}
void CMyShip::SetBomb(int bomb){
	m_bomb += bomb;
	if (m_bomb < 0){
		m_bomb = 0;
	}
	if (m_bomb > 99){
		m_bomb = 99;
	}
}
void CMyShip::SetHpMax(){
	m_bIsHpMax = !m_bIsHpMax;
}