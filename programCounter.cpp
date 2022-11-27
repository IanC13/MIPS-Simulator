#include <iostream>
//#include "mainMemory.cpp"

using namespace std;


class ProgramCounter{
public:
  int pc; // counter
  MainMemory memory;

  ProgramCounter();

  void nextInstruction();
  void nextInstructionBranch(int branchAddress);
  int returnPC();
};

//________________________________________________________________________

ProgramCounter::ProgramCounter():memory(){
  pc = 0;
}

void ProgramCounter::nextInstruction(){
  pc += 1;
}

void ProgramCounter::nextInstructionBranch(int branchAddress){
  pc = branchAddress;
}

int ProgramCounter::returnPC(){
  return pc;
}
