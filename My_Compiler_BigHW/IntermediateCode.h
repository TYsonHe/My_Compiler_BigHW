#pragma once
#include "Common.h"

// 用于生成新的标签名称
class NewLabeler {
private:
	int index;
public:
	NewLabeler();
	string newLabel();
};

// 管理中间代码的生成和输出
class IntermediateCode {
private:
	vector<Quaternary> code;
	map<string, vector<Block> >funcBlocks;
	NewLabeler nl;

	void output(ostream& out);
	void outputFile(ostream& out);
	void outputBlocks(ostream& out);
public:
	void emit(Quaternary q);
	void emit(string op, string src1, string src2, string des);
	void back_patch(list<int>nextList, int quad);
	void output();
	void output(const char* fileName);
	void divideBlocks(vector<pair<int, string> > funcEnter);
	void outputBlocks();
	void outputBlocks(const char* fileName);
	map<string, vector<Block> >* getFuncBlock();
	int nextQuad();
};