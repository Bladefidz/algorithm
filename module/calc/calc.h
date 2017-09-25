#ifndef CALC_H_  // Include only once
#define CALC_H_  // Guard
#define NUMBER '0'

void push(double);
double pop(void);
int getop(char[]);
int getch(void);
void ungetch(int);

#endif