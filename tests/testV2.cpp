#include <iostream>
#include "../mainMemory.cpp"
#include "../programCounter.cpp"
#include "../instructionRegister.cpp"
#include "../registerFile.cpp"
#include "../alu.cpp"
using namespace std;

/* Testing all the modules connected together. With state machine
 */

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

int i = 1;

void cycle();

int main(){
  // inputting test instructions to memory
  memory.writeTo(8, 0x8000004);
  memory.writeTo(4,0xFC000000);

  // inputting values into register for testing
  rf.writeTo(19,5); // $r1 = 2
  pc.nextInstructionBranch(8);
  cout << "The test instruction is: j main (main is in address 4)" << endl;
  cout << "Before the instruction is executed" << endl;
  cout << "The PC address is: " << pc.returnPC() << endl;
  cout << endl << "Execute Instruction" << endl;
  while(opcode != 0b111111){ // 0b111111 is the stop condition
    cycle();
  }

  cout << "The PC address is: " << pc.returnPC() << endl;



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
      /* Get the address of the registers that are in use depending on the
         instruction type and the value in those registers */
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

      if(opcode != 0b000011){
        pc.nextInstruction();
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
          currentState = fetch;
          break;
        }
      }
      else if(instructionType == 1){
        // J
        if(opcode == 0b000011){
          // jal
          pc.nextInstructionBranch(target); // go to the jump address
          currentState = fetch;
          break;
        }
        else if(opcode == 0b000010){
          // j
          pc.nextInstructionBranch(target);
          currentState = fetch;
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
            break;
          }
        }
        else if(opcode == 0b000001){
          // blt
          if(alu.operation(opcode,rsValue,rtValue) == 1){
            pc.nextInstructionBranch(immediate);
            currentState = fetch;
            break;
          }
        }
        else if(opcode == 0b000101){
          // bne
          if(alu.operation(opcode,rsValue,rtValue) == 1){
            pc.nextInstructionBranch(immediate);
            currentState = fetch;
            break;
          }
        }
        else if(opcode == 0b101011){
          // sw
          if(rs == 0b00001){
          }
        }
        else if(opcode == 0b100011){
          //lw
        }
      }
      currentState = memoryAccess;
      break;

    case memoryAccess:
      if (opcode == 0b101011 && rs != 0b00001){
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
      break;
  }
}
