//this is one of my first projects modified to fit here    
#include<stdio.h>
#include<math.h>
#include"header_files/calc.h"

double calc() {
    double num1;
    double num2;
    char op;

    printf("Enter the first number: ");
    scanf("%lf", &num1);

    printf("Enter the operator: ");
    scanf(" %c", &op);

    printf("Enter the second number: ");
    scanf("%lf", &num2);

    if(op == '+') {
        printf("%f", num1 + num2);
        return num1 + num2;
    }
    else if(op == '-') {
        printf("%f", num1 - num2);
        return num1 - num2;
    }
    else if(op == '*') {
        printf("%.5f", num1 * num2);
        return num1 * num2;
    }
    else if(op == '/') {
        if(num2 == 0){printf("Cannot divide by zero");}
        else{   
        printf("%.5f", num1 / num2);
        return num1 / num2;}
    }
    else{printf("invalid operator");}
}