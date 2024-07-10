#ifndef PROPERTIESCOMPUTE
#define PROPERTIESCOMPUTE
#include "aircraftfunc.h"


void SpeedControl(object *p);
void PosControl(object *p);
double IntegratingUnit();
void MissileSpeedControl(object *p);
void drawMissile(object *p);
void coverMissile(object *p);
int MissileActionControl_general(object *p);
int VarifyPostion_missile(object *p);
void FireMissile(object *p);
void MissileReset(object *p);

#endif
