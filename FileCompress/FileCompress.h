#include"HuffmanTree.h"
#include"assert.h"
typedef long long Type;
struct CharInfo
{
	unsigned char _ch;//���ֵ��ַ�
	Type _count;//ͳ�ƴ���
	string _code;//huffman����
	CharInfo(Type count = 0) :_ch(0), _count(count), _code("")
	{}
	//���ض�Ӧ�Ĳ�����
	CharInfo operator +(const CharInfo&fc)const
	{
		return CharInfo(_count + fc._count);
	}
	bool operator!=(const CharInfo fc)const
	{
		return _count != fc._count;
	}
	bool operator<(const CharInfo&fc)const
	{
		return _count < fc._count;
	}
};
class FileCompress
{
protected:
	CharInfo _infos[256];
public:
	FileCompress()
	{
		for (size_t i = 0; i < 256; i++)
		{
			_infos[i]._ch = i;
		}
	}
	string Compress(const char*filename)
	{
		assert(filename);
		FILE*pf = fopen(filename, "rb");
		assert(pf);
		unsigned char ch = fgetc(pf);
		//ͳ���ַ����ֵĴ���
		while (!feof(pf))
		{
			_infos[ch]._count++;
			ch = fgetc(pf);
		}
		//�Ը��ַ����ֵĴ�������һ��huffmanTree
		CharInfo invalid;//�Ƿ�ֵ
		HuffmanTree<CharInfo>ht(_infos, 256, invalid);
		//����huffman����
		string code;
		_CreateHuffmanCode(ht.GetRoot(), code);
		//ѹ���ļ�
		fseek(pf, 0, SEEK_SET);//�ص��ļ�ͷ
		string compressfile = ".compress";//ѹ������ļ���
		FILE*fin = fopen(compressfile.c_str(), "wb");
		assert(fin);
		size_t pos = 0;
		unsigned char value = 0;
		ch = fgetc(pf);
		while (!feof(pf))
		{
			string &code = _infos[ch]._code;
			for (size_t i = 0; i < code.size(); i++)
			{
				value <<= 1;
				if (code[i] == '1')
					value |= 1; //��ʾvalue=value||1
				else
					value |= 0; //do nothing
				pos++;
				if (pos == 8)
				{
					//��һ���ֽ�
					fputc(value, fin);
					value = 0;
					pos = 0;
				}
			}
			ch = fgetc(pf);
		}
		if (pos)
		{
			//�������8λ�����
			value <<= (8 - pos);
			fputc(value, fin);
		}
		//�����ļ�--�����ؽ�huffman��
		string configfilename = filename;
		configfilename = ".config";
		FILE*finconfig = fopen(configfilename.c_str(), "wb");
		assert(finconfig);
		string line;
		char buff[128];
		for (size_t i = 0; i < 256; i++)
		{
			//һ��һ�еĶ�ȡ
			if (_infos[i]._count)
			{
				line += _infos[i]._ch;
				line += ",";
				line += _itoa(_infos[i]._count, buff, 10);
				line += "\n";
				fwrite(line.c_str(), 1, line.size(), finconfig);
				line.clear();
			}
		}
		fclose(pf);
		fclose(fin);
		fclose(finconfig);
		return compressfile;
	}
	string UnCompress(const char*filename)
	{
		assert(filename);
		string configfilename = filename;
		size_t index = configfilename.rfind(".");
		configfilename = configfilename.substr(0, index);
		configfilename += ".config";
		FILE*foutconfig = fopen(configfilename.c_str(), "rb");
		assert(foutconfig);
		string line;
		//��ȡ�����ļ�--��ȡ�ַ����ֵĴ���
		unsigned char ch = 0;
		while (ReadLine(foutconfig, line))
		{
			if (line.empty())
			{
				line += '\n';
				continue;
			}
			//��������
			ch = line[0];
			_infos[ch]._count = atoi(line.substr(2).c_str());
			line.clear();
		}
		//����huffman��
		CharInfo invalid;
		HuffmanTree<CharInfo>hft(_infos, 256, invalid);
		//���ڵ��ȨֵҲ���ַ����ֵĴ����ܺ�
		HuffmanTreeNode<CharInfo>*root = hft.GetRoot();
		Type charcount = root->_weight._count;
		//��ѹ��
		string uncompressfilename = filename;
		index = uncompressfilename.rfind(".");
		uncompressfilename = uncompressfilename.substr(0, index);
		uncompressfilename += ".uncompress";
		FILE*fin = fopen(uncompressfilename.c_str(), "wb");
		assert(fin);
		//��ѹ����ԭ�ļ�
		string compressfilename = filename;
		FILE*fout = fopen(compressfilename.c_str(), "rb");
		assert(fout);
		HuffmanTreeNode<CharInfo>*cur = root;
		int pos = 7;
		ch = fgetc(fout);
		while (charcount > 0)
		{
			while (cur)
			{
				if (cur->_left == NULL&&cur->_right == NULL)
				{
					//Ҷ�ӽڵ�
					fputc(cur->_weight._ch, fin);
					cur = root;
					--charcount;
					if (charcount == 0)
						break;//���е��ַ����������
				}
				if (ch&(1 << pos))
				{
					//����ַ���ÿ��λ
					cur = cur->_right;//1������
				}
				else
				{
					cur = cur->_left;//0������
				}
				--pos;
				if (pos < 0)
				{
					//һ���ֽ�ѹ���
					ch = fgetc(fout);
					pos = 7;
				}
			}
		}
		fclose(foutconfig);
		fclose(fin);
		fclose(fout);
		return uncompressfilename;
	}
	//��ȡһ���ַ����ҷ���line��
	bool ReadLine(FILE*fout, string&line)
	{
		int ch = fgetc(fout);
		if (ch == EOF)
			return false;
		while (ch!=EOF&&ch!='\n')
		{
			line += ch;
			ch = fgetc(fout);
		}
		return false;
	}
protected:
	//�ݹ���huffmanTreeCode
	void _CreateHuffmanCode(HuffmanTreeNode<CharInfo>*root, string&code)
	{
		if (root == NULL)
		{
			return;
		}
		_CreateHuffmanCode(root->_left, code + '0');
		_CreateHuffmanCode(root->_right, code + '1');
		if (root->_left == NULL&&root->_right == NULL)
		{
			//Ҷ�ӽڵ�
			_infos[root->_weight._ch]._code = code;
		}
	}
	//�ǵݹ���huffmanTreeNode
	void _CreateHuffmanCode(HuffmanTreeNode<CharInfo>*root)
	{
		if (root == NULL)
			return;
		_CreateHuffmanCode(root->_left);
		_CreateHuffmanCode(root->_right);
		if (root->_left == NULL&&root->_right == NULL)
		{
			//Ҷ�ӽ��
			string&code = _infos[root->_weight._ch]._code;
			HuffmanTreeNode<CharInfo>*cur = root;
			HuffmanTreeNode<CharInfo>*parent = root->_parent;
			while (parent)
			{
				if (parent->_left == cur)
					code.push_back('0');//��0
				else
					code.push_back('1');//��1
				cur = parent;
				parent = cur->_parent;
			}
			//�����ǴӸ���Ҷ�ӽڵ�ģ���Ҫ����
			reverse(code.begin(), code.end());
		}
	}
};