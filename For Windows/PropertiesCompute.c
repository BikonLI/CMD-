#include "PropertiesCompute.h"

void SpeedControl(object *p)
{
    double time = IntegratingUnit();
    double spdx_offset = time * p->a_x, spdy_offset = time * p->a_y;
    p->spd_x += spdx_offset;
    p->spd_y += spdy_offset;
}

void PosControl(object *p)
{
    double time = IntegratingUnit();
    double posx_offset = time * p->spd_x, posy_offset = time * p->spd_y;
    p->x += posx_offset;
    p->y += posy_offset;
    p->pos_x = (int)(p->x);
    p->pos_y = (int)(p->y);
}

double IntegratingUnit()
{
    return TimeDifference(info.start_time, info.end_time);
}

void MissileSpeedControl(object *p)
{
    float differ_x, differ_y, target_tan, spd_y;
    if (p->id > 0)
    {
        differ_x = (B.pos_x - p->x);
        differ_y = (B.pos_y - p->y);

        if (fabs(differ_x) > 1)
        {
            target_tan = differ_y / differ_x;
            spd_y = target_tan * p->spd_x;
        }
        else
        {
            target_tan = 0;
        }

        if (differ_x * differ_x + differ_y * differ_y >= 30 && p->pos_x < B.pos_x - 6)
        {
            if (spd_y > 0)
                p->spd_y = (spd_y < 20) ? spd_y : 20;
            else
                p->spd_y = (spd_y > -20) ? spd_y : -20;
        }

        if (fabs(p->pos_x - A.pos_x) <= 4)
            p->spd_y = 0;
    }
    else
    {
        differ_x = (A.pos_x - p->x);
        differ_y = (A.pos_y - p->y);

        if (fabs(differ_x) > 1)
        {    
            target_tan = differ_y / differ_x;
            spd_y = target_tan * p->spd_x;
        }
        else
        {
            target_tan = 0;
        }

        if ((differ_x * differ_x + differ_y * differ_y) >= 30  && p->pos_x > A.pos_x + 6)
        {
            if (spd_y > 0)
                p->spd_y = (spd_y < 20) ? spd_y : 20;
            else
                p->spd_y = (spd_y > -20) ? spd_y : -20;
        }
        if (fabs(p->pos_x - B.pos_x) <= 4)
            p->spd_y = 0;
    }
}

void drawMissile(object *p)
{
    /*

    \|/——

    */
    curgoto(p->pos_x, p->pos_y);
    if (p->id > 0)
    {
        if (p->spd_y > 0 && p->spd_y / p->spd_x >= 0.8)
            printf("\\");
        else if (p->spd_y < 0 && fabs(p->spd_y / p->spd_x) >= 1)
            printf("/");
        else
            printf("|");
    }
    else
    {
        if (p->spd_y > 0 && fabs(p->spd_y / p->spd_x) >= 0.8)
            printf("/");
        else if (p->spd_y < 0 && p->spd_y / p->spd_x >= 0.8)
            printf("\\");
        else
            printf("|");
    }
}

void coverMissile(object *p)
{
    curgoto(p->properties[0], p->properties[1]);
    printf(" ");
}

int MissileActionControl_general(object *p)
{
    int if_hit = 0;
    if (p->existance)
    {
        MissileSpeedControl(p);
        SpeedControl(p);
        PosControl(p);

        p->rationality = VarifyPostion_missile(p);
        if ((p->spd_x <= 0 && p->id > 0) || (p->spd_x >= 0 && p->id < 0))
            p->rationality = 0;

        if (p->existance && p->rationality)
        {
            if (fabs(p->pos_x - p->properties[0]) >= 1 ||
                fabs(p->pos_y - p->properties[1]) >= 1)
            {
                drawMissile(p);
                coverMissile(p);
            }
            p->properties[0] = p->pos_x;
            p->properties[1] = p->pos_y;
            if (p->id > 0)
                if_hit = ReactionDetect(p->pos_x, p->pos_y, B.pos_x, B.pos_y, 'B');
            else
                if_hit = ReactionDetect(p->pos_x, p->pos_y, A.pos_x, A.pos_y, 'A');
            if (if_hit)
            {
                MissileReset(p);
            }
        }
        else
            MissileReset(p);
    }
    return if_hit;
}

int VarifyPostion_missile(object *p)
{
    if (
        p->pos_x >= 0 && p->pos_x < HIGHT &&
        p->pos_y >= 1 && p->pos_y <= WIDTH - 1)
        return 1;
    else
        return 0;
}

void FireMissile(object *p)
{
    if (!p->existance)
    {
        p->existance = 1;
        p->rationality = 0;
        if (p->id > 0)
        {
            p->x = A.pos_x + 2;
            p->y = A.pos_y;
        }
        else
        {
            p->x = B.pos_x - 2;
            p->y = B.pos_y;
        }
    }
}

void MissileReset(object *p)
{
    p->existance = 0;
    p->rationality = 0;
    curgoto(p->pos_x, p->pos_y);
    printf(" ");
    p->pos_x = NULLX;
    p->pos_y = NULLY;
    p->a_y = 0;
    p->a_x = 0;
    p->spd_y = 0;
    p->properties[0] = NULLX;
    p->properties[1] = NULLY;
    if (p->id > 0)
        p->spd_x = 10;
    else
        p->spd_x = -10;
}
