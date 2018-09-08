#include "stdafx.h"

//this function is primarily to help me debug.
void CheckErr(HRESULT hr)
{
	//char szHR[16];
	//ltoa((LONG)hr, szHR, 10);
	//::MessageBox(NULL, (LPCTSTR)szHR, "HRESULT VALUE", MB_OK);

	switch(hr)
	{
		case S_OK:
			::MessageBox(NULL, "S_OK", "NO ERROR", MB_OK);
		break;

		case S_FALSE:
			::MessageBox(NULL, "S_FALSE", "FALSE", MB_OK);
		break;

		case E_FAIL:
			::MessageBox(NULL, "Err: E_FAIL", "ERROR!", MB_OK);
		break;

		case E_POINTER:
			::MessageBox(NULL, "Err: E_POINTER", "ERROR!", MB_OK);
		break;

		case CONNECT_E_NOCONNECTION:
			::MessageBox(NULL, "Err: CONNECT_E_NOCONNECTION", "ERROR!", MB_OK);
		break;
		
		case E_NOINTERFACE:
			::MessageBox(NULL, "Err: E_NOINTERFACE", "ERROR!", MB_OK);
		break;

		case E_UNEXPECTED:
			::MessageBox(NULL, "Err: E_UNEXPECTED", "ERROR!", MB_OK);
		break;
		
		case DISP_E_EXCEPTION:
			::MessageBox(NULL, "Err: DISP_E_EXCEPTION", "ERROR!", MB_OK);
		break;

		case DISP_E_UNKNOWNLCID:
			::MessageBox(NULL, "Err: DISP_E_UNKNOWNLCID", "ERROR!", MB_OK);
		break;

		default:
			::MessageBox(NULL, "Err: ??????????", "", MB_OK);
		break;
	}
}

void ShowLastError(LPTSTR funcName)
{
	TCHAR szErrMsg[100];
	LPVOID lpMsgBuf;
	DWORD lastErr = GetLastError(); //grab the error code of last error
	
	//this will format the message into our buffer, using GetLastError() as its source
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL,
		lastErr, GetSystemDefaultLangID(), (LPTSTR)&lpMsgBuf, 0, NULL);
	
	//format, and output our information from the buffer into szErrMsg string.
	wsprintf(szErrMsg, " Function: %s\n Error#: %d\n Description: %s", 
		funcName, lastErr, lpMsgBuf);

	::MessageBox(NULL, szErrMsg, "Internal Error", MB_OK);
	
	//free the buffer from memory
	LocalFree(lpMsgBuf);
}

//converts binary data in ascii form to binary data in a BSTR
void BinAsciiToBinBSTR(LPCSTR szBinDataA, UINT iBinALen, BSTR* szBinData_Buf)
{
	//specify the length for the unicode string
	UINT iBinAWLen = iBinALen * 2; 
	//allocate memory for our unicode string
	WCHAR* szBinDataW = new WCHAR[iBinALen]; 
	//allocate memory for the ascii disguised unicode string
	CHAR* szBinDataAW = new CHAR[iBinAWLen]; 

	//this api function will convert our ascii data to unicode
	INT convLen = MultiByteToWideChar(	CP_ACP, 
										0, 
										szBinDataA, 
										iBinALen,
										szBinDataW, 
										iBinALen);

	//copy our unicode memory block into our ascii string memory buffer
	CopyMemory(szBinDataAW, szBinDataW, iBinAWLen);

	//places the ascii data (as it is) into our BSTR buffer
	*szBinData_Buf = SysAllocStringByteLen(szBinDataAW, iBinAWLen);

	//free memory
	delete[] szBinDataW;
	delete[] szBinDataAW;
}

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	HDC hdcMem, hdcMem2;
	HBITMAP hbmMask;
	BITMAP bm;

	GetObject(hbmColour, sizeof(BITMAP), &bm);
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	hdcMem = CreateCompatibleDC(0);
	hdcMem2 = CreateCompatibleDC(0);

	SelectObject(hdcMem, hbmColour);
	SelectObject(hdcMem2, hbmMask);

	SetBkColor(hdcMem, crTransparent);

	BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);

	return hbmMask;
}

CHIPVECINFO* CheckConnect4(SHORT CurWndIndex, SLOTWNDINFO *SlotWndInfoArr)
{
	CHIPVECINFO *vecConFour = new CHIPVECINFO[4];

	SHORT chipX, chipY;
	chipX = CurWndIndex;
	chipY = (GAME_ROWS-1) - (SlotWndInfoArr[CurWndIndex].chipCnt - 1);

	//Get rect of overall area
	RECT SlotsRect;
	SlotsRect.left = 0;
	SlotsRect.top = 0;
	SlotsRect.right = GAME_COLS-1;
	SlotsRect.bottom = GAME_ROWS-1;

	//Get rect of the area bounding all possibilities
	//i.e	CurWndIndex - 3, CurWndIndex + 3
	//		SlotWndInfo.chipCnt - 3, SlotWndInfo.chipCnt + 3
	RECT Con4RectBounds;
	Con4RectBounds.left = chipX - 3;
	Con4RectBounds.top = chipY - 3;
	Con4RectBounds.right = chipX + 3;
	Con4RectBounds.bottom = chipY + 3;

	//use IntersectRect to clip off all areas in the possibilities rect that
	//go out of the bounds in the ovarall area rect.
	RECT Con4Rect;

	IntersectRect(&Con4Rect, &SlotsRect, &Con4RectBounds);
	
	//create 4 RECT arrays representing the horizontal, vertical, 
	//left-diagonal, right-diagonal directions of possibilities
	SHORT chipCnt = 0;
	bool foundCon4 = false;
	UINT lastChipState;
	UINT curChipState;

	//check for a horizontal connect 4
	for(SHORT i = Con4Rect.left; i <= Con4Rect.right; i++)
	{
		curChipState = SlotWndInfoArr[i].chipLocState[chipY];
		
		//if its the first time through don't bother checking if our current
		//chip matches the last chip
		if(i > Con4Rect.left)
			chipCnt = (curChipState == lastChipState) ? chipCnt+1 : 0;
		
		if(chipCnt == 3)
		{
			vecConFour[0].points[0].x = i-3;
			vecConFour[0].points[1].x = i-2;
			vecConFour[0].points[2].x = i-1;
			vecConFour[0].points[3].x = i;
			vecConFour[0].points[3].y = vecConFour[0].points[2].y = 
				vecConFour[0].points[1].y = vecConFour[0].points[0].y = chipY;
			vecConFour[0].vecType = HORZ;
			vecConFour[0].IsConnectFour = true;

			foundCon4 = true;
			break;
		}

		lastChipState = curChipState;
	}
	
	//check for vertical connect 4
	chipCnt = 0;
	for(SHORT i = Con4Rect.top; i <= Con4Rect.bottom; i++)
	{
		curChipState = SlotWndInfoArr[chipX].chipLocState[i];

		if(i > Con4Rect.top)
			chipCnt = (curChipState == lastChipState) ? chipCnt+1 : 0;
		
		if(chipCnt == 3)
		{
			vecConFour[1].points[0].y = i-3;
			vecConFour[1].points[1].y = i-2;
			vecConFour[1].points[2].y = i-1;
			vecConFour[1].points[3].y = i;
			vecConFour[1].points[3].x = vecConFour[1].points[2].x = 
				vecConFour[1].points[1].x = vecConFour[1].points[0].x = chipX;
			vecConFour[1].vecType = VERT;
			vecConFour[1].IsConnectFour = true;

			foundCon4 = true;
			break;
		}

		lastChipState = curChipState;
	}
	
	
	//grab the coordinates from our possibilites bound rect, not just from the 
	//possibilities bounds rect that intersects with the overall slots rect.
	//so that we know our points align up with the placement of our chip
	POINT diagStartPnt;
	diagStartPnt.x = Con4RectBounds.left;
	diagStartPnt.y = Con4RectBounds.top;
	
	//check for left diagonal connect4
	bool firstLoop = true;
	chipCnt = 0;
	while(PtInRect(&Con4RectBounds, diagStartPnt) |
		(diagStartPnt.x == Con4RectBounds.right && 
			diagStartPnt.y == Con4RectBounds.bottom))
	{
		if(PtInRect(&Con4Rect, diagStartPnt) |
			(diagStartPnt.x == Con4Rect.right && diagStartPnt.y == Con4Rect.bottom))
		{
			curChipState = SlotWndInfoArr[diagStartPnt.x].chipLocState[diagStartPnt.y];
			
			if(firstLoop == false)
				chipCnt = (curChipState == lastChipState) ? chipCnt+1 : 0;

			if(chipCnt == 3)
			{
				vecConFour[2].points[0].x = diagStartPnt.x-3;
				vecConFour[2].points[0].y = diagStartPnt.y-3;
				vecConFour[2].points[1].x = diagStartPnt.x-2;
				vecConFour[2].points[1].y = diagStartPnt.y-2;
				vecConFour[2].points[2].x = diagStartPnt.x-1;
				vecConFour[2].points[2].y = diagStartPnt.y-1;
				vecConFour[2].points[3].x = diagStartPnt.x;
				vecConFour[2].points[3].y = diagStartPnt.y;
				vecConFour[2].vecType = DESCDIAG;
				vecConFour[2].IsConnectFour = true;

				foundCon4 = true;
				break;
			}

			if(firstLoop)
				firstLoop = false;

			lastChipState = curChipState;
		}

		diagStartPnt.x++;
		diagStartPnt.y++;
	};
	
	diagStartPnt.x = Con4RectBounds.right;
	diagStartPnt.y = Con4RectBounds.top;
	
	//check for right diagonal connect four
	firstLoop = true;
	chipCnt = 0;
	while((diagStartPnt.x <= Con4RectBounds.right && 
			diagStartPnt.y >= Con4RectBounds.top) &&
		(diagStartPnt.x >= Con4RectBounds.left && 
			diagStartPnt.y <= Con4RectBounds.bottom))
	{
		if((diagStartPnt.x <= Con4Rect.right && 
				diagStartPnt.y >= Con4Rect.top) &&
			(diagStartPnt.x >= Con4Rect.left && 
				diagStartPnt.y <= Con4Rect.bottom))
		{
			curChipState = SlotWndInfoArr[diagStartPnt.x].chipLocState[diagStartPnt.y];
			
			if(firstLoop == false)
				chipCnt = (curChipState == lastChipState) ? chipCnt+1 : 0;

			if(chipCnt == 3)
			{
				vecConFour[3].points[0].x = diagStartPnt.x+3;
				vecConFour[3].points[0].y = diagStartPnt.y-3;
				vecConFour[3].points[1].x = diagStartPnt.x+2;
				vecConFour[3].points[1].y = diagStartPnt.y-2;
				vecConFour[3].points[2].x = diagStartPnt.x+1;
				vecConFour[3].points[2].y = diagStartPnt.y-1;
				vecConFour[3].points[3].x = diagStartPnt.x;
				vecConFour[3].points[3].y = diagStartPnt.y;
				vecConFour[3].vecType = ASCDIAG;
				vecConFour[3].IsConnectFour = true;

				foundCon4 = true;
				break;
			}

			if(firstLoop)
				firstLoop = false;

			lastChipState = curChipState;
		}

		diagStartPnt.x--;
		diagStartPnt.y++;
	};


	if(foundCon4)
		return vecConFour;
	else
		return NULL;
}

void DrawAllChips(HDC hdc, HDC hdcMem, HBITMAP hbmChip, 
				  CHIPLOCSTATE ChipType, SLOTWNDINFO *lpSlotWndInfo, DWORD dwROP)
{
	BITMAP bm;
	INT Y = SLOT_HEIGHT - 34;

	SelectObject(hdcMem, hbmChip);
	GetObject(hbmChip, sizeof(bm), &bm);

	//look through, and draw the specified bitmap for the specified chiptype
	for(int i = 0; i < lpSlotWndInfo->chipCnt; i++)
	{
		if(lpSlotWndInfo->chipLocState[(GAME_ROWS-1) - i] == ChipType)
			BitBlt(hdc, 0, Y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, dwROP);
		
		Y -= 36;
	}
}

void DrawTurnIndicator(HWND hWnd, CURRENTTURN CurrentTurn, COLORREF clrRect)
{
	HDC hdc = GetDC(hWnd);

	DrawTurnIndicator(hdc, CurrentTurn, clrRect);

	ReleaseDC(hWnd, hdc);
}

void DrawTurnIndicator(HDC hdc, CURRENTTURN CurrentTurn, COLORREF clrRect)
{
	RECT rectIndicator;

	if(CurrentTurn == REDPLAYER)
	{
		rectIndicator.left = 310;
		rectIndicator.top = 5;
		rectIndicator.right = rectIndicator.left+58;
		rectIndicator.bottom = rectIndicator.top+290;
	}
	else if(CurrentTurn == BLUEPLAYER)
	{
		rectIndicator.left = 368;
		rectIndicator.top = 5;
		rectIndicator.right = rectIndicator.left+58;
		rectIndicator.bottom = rectIndicator.top+290;
	}
	
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	
	/*if(IsTransparent)
		SelectObject(hdc, GetStockObject(NULL_PEN));
		
		RoundRect(hdc, rectIndicator.left, rectIndicator.top, 
					rectIndicator.right, rectIndicator.bottom, 15, 15);

		SelectObject(hdc, GetStockObject(BLACK_PEN))
	else
	{*/
		SelectObject(hdc, CreatePen(PS_SOLID, 3, clrRect));

		RoundRect(hdc, rectIndicator.left, rectIndicator.top, 
						rectIndicator.right, rectIndicator.bottom, 15, 15);

		DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
	//}

	SelectObject(hdc, oldBrush);
}

void HighLightConnectFour(HBITMAP hbmHLChip, HBITMAP hbmHLChipMask,
						  CHIPVECINFO *lpConFourVecs, SLOTWNDINFO *SlotWndInfo)
{
	for(int i = 0; i < 4; i++)
	{
		if(lpConFourVecs[i].IsConnectFour == true)
		{
			POINTS *pnt = lpConFourVecs[i].points;
			
			switch(lpConFourVecs[i].vecType)
			{
				//if its vertical all the chips are within the same slot window
				case VERT:
				{
					HDC hdc = GetDC(SlotWndInfo[pnt[0].x].hWnd);
					HDC hdcChipBuffer = CreateCompatibleDC(hdc);
					HDC hdcBuffer = CreateCompatibleDC(hdc);
					HDC hdcMem = CreateCompatibleDC(hdc);
					HBITMAP hbmBuffer = CreateCompatibleBitmap(	hdc, 
																CHIP_WIDTH, 
																(36 * 4));
					HBITMAP hbmChipBuffer = CreateCompatibleBitmap(	hdc,
																	CHIP_WIDTH,
																	CHIP_HEIGHT);
					HBITMAP hbmOld;
					HBITMAP hbmOldBuffer;
					HBITMAP hbmOldChipBuffer;
					BITMAP bm;
				
					hbmOldChipBuffer = (HBITMAP)SelectObject(hdcChipBuffer, hbmChipBuffer);
					hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);
					
					COLORREF clrYellow = RGB(214, 214, 0);

					RECT r;
					SetRect(&r, 0, 0, CHIP_WIDTH, (36 * 4));
					RECT r2;
					SetRect(&r2, 0, 0, CHIP_WIDTH, CHIP_HEIGHT);

					HBRUSH hBrush = CreateSolidBrush(clrYellow);

					FillRect(hdcChipBuffer, &r2, hBrush);
					FillRect(hdcBuffer, &r, hBrush);

					hbmOld = (HBITMAP)SelectObject(hdcMem, hbmHLChipMask);
					GetObject(hbmHLChipMask, sizeof(bm), &bm);

					BitBlt(hdcChipBuffer, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCAND);

					SelectObject(hdcMem, hbmHLChip);
					GetObject(hbmHLChip, sizeof(bm), &bm);

					BitBlt(hdcChipBuffer, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCPAINT);

					int i = 0;

					UINT Y = 0;
					for(i = 0; i < 4; i++)
					{
						BitBlt(hdcBuffer, 0, Y, bm.bmWidth, bm.bmHeight, hdcChipBuffer, 0, 0, SRCCOPY);
						Y += 36;
					}

					Y = (36 * pnt[0].y);
					BitBlt(hdc, 0, Y, bm.bmWidth, (36 * 4), hdcBuffer, 0, 0, SRCCOPY);
					
					SelectObject(hdcChipBuffer, hbmOldChipBuffer);
					DeleteDC(hdcChipBuffer);

					SelectObject(hdcBuffer, hbmOldBuffer);
					DeleteDC(hdcBuffer);

					SelectObject(hdcMem, hbmOld);
					DeleteDC(hdcMem);

					DeleteObject(hbmBuffer);
					DeleteObject(hBrush);

					ReleaseDC(SlotWndInfo[pnt[0].x].hWnd, hdc);
				}
				break;

				//otherwise there spread accross seperate slot windows
				default:
				{
					HDC hdc[4];
					HDC hdcMem;
					HBITMAP hbmOld;
					BITMAP bm;

					//get the dc's of the 4 windows the connect four vector
					//are spread across;
					hdc[0] = GetDC(SlotWndInfo[pnt[0].x].hWnd);
					hdc[1] = GetDC(SlotWndInfo[pnt[1].x].hWnd);
					hdc[2] = GetDC(SlotWndInfo[pnt[2].x].hWnd);
					hdc[3] = GetDC(SlotWndInfo[pnt[3].x].hWnd);
					
					//we only need one memory dc, it will work with all of our
					//previously obtained dc's
					hdcMem = CreateCompatibleDC(hdc[0]);

					hbmOld = (HBITMAP)SelectObject(hdcMem, hbmHLChipMask);
					GetObject(hbmHLChipMask, sizeof(bm), &bm);

					INT i = 0;
					INT Y;

					for(; i < 4; i++)
					{
						Y = ((CHIP_HEIGHT + SLOT_VSPACE) * pnt[i].y);
						BitBlt(hdc[i], 0, Y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCAND);
					}

					SelectObject(hdcMem, hbmHLChip);
					GetObject(hbmHLChip, sizeof(bm), &bm);

					for(i = 0; i < 4; i++)
					{
						Y = ((CHIP_HEIGHT + SLOT_VSPACE) * pnt[i].y);
						BitBlt(hdc[i], 0, Y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCPAINT);
					}

					//free our bitmap GDI object out of the dc.
					SelectObject(hdcMem, hbmOld);
					//free all of our dc's
					DeleteDC(hdcMem);
					ReleaseDC(SlotWndInfo[pnt[0].x].hWnd, hdc[0]);
					ReleaseDC(SlotWndInfo[pnt[1].x].hWnd, hdc[1]);
					ReleaseDC(SlotWndInfo[pnt[2].x].hWnd, hdc[2]);
					ReleaseDC(SlotWndInfo[pnt[3].x].hWnd, hdc[3]);
				}
				break;
			}
		}
	}
}