#include <iostream>
#include <windows.h>
#include <random>

void SendLeftClick() {
    INPUT inputs[3] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(3, inputs, sizeof(INPUT));
}

void SendRightClick() {
    INPUT inputs[3] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(3, inputs, sizeof(INPUT));
}

void UpdateStatus(bool leftActive, bool rightActive) {
    std::cout << "\rLeft Click: " << (leftActive ? "\033[1;32m[ON]\033[0m" : "\033[1;31m[OFF]\033[0m")
              << "  Right Click: " << (rightActive ? "\033[1;32m[ON]\033[0m" : "\033[1;31m[OFF]\033[0m")
              << "    " << std::flush;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(20, 35);
    // Feature States
    bool leftActive = false;
    bool rightActive = false;
    // Key State Tracking
    bool leftKeyWasDown = false;
    bool rightKeyWasDown = false;
    bool leftMouseAlreadyClicked = false;
    bool rightMouseAlreadyClicked = false;

    std::cout << "========================================" << std::endl;
    std::cout << "       MOUSE DOUBLE CLICKER MACRO        " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "[LEFT ARROW]     : Double Left Click" << std::endl;
    std::cout << "[RIGHT ARROW]    : Double Right Click" << std::endl;
    std::cout << "[END]            : Exit Program" << std::endl;
    std::cout << "========================================" << std::endl;

    UpdateStatus(leftActive, rightActive);

    while (true) {
        bool statusChanged = false;

        // Left
        bool leftKeyIsDown = GetAsyncKeyState(VK_LEFT) & 0x8000;
        if (leftKeyIsDown && !leftKeyWasDown) {
            leftActive = !leftActive;
            Beep(leftActive ? 800 : 400, 190);
            statusChanged = true;
        }
        leftKeyWasDown = leftKeyIsDown;

        // Right
        bool rightKeyIsDown = GetAsyncKeyState(VK_RIGHT) & 0x8000;
        if (rightKeyIsDown && !rightKeyWasDown) {
            rightActive = !rightActive;
            Beep(rightActive ? 1200 : 600, 190);
            statusChanged = true;
        }
        rightKeyWasDown = rightKeyIsDown;

        if (statusChanged) {
            UpdateStatus(leftActive, rightActive);
        }
        // Left Click
        if (leftActive) {
            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                if (!leftMouseAlreadyClicked) {
                    Sleep(dis(gen)); 
                    SendLeftClick();
                    leftMouseAlreadyClicked = true; 
                }
            } else {
                leftMouseAlreadyClicked = false;
            }
        }
        // Right Click
        if (rightActive) {
            if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
                if (!rightMouseAlreadyClicked) {
                    Sleep(dis(gen)); 
                    SendRightClick();
                    rightMouseAlreadyClicked = true; 
                }
            } else {
                rightMouseAlreadyClicked = false;
            }
        }
        //EXIT
        if (GetAsyncKeyState(VK_END) & 0x8000) {
            std::cout << "\nExiting..." << std::endl;
            break;
        }
        Sleep(1);
    }
    return 0;
}