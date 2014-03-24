#pragma once

#include "Dimension.h"
#include <windows.h>
#include <string>
#include <sstream>

namespace DDEngine
{

	namespace StringUtils
	{
		std::wstring toWstring(std::string str);
		bool toBool(std::string str);
		float toFloat(std::string str);
		int toInt(std::string str);

		template<typename T>
		std::string toString(const T& value)
		{
			std::ostringstream oss;
			oss << value;
			return oss.str();
		}
	}

	namespace FileUtils
	{
		bool isFileLocked(std::wstring path);
		FILETIME getLastWriteTime(std::wstring path);
	}

	namespace DDEUtils
	{
		Dimension getDimensionFromHandle(HWND hwnd);
	}

}

#define HRESULT_RETURN_CHECK(result) if(FAILED(result)) return result;
#define RELEASE(component) if(component != NULL) component->Release();
#define DDE_MSG_DIALOG(R,T,M) if(FAILED(R)) MessageBox(NULL, L#M, L#T, MB_OK);