#include <iostream>
#include "../programCounter.cpp"
/* This file was used to test the Program Counter module separate from the complete simulator.
   For this to execute, the *comment* to include mainMemory.cpp in programCounter.cpp will
   need to be removed. ie. *Do* include mainMemory.cpp in programCounter.cpp
*/

using namespace std;

int main(){
  ProgramCounter pc;

  cout << "Testing PC increment\n";

  cout << pc.returnPC() << endl; // Expect 0
  pc.nextInstruction();
  cout << pc.returnPC() << endl; // Expect 4
  pc.nextInstructionBranch(5);
  cout << pc.returnPC() << endl; // Expect 9
  pc.nextInstruction();
  cout << pc.returnPC() << endl << endl; // Expect 13

  cout << "Testing with memory\n";

  MainMemory memory;
  pc.nextInstructionBranch(13);
  cout << "Address: 13\n";
  cout << memory.readFrom(pc.returnPC()) << endl; // Expect 0

  memory.writeTo(13, 0x00000032);
  cout << memory.readFrom(pc.returnPC()) << endl; // Expect 50

  pc.nextInstruction();
  cout << "Address: 17\n";
  cout << memory.readFrom(pc.returnPC()) << endl;
}
