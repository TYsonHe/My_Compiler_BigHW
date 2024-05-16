
/*
* Date: 2024-05-17
* ����: 2153698-��ʫ�
*/

#include"LexicalAnalyser.h"
#include "ParserAndSemanticAnalyser.h"
#include "ObjectCodeGenerator.h"


int main() {
	LexicalAnalyser lexicalAnalyser("test.txt"); // Ҫ�������ļ�
	lexicalAnalyser.analyse();
	lexicalAnalyser.outputToScreen();
	lexicalAnalyser.outputToFile("Lexical_analyse.txt");

	ParserAndSemanticAnalyser parserAndSemanticAnalyser("productions.txt");
	parserAndSemanticAnalyser.outputDFA("ItemSets.txt"); // �����Ŀ��DFA
	parserAndSemanticAnalyser.printLRTable("LRTable.txt"); // ���LR������
	parserAndSemanticAnalyser.analyse(lexicalAnalyser.getResult(), "LR1_analyse.txt"); // ���LR��������
	parserAndSemanticAnalyser.outputIntermediateCode();
	parserAndSemanticAnalyser.outputIntermediateCodeToFile("IntermediateCode.txt"); // ����м����

	IntermediateCode* code = parserAndSemanticAnalyser.getIntermediateCode();
	code->divideBlocks(parserAndSemanticAnalyser.getFuncEnter()); // ���л����黮��
	code->outputBlocks();

	ObjectCodeGenerator objectCodeGenerator;
	objectCodeGenerator.analyseBlock(code->getFuncBlock()); // ������Ծ�ʹ�����Ϣ
	objectCodeGenerator.outputIBlocks(); // �����Ծ�ʹ�����Ϣ
	objectCodeGenerator.generateCode();
	objectCodeGenerator.outputObjectCode();
	objectCodeGenerator.outputObjectCode("program.asm"); // ������յĻ����

	return 0;
}