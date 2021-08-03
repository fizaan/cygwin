#pragma once

#include <Windows.h>
#include "Palette.h"

// Declare global variables
CHAR_INFO * buffer;
HANDLE console;
short len, hei;
SMALL_RECT m_rectWindow;

COORD getPoint(int x, int y) {
	COORD c;
	c.X = x;
	c.Y = y;
	return c;
}

void init(short w, short h, short fontw, short fonth, int scaleFactor) { 
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	len = w;
	hei = h;

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw * scaleFactor;
	cfi.dwFontSize.Y = fonth * scaleFactor;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	SetCurrentConsoleFontEx(console, FALSE, &cfi);

	SetConsoleScreenBufferSize(console, getPoint(len, hei));

	m_rectWindow.Left = 0;
	m_rectWindow.Top = 0;
	m_rectWindow.Right = len - 1;
	m_rectWindow.Bottom = hei - 1;

	SetConsoleWindowInfo(console, TRUE, &m_rectWindow);

	int size = len * hei;
	buffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * size);

}

/*
		fillRect from point A(x,y) to point B(x,y)

		(0,0) +------------+
			  +            +
			  +            +
			  +            +
			  +------------+ (len,hei)
*/
void fillRect(wchar_t c, COORD pointA, COORD pointB, short colour) {
	for (int x = pointA.X; x < pointB.X; x++)
		for (int y = pointA.Y; y < pointB.Y; y++) {
			buffer[x + y * len].Char.UnicodeChar = c;
			buffer[x + y * len].Attributes = colour;
		}

}

void drawAt(wchar_t c, int x, int y, short colour) {
	buffer[x + y * len].Char.UnicodeChar = c;
	buffer[x + y * len].Attributes = colour;
}

// Javix9's Youtube - Beginner's guide to console engine
void showPalette() {
	for (int i = 0; i < 16; i++) {
		fillRect(PIXEL_QUARTER, getPoint(0, i * 6), getPoint(5, i * 6 + 5), i);
		fillRect(PIXEL_HALF, getPoint(6, i * 6), getPoint(11, i * 6 + 5), i);
		fillRect(PIXEL_THREEQUARTERS, getPoint(12, i * 6), getPoint(17, i * 6 + 5), i);
		fillRect(PIXEL_SOLID, getPoint(18, i * 6), getPoint(23, i * 6 + 5), i);

		fillRect(PIXEL_THREEQUARTERS, getPoint(24, i * 6), getPoint(29, i * 6 + 5), i | BG_WHITE);
		fillRect(PIXEL_QUARTER, getPoint(30, i * 6), getPoint(35, i * 6 + 5), i | BG_WHITE);
		fillRect(PIXEL_HALF, getPoint(36, i * 6), getPoint(41, i * 6 + 5), i | BG_WHITE);
	}
}

// Javidx9's Youtube -  Console game engine
void goNuts() {
	for (int x = 0; x < len; x++)
		for (int y = 0; y < hei; y++)
			drawAt(PIXEL_SOLID, x, y, rand() % 16);
}

void refresh() {
	WriteConsoleOutput(console, buffer, getPoint(len, hei), getPoint(0,0), &m_rectWindow);
}

void dismiss() { free(buffer); }
