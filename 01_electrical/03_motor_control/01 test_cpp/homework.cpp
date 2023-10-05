#include <stdio.h>

class cube
{
public:
	int length;
	int width;
	int height;
	
	void Init(int __length, int __width, int __height){ length = __length; width = __width; height = __height; }
	void Calculate_Area();
};

void cube::Calculate_Area()
{
	printf("%d\n", 2 * (length * width + length * height + width * height));
}

class cube_with_volume : public cube
{
public:
	int volume;
	void Calculate_Volume();
};

void cube_with_volume::Calculate_Volume()
{
	volume = length * width * height;
	printf("%d\n", volume);
}

int main()
{
	cube test1;
	test1.Init(1, 1, 1);
	test1.Calculate_Area(); 
	
	cube_with_volume test2;
	test2.Init(1, 2, 3);
	test2.Calculate_Volume();
}
