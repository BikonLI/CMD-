#include "KBM.h"

int KBM_QUIT = 0;

HANDLE KBM_MONITOR_THREAD_HANDLE = NULL;
DWORD KBM_MONITOR_THREAD_ID = 0;

HANDLE Mutex = NULL;

void is_down(int key, uint8_t *p);
void flash_ks(void);
void write_public_data(uint8_t *data, uint8_t content);
int read_public_data(uint8_t *data);
int is_change(void);
int _KBChange(void);

uint8_t KBM_KEYS_STATE[0x100] = {0};
uint8_t KBM_KEYS_EVENT[0x100] = {0}; // 0x80 changed; 0x01 pressed; Event[-1] 0x88 any changed
uint8_t ks_previous[0x100] = {0};
uint8_t ks_current[0x100] = {0};

DWORD WINAPI KeyStateUpdate(LPVOID lpParam)
{
    int i;
    while (!KBM_QUIT)
    {
        for (i = 0x41; i <= 0x5A; i++)
            is_down(i, &ks_current[i]);

        for (i = 0x30; i <= 0x39; i++)
            is_down(i, &ks_current[i]);

        is_down(VK_SPACE, &ks_current[VK_SPACE]);
        is_down(VK_TAB, &ks_current[VK_TAB]);
        is_down(VK_LSHIFT, &ks_current[VK_LSHIFT]);
        is_down(VK_LCONTROL, &ks_current[VK_LCONTROL]);
        is_down(VK_ESCAPE, &ks_current[VK_ESCAPE]);
        is_down(VK_BACK, &ks_current[VK_BACK]);
        is_down(VK_RETURN, &ks_current[VK_RETURN]);
        is_down(VK_RSHIFT, &ks_current[VK_RSHIFT]);
        is_down(VK_RCONTROL, &ks_current[VK_RCONTROL]);
        is_down(VK_UP, &ks_current[VK_UP]);
        is_down(VK_DOWN, &ks_current[VK_DOWN]);
        is_down(VK_LEFT, &ks_current[VK_LEFT]);
        is_down(VK_RIGHT, &ks_current[VK_RIGHT]);

        if (is_change())
        {
            flash_ks();
        }
    }
}

void MonitorStart(void)
{
    Mutex = CreateMutex(NULL, FALSE, NULL);

    KBM_MONITOR_THREAD_HANDLE = CreateThread(
        NULL,
        0,
        KeyStateUpdate,
        NULL,
        0,
        &KBM_MONITOR_THREAD_ID);
}

void MonitorEnd(void)
{
    KBM_QUIT = 1;

    CloseHandle(KBM_MONITOR_THREAD_HANDLE);
    CloseHandle(Mutex);
}

uint8_t KBChange(void)
{
    int flag;
    flag = read_public_data(&KBM_KEYS_EVENT[0xFF]);

    if (flag & 0x08)
    {
        if (flag == 0x88)
            flag = 0x80;
        else
            flag = 0x00;
        write_public_data(&KBM_KEYS_EVENT[0xFF], flag);
        return 1;
    }
    else
        return 0;
}

int _KBChange(void)
{
    int flag;
    flag = read_public_data(&KBM_KEYS_EVENT[0xFF]);

    if (flag & 0x80)
    {
        if (flag == 0x88)
            flag = 0x08;
        else
            flag = 0x00;
        write_public_data(&KBM_KEYS_EVENT[0xFF], flag);
        return 1;
    }
    else
        return 0;
}

uint8_t GetKeyCondition(int vKey)
{
    int result;
    result = read_public_data(&KBM_KEYS_STATE[vKey]);
    return result;
}

uint8_t GetKeyEvent(int vkey)
{
    int result;
    result = read_public_data(&KBM_KEYS_EVENT[vkey]);
    return result;
}

void *on_press(int vKey, void *(*KeyFunction)(void))
{
    void *ptr;
    int flag;

    flag = read_public_data(&KBM_KEYS_EVENT[vKey]) == 0x81;

    if (flag)
    {
        write_public_data(&KBM_KEYS_EVENT[vKey], 0x01);
        ptr = KeyFunction();
    }
    return ptr;
}

void *on_release(int vKey, void *(*KeyFunction)(void))
{
    void *ptr;
    int flag;

    flag = read_public_data(&KBM_KEYS_EVENT[vKey]) == 0x80;

    if (flag)
    {
        write_public_data(&KBM_KEYS_EVENT[vKey], 0x00);
        ptr = KeyFunction();
    }
    return ptr;
}

void CleanEvents(int vKey)
{
    int flag;
    flag = read_public_data(&KBM_KEYS_EVENT[vKey]);
    if (flag == 0x80)
        write_public_data(&KBM_KEYS_EVENT[vKey], 0x00);
    else if (flag == 0x81)
        write_public_data(&KBM_KEYS_EVENT[vKey], 0x01);
}

int is_change(void)
{
    int i, result = 0;
    for (i = 0; i < 0x100; i++)
    {
        if (ks_previous[i] != ks_current[i])
        {
            write_public_data(&KBM_KEYS_EVENT[0xFF], 0x88);
            result = 1;
            if (ks_current[i] == 1)
            {
                write_public_data(&KBM_KEYS_EVENT[i], 0x81);
            }
            else
            {
                write_public_data(&KBM_KEYS_EVENT[i], 0x80);
            }
        }
    }
    return result;
}

void flash_ks(void)
{
    int i;
    for (i = 0; i < 0x100; i++)
    {
        write_public_data(&KBM_KEYS_STATE[i], ks_current[i]); // flash ks
        ks_previous[i] = ks_current[i];
    }
}

void is_down(int key, uint8_t *p)
{
    if (GetAsyncKeyState(key) & 0x8000)
        *p = KBM_KEY_DOWN;
    else
        *p = KBM_KEY_UP;
}

void write_public_data(uint8_t *data, uint8_t content)
{
    WaitForSingleObject(Mutex, INFINITE);
    *data = content;
    ReleaseMutex(Mutex);
}

int read_public_data(uint8_t *data)
{
    int result;
    WaitForSingleObject(Mutex, INFINITE);
    result = *data;
    ReleaseMutex(Mutex);
    return result;
}
