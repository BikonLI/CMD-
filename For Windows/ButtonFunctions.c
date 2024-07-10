#include "ButtonFunctions.h"
#include "PropertiesCompute.h"

object Lose;
object Win;

void *funcw(void)
{
    array_shift(easter_egg_recA, 10);
    easter_egg_recA[9] = 'W';
}

void *funcs(void)
{
    array_shift(easter_egg_recA, 10);
    easter_egg_recA[9] = 'S';
}

void *funca(void)
{
    A.spd_y = -plane_speed;
    array_shift(easter_egg_recA, 10);
    easter_egg_recA[9] = 'A';
}

void *funca_r(void)
{
    A.spd_y = 0;
}

void *funcd(void)
{
    A.spd_y = plane_speed;
    array_shift(easter_egg_recA, 10);
    easter_egg_recA[9] = 'D';
}

void *funcd_r(void)
{
    A.spd_y = 0;
}

void *funcf(void)
{

    array_shift(easter_egg_recA, 10);
    easter_egg_recA[9] = 'F';

    FireControl(info.easterA, 'A');
}

void *funcr(void)
{
    array_shift(easter_egg_recA, 10);
    easter_egg_recA[9] = 'R';

    FireMissile(&MissileA);
}

void *funcu(void)
{
    array_shift(easter_egg_recB, 10);
    easter_egg_recB[9] = 'U';
}

void *funcj(void)
{
    array_shift(easter_egg_recB, 10);
    easter_egg_recB[9] = 'J';
}

void *funch(void)
{
    B.spd_y = -plane_speed;
    array_shift(easter_egg_recB, 10);
    easter_egg_recB[9] = 'H';
}

void *funch_r(void)
{
    B.spd_y = 0;
}

void *funck(void)
{
    B.spd_y = plane_speed;
    array_shift(easter_egg_recB, 10);
    easter_egg_recB[9] = 'K';
}

void *funck_r(void)
{
    B.spd_y = 0;
}

void *funcl(void)
{
    array_shift(easter_egg_recB, 10);
    easter_egg_recB[9] = 'L';
    FireControl(info.easterB, 'B');
}

void *funco(void)
{
    array_shift(easter_egg_recB, 10);
    easter_egg_recB[9] = 'O';
    FireMissile(&MissileB);
}

void *funcesc(void)
{
    terminate();
}

void drawBlood(object *p)
{
    int i;
    float count;
    count = p->properties[3] / 5;

    if (count <= 0)
    {
        count = 0;
        p->properties[3] = 0;
    }

    if (p->id == 'A')
    {
        curgoto(0, WIDTH + 2);
        printf("BLOOD");
        curgoto(1, WIDTH + 2);
        printf("|");
        printf("####################|");
    }
    else
    {
        curgoto(HIGHT - 1, WIDTH + 2);
        printf("BLOOD");
        curgoto(HIGHT - 2, WIDTH + 2);
        printf("|");
        printf("####################|");
    }

    for (i = 0; i < (20 - count); i++)
    {
        if (p->id == 'A')
        {
            curgoto(1, WIDTH + 22 - i);
            printf(" ");
        }
        else
        {
            curgoto(HIGHT - 2, WIDTH + 22 - i);
            printf(" ");
        }
    }
}

void drawWin(object *p)
{
    curgoto(p->pos_x, p->pos_y);
    printf("YOU WIN!");
}

void drawLose(object *p)
{
    curgoto(p->pos_x, p->pos_y);
    printf("YOU LOSE!");
}

void coverWin(object *p)
{
    curgoto(p->properties[0], p->properties[1]);
    printf("        ");
}

void coverLose(object *p)
{
    curgoto(p->properties[0], p->properties[1]);
    printf("         ");
}

