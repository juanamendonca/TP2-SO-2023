#include <stdbool.h>
#include <user_syscalls.h>
#include <functions.h>
#include "getInforegs.h"

extern void testRegs();

void getRegInfo() {
    uint64_t registers[18];
    bool isSaved;
    sys_registers(registers, &isSaved);
    if(!isSaved){
        print("Registers not saved. Press 'CTRL' to save registers status.");
		enter();
    }
    else{
        print("-- REGISTER VALUES --\n");
        enter();
        char *registersO[] = {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "R8 ", "R9 " , "R10", "R11", "R12", "R13", "R14", "R15"};
        for (int i = 0; i < 15; i++) {
        print(registersO[i]);
        print(": 0x");
        printHexa(registers[i]);
		enter();
    }
    }
    
    
}

void regsTester(){
    print("Registers should have the next values");
	enter();
	print("RAX:2");
	enter();
	print("RBX:2");
	enter();
	print("RCX:2");
	enter();
	print("RDX:6");
	enter();
	print("RSI:8");
	enter();
	print("RDI:1");
	enter();
	print("RBP:1");
	enter();
	print("R8:AN HEXA NUMBER");
	enter();
	print("R9:9");
	enter();
	print("R10:A");
	enter();
	print("R11:B");
	enter();
	print("R12:C");
	enter();
	print("R13:D");
	enter();
	print("R14:E");
	enter();
	print("R15:F");
	enter();
	print("Press 'CTRL' now");
	enter();
	testRegs();
	enter();
}