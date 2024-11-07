#include "A1_T4_S5_S6_20230053_20230421_20230370.h"
using namespace std;
#define nd <<endl;


vector<string> output;
void displayOutput();
int concToDec(char ch);
string checkH(string hexString);


//-------------------------------- Machine --------------------------------
Machine::Machine() {
    PC = 10,sh = 0;
    for (int i = 0; i < 256; i++)
    {
        memoryCell[i].setMemCell("00");
    }
    for (int i = 0; i < 16; i++)
    {
        reg[i].setReg("00");
    }
}

void Machine::menu() {
    cout << "Please Choose One:" nd
    cout << "1. Show status after instruction" nd
    cout << "2. Show memory" nd
    cout << "3. Show Output" nd
    cout << "4. Exit" nd
    cout << "Enter your choice: ";
    cin >> sh;
}

void Machine::loadProgram()
{
    int cnt = 0;
    string fileName;
    string operand, instruction;
    cout << "Please enter file name to load the instructions: ";
    cin >> fileName;
    read.open(fileName);
    if (read.fail())
    {
        cerr << "The file couldn't be opened!!!!" << endl;
        return;
    }
    while (read >> operand >> operand >> operand)
    {
        cnt++;
    }
    if (cnt > 128)
    {
        cerr << "There isn't no memory more!!" << endl;
        return;
    }
    read.close();
    read.open(fileName);

    for (int i = 0, j  = 10; i < cnt; i++, j += 2)
    {
        read >> operand;
        instruction = operand.substr(operand.size() - 1);
        read >> operand;
        instruction += operand.substr(operand.size() - 1);
        memoryCell[j].setMemCell(instruction);
        read >> operand;
        instruction = operand.substr(operand.size() - 2);
        memoryCell[j + 1].setMemCell(instruction);
    }
}

void Machine::runProgram()
{
    while (true) {
        menu();
        if (sh == 1) fetch();

        else if (sh == 2) mem();

        else if (sh == 3) displayOutput(),cout nd

        else if (sh == 4)
        {
            cout << "Exiting program, Welcome To use vole machine simulator\n";
            break;
        }
        else cout << "Invalid option, Please try again.\n";

    }
}

void Machine::Status()
{
    cout << "Program counter : " << hex << uppercase << PC << endl;
    cout << "Instruction register : " << instructionRegister << endl;
    cout << "\nRegisters:\n";
    for (int i = 0; i < 8; i++)
    {
        cout << hex << uppercase << i << ' ' << reg[i].getReg() << " \t ";
        cout << hex << uppercase << i + 8 << ' ' << reg[i + 8].getReg() << endl;
    }
    cout << "--------------------------------------------------------\n";
}

void Machine::mem() {
    cout << "------------------------------------------------------------------------------------------------------------\n\n";
    cout << "Memory cells:\n";
    for (int i = 0; i < 256; i++) {
        cout << hex << uppercase << setw(2) << setfill('0') << i << ' ' << memoryCell[i].getMemCell() << "  \t  ";
        if ((i + 1) % 8 == 0) {
            cout nd
        }
    }
    cout << "------------------------------------------------------------------------------------------------------------\n\n";
}

bool Machine::valid1()
{
    string charSet = "12345BC";
    return charSet.find(toupper(instructionRegister[0])) != string::npos;
}

bool Machine::valid2() {
    string charSet = "0123456789ABCDEF";
    return charSet.find(toupper(instructionRegister[1])) != string::npos;
}

bool Machine::valid3() {
    string str = instructionRegister.substr(2, 2);
    int intVal = stoi(str, nullptr, 16);
    return intVal >= 0 && intVal < 256;
}

void Machine::fetch() {
    instructionRegister = memoryCell[PC].getMemCell() + memoryCell[PC + 1].getMemCell();
    PC += 2;
    decodeInstruction();
}

void Machine::decodeInstruction() {
    callInstruction();
    cout << endl << endl << instructionRegister << endl << endl;
    if (instructionRegister == "C000")
        return;
    if (!(valid1() && valid2() && valid3())) {
        return;
    }
    if (sh == 1)
    {
        print();
        Status();
        fetch();
    }
    else if(sh == 2)
    {
        mem();
    }
}

void Machine::callInstruction()
{
    switch (instructionRegister[0])
    {
        case '1':load1();break;
        case '2':load2();break;
        case '3':store3();break;
        case '4':move4();break;
        case '5':add5();break;
        case '6':add6();break;
        case 'B':jumpB();break;
        case 'C':halt();break;
    }
}

void Machine::print() {
    static int instruction_number = 1;
    cout << instruction_number++ << "] ";

    switch (instructionRegister[0]) {
        case '1':cout << "Load R" << instructionRegister[1] << " with " << instructionRegister.substr(2, 2) << endl;break;
        case '2':cout << "Load R" << instructionRegister[1] << " with " << instructionRegister.substr(2, 2) << endl;break;
        case '3':cout << "Store R" << instructionRegister[1] << " into memory cell " << instructionRegister.substr(2, 2) << endl;break;
        case '4':cout << "Copy R" << instructionRegister[2] << " to R" << instructionRegister[3] << endl;break;
        case '5':cout << "Add R" << instructionRegister[2] << " with R" << instructionRegister[3] << " into R" << instructionRegister[1] << endl;break;
        case '6':cout << "Add R" << instructionRegister[1] << " and R" << instructionRegister[2] << " and store in R" << instructionRegister[3] << endl;break;
        case 'B':cout << "Compare R" << instructionRegister[1] << " with R0 and jump to address " << instructionRegister.substr(2, 2) << " if equal" << endl;break;
        case 'C':cout << "Program halted." << endl;break;
        default:cout << "Unknown instruction." << endl;break;
    }
}
void Machine::load1() {
    int cell = stoi(instructionRegister.substr(2, 2), nullptr, 16);
    int add = stoi(instructionRegister.substr(1, 1), nullptr, 16);
    reg[add].setReg(memoryCell[cell].getMemCell());
}

void Machine::load2() {
    int add = stoi(instructionRegister.substr(1, 1), nullptr, 16);
    reg[add].setReg(instructionRegister.substr(2, 2));
}

void Machine::store3()
{
    int idxR = concToDec(instructionRegister[1]);
    int firstCell = concToDec(instructionRegister[2]);
    int secCell = concToDec(instructionRegister[3]);
    int memIdx = (firstCell * 16) + secCell;
    memoryCell[memIdx].setMemCell(reg[idxR].getReg());
    output.push_back(checkH(memoryCell[0].getMemCell()));
}

void Machine::move4() {
    int R = concToDec(instructionRegister[2]);
    int S = concToDec(instructionRegister[3]);
    reg[S].setReg(reg[R].getReg());
}

unordered_map<char, int> hexToDec = {
        {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
        {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
        {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}
};

unordered_map<int, char> decToHex = {
        {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'}, {4, '4'},
        {5, '5'}, {6, '6'}, {7, '7'}, {8, '8'}, {9, '9'},
        {10, 'A'}, {11, 'B'}, {12, 'C'}, {13, 'D'}, {14, 'E'}, {15, 'F'}
};

string intToHex(int val)
{
    val &= 255;
    stringstream ss;
    ss << hex << uppercase << setw(2) << setfill('0') << val;
    return ss.str();
}

string hexAdd(string hex1, string hex2) {
    if (hex1.size() < hex2.size())
        swap(hex1, hex2);

    string ans = "";
    int carry = 0, i, j;

    for (i = hex1.size() - 1, j = hex2.size() - 1; j >= 0; i--, j--) {
        int sum = hexToDec[hex1[i]] + hexToDec[hex2[j]] + carry;
        char add_bit = decToHex[sum % 16];
        ans.push_back(add_bit);
        carry = sum / 16;
    }

    while (i >= 0) {
        int sum = hexToDec[hex1[i]] + carry;
        char add_bit = decToHex[sum % 16];
        ans.push_back(add_bit);
        carry = sum / 16;
        i--;
    }

    if (carry) {
        ans.push_back(decToHex[carry]);
    }

    reverse(ans.begin(), ans.end());

    unsigned int resultValue;
    sscanf(ans.c_str(), "%x", &resultValue);

    if (resultValue > 0x7F) {
        int signedResult = static_cast<int>(resultValue) - 256;
        return intToHex(signedResult);
    }

    return intToHex(static_cast<int>(resultValue));
}

void Machine::add5() {
    int idx1 = concToDec(instructionRegister[1]);
    string hex1 = reg[idx1].getReg();
    int idx2 = concToDec(instructionRegister[2]);
    string hex2 = reg[idx2].getReg();
    int idx3 = concToDec(instructionRegister[3]);
    string hex3 = reg[idx3].getReg();

    cout << hex2 << " + " << hex3 << " = " << hexAdd(hex2, hex3) nd
    reg[idx1].setReg(hexAdd(hex2, hex3));
}

void Machine::add6() {
    int idx1 = concToDec(instructionRegister[1]);
    float val1 = stof(reg[idx1].getReg());
    int idx2 = concToDec(instructionRegister[2]);
    float val2 = stof(reg[idx2].getReg());
    int idx3 = concToDec(instructionRegister[3]);

    float result = val1 + val2;

    reg[idx3].setReg(to_string(result));
}

void Machine::jumpB() {
    int R = concToDec(instructionRegister[1]);
    int address = stoi(instructionRegister.substr(2, 2), nullptr, 16);
    if (reg[R].getReg() == reg[0].getReg()) {
        PC = address;
    }
}

void Machine::halt() {
    cout<<"Program halted"<<endl;
}


//------------------------------- Register -------------------------------

void Register::setReg(string newVal) {
    value = newVal;
}

string Register::getReg() {
    return value;
}

//------------------------------- Memory -------------------------------

void Memory::setMemCell(string newVal) {
    value = newVal;
}

string Memory::getMemCell() {
    return value;
}

//------------------------------- Helper Functions -------------------------------

int concToDec(char ch) {
    if (48 <= ch && ch <= 57)
        return ch - 48;
    return ch - 55;
}

void displayOutput() {
    cout << "Output: ";
    for (int i = 0; i < output.size(); i++) {
        cout << output[i];
    }
    cout nd
}

string checkH(string hexString)
{

    string res = "";
    int ascV;
    stringstream s;
    s << hex << hexString;
    s >> ascV;

    if (isprint(ascV))
    {
        char character = static_cast<char>(ascV);
        res.push_back(character);
    }
    else
    {
        char signedValue = static_cast<signed char>(ascV);

        res += to_string(signedValue);
    }

    return res;
}

