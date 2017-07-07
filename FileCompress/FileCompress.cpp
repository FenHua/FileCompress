// FileCompress.cpp : 定义控制台应用程序的入口点。
//
#include"FileCompress.h"
#include"iostream"
#include"windows.h"
using namespace std;

void testFileCompress()
{
	FileCompress fc;
	cout << "开始压缩" << endl;
	cout << "压缩用时：";
	int start = GetTickCount();
	fc.Compress("1.png");
	int end = GetTickCount();
	cout << end - start << endl;
	cout << "开始解压" << endl;
	cout << "解压用时：" << endl;
	start = GetTickCount();
	fc.UnCompress("1.png.compress");//压缩文件
	end = GetTickCount();
	cout << end - start << endl;
}
void testFileUncompress()
{
	FileCompress fc;
	cout << "开始解压" << endl;
	cout << "解压用时：";
	int start = GetTickCount();
	fc.UnCompress("1,png");
	int end = GetTickCount();
	cout << end - start << endl;
}
int main()
{
	testFileCompress();
	testFileUncompress();
	system("pause");
	return 0;
}