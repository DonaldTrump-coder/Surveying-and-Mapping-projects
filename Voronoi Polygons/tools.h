#pragma once
#include <string>

extern CString input_text;
std::string get_open_file();
std::string get_save_file();
std::string open_file();
std::string* textsplit(std::string& text, char n);
int get_split_num(std::string& text, char n);
double get_distance(double x1, double y1, double x2, double y2);
void save_to_file(std::string result);