#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sys/stat.h> 

using namespace std;
typedef unsigned char BYTE;
struct MODIFY_DATA
{
	unsigned int finder;	//¶¨Î»×ÔÉíµÄ³£Á¿
	_off_t my_length;		//ÎÄ¼þ³¤¶È
} modify_data = { 0x12345678, 0 }; 

class CBindFile
{
public:
	CBindFile();
	~CBindFile();
	bool Bind_Files();
	void Unbind();
	char* firstName;
	char* secondName;
	char* finalName;
	//char* firstName = "calc.exe";	//ÎÄ¼þAÂ·¾¶
	//char* secondName = "ghost32.exe";	//ÎÄ¼þBÂ·¾¶
	//char* finalName = "final.exe";	//ºÏ³ÉÎÄ¼þÂ·¾¶
private:

};

CBindFile::CBindFile()
{
	this->firstName = "E://test//notepad.exe"; 
	this->secondName = "E://test//calc.exe";
	this->finalName = "E://test//final.exe";
}

CBindFile::~CBindFile()
{
}
bool CBindFile::Bind_Files()
{
	FILE* firstFile;	//×ÔÉíÎÄ¼þ
	FILE* out;		//×îÖÕºÏ³ÉÎÄ¼þ
	FILE* in;		//´ý°ó¶¨ÎÄ¼þ
	int bytesIn;	//Òª¶ÁÈ¡µÄbytes
	int totalBytes = 0;	//×Übytes
	BYTE* buf;
	struct _stat ST; //ÎÄ¼þ×´Ì¬

	unsigned int finder = 0x12345678;	//¶¨ÓÚÆðÊ¼µØÖ·
	unsigned int i, k;

	_stat(firstName, &ST);		//¶ÁÈ¡ÎÄ¼þAÊôÐÔ
	modify_data.my_length = ST.st_size;	
	if (modify_data.my_length == 0)
	{
		cout << "°ó¶¨ÎÄ¼þÖÐ£¬notepad.exe³¤¶ÈÎªÁãÊ±³ö´í!" << endl;
		return false;
	}

	buf = (BYTE *)malloc(modify_data.my_length);	//ÉêÇëÎÄ¼þAµÄ´æ´¢¿Õ¼ä
	if (buf == NULL)
	{
		cout << "°ó¶¨ÎÄ¼þÖÐ£¬·ÖÅänotepad.exe³¤¶ÈÎªÁãÊ±³ö´í!" << endl;
		return false;
	}

	firstFile = fopen(firstName, "rb");	//¶ÁÈ¡ÎÄ¼þAµÄÄÚÈÝ
	if (firstFile == NULL)
	{
		free(buf);
		cout << "°ó¶¨ÎÄ¼þÖÐ£¬´ò¿ªnotepad.exeÊ±³ö´í!" << endl;
		return false;
	}

	bytesIn = fread(buf, 1, modify_data.my_length, firstFile);		//Ð´Èë´æ´¢¿Õ¼ä
	fclose(firstFile);		//¹Ø±Õ¶ÁÐ´Á÷

	if (bytesIn != modify_data.my_length)
	{
		free(buf);
		cout << "°ó¶¨ÎÄ¼þÖÐ£¬²»ÄÜÍêÈ«¶ÁÈ¡notepad.exeÄÚÈÝÊ±³ö´í!" << endl;
		return false;
	}
	for (i = 0; i < (modify_data.my_length) - sizeof(finder); i += sizeof(finder))//Õâ¸öµØ·½ÎÒÒ²¿´²»¶®
	{
		for (k = 0; k < sizeof(finder); k++)
		{
			if (buf[i + k] != ((BYTE*)&finder)[k])
				break;
		}
		if (k == sizeof(finder))
		{
			memcpy(buf + i, &modify_data, sizeof(modify_data));
			break;
		}
	}
	
	if (i > modify_data.my_length - sizeof(finder))
	{
		cout << i << endl;
		cout << modify_data.my_length - sizeof(finder);
		free(buf);
		cout << "°ó¶¨ÎÄ¼þÖÐ£¬²»ÄÜ¶¨Î»notepad.exeÊ±³ö´í!" << endl;
		return false;
	}

	if (_stat(secondName, &ST) != 0 || ST.st_size == 0)
	{
		free(buf);
		cout << "°ó¶¨ÎÄ¼þÖÐ£¬¶ÁÈ¡calc.exeÊ±³ö´í!" << endl;
		return false;
	}

	out = fopen(finalName, "wb");	//´´½¨×îÖÕºÏ³ÉÎÄ¼þ
	if (out == NULL)
	{
		free(buf);
		cout << "°ó¶¨ÎÄ¼þÖÐ£¬´´½¨°ó¶¨ºóºÏ³ÉÎÄ¼þÊ±³ö´í!" << endl;
		return false;
	}

	totalBytes += fwrite(buf, 1, bytesIn, out);	

	in = fopen(secondName, "rb");	//´ò¿ªÎÄ¼þB
	if (out == NULL)
	{
		free(buf);
		cout << "°ó¶¨ÎÄ¼þÖÐ£¬´ò¿ªcalc.exeÊ±³ö´í!" << endl;
		return false;
	}

	totalBytes += fwrite(&ST.st_size, 1, sizeof(ST.st_size), out); //Ð´ÈëBÎÄ¼þµÄ³¤¶Èµ½ºÏ³ÉÎÄ¼þÖÐ

	while (bytesIn=fread(buf,1,modify_data.my_length,in))
	{
		totalBytes += fwrite(buf, 1, bytesIn, out);
	}
	fclose(in);
	fclose(out);
	free(buf);

	return true;

}
void CBindFile::Unbind(){
	FILE* finalFile;	//×ÔÉíÎÄ¼þ
	FILE* out;		//·Ö½âºóÎÄ¼þ
	BYTE* buf;
	int bytesIn;
	int totalBytes = 0;
	int prog_length;
	struct _stat ST; //ÎÄ¼þ×´Ì¬
	char temp_exe1[] = "E://test//temp.exe";

	_stat(firstName, &ST);
	modify_data.my_length = ST.st_size;
	buf = (BYTE*)malloc(modify_data.my_length);

	finalFile = fopen(finalName, "rb");
	if (finalFile == NULL)
	{
		free(buf);
		cout << "·ÖÀëÎÄ¼þÖÐ£¬´ò¿ªfinal.exeÊ±³ö´í!" << endl;
		return;
	}

	out = fopen(temp_exe1, "wb");
	if (out == NULL)
	{
		free(buf);
		cout << "·ÖÀëÎÄ¼þÖÐ£¬´´½¨±»°ó¶¨ÎÄ¼þÊ±³ö´í!" << endl;
		return;
	}
	fseek(finalFile, modify_data.my_length, SEEK_SET);
	_stat(secondName, &ST);
	cout<<prog_length<<endl;
	cout << ST.st_size<<endl;
	if (fread(&prog_length, sizeof(ST.st_size), 1, finalFile) == 0)
	{
		free(buf);
		cout << "·ÖÀëÎÄ¼þÖÐ£¬¶ÁÈ¡±»°ó¶¨ÎÄ¼þ³¤¶ÈÊ±³ö´í" << endl;
		return;
	}

	while (bytesIn = fread(buf, 1, sizeof(buf), finalFile))
	{
		if (totalBytes + bytesIn > prog_length)
			bytesIn = prog_length - totalBytes;
		totalBytes += fwrite(buf, 1, bytesIn, out);
	}
	fclose(out);
	fclose(finalFile);
	free(buf);
	cout << "·Ö½â³É¹¦" << endl;
}
int main(){
	int i;
	CBindFile *b = new CBindFile();
	/*if (b->Bind_Files())
	{
		cout << "ºÏ³É³É¹¦" << endl;
	}
	*/ 
	b->Unbind();
	//cout << "·Ö½â³É¹¦" << endl;
	cin >> i;
	return 0;
}
