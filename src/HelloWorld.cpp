#include <iostream>
#include "HelloWorld.hpp"

using namespace std;

//Lets implement the HelloWorld Object completely
HelloWorld::HelloWorld() {
    // This constructor does no initialization for HelloWorld Object.
}

void HelloWorld::setPrivateVariable(int val) {
    this->myPrivateVariable = val;
}

int HelloWorld::getPrivateVariable() {
    return this->myPrivateVariable;
}

void HelloWorld::printHelloWorld() {
    cout << "Hello World from HelloWorld Object" << endl;
}
