#include <iostream>
using namespace std;


class MainMemory{
public:
  static unsigned int memory[150]; // 4 GB of memory (2^32 x 8 bits)

  int writeTo(int address, int value); // Write to memory
  int readFrom(int address); // Read from memory
};

//------------------------------             ------------------------------

unsigned int MainMemory::memory[150] = {0};

int MainMemory::writeTo(int address, int value){
  memory[address] = value;
}

int MainMemory::readFrom(int address){
  return memory[address];
}
