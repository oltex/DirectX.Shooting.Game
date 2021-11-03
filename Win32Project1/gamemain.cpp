#include <dsound.h>
#include "dsutil.h"
#include "define.h"

BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7 g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_lpSecondarySurface;
extern LPDIRECTINPUT g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;
extern CTimer g_Timer;

CBackground g_Background;

CInterface g_Interface;
CInterface2 g_Interface2;
CInterface3 g_Interface3;

CBlock g_Block[MAX_BLOCK];

CMyShip g_MyShip;
CMyShip2 g_MyShip2;
CMyMissile g_MyMissile[MAX_MISSILES];

CEnemy g_Enemy[MAX_ENEMY];
CEnemyMissile g_EnemyMissile[MAX_ENEMY_MISSILES];
CBoss g_Boss;
CBossMissile g_BossMissile[MAX_BOSS_MISSILES];
CExploding g_Exploding[MAX_EXPLODES];
CItem g_Item[MAX_ITEM];

extern CSprite g_BackgroundSprite;
extern CSprite g_Background2Sprite;
extern CSprite g_Background3Sprite;
extern CSprite g_Background4Sprite;

extern CSprite g_InterfaceSprite;
extern CSprite g_Interface2Sprite;
extern CSprite g_Interface3Sprite;
extern CSprite g_Interface4Sprite;

extern CSprite g_MyShipSprite;
extern CSprite g_MyShip2Sprite;
extern CSprite g_MyMissileSprite;
extern CSprite g_EnemySprite;
extern CSprite g_EnemyMissileSprite;
extern CSprite g_BossSprite;
extern CSprite g_BossMissileSprite;
extern CSprite g_ExplodingSprite;
extern CSprite g_ItemSprite;

extern CSprite g_ClearSprite;
extern CSprite g_DeadSprite;

extern HSNDOBJ Sound[10];
extern void _Play(int num);

static BOOL m_bGameFirst = TRUE;
static BOOL m_bGameDead = FALSE;
static BOOL m_bGameClear = FALSE;
static BOOL m_bBoss = FALSE;
int enemyCnt = MAX_ENEMY;

static char szReplay[] = "You Dead.. Replay?(y/n)";
static char szWin[] = "You Win!! Replay?(y/n)";
void InitGame();
void MyShipDead();
void MyShipWin();

void GameMain(){
	static int nLastMissileTime = g_Timer.time();
	static int nLastMissileTime2[MAX_ENEMY];
	if (m_bGameFirst == TRUE) {
		InitGame();
		for (int i = 0; i < MAX_ENEMY; i++){
			nLastMissileTime2[i] = g_Timer.time();
		}
	}
	static int nLastMissileTime3 = g_Timer.time();
	static int nLastMissileTime4 = g_Timer.time();
	static int time = 0;

	g_MyShip.Stand();
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_LEFT)){
		g_MyShip.Left();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_RIGHT)){
		g_MyShip.Right();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_UP)){
		g_MyShip.Up();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_DOWN)){
		g_MyShip.Down();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_S)){
		g_MyShip.Jump();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_U)){
		if (g_Timer.elapsed(nLastMissileTime4, MY_MISSILE_INTERVAL)){
			g_MyShip.SetHpMax();
		}
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_A)){
		bool useCut = false;
		for (int i = 0; i < MAX_ENEMY; i++){
			if (g_Enemy[i].IsLive() == true && g_Enemy[i].GetEnemyType() == 0){
				if (g_MyShip.GetRect().left + (g_MyShip.GetbIsLeft() == true ? -10 : 0) < g_Enemy[i].GetRect().right && g_MyShip.GetRect().top < g_Enemy[i].GetRect().bottom && g_MyShip.GetRect().right + (g_MyShip.GetbIsLeft() == true ? 0 : 10)  > g_Enemy[i].GetRect().left && g_MyShip.GetRect().bottom > g_Enemy[i].GetRect().top){
					g_MyShip.Cut();
					useCut = true;
				}
			}
		}
		if (useCut == false){
			g_MyShip.Shot();
		}
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_D)){
		g_MyShip.Bomb();
	}
	if (g_MyShip.GetHp() == 0 && g_MyShip.IsLive() ==  true){
		int i;
		for (i = 0; i < MAX_EXPLODES; i++){
			if (!g_Exploding[i].IsLive())
				break;
		}
		if (i == MAX_EXPLODES)
			return;
		g_Exploding[i].Initialize(&g_ExplodingSprite, g_MyShip.GetX(), g_MyShip.GetY(), &g_Timer, 40, 50, 10, 40, 49,g_MyShip.GetbIsLeft(), 1);
		g_MyShip.Kill();
		MyShipDead();
	}
	g_MyShip.Handle(g_Block, MAX_BLOCK);
	if (g_MyShip.IsLive()){
		int i;
		for (i = 0; i < MAX_MISSILES; i++){
			if (!g_MyMissile[i].IsLive())
				break;
		}
		if (i == MAX_MISSILES)
			return;
		switch (g_MyShip.GetCurrentFrame()){
		case 36:
			if (g_Timer.elapsed(nLastMissileTime, MY_MISSILE_INTERVAL)){
				g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY() - 4, &g_Timer, 0, 50, 10, 10, 0, g_MyShip.GetbIsLeft(), 0);
				//_Play(1);
			}
			break;
		case 46:
			if (g_Timer.elapsed(nLastMissileTime, MY_MISSILE_INTERVAL)){
				g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY(), &g_Timer, 0, 50, 10, 0, -10, g_MyShip.GetbIsLeft(), 0);
				//_Play(1);
			}
			break;
		case 99:
			if (g_Timer.elapsed(nLastMissileTime, MY_MISSILE_INTERVAL)){
				g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY() + 5, &g_Timer, 0, 50, 10, 10, 0, g_MyShip.GetbIsLeft(), 0);
				//_Play(1);
			}
			break;
		case 92:
			if (g_Timer.elapsed(nLastMissileTime, MY_MISSILE_INTERVAL)){
				g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY(), &g_Timer, 0, 50, 10, 0, 10, g_MyShip.GetbIsLeft(), 0);
				//_Play(1);
			}
			break;
		case 130:
		case 135:
		case 255:
		case 260:
			if (g_Timer.elapsed(nLastMissileTime, MY_MISSILE_INTERVAL)){
				for (int j = 0; j < MAX_ENEMY; j++){
					if (g_Enemy[j].IsLive() == true && g_Enemy[j].GetEnemyType() == 0){
						if (g_MyShip.GetRect().left + (g_MyShip.GetbIsLeft() == true ? -10 : 0) < g_Enemy[j].GetRect().right && g_MyShip.GetRect().top < g_Enemy[j].GetRect().bottom && g_MyShip.GetRect().right + (g_MyShip.GetbIsLeft() == true ? 0 : 10)  > g_Enemy[j].GetRect().left && g_MyShip.GetRect().bottom > g_Enemy[j].GetRect().top){
							g_Enemy[j].SetHp(10);
						}
					}
				}
			}
			break;
		case 203:
		case 204:
		case 205:
			if (g_Timer.elapsed(nLastMissileTime, 75)){
				g_MyShip.SetBullet(-1);
				g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY() - 5 + rand()%10 + 5, &g_Timer, 9, 50, 10, 10, 0, g_MyShip.GetbIsLeft(), 1);
				//_Play(1);
			}
			break;
		case 120:
		case 245:
		case 125:
		case 250:
			if (g_Timer.elapsed(nLastMissileTime, MY_MISSILE_INTERVAL)){
				if (g_MyShip.GetBomb() > 0){
					g_MyShip.SetBomb(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY(), &g_Timer, 8, 50, 10, 4, -10, g_MyShip.GetbIsLeft(), 2);
					//_Play(1);
				}

			}
			break;
		case 199:
		case 200:
		case 201:
			switch (g_MyShip.GetDiagonal()){
			case 0:
				if (g_Timer.elapsed(nLastMissileTime, 75)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 9, 50, 10, 10, 0, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			case 1:
				if (g_Timer.elapsed(nLastMissileTime, 30)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 10, 50, 10, 7, -5, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			case 2:
				if (g_Timer.elapsed(nLastMissileTime, 30)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 11, 50, 10, 5, -7, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			case -1:
				if (g_Timer.elapsed(nLastMissileTime, 30)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 13, 50, 10, 7, 5, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			case -2:
				if (g_Timer.elapsed(nLastMissileTime, 30)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 14, 50, 10, 5, 7, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			}
			break;
		case 207:
		case 208:
		case 209:
			switch (g_MyShip.GetDiagonal()){
			case 1:
				if (g_Timer.elapsed(nLastMissileTime, 30)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 10, 50, 10, 7, -5, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			case 2:
				if (g_Timer.elapsed(nLastMissileTime, 30)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 11, 50, 10, 5, -7, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			case 3:
				if (g_Timer.elapsed(nLastMissileTime, 75)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY(), &g_Timer, 12, 50, 10, 0, -10, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			}
			break;
		case 229:
		case 230:
		case 231:
			switch (g_MyShip.GetDiagonal()){
			case -1:
				if (g_Timer.elapsed(nLastMissileTime, 30)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 13, 50, 10, 7, 5, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			case -2:
				if (g_Timer.elapsed(nLastMissileTime, 30)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY() - 5 + rand() % 10, &g_Timer, 14, 50, 10, 5, 7, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			case -3:
				if (g_Timer.elapsed(nLastMissileTime, 75)){
					g_MyShip.SetBullet(-1);
					g_MyMissile[i].Initialize(&g_MyMissileSprite, g_MyShip.GetX() - 5 + rand() % 10, g_MyShip.GetY(), &g_Timer, 15, 50, 10, 0, 10, g_MyShip.GetbIsLeft(), 1);
					//_Play(1);
				}
				break;
			}
			break;

		}
	}
	for (int i = 0; i < MAX_MISSILES; i++){
		g_MyMissile[i].Handle(g_Block, MAX_BLOCK, g_MyShip.GetX(), g_MyShip.GetY());
		if (g_MyMissile[i].IsLive() == true){
			if (g_MyMissile[i].GetBombTimer() > 75){
				int j;
				for (j = 0; j < MAX_EXPLODES; j++){
					if (!g_Exploding[j].IsLive())
						break;
				}
				if (j == MAX_EXPLODES)
					break;
				g_Exploding[j].Initialize(&g_ExplodingSprite, g_MyMissile[i].GetX(), g_MyMissile[i].GetY(), &g_Timer, 0, 50, 10, 0, 20, false,1);
				for (int l = 0; l < MAX_ENEMY; l++){
					if (g_MyMissile[i].GetRect().left - 5 < g_Enemy[l].GetRect().right && g_MyMissile[i].GetRect().top - 85 < g_Enemy[l].GetRect().bottom && g_MyMissile[i].GetRect().right + 5 > g_Enemy[l].GetRect().left && g_MyMissile[i].GetRect().bottom > g_Enemy[l].GetRect().top){
						g_Enemy[l].SetHp(g_MyMissile[i].GetDamage() + 5);
					}
				}
				if (g_MyMissile[i].GetRect().left - 5 < g_Boss.GetRect().right && g_MyMissile[i].GetRect().top - 85 < g_Boss.GetRect().bottom && g_MyMissile[i].GetRect().right + 5 > g_Boss.GetRect().left && g_MyMissile[i].GetRect().bottom > g_Boss.GetRect().top){
					g_Boss.SetHp(g_MyMissile[i].GetDamage() + 5);
				}
				g_MyMissile[i].Kill();
			}
			for (int j = 0; j < MAX_ENEMY; j++){
				if (g_Enemy[j].IsLive() == true){
					if (g_MyMissile[i].GetRect().left < g_Enemy[j].GetRect().right && g_MyMissile[i].GetRect().top < g_Enemy[j].GetRect().bottom && g_MyMissile[i].GetRect().right > g_Enemy[j].GetRect().left && g_MyMissile[i].GetRect().bottom > g_Enemy[j].GetRect().top){
						int k;
						switch (g_MyMissile[i].GetBulletType()){
						case 0:
						case 1:
							for (k = 0; k < MAX_EXPLODES; k++){
								if (!g_Exploding[k].IsLive())
									break;
							}
							if (k == MAX_EXPLODES)
								break;
							g_Exploding[k].Initialize(&g_ExplodingSprite, g_MyMissile[i].GetX(), g_MyMissile[i].GetY(), &g_Timer, 21, 50, 10, 21, 24, false, 1);
							break;
						case 2:
							for (k = 0; k < MAX_EXPLODES; k++){
								if (!g_Exploding[k].IsLive())
									break;
							}
							if (k == MAX_EXPLODES)
								break;
							g_Exploding[k].Initialize(&g_ExplodingSprite, g_MyMissile[i].GetX(), g_MyMissile[i].GetY(), &g_Timer, 0, 50, 10, 0, 20, false, 1);
							for (int l = 0; l < MAX_ENEMY; l++){
								if (g_MyMissile[i].GetRect().left - 5 < g_Enemy[l].GetRect().right && g_MyMissile[i].GetRect().top - 85 < g_Enemy[l].GetRect().bottom && g_MyMissile[i].GetRect().right + 5 > g_Enemy[l].GetRect().left && g_MyMissile[i].GetRect().bottom > g_Enemy[l].GetRect().top){
									g_Enemy[l].SetHp(g_MyMissile[i].GetDamage() + 5);
								}
							}
							break;
						}
						g_Enemy[j].SetHp(g_MyMissile[i].GetDamage());
						g_MyMissile[i].Kill();
					}
				}
			}
			if (g_Boss.IsLive() == true){
				if (g_MyMissile[i].GetRect().left < g_Boss.GetRect().right && g_MyMissile[i].GetRect().top < g_Boss.GetRect().bottom && g_MyMissile[i].GetRect().right > g_Boss.GetRect().left && g_MyMissile[i].GetRect().bottom > g_Boss.GetRect().top){
					int k;
					switch (g_MyMissile[i].GetBulletType()){
					case 0:
					case 1:
						for (k = 0; k < MAX_EXPLODES; k++){
							if (!g_Exploding[k].IsLive())
								break;
						}
						if (k == MAX_EXPLODES)
							break;
						g_Exploding[k].Initialize(&g_ExplodingSprite, g_MyMissile[i].GetX(), g_MyMissile[i].GetY(), &g_Timer, 21, 50, 10, 21, 24, false, 1);
						break;
					case 2:
						for (k = 0; k < MAX_EXPLODES; k++){
							if (!g_Exploding[k].IsLive())
								break;
						}
						if (k == MAX_EXPLODES)
							break;
						g_Exploding[k].Initialize(&g_ExplodingSprite, g_MyMissile[i].GetX(), g_MyMissile[i].GetY(), &g_Timer, 0, 50, 10, 0, 20, false, 1);
						if (g_MyMissile[i].GetRect().left - 5 < g_Boss.GetRect().right && g_MyMissile[i].GetRect().top - 85 < g_Boss.GetRect().bottom && g_MyMissile[i].GetRect().right + 5 > g_Boss.GetRect().left && g_MyMissile[i].GetRect().bottom > g_Boss.GetRect().top){
							g_Boss.SetHp(g_MyMissile[i].GetDamage() + 5);
						}
						break;
					}
					g_Boss.SetHp(g_MyMissile[i].GetDamage());
					g_MyMissile[i].Kill();
				}
			}
		}
	}
	for (int i = 0; i < MAX_EXPLODES; i++){
		g_Exploding[i].Handle();
	}
	for (int i = 0; i < MAX_ENEMY; i++){
		if (g_Enemy[i].GetHp() == 0 && g_Enemy[i].IsLive() == true){
			int j;
			for (j = 0; j < MAX_EXPLODES; j++){
				if (!g_Exploding[j].IsLive())
					break;
			}
			if (j == MAX_EXPLODES)
				break;
			switch (g_Enemy[i].GetEnemyType()){
			case 0:
				g_Exploding[j].Initialize(&g_ExplodingSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY(), &g_Timer, 78, 50, 10, 78, 84, g_Enemy[i].GetbIsLeft(), 1);
				break;
			case 1:
			case 2:
				g_Exploding[j].Initialize(&g_ExplodingSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY(), &g_Timer, 50, 50, 10, 50, 75, false, 1);
				break;
			}
			for (j = 0; j < MAX_ITEM; j++){
				if (!g_Item[j].IsLive())
					break;
			}
			if (j == MAX_ITEM)
				break;
			switch (g_Enemy[i].GetEnemyType()){
			case 1:
				g_Item[j].Initialize(&g_ItemSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY(), &g_Timer, 0, 50, 10, 1);
				break;
			case 2:
				g_Item[j].Initialize(&g_ItemSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY(), &g_Timer, 0, 50, 10, 0);
				break;
			}
			g_Enemy[i].Kill();
			
		}
		g_Enemy[i].Handle(g_Block, MAX_BLOCK, g_MyShip.GetX());
		if (g_Enemy[i].IsLive() == true) {
			int j;
			for (j = 0; j < MAX_ENEMY_MISSILES; j++) {
				if (!g_EnemyMissile[j].IsLive())
					break;
			}
			if (j == MAX_ENEMY_MISSILES)
				return;
			int k;
			switch (g_Enemy[i].GetCurrentFrame()){
			case 59:
			case 72:
				if (g_Timer.elapsed(nLastMissileTime2[i], 240)) {
					if (g_MyShip.IsLive() == true){
						if (g_Enemy[i].GetRect().left + (g_Enemy[i].GetbIsLeft() == true ? -20 : 0) < g_MyShip.GetRect().right && g_Enemy[i].GetRect().top < g_MyShip.GetRect().bottom && g_Enemy[i].GetRect().right + (g_Enemy[i].GetbIsLeft() == true ? 0 : 20)  > g_MyShip.GetRect().left && g_Enemy[i].GetRect().bottom > g_MyShip.GetRect().top){
							g_MyShip.SetHp(1);
						}
					}
				}
				break;
			case 34:
				if (g_Timer.elapsed(nLastMissileTime2[i], 240)) {
					g_EnemyMissile[j].Initialize(&g_EnemyMissileSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY(), &g_Timer, 0, 50, 10, 5, -13, g_Enemy[i].GetbIsLeft(), 0);
				}
				break;
			case 47:
				if (g_Timer.elapsed(nLastMissileTime2[i], 240)) {
					g_EnemyMissile[j].Initialize(&g_EnemyMissileSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY(), &g_Timer, 0, 50, 10, 3, 0, g_Enemy[i].GetbIsLeft(), 1);
				}
				break;
			case 90:
				if (g_Timer.elapsed(nLastMissileTime2[i], 240)) {
					g_EnemyMissile[j].Initialize(&g_EnemyMissileSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY()-20, &g_Timer, 0, 50, 10, 3, 0, g_Enemy[i].GetbIsLeft(), 2);
					for (k = 0; k < MAX_EXPLODES; k++){
						if (!g_Exploding[k].IsLive())
							break;
					}
					if (k == MAX_EXPLODES)
						break;
					g_Exploding[k].Initialize(&g_ExplodingSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY() - 20, &g_Timer, 25, 50, 10, 25, 39, g_Enemy[i].GetbIsLeft() ,1);
				}
				break;
			case 94:
				if (g_Timer.elapsed(nLastMissileTime2[i], 1000)) {
					g_EnemyMissile[j].Initialize(&g_EnemyMissileSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY()-15, &g_Timer, 0, 50, 10, 0, 3, g_Enemy[i].GetbIsLeft(), 3);
					for (j = 0; j < MAX_ENEMY_MISSILES; j++) {
						if (!g_EnemyMissile[j].IsLive())
							break;
					}
					if (j == MAX_ENEMY_MISSILES)
						return;
					g_EnemyMissile[j].Initialize(&g_EnemyMissileSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY(), &g_Timer, 0, 50, 10, 0, 3, g_Enemy[i].GetbIsLeft(), 3);
					for (j = 0; j < MAX_ENEMY_MISSILES; j++) {
						if (!g_EnemyMissile[j].IsLive())
							break;
					}
					if (j == MAX_ENEMY_MISSILES)
						return;
					g_EnemyMissile[j].Initialize(&g_EnemyMissileSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY()+15, &g_Timer, 0, 50, 10, 0, 3, g_Enemy[i].GetbIsLeft(), 3);
					for (k = 0; k < MAX_EXPLODES; k++){
						if (!g_Exploding[k].IsLive())
							break;
					}
					if (k == MAX_EXPLODES)
						break;
					g_Exploding[k].Initialize(&g_ExplodingSprite, g_Enemy[i].GetX(), g_Enemy[i].GetY() + 10, &g_Timer, 76, 50, 10, 76, 77, g_Enemy[i].GetbIsLeft(), 1);
				}
				break;
			}
		}
	}
	for (int i = 0; i < MAX_ENEMY_MISSILES; i++) {
		g_EnemyMissile[i].Handle(g_Block, MAX_BLOCK, g_MyShip.GetX(), g_MyShip.GetY());
		if (g_EnemyMissile[i].IsLive() == true && g_MyShip.IsLive() == true){
			if (g_EnemyMissile[i].GetRect().left < g_MyShip.GetRect().right && g_EnemyMissile[i].GetRect().top < g_MyShip.GetRect().bottom && g_EnemyMissile[i].GetRect().right > g_MyShip.GetRect().left && g_EnemyMissile[i].GetRect().bottom > g_MyShip.GetRect().top){
				g_MyShip.SetHp(g_EnemyMissile[i].GetDamage());
				g_EnemyMissile[i].Kill();
				int j;
				for (j = 0; j < MAX_EXPLODES; j++){
					if (!g_Exploding[j].IsLive())
						break;
				}
				if (j == MAX_EXPLODES)
					break;
				g_Exploding[j].Initialize(&g_ExplodingSprite, g_EnemyMissile[i].GetX(), g_EnemyMissile[i].GetY(), &g_Timer, 21, 50, 10, 21, 24, false , 1);
			}
		}
	}
	for (int i = 0; i < MAX_ITEM; i++){
		g_Item[i].Handle(g_Block, MAX_BLOCK);
		if (g_MyShip.IsLive() == true && g_Item[i].IsLive() == true){
			if (g_Item[i].GetRect().left < g_MyShip.GetRect().right && g_Item[i].GetRect().top < g_MyShip.GetRect().bottom && g_Item[i].GetRect().right > g_MyShip.GetRect().left && g_Item[i].GetRect().bottom > g_MyShip.GetRect().top){
				switch (g_Item[i].GetItemType()){
				case 0:
					g_MyShip.SetBullet(200);
					break;
				case 1:
					g_MyShip.SetBomb(10);
					break;
				}
				g_Item[i].Kill();
			}
		}
	}
	if (g_Boss.GetHp() == 0 && g_Boss.IsLive() == true){
		int i;
		for (i = 0; i < MAX_EXPLODES; i++){
			if (!g_Exploding[i].IsLive())
				break;
		}
		if (i == MAX_EXPLODES)
			return;
		for (int j = 0; j < 10; j++){
			for (i = 0; i < MAX_EXPLODES; i++){
				if (!g_Exploding[i].IsLive())
					break;
			}
			if (i == MAX_EXPLODES)
				return;
			g_Exploding[i].Initialize(&g_ExplodingSprite, g_Boss.GetX() - 25 + rand() % 50, g_Boss.GetY() - 25 + rand() % 50, &g_Timer, 50, 50, 10, 50, 75, false, 1);
		}
		g_Boss.Kill();
		MyShipWin();
	}
	g_Boss.Handle(g_Block, MAX_BLOCK, g_MyShip.GetX());
	if (g_Boss.IsLive() == true){
		int i;
		for (i = 0; i < MAX_BOSS_MISSILES; i++) {
			if (!g_BossMissile[i].IsLive())
				break;
		}
		if (i == MAX_BOSS_MISSILES)
			return;
		switch (g_Boss.GetCurrentFrame()){
		case 16:
			if (g_Timer.elapsed(nLastMissileTime3, 240)){
				switch (rand() % 2){
				case 0:
					g_BossMissile[i].Initialize(&g_BossMissileSprite, g_Boss.GetX(), g_Boss.GetY(), &g_Timer, 0, 50, 50,
						((float)g_MyShip.GetX() - (float)g_Boss.GetX()) / sqrtf(powf((float)g_MyShip.GetX() - (float)g_Boss.GetX(), 2) + powf((float)g_MyShip.GetY() - (float)(g_Boss.GetY() - 30), 2)) * 20, // x의 속도
						((float)g_MyShip.GetY() - (float)(g_Boss.GetY() - 30)) / sqrtf(powf((float)g_MyShip.GetX() - (float)g_Boss.GetX(), 2) + powf((float)g_MyShip.GetY() - (float)(g_Boss.GetY() - 30), 2)) * 20,	//y의 속도
						g_Boss.GetbIsLeft(), 0);
					break;
				case 1:
					g_BossMissile[i].Initialize(&g_BossMissileSprite, g_Boss.GetX(), g_Boss.GetY() - 30, &g_Timer, 0, 50, 50, -3, -2, g_Boss.GetbIsLeft(), 1);
					for (i = 0; i < MAX_BOSS_MISSILES; i++) {
						if (!g_BossMissile[i].IsLive())
							break;
					}
					if (i == MAX_BOSS_MISSILES)
						return;
					g_BossMissile[i].Initialize(&g_BossMissileSprite, g_Boss.GetX(), g_Boss.GetY() - 30, &g_Timer, 0, 50, 50, -4, -1, g_Boss.GetbIsLeft(), 1);
					for (i = 0; i < MAX_BOSS_MISSILES; i++) {
						if (!g_BossMissile[i].IsLive())
							break;
					}
					if (i == MAX_BOSS_MISSILES)
						return;
					g_BossMissile[i].Initialize(&g_BossMissileSprite, g_Boss.GetX(), g_Boss.GetY() - 30, &g_Timer, 0, 50, 50, -5, 0, g_Boss.GetbIsLeft(), 1);
					for (i = 0; i < MAX_BOSS_MISSILES; i++) {
						if (!g_BossMissile[i].IsLive())
							break;
					}
					if (i == MAX_BOSS_MISSILES)
						return;
					g_BossMissile[i].Initialize(&g_BossMissileSprite, g_Boss.GetX(), g_Boss.GetY() - 30, &g_Timer, 0, 50, 50, -4, 1, g_Boss.GetbIsLeft(), 1);
					for (i = 0; i < MAX_BOSS_MISSILES; i++) {
						if (!g_BossMissile[i].IsLive())
							break;
					}
					if (i == MAX_BOSS_MISSILES)
						return;
					g_BossMissile[i].Initialize(&g_BossMissileSprite, g_Boss.GetX(), g_Boss.GetY() - 30, &g_Timer, 0, 50, 50, -3, 2, g_Boss.GetbIsLeft(), 1);
					break;
				}
			}
			break;
		case 85:
			if (g_Timer.elapsed(nLastMissileTime3, 240)){
				if (g_MyShip.IsLive() == true){
					if (g_Boss.GetRect().left - 20 < g_MyShip.GetRect().right && g_Boss.GetRect().top + 40 < g_MyShip.GetRect().bottom && g_Boss.GetRect().right + 20 > g_MyShip.GetRect().left && g_Boss.GetRect().bottom > g_MyShip.GetRect().top){
						g_MyShip.SetHp(10);
					}
				}
			}
			break;
		}
	}
	for (int i = 0; i < MAX_BOSS_MISSILES; i++){
		g_BossMissile[i].Handle(g_Block, MAX_BLOCK, g_MyShip.GetX(), g_MyShip.GetY());
		if (g_BossMissile[i].IsLive() == true && g_MyShip.IsLive() == true){
			if (g_BossMissile[i].GetRect().left < g_MyShip.GetRect().right && g_BossMissile[i].GetRect().top < g_MyShip.GetRect().bottom && g_BossMissile[i].GetRect().right > g_MyShip.GetRect().left && g_BossMissile[i].GetRect().bottom > g_MyShip.GetRect().top){
				g_MyShip.SetHp(g_BossMissile[i].GetDamage());
				g_BossMissile[i].Kill();
				int j;
				for (j = 0; j < MAX_EXPLODES; j++){
					if (!g_Exploding[j].IsLive())
						break;
				}
				if (j == MAX_EXPLODES)
					break;
				g_Exploding[j].Initialize(&g_ExplodingSprite, g_BossMissile[i].GetX(), g_BossMissile[i].GetY(), &g_Timer, 21, 50, 10, 21, 24, false, 1);
			}
		}
	}

	g_Background.Handle(g_MyShip.GetX());



	//------------------------------------------------------------------------------------------------------

	g_Background2Sprite.Drawing(false, 0, 2176 - (g_MyShip.GetX() - g_MyShip.GetPX()), 198, g_lpSecondarySurface, true);
	g_Background3Sprite.Drawing(false, 0, 2240 - (g_MyShip.GetX() - g_MyShip.GetPX()), g_Background.GetPY(), g_lpSecondarySurface, true);
	g_Background.Draw(g_lpSecondarySurface, g_MyShip.GetX(), g_MyShip.GetPX());

	for (int i = 0; i < MAX_ENEMY_MISSILES; i++) {
		g_EnemyMissile[i].Draw(g_lpSecondarySurface, g_MyShip.GetX(), g_MyShip.GetPX(), g_Background.GetY(), g_Background.GetPY());
	}
	for (int i = 0; i < MAX_ENEMY; i++){
		g_Enemy[i].Draw(g_lpSecondarySurface, g_MyShip.GetX(), g_MyShip.GetPX(), g_Background.GetY(), g_Background.GetPY());
	}
	g_Boss.Draw(g_lpSecondarySurface, g_MyShip.GetX(), g_MyShip.GetPX(), g_Background.GetY(), g_Background.GetPY());
	for (int i = 0; i < MAX_BOSS_MISSILES; i++){
		g_BossMissile[i].Draw(g_lpSecondarySurface, g_MyShip.GetX(), g_MyShip.GetPX(), g_Background.GetY(), g_Background.GetPY());
	}
	for (int i = 0; i < MAX_ITEM; i++){
		g_Item[i].Draw(g_lpSecondarySurface, g_MyShip.GetX(), g_MyShip.GetPX(), g_Background.GetY(), g_Background.GetPY());
	}
	for (int i = 0; i < MAX_MISSILES; i++){
		g_MyMissile[i].Draw(g_lpSecondarySurface, g_MyShip.GetX(), g_MyShip.GetPX(), g_Background.GetY(), g_Background.GetPY());
	}
	for (int i = 0; i < MAX_EXPLODES; i++){
		g_Exploding[i].Draw(g_lpSecondarySurface, g_MyShip.GetX(), g_MyShip.GetPX(), g_Background.GetY(), g_Background.GetPY());
	}
	g_MyShip.Draw(g_lpSecondarySurface, g_Background.GetY(), g_Background.GetPY());

	g_Background4Sprite.Drawing(false, 0, 2240 - (g_MyShip.GetX() - g_MyShip.GetPX()), g_Background.GetPY(), g_lpSecondarySurface, true);

	g_Interface4Sprite.Drawing(false, 0, 320, 240, g_lpSecondarySurface, true);
	g_Interface.Draw(g_lpSecondarySurface, g_MyShip.GetBullet(), g_MyShip.GetBomb());
	g_Interface2.Draw(g_lpSecondarySurface, g_MyShip.GetWeapon() == true ? 1 : 0);
	g_Interface3.Draw(g_lpSecondarySurface, g_MyShip.GetHp());


	if (m_bGameClear == TRUE) 
		g_ClearSprite.Drawing(false, 0, 320, 240, g_lpSecondarySurface, true);
	if (m_bGameDead == TRUE) 
		g_DeadSprite.Drawing(false,0, 320, 240, g_lpSecondarySurface, true);
	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT))){
		if (hResult == DDERR_SURFACELOST)
			g_lpPrimarySurface->Restore();
	}
}
BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode){
	char KeyBuffer[256];
	HRESULT hResult;
	if (lpKeyboard->GetDeviceState(256, (LPVOID)KeyBuffer) == DIERR_INPUTLOST){
		while (hResult = lpKeyboard->Acquire() == DIERR_INPUTLOST)
			hResult = lpKeyboard->Acquire();
		lpKeyboard->GetDeviceState(256, (LPVOID)KeyBuffer);
	}
	return (KeyBuffer[dikcode] & 0x80);
}
void InitGame(){
	for (int i = 0; i < MAX_EXPLODES; i++){
		if (g_Exploding[i].IsLive()) g_Exploding[i].Kill();
	}
	for (int i = 0; i < MAX_MISSILES; i++){
		if (g_MyMissile[i].IsLive())
			g_MyMissile[i].Kill();
	}
	for (int i = 0; i < MAX_ENEMY_MISSILES; i++){
		if (g_EnemyMissile[i].IsLive())
			g_EnemyMissile[i].Kill();
	}
	for (int i = 0; i < MAX_ITEM; i++){
		if (g_Item[i].IsLive())
			g_Item[i].Kill();
	}
	for (int i = 0; i < MAX_BOSS_MISSILES; i++){
		if (g_BossMissile[i].IsLive())
			g_BossMissile[i].Kill();
	}

	if (g_Background.IsLive())
		g_Background.Kill();
	g_Background.Initialize(&g_BackgroundSprite, 2240, 275, &g_Timer, 0, 0, 0);



	if (g_Interface.IsLive())
		g_Interface.Kill();
	g_Interface.Initialize(&g_InterfaceSprite, 0, 0, &g_Timer, 0, 0, 0);
	if (g_Interface2.IsLive())
		g_Interface2.Kill();
	g_Interface2.Initialize(&g_Interface2Sprite, 0, 0, &g_Timer, 0, 0, 0);
	if (g_Interface3.IsLive())
		g_Interface3.Kill();
	g_Interface3.Initialize(&g_Interface3Sprite, 0, 0, &g_Timer, 0, 0, 0);



	for (int i = 0; i < MAX_BLOCK; i++){
		if (g_Block[i].IsLive())
			g_Block[i].Kill();
	}
	g_Block[0].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 480, 0, 816, 350, NULL, NULL, false);
	g_Block[1].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 480, 816, 1025, 320, 350, 320, false);
	g_Block[2].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 480, 1025, 1148, 320, NULL, NULL, false);
	g_Block[3].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 480, 1148, 1234, 320, 320, 340, false);
	g_Block[4].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 480, 1234, 2159, 340, NULL, NULL, false);
	g_Block[5].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 480, 2159, 2471, 340, 340, 382, false);
	g_Block[6].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 403, 2471, 2621, 382, NULL, NULL, false);
	g_Block[7].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 398, 2650, 2762, 382, NULL, NULL, false);
	g_Block[8].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 398, 2792, 3001, 382, NULL, NULL, false);
	g_Block[9].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 398, 3027, 3147, 382, NULL, NULL, false);
	g_Block[10].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 403, 3165, 3325, 382, NULL, NULL, false);
	g_Block[11].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 480, 3325, 3400, 365, 382, 365, false);
	g_Block[12].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 480, 3400, 4480, 365, NULL, NULL, false);
	g_Block[13].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 272, 1178, 1249, 255, NULL, NULL, true);
	g_Block[14].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 238, 1289, 1344, 222, NULL, NULL, true);
	g_Block[15].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 222, 1344, 1359, 206, 222, 206, true);
	g_Block[16].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 222, 1359, 1462, 206, NULL, NULL, true);
	g_Block[17].Initialize(NULL, 0, 0, &g_Timer, 0, 0, 0, 283, 1675, 1761, 271, NULL, NULL,true);



	if (g_MyShip.IsLive())
		g_MyShip.Kill();
	g_MyShip.Initialize(&g_MyShip2, &g_MyShipSprite, 100, 0, &g_Timer, 0, 50, 10);
	if (g_MyShip2.IsLive())
		g_MyShip2.Kill();
	g_MyShip2.Initialize(&g_MyShip2Sprite, 0, 0, &g_Timer, 0, 50, 10);

	for (int i = 0; i < MAX_ENEMY; i++){
		if (g_Enemy[i].IsLive())
			g_Enemy[i].Kill();
	}
	g_Enemy[0].Initialize(&g_EnemySprite, 750, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[1].Initialize(&g_EnemySprite, 980, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[2].Initialize(&g_EnemySprite, 1000, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[3].Initialize(&g_EnemySprite, 1220, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[4].Initialize(&g_EnemySprite, 1230, 110, &g_Timer, 0, 50, 10, 0);
	g_Enemy[5].Initialize(&g_EnemySprite, 1315, 90, &g_Timer, 0, 50, 10, 0);
	g_Enemy[6].Initialize(&g_EnemySprite, 1400, 90, &g_Timer, 0, 50, 10, 0);
	g_Enemy[7].Initialize(&g_EnemySprite, 1440, 90, &g_Timer, 0, 50, 10, 0);
	g_Enemy[8].Initialize(&g_EnemySprite, 1300, 220, &g_Timer, 0, 50, 10, 1);
	g_Enemy[9].Initialize(&g_EnemySprite, 1640, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[10].Initialize(&g_EnemySprite, 1720, 100, &g_Timer, 0, 50, 10, 2);
	g_Enemy[11].Initialize(&g_EnemySprite, 1800, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[12].Initialize(&g_EnemySprite, 1810, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[13].Initialize(&g_EnemySprite, 1430, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[14].Initialize(&g_EnemySprite, 2180, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[15].Initialize(&g_EnemySprite, 2280, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[16].Initialize(&g_EnemySprite, 2380, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[17].Initialize(&g_EnemySprite, 2860, 100, &g_Timer, 0, 50, 10, 2);
	g_Enemy[18].Initialize(&g_EnemySprite, 3570, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[19].Initialize(&g_EnemySprite, 3595, 220, &g_Timer, 0, 50, 10, 0);
	g_Enemy[20].Initialize(&g_EnemySprite, 3625, 220, &g_Timer, 0, 50, 10, 1);

	if (g_Boss.IsLive()) 
		g_Boss.Kill();
	g_Boss.Initialize(&g_BossSprite, 4050, 220, &g_Timer, 0, 50, 10);

	m_bGameDead = FALSE;
	m_bGameClear = FALSE;
	m_bGameFirst = FALSE;
	m_bBoss = FALSE;
	enemyCnt = MAX_ENEMY;

	SndObjPlay(Sound[0], DSBPLAY_LOOPING);
}
void MyShipDead(){
	m_bGameDead = TRUE;
	_Play(3);
}
void MyShipWin(){
	m_bGameClear = TRUE;
	_Play(3);
}
void _Play(int num){
	SndObjPlay(Sound[num], NULL);
}