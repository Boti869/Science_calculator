//this is one of my first projects modified to fit here    
#include<stdio.h>
#include<math.h>
#include"../header_files/calc.h"

double calc() {
    double num1;
    double num2;
    char op;

    wprintf(L"Enter the first number: ");
    scanf("%lf", &num1);

    wprintf(L"Enter the operator: ");
    scanf(" %c", &op);

    wprintf(L"Enter the second number: ");
    scanf("%lf", &num2);

    if(op == '+') {
       wprintf(L"%f", num1 + num2);
        return num1 + num2;
    }
    else if(op == '-') {
        wprintf(L"%f", num1 - num2);
        return num1 - num2;
    }
    else if(op == '*') {
        wprintf(L"%.5f", num1 * num2);
        return num1 * num2;
    }
    else if(op == '/') {
        if(num2 == 0){wprintf(L"Cannot divide by zero");}
        else{   
        wprintf(L"%.5f", num1 / num2);
        return num1 / num2;}
    }
    else if(op == '^') {
        wprintf(L"%.5f", pow(num1, num2));
        return pow(num1, num2);
    }
    else{wprintf(L"invalid operator");}
}