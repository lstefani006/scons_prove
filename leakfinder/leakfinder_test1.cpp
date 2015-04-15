#include<iostream>
#include<stdlib.h>

class my_class_a
{
private:
    int* leak;
    int* no_leak;
public:
    my_class_a()
    {
        leak = new int;
        no_leak = new int;
    }

    ~my_class_a()
    {
        delete [] no_leak;
    }
};

class my_class_b
{
private:
    my_class_a a_instance;
public:
    void foo()
    {
        int* leak = new int; 
		leak[0] = 0;
    }

    void bar()
    {
        foo();
    }

    void foobar()
    {
       bar();
    }
};

void cpp_function()
{
    char* leak = new char; 
	leak[0] = 1;
}

extern "C" void c_function()
{
    cpp_function();
}

void leo() {
	char *pp = NULL;
	//pp = (char*)malloc(100);
	pp = (char *)realloc(pp, 100);
	//free(pp);
	//pp = (char*)calloc(100, 1);
}

int main(void)
{
    std::cout << "leakfinder C++ thread example app" << std::endl;
    std::cout << "This application is expected to leak" << std::endl;
    my_class_b b;
    b.foobar();
    b.foo();


	for (int i = 0; i < 100; ++i)
		leo();

    c_function();

    std::cout << "leakfinder C++ thread example app all done" << std::endl;
    return 0;
}

