#ifndef MENU_H
#define MENU_H

#define PROMPT '>'

extern const char* menuHeadStart;
extern const char* menuHeadMap;
extern const char* menuHeadSim;

extern const char* menuStart[];
extern const char* menuMap[];
extern const char* menuSim[];

void menuPrint(const char* menu[]);
int menuInputOpt(int a, int b);
void pausa();

#endif

