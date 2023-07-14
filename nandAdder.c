#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <string.h>

int nand(int a, int b)
{
    int res = 1;

    if( a == 1 && b == 1 )
    {
        res = 0;
    }

    return res;
}

int not(int a)
{
    int res = 1;
    res = nand(a, a);

    return res;
}

int and(int a, int b)
{
    int res = 0;

    int nandResult = nand(a, b);
    res = not(nandResult);
    return res;

}

int or(int a, int b)
{
    
     int res = 0;

    res = (nand(not(a), not(b)));

    return res;
}

int xor(int a, int b)
{

    int res = 0;

    int not_a = nand(a, a);
    int not_b = nand(b, b);
    int and1 = nand(a, not_b);
    int and2 = nand(not_a, b);

    res = nand(and1, and2);
    return res;
}

int* halfadder(int* data, int* res)
{
    res[0] = 0;
    res[1] = 0;
    
    int a = data[0];
    int b = data[1];
    
    res[0] = xor(a, b);
    res[1] = and(a, b);
    
    return res;
}

int* fulladder(int* data, int* finalRes)
{
    finalRes[0] = 0; //result of add
    finalRes[1] = 0; //carry out

    int A = data[0];
    int B = data[1];
    int Cin = data[2];
        
        //column sum
    int firstData[2] = {A, B}; //add digits
    int firstRes[2] = {0, 0}; //result & carry
    
    halfadder(firstData, firstRes);

        //result and carryin sum
    int secondData[2] = {firstRes[0], Cin}; //add sum and carry in
    int secondRes[2] = {0,0}; //overall result & carry

    halfadder(secondData, secondRes);

        // sum of column bits and carry in, if any
    finalRes[0] = secondRes[0];

        // carry out only if either column sum or
        // result & carry in sum caused a carry
    finalRes[1] = or(firstRes[1], secondRes[1]);
    
    return finalRes;
}

int main( int argc, char* argv[])
{
    if( argc != 4)
    {
        printf("Fatal Error: expected 3 arguments, found: %d\n", argc-1);
        printf(" Usage: ./nandAdder -d 1 0\n");
        printf(" Usage: ./nandAdder 1 0 1\n");

    return -1;
    }

    int A = 0;
    int B = 0;
    int Cin = 0;

    if( strncmp( argv[1], "-d", 2) == 0 )
    {
        printf("****************Diagnostic mode*****************\n");

        A = atoi(argv[2]);
        B = atoi(argv[3]);
        Cin = -99;

        printf("Initialized with bits:\n A: %d B: %d \n\n", A, B);
        printf("Computed\t\tActual\n");
        printf("not A: %d\t\t%d\n", not(A),!A);
        printf("not B: %d\t\t%d\n", not(B), !B);
        printf("A and B: %d\t\t%d\n", and(A, B), (A && B));
        printf("A or B: %d\t\t%d\n", or(A, B), (A || B));
        printf("A xor B: %d\t\t%d\n", xor(A, B), ((!A && B) || (A && !B)) );
        printf("************************************************\n");
    }
    else
    {
        printf("================Compute mode====================\n");
        A = atoi(argv[1]);
        B = atoi(argv[2]);
        Cin = atoi(argv[3]);
        printf("Initialized with bits:\n A: %d B: %d Cin: %d\n\n", A, B, Cin);
        
        int data[3] = {A, B, Cin};
        int res[2] = {0, 0};

        fulladder(data, res);
        printf("Computed Result:\n bit: %d\n overflow: %d\n", res[0], res[1]);
        printf("================================================\n");
    }
    
    return 0;
}