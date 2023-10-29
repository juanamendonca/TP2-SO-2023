#include "exceptionTester.h"
extern void divzerotester();
extern void opcodetester();

void divisionTester(){
    enter();
    print("Dividing by zero..");
    enter();
	divzerotester();
}

void invalidOpTester(){
    enter();
    print("Invalid Operation..");
    enter();
	opcodetester();
}