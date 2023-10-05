#include <stdio.h>

void swap_c_fake(int a, int b)
{
	int c;
	c = a;
	a = b;
	b = c;
}

void swap_c(int *a, int *b)
{
	int c;
	c = *a;
	*a = *b;
	*b = c;
}

void swap_cpp(int &a, int &b)
{
	int c;
	c = a;
	a = b;
	b = c;
}

int main()
{
	int a = 1, b = 2;
	swap_cpp(a, b);
	printf("%d\n%d\n\n", a, b);
} 
