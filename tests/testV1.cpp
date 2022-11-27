#include <iostream>
#include "../mainMemory.cpp"
#include "../programCounter.cpp"
#include "../instructionRegister.cpp"
#include "../registerFile.cpp"
#include "../alu.cpp"
using namespace std;

/* Testing all the modules connected together. No state machine */

int main(){
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

  // inputting test instructions to memory
  memory.writeTo(0, 0x2001000A); // addi $r0 $r1 10

  // inputting values into register for testing
  rf.writeTo(1,2); // $r1 = 2


  // Fetch next instruction from memory
  instruction = memory.readFrom(pc.returnPC());

  // Decode the instruction in instruction register
  ir.getInstruction(instruction);

  // Get the address of the registers that are in use
  instructionType = ir.returnType();

  if (instructionType == 0){
    // R
    rs = ir.returnRs();
    rt = ir.returnRt();
    rd = ir.returnRd();
    shamt = ir.returnShamt();
    funct = ir.returnFunct();
  }
  else if (instructionType == 1){
    // J
    opcode = ir.returnOpcode();
    target = ir.returnTarget();
  }
  else{
    // I
    opcode = ir.returnOpcode();
    rs = ir.returnRs();
    rt = ir.returnRt();
    immediate = ir.returnImmediate();
  }

  // Get the value in the registers
  rsValue = rf.readFrom(rs);
  rtValue = rf.readFrom(rt);

  cout << rsValue << endl;
  cout << rtValue << endl;

  // ALU operation
  rsValue = alu.operation(opcode, rtValue, immediate);
  cout << rsValue << endl;

  // Write back
  rf.writeTo(rs, rsValue);
  rf.writeTo(rt, rtValue);

  // Check the values in the register now
  rsValue = rf.readFrom(rs);
  rtValue = rf.readFrom(rt);
  cout << rsValue << endl;
  cout << rtValue << endl;


  // Test 2

  memory.writeTo(1, 0x10080); // sll $r0 $r1 10
  rf.writeTo(1,4);

  pc.nextInstruction(); // Increment PC to get next instruction

  // Fetch next instruction from memory
  instruction = memory.readFrom(pc.returnPC());

  // Decode the instruction in instruction register
  ir.getInstruction(instruction);

  // Get the address of the registers that are in use
  instructionType = ir.returnType();

  if (instructionType == 0){
    // R
    rs = ir.returnRs();
    rt = ir.returnRt();
    rd = ir.returnRd();
    shamt = ir.returnShamt();
    funct = ir.returnFunct();
  }
  else if (instructionType == 1){
    // J
    opcode = ir.returnOpcode();
    target = ir.returnTarget();
  }
  else{
    // I
    opcode = ir.returnOpcode();
    rs = ir.returnRs();
    rt = ir.returnRt();
    immediate = ir.returnImmediate();
  }

  // Get the value in the registers
  rsValue = rf.readFrom(rs);
  rtValue = rf.readFrom(rt);

  cout << rsValue << endl;
  cout << rtValue << endl;

  // ALU operation
  rsValue = alu.operation(funct, rtValue, shamt);
  cout << rsValue << endl;

  // Write back
  rf.writeTo(rs, rsValue);
  rf.writeTo(rt, rtValue);

  // Check the values in the register now
  rsValue = rf.readFrom(rs);
  rtValue = rf.readFrom(rt);
  cout << rsValue << endl;
  cout << rtValue << endl;

}
