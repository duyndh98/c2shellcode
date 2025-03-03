#pragma once

#include <Windows.h>
#include <winnt.h>
#include <winternl.h>

#define ROR_SHIFT 13

namespace c2shellcode {

#ifndef _WIN64
#define getPEB() (PPEB)__readfsdword(0x30);
#else
#define getPEB() (PPEB)__readgsqword(0x60);
#endif

#define DEFINE_FUNC_PTR(module, function) \
	constexpr DWORD hash_##function = c2shellcode::ct_hash(module) + c2shellcode::ct_hash(#function); \
	typedef decltype(function) type_##function; \
	type_##function *##function = (type_##function *)c2shellcode::getProcAddrByHash(hash_##function)

#define DEFINE_FWD_FUNC_PTR(module, real_func, function) \
	constexpr DWORD hash_##function = c2shellcode::ct_hash(module) + c2shellcode::ct_hash(real_func); \
	typedef decltype(function) type_##function; \
	type_##function *##function = (type_##function *)c2shellcode::getProcAddrByHash(hash_##function)

	constexpr DWORD ct_ror(DWORD n) {
		return (n >> ROR_SHIFT) | (n << (sizeof(DWORD) * CHAR_BIT - ROR_SHIFT));
	}

	constexpr char ct_upper(const char c) {
		return (c >= 'a') ? (c - ('a' - 'A')) : c;
	}

	constexpr DWORD ct_hash(const char* str, DWORD sum = 0) {
		return *str ? ct_hash(str + 1, ct_ror(sum) + ct_upper(*str)) : sum;
	}

	DWORD rt_hash(const char* str);

	LDR_DATA_TABLE_ENTRY* getDataTableEntry(const LIST_ENTRY* ptr);

	PVOID getProcAddrByHash(DWORD hash);
}