#include<bits/stdc++.h>
#include "A1_T4_S5_20230421_20230053_20230370.h"

#include <map>
using namespace std;


// --------------------------- Register ---------------------------

Register::Register() {
    for (int i = 0; i < size; i++) {
        memory[i] = 0;
    }
}

int Register::getCell(int add)
{
    return memory[add];
}

void Register::setCell(int add, int val) {
    memory[add] = val;
}

// --------------------------- Memory ---------------------------

Memory::Memory() {
    for (int i = 0; i < size; i++) {
        memory[i] = "00";
    }
}

string Memory::getCell(int add) {
    return memory[add];
}

void Memory::setCell(int add, string val) {
    memory[add] = val;
}

// --------------------------- ALU ---------------------------

string ALU::hexToDec(string hex)
{
    map<char, int> hexMap = {{'A', 10}, {'B', 11},{'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}};
    int dec = 0, base = 1;
    for (int i = hex.length() - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            dec += (hex[i] - 48) * base;
            base = base * 16;
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            dec = hexMap[hex[i]];

        }
    }
    return to_string(dec);
}

string ALU::decToHex(int dec)
{
    string hex = "";
    while (dec > 0) {
        int rem = dec % 16;
        if (rem < 10) {
            hex = to_string(rem) + hex;
        } else {
            hex = char(rem + 55) + hex;
        }
        dec = dec / 16;
    }
    return hex;
}

string ALU::add(int idx1, int idx2, int idx3, Register &reg) {
    int val1 = reg.getCell(idx1), val2 = reg.getCell(idx2);

    int sum = val1 + val2;

    if (sum > 32767)
    {
        sum -= 65536;
    }
    else if (sum < -32768)
    {
        sum += 65536;
    }

    reg.setCell(idx3, sum);

    return decToHex(sum);
}

bool ALU::isValid(string hex)
{
    for (int i = 0; i < hex.length(); i++) {
        if (!((hex[i] >= '0' && hex[i] <= '9') || (hex[i] >= 'A' && hex[i] <= 'F'))) {
            return false;
        }
    }
    return true;
}

// --------------------------- CU ---------------------------

void CU::load(int idx1, int idx2, Register &reg, Memory &mem) {
    string hex = mem.getCell(idx2);
    if (hex.empty()) {
        cerr << "Memory cell is empty" << endl;
        return;
    }
    if (!ALU().isValid(hex)) {
        cerr << "Invalid hexa value" << endl;
        return;
    }
    int dec = stoi(ALU().hexToDec(hex));
    reg.setCell(idx1, dec);
}

void CU::load(int idxReg, int val, Register &reg) {
    reg.setCell(idxReg, val);
}


void CU::store(int idx1, int idx2, Register &reg, Memory &mem) {
    string hex = ALU().decToHex(reg.getCell(idx1));
    mem.setCell(idx2, hex);
}

void CU::move(int idx1, int idx2, Register &reg) {
    reg.setCell(idx2, reg.getCell(idx1));
}

void CU::jump(int idxReg, int idxMem, Register &reg, int &pc) {
    if (reg.getCell(idxReg) == reg.getCell(0)) {
        pc = idxMem;
    }
}

void CU::halt() {
    cout << "Program halted" << endl;
}

// --------------------------- CPU ---------------------------

void CPU::fetch(Memory &mem) {
    instructionRegister = mem.getCell(programCounter);
}

vector<int> CPU::decode() {
    vector<int> decoded;
    for (int i = 0; i < instructionRegister.length(); i++) {
        if (isdigit(instructionRegister[i])) {
            decoded.push_back(instructionRegister[i] - '0');

        }else {
            decoded.push_back(stoi(ALU::hexToDec(&instructionRegister[i])));
        }
    }
    return decoded;
}

void CPU::execute(Register &reg, Memory &mem, vector<int> decoded) {
    cout << "Executing instruction: " << instructionRegister << endl;
    int opcode = decoded[0];
    int R = decoded[1];
    int S = decoded[2];
    int T = decoded[3];
    int XY = (decoded[2] * 16) + decoded[3];

    switch (opcode) {
        case 1 : cu.load(R, XY, reg, mem);break;

        case 2 : cu.load(R, XY, reg);break;

        case 3 :
            if (S == 0 && T == 0) {
                cu.store(R, 0x00, reg, mem);
            } else {
                cu.store(R, XY, reg, mem);
            }
            break;

        case 4 : cu.move(R, S, reg);break;

        case 5 : alu.add(S, T, R, reg);break;

        case 6 : alu.add(S, T, R, reg);break;

        case 11 : cu.jump(R, XY, reg, programCounter);break;

        case 12 : cu.halt();break;

        default : cout << "Invalid opcode " << opcode << endl;break;
    }
}

void CPU::runNextStep(Memory &mem) {
    fetch(mem);
    vector<int> decoded = decode();
    execute(reg, mem, decoded);
    programCounter++;
}

// --------------------------- Machine ---------------------------

void Machine::loadProgramFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file" << filename << endl;
        return;
    }

    string line;
    int address = 0;
    while (getline(file, line) && address < 256) {
        memory.setCell(address, line);
        address++;
    }

    file.close();

    for (int i = 0; i < address; i++) {
        Processor.runNextStep(memory);
    }

}


void Machine::outputState() {
    cout << "Memory:" << endl;
    for (int i = 0; i < 256; i++) {
        cout << i << ": " << memory.getCell(i) << endl;
    }
    cout << endl << "Registers:" << endl;
    for (int i = 0; i < 16; i++) {
        cout << "R." << i << ": " << Processor.reg.getCell(i) << ".  ";
    }

    cout << "\nProgram Counter: " << Processor.programCounter << endl;
}



// --------------------------- MainUI ---------------------------

bool MainUI::enterFileOrInstructions() {

    int input = inputChoice();
    cin.ignore();

    if (input == 1)
    {
        string filename = inputFileName();
        machine.loadProgramFile(filename);
    }
    else if (input == 2)
    {
        cout << "Enter instructions (one per line , type 'finish' to finish input):" << endl;
        string inst;
        int address = 0;

        while (true) {
            getline(cin, inst);

            if (inst == "finish") {
                break;
            }

            if (address >= 256)
            {
                cerr << "Exceeded memory capacity" << endl;
                return false;
            }

            machine.memory.setCell(address, inst);
            address++;
        }

        for (int i = 0; i < address; i++) {
            machine.Processor.runNextStep(machine.memory);
        }
    } else {
        cout << "Invalid input" << endl;
        return false;
    }
    return true;
}


void MainUI::DisplayMenu() {
    cout << "\nPlease Choose : " << endl;
    cout << "1. Show state" << endl;
    cout << "2. Exit" << endl;
    cout << "Enter your choice: ";
    while (true) {
        int input;
        cin >> input;
        if (input == 1) {
            machine.outputState();
        } else if (input == 2) {
            exit(0);
        } else {
            cout << "Invalid input" << endl;
        }
        DisplayMenu();
    }
}

bool MainUI::getFileOrInstructions() {
    while (true) {
        if (!enterFileOrInstructions()) {
            continue;
        }
        while (true) {
            DisplayMenu();
            string input;
            cin >> input;
            if (input == "state") {
                machine.outputState();
            } else if (input == "exit") {
                return true;
            } else {
                cout << "Invalid input" << endl;
            }
        }
    }
}

int MainUI::inputChoice() {
    cout << "Please choose file or instructions" << endl;
    cout << "1. file" << endl;
    cout << "2. instructions" << endl;
    cout << "Enter your choice: ";
    int input;
    cin >> input;
    return input;
}

string MainUI::inputFileName() {
    cout << "Enter the name of the file" << endl;
    string fileName;
    cin >> fileName;
    return fileName;
}




