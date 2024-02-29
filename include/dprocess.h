#pragma once
#include "dconfig.h"
#include "dutils.h"
#include "dwin_api.h"
#include "dimages.h"
#include "dmovilize.h"

cv::Mat archi_img;
void update_archi_image() {
    char * archi_img_path = config_archi_img();
    archi_img = cv::imread(archi_img_path);
    /* validate the image */
    if (archi_img.empty()) {
        log_error("Error loading archi_img %s. \n", archi_img_path);
    }
    free(archi_img_path);
}

cv::Mat process_single_file(const char *directoryPath, const char* fileName) {
    Sleep(100); /* added for stability porpouses */
    char outoutFilePath[MAX_PATH];
    char * output_dir = config_output_dir();

    char new_img_FilePath[MAX_PATH];
    
    /* New file arrived */
    sprintf(new_img_FilePath, "%s\\%s", directoryPath, fileName);

    cv::Mat new_img = cv::imread(new_img_FilePath);
    /* validate the image */
    if (new_img.empty()) {
        log_error("Error loading new image at path %s \n", new_img_FilePath);
        free(output_dir);
        return new_img;
    }

    /* search for archis */
    cv::Mat archi_mask = createMaskFromTemplate(archi_img);
    MatchResult matchResult = findTemplateWithMask(new_img, archi_img, archi_mask);
    log_debug("New image [%s]\t value: %f\n", new_img_FilePath, matchResult.maxVal);
    
    if (matchResult.maxVal < config_threshold()) {
        
        // log_event("Match found at (%5d, %5d) \t : %f\n", matchResult.minLocation.x, matchResult.minLocation.y, matchResult.minVal);
        // Draw a rectangle around the matched region
        // cv::rectangle(new_img, matchResult.minLocation, cv::Point(matchResult.minLocation.x + archi_img.cols, matchResult.minLocation.y + archi_img.rows), cv::Scalar(0, 255, 0), 2);
        // cv::rectangle(new_img, 
        //     cv::Point(config_box_change_x(), config_box_change_y()), 
        //     cv::Point(config_box_change_x() + config_box_change_size(), config_box_change_y() + config_box_change_size()), cv::Scalar(255, 0, 0), 2);
        // cv::imwrite("waka.bmp", new_img);
        // cv::imshow("Match", img);
        // cv::waitKey(0);

        sprintf(outoutFilePath, "%s\\%s", output_dir, fileName);
        log_eureka("Archi found: [%f] %s\n", matchResult.maxVal, outoutFilePath);
        copy_file(new_img_FilePath, outoutFilePath);
        notify_sound();
    }

    if(config_delete_temp()) {
        DeleteSingleFile(directoryPath, fileName);
    }

    /* terminate */
    free(output_dir);

    return new_img;
}

void process_step(const char *directoryPath, const char* fileName) {
    tick("process step");

    /* Process the new file here */
    cv::Mat new_img = process_single_file(directoryPath, fileName);
    
    /* movilize step */
    step_pilot(new_img);

    tock("process step");
}

void process_loop() {

    log_info("process_loop() started\n");
    /* update archi image */
    update_archi_image();

    /* start an observation loop for new files in temp folder */
    DWORD bytesReturned;
    HANDLE hDir = NULL;
    char buffer[1024];
    char *temp_dir = NULL;
    FILE_NOTIFY_INFORMATION *pNotify;
    BOOL result;
    int count;
    char fileName[MAX_PATH];

    /* read the configuration */
    temp_dir = config_temp_dir();

    if (temp_dir == NULL) { log_error("Bad config\n"); goto exit; }
    
    /* capture the folder handle */
    hDir = CreateFileW(
        ConvertToWideChar(temp_dir),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        log_error("Error opening directory [%s]: %s\n", temp_dir, win_error_message());
        goto exit;
    }

    /* loop indefinietly waiting for new files to arrive */
    while (TRUE) {
        result = ReadDirectoryChangesW(
            hDir,                             /* Directory handle */
            buffer,                           /* Buffer to receive change data */
            sizeof(buffer),                   /* Buffer size */
            FALSE,                            /* Watch subdirectories */
            FILE_NOTIFY_CHANGE_FILE_NAME,     /* Changes to watch for */
            &bytesReturned,                   /* Bytes returned */
            NULL,                             /* Overlapped buffer */
            NULL                              /* Completion routine */
        );

        if (!result) {
            log_error("Error reading directory changes: %s\n", win_error_message());
            break;
        } else {
            pNotify = (FILE_NOTIFY_INFORMATION *)buffer;
            do {
                if (pNotify->Action == FILE_ACTION_ADDED) {
                    /* A new file was added, process it */
                    count = WideCharToMultiByte(
                        CP_UTF8, 0, 
                        pNotify->FileName,
                        pNotify->FileNameLength / sizeof(WCHAR), /* Number of wide characters to convert */
                        fileName, /* Destination buffer for the converted string */
                        sizeof(fileName), /* Size of the destination buffer */
                        NULL, NULL
                    );
                    if (count > 0) {
                        fileName[count] = '\0'; /* Null-terminate the string */
                        process_step(temp_dir, fileName);
                    } else {
                        /* Handle the conversion error */
                        log_error("Error converting wide character string to multi-byte string.\n");
                    }
                }
                pNotify = (FILE_NOTIFY_INFORMATION *)((char *)pNotify + pNotify->NextEntryOffset);
            } while (pNotify->NextEntryOffset != 0);
        }
    }

exit:
    /* Clean up */
    free(temp_dir);
    CloseHandle(hDir);

    log_debug("process_loop() ended\n");
}
