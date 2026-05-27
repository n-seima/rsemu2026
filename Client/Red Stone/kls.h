#ifndef _KLS_H_
#define _KLS_H_

#ifndef _LIB
#pragma comment(lib, "kls.lib")
#endif // _LIB

BOOL SetShield(HWND hWnd, HINSTANCE hInstance);
void UnsetShield();

int GetInputChars(char* Chars);
void ClearInputChars();

#endif _KLS_H_