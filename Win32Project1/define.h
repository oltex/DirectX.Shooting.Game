#ifndef _define_h_
#define _define_h_

#define CLASS_NAME "DirextX for Shooting"

#pragma comment(lib,"Dsound.lib")
#pragma comment(lib,"Ddraw.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"dxguid.lib")

#define MAX_EXPLODES 50
#define MAX_MISSILES 50
#define MAX_ENEMY 21
#define MAX_ENEMY_MISSILES 50
#define MAX_BOSS_MISSILES 50
#define MAX_BLOCK 18
#define MAX_ITEM 50

#define MY_MISSILE_INTERVAL 240
#define BOSS_MISSILE_INTERVAL 100

#include <Windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <list>
#include <math.h>
#include "stdio.h"

using namespace std;

#include "bmp.h"
#include "timer.h"
#include "enemy.h"
#include "sprite.h"
#include "myship.h"
#include "myship2.h"
#include "mymissile.h"
#include "exploding.h"
#include "enemymissile.h"
#include "boss.h"
#include "bossmissile.h"
#include "background.h"
#include "interface.h"
#include "interface2.h"
#include "interface3.h"
#include "block.h"
#include "item.h"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int COLOR_DEPTH = 32;

const int COLOR_KEY = 0;
#endif