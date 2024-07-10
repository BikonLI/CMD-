#ifndef BUTTONFUNCTIONS_H
#define BUTTONFUNCTIONS_H

#include "KBM.h"
#include "aircraftfunc.h"
// #include "PropertiesCompute.h"

void* funca(void);
void* funcs(void);
void* funcd(void);
void* funcw(void);
void* funcf(void);
void* funcr(void);
void* funcu(void);
void* funcj(void);
void* funch(void);
void* funck(void);
void* funcl(void);
void* funco(void);
void* funcesc(void);

void* funca_r(void);
void* funcd_r(void);
void* funch_r(void);
void* funck_r(void);

void drawBlood(object *p);
void drawWin(object *p);
void drawLose(object *p);
void coverWin(object *p);
void coverLose(object *p);

extern object Win;
extern object Lose;

#endif