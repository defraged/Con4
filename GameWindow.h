#pragma once

#define IDC_GAME_WND 227

#define MAX_PLAYERS 2
#define MAX_VIEWERS 4

#define GAME_MAX 6
#define GAME_MIN 1

#define CHIP_WIDTH 34
#define CHIP_HEIGHT 34

#define SLOT_WIDTH 34
#define SLOT_HEIGHT 214
#define SLOT_VSPACE 2
#define SLOT_HSPACE 3
#define SLOT_STARTPNTX 19
#define SLOT_STARTPNTY 65

#define SEAT1_TOP 15
#define SEAT1_LEFT 319
#define SEAT1_WIDTH 40
#define SEAT1_HEIGHT 55

#define SEAT2_TOP 15
#define SEAT2_LEFT 377
#define SEAT2_WIDTH 40
#define SEAT2_HEIGHT 55

typedef struct _SLOTWNDINFO
{
	HWND hWnd;
	SHORT Index;
	SHORT chipCnt;
	bool IsPlacingChip;
	UINT chipLocState[GAME_ROWS];
} SLOTWNDINFO;

typedef struct _CHIPINFO
{
	INT width;
	INT height;
	INT x;
	INT y;

	INT dx;
	INT dy;
} CHIPINFO;

//for now i'll leave these functions here but later on it'd be nice to 
//move all my utility functions into organized files
void CheckErr(HRESULT hr);

void ShowLastError(LPTSTR funcName);

void BinAsciiToBinBSTR(LPCSTR szBinDataA, UINT szALen, BSTR* szBinData_Buf);


HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);

CHIPVECINFO* CheckConnect4(SHORT CurWndIndex, SLOTWNDINFO *SlotWndInfoArr);

void DrawAllChips(HDC hdc, HDC hdcMem, HBITMAP hbmChip, 
				  CHIPLOCSTATE ChipType, SLOTWNDINFO *lpSlotWndInfo, DWORD dwROP);

void DrawTurnIndicator(HWND hWnd, CURRENTTURN CurrentTurn, COLORREF clrRect);
void DrawTurnIndicator(HDC hdc, CURRENTTURN CurrentTurn, COLORREF clrRect);

void HighLightConnectFour(HBITMAP hbmHLChip, HBITMAP hbmHLChipMask, 
						  CHIPVECINFO *lpConFourVecs, SLOTWNDINFO *SlotWndInfo);
