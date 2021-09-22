#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>
#include <atlimage.h>
#include <direct.h>
#include <io.h>
#include <windows.h>

#pragma comment(lib, "Gdiplus.lib")
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4996)
#pragma warning(disable : 6001)
#pragma warning(disable : 6031)
#pragma warning(disable : 6255)
#pragma warning(disable : 6385)
#pragma warning(disable : 6387)
#pragma warning(disable : 28183)

// 程序默认标签名
std::string Title = "BDO - Flysky";
// 快捷按键
const int QuickKey = VK_OEM_3;
// 窗口句柄
HWND exeWhnd = GetConsoleWindow();
HWND gameWhnd, headWhnd;
// 存储读取的文件内容
std::string Code[200][5];
int Length = 0;
// 按键按下抬起延迟、执行后等待时间
const int delayUp = 50;
const int delayWait = 500;
// 按键名、虚拟键码、OEM扫描码
const std::string KeyCode[86][3] = {
		{"Lb", "1", "0"},
		{"Rb", "2", "0"},
		{"Mb", "4", "0"},
		{"Esc", "27", "1"},
		{"1", "49", "2"},
		{"2", "50", "3"},
		{"3", "51", "4"},
		{"4", "52", "5"},
		{"5", "53", "6"},
		{"6", "54", "7"},
		{"7", "55", "8"},
		{"8", "56", "9"},
		{"9", "57", "10"},
		{"0", "48", "11"},
		{"-", "189", "12"},
		{"=", "187", "13"},
		{"Bs", "8", "14"},
		{"Tab", "9", "15"},
		{"Q", "81", "16"},
		{"W", "87", "17"},
		{"E", "68", "18"},
		{"R", "82", "19"},
		{"T", "84", "20"},
		{"Y", "89", "21"},
		{"U", "85", "22"},
		{"I", "73", "23"},
		{"O", "79", "24"},
		{"P", "80", "25"},
		{"[", "219", "26"},
		{"]", "221", "27"},
		{"Enter", "13", "28"},
		{"Ctrl", "17", "29"},
		{"A", "65", "30"},
		{"S", "83", "31"},
		{"D", "68", "32"},
		{"F", "70", "33"},
		{"G", "71", "34"},
		{"H", "72", "35"},
		{"J", "74", "36"},
		{"K", "75", "37"},
		{"L", "76", "38"},
		{";", "186", "39"},
		{"'", "222", "40"},
		{"`", "192", "41"},
		{"LShift", "160", "42"},
		{"\\", "220", "43"},
		{"Z", "90", "44"},
		{"X", "88", "45"},
		{"C", "67", "46"},
		{"V", "86", "47"},
		{"B", "66", "48"},
		{"N", "78", "49"},
		{"M", "77", "50"},
		{",", "188", "51"},
		{".", "190", "52"},
		{"/", "191", "53"},
		{"RShift", "161", "54"},
		{"PrtSc", "42", "55"},
		{"Alt", "18", "56"},
		{"Space", "32", "57"},
		{"Caps", "20", "58"},
		{"F1", "112", "59"},
		{"F2", "113", "60"},
		{"F3", "114", "61"},
		{"F4", "115", "62"},
		{"F5", "116", "63"},
		{"F6", "117", "64"},
		{"F7", "118", "65"},
		{"F8", "119", "66"},
		{"F9", "120", "67"},
		{"F10", "121", "68"},
		{"Num", "144", "69"},
		{"Scroll", "145", "70"},
		{"Home", "36", "71"},
		{"Up", "38", "72"},
		{"PgUp", "33", "73"},
		{"/-", "109", "74"},
		{"Left", "37", "75"},
		{"Center", "1", "76"},
		{"Right", "39", "77"},
		{"/+", "107", "78"},
		{"End", "35", "79"},
		{"Down", "40", "80"},
		{"PgDn", "34", "81"},
		{"Insert", "45", "82"},
		{"Delete", "46", "83"},
};

// string to int
int Getint(std::string str)
{
	return atoi(str.c_str());
}
// string to wstring
std::wstring Getwstring(std::string str)
{
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t *wszUtf8 = new wchar_t[len + 1.0];
	memset(wszUtf8, 0, len * 2.0 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	return wszUtf8;
}
// 返回str在数组中对应的值
int GetCodeNum(std::string str, int tag)
{
	int i = 0;
	while (KeyCode[i])
	{
		if (str == KeyCode[i][0])
			return Getint(KeyCode[i][tag + 1]);
		i++;
	}
	return 0;
}
// 模拟键盘
void K(std::string key, int end)
{
	std::cout << "\r\u6309\u952e  [ " << key << " ]" << std::endl;
	int e1 = GetCodeNum(key, 0);
	int e2 = GetCodeNum(key, 1);
	keybd_event(e1, e2, 0, 0);
	Sleep(delayUp);
	keybd_event(e1, e2, 2, 0);
	Sleep(end ? end : delayWait);
}
void KD(std::string key, int end)
{
	std::cout << "\r\u6309\u4e0b  [ " << key << " ]" << std::endl;
	int e1 = GetCodeNum(key, 0);
	int e2 = GetCodeNum(key, 1);
	keybd_event(e1, e2, 0, 0);
	Sleep(end ? end : delayWait);
}
void KU(std::string key, int end)
{
	std::cout << "\r\u62ac\u8d77  [ " << key << " ]" << std::endl;
	int e1 = GetCodeNum(key, 0);
	int e2 = GetCodeNum(key, 1);
	keybd_event(e1, e2, 2, 0);
	Sleep(end ? end : delayWait);
}
// 模拟键盘(组合按键)
void K2(std::string key1, std::string key2, int delay, int end)
{
	std::cout << "\r\u6309\u952e  [ " << key1 << " + " << key2 << " ]" << std::endl;
	int e11 = GetCodeNum(key1, 0);
	int e12 = GetCodeNum(key1, 1);
	int e21 = GetCodeNum(key2, 0);
	int e22 = GetCodeNum(key2, 1);
	keybd_event(e11, e12, 0, 0);
	Sleep(20);
	keybd_event(e21, e22, 0, 0);
	Sleep(delay ? delay : delayUp);
	keybd_event(e21, e22, 2, 0);
	Sleep(20);
	keybd_event(e11, e12, 2, 0);
	Sleep(end ? end : delayWait);
}
// 移动鼠标指针
void MoveMouse(int x, int y)
{
	SetCursorPos(x, y);
	Sleep(delayWait);
}
// 模拟鼠标左击
void L(int delay, int end)
{
	std::cout << "\r\u5de6\u51fb" << std::endl;
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(delay ? delay : delayUp);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
void LD(int end)
{
	std::cout << "\r\u6309\u4e0b" << std::endl;
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
void LU(int end)
{
	std::cout << "\r\u62ac\u8d77" << std::endl;
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
void L(int x, int y, int end)
{
	std::cout << "\r\u5de6\u51fb  [ " << x << "," << y << " ]" << std::endl;
	MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(delayUp);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
// 模拟鼠标右击
void R(int delay, int end)
{
	std::cout << "\r\u53f3\u51fb" << std::endl;
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	Sleep(delay ? delay : delayUp);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
void RD(int end)
{
	std::cout << "\r\u6309\u4e0b" << std::endl;
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
void RU(int end)
{
	std::cout << "\r\u62ac\u8d77" << std::endl;
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
void R(int x, int y, int end)
{
	std::cout << "\r\u53f3\u51fb  [ " << x << "," << y << " ]" << std::endl;
	MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	Sleep(delayUp);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
// 模拟扔垃圾
void D(int x1, int y1, int x2, int y2, int end)
{
	std::cout << "\r\u4e22\u5f03  [ " << x1 << "," << y1 << " " << x2 << "," << y2 << " ]" << std::endl;
	MoveMouse(x1, y1);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(20);
	MoveMouse(x2, y2);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(delayUp);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(20);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end ? end : delayWait);
}
// 模拟鼠标滚轮
void W(int x, int y, int z, int end)
{
	std::cout << "\r\u6eda\u8f6e  [ " << x << "," << y << " " << z << " ]" << std::endl;
	MoveMouse(x, y);
	while (z)
	{
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -WHEEL_DELTA, 0);
		z--;
		if (z)
			Sleep(50 + rand() % 101); //[50,150]
	}
	Sleep(end ? end : delayWait);
}
// 模拟鼠标移动
void M(int x, int end)
{
	std::cout << "\r\u79fb\u52a8  [ " << x << " ]" << std::endl;
	if (x == 0)
		return;
	POINT p;
	GetCursorPos(&p);
	int ox = p.x, oy = p.y;
	// 左右单次移动量(正数：右，负数：左)
	int offsetX = x / abs(x) * 20;
	while (abs(x) >= abs(offsetX))
	{
		ox += offsetX;
		SetCursorPos(ox, oy);
		x -= offsetX;
		Sleep(5);
	}
	SetCursorPos(ox + x, oy);
	Sleep(end ? end : delayWait);
}
// 等待时间
void S(int end)
{
	Sleep(end ? end : delayWait);
}
// 热键运行
void HKD(std::string str)
{
	std::cout << "\r\u70ed\u952e  [ " << str << " ]" << std::endl;
	int hotkey = GetCodeNum(str, 0);
	while (true)
	{
		if (GetAsyncKeyState(hotkey) & 0x8000)
			break;
		Sleep(1);
	}
}
void HKU(std::string str)
{
	std::cout << "\r\u70ed\u952e  [ " << str << " ]" << std::endl;
	int hotkey = GetCodeNum(str, 0);
	while (true)
	{
		if (GetAsyncKeyState(hotkey) & 0x8000)
		{
			while (GetAsyncKeyState(hotkey) & 0x8000)
				Sleep(1);
			break;
		}
		Sleep(1);
	}
}
// 热键停止
void Kill(std::string str)
{
	int hotkey = GetCodeNum(str, 0);
	while (true)
	{
		if (GetAsyncKeyState(hotkey) & 0x8000)
			exit(0);
		Sleep(1);
	}
}
// 发送字符串
void SendAscii(wchar_t data, BOOL shift)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));
	if (shift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		SendInput(1, input, sizeof(INPUT));
	}
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = data;
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(2, input, sizeof(INPUT));
	if (shift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		input[0].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, input, sizeof(INPUT));
	}
}
void SendUnicode(wchar_t data)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0;
	input[0].ki.wScan = data;
	input[0].ki.dwFlags = 0x4; //KEYEVENTF_UNICODE;
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0;
	input[1].ki.wScan = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4; //KEYEVENTF_UNICODE;
	SendInput(2, input, sizeof(INPUT));
}
void SendKeys(CString msg)
{
	short vk;
	BOOL shift;
	USES_CONVERSION;
	wchar_t *data = T2W(msg.GetBuffer(0));
	int len = wcslen(data);
	for (int i = 0; i < len; i++)
	{
		if (data[i] >= 0 && data[i] < 256) //ascii字符
		{
			vk = VkKeyScanW(data[i]);
			if (vk == -1)
				SendUnicode(data[i]);
			else
			{
				if (vk < 0)
					vk = ~vk + 0x1;
				shift = vk >> 8 & 0x1;
				if (GetKeyState(VK_CAPITAL) & 0x1)
					if (data[i] >= 'a' && data[i] <= 'z' || data[i] >= 'A' && data[i] <= 'Z')
						shift = !shift;
				SendAscii(vk & 0xFF, shift);
			}
		}
		else //unicode字符
			SendUnicode(data[i]);
	}
}
void T(std::string str, int end)
{
	str = str.substr(str.find('"') + 1, str.find_last_of('"') - 1);
	std::cout << "\r\u6587\u672c  [ " << str << " ]" << std::endl;
	SendKeys(str.c_str());
	Sleep(end ? end : delayWait);
}
// 截图保存
void SC(std::string str, int end)
{
	std::cout << "\r\u622a\u56fe" << std::endl;
	std::string path = str.substr(str.find('"') + 1, str.find_last_of('"') - 1) + '/';
	replace(path.begin(), path.end(), '\\', '/');
	// 创建文件夹
	char tmpDirPath[256] = {0};
	for (int i = 0; i < path.length(); i++)
	{
		tmpDirPath[i] = path[i];
		if (tmpDirPath[i] == '/')
			if (_access(tmpDirPath, 0) == -1)
				_mkdir(tmpDirPath);
	}
	// 基于时间的截图名
	time_t now = time(0);
	tm *ltm = localtime(&now);
	path = path + std::to_string(1900 + ltm->tm_year) + '-' + std::to_string(1 + ltm->tm_mon) + '-' + std::to_string(ltm->tm_mday) + '_' + std::to_string(ltm->tm_hour) + '-' + std::to_string(ltm->tm_min) + '-' + std::to_string(ltm->tm_sec) + ".png";
	// 截图保存
	HDC hDCScreen = GetDC(NULL);
	int nWidthScreen = GetDeviceCaps(hDCScreen, HORZRES);
	int nHeightScreen = GetDeviceCaps(hDCScreen, VERTRES);
	int nBitPerPixel = GetDeviceCaps(hDCScreen, BITSPIXEL);
	CImage m_MyImage;
	m_MyImage.Create(nWidthScreen, nHeightScreen, nBitPerPixel);
	HDC hDCImg = m_MyImage.GetDC();
	BitBlt(hDCImg, 0, 0, nWidthScreen, nHeightScreen, hDCScreen, 0, 0, SRCCOPY);
	m_MyImage.Save((char *)path.c_str(), Gdiplus::ImageFormatPNG);
	m_MyImage.ReleaseDC();
	Sleep(end ? end : delayWait);
}
// 打开游戏
void OPEN(int end)
{
	while (IsWindowVisible(gameWhnd))
		Sleep(5000);
	headWhnd = GetForegroundWindow();
	std::cout << "\r\u6253\u5f00" << std::endl;
	SetForegroundWindow(gameWhnd);
	SetActiveWindow(gameWhnd);
	SetFocus(gameWhnd);
	ShowWindow(gameWhnd, 1);
	SetWindowPos(gameWhnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	if (end < 0)
	{
		end = -end;
		Sleep(50);
		SetWindowPos(gameWhnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	if (!IsWindowVisible(gameWhnd))
		exit(0);
	Sleep(end ? end : delayWait);
}
// 关闭游戏
void CLOSE()
{
	if (IsWindowVisible(gameWhnd))
	{
		std::cout << "\r\u5173\u95ed" << std::endl;
		SetWindowPos(gameWhnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		ShowWindow(gameWhnd, 0);
		SetForegroundWindow(headWhnd);
		SetActiveWindow(headWhnd);
		SetFocus(headWhnd);
	}
}
// 读取ANSI编码文件并储存于Code
void ReadFiletoCode(std::string file)
{
	std::string line, out;
	std::ifstream fin(file, std::ios::binary);
	if (fin)
	{
		while (getline(fin, line))
		{
			int y = 0;
			std::istringstream str(line);
			while (str >> out)
			{
				Code[Length][y] = out;
				y++;
			}
			Length++;
		}
	}
	fin.close();
}
// 输出字的颜色
void CoutColor(int tag)
{
	switch (tag)
	{
	case 0:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		break;
	case 111:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 100:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	}
}
// 倒计时
void ShowTime(int delay)
{
	while (delay != 0)
	{
		std::cout << "\r\u5012\u8ba1\u65f6: ";
		CoutColor(100);
		std::cout << std::setw(2) << std::setfill('0') << delay / 60 << "\u5206" << std::setw(2) << std::setfill('0') << delay % 60 << "\u79d2 ";
		CoutColor(111);
		Sleep(1000);
		delay--;
		if (delay <= 3 && !IsWindowVisible(gameWhnd))
		{
			HWND sqa = GetForegroundWindow();
			if (exeWhnd != sqa)
			{
				ShowWindow(exeWhnd, 1);
				SetWindowPos(exeWhnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				SetForegroundWindow(sqa);
			}
			if (delay == 0)
				ShowWindow(exeWhnd, SW_SHOWMINIMIZED);
		}
	}
}
// 设置窗口标题
void SetTitle(std::string path)
{
	std::string str1 = path.substr(path.find_last_of('\\') + 1);
	std::string str2 = str1.substr(0, str1.find_last_of('.'));
	LPCSTR title = (LPCSTR)str2.c_str();
	SetConsoleTitleA(title);
}
// 设置窗口样式
void SetStyle()
{
	SMALL_RECT rc = {0, 0, 30, 20};
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rc);
	SetWindowLongPtrA(
			exeWhnd,
			GWL_STYLE,
			GetWindowLongPtrA(exeWhnd, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
	Sleep(50);
	ShowWindow(exeWhnd, SW_SHOWMINIMIZED);
}
// str复制到剪切板
void ClipBoard(char *str)
{
	HWND hWnd = NULL;
	OpenClipboard(hWnd);
	EmptyClipboard();
	HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 1000);
	strcpy((char *)GlobalLock(hHandle), str);
	SetClipboardData(CF_TEXT, hHandle);
	GlobalUnlock(hHandle);
	CloseClipboard();
}
// 快捷键获取坐标，输出显示并复制到剪切板
void GetPosition()
{
	int hotkey = 1000;
	RegisterHotKey(NULL, hotkey, MOD_ALT, QuickKey);
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.wParam == hotkey)
		{
			POINT p;
			GetCursorPos(&p);
			std::string str = std::to_string(p.x) + "," + std::to_string(p.y);
			char *position = (char *)str.c_str();
			std::cout << position << std::endl;
			ClipBoard(position);
		}
	}
}
// 快捷键获取坐标
void GetPosition(int &x, int &y)
{
	int hotkey = 1000;
	RegisterHotKey(NULL, hotkey, MOD_ALT, QuickKey);
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.wParam == hotkey)
		{
			POINT p;
			GetCursorPos(&p);
			x = p.x;
			y = p.y;
			break;
		}
	}
}
// CImage转Gdiplus::Bitmap
Gdiplus::Bitmap *CImage2Image(CImage *pImage)
{
	Gdiplus::Bitmap *image = new Gdiplus::Bitmap(pImage->GetWidth(), pImage->GetHeight());
	Gdiplus::Rect bound(0, 0, pImage->GetWidth(), pImage->GetHeight());
	Gdiplus::BitmapData lockedBitmapData;
	int bpp = pImage->GetBPP();
	int imageRowSize = pImage->GetWidth() * (bpp / 8);
	if (bpp == 24)
		image->LockBits(&bound, Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &lockedBitmapData);
	else if (bpp == 32)
		image->LockBits(&bound, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &lockedBitmapData);
	BYTE *pSrcPointer = (BYTE *)pImage->GetBits();
	BYTE *pDstPointer = (BYTE *)lockedBitmapData.Scan0;
	for (int i = 0; i < pImage->GetHeight(); i++)
	{
		memcpy(pDstPointer, pSrcPointer, imageRowSize);
		pSrcPointer += pImage->GetPitch();
		pDstPointer += lockedBitmapData.Stride;
	}
	image->UnlockBits(&lockedBitmapData);
	return image;
}
// 全屏截取
Gdiplus::Bitmap *ScreenShot()
{
	HDC hDCScreen = GetDC(NULL);
	int nWidthScreen = GetDeviceCaps(hDCScreen, HORZRES);
	int nHeightScreen = GetDeviceCaps(hDCScreen, VERTRES);
	int nBitPerPixel = GetDeviceCaps(hDCScreen, BITSPIXEL);
	CImage m_MyImage;
	m_MyImage.Create(nWidthScreen, nHeightScreen, nBitPerPixel);
	HDC hDCImg = m_MyImage.GetDC();
	BitBlt(hDCImg, 0, 0, nWidthScreen, nHeightScreen, hDCScreen, 0, 0, SRCCOPY);
	Gdiplus::Bitmap *image = CImage2Image(&m_MyImage);
	m_MyImage.ReleaseDC();
	return image;
}
// 快捷键获取坐标RGB颜色，输出显示，复制到剪切板
void GetXyColor()
{
	int x = 0, y = 0;
	GetPosition(x, y);
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	Gdiplus::Bitmap *image = ScreenShot();
	Gdiplus::Color color1, color2;
	image->GetPixel(x, y, &color1);
	image->GetPixel(x + 3, y + 3, &color2);
	std::string colorStr;
	colorStr = "color(" + std::to_string(color1.GetRed()) + "," + std::to_string(color1.GetGreen()) + "," + std::to_string(color1.GetBlue()) + ")(";
	colorStr = colorStr + std::to_string(color2.GetRed()) + "," + std::to_string(color2.GetGreen()) + "," + std::to_string(color2.GetBlue()) + ")";
	Gdiplus::GdiplusShutdown(gdiplusToken);
	char *str = (char *)colorStr.c_str();
	std::cout << str << std::endl;
	ClipBoard(str);
}
// 判断某点颜色是否匹配
bool checkColor(Gdiplus::Bitmap *image, int array[], int i, int j)
{
	Gdiplus::Color color;
	image->GetPixel(i, j, &color);
	return array[0] == color.GetRed() &&
				 array[1] == color.GetGreen() &&
				 array[2] == color.GetBlue();
}
// 根据颜色找到匹配的第一个坐标
void getColorXY(int array[][3], int &x, int &y)
{
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	Gdiplus::Bitmap *image = ScreenShot();
	int width = image->GetWidth();
	int height = image->GetHeight();
	Gdiplus::Color color1, color2;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (checkColor(image, array[0], i, j) && checkColor(image, array[1], i + 3, j + 3))
			{
				x = i;
				y = j;
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}
		}
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
}
// 得到坐标
void GetXY(std::string &str, int &x, int &y)
{
	// x,y
	if (str == "x,y")
	{
		GetPosition(x, y);
		str = std::to_string(x) + ',' + std::to_string(y);
		Sleep(1000);
	}
	// color(255,255,255)(0,0,0)
	else if (str.substr(0, 5) == "color")
	{
		int colorArray[2][3];
		std::string strColor = str;
		for (int i = 0; i < 2; i++)
		{
			strColor = strColor.substr(strColor.find("(") + 1); //r,g,b)(r,g,b)
			colorArray[i][0] = Getint(strColor.substr(0, strColor.find(",")));
			strColor = strColor.substr(strColor.find(",") + 1); //g,b)(r,g,b)
			colorArray[i][1] = Getint(strColor.substr(0, strColor.find(",")));
			strColor = strColor.substr(strColor.find(",") + 1); //b)(r,g,b)
			colorArray[i][2] = Getint(strColor.substr(0, strColor.find(")")));
		}
		getColorXY(colorArray, x, y);
	}
	// 0,0
	else
	{
		size_t division = str.find_last_of(',');
		x = Getint(str.substr(0, division));
		y = Getint(str.substr(1 + division));
	}
}
// FOR循环
int RunKey(std::string[], int);
int RunFor(int line)
{
	int j, times = Getint(Code[line][1]);
	while (times--)
	{
		j = line + 1;
		while (Code[j][0] != "END")
			j = RunKey(Code[j], j);
	}
	return j;
}
// IF判断
int RunIf(int line)
{
	int j = line + 1;
	int x = 0, y = 0;
	GetXY(Code[line][1], x, y);
	if (x != 0 && y != 0)
		while (Code[j][0] != "FI")
			j = RunKey(Code[j], j);
	else
	{
		int k = 1; //记录FI数,含嵌套情况
		while (k != 0)
		{
			if (Code[j][0] == "IF")
				k++;
			else if (Code[j][0] == "FI")
				k--;
			j++;
		}
		j--; //定位到FI那一行
	}
	return j;
}
// 计算hash
constexpr unsigned int hash(std::string str, int off = 0)
{
	const char *s = (char *)str.c_str();
	return !s[off] ? 5381 : (hash(s, off + 1) * 33) ^ s[off];
}
// 单行脚本处理
int RunKey(std::string str[], int line)
{
	if (gameWhnd == GetForegroundWindow())
	{
		switch (hash(str[0]))
		{
		case hash("K"):
			K(str[1], Getint(str[2]));
			break;
		case hash("KD"):
			KD(str[1], Getint(str[2]));
			break;
		case hash("KU"):
			KU(str[1], Getint(str[2]));
			break;
		case hash("K2"):
			K2(str[1], str[2], Getint(str[3]), Getint(str[4]));
			break;
		case hash("L"):
		case hash("R"):
			if (str[1].find(',') != std::string::npos)
			{
				int x = 0, y = 0;
				GetXY(str[1], x, y);
				if (x != 0 || y != 0)
				{
					if (str[0] == "L")
						L(x, y, Getint(str[2]));
					if (str[0] == "R")
						R(x, y, Getint(str[2]));
				}
			}
			else
			{
				if (str[0] == "L")
					L(Getint(str[1]), Getint(str[2]));
				if (str[0] == "R")
					R(Getint(str[1]), Getint(str[2]));
			}
			break;
		case hash("LD"):
			LD(Getint(str[1]));
			break;
		case hash("LU"):
			LU(Getint(str[1]));
			break;
		case hash("RD"):
			RD(Getint(str[1]));
			break;
		case hash("RU"):
			RU(Getint(str[1]));
			break;
		case hash("D"):
		{
			int x1 = 0, y1 = 0;
			int x2 = 0, y2 = 0;
			GetXY(str[1], x1, y1);
			GetXY(str[2], x2, y2);
			if ((x1 != 0 || y1 != 0) && (x2 != 0 || y2 != 0))
				D(x1, y1, x2, y2, Getint(str[3]));
			break;
		}
		case hash("W"):
		{
			int x = 0, y = 0;
			GetXY(str[1], x, y);
			if (x != 0 || y != 0)
				W(x, y, Getint(str[2]), Getint(str[3]));
			break;
		}
		case hash("M"):
			M(Getint(str[1]), Getint(str[2]));
			break;
		case hash("T"):
			T(str[1], Getint(str[2]));
			break;
		case hash("SC"):
			SC(str[1], Getint(str[2]));
			break;
		case hash("WHILE"):
		{
			int x = 0, y = 0;
			do
			{
				GetXY(str[1], x, y);
				Sleep(200);
			} while (x == 0 && y == 0);
			break;
		}
		case hash("CLOSE"):
			CLOSE();
			break;
		}
	}
	switch (hash(str[0]))
	{
	case hash("FOR"):
		line = RunFor(line);
		break;
	case hash("IF"):
		line = RunIf(line);
		break;
	case hash("S"):
		S(Getint(str[1]));
		break;
	case hash("HKD"):
		HKD(str[1]);
		break;
	case hash("HKU"):
		HKU(str[1]);
		break;
	case hash("OPEN"):
		OPEN(Getint(str[1]));
		break;
	}
	return line + 1;
}
// 脚本整体处理
void RunCode()
{
	int startLine = 0, num = 1;
	while (Code[startLine][0] != "BDO")
		startLine++;
	if (Code[startLine][3] != "\0")
	{
		std::thread t(Kill, Code[startLine][3]);
		t.detach();
	}
	int total = Getint(Code[startLine][1]), time = Getint(Code[startLine][2]);
	while (total--)
	{
		gameWhnd = FindWindow("BlackDesertWindowClass", NULL);
		system("cls");
		CoutColor(111);
		std::cout << "\u8fd0\u884c\u6b21\u6570: ";
		CoutColor(100);
		std::cout << num++ << "/" << Getint(Code[startLine][1]);
		std::cout << "  " << Code[startLine][3] << std::endl;
		std::cout << std::endl;
		CoutColor(0);
		int i = startLine + 1;
		while (i < Length)
			i = RunKey(Code[i], i);
		CoutColor(111);
		std::cout << std::endl;
		if (total != 0)
			ShowTime(time);
	}
}
// 主函数
int main(int argc, char *argv[])
{
	switch (argc)
	{
	case 1:
		SetStyle();
		SetTitle(Title);
		ShowWindow(exeWhnd, 1);
		CoutColor(111);
		std::cout << "\u9009\u62e9\u6267\u884c\u9879\u76ee\uff1a" << std::endl;
		CoutColor(100);
		std::cout << "1\u002e\u83b7\u53d6\u9f20\u6807\u5750\u6807" << std::endl;
		std::cout << "2\u002e\u83b7\u53d6\u9f20\u6807\u5750\u6807\u5904\u989c\u8272" << std::endl;
		CoutColor(0);
		std::cout << "\u8f93\u5165\uff1a";
		CoutColor(111);
		char ch;
		ch = getchar();
		ShowWindow(exeWhnd, SW_SHOWMINIMIZED);
		system("cls");
		std::cout << "\u5feb\u6377\u952e\u0020\u005b\u0020\u0041\u004c\u0054\u0020\u002b\u0020\u0060\u0020\u005d" << std::endl
							<< std::endl;
		CoutColor(0);
		switch (ch)
		{
		case 49:
			GetPosition();
			break;
		default:
			while (true)
				GetXyColor();
			break;
		}
		break;
	case 2:
		SetStyle();
		SetTitle(argv[1]);
		ReadFiletoCode(argv[1]);
		Sleep(1000);
		RunCode();
		break;
	}
	if (argc != 2)
		system("pause");
	return 0;
}
