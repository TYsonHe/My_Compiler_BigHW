#include "IntermediateCode.h"

NewLabeler::NewLabeler() {
	index = 1;
}

string NewLabeler::newLabel() {
	return string("Label") + to_string(index++);
}

void IntermediateCode::divideBlocks(vector<pair<int, string> > funcEnter) {
	// 对每一个函数块
	for (vector<pair<int, string> >::iterator iter = funcEnter.begin(); iter != funcEnter.end(); iter++) {
		vector<Block>blocks; // 存储当前函数的基本块
		priority_queue<int, vector<int>, greater<int> >block_enter; // 记录所有基本块的入口点，使用优先队列确保入口点按升序排列
		block_enter.push(iter->first); // 将当前函数的起始地址作为第一个入口点

		// 获取当前函数的结束地址
		int endIndex = static_cast<int>(iter + 1 == funcEnter.end() ? code.size() : (iter + 1)->first);
		// 遍历当前函数的中间代码
		for (int i = iter->first; i != endIndex; i++) {
			if (code[i].op[0] == 'j') {
				if (code[i].op == "j") {
					// 若操作符是j
					block_enter.push(atoi(code[i].des.c_str())); // 将跳转目标地址作为新的入口点
				}
				else {
					// 若操作符是j=-,,j!=.j>=，j>，j<=，j<
					if (i + 1 < endIndex) {
						block_enter.push(i + 1); // 将下一条指令地址作为新的入口点
					}
					block_enter.push(atoi(code[i].des.c_str())); // 将跳转目标地址作为新的入口点
				}
			}
			else if (code[i].op == "return" || code[i].op == "call") {
				if (i + 1 < endIndex) {
					block_enter.push(i + 1); // 将下一条指令地址作为新的入口点（返回语句或函数调用后面的指令）
				}
			}
		}

		// 划分基本块	
		Block block;
		map<int, string>labelEnter; // 入口点和标签的对应关系
		map<int, int>enter_block; // 建立入口点和block的对应关系
		int firstFlag = true; // 函数块第一块标记，该块命名为函数名
		int enter;
		int lastEnter = block_enter.top();
		block_enter.pop();
		while (!block_enter.empty()) {
			// 插入四元式到block中
			enter = block_enter.top();
			block_enter.pop();

			if (enter == lastEnter) {
				// 跳过相同入口点的情况
				continue;
			}

			for (int i = lastEnter; i != enter; i++) {
				// 将指令插入当前基本块中
				block.codes.push_back(code[i]);
			}

			if (!firstFlag) {
				// 该基本块不是函数块的第一块基本块
				block.name = nl.newLabel(); // 生成新的标签作为基本块的名字
				labelEnter[lastEnter] = block.name; // 将入口点和标签的对应关系记录下来
			}
			else {
				// 该基本块是函数块的第一块基本块
				block.name = iter->second; // 使用函数名作为基本块的名字
				firstFlag = false;
			}
			enter_block[lastEnter] = static_cast<int>(blocks.size()); // 记录入口点和基本块索引的对应关系
			blocks.push_back(block); // 将当前基本块加入到基本块列表中
			lastEnter = enter;
			block.codes.clear(); // 清空当前基本块的指令列表，准备存放下一个基本块的指令
		}
		if (!firstFlag) {
			// 该基本块不是函数块的第一块基本块
			block.name = nl.newLabel();
			labelEnter[lastEnter] = block.name;
		}
		else {
			// 该基本块是函数块的第一块基本块
			block.name = iter->second;
			firstFlag = false;
		}
		// 处理最后一个基本块（最后一个入口点到中间代码末尾的部分）
		if (iter + 1 != funcEnter.end()) {
			// 在两个函数的起点之间
			for (int i = lastEnter; i != (iter + 1)->first; i++) {
				block.codes.push_back(code[i]);
			}
		}
		else {
			// 在最后一个函数至中间代码末尾
			for (int i = lastEnter; i != code.size(); i++) {
				block.codes.push_back(code[i]);
			}
		}
		enter_block[lastEnter] = static_cast<int>(blocks.size());
		blocks.push_back(block);

		// 设置基本块的后继基本块索引
		int blockIndex = 0;
		for (vector<Block>::iterator bIter = blocks.begin(); bIter != blocks.end(); bIter++, blockIndex++) {
			// 遍历每一个基本块
			vector<Quaternary>::reverse_iterator lastCode = bIter->codes.rbegin();
			if (lastCode->op[0] == 'j') {
				if (lastCode->op == "j") {
					// 若操作符是j
					bIter->next1 = enter_block[atoi(lastCode->des.c_str())]; // 设置无条件跳转的目标基本块索引
					bIter->next2 = -1; // 设置无条件跳转的第二个目标基本块索引为-1
				}
				else {
					// 若操作符是j=-,,j!=.j>=，j>，j<=，j<
					bIter->next1 = blockIndex + 1; // 设置有条件跳转的第一个目标基本块索引为下一个基本块索引
					bIter->next2 = enter_block[atoi(lastCode->des.c_str())]; // 设置有条件跳转的第二个目标基本块索引
					bIter->next2 = bIter->next1 == bIter->next2 ? -1 : bIter->next2; // 如果两个目标基本块索引相同，则将第二个目标基本块索引设置为-1
				}
				lastCode->des = labelEnter[atoi(lastCode->des.c_str())]; // 将跳转目标地址替换为对应的标签
			}
			else if (lastCode->op == "return") {
				// 如果最后一条指令是返回指令
				bIter->next1 = bIter->next2 = -1; // 设置返回指令后没有后继基本块
			}
			else {
				// 如果最后一条指令不是跳转指令也不是返回指令
				bIter->next1 = blockIndex + 1; // 设置下一个基本块索引为当前基本块索引加1
				bIter->next2 = -1; // 设置第二个后继基本块索引为-1
			}


		}

		// 将当前函数的基本块列表存入函数块映射中
		funcBlocks[iter->second] = blocks;
	}
}

void IntermediateCode::output(ostream& out) {
	int i = 0;
	for (vector<Quaternary>::iterator iter = code.begin(); iter != code.end(); iter++, i++) {
		out << CYAN;
		out << setw(4) << i;
		out << "( " << iter->op << " , ";
		out << iter->src1 << " , ";
		out << iter->src2 << " , ";
		out << iter->des << " )";
		out << endl;
		out << RESET;
	}
}

void IntermediateCode::outputFile(ostream& out) {
	int i = 0;
	for (vector<Quaternary>::iterator iter = code.begin(); iter != code.end(); iter++, i++) {
		out << setw(4) << i;
		out << "( " << iter->op << " , ";
		out << iter->src1 << " , ";
		out << iter->src2 << " , ";
		out << iter->des << " )";
		out << endl;
	}
}

void IntermediateCode::outputBlocks(ostream& out) {
	for (map<string, vector<Block> >::iterator iter = funcBlocks.begin(); iter != funcBlocks.end(); iter++) {
		out << "[" << iter->first << "]" << endl;
		for (vector<Block>::iterator bIter = iter->second.begin(); bIter != iter->second.end(); bIter++) {
			out << bIter->name << ":" << endl;
			for (vector<Quaternary>::iterator cIter = bIter->codes.begin(); cIter != bIter->codes.end(); cIter++) {
				out << "    " << "(" << cIter->op << "," << cIter->src1 << "," << cIter->src2 << "," << cIter->des << ")" << endl;
			}
			out << "    " << "next1 = " << bIter->next1 << endl;
			out << "    " << "next2 = " << bIter->next2 << endl;
		}
		cout << endl;
	}
}

void IntermediateCode::emit(Quaternary q) {
	code.push_back(q);
}

void IntermediateCode::emit(string op, string src1, string src2, string des) {
	emit(Quaternary{ op,src1,src2,des });
}

void IntermediateCode::back_patch(list<int>nextList, int quad) {
	for (list<int>::iterator iter = nextList.begin(); iter != nextList.end(); iter++) {
		code[*iter].des = to_string(quad);
	}
}

void IntermediateCode::output() {
	output(cout);
}

void IntermediateCode::output(const char* fileName) {
	ofstream fout;
	fout.open(fileName);
	if (!fout.is_open()) {
		cerr << "file " << fileName << " open error" << endl;
		return;
	}
	outputFile(fout);

	fout.close();
}

void IntermediateCode::outputBlocks() {
	outputBlocks(cout);
}

void IntermediateCode::outputBlocks(const char* fileName) {
	ofstream fout;
	fout.open(fileName);
	if (!fout.is_open()) {
		cerr << "file " << fileName << " open error" << endl;
		return;
	}
	outputBlocks(fout);

	fout.close();
}

int IntermediateCode::nextQuad() {

	return static_cast<int>(code.size());
}

map<string, vector<Block> >* IntermediateCode::getFuncBlock() {
	return &funcBlocks;
}