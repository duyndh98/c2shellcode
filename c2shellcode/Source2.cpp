#include "Header.h"

namespace ns2 {

	SHELLCODE_FUNC(void Func2()
	{
		CHAR strMboxTitle[] = { 'S','h','e','l','l','S','t','d','i','o', 0 };
		CHAR strMboxMsg[] = { 'H','e','l','l','o',' ', 'W','o','r','l','d','!',0 };

		DEFINE_FUNC_PTR("user32.dll", MessageBoxA);
		MessageBoxA(NULL, strMboxMsg, strMboxTitle, MB_OK);
	})
}