#include <iostream>
#include <windows.h>
#include <vector>


//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerhotkey
bool wasCapsLockHeld = false;
bool typedModifierKey = false;
int main()
{
    const int keyDelay = 2; //ms
    const int modifierKey = VK_CAPITAL; //165 or VK_CAPITAL (165=AltCar)
    //https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    std::vector<std::vector<int>> keys{
      //{input key, output key} (NULL are special cases)
        {0x41 - 'a' + 'm', NULL},
        {0x41 - 'a' + 'n', NULL},
        {0x41 - 'a' + 'u', VK_LEFT},
        {0x41 - 'a' + 'o', VK_RIGHT},
        {0x41 - 'a' + 'i', VK_UP},
        {0x41 - 'a' + 'j', VK_LEFT},
        {0x41 - 'a' + 'k', VK_DOWN},
        {0x41 - 'a' + 'l', VK_RIGHT},
        {188, VK_HOME},
        {190, VK_END},
        //{186, VK_HOME},
        //{192, VK_END}, //Canadian Multilingual Keyboard
        //{222, VK_END}, //US Keyboard
        //{221/*]*/, VK_ESCAPE},
        {8/*BS*/, VK_DELETE},
    };
    const int uModifierKeys = MOD_CONTROL | MOD_ALT;
    for(unsigned i = 0; i < keys.size(); i++){
        RegisterHotKey(NULL, i + 0x0000, uModifierKeys, keys[i][0]);
        RegisterHotKey(NULL, i + 0x1000, uModifierKeys | MOD_SHIFT, keys[i][0]);
        //RegisterHotKey(NULL, i + 0x2000, uModifierKeys | MOD_CONTROL, keys[i][0]);
        //RegisterHotKey(NULL, i + 0x3000, uModifierKeys | MOD_SHIFT | MOD_CONTROL, keys[i][0]);
    }
    #define KEYEVENTF_KEYDOWN 0
    std::cout << "This program uses pre-defined key combinations to type other keys" << std::endl;
    std::cout << "Here are some key combinations that are included in the program:" << std::endl;
    std::cout << "  - CAPS + m: select current word" << std::endl;
    std::cout << "  - CAPS + n: select current line" << std::endl;
    std::cout << "  - CAPS + i: up arrow" << std::endl;
    std::cout << "  - CAPS + j: left arrow" << std::endl;
    std::cout << "  - CAPS + k: down arrow" << std::endl;
    std::cout << "  - CAPS + l: right arrow" << std::endl;
    std::cout << "  - CAPS + u: CTRL + left arrow" << std::endl;
    std::cout << "  - CAPS + o: CTRL + right arrow" << std::endl;
    std::cout << "  - CAPS + ,: home" << std::endl;
    std::cout << "  - CAPS + .: end" << std::endl;
    std::cout << "  - CAPS + BS: del" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Listenning for HotKeys." << std::endl;
    while(true){
        Sleep(keyDelay);
        //https://stackoverflow.com/questions/13905342/winapi-how-to-get-the-caps-lock-state
        if(GetKeyState(modifierKey) & 0x8000){
            for(unsigned short k = 0; k < 256; k++)
                if((GetAsyncKeyState(k) == -32767) && !(k == VK_LCONTROL || k == VK_RCONTROL || k == 164 || k == modifierKey || k == VK_CONTROL || k == VK_MENU)) typedModifierKey = true; //std::cout << (unsigned int)k << std::endl;
            if(!wasCapsLockHeld){
                keybd_event(VK_MENU, 0, KEYEVENTF_KEYDOWN, 0);
                Sleep(keyDelay);
                keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYDOWN, 0);
                typedModifierKey = false;
                wasCapsLockHeld = true;
            }
        }else{
            if(wasCapsLockHeld){
                keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
                Sleep(keyDelay);
                keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
                if(typedModifierKey){
                    Sleep(keyDelay);
                    keybd_event(modifierKey, 0, KEYEVENTF_KEYDOWN, 0);
                    Sleep(keyDelay);
                    keybd_event(modifierKey, 0, KEYEVENTF_KEYUP, 0);
                }
                typedModifierKey = false;
                wasCapsLockHeld = false;
            }
        }
        MSG msg = {0};
        //https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage
        PeekMessage(&msg, NULL, 0, 0, 0x0001);
        if(msg.message == WM_HOTKEY){
            if(msg.wParam >= 0x1000) msg.wParam -= 0x1000;
            //typedModifierKey = true;
            std::vector<int> keyPair = keys[msg.wParam & 0xff];
            //bool shiftHeld = shiftHeld || msg.wParam & 0x100;
            /*if(!(GetKeyState(modifierKey) & 0xf0)){
                keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
                Sleep(keyDelay);
                keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYDOWN, 0);
                keybd_event(keyPair[0], 0, KEYEVENTF_KEYDOWN, 0);
                Sleep(keyDelay);
                keybd_event(keyPair[0], 0, KEYEVENTF_KEYUP, 0);
                //keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
                //Sleep(keyDelay);
                //keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYDOWN, 0);
                continue;
            }*/
            std::cout << "CAPS + " << keyPair[0] << " detected. Sending Key: " << keyPair[1] << std::endl;
            if(msg.wParam == 0){
                keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
                //keybd_event(modifierKey, 0, KEYEVENTF_KEYUP, 0);

                keybd_event(VK_LEFT, 0, KEYEVENTF_KEYDOWN, 0);
                Sleep(keyDelay);
                keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);

                keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYDOWN, 0);
                keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYDOWN, 0);
                Sleep(keyDelay);
                keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);

                keybd_event(VK_MENU, 0, KEYEVENTF_KEYDOWN, 0);
                //keybd_event(modifierKey, 0, KEYEVENTF_KEYDOWN, 0);
                continue;
            }
            if(msg.wParam == 1){
                keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
                //keybd_event(modifierKey, 0, KEYEVENTF_KEYUP, 0);

                keybd_event(VK_HOME, 0, KEYEVENTF_KEYDOWN, 0);
                Sleep(keyDelay);
                keybd_event(VK_HOME, 0, KEYEVENTF_KEYUP, 0);

                keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYDOWN, 0);

                keybd_event(VK_END, 0, KEYEVENTF_KEYDOWN, 0);
                Sleep(keyDelay);
                keybd_event(VK_END, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYDOWN, 0);
                Sleep(keyDelay);
                keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);

                keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);

                keybd_event(VK_MENU, 0, KEYEVENTF_KEYDOWN, 0);
                keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYDOWN, 0);
                //keybd_event(modifierKey, 0, KEYEVENTF_KEYDOWN, 0);
                continue;
            }
            //http://www.cplusplus.com/forum/windows/47266/
            keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
            Sleep(keyDelay);
            if(msg.wParam < 4) keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYDOWN, 0);
            //keybd_event(modifierKey, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(keyPair[1], 0, KEYEVENTF_KEYDOWN, 0);
            Sleep(keyDelay);
            keybd_event(keyPair[1], 0, KEYEVENTF_KEYUP, 0);
            if(msg.wParam < 4) keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
            Sleep(keyDelay);
            keybd_event(VK_MENU, 0, KEYEVENTF_KEYDOWN, 0);
            keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYDOWN, 0);
            //keybd_event(modifierKey, 0, KEYEVENTF_KEYDOWN, 0);
        }
    }
}

