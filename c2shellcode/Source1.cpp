#include "Header.h"

namespace ns2 {
	void Func2();
}

namespace ns1 {

	SHELLCODE_ENTRY(void Func1()
	{
		CHAR strUser32[] = { 'u','s','e','r','3','2','.','d','l','l',0 };

		DEFINE_FUNC_PTR("kernel32.dll", LoadLibraryA);
		LoadLibraryA(strUser32);

		ns2::Func2();

		return;
	})
}