#include <dsound.h>
#include "dsutil.h"
#include "define.h"

bool InitDirectDraw(void);
bool InitDirectInput(void);

extern HWND g_hwnd;
extern HINSTANCE g_hInstance;

extern LPDIRECTDRAW7 g_lpDirectDrawObject;
extern LPDIRECTDRAWSURFACE7 g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_lpSecondarySurface;

extern LPDIRECTINPUT8 g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;

LPDIRECTSOUND SoundOBJ;
LPDIRECTSOUNDBUFFER SoundDSB;
DSBUFFERDESC DSB_desc;
HSNDOBJ Sound[10];

bool InitializeDirectX(void){
	if (!InitDirectDraw())
		return false;
	if (!InitDirectInput())
		return false;
	return true;
}
bool InitDirectDraw(void){
	if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&g_lpDirectDrawObject, IID_IDirectDraw7, NULL)))
		return false;
	if (FAILED(g_lpDirectDrawObject->SetCooperativeLevel(g_hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)))
		return false;
	if (FAILED(g_lpDirectDrawObject->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH, 0, 0)))
		return false;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;
	if (FAILED(g_lpDirectDrawObject->CreateSurface(&ddsd, &g_lpPrimarySurface, NULL)))
		return false;
	DDSCAPS2 ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (FAILED(g_lpPrimarySurface->GetAttachedSurface(&ddscaps, &g_lpSecondarySurface)))
		return false;
	LPDIRECTDRAWCLIPPER lpClipper;
	if (FAILED(g_lpDirectDrawObject->CreateClipper(NULL, &lpClipper, NULL)))
		return false;
	if (FAILED(lpClipper->SetHWnd(NULL, g_hwnd)))
		return false;
	if (FAILED(g_lpSecondarySurface->SetClipper(lpClipper)))
		return false;
	return true;
}
bool InitDirectInput(void){
	if (FAILED(DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&g_lpDirectInputObject, NULL)))
		return false;
	if (FAILED(g_lpDirectInputObject->CreateDevice(GUID_SysKeyboard, &g_lpDirectInputKeyboard, NULL)))
		return false;
	if (FAILED(g_lpDirectInputKeyboard->SetCooperativeLevel(g_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;
	if (FAILED(g_lpDirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard)))//
		return false;
	g_lpDirectInputKeyboard->Acquire();
	return true;
}
extern BOOL _InitDirectSound(void){
	if (DirectSoundCreate(NULL, &SoundOBJ, NULL) == DS_OK){
		if (SoundOBJ->SetCooperativeLevel(g_hwnd, DSSCL_PRIORITY) != DS_OK)
			return FALSE;
		memset(&DSB_desc, 0, sizeof(DSBUFFERDESC));
		DSB_desc.dwSize = sizeof(DSBUFFERDESC);
		DSB_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
		if (SoundOBJ->CreateSoundBuffer(&DSB_desc, &SoundDSB, NULL) != DS_OK)
			return FALSE;
		SoundDSB->SetVolume(0);
		SoundDSB->SetPan(0);
		Sound[0] = SndObjCreate(SoundOBJ, "sound\\bg.wav", 1);
		Sound[1] = SndObjCreate(SoundOBJ, "sound\\gun.wav", 2);
		Sound[2] = SndObjCreate(SoundOBJ, "sound\\explosion.wav", 3);
		Sound[3] = SndObjCreate(SoundOBJ, "sound\\gameend.wav", 2);
		return TRUE;
	}
	return FALSE;
}