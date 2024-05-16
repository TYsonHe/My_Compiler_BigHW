
/*
* Date: 2024-05-17
* 作者: 2153698-何诗锟
*/

#include"LexicalAnalyser.h"
#include "ParserAndSemanticAnalyser.h"
#include "ObjectCodeGenerator.h"


int main() {
	LexicalAnalyser lexicalAnalyser("test.txt"); // 要分析的文件
	lexicalAnalyser.analyse();
	lexicalAnalyser.outputToScreen();
	lexicalAnalyser.outputToFile("Lexical_analyse.txt");

	ParserAndSemanticAnalyser parserAndSemanticAnalyser("productions.txt");
	parserAndSemanticAnalyser.outputDFA("ItemSets.txt"); // 输出项目集DFA
	parserAndSemanticAnalyser.printLRTable("LRTable.txt"); // 输出LR分析表
	parserAndSemanticAnalyser.analyse(lexicalAnalyser.getResult(), "LR1_analyse.txt"); // 输出LR分析过程
	parserAndSemanticAnalyser.outputIntermediateCode();
	parserAndSemanticAnalyser.outputIntermediateCodeToFile("IntermediateCode.txt"); // 输出中间代码

	IntermediateCode* code = parserAndSemanticAnalyser.getIntermediateCode();
	code->divideBlocks(parserAndSemanticAnalyser.getFuncEnter()); // 进行基本块划分
	code->outputBlocks();

	ObjectCodeGenerator objectCodeGenerator;
	objectCodeGenerator.analyseBlock(code->getFuncBlock()); // 分析活跃和待用信息
	objectCodeGenerator.outputIBlocks(); // 输出活跃和待用信息
	objectCodeGenerator.generateCode();
	objectCodeGenerator.outputObjectCode();
	objectCodeGenerator.outputObjectCode("program.asm"); // 输出最终的汇编结果

	return 0;
}