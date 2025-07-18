#include "pch.h"
#include "tools.h"
#include <fstream>

CString input_text = _T("");

std::string get_open_file()
{
	std::string filepath="";
	CFileDialog filedialog(TRUE);
	if (filedialog.DoModal() == IDOK)
	{
		filepath = CT2A(filedialog.GetPathName());
	}
	return filepath;
}

std::string get_save_file()
{
	std::string filepath = "";
	CFileDialog filedialog(TRUE);
	if (filedialog.DoModal() == IDOK)
	{
		filepath = CT2A(filedialog.GetPathName());
	}
	return filepath;
}

std::string open_file()
{
	input_text = _T("");
	std::string filename = get_open_file();
	std::ifstream fin;
	fin.open(filename,std::ios::in);
	std::string result = "";
	std::string buffer;
	while (std::getline(fin, buffer))
	{
		result += buffer;
		result += "\n";
		input_text += CString(buffer.data());
		input_text += _T("\r\n");
	}
	fin.close();
	return result;
}

void save_to_file(std::string result)
{
	std::string filename;
	filename = get_save_file();
	std::ofstream fout;
	fout.open(filename, std::ios::out);
	fout << result;
	fout.close();
}

std::string* textsplit(std::string& text, char n)
{
	int charnum = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == n)
			charnum++;
	}
	std::string* result = new std::string[charnum + 1];
	std::string buffer = "";
	int index = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == n)
		{
			result[index] = buffer;
			index++;
			buffer.clear();
		}
		else
		{
			buffer += text[i];
		}
	}
	result[index] = buffer;
	return result;
}

int get_split_num(std::string& text, char n)
{
	int charnum = 0;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == n)
			charnum++;
	}
	return charnum;
}

double get_distance(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}

