#include <iostream>
#include <math.h>
#include <winbgim.h>
#include <ctime>
#include <string.h>
#include <time.h>
#include <mmsystem.h>

using namespace std;

bool cron_status = false;
int alarm_poz;
int alarm_val;
int fundal_cnt = 0, ace;
float ora_reala, minut, sec;
int zi_sapt, zi, luna, an;
int contor_clipire = 0;
const float PI = 3.14159265;

int decalaje[5] = {2, 9, -5, 0, 8};
int zile_luna[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int decc = 0;

int startOra = 180;
int startMin = 460;
int startY = 350;


//Componente ceas digital
int gap;
const int segmente[10][7]=
{
    {1, 1, 1, 1, 1, 1, 0}, //0
    {0, 1, 1, 0, 0, 0, 0}, //1
    {1, 1, 0, 1, 1, 0, 1}, //2
    {1, 1, 1, 1, 0, 0, 1}, //3
    {0, 1, 1, 0, 0, 1, 1}, //4
    {1, 0, 1, 1, 0, 1, 1}, //5
    {1, 0, 1, 1, 1, 1, 1}, //6
    {1, 1, 1, 0, 0, 0, 0}, //7
    {1, 1, 1, 1, 1, 1, 1}, //8
    {1, 1, 1, 1, 0, 1, 1}  //9
};

//Cordonate
int bPlus_x1 = 50, bPlus_y1 = 700, bPlus_x2 = 125, bPlus_y2 = 750;
int bMinus_x1 = 135, bMinus_y1 = 700, bMinus_x2 = 210, bMinus_y2 = 750;
int bStart_x1 = 700, bStart_y1 = 700, bStart_x2 = 900, bStart_y2 = 750;
int bReset_x1 = 220, bReset_y1 = 700, bReset_x2 = 295, bReset_y2 = 750;
int luna_x1 = 800, luna_y1 = 110, luna_r1 = 65, luna_x2 = 800, luna_y2 = 140, luna_r2 = 50;
int bFus_x1 = 50, bFus_y1 = 50, bFus_x2 = 295, bFus_y2 = 100;
int bFundal_x1 = 50, bFundal_y1 = 110, bFundal_x2 = 170, bFundal_y2 = 160;
int bAlarma_x1 = 175, bAlarma_y1 = 110, bAlarma_x2 = 295, bAlarma_y2 = 160;
int bSchimba_x1 = 50, bSchimba_y1 = 650, bSchimba_x2 = 295, bSchimba_y2 = 690;
//---------------------------------------------------------------------------------------------------------------
int Digital_x1 = 150, Digital_y1 = 300, Digital_x2 = 850, Digital_y2 = 550, timp_offset = (Digital_y2-Digital_y1)/5;
int bPlus_Digital_x1 = 70, bPlus_Digital_y1 = 345, bPlus_Digital_x2 = 35, bPlus_Digital_y2 = 35;
int bMinus_Digital_x1 = 70, bMinus_Digital_y1 = 425, bMinus_Digital_x2 = 35, bMinus_Digital_y2 = 35;
int TimpSwitch_Digital_x1 = 70, TimpSwitch_Digital_y1 = 505, TimpSwitch_Digital_x2 = 35, TimpSwitch_Digital_y2 = 35;
int bFundal_Digital_x1 = 50, bFundal_Digital_y1 = 34, bFundal_Digital_x2 = 200, bFundal_Digital_y2 = 84;
int bAlarm_Dig_x1=215, bAlarm_Dig_y1=34, bAlarm_Dig_x2=365, bAlarm_Dig_y2=84;
int bFusDig_x1=380, bFusDig_y1=34, bFusDig_x2=530, bFusDig_y2=84;
int bCalDig_x1=545, bCalDig_y1=34, bCalDig_x2=695, bCalDig_y2=84;
int bClockSwitch_x1 = 900, bClockSwitch_y1 = 34, bClockSwitch_x2 = 990, bClockSwitch_y2 = 84;
int bResetDigital_x1 = 416, bResetDigital_y1 = 704, bResetDigital_x2 = 581, bResetDigital_y2 = 751;
//---------------------------------------------------------------------------------------------------------------

void timp_curent(int decalaj)
{
    time_t raw_time;
    struct tm* info;
    time(&raw_time);
    raw_time = raw_time + (decalaj*3600);
    info = gmtime(&raw_time);
    ora_reala = info->tm_hour;
    if (ora_reala >= 24) ora_reala -= 24;
    if (ora_reala < 0) ora_reala += 24;
    minut = info->tm_min;
    sec = info->tm_sec;
    zi = info->tm_mday;
    zi_sapt = info->tm_wday;
    luna = info->tm_mon;

    an = info ->tm_year + 1900;

}

bool esteClickInButon(int mx, int my, int x1, int y1, int x2, int y2)
{
    return (mx >= x1 && mx <= x2 && my >= y1 && my <= y2);
}

void Buton(int x1, int y1, int x2, int y2, char* text, int culoareFundal, int culoareText)
{
    //Fundal buton
    setfillstyle(SOLID_FILL, culoareFundal);
    bar(x1, y1, x2, y2);

    setcolor(WHITE);
    rectangle(x1, y1, x2, y2);

    setcolor(culoareText);
    setbkcolor(culoareFundal);
    settextstyle(6, 0, 1);
    int text_x = (x1 + x2) / 2 - textwidth(text) / 2;
    int text_y = (y1 + y2) / 2 - textheight(text) / 2;

    outtextxy(text_x, text_y, text);
    setbkcolor(BLACK);
}

void DeseneazaButonLuna(int x1, int y1, int r1, int x2, int y2, int r2, bool cron_status)
{
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, RGB(217, 41, 130));
    fillellipse(x1, y1, r1, r1);

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x2, y2, r2, r2);

    setcolor(WHITE);
    setbkcolor(RGB(217, 41, 130));
    if (cron_status == true)
        outtextxy(x1 - textwidth("STOP") / 2, y1 - textheight("STOP") / 2 - 35, "STOP");
    else
        outtextxy(x1 - textwidth("START") / 2, y1 - textheight("START") / 2 - 35, "START");
    setbkcolor(BLACK);
}

void Fundal(bool status)
{
    setactivepage(0);
    setvisualpage(0);
    cleardevice();

    // Butoane
    Buton(bPlus_x1, bPlus_y1, bPlus_x2, bPlus_y2, "+M", RED, YELLOW);
    Buton(bMinus_x1, bMinus_y1, bMinus_x2, bMinus_y2, "-M", RED, YELLOW);
    Buton(bReset_x1, bReset_y1, bReset_x2, bReset_y2, "RE", RED, YELLOW);
    Buton(bStart_x1, bStart_y1, bStart_x2, bStart_y2, "START", GREEN, WHITE);
    Buton(bFus_x1, bFus_y1, bFus_x2, bFus_y2, "FUS ORAR", RED, YELLOW);
    Buton(bFundal_x1, bFundal_y1, bFundal_x2, bFundal_y2, "FUNDAL", RED, YELLOW);
    Buton(bAlarma_x1, bAlarma_y1, bAlarma_x2, bAlarma_y2, "ALARMA", RED, YELLOW);
    DeseneazaButonLuna(luna_x1, luna_y1, luna_r1, luna_x2, luna_y2, luna_r2, status);
    Buton(bClockSwitch_x1, bClockSwitch_y1, bClockSwitch_x2, bClockSwitch_y2, "Digital", RED, WHITE);
    Buton(bSchimba_x1, bSchimba_y1, bSchimba_x2, bSchimba_y2, "Schimba acele", COLOR(128, 128, 128), BLACK);
}

bool clickElipsa(int mx, int my, int x1, int y1, int r1, int x2, int y2, int r2)
{
    long long d1 = (mx - x1) * (mx - x1) + (my - y1) * (my - y1);
    long long d2 = (mx - x2) * (mx - x2) + (my - y2) * (my - y2);
    long long r1_sq = r1 * r1;
    long long r2_sq = r2 * r2;
    if (d1 <= r1_sq && d2 > r2_sq)
        return true;
    return false;
}

void DeseneazaFata(int xc, int yc, int raza, int fundal)
{
    if (fundal_cnt == 0)
    {
        // Fundal ceas
        setfillstyle(SOLID_FILL, RGB(99, 99, 99));
        fillellipse(xc, yc, raza + 10, raza + 10);

        // Marginea ceasului
        setcolor(RGB(92, 64, 51));
        setlinestyle(SOLID_LINE, 0, 10);
        circle(xc, yc, raza + 10);

        // Liniile mari ale ceasului si marime+font cifre ceas
        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 4);
        settextstyle(1, 0, 4);

        int cnt = 3;
        for (int i = 0; i < 12; i++)
        {
            if (cnt > 12) cnt = 1;
            float a = i * (2 * PI / 12);
            line(xc + (raza - 15) * cos(a), yc + (raza - 15) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));

            // Afisarea numerotare ceas
            char txt[10];
            sprintf(txt, "%d", cnt);
            setcolor(WHITE);
            setbkcolor(RGB(99, 99, 99));
            outtextxy(xc + (raza - 40) * cos(a) - (textwidth(txt) / 2), yc + (raza - 40) * sin(a) - (textheight(txt) / 2), txt);
            setbkcolor(BLACK);
            cnt++;
        }

        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 1);
        for (int i = 0; i < 60; i++)
        {
            float a = i * (2 * PI / 60);
            line(xc + (raza - 10) * cos(a), yc + (raza - 10) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));
        }
    }
    else if (fundal_cnt == 1)
    {
        // Fundal ceas
        setfillstyle(SOLID_FILL, RGB(159, 223, 159));
        fillellipse(xc, yc, raza + 10, raza + 10);

        setfillstyle(SOLID_FILL, RGB(0, 102, 0));
        fillellipse(xc, yc, raza - 70, raza - 70);

        // Marginea ceasului
        setcolor(RGB(0, 102, 0));
        setlinestyle(SOLID_LINE, 0, 10);
        circle(xc, yc, raza + 10);

        // Liniile mari ale ceasului si marime+font cifre ceas
        setcolor(BLACK);
        setlinestyle(SOLID_LINE, 0, 4);

        int cnt = 3;
        for (int i = 0; i < 12; i++)
        {
            if (cnt > 12) cnt = 1;

            float a = i * (2 * PI / 12);
            line(xc + (raza - 15) * cos(a), yc + (raza - 15) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));

            // Afisarea numerotare ceas
            char txt[10];
            if (cnt % 3 == 0)
            {
                sprintf(txt, "%d", cnt);
                setcolor(BLACK);
                setbkcolor(RGB(159, 223, 159));
                settextstyle(1, 0, 6);
                outtextxy(xc + (raza - 40) * cos(a) - (textwidth(txt) / 2), yc + (raza - 40) * sin(a) - (textheight(txt) / 2), txt);
            }
            setbkcolor(BLACK);
            cnt++;
        }

        setcolor(BLACK);
        setlinestyle(SOLID_LINE, 0, 1);
        for (int i = 0; i < 60; i++)
        {
            float a = i * (2 * PI / 60);
            line(xc + (raza - 10) * cos(a), yc + (raza - 10) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));
        }
    }
    else if (fundal_cnt == 2)
    {
        // Fundal ceas
        setfillstyle(SOLID_FILL, RGB(255, 128, 170));
        fillellipse(xc, yc, raza + 10, raza + 10);

        setfillstyle(SOLID_FILL, RGB(255, 204, 221));
        fillellipse(xc, yc, raza - 70, raza - 70);

        // Marginea ceasului
        setcolor(RGB(255, 204, 220));
        setlinestyle(SOLID_LINE, 0, 15);
        circle(xc, yc, raza + 10);

        // Liniile mari ale ceasului si marime+font cifre ceas
        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 6);

        int cnt = 3;
        for (int i = 0; i < 12; i++)
        {
            if (cnt > 12) cnt = 1;

            float a = i * (2 * PI / 12);
            line(xc + (raza - 30) * cos(a), yc + (raza - 30) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));

            // Afisarea numerotare ceas
            setbkcolor(WHITE);
            cnt++;
        }

        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 1);
        for (int i = 0; i < 60; i++)
        {
            float a = i * (2 * PI / 60);
            line(xc + (raza - 10) * cos(a), yc + (raza - 10) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));
        }
    }
    else if (fundal_cnt == 3)
    {
        // Fundal ceas
        setcolor(WHITE);
        fillellipse(xc, yc, raza + 20, raza + 20);

        setfillstyle(SOLID_FILL, RGB(179, 217, 255));
        fillellipse(xc, yc, raza - 60, raza - 60);

        // Marginea ceasului
        setcolor(RGB(128, 191, 255));
        setlinestyle(SOLID_LINE, 0, 15);
        circle(xc, yc, raza - 60);

        // Liniile mari ale ceasului si marime+font cifre ceas
        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 6);

        int cnt = 3;
        for (int i = 0; i < 12; i++)
        {
            if (cnt > 12) cnt = 1;

            float a = i * (2 * PI / 12);
            line(xc + (raza - 30) * cos(a), yc + (raza - 30) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));

            // Afisarea numerotare ceas
            setbkcolor(WHITE);
            cnt++;
        }

        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 1);
        for (int i = 0; i < 60; i++)
        {
            float a = i * (2 * PI / 60);
            line(xc + (raza - 10) * cos(a), yc + (raza - 10) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));
        }
    }
    else if (fundal_cnt == 4)
    {
        // Fundal ceas
        setfillstyle(SOLID_FILL, RGB(255, 204, 0));
        fillellipse(xc, yc, raza - 30, raza - 30);

        setfillstyle(SOLID_FILL, RGB(255, 224, 102));
        fillellipse(xc, yc, raza - 120, raza - 120);

        // Marginea ceasului
        setcolor(RGB(255, 224, 102));
        setlinestyle(SOLID_LINE, 0, 10);
        circle(xc, yc, raza - 45);

        // Liniile mari ale ceasului si marime+font cifre ceas
        setcolor(RGB(255, 224, 102));
        setlinestyle(SOLID_LINE, 0, 6);

        int cnt = 3;
        for (int i = 0; i < 12; i++)
        {
            if (cnt > 12) cnt = 1;

            float a = i * (2 * PI / 12);
            setcolor(RGB(255, 204, 0));
            line(xc + (raza - 50) * cos(a), yc + (raza - 50) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));

            // Afisarea numerotare ceas
            char txt[10];
            sprintf(txt, "%d", cnt);
            setbkcolor(RGB(255, 204, 0));
            setcolor(BLACK);
            settextstyle(1, 0, 4);
            outtextxy(xc + (raza - 80) * cos(a) - (textwidth(txt) / 2), yc + (raza - 80) * sin(a) - (textheight(txt) / 2), txt);
            setbkcolor(BLACK);
            cnt++;
        }

        setcolor(RGB(255, 224, 102));
        setlinestyle(SOLID_LINE, 0, 1);
        for (int i = 0; i < 60; i++)
        {
            float a = i * (2 * PI / 60);
            line(xc + (raza - 30) * cos(a), yc + (raza - 30) * sin(a),
                 xc + raza * cos(a), yc + raza * sin(a));
        }
    }
}

void DeseneazaFataCron(int xc, int yc, int raza)
{
    // Fundal ceas
    setfillstyle(SOLID_FILL, RGB(237, 237, 237));
    fillellipse(xc, yc, raza + 10, raza + 10);

    // Marginea ceasului
    setcolor(RGB(204, 0, 102));
    setlinestyle(SOLID_LINE, 0, 10);
    circle(xc, yc, raza + 10);

    // Liniile mari ale ceasului si marime+font cifre ceas
    setcolor(BLACK);
    setlinestyle(SOLID_LINE, 0, 2);
    settextstyle(1, 0, 1);

    int cnt = 15;
    for (int i = 0; i < 12; i++)
    {
        if (cnt == 60) cnt = 0;

        float a = i * (2 * PI / 12);
        line(xc + (raza - 15) * cos(a), yc + (raza - 15) * sin(a),
             xc + raza * cos(a), yc + raza * sin(a));

        // Afisarea numerotare ceas
        char txt[3];
        sprintf(txt, "%d", cnt);
        setcolor(BLACK);
        setbkcolor(RGB(237, 237, 237));
        outtextxy(xc + (raza - 30) * cos(a) - (textwidth(txt) / 2), yc + (raza - 30) * sin(a) - (textheight(txt) / 2), txt);
        setbkcolor(BLACK);
        cnt += 5;
    }

    setlinestyle(SOLID_LINE, 0, 1);
    for (int i = 0; i < 60; i++)
    {
        if (i % 5 == 0)
        {
            setcolor(BLACK);
        }
        else
        {
            setcolor(RGB(204, 0, 102));
        }
        float a = i * (2 * PI / 60);
        line(xc + (raza - 10) * cos(a), yc + (raza - 10) * sin(a),
             xc + raza * cos(a), yc + raza * sin(a));
    }
}

void SchimbaAcele(int ace, int xc, int yc, float angle_ora, float angle_min, float angle_sec)
{
    if (ace == 0)
    {
        setcolor(BLACK);
        setlinestyle(0, 0, 5);
        line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora)); // Ora

        setcolor(YELLOW);
        setlinestyle(0, 0, 4);
        line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min)); // Minutar

        setcolor(RED);
        setlinestyle(0, 0, 2);
        line(xc, yc, xc + 170 * cos(angle_sec), yc + 170 * sin(angle_sec)); // Secundar
    }
    else if (ace == 1)
    {
        setcolor(BLACK);
        setlinestyle(0, 0, 5);
        line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora));

        setcolor(WHITE);
        setlinestyle(0, 0, 4);
        line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min));

        setcolor(YELLOW);
        setlinestyle(0, 0, 2);
        line(xc, yc, xc + 170 * cos(angle_sec), yc + 170 * sin(angle_sec));

    }
    else if (ace == 2)
    {
        setcolor(BLACK);
        setlinestyle(0, 0, 5);
        line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora));

        setcolor(RGB(122, 54, 31));
        setlinestyle(0, 0, 4);
        line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min));

        setcolor(RGB(179, 0, 0));
        setlinestyle(0, 0, 2);
        line(xc, yc, xc + 170 * cos(angle_sec), yc + 170 * sin(angle_sec));
    }
    else if (ace == 3)
    {
        setcolor(BLACK);
        setlinestyle(0, 0, 5);
        line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora));

        setcolor(RGB(122, 54, 31));
        setlinestyle(0, 0, 4);
        line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min));

        setcolor(RGB(0, 128, 255));
        setlinestyle(0, 0, 2);
        line(xc, yc, xc + 170 * cos(angle_sec), yc + 170 * sin(angle_sec));
    }
    else if (ace == 4)
    {
        setcolor(BLACK);
        setlinestyle(0, 0, 5);
        line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora));

        setcolor(RGB(102, 82, 0));
        setlinestyle(0, 0, 4);
        line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min));

        setcolor(RGB(102, 51, 0));
        setlinestyle(0, 0, 2);
        line(xc, yc, xc + 170 * cos(angle_sec), yc + 170 * sin(angle_sec));
    }
}

void DeseneazaLimbile(int xc, int yc, double timp_simulat)
{
    if (alarm_poz != 1)
    {
        float angle_sec = (timp_simulat * (2 * PI / 60)) - PI / 2;
        float angle_min = (timp_simulat / 60.0 * (2 * PI / 60)) - PI / 2;
        float angle_ora = (timp_simulat / 3600.0 * (2 * PI / 12)) - PI / 2;

        SchimbaAcele(ace, xc, yc, angle_ora, angle_min, angle_sec);

        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(xc, yc, 5, 5);
    }
    else
    {
        float angle_min = (alarm_val / 60.0 * (2 * PI / 60)) - PI / 2;
        float angle_ora = (alarm_val / 3600.0 * (2 * PI / 12)) - PI / 2;

        if (fundal_cnt == 0)
        {
            setcolor(BLACK);
            setlinestyle(0, 0, 5);
            line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora));
            setcolor(YELLOW);
            setlinestyle(0, 0, 4);
            line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min));
            setfillstyle(SOLID_FILL, WHITE);
            fillellipse(xc, yc, 5, 5);
        }
        else if (fundal_cnt == 1)
        {
            setcolor(BLACK);
            setlinestyle(0, 0, 5);
            line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora));
            setcolor(WHITE);
            setlinestyle(0, 0, 4);
            line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min));
            setfillstyle(SOLID_FILL, WHITE);
            fillellipse(xc, yc, 5, 5);
        }
        else if (fundal_cnt == 2 || fundal_cnt == 3)
        {
            setcolor(BLACK);
            setlinestyle(0, 0, 5);
            line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora));
            setcolor(RGB(122, 54, 31));
            setlinestyle(0, 0, 4);
            line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min));
            setfillstyle(SOLID_FILL, WHITE);
            fillellipse(xc, yc, 5, 5);
        }
        else if (fundal_cnt == 4)
        {
            setcolor(BLACK);
            setlinestyle(0, 0, 5);
            line(xc, yc, xc + 100 * cos(angle_ora), yc + 100 * sin(angle_ora));
            setcolor(RGB(102, 82, 0));
            setlinestyle(0, 0, 4);
            line(xc, yc, xc + 150 * cos(angle_min), yc + 150 * sin(angle_min));
            setfillstyle(SOLID_FILL, WHITE);
            fillellipse(xc, yc, 5, 5);
        }
    }
}

void DeseneazaLimbileCron(int xc, int yc, double timp_simulat)
{
    float angle_sec = (timp_simulat * (2 * PI / 60)) - PI / 2;
    float angle_min = (timp_simulat / 60.0 * (2 * PI / 60)) - PI / 2;

    // Minutar
    setcolor(BLACK);
    setlinestyle(0, 0, 4);
    line(xc, yc, xc + 50 * cos(angle_min), yc + 50 * sin(angle_min));

    // Secundar
    setcolor(RED);
    setlinestyle(0, 0, 2);
    line(xc, yc, xc + 70 * cos(angle_sec), yc + 70 * sin(angle_sec));

    // Punct centru
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(xc, yc, 5, 5);

    char timp_txt[30];
    int minute = (int)timp_simulat / 60;
    double secunde_ramase = timp_simulat - (minute * 60);

    if (minute > 0)
    {
        sprintf(timp_txt, "%d:%05.2f", minute, secunde_ramase);
    }
    else
    {
        sprintf(timp_txt, "%.2f s", timp_simulat);
    }

    setbkcolor(RGB(204, 0, 102));
    int latime_text = textwidth(timp_txt);
    setcolor(WHITE);
    outtextxy(xc - latime_text / 2, yc + 125, timp_txt);
    setbkcolor(BLACK);
}

void decideTara(int decc, int xc)
{
    if (decc == 0) outtextxy(xc - textwidth("Romania") / 2, 654 - textheight("Romania") / 2, "Romania");
    else if (decc == 1) outtextxy(xc - textwidth("Coreea de Sud") / 2, 654 - textheight("Coreea de Sud") / 2, "Coreea de Sud");
    else if (decc == 2) outtextxy(xc - textwidth("New York") / 2, 654 - textheight("New York") / 2, "New York");
    else if (decc == 3) outtextxy(xc - textwidth("Londra") / 2, 654 - textheight("Londra") / 2, "Londra");
    else if (decc == 4) outtextxy(xc - textwidth("China") / 2, 654 - textheight("China") / 2, "China");
}

void SetareFundalText(int fundal_cnt)
{
    if (fundal_cnt == 0) setbkcolor(RGB(99, 99, 99));
    else if (fundal_cnt == 1) setbkcolor(RGB(0, 102, 0));
    else if (fundal_cnt == 2) setbkcolor(RGB(255, 128, 170));
    else if (fundal_cnt == 3) setbkcolor(RGB(128, 191, 255));
    else if (fundal_cnt == 4) setbkcolor(RGB(255, 204, 0));
}

int Analog()
{
    mciSendString("open \"alarma_ceas.wav\" type waveaudio alias alarm", NULL, 0, NULL);

    bool time_not_saved = true;
    bool cron_pornit = false;
    int xc = 500, yc = 400;
    int raza = 200;

    timp_curent(decalaje[decc]);
    double timp_simulat = (ora_reala * 3600) + (minut * 60) + sec;
    int timp_trecut = 0, timp_trecut_alarma = 0;
    double timp_cron = 0;
    double saved_time;
    bool pauza = false;
    fundal_cnt = 0;
    alarm_poz = 0;
    alarm_val = 0;
    clock_t t1, t2;

    unsigned int marime_img = imagesize(0, 0, 1000, 800);
    void* imagine_fundal_1 = malloc(marime_img);
    void* imagine_fundal_2 = malloc(marime_img);

Redesen:
    bool saved_status = cron_status;
    if (fundal_cnt > 4)
    {
        fundal_cnt = 0;
    }

    Fundal(false);
    DeseneazaFata(xc, yc, raza, fundal_cnt);
    DeseneazaFataCron(800, 200, 100);
    getimage(0, 0, 1000, 800, imagine_fundal_1);

    Fundal(true);
    DeseneazaFata(xc, yc, raza, fundal_cnt);
    DeseneazaFataCron(800, 200, 100);
    getimage(0, 0, 1000, 800, imagine_fundal_2);

    cron_status = saved_status;
    int pagina = 1;
    t1 = clock();
    cron_status = saved_status;
    settextstyle(4, 0, 1);
    while (true)
    {
        setactivepage(pagina);
        if (cron_status == false)
        {
            putimage(0, 0, imagine_fundal_1, COPY_PUT);
        }
        else
        {
            putimage(0, 0, imagine_fundal_2, COPY_PUT);
        }
        if(kbhit())
        {
            int tasta = getch();

            if (tasta == 27) break;//ESC

            else if (tasta == 'a' || tasta == 'A')
            {
                if(time_not_saved)
                {
                    saved_time = timp_simulat;
                    time_not_saved = !time_not_saved;
                }
                alarm_poz++;
                if (alarm_poz > 2)
                {
                    alarm_val = 0;
                    alarm_poz = 0;
                    t1 = clock();
                }
                else if (alarm_poz == 0)
                {
                    pauza = false;
                    time_not_saved = true;
                    timp_simulat = saved_time;
                    t1 = clock();
                }
                else if (alarm_poz == 1)
                {
                    pauza = true;
                    time_not_saved = false;
                    timp_simulat = 0;
                    t1 = clock();
                }
                else if (alarm_poz == 2)
                {
                    pauza = false;
                    time_not_saved = true;
                    timp_simulat = saved_time;
                    if (alarm_val == 0)
                    {
                        alarm_poz = 0;
                    }
                    t1 = clock();
                }
            }
            else if (tasta == 32 && alarm_poz != 1)//Space
            {
                pauza = false;
                t1 = clock();
            }
            else if (tasta == 0 || tasta == 224)
            {
                int tasta_spec = getch();
                if (tasta_spec == 72)//Sus
                {
                    if (alarm_poz == 1) alarm_val += 60;
                    else
                    {
                        timp_simulat += 60;
                        pauza = true;
                    }
                }
                else if (tasta_spec == 80) //Jos
                {
                    if (alarm_poz == 1 && alarm_val >= 60) alarm_val -= 60;
                    else
                    {
                        timp_simulat -= 60;
                        pauza = true;
                    }
                }
            }
        }
        while (ismouseclick(WM_LBUTTONDOWN))
        {
            // Coordonate click
            int mx, my;
            getmouseclick(WM_LBUTTONDOWN, mx, my);

            // Plus
            if (esteClickInButon(mx, my, bPlus_x1, bPlus_y1, bPlus_x2, bPlus_y2))
            {
                if (alarm_poz != 1)
                {
                    pauza = true;
                    timp_simulat += 60;
                }
                else if (alarm_poz == 1)
                {
                    alarm_val += 60;
                }
            }
            // Minus
            if (esteClickInButon(mx, my, bMinus_x1, bMinus_y1, bMinus_x2, bMinus_y2))
            {
                if (alarm_poz != 1)
                {
                    pauza = true;
                    timp_simulat -= 60;
                }
                else if (alarm_poz == 1 && alarm_val >= 60)
                {
                    alarm_val -= 60;
                }
            }
            // Start
            if (esteClickInButon(mx, my, bStart_x1, bStart_y1, bStart_x2, bStart_y2))
            {
                pauza = false;
                t1 = clock();
            }
            // Reset
            if (esteClickInButon(mx, my, bReset_x1, bReset_y1, bReset_x2, bReset_y2))
            {
                timp_curent(decalaje[decc]);
                timp_simulat = (ora_reala * 3600) + (minut * 60) + sec;
                pauza = false;
                t1 = clock();
            }
            // Fus Orar
            if (esteClickInButon(mx, my, bFus_x1, bFus_y1, bFus_x2, bFus_y2))
            {
                decc++;
                if (decc > 4) decc = 0;
                timp_curent(decalaje[decc]);
                timp_simulat = (ora_reala * 3600) + (minut * 60) + sec;
                t1 = clock();
            }
            // Elipsa / Cronometru
            if (clickElipsa(mx, my, luna_x1, luna_y1, luna_r1, luna_x2, luna_y2, luna_r2))
            {
                if (cron_pornit == false)
                    timp_cron = 0;
                cron_pornit = !cron_pornit;
                cron_status = !cron_status;
                t1 = clock();
            }
            // Fundal
            if (esteClickInButon(mx, my, bFundal_x1, bFundal_y1, bFundal_x2, bFundal_y2))
            {
                fundal_cnt++;
                goto Redesen;
            }
            // Alarma
            if (esteClickInButon(mx, my, bAlarma_x1, bAlarma_y1, bAlarma_x2, bAlarma_y2))
            {
                if(time_not_saved)
                {
                    saved_time = timp_simulat;
                    time_not_saved = !time_not_saved;
                }
                alarm_poz++;
                if (alarm_poz > 2)
                {
                    alarm_val = 0;
                    alarm_poz = 0;
                    t1 = clock();
                }
                else if (alarm_poz == 0)
                {
                    pauza = false;
                    time_not_saved = true;
                    timp_simulat = saved_time;
                    t1 = clock();
                }
                else if (alarm_poz == 1)
                {
                    pauza = true;
                    time_not_saved = false;
                    timp_simulat = 0;
                    t1 = clock();
                }
                else if (alarm_poz == 2)
                {
                    pauza = false;
                    time_not_saved = true;
                    timp_simulat = saved_time;
                    if (alarm_val == 0)
                    {
                        alarm_poz = 0;
                    }
                    t1 = clock();
                }
            }
            if (esteClickInButon(mx, my, bClockSwitch_x1, bClockSwitch_y1, bClockSwitch_x2, bClockSwitch_y2))
            {
                free(imagine_fundal_1);
                free(imagine_fundal_2);
                mciSendString("close alarm", NULL, 0, NULL);
                return 1;
            }
            if (esteClickInButon(mx, my, bSchimba_x1, bSchimba_y1, bSchimba_x2, bSchimba_y2))
            {
                ace++;
                if (ace > 4)
                {
                    ace = 0;
                }
            }
        }

        decideTara(decc, xc);
        SetareFundalText(fundal_cnt);
        int ora_curenta_24h = ((int)timp_simulat / 3600) % 24;

        if (ora_curenta_24h >= 12) outtextxy(xc - textwidth("PM") / 2, xc - 5 - textheight("PM") / 2, "PM");
        else outtextxy(xc - textwidth("AM") / 2, xc - 5 - textheight("AM") / 2, "AM");

        if (alarm_poz == 2)
        {
            char txt[10];
            int ora_alarm = alarm_val / 3600;
            int min_alarm = (alarm_val - (ora_alarm) * 3600) / 60;
            if (ora_alarm > 0)
            {
                sprintf(txt, "%d:%02d", ora_alarm, min_alarm);
                outtextxy((bAlarma_x1 + bAlarma_x2) / 2 - textwidth(txt) / 2, bAlarma_x1 - textheight(txt) / 2, txt);
            }
            else if (min_alarm < 60 && min_alarm >= 1)
            {
                sprintf(txt, "%d min", min_alarm);
                outtextxy((bAlarma_x1 + bAlarma_x2) / 2 - textwidth(txt) / 2, bAlarma_x1 - textheight(txt) / 2, txt);
            }
            else if (min_alarm < 1 && alarm_val != 0)
            {
                sprintf(txt, "%d sec", alarm_val);
                outtextxy((bAlarma_x1 + bAlarma_x2) / 2 - textwidth(txt) / 2, bAlarma_x1 - textheight(txt) / 2, txt);
            }
        }

        DeseneazaLimbile(xc, yc, timp_simulat);
        DeseneazaLimbileCron(800, 200, timp_cron);
        setvisualpage(pagina);
        pagina = 1 - pagina;

        delay(1);
        t2 = clock();
        double durata_reala = (double)(t2 - t1);
        t1 = t2;
        if (alarm_poz == 2 && !pauza)
        {
            if (alarm_val > 0)
            {
                timp_trecut_alarma += durata_reala;
                if (timp_trecut_alarma >= 1000)
                {
                    alarm_val--;
                    timp_trecut_alarma -= 1000;
                    if (alarm_val == 0)
                    {
                        mciSendString("play alarm from 0", NULL, 0, NULL);
                    }
                }
            }
        }

        if (!pauza)
        {
            timp_trecut += durata_reala;
            if (timp_trecut >= 1000)
            {
                PlaySound("click_ceas.wav", NULL, SND_FILENAME | SND_ASYNC);
                timp_simulat++;
                timp_trecut -= 1000;
            }
        }

        if (cron_pornit) timp_cron += durata_reala / 1000.0;
    }

    free(imagine_fundal_1);
    free(imagine_fundal_2);
    mciSendString("close alarm", NULL, 0, NULL);
    return -1;
}

//--------------------------------------------------------------------------------------------------------------------------
void ButonDigital(int x1, int y1, int x2, int y2, char* text, int culoareFundal, int culoareText)
{
    //Fundal buton
    setfillstyle(SOLID_FILL, culoareFundal);
    fillellipse(x1, y1, x2, y2);

    setcolor(culoareText);
    setbkcolor(culoareFundal);
    settextstyle(4, 0, 2);
    int text_x = x1 - textwidth(text) / 2;
    int text_y = y1 - textheight(text) / 2;

    outtextxy(text_x, text_y, text);
    setbkcolor(BLACK);
}

int get_day_of_week(int d, int m, int y)
{
    m = m + 1;
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

bool clickElipsaDigitala(int mx, int my, int x1, int y1, int x_r, int y_r)
{
    long long dx = mx - x1;
    long long dy = my - y1;
    long long xr_sq = (long long)x_r * x_r;
    long long yr_sq = (long long)y_r * y_r;
    if (dx * dx * yr_sq + dy * dy * xr_sq <= xr_sq * yr_sq)
        return true;
    return false;
}

void deseneazaCifra(int x, int y, int cifra, int marime, int fundal_cnt)
{
    int gap = 4;
    if (fundal_cnt == 0)
        setcolor(BLACK);
    else if (fundal_cnt == 1)
        setcolor(WHITE);
    else if (fundal_cnt == 2)
        setcolor(RGB(230, 115, 0));
    else if (fundal_cnt == 3)
        setcolor(RGB(12, 59, 32));
    else if (fundal_cnt == 4)
        setcolor(RGB(91, 16, 173));
    setlinestyle(SOLID_LINE, 0, marime / 6);

    //Segment A/Sus
    if (segmente[cifra][0]) line(x + gap, y, x + marime - gap, y);
    //Segment B/Dreapta-Sus
    if (segmente[cifra][1]) line(x + marime, y + gap, x + marime, y + marime - gap);
    //Segment C/Dreapta-Jos
    if (segmente[cifra][2]) line(x + marime, y + marime + gap, x + marime, y + 2 * marime - gap);
    //Segment D/Jos
    if (segmente[cifra][3]) line(x + gap, y + 2 * marime, x + marime - gap, y + 2 * marime);
    //Segment E/St nga-Jos
    if (segmente[cifra][4]) line(x, y + marime + gap, x, y + 2 * marime - gap);
    //Segment F/St nga-Sus
    if (segmente[cifra][5]) line(x, y + gap, x, y + marime - gap);
    //Segment G/Mijloc
    if (segmente[cifra][6]) line(x + gap, y + marime, x + marime - gap, y + marime);
}

void DeseneazaDigital(int x1, int y1, int x2, int y2, int fundal_cnt, int setare_calendar)
{
    int adancime = 80;
    int dx = adancime;
    int dy = -adancime / 1.5;

    int poly_sus[] =
    {
        x1, y1,              //Sus stanga fata
        x2, y1,              //Sus dreapta fata
        x2 + dx, y1 + dy,    //Sus  dreapta spate
        x1 + dx, y1 + dy,    //Sus stanga spate
        x1, y1               //Conexiunea finala
    };

    int poly_margine[] =
    {
        x2, y1,              //Margine dreapta fata
        x2 + dx, y1 + dy,    //Margine dreapta spate
        x2 + dx, y2 + dy,    //Margine dreapta spate
        x2, y2,              //Margine dreapta fata
        x2, y1               //Conexiunea finala
    };

    if(fundal_cnt==0)
    {
        setcolor(RGB(89, 89, 89));
        setlinestyle(SOLID_LINE, 0, 0);
        setfillstyle(SOLID_FILL, RGB(89, 89, 89));
        fillpoly(5, poly_sus);
        fillpoly(5, poly_margine);

        setfillstyle(SOLID_FILL, WHITE);
        bar(x1, y1, x2, y2);

        //Setari margine
        setlinestyle(SOLID_LINE, 0, 5);

        //Marginile
        setcolor(WHITE);
        line(x1, y1, x1 + dx, y1 + dy); //Stanga sus margine diagonal
        line(x1 + dx, y1 + dy, x2 + dx, y1 + dy); //Mijloc sus spate
        line(x2 + dx, y1 + dy, x2 + dx, y2 + dy); // Margine dreapta
        line(x2, y1, x2 + dx, y1 + dy); //Dreapta sus margine diagonala
        line(x2 + dx, y2 + dy, x2, y2); //Dreapta jos margine diagonala

        setcolor(RGB(54, 54, 54));
        setlinestyle(SOLID_LINE, 0, 7);
        rectangle(x1, y1, x2, y2);

    }
    else if(fundal_cnt==1)
    {
        setcolor(RGB(77, 52, 52));
        setlinestyle(SOLID_LINE, 0, 0);
        setfillstyle(SOLID_FILL, RGB(77, 52, 52));
        fillpoly(5, poly_sus);
        fillpoly(5, poly_margine);

        setfillstyle(SOLID_FILL, RGB(219, 37, 37));
        bar(x1, y1, x2, y2);

        //Marginile
        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 5);
        line(x1, y1, x1 + dx, y1 + dy); //Stanga sus margine diagonal
        line(x1 + dx, y1 + dy, x2 + dx, y1 + dy); //Mijloc sus spate
        line(x2 + dx, y1 + dy, x2 + dx, y2 + dy); // Margine dreapta
        line(x2, y1, x2 + dx, y1 + dy); //Dreapta sus margine diagonala
        line(x2 + dx, y2 + dy, x2, y2); //Dreapta jos margine diagonala
        rectangle(x1, y1, x2, y2);
    }
    else if(fundal_cnt==2)
    {
        setcolor(RGB(102, 51, 0));
        setfillstyle(SOLID_FILL, RGB(38, 38, 38));
        bar(x1, y1, x2, y2);
        setlinestyle(3, 0, 5);
        rectangle(x1, y1, x2, y2);
    }
    else if(fundal_cnt==3)
    {

        setfillstyle(SOLID_FILL, RGB(10, 161, 120));//Verde

        setcolor(RGB(12, 59, 32));
        setlinestyle(SOLID_LINE, 0, 7);
        fillellipse((x2+x1)/2, (y2+y1)/2,380, 170);
        ellipse((x2+x1)/2, (y2+y1)/2, 0, 360, 380,170);
    }
    else if(fundal_cnt==4)
    {
        setcolor(BLACK);
        setcolor(BLACK);
        setlinestyle(SOLID_LINE, 0, 1);
        setfillstyle(SOLID_FILL, RGB(0, 0, 0));

        fillpoly(5, poly_sus);
        fillpoly(5, poly_margine);

        setfillstyle(SOLID_FILL, RGB(184, 154, 217));//Mov
        bar(x1, y1, x2, y2);
        setcolor(RGB(91, 16, 173));
        setlinestyle(3, 0, 9);
        rectangle(x1, y1, x2, y2);
        setlinestyle(3, 0, 5);
        rectangle(x1+20, y1, x2-20, y2);
        rectangle(x1, y1+20, x2, y2-20);
    }
    setcolor(WHITE);
    ButonDigital(bPlus_Digital_x1, bPlus_Digital_y1, bPlus_Digital_x2, bPlus_Digital_y2, "+", COLOR(153, 102, 51), WHITE);
    ButonDigital(bMinus_Digital_x1, bMinus_Digital_y1, bMinus_Digital_x2, bMinus_Digital_y2, "-", COLOR(153, 102, 51), WHITE);
    ButonDigital(TimpSwitch_Digital_x1, TimpSwitch_Digital_y1, TimpSwitch_Digital_x2, TimpSwitch_Digital_y2, "M/H", COLOR(153, 102, 51), WHITE);
    Buton(bFundal_Digital_x1, bFundal_Digital_y1, bFundal_Digital_x2, bFundal_Digital_y2, "Fundal", COLOR(153, 102, 51), WHITE);
    Buton(bAlarm_Dig_x1, bAlarm_Dig_y1, bAlarm_Dig_x2, bAlarm_Dig_y2, "Alarma", COLOR(153, 102, 51), WHITE);
    Buton(bFusDig_x1, bFusDig_y1, bFusDig_x2, bFusDig_y2, "Fus orar",COLOR(153, 102, 51), WHITE);
    Buton(bClockSwitch_x1, bClockSwitch_y1, bClockSwitch_x2, bClockSwitch_y2, "Analog", RED, WHITE);
    Buton(bResetDigital_x1, bResetDigital_y1, bResetDigital_x2, bResetDigital_y2, "Resetare", GREEN, WHITE);

    if(setare_calendar == 0)
    {
        Buton(bCalDig_x1, bCalDig_y1, bCalDig_x2, bCalDig_y2, "Calendar",COLOR(153, 102, 51), WHITE);
    }
    else if(setare_calendar == 1)
    {
        Buton(bCalDig_x1, bCalDig_y1, bCalDig_x2, bCalDig_y2, "Ziua",COLOR(153, 102, 51), WHITE);
    }
    else if(setare_calendar == 2)
    {
        Buton(bCalDig_x1, bCalDig_y1, bCalDig_x2, bCalDig_y2, "Luna",COLOR(153, 102, 51), WHITE);
    }
    else if(setare_calendar == 3)
    {
        Buton(bCalDig_x1, bCalDig_y1, bCalDig_x2, bCalDig_y2, "Anul",COLOR(153, 102, 51), WHITE);
    }
}

void GestioneazaCeas(float hour, float minute)
{
    int marime = 70;
    int pas = marime + 30;
    int ora_digitala = (int)hour;
    int minut_digital = (int)minute;
    // ora
    if(hour > 9)
    {
        deseneazaCifra(startOra, startY, ora_digitala / 10, marime, fundal_cnt);
        deseneazaCifra(startOra + pas, startY, ora_digitala % 10, marime, fundal_cnt);
    }
    else
    {
        deseneazaCifra(startOra, startY, 0, marime, fundal_cnt);
        deseneazaCifra(startOra + pas, startY, ora_digitala, marime, fundal_cnt);
    }

    // min
    if(minute > 9)
    {
        deseneazaCifra(startMin, startY, minut_digital / 10, marime, fundal_cnt);
        deseneazaCifra(startMin + pas, startY, minut_digital % 10, marime, fundal_cnt);
    }
    else
    {
        deseneazaCifra(startMin, startY, 0, marime, fundal_cnt);
        deseneazaCifra(startMin + pas, startY, minut_digital, marime, fundal_cnt);
    }
}
void Clipire()
{
    if(contor_clipire < 10)
    {
        if(fundal_cnt==0)
        {
            setbkcolor(WHITE);
            setcolor(BLACK);
        }
        else if(fundal_cnt==1)
        {
            setbkcolor(RGB(219, 37, 37));
            setcolor(WHITE);
        }
        else if(fundal_cnt==2)
        {
            setbkcolor(RGB(38, 38, 38));
            setcolor(RGB(230, 115, 0));
            settextstyle(2, 0, 30);
        }
        else if(fundal_cnt==3)
        {
            setbkcolor(RGB(10, 161, 120));
            setcolor(RGB(12, 59, 32));
        }
        else if(fundal_cnt==4)
        {
            setbkcolor(RGB(184, 154, 217));
            setcolor(RGB(91, 16, 173));
        }

        int centerX = (startOra+startMin)/1.6;
        int centerY = (Digital_y1 + Digital_y2) / 2;

        fillellipse(centerX, centerY - 25, 6, 6);
        fillellipse(centerX, centerY + 25, 6, 6);
    }
    setbkcolor(BLACK);
}

void DataDigitala()
{
    char data_num[20];
    char ziua_sapt[20];
    char an_curent[20];
    const char* zilele[7] = {"Dum", "Lun", "Mar", "Mie", "Joi", "Vin", "Sam"};
    if(zi_sapt < 0) zi_sapt = 6;
    if(zi_sapt > 6) zi_sapt = 0;
    sprintf(ziua_sapt, "%s", zilele[zi_sapt]);
    sprintf(data_num, "%02d/%02d", zi, luna+1);
    sprintf(an_curent, "%d", an);
    if(fundal_cnt==0)
    {
        setbkcolor(WHITE);
        setcolor(BLACK);
        settextstyle(9,0, 5);

        outtextxy(750-textwidth(ziua_sapt) / 2, 375 - textheight(ziua_sapt) / 2, ziua_sapt);
        outtextxy(750-textwidth(data_num) / 2, 425 - textheight(data_num) / 2, data_num);
        outtextxy(750-textwidth(an_curent) / 2, 475 - textheight(an_curent) / 2, an_curent);
    }
    else if(fundal_cnt==1)
    {
        setbkcolor(RGB(219, 37, 37));
        setcolor(WHITE);
        settextstyle(3,0, 5);

        outtextxy(750-textwidth(ziua_sapt) / 2, 375 - textheight(ziua_sapt) / 2, ziua_sapt);
        outtextxy(750-textwidth(data_num) / 2, 425 - textheight(data_num) / 2, data_num);
        outtextxy(750-textwidth(an_curent) / 2, 475 - textheight(an_curent) / 2, an_curent);
    }
    else if(fundal_cnt==2)
    {
        setbkcolor(RGB(38, 38, 38));
        setcolor(RGB(230, 115, 0));
        settextstyle(3,0, 5);

        outtextxy(750-textwidth(ziua_sapt) / 2, 375 - textheight(ziua_sapt) / 2, ziua_sapt);
        outtextxy(750-textwidth(data_num) / 2, 425 - textheight(data_num) / 2, data_num);
        outtextxy(750-textwidth(an_curent) / 2, 475 - textheight(an_curent) / 2, an_curent);
    }
    else if(fundal_cnt==3)
    {
        setbkcolor(RGB(10, 161, 120));
        setcolor(RGB(12, 59, 32));
        settextstyle(6,0, 4);

        outtextxy(770-textwidth(ziua_sapt) / 2, 375 - textheight(ziua_sapt) / 2, ziua_sapt);
        outtextxy(770-textwidth(data_num) / 2, 425 - textheight(data_num) / 2, data_num);
        outtextxy(770-textwidth(an_curent) / 2, 475 - textheight(an_curent) / 2, an_curent);
    }
    else if(fundal_cnt==4)
    {
        setbkcolor(RGB(184, 154, 217));
        setcolor(RGB(91, 16, 173));
        settextstyle(10, 0, 4);

        outtextxy(750-textwidth(ziua_sapt) / 2, 375 - textheight(ziua_sapt) / 2, ziua_sapt);
        outtextxy(750-textwidth(data_num) / 2, 425 - textheight(data_num) / 2, data_num);
        outtextxy(750-textwidth(an_curent) / 2, 475 - textheight(an_curent) / 2, an_curent);

    }

    setbkcolor(BLACK);
}

int Digital()
{
    mciSendString("open \"ceas_digital_alarm.wav\" type waveaudio alias alarm", NULL, 0, NULL);
    alarm_poz = 0;
    alarm_val = 0;
    fundal_cnt = 0;
    int pagina = 0;
    int contor_cal = 0;
    int zi_initiala = -1;
    int luna_initiala = -1;
    int an_initial = -1;
    int limit;
    bool timp_switch = false, redraw = false;
    timp_curent(decalaje[decc]);
    clock_t t1, t2;
    double timp_acumulat = 0;
    double durata_reala = 0;
    float ora_ceas_dig = ora_reala;
    float minut_ceas_dig = minut;
    float sec_ceas_dig = sec;
    float ora_alarm = 0, min_alarm = 0;
    t1 = clock();

    unsigned int marime_img = imagesize(0, 0, 999, 799);
    void* imagine_fundal_1 = malloc(marime_img);

    setactivepage(0);
    setvisualpage(0);
    cleardevice();
    while(true)
    {
        if(kbhit())
        {
            int tasta = getch();

            if (tasta == 27) break;//ESC

            else if (tasta == 'a' || tasta == 'A')
            {
                alarm_poz++;
                redraw=true;
                if (alarm_poz == 1)
                {
                    ora_alarm = 0;
                    min_alarm = 0;
                }
                else if (alarm_poz == 2)
                {
                    if (ora_alarm == 0 && min_alarm == 0)
                    {
                        alarm_poz = 0;
                    }
                }
                else if (alarm_poz > 2)
                {
                    alarm_poz = 0;
                    alarm_val = 0;
                    mciSendString("stop alarm", NULL, 0, NULL);
                }
            }
            else if (tasta == 32)//Space
            {
                timp_switch = !timp_switch;
            }
            else if (tasta == 0 || tasta == 224)
            {
                int tasta_speciala = getch();
                if (tasta_speciala == 72)//sus
                {
                    if (alarm_poz == 1)
                    {
                        if (!timp_switch)
                        {
                            min_alarm++;
                            if (min_alarm >= 60) min_alarm = 0;
                        }
                        else
                        {
                            ora_alarm++;
                            if (ora_alarm >= 24) ora_alarm = 0;
                        }
                    }
                    else if (contor_cal == 0)
                    {
                        if (!timp_switch)
                        {
                            minut_ceas_dig++;
                            if (minut_ceas_dig >= 60) minut_ceas_dig = 0;
                        }
                        else
                        {
                            ora_ceas_dig++;
                            if (ora_ceas_dig >= 24) ora_ceas_dig = 0;
                        }
                    }
                    else if(contor_cal==1)
                    {
                        zi++;
                        zi_sapt++;
                        if (zi_sapt > 6) zi_sapt = 0;

                        limit = zile_luna[luna];
                        if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0))) limit = 29;

                        if (zi > limit)
                        {
                            zi = 1;
                            luna++;
                            if (luna > 11)
                            {
                                luna = 0;
                                an++;
                            }
                        }
                    }
                    else if(contor_cal==2)
                    {
                        luna++;
                        if (luna > 11)
                        {
                            luna = 0;
                            an++;
                        }

                        limit = zile_luna[luna];
                        if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0))) limit = 29;

                        if (zi > limit) zi = limit;
                        zi_sapt = get_day_of_week(zi, luna, an);
                    }
                    else if(contor_cal==3)
                    {
                        an++;
                        if (luna == 1 && zi == 29)
                        {
                            if (!(an % 4 == 0 && (an % 100 != 0 || an % 400 == 0))) zi = 28;
                        }
                        zi_sapt = get_day_of_week(zi, luna, an);
                    }
                }
                else if (tasta_speciala == 80)//Jos
                {
                    if (alarm_poz == 1)
                    {
                        if (!timp_switch)
                        {
                            min_alarm--;
                            if (min_alarm < 0) min_alarm = 59;
                        }
                        else
                        {
                            ora_alarm--;
                            if (ora_alarm < 0) ora_alarm = 23;
                        }
                    }
                    else if (contor_cal == 0)
                    {
                        if (!timp_switch)
                        {
                            minut_ceas_dig--;
                            if (minut_ceas_dig < 0) minut_ceas_dig = 59;
                        }
                        else
                        {
                            ora_ceas_dig--;
                            if (ora_ceas_dig < 0) ora_ceas_dig = 23;
                        }
                    }
                    else if (contor_cal == 1)
                    {
                        zi--;
                        zi_sapt--;
                        if (zi_sapt < 0) zi_sapt = 6;

                        if (zi < 1)
                        {
                            luna--;
                            if (luna < 0)
                            {
                                luna = 11;
                                an--;
                            }

                            limit = zile_luna[luna];
                            if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0))) limit = 29;

                            zi = limit;
                            zi_sapt = get_day_of_week(zi, luna, an);
                        }
                    }
                    else if (contor_cal == 2)
                    {
                        luna--;
                        if (luna < 0)
                        {
                            luna = 11;
                            an--;
                        }

                        limit = zile_luna[luna];
                        if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0))) limit = 29;

                        if (zi > limit) zi = limit;
                        zi_sapt = get_day_of_week(zi, luna, an);
                    }
                    else if (contor_cal == 3)
                    {
                        an--;
                        if (luna == 1 && zi == 29)
                        {
                            if (!(an % 4 == 0 && (an % 100 != 0 || an % 400 == 0))) zi = 28;
                        }
                        zi_sapt = get_day_of_week(zi, luna, an);
                    }
                }
            }
        }
        if(fundal_cnt>4)
        {
            fundal_cnt = 0;
        }
        if(zi_initiala != zi || luna_initiala != luna || an_initial != an || redraw)
        {
            redraw = false;
            zi_initiala = zi;
            luna_initiala = luna;
            an_initial = an;

            setactivepage(pagina);
            cleardevice();
            DeseneazaDigital(Digital_x1, Digital_y1, Digital_x2, Digital_y2, fundal_cnt, contor_cal);
            DataDigitala();

            if(fundal_cnt == 0)
            {
                setcolor(WHITE);
            }
            else if(fundal_cnt == 1)
            {
                setcolor(WHITE);
            }
            else if(fundal_cnt == 2)
            {
                setcolor(RGB(102, 51, 0));
            }
            else if(fundal_cnt == 3)
            {
                setcolor(RGB(12, 59, 32));
            }
            else if(fundal_cnt == 4)
            {
                setcolor(RGB(91, 16, 173));
            }
            settextstyle(1, 0, 4);
            decideTara(decc, 500);

            if (alarm_poz == 2)
            {
                settextstyle(1, 0, 3);
                setcolor(GREEN);

                if (fundal_cnt == 0) setbkcolor(WHITE);
                else if (fundal_cnt == 1)
                {
                    setbkcolor(RGB(219, 37, 37));
                }
                else if (fundal_cnt == 2) setbkcolor(RGB(38, 38, 38));
                else if (fundal_cnt == 3) setbkcolor(RGB(10, 161, 120));
                else setbkcolor(RGB(184, 154, 217));

                outtextxy(480, 510, "ALARM ON");
                setbkcolor(BLACK); // Resetam la default
            }

            getimage(0, 0, 999, 799, imagine_fundal_1);
        }

        setactivepage(pagina);
        cleardevice();
        putimage(0, 0, imagine_fundal_1, COPY_PUT);
        if (alarm_poz == 1)
        {
            GestioneazaCeas(ora_alarm, min_alarm);

            settextstyle(1, 0, 3);

            if (fundal_cnt == 0)
            {
                setbkcolor(WHITE);
            }
            else if (fundal_cnt == 1)
            {
                setcolor(BLACK);
                setbkcolor(RGB(219, 37, 37));
            }
            else if (fundal_cnt == 2)
                setbkcolor(RGB(38, 38, 38));
            else if (fundal_cnt == 3)
                setbkcolor(RGB(10, 161, 120));
            else setbkcolor(RGB(184, 154, 217));

            setcolor(RED);

            outtextxy(480, 510, "SET ALARM");
        }
        else
        {
            GestioneazaCeas(ora_ceas_dig, minut_ceas_dig);
        }

        Clipire();
        setvisualpage(pagina);
        pagina = 1 - pagina;

        t2 = clock();
        durata_reala = (double)(t2 - t1);
        t1 = t2;
        if (durata_reala > 200)
        {
            durata_reala = 200;
        }
        timp_acumulat += durata_reala;
        while(timp_acumulat >= 50)
        {
            timp_acumulat -= 50;
            contor_clipire++;

            if(contor_clipire >= 20)
            {
                contor_clipire = 0;
                sec_ceas_dig++;
                PlaySound("ceas_digital.wav", NULL, SND_FILENAME | SND_ASYNC);
                if(sec_ceas_dig >= 60)
                {
                    sec_ceas_dig = 0;
                    minut_ceas_dig++;
                    if (minut_ceas_dig >= 60)
                    {
                        minut_ceas_dig = 0;
                        ora_ceas_dig++;
                        if (ora_ceas_dig >= 24)
                        {
                            ora_ceas_dig = 0;
                            zi++;
                            zi_sapt++;
                            if(zi_sapt > 6) zi_sapt = 0;
                            limit = zile_luna[luna];
                            if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                            {
                                limit = 29;
                            }
                            if(zi > limit)
                            {
                                zi = 1;
                                luna++;
                                if(luna > 11)
                                {
                                    luna = 0;
                                    an++;
                                }
                            }
                            zi_sapt = get_day_of_week(zi, luna, an);
                        }
                    }
                }
                if (alarm_poz == 2)
                {
                    if(ora_alarm == ora_ceas_dig && min_alarm == minut_ceas_dig)
                    {
                        mciSendString("play alarm from 0", NULL, 0, NULL);
                        alarm_poz = 0;
                        ora_alarm = 0;
                        min_alarm = 0;
                        redraw=true;
                    }
                }
            }
        }

        while(ismouseclick(WM_LBUTTONDOWN))
        {
            int mx, my;
            getmouseclick(WM_LBUTTONDOWN, mx, my);
            //Plus
            if(clickElipsaDigitala(mx, my, bPlus_Digital_x1, bPlus_Digital_y1, bPlus_Digital_x2, bPlus_Digital_y2))
            {
                if (alarm_poz == 1)
                {
                    if (timp_switch == false)
                    {
                        min_alarm++;
                        if (min_alarm >= 60)
                        {
                            min_alarm = 0;
                        }
                    }
                    else
                    {
                        ora_alarm++;
                    }

                    if(ora_alarm > 24) ora_alarm = 0;
                }
                else if(contor_cal == 0 && alarm_poz != 1)
                {
                    if (timp_switch == false)
                    {
                        minut_ceas_dig++;
                        if(minut_ceas_dig >= 60)
                        {
                            minut_ceas_dig = 0;
                            ora_ceas_dig++;
                        }
                    }
                    else
                    {
                        ora_ceas_dig++;
                    }

                    if(ora_ceas_dig >= 24)
                    {
                        ora_ceas_dig = 0;
                        zi++;
                        zi_sapt++;
                        if(zi_sapt > 6) zi_sapt = 0;
                        limit = zile_luna[luna];
                        if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                        {
                            limit = 29;
                        }
                        if(zi > limit)
                        {
                            zi = 1;
                            luna++;
                            if(luna > 11)
                            {
                                luna = 0;
                                an++;
                            }
                        }
                    }
                }
                else if(contor_cal == 1 && alarm_poz != 1)
                {
                    zi++;
                    zi_sapt++;
                    if(zi_sapt > 6)
                    {
                        zi_sapt = 0;
                    }
                    limit = zile_luna[luna];
                    if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                    {
                        limit = 29;
                    }
                    if(zi > limit)
                    {
                        zi = 1;
                        luna++;
                        if(luna > 11)
                        {
                            luna = 0;
                            an++;
                        }
                    }
                }
                else if(contor_cal == 2 && alarm_poz != 1)
                {
                    luna++;
                    if(luna > 11)
                    {
                        luna = 0;
                        an++;
                    }
                    limit = zile_luna[luna];
                    if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                    {
                        limit = 29;
                    }
                    if(zi > limit)
                    {
                        zi = limit;
                    }
                    zi_sapt = get_day_of_week(zi, luna, an);
                }
                else if(contor_cal == 3 && alarm_poz != 1)
                {
                    an++;
                    if(luna == 1 && zi == 29)
                    {
                        if (! (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                        {
                            zi = 28;
                        }
                    }
                    zi_sapt = get_day_of_week(zi, luna, an);
                }
            }
            //Minus
            if(clickElipsaDigitala(mx, my, bMinus_Digital_x1, bMinus_Digital_y1, bMinus_Digital_x2, bMinus_Digital_y2))
            {
                if (alarm_poz == 1)
                {
                    if (timp_switch == false)
                    {
                        min_alarm--;
                        if (min_alarm < 0)
                        {
                            min_alarm = 59;
                        }
                    }
                    else
                    {
                        ora_alarm--;
                    }

                    if(ora_alarm < 0) ora_alarm = 23;
                }
                else if(contor_cal == 0 && alarm_poz != 1)
                {
                    if (timp_switch == false)
                    {
                        minut_ceas_dig--;
                        if(minut_ceas_dig < 0)
                        {
                            minut_ceas_dig = 59;
                            ora_ceas_dig--;
                        }
                    }
                    else
                    {
                        ora_ceas_dig--;
                    }

                    if(ora_ceas_dig < 0)
                    {
                        ora_ceas_dig = 23;
                        zi--;
                        zi_sapt--;
                        if(zi_sapt < 0) zi_sapt = 6;
                        if(zi < 1)
                        {
                            luna--;
                            if(luna < 0)
                            {
                                luna = 11;
                                an--;
                            }
                            limit = zile_luna[luna];
                            if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                            {
                                limit = 29;
                            }
                            zi = limit;
                        }
                    }
                }
                else if(contor_cal == 1 && alarm_poz != 1)
                {
                    zi--;
                    zi_sapt--;
                    if(zi_sapt < 0) zi_sapt = 6;

                    if(zi < 1)
                    {
                        luna--;
                        if(luna < 0)
                        {
                            luna = 11;
                            an--;
                        }
                        limit = zile_luna[luna];
                        if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                        {
                            limit = 29;
                        }
                        zi = limit;
                        zi_sapt = get_day_of_week(zi, luna, an);
                    }
                }
                else if(contor_cal == 2 && alarm_poz != 1)
                {
                    luna--;
                    if(luna < 0)
                    {
                        luna = 11;
                        an--;
                    }
                    limit = zile_luna[luna];
                    if (luna == 1 && (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                    {
                        limit = 29;
                    }
                    if(zi > limit)
                    {
                        zi = limit;
                    }
                    zi_sapt = get_day_of_week(zi, luna, an);
                }
                else if(contor_cal == 3 && alarm_poz != 1)
                {
                    an--;
                    if(luna == 1 && zi == 29)
                    {
                        if (! (an % 4 == 0 && (an % 100 != 0 || an % 400 == 0)))
                        {
                            zi = 28;
                        }
                    }
                    zi_sapt = get_day_of_week(zi, luna, an);
                }
            }
            //Timp switch
            if(clickElipsaDigitala(mx, my, TimpSwitch_Digital_x1, TimpSwitch_Digital_y1, TimpSwitch_Digital_x2, TimpSwitch_Digital_y2))
            {
                timp_switch = !timp_switch;
            }
            //Fus
            if(esteClickInButon(mx, my, bFusDig_x1, bFusDig_y1, bFusDig_x2, bFusDig_y2))
            {
                decc++;
                if (decc > 4) decc = 0;
                timp_curent(decalaje[decc]);
                ora_ceas_dig = ora_reala;
                minut_ceas_dig = minut;
                sec_ceas_dig = sec;
                t1 = clock();
                redraw = true;
            }
            //Fundal
            if(esteClickInButon(mx, my, bFundal_Digital_x1, bFundal_Digital_y1, bFundal_Digital_x2, bFundal_Digital_y2))
            {
                fundal_cnt++;
                redraw = !redraw;
            }
            //Alarma
            if(esteClickInButon(mx, my, bAlarm_Dig_x1, bAlarm_Dig_y1, bAlarm_Dig_x2, bAlarm_Dig_y2))
            {
                alarm_poz++;
                redraw=true;
                if (alarm_poz == 1)
                {
                    ora_alarm = 0;
                    min_alarm = 0;
                }
                else if (alarm_poz == 2)
                {
                    if (ora_alarm == 0 && min_alarm == 0)
                    {
                        alarm_poz = 0;
                    }
                }
                else if (alarm_poz > 2)
                {
                    alarm_poz = 0;
                    alarm_val = 0;
                    mciSendString("stop alarm", NULL, 0, NULL);
                    redraw=true;
                }
            }
            //Calendar
            if(esteClickInButon(mx, my, bCalDig_x1, bCalDig_y1, bCalDig_x2, bCalDig_y2))
            {
                contor_cal++;
                if(contor_cal > 3)
                {
                    contor_cal = 0;
                }
                redraw = true;
            }
            //Schimb ceas
            if (esteClickInButon(mx, my, bClockSwitch_x1, bClockSwitch_y1, bClockSwitch_x2, bClockSwitch_y2))
            {
                free(imagine_fundal_1);
                mciSendString("close alarm", NULL, 0, NULL);
                return 0;
            }
            //Resetare
            if (esteClickInButon(mx, my, bResetDigital_x1, bResetDigital_y1, bResetDigital_x2, bResetDigital_y2))
            {
                timp_curent(decalaje[decc]);
                ora_ceas_dig = ora_reala;
                minut_ceas_dig = minut;
                sec_ceas_dig = sec;
                t1 = clock();
            }
        }
        delay(10);
    }
    free(imagine_fundal_1);
    mciSendString("close alarm", NULL, 0, NULL);
    return -1;
}

int main()
{
    int clock_mode = 0;
    while(clock_mode != -1)
    {
        initwindow(1000, 800, "Ceas");
        if(clock_mode == 0)
        {
            clock_mode = Analog();
        }
        else if(clock_mode == 1)
        {
            clock_mode = Digital();
        }
        closegraph();
    }
    return 0;
}
