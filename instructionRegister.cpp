#include <iostream>
#include <string>
//#include "programCounter.cpp"


using namespace std;


class InstructionRegister{
public:
  MainMemory memory;
  ProgramCounter pc;
  InstructionRegister();

  void decodeOpcode(int bits);
  void decodeOthers(int bits);
  void getInstruction(int instruction);

  string classifyInstruction(); // Classify what instruction it is
  string classifyRegister(int r);

  int returnOpcode();
  int returnRs();
  int returnRt();
  int returnRd();
  int returnShamt();
  int returnFunct();
  int returnImmediate();
  int returnTarget();
  int returnType();

private:
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

  int type; // 0 - R, 1 - J, 2 - I

  string ins;
  string reg;
};

//------------------------------             ------------------------------

InstructionRegister::InstructionRegister():pc(),memory(){}

void InstructionRegister::getInstruction(int instruction){
  //int instruction = memory.readFrom(pc.returnPC());
  decodeOpcode(instruction);
  decodeOthers(instruction);
}

void InstructionRegister::decodeOpcode(int bits){
  opcode = (bits >> 26) & 0b111111;
}

void InstructionRegister::decodeOthers(int bits){
  if (opcode == 0b000000){
    type = 0; // R type
    rs = (bits >> 21) & 0b00000011111;
    rt = (bits >> 16) & 0b0000000000011111;
    rd = (bits >> 11) & 0b000000000000000011111;
    shamt = (bits >> 6) & 0b00000000000000000000011111;
    funct = (bits >> 0) & 0b00000000000000000000000000111111;
  }
  else if (opcode == 0b000010 || opcode == 0b000011){
    type = 1; // J type
    target = (bits) & 0b00000011111111111111111111111111;
  }
  else{
    type = 2; // I type

    rs = (bits >> 21) & 0b00000011111;
    rt = (bits >> 16) & 0b0000000000011111;
    immediate = (bits >> 0) & 0b00000000000000001111111111111111;
  }
}

string InstructionRegister::classifyInstruction(){
  if(type == 0){
    // R
    string ins, rsString, rtString, rdString;
    if(funct == 0b100000){
      // add
      rsString = classifyRegister(rs);
      rtString = classifyRegister(rt);
      rdString = classifyRegister(rd);
      ins = "add " + rdString + "," + rsString + "," + rtString;
      return ins;
    }
    else if(funct == 0b000010){
      // srl
      rtString = classifyRegister(rt);
      rdString = classifyRegister(rd);
      ins = "srl " + rdString + "," + rtString + "," + to_string(shamt);
      return ins;
    }
    else if(funct == 0b000000){
      rtString = classifyRegister(rt);
      rdString = classifyRegister(rd);
      ins = "sll " + rdString + "," + rtString + "," + to_string(shamt);
      return ins;
    }
    else if(funct == 0b001000){
      rsString = classifyRegister(rs);
      ins = "jr " + rsString;
      return ins;
    }
  }
  else if(type == 1){
    // J
    if(opcode == 0b000011){
      ins = "jal Square";
      return ins;
    }
    else if(opcode == 0b000010){
      ins = "j Main";
      return ins;
    }
  }
  else if(type == 2){
    // I
    string rsString, rtString;
    if(opcode == 0b001000){
      rsString = classifyRegister(rs);
      rtString = classifyRegister(rt);
      ins = "addi " + rtString + "," + rsString + "," + to_string(immediate);
      return ins;
    }
    else if(opcode == 0b001100){
      rsString = classifyRegister(rs);
      rtString = classifyRegister(rt);
      ins = "andi " + rtString + "," + rsString + "," + to_string(immediate);
      return ins;
    }
    else if(opcode == 0b100011){
      ins = "lw $s4,$ra";
      return ins;
    }
    else if(opcode == 0b101011){
      rsString = classifyRegister(rs);
      rtString = classifyRegister(rt);
      if (rs == 0b00001){
        ins = "sw $s4,$ra";
      }
      else{
        ins = "sw " + rtString + "," + "50(" + rsString + ")";
      }
      return ins;
    }
    else if(opcode == 0b000100){
      rsString = classifyRegister(rs);
      rtString = classifyRegister(rt);
      ins = "beq " + rsString +"," + rtString + ", odd";
      return ins;
    }
    else if(opcode == 0b000001){
      rsString = classifyRegister(rs);
      rtString = classifyRegister(rt);
      ins = "blt " + rsString + "," + rtString + ", lessThanTwo" ;
      return ins;
    }
    else if(opcode == 0b000101){
      rsString = classifyRegister(rs);
      rtString = classifyRegister(rt);
      ins = "bne " + rsString + "," + rtString + ", continue";
      return ins;
    }
  }
}

string InstructionRegister::classifyRegister(int r){
  string reg;
  if(r == 0){
    reg = "$zero";
  }
  else if(r == 1){
    reg = "$at";
  }
  else if(r == 2){
    reg = "$v0";
  }
  else if(r == 3){
    reg = "$v1";
  }
  else if(r == 4){
    reg = "$a0";
  }
  else if(r == 5){
    reg = "$a1";
  }
  else if(r == 6){
    reg = "$a2";
  }
  else if(r == 7){
    reg = "$a3";
  }
  else if(r == 8){
    reg = "$t0";
  }
  else if(r == 9){
    reg = "$t1";
  }
  else if(r == 10){
    reg = "$t2";
  }
  else if(r == 11){
    reg = "$t3";
  }
  else if(r == 12){
    reg = "$t4";
  }
  else if(r == 13){
    reg = "$t5";
  }
  else if(r == 14){
    reg = "$t6";
  }
  else if(r == 15){
    reg = "$t7";
  }
  else if(r == 16){
    reg = "$s0";
  }
  else if(r == 17){
    reg = "$s1";
  }
  else if(r == 18){
    reg = "$s2";
  }
  else if(r == 19){
    reg = "$s3";
  }
  else if(r == 20){
    reg = "$s4";
  }
  else if(r == 21){
    reg = "$s5";
  }
  else if(r == 22){
    reg = "$s6";
  }
  else if(r == 23){
    reg = "$s7";
  }
  else if(r == 24){
    reg = "$t8";
  }
  else if(r == 25){
    reg = "$t9";
  }
  else if(r == 26){
    reg = "$k0";
  }
  else if(r == 27){
    reg = "$k1";
  }
  else if(r == 28){
    reg = "$gp";
  }
  else if(r == 29){
    reg = "$sp";
  }
  else if(r == 30){
    reg = "$fp";
  }
  else if(r == 31){
    reg = "$ra";
  }
  return reg;
}

int InstructionRegister::returnOpcode(){
  return opcode;
}

int InstructionRegister::returnRs(){
  return rs;
}

int InstructionRegister::returnRt(){
  return rt;
}

int InstructionRegister::returnRd(){
  return rd;
}

int InstructionRegister::returnShamt(){
  return shamt;
}

int InstructionRegister::returnFunct(){
  return funct;
}

int InstructionRegister::returnImmediate(){
  return immediate;
}

int InstructionRegister::returnTarget(){
  return target;
}

int InstructionRegister::returnType(){
  return type;
}
