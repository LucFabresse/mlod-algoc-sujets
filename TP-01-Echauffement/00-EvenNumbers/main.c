#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

bool isEven(int i) { return i%2==0; }

int main (void)
{
   for(int i=0;i<11;i++)
	   printf("%i est %s\n",i,isEven(i)? "pair" : "impair");

	return EXIT_SUCCESS;
}