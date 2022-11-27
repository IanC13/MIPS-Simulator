#include <iostream>
//#include "instructionRegister.cpp"

using namespace std;

class RegisterFile{
public:
  static unsigned int registerFile[32];

  int writeTo(int reg, int value);
  int readFrom(int reg);
};

unsigned int RegisterFile::registerFile[32] = {0};

int RegisterFile::writeTo(int reg, int value){
  registerFile[reg] = value;
}

int RegisterFile::readFrom(int reg){
  return registerFile[reg];
}
