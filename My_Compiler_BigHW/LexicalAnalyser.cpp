#include "LexicalAnalyser.h"

string token_to_string(Token t) {
	const char* LexicalTypeStr[] = {
	"ENDFILE", "ERROR",
	"IF", "ELSE", "INT", "RETURN", "VOID", "WHILE",
	"ID", "NUM",
	"LBRACE", "RBRACE", "GTE", "LTE", "NEQ", "EQ", "ASSIGN", "LT", "GT", "PLUS", "MINUS", "MULT", "DIV", "LPAREN", "RPAREN", "SEMI", "COMMA",
	"LCOMMENT", "PCOMMENT"
	};

	string res;
	res += LexicalTypeStr[t.first];
	res += "  " + t.second;
	return res;
}

LexicalAnalyser::LexicalAnalyser(const char* path) {
	lineCount = 0;
	openFile(path);
}

LexicalAnalyser::~LexicalAnalyser() {
	if (src.is_open()) {
		src.close();
	}
}

void LexicalAnalyser::openFile(const char* path) {
	src.open(path, ios::in);
	if (!src.is_open()) {
		cerr << "file " << path << " open error" << endl;
		exit(-1);
	}
}

//��ȡ��һ���ַ������Կհ׷�
char LexicalAnalyser::getNextChar() {
	char c;
	while (src >> c) {
		if (c == ' ' || c == '\t') {
			continue;
		}
		else if (c == '\n') {
			lineCount++;
			return '\n';
		}
		break;
	}
	if (src.eof())
		return 0;
	else
		return c;
}

Token LexicalAnalyser::getNextToken() {
	char c = getNextChar();
	switch (c) {
	case '\n':
		return Token(NEXTLINE, "\n");
	case '(':
		return Token(LPAREN, "(");
		break;
	case ')':
		return Token(RPAREN, ")");
		break;
	case '{':
		return Token(LBRACE, "{");
		break;
	case '}':
		return Token(RBRACE, "}");
		break;
	case '#':
		return Token(ENDFILE, "#");
		break;
	case '+':
		return Token(PLUS, "+");
		break;
	case '-':
		return Token(MINUS, "-");
		break;
	case '*':
		return Token(MULT, "*");
		break;
	case ',':
		return Token(COMMA, ",");
		break;
	case ';':
		return Token(SEMI, ";");
		break;
	case '=':
		if (src.peek() == '=') {
			src.get();
			return Token(EQ, "==");
		}
		else {
			return Token(ASSIGN, "=");
		}
		break;
	case '>':
		if (src.peek() == '=') {
			src.get();
			return Token(GTE, ">=");
		}
		else {
			return Token(GT, ">");
		}
		break;
	case '<':
		if (src.peek() == '=') {
			src.get();
			return Token(LTE, "<=");
		}
		else {
			return Token(LT, "<");
		}
		break;
	case '!':
		if (src.peek() == '=') {
			src.get();
			return Token(NEQ, "!=");
		}
		else {
			return Token(ERROR, string("�ʷ�������") + to_string(lineCount) + string("�У�δʶ��ķ���!"));
		}
		break;
	case '/':
		//��ע��
		if (src.peek() == '/') {
			char buf[1024];
			src.getline(buf, 1024);
			return Token(LCOMMENT, string("/") + buf);
		}
		//��ע��
		else if (src.peek() == '*') {
			src.get();
			string buf = "/*";
			while (src >> c) {
				buf += c;
				if (c == '*') {
					src >> c;
					buf += c;
					if (c == '/') {
						return Token(PCOMMENT, buf);
						break;
					}
				}
			}
			//�������û�ҵ�*/��������whileѭ�������˳�
			if (src.eof()) {
				return Token(ERROR, string("�ʷ�������") + to_string(lineCount) + string("�У���ע��û��ƥ���*/"));
			}
		}
		//����
		else {
			return Token(DIV, "/");
		}
		break;
	default:
		if (isdigit(c)) {
			string buf;
			buf.push_back(c);
			while (c = src.peek()) {
				if (isdigit(c)) {
					src >> c;
					buf += c;
				}
				else {
					break;
				}
			}
			return Token(NUM, buf);
		}
		else if (isalpha(c)) {
			string buf;
			buf.push_back(c);
			while (c = src.peek()) {
				if (isdigit(c) || isalpha(c)) {
					src >> c;
					buf += c;
				}
				else {
					break;
				}
			}
			if (buf == "int") {
				return Token(INT, "int");
			}
			else if (buf == "void") {
				return Token(VOID, "void");
			}
			else if (buf == "if") {
				return Token(IF, "if");
			}
			else if (buf == "else") {
				return Token(ELSE, "else");
			}
			else if (buf == "while") {
				return Token(WHILE, "while");
			}
			else if (buf == "return") {
				return Token(RETURN, "return");
			}
			else {
				return Token(ID, buf);
			}
		}
		else {
			return Token(ERROR, string("�ʷ�������") + to_string(lineCount) + string("�У�δʶ��ķ���") + c);
		}
	}
	return Token(ERROR, "UNKOWN ERROR");
}

void LexicalAnalyser::analyse() {
	while (1) {
		Token t = getNextToken();
		result.push_back(t);
		if (t.first == ERROR) {
			outputError(t.second);
		}
		else if (t.first == ENDFILE) {
			break;
		}
	}
}

void LexicalAnalyser::outputToStream(ostream& out) {
	if (result.back().first == ERROR) {
		out << token_to_string(result.back()) << endl;
	}
	else {
		list<Token>::iterator iter;
		for (iter = result.begin(); iter != result.end(); iter++) {
			out << token_to_string(*iter) << endl;
		}
	}
}

void LexicalAnalyser::outputToScreen() {
	outputToStream(cout);
}

void LexicalAnalyser::outputToFile(const char* fileName) {
	ofstream fout;
	fout.open(fileName, ios::out);
	if (!fout.is_open()) {
		cerr << "file " << fileName << " open error" << endl;
		return;
	}
	outputToStream(fout);
	fout.close();
}

list<Token>& LexicalAnalyser::getResult() {
	return result;
}