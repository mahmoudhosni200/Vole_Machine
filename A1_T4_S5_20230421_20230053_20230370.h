#ifndef VOLE_H


#include <bits/stdc++.h>

using namespace std;

class Memory {
    string memory[256];
    int size = 256;
public:
    Memory();
    string getCell(int add);
    void setCell(int add, string val);
};

class Register {
    int memory[16]{};
    int size = 16;
public:
    Register();
    int getCell(int add);
    void setCell(int add, int val);
};

class CU {
public:
    void load(int idx1, int idx2, Register &reg, Memory &mem);
    void load(int idxReg, int val, Register &reg);
    void store (int idx1, int idx2, Register &reg, Memory &mem);
    void move (int idx1, int idx2, Register &reg);
    void jump (int idxReg, int idxMem, Register &reg, int &pc);
    void halt();
};


class ALU {
public:
    static string hexToDec(string hex);
    string decToHex(int dec);
    bool isValid(string hex);
    string add(int idx1, int idx2, int idx3, Register &reg);
};



class CPU {
    string instructionRegister;
    ALU alu;
    CU cu;
    void fetch(Memory &mem);
    vector<int> decode();
    void execute(Register &reg, Memory &mem, vector<int> decoded);
public:
    int programCounter;
    Register reg;
    void runNextStep(Memory &mem);
};

class Machine {
public:
    CPU Processor;
    Memory memory;
    void loadProgramFile(const string &filenam);
    void outputState();
};

class MainUI{
    Machine machine;
    bool enterFileOrInstructions();
public:
    bool getFileOrInstructions();
    void DisplayMenu();
    string inputFileName();
    int inputChoice();
};

#endif