#include <iostream>
#include <windows.h>

unsigned int keyCode(char chr) { return (unsigned int)0x41 + chr - 'a'; } //a function from ascii to key codes
const int modifierKey = VK_CAPITAL; //the modifier key used
const char shiftKey = keyCode('d');
const char altTabKey = 188;
bool keyPressed = 0; //a bool representing if a key was pressed before the modifier key was released
bool sendingKey = false; //a flag to re-enable key presses when the program itself is sending key presses
bool shiftKeyHeld = false; //a flag to know if the shift key is currently held down by the program
bool altTabKeyHeld = false; //a flag to know if the altTab key is currently held down by the program
#define KEYEVENTF_KEYDOWN 0 //why doesn't that already exist???
const int keyDelay = 0; //ms //the delay between output key presses
//a struct containing a character and a pointer to a handler function
struct keyFunc{
    unsigned int chr;
    void (*func)(unsigned int);
};
//some handler functions
void keyFuncMap(unsigned int vkCode);
void keyCtrlFuncMap(unsigned int vkCode);
void keySelectWordFunc(unsigned int vkCode);


//the handler function associated with each key
keyFunc keyFuncs[] = {
    //{input key, handler function}
    {keyCode('u'), keyCtrlFuncMap},
    {keyCode('o'), keyCtrlFuncMap},
    {keyCode('i'), keyFuncMap},
    {keyCode('j'), keyFuncMap},
    {keyCode('k'), keyFuncMap},
    {keyCode('l'), keyFuncMap},
    {keyCode('m'), keyFuncMap},
    {190/* . */  , keyFuncMap},
    {keyCode('p'), keyCtrlFuncMap},
    {keyCode('p'), keyFuncMap},
    {keyCode('p'), keyCtrlFuncMap},
    {keyCode('p'), keyFuncMap},
    {186/* ; */  , keyCtrlFuncMap},
    {186/* ; */  , keyFuncMap},
    {186/* ; */  , keyCtrlFuncMap},
    {186/* ; */  , keyFuncMap},
    {VK_BACK     , keyFuncMap},
};
//the key map used in handler functions
unsigned int keyMap[][2] = {
    //{input key, output key}
    {keyCode('u'), VK_LEFT},
    {keyCode('o'), VK_RIGHT},
    {keyCode('i'), VK_UP},
    {keyCode('j'), VK_LEFT},
    {keyCode('k'), VK_DOWN},
    {keyCode('l'), VK_RIGHT},
    {keyCode('m'), VK_HOME},
    {190/* . */  , VK_END},
    {keyCode('p'), VK_UP},
    {186/* ; */  , VK_DOWN},
    {VK_BACK     , VK_DELETE},
};

//function that presses a key from keyMap
void keyFuncMap(unsigned int vkCode){
    //find the key from keyMap
    for(auto km : keyMap){
        if(km[0] == vkCode){
            //send the corresponding key code
            sendingKey = true;
            keybd_event(km[1], 0, KEYEVENTF_KEYDOWN, 0);
            Sleep(keyDelay);
            keybd_event(km[1], 0, KEYEVENTF_KEYUP, 0);
            sendingKey = false;
        }
    }
}
//function that presses a key from keyMap with Ctrl held down
void keyCtrlFuncMap(unsigned int vkCode){
    sendingKey = true;
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYDOWN, 0);
    keyFuncMap(vkCode);
    sendingKey = true;
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    sendingKey = false;
}
//function that presses keys to select a word
void keySelectWordFunc(unsigned int vkCode){
    sendingKey = true;

    //CTRL + LEFT
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYDOWN, 0);

    keybd_event(VK_LEFT, 0, KEYEVENTF_KEYDOWN, 0);
    Sleep(keyDelay);
    keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);

    //CTRL + SHIFT + RIGHT
    keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYDOWN, 0);
    keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYDOWN, 0);
    Sleep(keyDelay);
    keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);

    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

    sendingKey = false;
}

//https://www.unknowncheats.me/forum/c-and-c-/83707-setwindowshookex-example.html
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){
    //get information about the key in kbdStruct
    KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
    //if the action is valid...
    if(nCode >= 0 /*valid action*/){
        //if a key is pressed (and is not the modifier key)...
        if(wParam == WM_KEYDOWN /*key pressed down*/ && kbdStruct.vkCode != modifierKey && !sendingKey){
            //if the modifier key is pressed, then call the corresponding handler function with the corresponding key map
            if(GetKeyState(modifierKey) & 0x8000){
                keyPressed = true;

                bool foundMatch = false;
                //if shiftKey key was pressed, then press SHIFT
                if((const char)kbdStruct.vkCode == shiftKey){
                    if(!shiftKeyHeld) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYDOWN, 0);
                    shiftKeyHeld = true;
                    foundMatch = true;
                }
                //if altTabKey key was pressed, then press CTRL + ALT + TAB
                if((const char)kbdStruct.vkCode == altTabKey){
                    if(!altTabKeyHeld){
                        keybd_event(VK_MENU, 0, KEYEVENTF_KEYDOWN, 0);
                        keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYDOWN, 0);
                    }
                    sendingKey = true;
                    keybd_event(VK_TAB, 0, KEYEVENTF_KEYDOWN, 0);
                    Sleep(keyDelay);
                    keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
                    sendingKey = false;
                    foundMatch = true;
                }
                
                for(auto kf : keyFuncs)
                    if(kf.chr == kbdStruct.vkCode){
                        foundMatch = true;
                        kf.func(kbdStruct.vkCode);
                        std::cout << "\rCAPS + " << kbdStruct.vkCode << " detected. Sending corresponding key.  "; //spaces for vkCode.str()
                    }
                //if no match was found, press CTRL + key
                if(!foundMatch){
                    sendingKey = true;
                    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYDOWN, 0);
                    keybd_event((unsigned int)kbdStruct.vkCode, 0, KEYEVENTF_KEYDOWN, 0);
                    Sleep(keyDelay);
                    keybd_event((unsigned int)kbdStruct.vkCode, 0, KEYEVENTF_KEYUP, 0);
                    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
                    sendingKey = false;
                    std::cout << "\rCAPS + " << kbdStruct.vkCode << " detected. Sending corresponding key.  "; //spaces for vkCode.str()
                }
                //https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644985(v=vs.85) in section 'Return value'
                //prevent the hook from propagating (aka freaking disable the keyboard), but allow modifier keys to be pressed
                unsigned int c = kbdStruct.vkCode;
                if(c != VK_SHIFT && c != VK_LSHIFT && c != VK_RSHIFT
                && c != VK_CONTROL && c != VK_LCONTROL && c != VK_RCONTROL
                && c != VK_MENU && c != VK_LMENU && c != VK_RMENU) return 1;
            }
        }
        else if(wParam == WM_KEYUP /*key released*/ && kbdStruct.vkCode != modifierKey && !sendingKey){
            //if shiftKey key was released, then release SHIFT
            if(kbdStruct.vkCode == shiftKey){
                if(shiftKeyHeld) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
                shiftKeyHeld = false;
            }
        }
        //if the modifier key was released...
        else if(wParam == WM_KEYUP /*key released*/ && kbdStruct.vkCode == modifierKey){
            //if modifierKey key was released and altKey was pressed, then release ALT
                std::cout << "RELEASING ALT" << std::endl;
            if(altTabKeyHeld){
                keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
                altTabKeyHeld = false;
            }
            //if another key was pressed before the modifier key...
            if(keyPressed == true){
                //turn the Caps Lock 'Light' back off by simulating a key press
                keyPressed = false;
                Sleep(keyDelay);
                keybd_event(modifierKey, 0, KEYEVENTF_KEYUP, 0);
                Sleep(keyDelay);
                keybd_event(modifierKey, 0, KEYEVENTF_KEYDOWN, 0);
                Sleep(keyDelay);
                keybd_event(modifierKey, 0, KEYEVENTF_KEYUP, 0);
            }
        }
    }else{
        std::cout << "\r" << "Invalid Action. Calling next hook.                 " << std::endl;
    }
    //otherwise, do not intercept the key press
    return CallNextHookEx(NULL /*ignored*/, nCode, wParam, lParam);
}

int main()
{
    //print some information about the program
    std::cout << "This program uses pre-defined key combinations to type other keys" << std::endl;
    std::cout << "Here are some key combinations that are included in the program:" << std::endl;
    std::cout << "  - CAPS + u:  CTRL + left arrow" << std::endl;
    std::cout << "  - CAPS + o:  CTRL + right arrow" << std::endl;
    std::cout << "  - CAPS + i:  up arrow" << std::endl;
    std::cout << "  - CAPS + j:  left arrow" << std::endl;
    std::cout << "  - CAPS + k:  down arrow" << std::endl;
    std::cout << "  - CAPS + l:  right arrow" << std::endl;
    std::cout << "  - CAPS + m:  home" << std::endl;
    std::cout << "  - CAPS + .:  end" << std::endl;
    std::cout << "  - CAPS + p:  scroll up" << std::endl;
    std::cout << "  - CAPS + ;:  scroll down" << std::endl;
    std::cout << "  - CAPS + ,:  select current word" << std::endl;
    std::cout << "  - CAPS + BS: del" << std::endl;
    std::cout << "  - CAPS + d:  SHIFT" << std::endl;
    std::cout << "  - CAPS + ?:  CTRL + ?" << std::endl << std::endl;
    std::cout << "Listenning for HotKeys." << std::endl << std::endl;

    //register a key hook (the secret to making this work)
    HHOOK keybdHook;
    //https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowshookexa
    if(!(keybdHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0))){
        std::cout << "An error occured when registering the keyboard hook." << std::endl;
    };

    //this is needed for some reason
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){}

    return 0;
}
