#ifndef A1_T4_S5_S6_20230053_20230421_20230370_H
#define A1_T4_S5_S6_20230053_20230421_20230370_H
#include <bits/stdc++.h>

using namespace std;
// Menna Talla Gamal
class Memory {
private:
    string value;
public:
    Memory() : value("00") {}
    void setMemCell(string newVal);
    string getMemCell();
};
// Israa Abd El Haq
class Register {
private:
    string value;
public:
    Register() : value("00") {}
    void setReg(string newVal);
    string getReg();
};
// Mahmoud Hosny, Menna Talla Gamal
class Machine {
private:
    Memory memoryCell[256];
    Register reg[16];
    string instructionRegister;
    int PC = 10;
    int sh;
    ifstream read;

public:
    Machine();
    void load1();
    void load2();
    void store3();
    void move4();
    void add5();
    void add6();
    void jumpB();
    void halt();
    void menu();
    void decodeInstruction();
    void callInstruction();
//    Menna Talaa Gamal
    void loadProgram();
    void runProgram();
    void Status();
    void mem();
    void print();
    void fetch();
    bool valid1();
    bool valid2();
    bool valid3();
};


#endif
