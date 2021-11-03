#ifndef _myship_h_
#define _myship_h_
#include "gobject.h"
#include "myship2.h"
#include "block.h"
#include "mymissile.h"
#include "sprite.h"
#include "timer.h"
class CMyShip : public CGObject{
private:
	CMyShip2* m_pMyShip2;
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int m_jump = -15;
	int m_gravity = 1;
	int m_ySpeed;
	bool m_bIsLeft, m_bIsUp, m_bIsDown, m_bIsJump, m_bIsMove, m_bIsShot, m_bIsBomb, m_bIsCut, m_bIsWeapon;
	bool m_bNeedLowerBody;
	int p_x;
	int m_bullet, m_hp, m_bomb;
	int m_diagonal;
	bool m_bCompulsionDraw;
	int m_lastX;
	bool m_bIsHpMax = false;
public:
	CMyShip();
	~CMyShip();
	void Initialize(CMyShip2* pCMyShip2, CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int y, int p_y);
	void Handle(CBlock* block, int max_block);
	void Stand();
	void Left();
	void Right();
	void Up();
	void Down();
	void Jump();
	void Shot();
	void Bomb();
	void Cut();
	void SetHp(int hp);
	void SetWeapon();
	int GetPX();
	bool GetbIsLeft();
	int GetDiagonal();
	int GetHp();
	int GetBullet();
	int GetBomb();
	bool GetWeapon();
	void Kill();
	void SetBomb(int bomb);
	void SetBullet(int bullet);
	void SetHpMax();
};
#endif