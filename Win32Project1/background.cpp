#include "background.h"
CBackground::CBackground() {
}
CBackground::~CBackground() {
}
void CBackground::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval) {
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}
void CBackground::Handle(int x){
	if (x < 682){
		p_y = 331;
	}
	if (x >= 682 && x < 706){
		p_y = 355 - (24 - (x - 681));
	}
	if (x >= 706 && x < 806){
		p_y = 355;
	}
	if (x >= 806 && x < 822){
		p_y = 371 - (16 - (x - 805));
	}
	if (x >= 822 && x < 2256){
		p_y = 371;
	}
	if (x >= 2256 && x < 2353){
		p_y = 275 + (96 - (x - 2256));
	}
	if (x >= 2353){
		p_y = 275;
	}
	if (x >= 3473 && x < 3553){
		p_y = 355 - (80 - (x - 3472));
	}
	if (x >= 3553){
		p_y = 355;
	}
}
void CBackground::Draw(LPDIRECTDRAWSURFACE7 lpSurface, int x, int p_x) {
	m_pSprite->Drawing(false, 0, m_x - (x-p_x), p_y, lpSurface, true);
}
int CBackground::GetPY(){
	return p_y;
}