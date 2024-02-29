#include "dconfig.h"

std::vector<std::string> schedule;
size_t current_schedule_index = INFINITE;

std::vector<std::string> splitSchedule() {
    std::vector<std::string> result;
    char *schedule_path = config_schedule();
    std::ifstream file(schedule_path);

    if (!file.is_open()) {
        log_error("Error opening schedule file: %s\n", schedule_path);
        free(schedule_path);
        return result; 
    }

    std::string fileContents;
    std::getline(file, fileContents, '\0'); // Read entire file

    // Process the file contents
    size_t currentPos = 0;
    size_t nextPos;

    while ((nextPos = fileContents.find(']', currentPos)) != std::string::npos) {
        std::string element = fileContents.substr(currentPos, nextPos - currentPos);

        // Remove unwanted characters
        const std::string toRemove = " []\n\r";
        element.erase(0, element.find_first_not_of(toRemove));
        element.erase(element.find_last_not_of(toRemove) + 1);

        result.push_back(element);
        currentPos = nextPos + 1; // Start searching after the ']'
    }

    if(result.size() == 0) {
        log_error("Schedule %s is empty \n", schedule_path);
        exit(1);
    } else {
        log_info("Loaded schedule: %s \n", schedule_path);
    }

    free(schedule_path);
    return result;
}

std::vector<std::string> reset_schedule() {
    log_info("Reseting schedule...\n");
    /* reset the index */
    if(current_schedule_index == INFINITE) {
        /* the first run you might start at a different index */
        current_schedule_index = config_schedule_index_0();
    } else {
        /* any other run reset to the start */
        current_schedule_index = 0;
    }
    schedule = splitSchedule();
    return schedule;
}
std::string get_next_schedule_location() {
    std::string next_location;
    if(current_schedule_index >= schedule.size()) {
        if(config_schedule_loop()) {
            reset_schedule();
        } else {
            log_info("Schedule is terminated...");
            exit(1);
        }
    }
    next_location = schedule[current_schedule_index];
    log_event("Next schedule location[%d]: \n", current_schedule_index, next_location.c_str());
    current_schedule_index++;
    return next_location;
}