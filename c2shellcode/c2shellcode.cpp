// c2shellcode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Header.h"

namespace ns1 {
	void Func1();
}

void WriteShellcode(LPVOID shellcode, LPCWSTR file_path)
{
	auto size = (DWORD_PTR)c2shellcode::EndShellcode - (DWORD_PTR)shellcode;
	auto file_handle = CreateFileW(file_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD length = 0;
	WriteFile(file_handle, shellcode, size, &length, NULL);
	CloseHandle(file_handle);

#ifdef _DEBUG
	auto buffer = (LPBYTE)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(buffer, shellcode, size);

	auto fn = (void (*)())(buffer);
	fn();
	
	VirtualFree(buffer, size, MEM_DECOMMIT);
#endif

	return;
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	WriteShellcode(ns1::Func1, L"shellcode.bin");
	
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
