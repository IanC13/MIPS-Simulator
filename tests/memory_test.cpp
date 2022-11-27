#include <iostream>
#include "../mainMemory.cpp"
/* This file was used to test the Main Memory module separate from the complete simulator.

   This test also shows that the Register File module is working as it is defined in a
   similar way as the Main Memory.
*/

using namespace std;

int main(){
  cout << "Memory Test\n";
  MainMemory memory;

  cout << "Reading with no values\n";

  cout << "Address: 0\n";
  cout << memory.readFrom(0) << endl;

  cout << "Address: 5\n";
  cout << memory.readFrom(5) << endl;

  cout << "Address: 31\n";
  cout << memory.readFrom(31) << endl << endl; // Expect all to be 0 as it is static


  cout << "Writing to Memory\n\n";

  cout << "Address: 0\n";
  memory.writeTo(0, 0x0000000F);
  cout << memory.readFrom(0) << endl; // Expect 15

  cout << "Address: 4\n";
  memory.writeTo(4, 0x00000032);
  cout << memory.readFrom(4) << endl; // Expect 50

  cout << "Address: 15\n";
  memory.writeTo(15, 32);
  cout << memory.readFrom(15) << endl; // Expect 32

  cout << "Address: 27\n";
  memory.writeTo(27, 0b00000010);
  cout << memory.readFrom(27) << endl; // Expect 2
}
