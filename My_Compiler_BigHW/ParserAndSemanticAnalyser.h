#pragma once
#include "Common.h"
#include "IntermediateCode.h"
#ifndef PARSER_H
#define PARSER_H
// ����
class Symbol {
public:
	bool isVt; // �Ƿ�Ϊ�ս��
	string content; // ����
	friend bool operator ==(const Symbol& one, const Symbol& other);
	friend bool operator < (const Symbol& one, const Symbol& other);
	Symbol(const Symbol& sym);
	Symbol(const bool& isVt, const string& content);
	Symbol();
};

// ��������
enum DecType {
	DEC_VAR, // ��������
	DEC_FUN  // ��������
};

// ��������
enum DType { 
	D_VOID, // void ����
	D_INT   // int ����
};

/*********************�������й��﷨����Ķ���*********************/
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
	// ������
	{true,"int"},{true,"void"},{true,"if"},{true,"else"},{true,"while"},{true,"return"},
	// �������
	{true,"+"},{true,"-"},{true,"*"},{true,"/"},{true,"="},
	{true,"=="},{true,">"},{true,"<"},{true,"!="},{true,">="},{true,"<="},
	// ���
	{true,";"},{true,","},{true,"("},{true,")"},{true,"{"},{true,"}"},
	// ID
	{true,"ID"},
	{false,"ID"}// ���ս����ID
};

// ����ʽ
struct Production {
	int id; // ����ʽ�ı�ʶid������Ƚ�
	Symbol left;
	vector<Symbol>right;
};

// ��Ŀ
struct Item {
	int pro; // ����ʽid
	int pointPos; // ��Ŀ��.��λ��
	friend bool operator ==(const Item& one, const Item& other);
	friend bool operator <(const Item& one, const Item& other);
};

// DFA��Ŀ��
struct I {
	set<Item> items;
};

// ����GOTO��
typedef pair<int, Symbol> GOTO;

// ����DFA�����ڹ���LR������
struct DFA {
	list<I> stas;
	map<GOTO, int> goTo;
};

// ����LR���������Ϊ
enum Behave { 
	reduct, // ��Լ
	shift,  // �ƽ�
	accept, // ����
	error   // ����
};

// ������Ϊ����һ״̬
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
	int nowLevel; // ��ǰ���������ڵ����鼶��

	// ����DFAǰ��
	vector<Production>productions; // ����ʽ����
	map<Symbol, set<Symbol> >first; // �ɲ���ʽ�������first��
	map<Symbol, set<Symbol> >follow; // �ɲ���ʽ�������follow��
	
	// ����LR������
	DFA dfa;  // ���ɵ�DFA
	map<GOTO, Behavior> LR1_Table; // �ɲ���ʽ�������LR1��
	
	// ����LR��������
	stack<Symbol*> symStack; // ����ջ
	stack<int> staStack; // ״̬ջ
	vector<Var> varTable; // ������
	vector<Func> funcTable; // ������
	IntermediateCode code; // ���ɵ���Ԫʽ�м����
	NewTemper nt; // �м����

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