void Person_Init();
void Eat();
void Sleep();
void Play_With_Doudou();
void Friend_Waiting();
void Keep_And_Clean();
void Friend_Talking();

int main()
{
    Person_Init();
    while(1)
    {
        Eat();
        Sleep();
        Play_With_Doudou();
    }
}

void Interrupt()
{
    Keep_And_Clean();
    Friend_Talking();
}