#pragma once

#include <windows.h>

namespace DDEngine
{
struct DLLDataContainer
{

	DLLDataContainer() {
		dataBlob = NULL;
		dataSize = 0;
	}

	DLLDataContainer (LPVOID dataBlob, 
						DWORD dataSize)
						:
						dataBlob(dataBlob),
						dataSize(dataSize) {
	
	}

	LPVOID dataBlob;
	DWORD dataSize;
};

class DLLResourceLoader {

	private:
		
		DLLResourceLoader() { }

		static DLLDataContainer loadFromDLL(HMODULE hModule, LPCWSTR type, int ID);

	public:
		
		~DLLResourceLoader() { }
	
		static DLLDataContainer loadCEGUI_scheme(HMODULE hModule);
		static DLLDataContainer loadCEGUI_imageset(HMODULE hModule);
		static DLLDataContainer loadCEGUI_image(HMODULE hModule);
		static DLLDataContainer loadCEGUI_font(HMODULE hModule);
		static DLLDataContainer loadCEGUI_fontTTF(HMODULE hModule);
		static DLLDataContainer loadCEGUI_looknfeel(HMODULE hModule);

		static DLLDataContainer loadHUD_spritefont(HMODULE hModule);

	protected:
	
};
}