#ifndef AIRCRAFTFUNC_H
#define AIRCRAFTFUNC_H  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <windows.h>


typedef struct
{
    int id;
    int hit;
    int pos_x;
    int pos_y;
    int existance;
    int rationality;
    float properties[10];
    double x;
    double y;
    double spd_x;
    double spd_y;
    double a_x;
    double a_y;
    time_t start_time;
} object;

typedef struct
{
    int processing;
    int score_A;
    int score_B;
    int easterA;
    int easterB;
    int missileA;
    int missileB;
    time_t start_time;
    time_t end_time;
    time_t game_time;
    time_t startup_time;
} Utinfo;

void startup(void); // Initialize the game
void Pinit(void);
void Binit(void);
void Interfaceinit(void);
void UInfoinit(void);
void Missileinit(void);
void canvas(int if_waste); // Generate the game canvas
void Wcanvas(void);        // Generate a blank canvas
void updateWithoutInput(void);
void updateWithInput(void);
void curgoto(int x, int y); // Move the cursor
void HideCursor(int state);
void drawA(int x, int y);         // Draw plane A
void coverA(int x, int y);        // Cover plane A
void drawB(int x, int y);         // Draw plane B
void coverB(int x, int y);        // Cover plane A
void BulletInitialize(object *p); // Initialize a general bullet
void fire_general(object *p);     // Fire bullet general method
void fire_general(object *p);     // Fire bullet general method
void BulletReset(object *p);      // Reset the bullet
void drawBullet(int x, int y);    // Draw bullet
void coverBullet(int x, int y);   // Cover bullet
void Frame_start(void);           // Do things before each frame
void terminate(void);             // Terminate the game
void array_shift(int *p, int n);
void FireControl(int ifeaster, char user); // Control the fire button
void SpeedSync(void);
void Afire_easter(void);                    // Fire bullet easter method
void Bfire_easter(void);                    // Fire bullet easter method
void BulletCrash(object **p, int n);        // Detect the contacts between bullets
void PlaneActionControl(object *p);

int BulletActionControl_general(object *p); // Control the general bullet actions and return if_hit
int ifBelongsToUser(char letter, char user);
int comparison(char *p1, int *p2, int n);
int VarifyPostion_aircraft(int x, int y);
int VarifyPostion_bullet(int x, int y);
int ReactionDetect(int Bx, int By, int Px, int Py, char plane); // Detect the contacts between plane and bullet
double TimeDifference(time_t start, time_t end);                // caculate the difference (unit is second)

extern const int HIGHT, WIDTH;
extern const int NULLX, NULLY;

extern int easter_egg_recA[10], easter_egg_recB[10];
extern char easter_eggA[11], easter_eggB[11];

extern object A;
extern object B;
extern object Ablt;
extern object Bblt;
extern object AEb[10];
extern object BEb[10];
extern object *Projectile[22];
extern object MissileA;
extern object MissileB;
extern Utinfo info;

extern float plane_speed;

#endif
