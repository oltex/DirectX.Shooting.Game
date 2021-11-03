#ifndef _enemy_h_
#define _enemy_h_
#include "gobject.h"
#include "block.h"

enum { MOVE, MOVE2, THROW, ROLL, CUT, CUTJUMP, SHOT };
enum { SOLDIER, TANK, HELICOPTER };

class CEnemy : public CGObject{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int m_xSpeed, m_ySpeed;
	int m_jump = -15, m_gravity = 1;
	bool m_bIsLeft;
	int m_state;
	int m_hp;
	int m_enemyType;
	int m_nowSprite1, m_nowSprite2;
	bool m_bIsActive;
public:
	CEnemy();
	~CEnemy();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval, int enemyType);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x, int y, int p_y);
	void Handle(CBlock* block, int max_block, int x);
	bool GetbIsLeft();
	int GetHp();
	void SetHp(int hp);
	int GetEnemyType();
};
#endif