#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define WIDTH 120
#define HEIGHT 30
#define DELAY 0

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // Open serial port
    hSerial = CreateFile("COM4",
                         GENERIC_READ,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         0,
                         NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening COM4\n");
        return 1;
    }

    // Configure serial port
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutConstant = 1;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    SetCommTimeouts(hSerial, &timeouts);

    char buf[256];
    DWORD bytesRead;

    int graph[WIDTH] = {0};
    int pos = 0;

    // Print empty graph once
    for (int row = HEIGHT-1; row >= 0; row--) {
        for (int col = 0; col < WIDTH; col++) printf(" ");
        printf("\n");
    }

    while (1) {
        if (ReadFile(hSerial, buf, sizeof(buf)-1, &bytesRead, NULL) && bytesRead > 0) {
            buf[bytesRead] = '\0';

            int adc_val;
            if (sscanf(buf, "ADC: %d", &adc_val) == 1) {
                int y = adc_val * HEIGHT / 1024;
                if (y >= HEIGHT) y = HEIGHT-1;
                if (y < 0) y = 0;

                int prev = graph[pos];
                graph[pos] = y;

                // Move cursor to top-left
                COORD coord = {0, 0};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

                // Draw graph row by row with scale
                for (int row = HEIGHT-1; row >= 0; row--) {
                    // Y-axis labels every 5 rows
                    if (row % 5 == 0) {
                        int voltage_mv = (row * 3300) / HEIGHT;
                        printf("%4dmV|", voltage_mv);
                    } else {
                        printf("     |");
                    }
                    
                    for (int col = 0; col < WIDTH; col++) {
                        int idx = (pos + col + 1) % WIDTH;
                        int idx_prev = (pos + col) % WIDTH;

                        if (graph[idx] == row) printf("*");
                        else printf(" ");
                    }
                    printf("\n");
                }
                
                // X-axis
                printf("     +");
                for (int i = 0; i < WIDTH; i++) printf("-");
                printf("\n");

                printf("ADC: %4d (%u.%02uV) | Samples: %d\n", 
                       adc_val, 
                       (adc_val * 3300 / 1024) / 1000,
                       ((adc_val * 3300 / 1024) % 1000) / 10,
                       pos);
                pos = (pos + 1) % WIDTH;
                if (DELAY > 0) Sleep(DELAY);
            }
        }
    }

    CloseHandle(hSerial);
    return 0;
}
