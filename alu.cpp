#include <iostream>

using namespace std;

class Alu{
public:
  int operation(int instruction, int para1, int para2);
private:
  int add(int value1, int value2);
  int sll(int value, int amount);
  int srl(int value, int amount);
  int andi(int value1, int value2);
  int beq(int value1, int value2);
  int blt(int value1, int value2);
  int bne(int value1, int value2);
};

int Alu::operation(int instruction, int para1, int para2){
  if(instruction == 0b100000 || instruction == 0b001000){
    // add, addi
    return add(para1, para2);
  }
  else if(instruction == 0b000010){
    // srl
    return srl(para1, para2);
  }
  else if(instruction == 0b000000){
    // sll
    return sll(para1, para2);
  }
  else if(instruction == 0b001100){
    // andi
    return andi(para1, para2);
  }
  else if(instruction == 0b000100){
    // beq
    return beq(para1, para2);
  }
  else if(instruction == 0b000001){
    // blt
    return blt(para1, para2);
  }
  else if(instruction == 0b101011){
    // sw
    return add(para1, para2); // this calculates immediate($rs)
  }
  else if(instruction == 0b000101){
    // bne
    return bne(para1, para2);
  }
}

int Alu::add(int value1, int value2){
  return (value1 + value2);
}

int Alu::sll(int value, int amount){
  return (value << amount);
}

int Alu::srl(int value, int amount){
  return (value >> amount);
}

int Alu::andi(int value1, int value2){
  return(value1 & value2);
}

int Alu::beq(int value1, int value2){
  if(value1 == value2){
    return 1;
  }
  else{
    return 0;
  }
}

int Alu::blt(int value1, int value2){
  if(value1 < value2){
    return 1;
  }
  else{
    return 0;
  }
}

int Alu::bne(int value1,int value2){
  if(value1 != value2){
    return 1;
  }
  else{
    return 0;
  }
}
