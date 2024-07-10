#include "aircraftfunc.h"
#include "PropertiesCompute.h"

const int HIGHT = 39, WIDTH = 80;
const int NULLX = 50, NULLY = 90;

int easter_egg_recA[10], easter_egg_recB[10];
char easter_eggA[] = "WWSSADADFR", easter_eggB[] = "UUJJHKHKLO";

float plane_speed = 8.0;

object A;
object B;
object Ablt;
object Bblt;
object AEb[10];
object BEb[10];
object *Projectile[22];
object MissileA;
object MissileB;
Utinfo info;

void BulletInitialize(object *p)
{
    p->existance = 0;
    p->rationality = 0;
    p->pos_x = NULLX;
    p->pos_y = NULLY;
    p->properties[3] = 5;
    if (p->id > 0)
    {
        p->spd_x = 3.0;
        p->a_x = 0.1;
    }
    else
    {
        p->spd_x = -3.0;
        p->a_x = -0.1;
    }
}

int ifBelongsToUser(char letter, char user)
{
    int i, result;
    char array[11];
    if (user == 'A')
        strcpy(array, easter_eggA);
    if (user == 'B')
        strcpy(array, easter_eggB);
    for (i = 0; i < 10; i++)
    {
        result = (array[i] == letter);
        if (result)
            break;
    }

    return result;
}

void array_shift(int *p, int n)
{
    int i;
    for (i = 1; i < n; i++)
        p[i - 1] = p[i];
}

double TimeDifference(time_t start, time_t end)
{
    double result;
    result = (double)(end - start) / CLOCKS_PER_SEC;
    return result;
}

int comparison(char *p1, int *p2, int n)
{
    int i, result;
    for (i = 0; i < n; i++)
    {
        result = (p1[i] == p2[i]);
        if (!result)
            break;
    }
    return result;
}

void Pinit(void)
{
    A.id = (int)('A');
    A.existance = 1;
    A.rationality = 1;
    A.pos_x = 5;
    A.pos_y = WIDTH / 2;
    A.x = 5;
    A.y = (int)(WIDTH / 2);
    A.spd_y = 0;
    A.a_y = 0.1;
    A.properties[3] = 100;
    A.properties[0] = NULLX;
    A.properties[1] = NULLY;

    B.id = (int)('B');
    B.existance = 1;
    B.rationality = 1;
    B.pos_x = 32;
    B.pos_y = WIDTH / 2;
    B.x = 32;
    B.y = (int)(WIDTH / 2);
    B.spd_y = 0;
    B.a_y = 0.1;
    B.properties[3] = 100;
    B.properties[0] = NULLX;
    B.properties[1] = NULLY;
}

void Binit(void)
{
    int n;
    Ablt.id = 1;
    Bblt.id = -1;
    BulletInitialize(&Ablt);
    BulletInitialize(&Bblt);
    Projectile[0] = &Ablt;
    Projectile[1] = &Bblt;

    for (n = 0; n < 10; n++)
    {
        AEb[n].id = n + 2;
        BEb[n].id = -(n + 2);
        BulletInitialize(&AEb[n]);
        BulletInitialize(&BEb[n]);
        Projectile[n + 2] = &AEb[n];
        Projectile[n + 12] = &BEb[n];
    }
}

void Interfaceinit(void)
{
    int n, input = 0;
    for (n = 5; n >= 0; n--)
    {
        Sleep(1000);
        printf("\rPlease switch to full screen in %d seconds...", n);
    }

    system("cls");
    Wcanvas();
    curgoto(20, 10);
    printf("Press any key to start...");
    while (input == 0)
        input = getch();
    system("cls");
    HideCursor(1);
    canvas(1);
}

void UInfoinit(void)
{
    info.easterA = 0;
    info.easterB = 0;
    info.missileA = 0;
    info.missileB = 0;
    info.processing = 1;
    info.end_time = clock();
}

void Missileinit(void)
{
    MissileA.existance = 0;
    MissileA.rationality = 0;
    MissileA.pos_x = NULLX;
    MissileA.pos_y = NULLY;
    MissileA.id = 12;
    MissileA.a_x = -0.6;
    MissileA.a_y = 0;
    MissileA.spd_x = 10;
    MissileA.spd_y = 0;
    MissileA.properties[3] = 50;
    MissileA.properties[0] = NULLX;
    MissileA.properties[1] = NULLY;

    MissileB.existance = 0;
    MissileB.rationality = 0;
    MissileB.pos_x = NULLX;
    MissileB.pos_y = NULLY;
    MissileB.id = -12;
    MissileB.a_x = -0.6;
    MissileB.a_y = 0;
    MissileB.spd_x = -10;
    MissileB.spd_y = 0;
    MissileB.properties[3] = 50;
    MissileB.properties[0] = NULLX;
    MissileB.properties[1] = NULLY;
}

void terminate(void)
{
    info.processing = 0;
}

int VarifyPostion_aircraft(int x, int y)
{
    if (x < HIGHT && y < WIDTH - 2 && x > 4 && y > 4)
        return 1;
    else
        return 0;
}

int VarifyPostion_bullet(int x, int y)
{
    if (x < HIGHT && y < WIDTH - 2 && x > -1 && y > 0)
        return 1;
    else
        return 0;
}

void canvas(int if_waste)
{
    curgoto(0, 0);
    int i, j;

    if (if_waste)
    {
        for (i = 0; i < HIGHT; i++)
        {
            for (j = 0; j < WIDTH + 2; j++)
            {
                curgoto(i, j);
                if (j == 0 || j == WIDTH + 1)
                {
                    printf("|");
                    Sleep(1);
                }
            }
        }
    }

    for (i = 0; i < HIGHT; i++)
    {
        curgoto(i, 0);
        printf("|");
        curgoto(i, WIDTH + 1);
        printf("|");
    }
    curgoto(0, 0);
}

void Wcanvas(void)
{
    curgoto(0, 0);
    int i, j;
    for (i = 0; i < NULLX + 20; i++)
    {
        for (j = 0; j < NULLY + 20; j++)
            printf(" ");
        printf("\n");
    }
    curgoto(0, 0);
}

void curgoto(int x, int y)
{
    printf("\033[%dA", NULLX + 20);
    printf("\033[%dD", NULLY + 20); // Move cursor to (0, 0)
    printf("\033[%dB", x - 1);
    printf("\033[%dC", y - 1);
}

void HideCursor(int state)
{
    CONSOLE_CURSOR_INFO cursor_info;
    cursor_info.dwSize = 1;
    if (state)
        cursor_info.bVisible = 0;
    else
        cursor_info.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void drawA(int x, int y)
{
    curgoto(x, y);
    printf("*");
    curgoto(x, y - 1);
    printf("*");
    curgoto(x, y - 2);
    printf("*");
    curgoto(x, y - 3);
    printf("*");
    curgoto(x, y + 1);
    printf("*");
    curgoto(x, y + 2);
    printf("*");
    curgoto(x, y + 3);
    printf("*");
    curgoto(x - 2, y);
    printf("*");
    curgoto(x - 2, y - 1);
    printf("*");
    curgoto(x - 2, y + 1);
    printf("*");
    curgoto(x - 1, y);
    printf("*");
    curgoto(x + 1, y);
    printf("*");
}

void coverA(int x, int y)
{
    curgoto(x, y);
    printf(" ");
    curgoto(x, y - 1);
    printf(" ");
    curgoto(x, y - 2);
    printf(" ");
    curgoto(x, y - 3);
    printf(" ");
    curgoto(x, y + 1);
    printf(" ");
    curgoto(x, y + 2);
    printf(" ");
    curgoto(x, y + 3);
    printf(" ");
    curgoto(x - 2, y);
    printf(" ");
    curgoto(x - 2, y - 1);
    printf(" ");
    curgoto(x - 2, y + 1);
    printf(" ");
    curgoto(x - 1, y);
    printf(" ");
    curgoto(x + 1, y);
    printf(" ");
}

void drawB(int x, int y)
{
    curgoto(x, y);
    printf("*");
    curgoto(x, y - 1);
    printf("*");
    curgoto(x, y - 2);
    printf("*");
    curgoto(x, y - 3);
    printf("*");
    curgoto(x, y + 1);
    printf("*");
    curgoto(x, y + 2);
    printf("*");
    curgoto(x, y + 3);
    printf("*");
    curgoto(x + 2, y);
    printf("*");
    curgoto(x + 2, y - 1);
    printf("*");
    curgoto(x + 2, y + 1);
    printf("*");
    curgoto(x + 1, y);
    printf("*");
    curgoto(x - 1, y);
    printf("*");
}

void coverB(int x, int y)
{
    curgoto(x, y);
    printf(" ");
    curgoto(x, y - 1);
    printf(" ");
    curgoto(x, y - 2);
    printf(" ");
    curgoto(x, y - 3);
    printf(" ");
    curgoto(x, y + 1);
    printf(" ");
    curgoto(x, y + 2);
    printf(" ");
    curgoto(x, y + 3);
    printf(" ");
    curgoto(x + 2, y);
    printf(" ");
    curgoto(x + 2, y - 1);
    printf(" ");
    curgoto(x + 2, y + 1);
    printf(" ");
    curgoto(x + 1, y);
    printf(" ");
    curgoto(x - 1, y);
    printf(" ");
}

void drawBullet(int x, int y)
{
    curgoto(x, y);
    printf("|");
}

void coverBullet(int x, int y)
{
    curgoto(x, y);
    printf(" ");
}

void fire_general(object *p)
{
    p->existance = 1;
    p->rationality = 0;
    p->start_time = clock();
    if (p->id > 0)
    {
        p->pos_x = A.pos_x + 2;
        p->pos_y = A.pos_y;
    }
    else
    {
        p->pos_x = B.pos_x - 2;
        p->pos_y = B.pos_y;
    }
}

void BulletReset(object *p)
{
    p->existance = 0;
    p->rationality = 0;
    coverBullet(p->properties[0], p->pos_y);
    p->start_time = 0;
    p->pos_x = NULLX;
    p->pos_y = NULLY;
}

int BulletActionControl_general(object *p)
{
    double using_time, offset;
    int pos_x, if_hit = 0;
    info.game_time = clock();

    if (p->id > 0)
        pos_x = p->pos_x + 1;
    else
        pos_x = p->pos_x - 1;

    if (p->existance)
    {
        using_time = TimeDifference(p->start_time, info.game_time);
        offset = using_time * p->spd_x;

        if (fabs(offset) >= 1.0)
            pos_x += (int)(offset);

        p->rationality = VarifyPostion_bullet(pos_x, p->pos_y);
    }

    if (p->existance && p->rationality)
    {
        drawBullet(pos_x, p->pos_y);
        p->properties[0] = pos_x;

        if (p->id > 0)
            coverBullet(pos_x - 1, p->pos_y);
        else
            coverBullet(pos_x + 1, p->pos_y);
        if (p->id > 0)
            if_hit = ReactionDetect(pos_x, p->pos_y, B.pos_x, B.pos_y, 'B');
        else
            if_hit = ReactionDetect(pos_x, p->pos_y, A.pos_x, A.pos_y, 'A');
        if (if_hit)
        {
            BulletReset(p);
        }
    }
    else
        BulletReset(p);

    return if_hit;
}

int ReactionDetect(int Bx, int By, int Px, int Py, char plane)
{
    int index[2];
    if (plane == 'A')
        index[0] = Px - 2;
    if (plane == 'B')
        index[0] = Px - 1;
    index[1] = Py - 3;

    if ((Bx >= index[0]) && (Bx <= index[0] + 3) && (By >= index[1]) && (By <= index[1] + 6))
        return 1;
    else
        return 0;
}

void BulletCrash(object **p, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (
                p[i]->id > 0 && p[j]->id < 0 &&
                p[i]->existance && p[i]->rationality &&
                p[j]->existance && p[j]->rationality &&
                p[i]->pos_y == p[j]->pos_y)
            {
                if ((p[i]->properties[0] - 2) == p[j]->properties[0])
                {
                    BulletReset(p[i]);
                    BulletReset(p[j]);
                }
                else if ((p[i]->properties[0] - 1) == p[j]->properties[0])
                {
                    BulletReset(p[i]);
                    BulletReset(p[j]);
                }
            }
        }
    }
}

void PlaneActionControl(object *p)
{
    if (plane_speed <= 20)
        plane_speed += 0.0005;
    PosControl(p);
    p->rationality = VarifyPostion_aircraft(p->pos_x, p->pos_y);
    if (!p->rationality)
    {
        if (p->pos_y >= WIDTH - 2)
        {
            p->pos_y = WIDTH - 3;
            p->y = WIDTH - 3;
            p->rationality = 1;
        }
        else
        {
            p->pos_y = 5;
            p->y = 5;
            p->rationality = 1;
        }
    }
    if (fabs(p->pos_y - p->properties[1]) >= 1)
    {
        if (p->id == 'A')
        {
            coverA(p->properties[0], p->properties[1]);
            drawA(p->pos_x, p->pos_y);

        }
        else
        {
            coverB(p->properties[0], p->properties[1]);
            drawB(p->pos_x, p->pos_y);
        }
        p->properties[0] = p->pos_x;
        p->properties[1] = p->pos_y;
    }
}

void FireControl(int ifeaster, char user)
{
    if (user == 'A')
    {
        if (!ifeaster)
        {
            if (!Ablt.existance)
                fire_general(&Ablt);
        }
        else
            Afire_easter();
    }
    if (user == 'B')
    {
        if (!ifeaster)
        {
            if (!Bblt.existance)
                fire_general(&Bblt);
        }
        else
            Bfire_easter();
    }
}

void SpeedSync(void)
{
    if (Ablt.spd_x < 20.0)
    {
        int i;
        double time_difference = TimeDifference(info.start_time, info.end_time);
        double speed_offsetA = time_difference * Ablt.a_x;
        double speed_offsetB = time_difference * Bblt.a_x;

        Ablt.spd_x += speed_offsetA;
        Bblt.spd_x += speed_offsetB;

        for (i = 0; i < 10; i++)
        {
            AEb[i].spd_x += speed_offsetA;
            BEb[i].spd_x += speed_offsetB;
        }
    }
}

void Afire_easter(void)
{
    int i, allow_fire;
    for (i = 0; i < 10; i++)
    {
        allow_fire = !AEb[i].existance && !AEb[i].rationality;
        if (allow_fire)
        {
            fire_general(&AEb[i]);
            break;
        }
    }
}

void Bfire_easter(void)
{
    int i, allow_fire;
    for (i = 0; i < 10; i++)
    {
        allow_fire = !BEb[i].existance && !BEb[i].rationality;
        if (allow_fire)
        {
            fire_general(&BEb[i]);
            break;
        }
    }
}
