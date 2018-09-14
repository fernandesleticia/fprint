//
// Created by leticia on 14/09/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libfprint/fprint.h>
#include <assert.h>

struct fp_dscv_dev *discover_device(struct fp_dscv_dev **discovered_devs);
struct fp_print_data *enroll(struct fp_dev *dev);


struct fp_dscv_dev *discover_device(struct fp_dscv_dev **discovered_devs)
{
    struct fp_dscv_dev *ddev = discovered_devs[0];
    struct fp_driver *drv;
    if (!ddev)
        return NULL;

    drv = fp_dscv_dev_get_driver(ddev);
    printf("Found device claimed by %s driver\n", fp_driver_get_full_name(drv));
    return ddev;
}

struct fp_print_data *enroll(struct fp_dev *dev) {
    struct fp_print_data *enrolled_print = NULL;
    int r;



    printf("You will need to successfully scan your finger %d times to "
           "complete the process.\n", fp_dev_get_nr_enroll_stages(dev));

    do {
        sleep(1);
        printf("\nScan your finger now.\n");
        r = fp_enroll_finger(dev, &enrolled_print);
        if (r < 0) {
            printf("Enroll failed with error %d\n", r);
            return NULL;
        }
        switch (r) {
            case FP_ENROLL_COMPLETE:
                printf("Enroll complete!\n");
                break;
            case FP_ENROLL_FAIL:
                printf("Enroll failed, something wen't wrong :(\n");
                return NULL;
            case FP_ENROLL_PASS:
                printf("Enroll stage passed. Yay!\n");
                break;
            case FP_ENROLL_RETRY:
                printf("Didn't quite catch that. Please try again.\n");
                break;
            case FP_ENROLL_RETRY_TOO_SHORT:
                printf("Your swipe was too short, please try again.\n");
                break;
            case FP_ENROLL_RETRY_CENTER_FINGER:
                printf("Didn't catch that, please center your finger on the "
                       "sensor and try again.\n");
                break;
            case FP_ENROLL_RETRY_REMOVE_FINGER:
                printf("Scan failed, please remove your finger and then try "
                       "again.\n");
                break;
        }
    } while (r != FP_ENROLL_COMPLETE);

    if (!enrolled_print) {
        fprintf(stderr, "Enroll complete but no print?\n");
        return NULL;
    }

    printf("Enrollment completed!\n\n");
    return enrolled_print;
}
