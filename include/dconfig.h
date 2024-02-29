#pragma once
#define _WIN32_WINNT 0x0A00 /* Target Windows 10 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <cstdlib>

#define CONFIG_FILE "./.config"
#define MAX_CONFIG_LINE 256

int readConfigValue(const char* configFile, const char* searchKey, char* valueBuffer, int bufferLength);
char *config_wrapper(const char* key);
char *config_window();
char *config_temp_dir();
char *config_log_lvl();
int config_period_sample();

#include "dutils.h"

/* Function now takes an additional parameter for the key to search for */
/* and a buffer to store the found value. */
/* Returns 1 if the key is found and the value is copied into the buffer, 0 otherwise. */
int readConfigValue(const char* configFile, const char* searchKey, char* valueBuffer, int bufferLength) {
    char buf[MAX_CONFIG_LINE];
    FILE* file = fopen(configFile, "r");
    if (!file) {
        log_error("Failed to open config file, problem opening the file.");
        return 0; /* Indicate failure to open file */
    }

    while (fgets(buf, sizeof(buf), file)) {
        char* line = strtok(buf, "\n"); /* Remove newline character */
        if (line[0] == '#' || line[0] == ';') continue; /* Skip comments */

        char* key = strtok(line, "=");
        char* value = strtok(NULL, "=");

        if (key && value && strcmp(key, searchKey) == 0) {
            /* If the searchKey matches the current key, copy the value to the buffer */
            strncpy(valueBuffer, value, bufferLength - 1);
            valueBuffer[bufferLength - 1] = '\0'; /* Ensure null-termination */
            fclose(file); /* Close the file before returning */
            return 1; /* Indicate success */
        }
    }

    fclose(file); /* Ensure the file is closed if the key is not found */
    return 0; /* Indicate that the key was not found */
}

/* Wrapper to capture a config value */
char *config_wrapper(const char* key) {
    /* Allocate memory */
    char* value = (char*)calloc(MAX_CONFIG_LINE, sizeof(char));
    if (value == NULL) {
        /* Handle memory allocation failure */
        return NULL; /* Return NULL if allocation fails */
    }
    /* Read the configuration file */
    if (readConfigValue(CONFIG_FILE, key, value, MAX_CONFIG_LINE)) {
        return value;
    } else {
        log_error("Key '%s' not found.\n", key);
        return NULL;
    }
}
int config_integer_wrapper(const char* key) {
    int ret = -1;
    char* endPtr;
    char *value_str = config_wrapper(key);
    double num = strtod(value_str, &endPtr);
    
    if (endPtr == value_str) {
        log_info("No digits were found in %s\n", key);
        ret = -1;
    } else {
        ret = num;
    }
    free(value_str);
    return ret;
}
double config_double_wrapper(const char* key) {
    double ret = -1;
    char* endPtr;
    char *value_str = config_wrapper(key);
    double num = strtod(value_str, &endPtr);

    if (endPtr == value_str) {
        log_info("No digits were found in %s\n", key);
    } else if (num <= 0) {
        log_error("Bad number for key : %s: %s\n", key, value_str);
    } else {
        ret = num;
    }

    free(value_str);

    return ret;
}
/* get config value for delete_temp key */
BOOL config_bolean_wrapper(const char* key) {
    char * temp = config_wrapper(key);
    BOOL ret = strcmp(temp, "true") == 0;
    free(temp);
    return ret;
}
/* get config value for window key */
char *config_window() {
    return config_wrapper("window");
}
/* get config value for temp_dir key */
char *config_temp_dir() {
    return config_wrapper("temp_dir");
}
/* get config value for output_dir key */
char *config_output_dir() {
    return config_wrapper("output_dir");
}
/* get config value for log_lvl key */
char *config_log_lvl() {
    return config_wrapper("log_lvl");
}
/* get config value for archi_img key */
char *config_archi_img() {
    return config_wrapper("archi_img");
}
/* get config value for schedule key */
char *config_schedule() {
    return config_wrapper("schedule");
}
/* get config value for delete_temp key */
BOOL config_delete_temp() {
    return config_bolean_wrapper("delete_temp");
}
/* get config value for schedule_loop key */
BOOL config_schedule_loop() {
    return config_bolean_wrapper("schedule_loop");
}
/* get config value for do_schedule key */
BOOL config_do_schedule() {
    return config_bolean_wrapper("do_schedule");
}
/* get config value for period_sample key */
int config_period_sample() {
    return config_integer_wrapper("period_sample");
}
/* get config value for threshold key */
double config_threshold() {
    return config_double_wrapper("threshold");
}
/* get config value for box_change_x key */
int config_box_change_x() {
    return config_integer_wrapper("box_change_x");
}
/* get config value for box_change_y key */
int config_box_change_y() {
    return config_integer_wrapper("box_change_y");
}
/* get config value for box_change_size key */
int config_box_change_size() {
    return config_integer_wrapper("box_change_size");
}
/* get config value for schedule_index_0 key */
int config_schedule_index_0() {
    return config_integer_wrapper("schedule_index_0");
}
/* get config value for box_change_diff_norm_threshold key */
double config_box_change_diff_norm_threshold() {
    return config_double_wrapper("box_change_diff_norm_threshold");
}
/* get config value for delta_position_threshold key */
double config_delta_position_threshold() {
    return config_double_wrapper("delta_position_threshold");
}
/* get config value for down_size_coef key */
double config_down_size_coef() {
    return config_double_wrapper("down_size_coef");
}