
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdlib.h>

char rbuf[BUFSIZ] = { 0 };
char wbuf[BUFSIZ] = { 0 };
char cbuf[BUFSIZ] = { 0 };

void read_message() {
	int length = 0;
	memset(rbuf, 0, BUFSIZ);
	fread(&length, sizeof(int), 1, stdin);
	fread(rbuf, sizeof(char), length, stdin);
}

void write_message(const char* message) {
	sprintf_s(wbuf, message);
	int t_length = strlen(wbuf);
	fwrite(&t_length, sizeof(int), 1, stdout);
	fwrite(wbuf, sizeof(char), t_length, stdout);
	fflush(stdout);
	memset(wbuf, 0, BUFSIZ);
}

void setOnTop(HWND hwnd) {
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void unsetOnTop(HWND hwnd) {
	SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void changeOpacity(HWND hwnd, BYTE alpha) {
	if (!(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED)) {
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	}
	SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
}
const int OPACITY_LEN = strlen("\"opacity:");
int main()
{
	while (1) {
		read_message();
		HWND fgwin = GetForegroundWindow();
		if (strcmp(rbuf, "\"on\"") == 0) {
			setOnTop(fgwin);
			write_message("{\"msg\":\"done\"}");
		}
		else if (strcmp(rbuf, "\"off\"") == 0) {
			unsetOnTop(fgwin);
			write_message("{\"msg\":\"done\"}");
		}
		else if (strncmp(rbuf, "\"opacity:", OPACITY_LEN) == 0) {
			BYTE val = 0;
			for (char* ch = rbuf + OPACITY_LEN; *ch != '\"'; ch++)
				val = val * 10 + (*ch - '0');
			changeOpacity(fgwin, val);
			write_message("{\"msg\":\"done\"}");
		}
		else {
			write_message("{\"msg\":\"wrong message\"}");
		}
	}
	return 0;
}