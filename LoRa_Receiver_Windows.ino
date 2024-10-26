#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    char buffer[256];

    // Open the COM port (replace COM1 with your actual COM port)
    hSerial = CreateFile(
        "COM9",          // Port name
        GENERIC_READ,           // Read access
        0,                      // No sharing
        NULL,                   // No security
        OPEN_EXISTING,          // Open existing port only
        0,                      // Non-overlapped I/O
        NULL);                  // Null for template file

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening COM port\n");
        return 1;
    }

    // Set device parameters (9600 baud, 8 data bits, no parity, 1 stop bit)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error getting COM state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error setting COM state\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Continuously listen for data
    while (1) {
        DWORD bytes_read;
        memset(buffer, 0, sizeof(buffer));  // Clear buffer

        // Read data from COM port
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytes_read, NULL) && bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null-terminate the string
            printf("Received message: %s\n", buffer);
        }

        Sleep(500);  // Wait before checking for more data
    }

    CloseHandle(hSerial);
    return 0;
}
