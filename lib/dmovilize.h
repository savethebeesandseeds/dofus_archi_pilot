#pragma once
#include "dconfig.h"
#include "dschedule.h"

/* to know detect when the character has stop moving */
cv::Mat pastBoxImg;
std::chrono::high_resolution_clock::time_point pastBoxTime = std::chrono::high_resolution_clock::now();

bool is_new_zone(cv::Mat current_img) {
    /* determine the difference between the past and the new image */
    bool ret = false;

    /* compare only with a subset of the image */
    cv::Mat newBoxImg = createSubImage(current_img, config_box_change_x(), config_box_change_y(), config_box_change_size(), config_box_change_size());
    if(pastBoxImg.cols != newBoxImg.cols) {
        ret = true;
    } else {
        /* calculate the L2 norm difference of the subsets */
        double norm_diff = compareImagesByNorm(pastBoxImg, newBoxImg);
        if(norm_diff > config_box_change_diff_norm_threshold()) {
            /* update, log and return */
            log_debug("Map has changed with diff: %f\n", norm_diff);
            ret = true;
        }
    }

    pastBoxImg = newBoxImg;

    return ret;
}

/* to move the pilot arround following a schedule */
void step_pilot(cv::Mat current_img) {
    if(!config_do_schedule()) {
        return;
    }
    /* how long has it been since the last image change */
    auto newBoxTime = std::chrono::high_resolution_clock::now();
    if(is_new_zone(current_img)) {
        /* update the pastBoxTime */
        pastBoxTime = newBoxTime;
    }
    std::chrono::duration<double> elapsed = newBoxTime - pastBoxTime;
    double delta_seconds = elapsed.count();
    if(delta_seconds > config_delta_position_threshold()) {
        /* sufficient time has enlapsed since the last screen update */
        pastBoxTime = newBoxTime;
        char buff[256];
        buff[0] = '\0';

        /* capture the character window */
        char *window = config_window();
        HWND hWnd = FindWindowByPartialTitle(window);
        free(window);
        sprintf(buff, "/travel %s", get_next_schedule_location().c_str());

        /* proceed to send the commands and move */
        sendCtrlEnter(hWnd);
        Sleep(251);
        sendText(hWnd, buff);
        Sleep(100);
        sendEnter(hWnd);
        Sleep(253);
        sendEnter(hWnd);

        log_debug("Command dispatch: %s\n", buff);

    }
}