#include "DDEUtils.h"
#include <sstream>
#include <cctype>
#include <iomanip>
#include <algorithm>

using namespace DDEngine;

Dimension DDEUtils::getDimensionFromHandle( HWND hwnd ) {
	RECT rectangle;
	GetClientRect( hwnd, &rectangle );
	UINT width = rectangle.right - rectangle.left;
	UINT height = rectangle.bottom - rectangle.top;

	Dimension d;
	d.WIDTH = width;
	d.HEIGHT = height;

	return d;
}

bool StringUtils::toBool(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), 
		::tolower);
	std::istringstream is(str);
	bool b = false;
	is >> std::boolalpha >> b;
	return b;
}

float StringUtils::toFloat(std::string str) {
	return (float) ::atof(str.c_str());
}

int StringUtils::toInt(std::string str) {
	return ::atoi(str.c_str());
}

std::wstring StringUtils::toWstring(std::string str) {
	using namespace std;
	wstring res = wstring();
	res.assign(str.begin(), str.end());
	return res;
}

bool FileUtils::isFileLocked( std::wstring path ) {
	HANDLE fh = CreateFile(path.c_str(), GENERIC_READ
		| GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	
	if (fh == INVALID_HANDLE_VALUE) {
		return true; 
	}

	CloseHandle(fh);
	return false;
}

FILETIME FileUtils::getLastWriteTime( std::wstring path ) {
	HANDLE fh = CreateFile(path.c_str(), GENERIC_READ, 0, 
		NULL, OPEN_EXISTING, 0, NULL);
	FILETIME writeTime;

	GetFileTime(fh, NULL, NULL, &writeTime);
	CloseHandle(fh);

	return writeTime;
}

#include <comdef.h>

void DDEngine::Win32Utils::showFailMessage(HRESULT result, std::string title, std::string message)
{
	if(FAILED(result)) {
		_com_error err(result);
		std::wstring errMsg(err.ErrorMessage());

		std::wstring tit = StringUtils::toWstring(title);
		std::wstring msg = StringUtils::toWstring(message) + L"\n\nDescription:\n" + errMsg;
		MessageBox(NULL, msg.c_str(), tit.c_str(), MB_OK);
	}
}

void DDEngine::Win32Utils::showMessage(std::string title, std::string message)
{
	std::wstring tit = StringUtils::toWstring(title);
	std::wstring msg = StringUtils::toWstring(message);
	MessageBox(NULL, msg.c_str(), tit.c_str(), MB_OK);
}
