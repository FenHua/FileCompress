#include"HuffmanTree.h"
#include"assert.h"
typedef long long Type;
struct CharInfo
{
	unsigned char _ch;//出现的字符
	Type _count;//统计次数
	string _code;//huffman编码
	CharInfo(Type count = 0) :_ch(0), _count(count), _code("")
	{}
	//重载对应的操作符
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
		//统计字符出现的次数
		while (!feof(pf))
		{
			_infos[ch]._count++;
			ch = fgetc(pf);
		}
		//以该字符出现的次数构建一颗huffmanTree
		CharInfo invalid;//非法值
		HuffmanTree<CharInfo>ht(_infos, 256, invalid);
		//生成huffman编码
		string code;
		_CreateHuffmanCode(ht.GetRoot(), code);
		//压缩文件
		fseek(pf, 0, SEEK_SET);//回到文件头
		string compressfile = ".compress";//压缩后的文件名
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
					value |= 1; //表示value=value||1
				else
					value |= 0; //do nothing
				pos++;
				if (pos == 8)
				{
					//满一个字节
					fputc(value, fin);
					value = 0;
					pos = 0;
				}
			}
			ch = fgetc(pf);
		}
		if (pos)
		{
			//解决不足8位的情况
			value <<= (8 - pos);
			fputc(value, fin);
		}
		//配置文件--便于重建huffman树
		string configfilename = filename;
		configfilename = ".config";
		FILE*finconfig = fopen(configfilename.c_str(), "wb");
		assert(finconfig);
		string line;
		char buff[128];
		for (size_t i = 0; i < 256; i++)
		{
			//一行一行的读取
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
		//读取配置文件--获取字符出现的次数
		unsigned char ch = 0;
		while (ReadLine(foutconfig, line))
		{
			if (line.empty())
			{
				line += '\n';
				continue;
			}
			//读到空行
			ch = line[0];
			_infos[ch]._count = atoi(line.substr(2).c_str());
			line.clear();
		}
		//构建huffman树
		CharInfo invalid;
		HuffmanTree<CharInfo>hft(_infos, 256, invalid);
		//根节点的权值也是字符出现的次数总和
		HuffmanTreeNode<CharInfo>*root = hft.GetRoot();
		Type charcount = root->_weight._count;
		//解压缩
		string uncompressfilename = filename;
		index = uncompressfilename.rfind(".");
		uncompressfilename = uncompressfilename.substr(0, index);
		uncompressfilename += ".uncompress";
		FILE*fin = fopen(uncompressfilename.c_str(), "wb");
		assert(fin);
		//由压缩还原文件
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
					//叶子节点
					fputc(cur->_weight._ch, fin);
					cur = root;
					--charcount;
					if (charcount == 0)
						break;//所有的字符都处理完成
				}
				if (ch&(1 << pos))
				{
					//检查字符的每个位
					cur = cur->_right;//1向右走
				}
				else
				{
					cur = cur->_left;//0向左走
				}
				--pos;
				if (pos < 0)
				{
					//一个字解压完成
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
	//读取一行字符并且放在line中
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
	//递归求huffmanTreeCode
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
			//叶子节点
			_infos[root->_weight._ch]._code = code;
		}
	}
	//非递归求huffmanTreeNode
	void _CreateHuffmanCode(HuffmanTreeNode<CharInfo>*root)
	{
		if (root == NULL)
			return;
		_CreateHuffmanCode(root->_left);
		_CreateHuffmanCode(root->_right);
		if (root->_left == NULL&&root->_right == NULL)
		{
			//叶子结点
			string&code = _infos[root->_weight._ch]._code;
			HuffmanTreeNode<CharInfo>*cur = root;
			HuffmanTreeNode<CharInfo>*parent = root->_parent;
			while (parent)
			{
				if (parent->_left == cur)
					code.push_back('0');//左0
				else
					code.push_back('1');//右1
				cur = parent;
				parent = cur->_parent;
			}
			//编码是从根到叶子节点的，需要逆置
			reverse(code.begin(), code.end());
		}
	}
};