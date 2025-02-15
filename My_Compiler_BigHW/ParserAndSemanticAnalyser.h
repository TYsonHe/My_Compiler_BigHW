#pragma once
#include "Common.h"
#include "IntermediateCode.h"
#ifndef PARSER_H
#define PARSER_H
// 符号
class Symbol {
public:
	bool isVt; // 是否为终结符
	string content; // 内容
	friend bool operator ==(const Symbol& one, const Symbol& other);
	friend bool operator < (const Symbol& one, const Symbol& other);
	Symbol(const Symbol& sym);
	Symbol(const bool& isVt, const string& content);
	Symbol();
};

// 声明类型
enum DecType {
	DEC_VAR, // 变量声明
	DEC_FUN  // 函数声明
};

// 数据类型
enum DType { 
	D_VOID, // void 声明
	D_INT   // int 声明
};

/*********************下面是有关语法规则的定义*********************/
struct Var {
	string name;
	DType type;
	int level;
};

struct Func {
	string name;
	DType returnType;
	list<DType> paramTypes;
	int enterPoint;
};

class Id :public Symbol {
public:
	string name;
	Id(const Symbol& sym, const string& name);
};

class Num :public Symbol {
public:
	string number;
	Num(const Symbol& sym, const string& number);
};

class FunctionDeclare :public Symbol {
public:
	list<DType>plist;
	FunctionDeclare(const Symbol& sym);
};

class Parameter :public Symbol {
public:
	list<DType>plist;
	Parameter(const Symbol& sym);
};

class ParameterList :public Symbol {
public:
	list<DType>plist;
	ParameterList(const Symbol& sym);
};

class SentenceBlock :public Symbol {
public:
	list<int>nextList;
	SentenceBlock(const Symbol& sym);
};

class SentenceList :public Symbol {
public:
	list<int>nextList;
	SentenceList(const Symbol& sym);
};

class Sentence :public Symbol {
public:
	list<int>nextList;
	Sentence(const Symbol& sym);
};

class WhileSentence :public Symbol {
public:
	list<int>nextList;
	WhileSentence(const Symbol& sym);
};

class IfSentence :public Symbol {
public:
	list<int>nextList;
	IfSentence(const Symbol& sym);
};

class Expression :public Symbol {
public:
	string name;
	list<int>falseList;
	Expression(const Symbol& sym);
};

class M :public Symbol {
public:
	int quad;
	M(const Symbol& sym);
};

class N :public Symbol {
public:
	list<int> nextList;
	N(const Symbol& sym);
};

class AddExpression :public Symbol {
public:
	string name;
	AddExpression(const Symbol& sym);
};

class Nomial :public Symbol {
public:
	string name;
	Nomial(const Symbol& sym);
};

class Factor :public Symbol {
public:
	string name;
	Factor(const Symbol& sym);
};

class ArgumentList :public Symbol {
public:
	list<string> alist;
	ArgumentList(const Symbol& sym);
};

const Symbol symbol[] = {
	// 保留字
	{true,"int"},{true,"void"},{true,"if"},{true,"else"},{true,"while"},{true,"return"},
	// 运算符号
	{true,"+"},{true,"-"},{true,"*"},{true,"/"},{true,"="},
	{true,"=="},{true,">"},{true,"<"},{true,"!="},{true,">="},{true,"<="},
	// 界符
	{true,";"},{true,","},{true,"("},{true,")"},{true,"{"},{true,"}"},
	// ID
	{true,"ID"},
	{false,"ID"}// 非终结符的ID
};

// 产生式
struct Production {
	int id; // 产生式的标识id，方便比较
	Symbol left;
	vector<Symbol>right;
};

// 项目
struct Item {
	int pro; // 产生式id
	int pointPos; // 项目中.的位置
	friend bool operator ==(const Item& one, const Item& other);
	friend bool operator <(const Item& one, const Item& other);
};

// DFA项目集
struct I {
	set<Item> items;
};

// 定义GOTO表
typedef pair<int, Symbol> GOTO;

// 定义DFA，用于构建LR分析表
struct DFA {
	list<I> stas;
	map<GOTO, int> goTo;
};

// 定义LR分析表的行为
enum Behave { 
	reduct, // 规约
	shift,  // 移进
	accept, // 接受
	error   // 出错
};

// 定义行为和下一状态
struct Behavior {
	Behave behavior;
	int nextStat;
};

class NewTemper {
private:
	int now;
public:
	NewTemper();
	string newTemp();
};

class ParserAndSemanticAnalyser {
private:
	int lineCount;
	int nowLevel; // 当前分析行所在的语句块级次

	// 构造DFA前置
	vector<Production>productions; // 产生式集合
	map<Symbol, set<Symbol> >first; // 由产生式表构造出的first集
	map<Symbol, set<Symbol> >follow; // 由产生式表构造出的follow集
	
	// 构造LR分析表
	DFA dfa;  // 生成的DFA
	map<GOTO, Behavior> LR1_Table; // 由产生式表构造出的LR1表
	
	// 构造LR分析过程
	stack<Symbol*> symStack; // 符号栈
	stack<int> staStack; // 状态栈
	vector<Var> varTable; // 变量表
	vector<Func> funcTable; // 函数表
	IntermediateCode code; // 生成的四元式中间代码
	NewTemper nt; // 中间变量

	void readProductions(const char* fileName);
	I derive(Item item);
	void createDFA();
	void outputDFA(ostream& out);
	void printLRTable(ostream& out);
	void analyse(list<Token>& words, ostream& out);
	void outputSymbolStack(ostream& out);
	void outputStateStack(ostream& out);
	void getFirst();
	void getFollow();
	Func* lookUpFunc(string ID);
	Var* lookUpVar(string ID);
	bool match(list<string>& argument_list, list<DType>& parameter_list);
	Symbol* popSymbol();
	void pushSymbol(Symbol* sym);
public:
	ParserAndSemanticAnalyser(const char* fileName);
	void outputDFA();
	void outputDFA(const char* fileName);
	void printLRTable(const char* fileName);
	void outputIntermediateCode();
	void outputIntermediateCodeToFile(const char* fileName);
	void analyse(list<Token>& words, const char* fileName);
	void analyse(list<Token>& words);
	vector<pair<int, string> > getFuncEnter();
	IntermediateCode* getIntermediateCode();
};

#endif // !PARSER_H