#pragma once
#include "dconfig.h"
#include "dutils.h"
#include "dwin_api.h"

void capture_loop() {
    log_info("capture_loop() started\n");
    Sleep(100); /* to let some set up gap for the other threads */

    /* Initialize the heap variables */
    char *window = NULL;
    char *temp_dir = NULL;
    char output_file[MAX_PATH];
    int delay = -1;
    long int count = 0;

    /* read the configuration */
    window = config_window();
    temp_dir = config_temp_dir();
    delay = config_period_sample();
    
    if (window == NULL || temp_dir == NULL || delay == -1) { 
        log_error("Bad config\n"); 
        free(window);
        free(temp_dir);
        return;
    }

    /* Capture the window */
    HWND hWnd = FindWindowByPartialTitle(window);
    if (hWnd == NULL) {
        log_error("Window [%s] not found: %s\n", window, win_error_message());
        free(window);
        free(temp_dir);
        return;
    }

    /* Loop to capture events */
    while(0xF) {

        /* Further actions here... */
        sprintf(output_file, "../data/temp/window_capture_%ld.bmp", count);
        if (!CaptureWindow(hWnd, output_file)) {
            log_error("Unable to capture image: %s\n", win_error_message());
        }

        /* Delay */
        Sleep(delay);
        count++;
    }

    free(window);
    free(temp_dir);

    log_debug("capture_loop() ended\n");
}