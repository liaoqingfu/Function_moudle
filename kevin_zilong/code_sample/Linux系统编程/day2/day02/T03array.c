
int main()
{
    int a = 10;
    int* b = alloca(a*sizeof(int));

    b[0] = 1;

}
