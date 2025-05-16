#include <windows.h>
#include <stdio.h>
#include "array_shuffle.h"

#define one_digit_2_char(x) ((char) (x+48))
#define HMENU_PAUSE 0xc9
#define HMENU_HIDETIMER 0xca
#define F5 698
#define D5 587
#define B4 493
#define G4 391

void edit_char_arr(char arr[4], int num){

    if(num<10){
        arr[0] = '0';
        arr[1] = one_digit_2_char(num);
        arr[2] = ' ';
    }else if(num<100){
        arr[0] = one_digit_2_char( (num-(num%10))/10 );
        arr[1] = one_digit_2_char(num%10);
        arr[2] = ' ';
    }else if(num==100){
        arr[0] = '1';
        arr[1] = '0';
        arr[2] = '0';
    }

    arr[3] = '\0';

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Killkiss
//～人は壊れてゆく～
//～可笑しいわね～
//Global Variable Area
unsigned short int next = 1;
unsigned short int sec_spent = 0;
unsigned short int min_spent = 0;
HWND next_bar = NULL;
HWND time_bar = NULL;
//HWND timer = NULL;
HWND pause = NULL;
HWND show_time_bar = NULL;
HWND btn_handles[100];
// A list of each button's label and their position in the btn_handles
//[6, 65, 45, 54]
// |
// |
//Btn with the label 1, and its handle is btn_handles[6]
int list[100];

bool paused = false;
//End Of Global Variable Area
//～Ave mujica, 奇跡を日常に～
//AVE MUJICA


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    
    WNDCLASS game = {0};
    game.lpfnWndProc = WindowProc;
    game.hInstance = hInstance;
    game.lpszClassName = "theGame";
    game.hCursor = LoadCursor(0, IDC_ARROW);
    game.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    RegisterClass(&game);

    HWND root = CreateWindowEx(
        0,
        "theGame",
        "100 click!",        
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,

        0x1c2, 0x46, 0x3f9, 0x3b6, //x, y, width, height   

        NULL,     
        NULL,     
        hInstance 
        , NULL    
    );

    if(root == NULL){
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
    case WM_CREATE: 
    {

        struct arrWithLen shuffled_100 = new_arr(100);
        for(int i=0; i<100; i++){
            arr_set(&shuffled_100, i, i+1);
        }
        arr_shuffle(&shuffled_100, 4);
        for(int i=0; i<100; i++){
            list[arr_get(shuffled_100, i)-1] = i;
        }
        char number_str[4];

        for(int i=0; i<100; i++){

            edit_char_arr(number_str, arr_get(shuffled_100, i));

            btn_handles[i] = CreateWindow(
                TEXT("BUTTON"),
                (LPSTR) number_str,
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,

                (0+100*(i%10)), (0+85*((i-i%10)/10)), 100, 85,

                hwnd,
                (HMENU) arr_get(shuffled_100, i),
                (HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );
        }

        free_arr(&shuffled_100);

        next_bar = CreateWindow(
            "STATIC",
            (LPSTR) "Next: 1",
            WS_VISIBLE | WS_CHILD,
            0, 870, 100, 100,
            hwnd,
            NULL,
            GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );

        if(next_bar==NULL){
            PostQuitMessage(0);
        }

        time_bar = CreateWindow(
            "STATIC",
            (LPSTR) "00:00",
            WS_VISIBLE | WS_CHILD,
            450, 870, 150, 100,
            hwnd,
            NULL,
            GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );

        pause = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Pause"),
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            700, 855, 70, 50,
            hwnd,
            (HMENU) HMENU_PAUSE,
            GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );

        show_time_bar = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Hide Stopwatch"),
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            850, 855, 150, 50,
            hwnd,
            (HMENU) HMENU_HIDETIMER,
            GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );

        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:

        if(LOWORD(wParam) == HMENU_PAUSE){
            paused = !paused;
            if(paused){
                SetWindowText(pause, (LPSTR) "Continue");
            }else{
                SetWindowText(pause, (LPSTR) "Pause");
            }
            break;
        }


        if(LOWORD(wParam) == HMENU_HIDETIMER){

            if(IsWindowVisible(time_bar)){
                ShowWindow(time_bar, SW_HIDE);
                SetWindowText(show_time_bar, (LPSTR) "Show Stopwatch");
            }else{
                ShowWindow(time_bar, SW_NORMAL);
                SetWindowText(show_time_bar, (LPSTR) "Hide Stopwatch");
            }
            
            break;
        }

        char next_str[10];

        if( (LOWORD(wParam)==1)&&(next==1) ){
            HWND a = SetTimer(
                hwnd,
                0xc8,   
                1000,  //1 second
                (TIMERPROC) NULL
            );

            if(a==NULL){
                PostQuitMessage(1);
            }
        }

        if(LOWORD(wParam) == ((int) next)){
            
            if(paused){
                break;
            }

            unsigned short int temp = list[next-1];
            // temp is the index of the handle of the btn with correspoding label in the btn_handles

            if(next!=100){
                next += 1;
            }else{
                KillTimer(hwnd, 0xc8);
                Beep(F5, 800);
                Beep(D5, 800);
                Beep(B4, 800);
                Beep(G4, 1600);
                char congrat[41];
                char time_str[5];
        
                if(min_spent<10){
                    time_str[0] = 0x30;
                    time_str[1] = one_digit_2_char(min_spent);
                }else{
                    time_str[0] = one_digit_2_char((min_spent-min_spent%10)/10);
                    time_str[1] = one_digit_2_char(min_spent%10);
                }
                
                time_str[2] = 0x3a;

                if(sec_spent<10){
                    time_str[3] = 0x30;
                    time_str[4] = one_digit_2_char(sec_spent);
                }else{
                    time_str[3] = one_digit_2_char((sec_spent-sec_spent%10)/10);
                    time_str[4] = one_digit_2_char(sec_spent%10);
                }

                sprintf(&congrat, "Congrats, you finished the game in %s !", time_str);
                SetWindowText(time_bar, congrat);
            }
            
            next_str[0] = 'N';
            next_str[1] = 'e';
            next_str[2] = 'x';
            next_str[3] = 't';
            next_str[4] = ':';
            next_str[5] = ' ';

            if(next==100){
                next_str[6] = '1';
                next_str[7] = '0';
                next_str[8] = '0';
            }else if(next>9){
                next_str[6] = one_digit_2_char( (next-(next%10))/10 );
                next_str[7] = one_digit_2_char(next%10);
                next_str[8] = ' ';
            }else{
                next_str[6] = one_digit_2_char(next);
                next_str[7] = ' ';
                next_str[8] = ' ';
            }
            next_str[9] = 0;

            SetWindowText(next_bar, (LPSTR) next_str);
            SetWindowText(btn_handles[temp], "<OK>");

        }

        break;
    
    case WM_TIMER:

        if(paused){
            break;
        }

        if(sec_spent!=59){
            sec_spent += 1;
        }else{
            sec_spent = 0;
            min_spent += 1;
        }
        
        char time_str[6];
        
        if(min_spent<10){
            time_str[0] = 0x30;
            time_str[1] = one_digit_2_char(min_spent);
        }else{
            time_str[0] = one_digit_2_char((min_spent-min_spent%10)/10);
            time_str[1] = one_digit_2_char(min_spent%10);
        }
        
        time_str[2] = 0x3a;

        if(sec_spent<10){
            time_str[3] = 0x30;
            time_str[4] = one_digit_2_char(sec_spent);
        }else{
            time_str[3] = one_digit_2_char((sec_spent-sec_spent%10)/10);
            time_str[4] = one_digit_2_char(sec_spent%10);
        }

        time_str[5] = 0x0;

        SetWindowText(time_bar, (LPSTR) time_str);

        break;
        
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}