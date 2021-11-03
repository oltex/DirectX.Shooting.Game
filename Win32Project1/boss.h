#ifndef _boss_h_
#define _boss_h_
#include "gobject.h"
#include "block.h"

enum { BOSSMOVE, BOSSMOVE2 , BOSSSHOT, BOSSSTANDINGUP, BOSSSTANDINGMOVE, BOSSSTANDINGSMASHY };

class CBoss : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	int m_xSpeed, m_ySpeed;
	int m_hp;
	bool m_bIsLeft;
	int m_gravity = 1;
	int m_state;
	bool m_bIsStanding;
	bool m_bIsHandling;
public:
	CBoss();
	~CBoss();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y);
	void Handle(CBlock* block, int max_block, int x);
	void SetHp(int hp);
	int GetHp();
	bool GetbIsLeft();
};
#endif