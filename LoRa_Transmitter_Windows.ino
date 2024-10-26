#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    char userInput[256];

    // Open the COM port (replace COM1 with your actual COM port)
    hSerial = CreateFile(
        "COM3",          // Port name
        GENERIC_WRITE | GENERIC_READ,  // Write and read access
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

    // Continuously ask for input and send to the receiver
    while (1) {
        printf("Enter a message to send (or 'exit' to quit): ");
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = 0; // Remove newline character

        // Exit condition
        if (strcmp(userInput, "exit") == 0) {
            break;
        }

        // Send user input to COM port
        DWORD bytes_written;
        if (!WriteFile(hSerial, userInput, strlen(userInput), &bytes_written, NULL)) {
            printf("Error writing to COM port\n");
        } else {
            printf("Message sent: %s\n", userInput);
        }

        Sleep(500);  // Wait for a bit before sending the next message
    }

    CloseHandle(hSerial);
    return 0;
}
