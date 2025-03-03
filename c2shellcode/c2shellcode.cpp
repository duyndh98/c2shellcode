// c2shellcode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Header.h"

#pragma code_seg(push, ".textex$0111")
void Entry()
{
	CHAR strUser32[] = { 'u','s','e','r','3','2','.','d','l','l',0 };
	CHAR strMboxTitle[] = { 'S','h','e','l','l','S','t','d','i','o', 0 };
	CHAR strMboxMsg[] = { 'H','e','l','l','o',' ', 'W','o','r','l','d','!',0 };

	DEFINE_FUNC_PTR("kernel32.dll", LoadLibraryA);
	LoadLibraryA(strUser32);

	DEFINE_FUNC_PTR("user32.dll", MessageBoxA);
	MessageBoxA(NULL, strMboxMsg, strMboxTitle, MB_OK);

	return;
}
#pragma code_seg(pop)

void WriteShellcode(LPVOID shellcode, DWORD size, LPVOID entry)
{
	auto buf = (LPBYTE)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(buf, shellcode, size);

	auto fn = (void (*)())(buf + (DWORD_PTR)entry - (DWORD_PTR)shellcode);
	fn();
	
	return;
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	auto size = (DWORD_PTR)c2shellcode::EndShellcode - (DWORD_PTR)c2shellcode::BeginShellcode;
	auto shellcode = c2shellcode::BeginShellcode;

	WriteShellcode(shellcode, size, Entry);
	//Test();

	
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
