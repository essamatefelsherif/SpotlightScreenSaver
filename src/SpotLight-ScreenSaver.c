#include <windows.h>
#include <ScrnSave.h>
#include <commctrl.h>
#include "resource.h"

#pragma comment(lib, "Scrnsavw.lib")
#pragma comment(lib, "comctl32.lib")

#define WM_USER_MOVE (WM_USER + 1)

#define IDT_TIMER0			1000
#define IDT_TIMER1			1001
#define IDT_TIMER2			1002
#define IDT_TIMER3			1003
#define IDT_TIMER4			1004
#define IDT_TIMER5			1005

#define IDT_TIMER0_DELAY	 500
#define IDT_TIMER1_DELAY	1000
#define IDT_TIMER2_DELAY	 300
#define IDT_TIMER3_DELAY	   1
#define IDT_TIMER4_DELAY	1000
#define IDT_TIMER5_DELAY	   1

#define SHADE			      48

#define SPOT_FACTOR_X	      12
#define SPOT_FACTOR_Y	      12
#define SPOT_FACTOR_XMIN     128
#define SPOT_FACTOR_YMIN     128

#define NUM_SPOTS			   1
#define NUM_SPOTS_MAX		   4

typedef enum _tag_SpotShape {
	SS_ELLIPSE   = 0,
	SS_RECTANGLE = 1
} SpotShape;

typedef enum _tag_BLT {
	BLT_INIT        = 0,
	BLT_SCREEN_ORIG = 1,
	BLT_SCREEN_DARK = 2
} BLT;

LRESULT WINAPI ScreenSaverProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){

	static RECT rcClient;
	static HDC hdcMem, hdcMemDark;

	static BLT blt = BLT_INIT;

	static int cxScreen, cyScreen, xScreen, yScreen;
	static int cxDiam, cyDiam, cxDiamMin, cyDiamMin;

	static HRGN hrgn[NUM_SPOTS_MAX], hrgn_forw[NUM_SPOTS_MAX], hrgn_forw_diff1[NUM_SPOTS_MAX], hrgn_forw_diff2[NUM_SPOTS_MAX];
	static RECT rcRgn[NUM_SPOTS_MAX];
	static int dispX[NUM_SPOTS_MAX], dispY[NUM_SPOTS_MAX];
	static int nSpots = NUM_SPOTS;

	static SpotShape ss = SS_ELLIPSE;

	static TCHAR buffer[80];

	switch (message) {

	case WM_CREATE:{

		HDC hdcScreen;
		HBITMAP hbmScreen, hbmScreenDark;
		HBRUSH hbr, hbr_prev;
		CREATESTRUCT* pcs;
		WINDOWINFO wi;

		GetWindowInfo(hwnd, &wi);
		pcs = (CREATESTRUCT*)lParam;

		cxScreen = GetSystemMetrics(SM_CXSCREEN);
		cyScreen = GetSystemMetrics(SM_CYSCREEN);

		if (pcs->style & WS_CHILD){
			xScreen = wi.rcWindow.left;
			yScreen = wi.rcWindow.top;
		}
		else {
			xScreen = yScreen = 0;
		}

		cyDiam    = cxDiam    = (pcs->cx / SPOT_FACTOR_X > pcs->cy / 2) ? pcs->cy / 2 : pcs->cx / SPOT_FACTOR_X;
		cyDiamMin = cxDiamMin = (pcs->cx / SPOT_FACTOR_XMIN < 8) ? 8 : pcs->cx / SPOT_FACTOR_XMIN;

		hdcScreen  = GetDC(NULL);
		hdcMem     = CreateCompatibleDC(hdcScreen);
		hdcMemDark = CreateCompatibleDC(hdcScreen);

		hbmScreen     = CreateCompatibleBitmap(hdcScreen, cxScreen, cyScreen);
		hbmScreenDark = CreateCompatibleBitmap(hdcScreen, cxScreen, cyScreen);

		SelectObject(hdcMem, hbmScreen);
		SelectObject(hdcMemDark, hbmScreenDark);

		BitBlt(hdcMem, 0, 0, cxScreen, cyScreen, hdcScreen, 0, 0, SRCCOPY);

		hbr = CreateSolidBrush(RGB(SHADE, SHADE, SHADE));
		hbr_prev = SelectObject(hdcMemDark, hbr);

		BitBlt(hdcMemDark, 0, 0, cxScreen, cyScreen, hdcScreen, 0, 0, MERGECOPY);

		SelectObject(hdcMemDark, hbr_prev);
		DeleteObject(hbr);
		ReleaseDC(NULL, hdcScreen);

		GetClientRect(hwnd, &rcClient);

		SetTimer(hwnd, IDT_TIMER0, IDT_TIMER0_DELAY, NULL);
	}
	return 0;

	case WM_ERASEBKGND:{
		HDC hdc = (HDC)wParam;
		if(blt == BLT_SCREEN_ORIG){
			BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcMem, xScreen, yScreen, SRCCOPY);
			return 1;
		}
		else
		if(blt == BLT_SCREEN_DARK){
			BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcMemDark, xScreen, yScreen, SRCCOPY);
			return 1;
		}
	}
	return 0;

	case WM_TIMER:{
		switch (wParam) {
		case IDT_TIMER0:
			KillTimer(hwnd, IDT_TIMER0);

			blt = BLT_SCREEN_DARK;
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);

			SetTimer(hwnd, IDT_TIMER1, IDT_TIMER1_DELAY, NULL);
			break;
		case IDT_TIMER1:
			KillTimer(hwnd, IDT_TIMER1);

			switch (nSpots) {
			case 1:
				rcRgn[0].left   = (rcClient.right  - cxDiamMin) / 2;
				rcRgn[0].top    = (rcClient.bottom - cyDiamMin) / 2;
				rcRgn[0].right  = rcRgn[0].left + cxDiamMin - 1;
				rcRgn[0].bottom = rcRgn[0].top  + cyDiamMin - 1;

				if(ss == SS_ELLIPSE)
					hrgn[0] = CreateEllipticRgnIndirect(&rcRgn[0]);
				else
				if(ss == SS_RECTANGLE)
					hrgn[0] = CreateRectRgnIndirect(&rcRgn[0]);
				break;
			case 2:
				rcRgn[0].left   = rcClient.right / 2 - (rcClient.right / 2 - cxDiamMin) / 2 - cxDiamMin;
				rcRgn[0].top    = (rcClient.bottom - cyDiamMin) / 2;
				rcRgn[0].right  = rcRgn[0].left + cxDiamMin - 1;
				rcRgn[0].bottom = rcRgn[0].top  + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[0] = CreateEllipticRgnIndirect(&rcRgn[0]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[0] = CreateRectRgnIndirect(&rcRgn[0]);

				rcRgn[1].left   = rcClient.right / 2 + (rcClient.right / 2 - cxDiamMin) / 2;
				rcRgn[1].top    = (rcClient.bottom - cyDiamMin) / 2;
				rcRgn[1].right  = rcRgn[1].left + cxDiamMin - 1;
				rcRgn[1].bottom = rcRgn[1].top  + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[1] = CreateEllipticRgnIndirect(&rcRgn[1]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[1] = CreateRectRgnIndirect(&rcRgn[1]);
				break;
			case 3:
				rcRgn[0].left   = (rcClient.right - cxDiamMin) / 2;
				rcRgn[0].top    = rcClient.bottom / 2 - (rcClient.bottom / 2 - cyDiamMin) / 2 - cyDiamMin;
				rcRgn[0].right  = rcRgn[0].left + cxDiamMin - 1;
				rcRgn[0].bottom = rcRgn[0].top + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[0] = CreateEllipticRgnIndirect(&rcRgn[0]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[0] = CreateRectRgnIndirect(&rcRgn[0]);

				rcRgn[1].left = rcClient.right / 2 - (rcClient.right / 2 - cxDiamMin) / 2 - cxDiamMin;
				rcRgn[1].top = rcClient.bottom / 2 + (rcClient.bottom / 2 - cyDiamMin) / 2;
				rcRgn[1].right = rcRgn[1].left + cxDiamMin - 1;
				rcRgn[1].bottom = rcRgn[1].top + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[1] = CreateEllipticRgnIndirect(&rcRgn[1]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[1] = CreateRectRgnIndirect(&rcRgn[1]);

				rcRgn[2].left   = rcClient.right / 2  + (rcClient.right / 2  - cxDiamMin) / 2;
				rcRgn[2].top    = rcClient.bottom / 2 + (rcClient.bottom / 2 - cyDiamMin) / 2;
				rcRgn[2].right  = rcRgn[2].left + cxDiamMin - 1;
				rcRgn[2].bottom = rcRgn[2].top + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[2] = CreateEllipticRgnIndirect(&rcRgn[2]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[2] = CreateRectRgnIndirect(&rcRgn[2]);
				break;
			case 4:
				rcRgn[0].left = rcClient.right / 2 - (rcClient.right / 2 -  cxDiamMin) / 2 - cxDiamMin;
				rcRgn[0].top = rcClient.bottom / 2 - (rcClient.bottom / 2 - cyDiamMin) / 2 - cyDiamMin;
				rcRgn[0].right = rcRgn[0].left + cxDiamMin - 1;
				rcRgn[0].bottom = rcRgn[0].top + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[0] = CreateEllipticRgnIndirect(&rcRgn[0]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[0] = CreateRectRgnIndirect(&rcRgn[0]);

				rcRgn[1].left = rcClient.right / 2 + (rcClient.right / 2  - cxDiamMin) / 2;
				rcRgn[1].top = rcClient.bottom / 2 - (rcClient.bottom / 2 - cyDiamMin) / 2 - cyDiamMin;
				rcRgn[1].right = rcRgn[1].left + cxDiamMin - 1;
				rcRgn[1].bottom = rcRgn[1].top + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[1] = CreateEllipticRgnIndirect(&rcRgn[1]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[1] = CreateRectRgnIndirect(&rcRgn[1]);

				rcRgn[2].left = rcClient.right / 2 - (rcClient.right / 2 - cxDiamMin) / 2 - cxDiamMin;
				rcRgn[2].top = rcClient.bottom / 2 + (rcClient.bottom / 2 - cyDiamMin) / 2;
				rcRgn[2].right = rcRgn[2].left + cxDiamMin - 1;
				rcRgn[2].bottom = rcRgn[2].top + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[2] = CreateEllipticRgnIndirect(&rcRgn[2]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[2] = CreateRectRgnIndirect(&rcRgn[2]);

				rcRgn[3].left = rcClient.right / 2 + (rcClient.right / 2 - cxDiamMin) / 2;
				rcRgn[3].top = rcClient.bottom / 2 + (rcClient.bottom / 2 - cyDiamMin) / 2;
				rcRgn[3].right = rcRgn[3].left + cxDiamMin - 1;
				rcRgn[3].bottom = rcRgn[3].top + cyDiamMin - 1;

				if (ss == SS_ELLIPSE)
					hrgn[3] = CreateEllipticRgnIndirect(&rcRgn[3]);
				else
				if (ss == SS_RECTANGLE)
					hrgn[3] = CreateRectRgnIndirect(&rcRgn[3]);

				break;
			}

			blt = BLT_SCREEN_ORIG;

			for (int i = 0; i < NUM_SPOTS; i++)
				InvalidateRgn(hwnd, hrgn[i], TRUE);
			UpdateWindow(hwnd);

			SetTimer(hwnd, IDT_TIMER2, IDT_TIMER2_DELAY, NULL);
			break;
		case IDT_TIMER2:
			KillTimer(hwnd, IDT_TIMER2);
			SetTimer(hwnd, IDT_TIMER3, IDT_TIMER3_DELAY, NULL);
			break;
		case IDT_TIMER3:
			if (cxDiamMin < cxDiam && cyDiamMin < cyDiam) {
				HRGN hrgnTemp[NUM_SPOTS];

				cxDiamMin++;
				cyDiamMin++;

				for (int i = 0; i < NUM_SPOTS; i++) {
					rcRgn[i].left--;
					rcRgn[i].top--;
					rcRgn[i].right++;
					rcRgn[i].bottom++;

					if (ss == SS_ELLIPSE)
						hrgnTemp[i] = CreateEllipticRgnIndirect(&rcRgn[i]);
					else
					if(ss == SS_RECTANGLE)
						hrgnTemp[i] = CreateRectRgnIndirect(&rcRgn[i]);

					CombineRgn(hrgn[i], hrgnTemp[i], NULL, RGN_COPY);
					DeleteObject(hrgnTemp[i]);

					blt = BLT_SCREEN_ORIG;
					InvalidateRgn(hwnd, hrgn[i], TRUE);
					UpdateWindow(hwnd);
				}
			}
			else {
				KillTimer(hwnd, IDT_TIMER3);
				SetTimer(hwnd, IDT_TIMER4, IDT_TIMER4_DELAY, NULL);
			}
			break;
		case IDT_TIMER4:
			KillTimer(hwnd, IDT_TIMER4);

			for(int i = 0; i < NUM_SPOTS; i++) {

				if (ss == SS_RECTANGLE) {
					hrgn_forw[i] = CreateRectRgn(0, 0, 1, 1);
					hrgn_forw_diff1[i] = CreateRectRgn(0, 0, 1, 1);
					hrgn_forw_diff2[i] = CreateRectRgn(0, 0, 1, 1);
				}
				else
				if (ss == SS_ELLIPSE) {
					hrgn_forw[i] = CreateEllipticRgn(0, 0, 1, 1);
					hrgn_forw_diff1[i] = CreateEllipticRgn(0, 0, 1, 1);
					hrgn_forw_diff2[i] = CreateEllipticRgn(0, 0, 1, 1);
				}

				switch (i) {
				case 0:
					dispX[i] = 1;  dispY[i] = 1;
					break;
				case 1:
					dispX[i] = -1;  dispY[i] = 1;
					break;
				case 2:
					dispX[i] = 1;  dispY[i] = -1;
					break;
				case 3:
					dispX[i] = -1;  dispY[i] = -1;
					break;
				}
			}

			SetTimer(hwnd, IDT_TIMER5, IDT_TIMER5_DELAY, NULL);
			break;
		case IDT_TIMER5:
			PostMessage(hwnd, WM_USER_MOVE, 0, 0);
			break;
		}
	}
	return 0;

	case WM_USER_MOVE:{
		for (int i = 0; i < NUM_SPOTS; i++) {
			GetRgnBox(hrgn[i], &rcRgn[i]);

			if (rcRgn[i].right >= rcClient.right || rcRgn[i].left <= 0)
				dispX[i] = -dispX[i];
			if (rcRgn[i].bottom >= rcClient.bottom || rcRgn[i].top <= 0)
				dispY[i] = -dispY[i];

			CombineRgn(hrgn_forw[i], hrgn[i], NULL, RGN_COPY);
			OffsetRgn(hrgn_forw[i], dispX[i], dispY[i]);

			CombineRgn(hrgn_forw_diff1[i], hrgn_forw[i], hrgn[i], RGN_DIFF);
			CombineRgn(hrgn_forw_diff2[i], hrgn[i], hrgn_forw[i], RGN_DIFF);
		}

		for (int i = 0; i < NUM_SPOTS; i++) {
			blt = BLT_SCREEN_ORIG;
			InvalidateRgn(hwnd, hrgn_forw_diff1[i], TRUE);
			UpdateWindow(hwnd);
		}

		for (int i = 0; i < NUM_SPOTS; i++)
			for(int j = 0; j < NUM_SPOTS; j++)
				if(i != j)
					CombineRgn(hrgn_forw_diff2[i], hrgn_forw_diff2[i], hrgn_forw[j], RGN_DIFF);

		for (int i = 0; i < NUM_SPOTS; i++) {
			blt = BLT_SCREEN_DARK;
			InvalidateRgn(hwnd, hrgn_forw_diff2[i], TRUE);
			UpdateWindow(hwnd);
		}

		for (int i = 0; i < NUM_SPOTS; i++) {
			CombineRgn(hrgn[i], hrgn_forw[i], NULL, RGN_COPY);
		}
	}
	return 0;

	case WM_DESTROY:{
		KillTimer(hwnd, IDT_TIMER5);

		for (int i = 0; i < NUM_SPOTS; i++) {
			DeleteObject(hrgn[i]);
			DeleteObject(hrgn_forw[i]);
			DeleteObject(hrgn_forw_diff1[i]);
			DeleteObject(hrgn_forw_diff2[i]);
		}
		PostQuitMessage(0);
	}
	return 0;
	}
	return DefScreenSaverProc(hwnd, message, wParam, lParam);
}

BOOL CALLBACK ScreenSaverConfigureDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	TCHAR str[50];
	HINSTANCE hinst;

	switch (message) {
	case WM_INITDIALOG:
		hinst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
		LoadString(hinst, IDS_STRING, str, 50);
		MessageBox(
			hwnd,
			TEXT("No Options\r\n\r\nThis screen saver has no options that you can set."),
			str,
			MB_OK | MB_ICONINFORMATION);
		EndDialog(hwnd, 0);
		return 1;
	}
	return 0;
}

BOOL WINAPI RegisterDialogClasses(HINSTANCE hinst) {
	return 1;
}
