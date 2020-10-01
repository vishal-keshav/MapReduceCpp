#pragma once


/*
 * helloWorld Object allows to set and get an int type variable.
 * This object also prints hello world.
 * 
 * Object oriented thinking require us to think what functionality an
 * object will have and what attributes it will contains. This way, we
 * can decouple the development work.
 */
class HelloWorld {
public:
    HelloWorld(void); // This is just constructor declaration
    void setPrivateVariable(int value);
    int getPrivateVariable();
    void printHelloWorld();

private:
    int myPrivateVariable;
};
