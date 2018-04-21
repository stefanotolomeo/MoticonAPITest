//
// Created by Stefano Tolomeo on 12/03/18.
//

#include <jni.h>
#include <string>

#include "../../../libs/include/insole.h"
#include "../../../libs/include/mdevice.h"
#include "../../../libs/include/mmacros.h"
#include "../../../libs/include/moticonapi.h"
#include "../../../libs/include/mresult.h"
#include "../../../libs/include/mtypes.h"
#include "../../../libs/include/pstdint.h"

void print_exception(int error){
    switch(error){
        case MR_ABORTED:        printf("MR_ABORTED\n"); break;
        case MR_ANT_NO_DEVICE:  printf("MR_ANT_NO_DEVICE\n"); break;
        case MR_ASSERT:         printf("MR_ASSERT\n"); break;
        case MR_CHECKSUM_BAD:   printf("MR_CHECKSUM_BAD\n"); break;
        case MR_FAILED:         printf("MR_FAILED\n"); break;
        case MR_FATAL_ERROR:    printf("MR_FATAL_ERROR\n"); break;
        case MR_INVALID_ARG:    printf("MR_INVALID_ARG\n"); break;
        case MR_NO_DATA:        printf("MR_NO_DATA\n"); break;
        case MR_NO_LICENSE:     printf("MR_NO_LICENSE\n"); break;
        case MR_NOT_SUPPORTED:  printf("MR_NOT_SUPPORTED\n"); break;
        case MR_OUT_OF_MEMORY:  printf("MR_OUT_OF_MEMORY\n"); break;
        case MR_QUEUE_OVERFLOW: printf("MR_QUEUE_OVERFLOW\n"); break;
        case MR_RECORDING:      printf("MR_RECORDING\n"); break;
        case MR_RECOVERING:     printf("MR_RECOVERING\n"); break;
        case MR_TIMEOUT:        printf("MR_TIMEOUT\n"); break;
        case MR_WRONG_STATE:    printf("MR_WRONG_STATE\n"); break;
        default: printf("< %i unknown error >\n");
    }
}

extern "C"
JNIEXPORT jint
JNICALL
Java_com_aislab_moticonapitest_MainActivity_getID(
        JNIEnv *env,
        jobject, /* this */
        insoleid_t insoleid) {

    return INSOLEID_ID(insoleid);
}

extern "C"
JNIEXPORT jint
JNICALL
Java_com_aislab_moticonapitest_MainActivity_getSize(
        JNIEnv *env,
        jobject, /* this */
        insoleid_t insoleid) {

    return INSOLEID_SIZE(insoleid);
}

extern "C"
JNIEXPORT jchar
JNICALL
Java_com_aislab_moticonapitest_MainActivity_getSide(
        JNIEnv *env,
        jobject, /* this */
        insoleid_t insoleid) {

    return INSOLEID_SIDE(insoleid);
}

extern "C"
JNIEXPORT void
JNICALL
Java_com_aislab_moticonapitest_Mapi_switchMode(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole_l,
        pinsole_t insole_r,
        jint mode) {

    uchar_t targetMode;
    switch (mode){
        case 1: {targetMode = INS_DATA_MODE_10; break; }
        case 2: {targetMode = INS_DATA_MODE_20; break; }
        case 3: {targetMode = INS_DATA_MODE_50; break;}
        case 4: {targetMode = INS_DATA_MODE_100; break;}

        case 11: {targetMode = INS_FIND_MODE; break;}
        case 12: {targetMode = INS_CONTROL_MODE; break;}

        default:{targetMode = INS_DATA_MODE_100; break;}
    }

    insole_switch_mode(insole_l, targetMode);
    insole_switch_mode(insole_r, targetMode);
}

extern "C"
JNIEXPORT jchar
JNICALL
Java_com_aislab_moticonapitest_MainActivity_getMode(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole) {

    return insole_get_mode(insole);
}



void check_and_print_data(pinsole_t insole, insole_event_t *ins_event){
    /* checks if a new complete set of sensor data is available and prints it */

    int error;

    /* only every second INSOLE_NEW_DATA event the sensor set is completely updated*/
    if (ins_event->code == INSOLE_NEW_DATA && ins_event->frame % 2){
        ushort_t acc[3];
        ushort_t sensors[13];
        int i;

        /* get the received sensor values from the insole object */
        CHECK(insole_get_acceleration(insole, acc));
        CHECK(insole_get_pressure(insole, &sensors[0]));


        /* print the received sensor values */
        printf("%08x %c sensor data frame %5i: acceleration: ", insole_get_id(insole),
               INSOLEID_SIDE(insole_get_id(insole)), ins_event->frame);
        for (i=0; i<3; i++)
            printf("%4i ", acc[i]);

        printf("pressure: ");
        for (i=0; i<12; i++)
            printf("%5.2f ", sensors[i] / 100.);

        printf("\n");
    }

    BEGIN_EXCEPT
    printf("Got error ");
    print_exception(error);

    END_EXCEPT
    return;
}

extern "C"
JNIEXPORT jboolean
JNICALL
Java_com_aislab_moticonapitest_Mapi_checkEvent(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole) {

    jint error;
    jboolean success = JNI_FALSE;
    insole_event_t ins_event;
    WCHECK(insole_get_event(insole, &ins_event));
    if (error == MR_SUCCESS) {
        if (ins_event.code == INSOLE_CONNECTED)
            printf("Left insole connected\n");
            success = JNI_TRUE;
            check_and_print_data(insole, &ins_event);
    }

    BEGIN_EXCEPT
        printf("Got error ");
        print_exception(error);

    END_EXCEPT
    printf("Got error ");

    return success;
}


