#pragma once

#include <windows.h>

namespace DDEngine
{

struct DataContainer
{

	DataContainer() {
		dataBlob = NULL;
		dataSize = 0;
	}

	DataContainer (LPVOID dataBlob, 
						DWORD dataSize)
						:
						dataBlob(dataBlob),
						dataSize(dataSize) {
	
	}

	LPVOID dataBlob;
	DWORD dataSize;
};

namespace DLLResourceLoader {

		DataContainer loadFromDLL(HMODULE hModule, LPCWSTR type, int ID);
	
		DataContainer loadCEGUI_scheme(HMODULE hModule);
		DataContainer loadCEGUI_imageset(HMODULE hModule);
		DataContainer loadCEGUI_image(HMODULE hModule);
		DataContainer loadCEGUI_font(HMODULE hModule);
		DataContainer loadCEGUI_fontTTF(HMODULE hModule);
		DataContainer loadCEGUI_looknfeel(HMODULE hModule);

		DataContainer loadHUD_spritefont(HMODULE hModule);
	
		HICON loadWindowIcon();
};
}