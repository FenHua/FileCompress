// FileCompress.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include"FileCompress.h"
#include"iostream"
#include"windows.h"
using namespace std;

void testFileCompress()
{
	FileCompress fc;
	cout << "��ʼѹ��" << endl;
	cout << "ѹ����ʱ��";
	int start = GetTickCount();
	fc.Compress("1.png");
	int end = GetTickCount();
	cout << end - start << endl;
	cout << "��ʼ��ѹ" << endl;
	cout << "��ѹ��ʱ��" << endl;
	start = GetTickCount();
	fc.UnCompress("1.png.compress");//ѹ���ļ�
	end = GetTickCount();
	cout << end - start << endl;
}
void testFileUncompress()
{
	FileCompress fc;
	cout << "��ʼ��ѹ" << endl;
	cout << "��ѹ��ʱ��";
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