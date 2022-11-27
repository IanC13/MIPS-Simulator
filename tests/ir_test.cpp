#include <iostream>
#include <string>
#include <bitset>
#include "../InstructionRegister.cpp"

/* This file was used to test the Instruction Register module separate from the complete simulator.
   For this to execute, the *comment* to include programCounter.cpp in instructionRegister.cpp
   and the *comment* to include mainMemory.cpp in programCounter.cpp will need to be removed.
   ie. *Do* include mainMemory.cpp in programCounter.cpp and *Do* include programCounter.cpp
   in instructionRegister.cpp
*/

using namespace std;

int main(){
  InstructionRegister ir;
  ProgramCounter pc;
  MainMemory memory;




  memory.writeTo(0,0b00000000000100101001100010000010);
  cout << "Instruction under test is: srl $s3,$s2,2" << endl;

  ir.getInstruction(memory.readFrom(0));
  std::bitset<6> y(ir.returnOpcode());
  cout << "opcode: " << y << endl;

  std::bitset<5> i(ir.returnRs());
  cout << "rs: " << i << endl;

  std::bitset<5> j(ir.returnRt());
  cout << "rt:" << j  << endl;

  std::bitset<5> k(ir.returnRd());
  cout << "rd: "<< k << endl;

  cout << "shamt: " << ir.returnShamt() << endl;

  std::bitset<6> x(ir.returnFunct());
  cout << "funct: " << x << endl;

  //cout << "immediate: " << ir.returnImmediate() << endl;

  //cout << "target: " << ir.returnTarget() << endl;

}
