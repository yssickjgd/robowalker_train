// Type是基础数据类型, 比如int char double等 
template <typename Type>
void Math_Constrain(Type *x, Type Min, Type Max)
{
    if (*x < Min)
    {
        *x = Min;
    }
    else if (*x > Max)
    {
        *x = Max;
    }
}

void int_constrain(int *x, int Min, int Max)
{
    if (*x < Min)
    {
        *x = Min;
    }
    else if (*x > Max)
    {
        *x = Max;
    }
}

int main()
{
	int a = 2000;
	Math_Constrain(&a, 100, 200);
//	constrain(&a, 100, 200); 
	return(a);
} 
