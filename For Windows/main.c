#include "aircraftfunc.h"
#include "PropertiesCompute.h"
#include "KBM.h"
#include "ButtonFunctions.h"

void startup(void);
void Frame_start(void);
void updateWithInput(void);
void updateWithoutInput(void);

unsigned long long int times;

int main(void)
{
    times = 0;
    startup();
    while (info.processing)
    {
        times++;
        Frame_start();
        updateWithInput();
        updateWithoutInput();
    }
}

void startup(void)
{
    Pinit();
    Binit();
    Missileinit();
    Interfaceinit();
    UInfoinit();
    MonitorStart();
    drawBlood(&A);
    drawBlood(&B);
    info.startup_time = clock();
}

void Frame_start(void)
{
    info.start_time = info.end_time;
    info.end_time = clock();
    SpeedSync();
}

void updateWithInput(void)
{
    on_press(VK_A, funca);
    on_press(VK_W, funcw);
    on_press(VK_S, funcs);
    on_press(VK_D, funcd);
    on_press(VK_F, funcf);
    on_press(VK_R, funcr);
    on_press(VK_U, funcu);
    on_press(VK_J, funcj);
    on_press(VK_H, funch);
    on_press(VK_K, funck);
    on_press(VK_L, funcl);
    on_press(VK_O, funco);

    on_release(VK_A, funca_r);
    on_release(VK_D, funcd_r);
    on_release(VK_H, funch_r);
    on_release(VK_K, funck_r);

    on_press(VK_ESCAPE, funcesc);

    if (!info.easterA)
        info.easterA = comparison(easter_eggA, easter_egg_recA, 10);
    if (!info.easterB)
        info.easterB = comparison(easter_eggB, easter_egg_recB, 10);

    PlaneActionControl(&A);
    PlaneActionControl(&B);

    if (info.easterA || info.easterB)
    {
        // actions of easter egg.
    }
}

void updateWithoutInput(void)
{
    int hit_A_count = 0, hit_B_count = 0, missile_hit_A, missile_hit_B, i;

    BulletCrash(Projectile, 22);
    for (i = 0; i < 22; i++)
    {
        if (Projectile[i]->id > 0)
            hit_B_count += BulletActionControl_general(Projectile[i]);
        else
            hit_A_count += BulletActionControl_general(Projectile[i]);
    }

    missile_hit_B = MissileActionControl_general(&MissileA);
    missile_hit_A = MissileActionControl_general(&MissileB);

    if (hit_A_count || hit_B_count || missile_hit_B || missile_hit_A) // The operations afer hit
    {
        if (hit_A_count)
        {
            A.properties[3] -= 5;
        }
        if (hit_B_count)
        {
            B.properties[3] -= 5;
        }
        if (missile_hit_A)
        {
            A.properties[3] -= 50;
        }
        if (missile_hit_B)
        {
            B.properties[3] -= 50;
        }

        coverA(A.pos_x, A.pos_y);
        coverB(B.pos_x, B.pos_y);
        drawA(A.pos_x, A.pos_y);
        drawB(B.pos_x, B.pos_y);

        drawBlood(&A);
        drawBlood(&B);

        if (A.properties[3] == 0 || B.properties[3] == 0)
        {
            Sleep(500);
            Wcanvas();
            Sleep(500);
            curgoto(HIGHT / 2, WIDTH / 2);
            if (A.properties[3] == 0)
            {
                Win.a_y = 8.0;
                Win.spd_y = -20;
                Win.pos_x = 3 * HIGHT / 4;
                Win.pos_y = WIDTH + 30;
                Win.x = 3 * HIGHT / 4;
                Win.y = WIDTH + 40;
                Win.properties[0] = NULLX;
                Win.properties[1] = NULLY;
                Lose.a_y = -8.0;
                Lose.spd_y = 20;
                Lose.pos_x = HIGHT / 4;
                Lose.pos_y = 0;
                Lose.x = HIGHT / 4;
                Lose.y = 0;
                Lose.properties[0] = NULLX;
                Lose.properties[1] = NULLY;
            }
            else
            {
                Win.a_y = 8.0;
                Win.spd_y = -20;
                Win.pos_x = HIGHT / 4;
                Win.pos_y = WIDTH + 40;
                Win.x = HIGHT / 4;
                Win.y = WIDTH + 40;
                Win.properties[0] = NULLX;
                Win.properties[1] = NULLY;
                Lose.a_y = -8.0;
                Lose.spd_y = 20;
                Lose.pos_x = 3 * HIGHT / 4;
                Lose.pos_y = 0;
                Lose.x = 3 * HIGHT / 4;
                Lose.y = 0;
                Lose.properties[0] = NULLX;
                Lose.properties[1] = NULLY;
            }

            while (Win.spd_y < 0 && Lose.spd_y > 0)
            {
                SpeedControl(&Win);
                SpeedControl(&Lose);
                PosControl(&Win);
                PosControl(&Lose);

                if (fabs(Win.pos_y - Win.properties[1]) >= 1)
                {
                    coverWin(&Win);
                    drawWin(&Win);
                    Win.properties[0] = Win.pos_x;
                    Win.properties[1] = Win.pos_y;
                }
                if (fabs(Lose.pos_y - Lose.properties[1]) >= 1)
                {
                    coverLose(&Lose);
                    drawLose(&Lose);
                    Lose.properties[0] = Lose.pos_x;
                    Lose.properties[1] = Lose.pos_y;
                }
                Sleep(1);
            }
            while (1)
            {
                
            }
            
            
        }
    }
}
