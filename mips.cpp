#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include "mainMemory.cpp"
#include "programCounter.cpp"
#include "instructionRegister.cpp"
#include "registerFile.cpp"
#include "alu.cpp"
using namespace std;

MainMemory memory;

ProgramCounter pc;
InstructionRegister ir;
RegisterFile rf;
Alu alu;

int instruction;
int instructionType;
int opcode;
// R and I
int rs; // First source register
int rt; // second source register
// R only
int rd; // Destination register
int shamt; // Shift amount
int funct; // function
// I only
int immediate;
// J only
int target;

int rsValue;
int rtValue;
int rdValue;

enum State {fetch, decode, execute, memoryAccess, writeBack};
State currentState = fetch;

int instructionsExecuted;

std::vector<int> returnAddress;
int returnAddr;
std::vector<int> s4Stack;

void cycle();
void fileToMemory();
string instructionLog();
int j = 0;

int main(){
  fileToMemory();

  while(opcode != 0b111111){ // 0b111111 is the stop condition
    cycle();
  }

  cout << "Instructions executed: " << instructionsExecuted << endl;

  // check that the calculations are correct and has been stored into memory correctly
  for(int i = 50; i < 150; i++){
    // 50 is start of data memory
    cout << "The Square of " << i-50 << " is: ";
    cout << memory.readFrom(i) << endl;
  }
}

string instructionLog(){
  // prints the instructions issued and also writes it into a text file
  ofstream logFile;
  if(j == 0){
    logFile.open("instructionLog.txt");
  }
  else{
    logFile.open("instructionLog.txt", ios::out | ios::app);
  }
  for(j; j < 1; j++){
    cout << "Instruction Log" << endl;
  }
  string logOutput = ir.classifyInstruction();
  cout << logOutput << endl;
  logFile << logOutput << endl;

  logFile.close();
}

void fileToMemory(){
  // Reads in machine code and writes it to Main Memory array
  int i = 0;
  int integer;
  ifstream file("showcase.txt"); // read machine code (bits) from file
  bitset <32> x; // variable that is 32 bits

  while(file >> x){
    integer = (int)(x.to_ulong()); // Change the binary to integer as I've defined everything to be int lol
    memory.writeTo(i, integer);
    i += 1;
  }
}


void cycle(){
  // fetch/decode/execute/memory access/write back cycle state machine
  switch(currentState){
    case fetch:
      // Fetch next instruction from memory
      instruction = memory.readFrom(pc.returnPC());
      currentState = decode;
      break;

    case decode:
      // Decode the instruction in instruction register
      ir.getInstruction(instruction);

      // Get instruction type
      instructionType = ir.returnType();

      /* Get the address of the registers that are in use
         and the value in those registers depending on the
         instruction type*/
      if (instructionType == 0){
        // R
        rs = ir.returnRs(); // returns the register
        rt = ir.returnRt(); // returns the register
        rd = ir.returnRd(); // returns the register
        shamt = ir.returnShamt(); // returns the value
        funct = ir.returnFunct(); // returns the value

        rsValue = rf.readFrom(rs);
        rtValue = rf.readFrom(rt);
        rdValue = rf.readFrom(rd);

      }
      else if (instructionType == 1){
        // J
        opcode = ir.returnOpcode(); // returns the value
        target = ir.returnTarget(); // returns the value
      }
      else{
        // I
        opcode = ir.returnOpcode(); // returns the value
        rs = ir.returnRs(); // returns the register (not value in the register)
        rt = ir.returnRt();
        immediate = ir.returnImmediate(); // returns the value

        rsValue = rf.readFrom(rs);
        rtValue = rf.readFrom(rt);

      }
      currentState = execute;
      break;

    case execute:
      instructionLog(); // outputs the instructions executed
      if(opcode != 0b000011){
        /* 000011 is jal. Increment the PC here if it is not jal. Jal uses
           the current PC address*/
        pc.nextInstruction(); // Increments PC
      }
      if(instructionType == 0){
        // R
        if(funct == 0b100000){
          // add
          rdValue = alu.operation(funct, rsValue, rtValue);
        }
        else if(funct == 0b000010){
          // srl
          rdValue = alu.operation(funct, rtValue, shamt);
        }
        else if(funct == 0b000000){
          // sll
          rdValue = alu.operation(funct, rtValue, shamt);
        }
        else if(funct == 0b001000){
          // jr
          // jump to return address.
          returnAddr = returnAddress.back() + 1; // retrieves the return address. +1 for next instruction
          returnAddress.pop_back(); // pop it off the stack
          pc.nextInstructionBranch(returnAddr); // jumps back to return address
          currentState = fetch;
          instructionsExecuted += 1;
          break;
        }
      }
      else if(instructionType == 1){
        // J
        if(opcode == 0b000011){
          // jal
          //jump and link (store current address in stack)
          returnAddress.push_back(pc.returnPC()); // pushes the current PC address onto the stack
          pc.nextInstructionBranch(target); // go to the jump address
          currentState = fetch;
          instructionsExecuted += 1;
          break;
        }
        else if(opcode == 0b000010){
          // j
          pc.nextInstructionBranch(target);
          currentState = fetch;
          instructionsExecuted += 1;
          break;
        }
      }
      else if(instructionType == 2){
        // I
        if(opcode == 0b001000){
          // addi
          rtValue = alu.operation(opcode, rsValue, immediate);
        }
        else if(opcode == 0b001100){
          // andi
          rtValue = alu.operation(opcode, rsValue, immediate);
        }
        else if(opcode == 0b000100){
          // beq
          if(alu.operation(opcode,rsValue,rtValue) == 1){
            pc.nextInstructionBranch(immediate);
            // in this case, immediate is the branching address
            currentState = fetch;
            instructionsExecuted += 1;
            break;
          }
        }
        else if(opcode == 0b000001){
          // blt
          if(alu.operation(opcode,rsValue,rtValue) == 1){
            pc.nextInstructionBranch(immediate);
            currentState = fetch;
            instructionsExecuted += 1;
            break;
          }
        }
        else if(opcode == 0b000101){
          // bne
          if(alu.operation(opcode,rsValue,rtValue) == 1){
            pc.nextInstructionBranch(immediate);
            currentState = fetch;
            instructionsExecuted += 1;
            break;
          }
        }
        else if(opcode == 0b101011){
          // sw
          if(rs == 0b00001){
            s4Stack.push_back(rf.readFrom(20));
            // Push $s4 onto stack. Value in this register is needed when the recursion unwinds
          }
        }
        else if(opcode == 0b100011){
          //lw
          rf.writeTo(20, s4Stack.back()); // pop $s4 off of stack
          s4Stack.pop_back();
        }
      }
      currentState = memoryAccess;
      break;

    case memoryAccess:
      if (opcode == 0b101011 && rs != 0b00001){ // store to data memory and not stack
        // sw
        if(rsValue == 0){
          memory.writeTo( (50 + rsValue), 0);
        }
        else{
          memory.writeTo( (50 + rsValue), rtValue);
        }
      }
      currentState = writeBack;
      break;

    case writeBack:
      if(instructionType == 0){
        // R
        rf.writeTo(rd,rdValue);
      }
      else if(instructionType == 2){
        // I
        rf.writeTo(rt,rtValue);
      }
      currentState = fetch;
      instructionsExecuted += 1;
      break;
  }
}
