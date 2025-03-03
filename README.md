# c2shellcode
c2shellcode is a lightweight and powerful tool that converts C source code into raw shellcode, making it easy to craft payloads for security research, exploit development, and reverse engineering.

## Notes ([ShellcodeStdio](https://github.com/jackullrich/ShellcodeStdio))

* You must declare all your variables on the stack.
* Strings are no exception. This is how to create a char*, wchar_t* on the stack. Don't forget to null-terminate.
```cpp
unsigned char str[] = { 'E', 'x', 'a', 'm', 'p', 'l', 'e', 0 }; 
unsigned char w_str[] = { 'E', 0, 'x', 0, 'a', 0, 'm', 0, 'p', 0, 'l', 0, 'e', 0, 0 };
```
* If you want to use an external function (i.e Windows API) you must first ensure the module containing the function is loaded into the current process.
* The second part of calling an external function is calling the DEFINE_FUNC_PTR or DEFINE_FWD_FUNC_PTR macro with the name of the module and the typedef of the function you want to call.
* Any forwarded functions will need to be defined using the DEFINE_FWD_FUNC_PTR macro.
* ~~If you wish to add a function you must keep it between the MalCodeBegin and MalCodeEnd markers.~~
* ~~Don't forget to call the ScStdio::MalCode() at some point, otherwise when compiling in release with the suggested switches, the code will optimized away.~~

## New features

- [x] You can add functions from any source file; they are no longer restricted to the MalCodeBegin and MalCodeEnd markers.
- [x] You can define an Entrypoint function without explicitly calling it; it will be preserved even in release builds with the suggested optimization switches.

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

## References
* https://anvilproject.org/guides/content/creating-links
* https://devblogs.microsoft.com/oldnewthing/20181107-00/?p=100155
