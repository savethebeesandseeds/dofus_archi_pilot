#include "dconfig.h"
#include "dutils.h"
#include "dcapture.h"
#include "dpilot_process.h"

int main() {
    log_info("[Archi Pilot] Application Started...\n");
    /* read the configuration */
    char *temp_dir = config_temp_dir_pilot();
    if (temp_dir == NULL) { 
        log_error("Bad config\n"); 
        free(temp_dir);
        return 1;
    }

    /* claen the enviroment */
    log_info("Cleaning temporal directory: %s...\n", temp_dir);
    DeleteFilesInDirectory(temp_dir);
    
    /* dispatch the thraed that captures the images and the thread that process the images */
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            pilot_process_loop();
        }
        #pragma omp section
        {
            pilot_capture_loop();
        }
    }

    free(temp_dir);
    return 0;
}