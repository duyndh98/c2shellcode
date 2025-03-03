# c2shellcode
c2shellcode is a lightweight and powerful tool that converts C source code into raw shellcode, making it easy to craft payloads for security research, exploit development, and reverse engineering.

## Demo

```cpp
#include "Header.h"

SHELLCODE_FUNC(void Func2()
{
	CHAR strMboxTitle[] = { 'S','h','e','l','l','S','t','d','i','o', 0 };
	CHAR strMboxMsg[] = { 'H','e','l','l','o',' ', 'W','o','r','l','d','!',0 };

	DEFINE_FUNC_PTR("user32.dll", MessageBoxA);
	MessageBoxA(NULL, strMboxMsg, strMboxTitle, MB_OK);
})

SHELLCODE_ENTRY(void Func1()
{
	CHAR strUser32[] = { 'u','s','e','r','3','2','.','d','l','l',0 };

	DEFINE_FUNC_PTR("kernel32.dll", LoadLibraryA);
	LoadLibraryA(strUser32);

	Func2();

	return;
})

void WriteShellcode(LPVOID shellcode, LPCWSTR file_path)
{
	auto size = (DWORD_PTR)c2shellcode::EndShellcode - (DWORD_PTR)shellcode;
	auto file_handle = CreateFileW(file_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD length = 0;
	WriteFile(file_handle, shellcode, size, &length, NULL);
	CloseHandle(file_handle);

	return;
}

int main()
{
	WriteShellcode(Func1, L"shellcode.bin");
	return 0;
}
```

## Acknowledgments
A big thanks to @jackullrich for creating [ShellcodeStdio](https://github.com/jackullrich/ShellcodeStdio), which inspired this project.
