#include "DLLResourceLoader.h"

using namespace DDEngine;

DataContainer DLLResourceLoader::loadFromDLL( HMODULE hModule, LPCWSTR type, int ID ) {
	HRSRC hrsrc = FindResourceEx(hModule, type, MAKEINTRESOURCE(ID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
	HGLOBAL hGlobal = LoadResource(hModule, hrsrc);
	
	LPVOID dataBlobScheme = LockResource(hGlobal);
	DWORD dataSizeScheme = SizeofResource(hModule, hrsrc);

	return DataContainer(dataBlobScheme, dataSizeScheme);
}

DataContainer DLLResourceLoader::loadCEGUI_scheme( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_SCHEME", 112);
}

DataContainer DLLResourceLoader::loadCEGUI_imageset( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_IMAGESET", 109);
}

DataContainer DLLResourceLoader::loadCEGUI_image( HMODULE hModule ) {
	return loadFromDLL(hModule, RT_RCDATA, 110);
}

DataContainer DLLResourceLoader::loadCEGUI_font( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_FONT", 103);
}

DataContainer DLLResourceLoader::loadCEGUI_looknfeel( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_LOOKNFEEL", 111);
}

DataContainer DLLResourceLoader::loadCEGUI_fontTTF( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_FONT", 102);
}

DataContainer DLLResourceLoader::loadHUD_spritefont( HMODULE hModule ) {
	return loadFromDLL(hModule, L"SPRITEFONT", 113);
}

HICON DLLResourceLoader::loadWindowIcon() {
	HINSTANCE dll = LoadLibrary(L"dderes.dll");
	HICON icon = (HICON) LoadImage(dll, MAKEINTRESOURCE(114), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	FreeLibrary(dll);
	return icon;
}