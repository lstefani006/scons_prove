#include<iostream>
#include<stdlib.h>

#include <leakfinder.h>

void test1() 
{
	LeakFinder d;
	malloc(123456);   // leak
}
void test2() 
{
	LeakFinder d;
	void *p = malloc(123456);   // leak
	free(p);
}

int main(void)
{
	test1();
	test2();
    return 0;
}

