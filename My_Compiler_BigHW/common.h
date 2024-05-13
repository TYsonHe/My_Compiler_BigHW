#ifndef COMMON_H
#define COMMON_H
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <cstdlib>
#include <cassert>
#include <functional>


// ������ɫ���
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

using namespace std;


typedef enum //ö�����ͣ������������
{
	/* ������־�ʹ����־ */
	ENDFILE, ERROR,
	/* ������ */
	IF, ELSE, INT, RETURN, VOID, WHILE,
	/* ������������ */
	ID, NUM,
	/* �����ַ� */
	/*  {       }    >=   <=   !=   ==    =    <   >    +      -     *     /     (        )     ;     ,   */
	LBRACE, RBRACE, GTE, LTE, NEQ, EQ, ASSIGN, LT, GT, PLUS, MINUS, MULT, DIV, LPAREN, RPAREN, SEMI, COMMA,
	/* ��ע��  ��ע�� */
	LCOMMENT, PCOMMENT,
	/*���з�*/
	NEXTLINE
} LexicalType;

typedef pair<LexicalType, string> Token;

struct Quaternary {
	string op;//������
	string src1;//Դ������1
	string src2;//Դ������2
	string des;//Ŀ�Ĳ�����
};

struct Block {
	string name;//�����������
	vector<Quaternary> codes;//�������е���Ԫʽ
	int next1;//���������һ���ӿ�
	int next2;//���������һ���ӿ�
};

void outputError(string err);
#endif // !COMMON_H

