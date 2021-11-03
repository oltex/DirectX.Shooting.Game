#ifndef _block_h_
#define _block_h_
#include "gobject.h"
class CBlock : public CGObject {
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int m_startY, m_endY;
	bool m_passBullet;
public:
	CBlock();
	~CBlock();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval,int bottom ,int left, int right,int top, int startY, int endY, bool passBullet);
	int GetCollider(int x, int y, int b_y);
	bool GetPasssBullet();
};
#endif