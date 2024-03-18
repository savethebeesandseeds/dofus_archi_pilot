#pragma once
#include "dconfig.h"
#include "dutils.h"
#include "dwin_api.h"

void pilot_capture_loop() {
    log_info("pilot_capture_loop() started\n");
    Sleep(100); /* to let some set up gap for the other threads */

    /* Initialize the heap variables */
    char *window = NULL;
    char *temp_dir = NULL;
    char output_file[MAX_PATH];
    int delay = -1;
    long int count = 0;

    /* read the configuration */
    window = config_window();
    temp_dir = config_temp_dir_pilot();
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
        sprintf(output_file, "%s/window_capture_%ld.bmp", temp_dir, count);
        if (!CaptureWindow(hWnd, output_file)) {
            log_error("Unable to capture [pilot] image: %s\n", win_error_message());
            free(window);
            free(temp_dir);
            /* Wait a minute and restart */
            Sleep(60000);
            log_debug("pilot_capture_loop() restarted\n");
            pilot_capture_loop();
        }

        /* Delay */
        Sleep(delay);
        count++;
    }

    free(window);
    free(temp_dir);

    log_debug("pilot_capture_loop() ended\n");
}