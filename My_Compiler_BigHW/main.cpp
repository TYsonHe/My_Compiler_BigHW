#include"LexicalAnalyser.h"
#include "ParserAndSemanticAnalyser.h"
#include "ObjectCodeGenerator.h"
int main() {
	LexicalAnalyser lexicalAnalyser("test.txt");
	lexicalAnalyser.analyse();
	lexicalAnalyser.outputToScreen();
	lexicalAnalyser.outputToFile("Lexical_analyse.txt");

	ParserAndSemanticAnalyser parserAndSemanticAnalyser("productions.txt");
	parserAndSemanticAnalyser.outputDFA("ItemSets.txt");
	parserAndSemanticAnalyser.analyse(lexicalAnalyser.getResult(), "LR1_analyse.txt");
	parserAndSemanticAnalyser.outputIntermediateCode();
	parserAndSemanticAnalyser.outputIntermediateCodeToFile("IntermediateCode.txt");

	IntermediateCode* code = parserAndSemanticAnalyser.getIntermediateCode();
	code->divideBlocks(parserAndSemanticAnalyser.getFuncEnter());
	code->outputBlocks();

	ObjectCodeGenerator objectCodeGenerator;
	objectCodeGenerator.analyseBlock(code->getFuncBlock());
	objectCodeGenerator.outputIBlocks();
	objectCodeGenerator.generateCode();
	objectCodeGenerator.outputObjectCode();
	objectCodeGenerator.outputObjectCode("program.asm");

	return 0;
}