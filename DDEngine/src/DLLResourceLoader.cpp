#include "DLLResourceLoader.h"

using namespace DDEngine;

DLLDataContainer DLLResourceLoader::loadFromDLL( HMODULE hModule, LPCWSTR type, int ID ) {
	HRSRC hrsrc = FindResourceEx(hModule, type, MAKEINTRESOURCE(ID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
	HGLOBAL hGlobal = LoadResource(hModule, hrsrc);
	
	LPVOID dataBlobScheme = LockResource(hGlobal);
	DWORD dataSizeScheme = SizeofResource(hModule, hrsrc);

	return DLLDataContainer(dataBlobScheme, dataSizeScheme);
}

DLLDataContainer DLLResourceLoader::loadCEGUI_scheme( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_SCHEME", 112);
}

DLLDataContainer DLLResourceLoader::loadCEGUI_imageset( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_IMAGESET", 109);
}

DLLDataContainer DLLResourceLoader::loadCEGUI_image( HMODULE hModule ) {
	return loadFromDLL(hModule, RT_RCDATA, 110);
}

DLLDataContainer DLLResourceLoader::loadCEGUI_font( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_FONT", 103);
}

DLLDataContainer DLLResourceLoader::loadCEGUI_looknfeel( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_LOOKNFEEL", 111);
}

DLLDataContainer DLLResourceLoader::loadCEGUI_fontTTF( HMODULE hModule ) {
	return loadFromDLL(hModule, L"GUI_FONT", 102);
}

DLLDataContainer DLLResourceLoader::loadHUD_spritefont( HMODULE hModule ) {
	return loadFromDLL(hModule, L"SPRITEFONT", 113);
}
