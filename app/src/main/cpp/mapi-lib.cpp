//
// Created by Stefano Tolomeo on 09/03/18.
//

#include <jni.h>
#include <android/log.h>
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
        //case MR_INVALID_ARG:    printf("MR_INVALID_ARG\n"); break;
        case MR_INVALID_ARG:__android_log_write(ANDROID_LOG_ERROR, "Exception", "MR_INVALID_ARG");break;
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

/* **** MAPI FUNCTIONS **** */
extern "C"
JNIEXPORT jlong
JNICALL
Java_com_aislab_moticonapitest_Mapi_mapiVersion(
        JNIEnv *env,
        jobject /* this */) {

    //version_t version = mapi_version();
    return (jlong) VERSION_GET_MAIN(mapi_version());
}

extern "C"
JNIEXPORT int
JNICALL
Java_com_moticon_MoticonAPI_mapiInit(
        JNIEnv *env,
        jobject /* this */) {

    //int res = mapi_init(NULL);    // Not work, see screenshot 2
    int res = mapi_init("");        // Not work, see screenshot 1
    return res;
}

extern "C"
JNIEXPORT void
JNICALL
Java_com_aislab_moticonapitest_Mapi_mapiFinish(
        JNIEnv *env,
        jobject /* this */) {

    mapi_finish();
}

extern "C"
JNIEXPORT void
JNICALL
Java_com_aislab_moticonapitest_Mapi_mapiReset(
        JNIEnv *env,
        jobject /* this */) {

    return mapi_reset();
}

extern "C"
JNIEXPORT jboolean
JNICALL
Java_com_aislab_moticonapitest_Mapi_mapiWaitForEvents(
        JNIEnv *env,
        jobject, /* this */
        time_t timeout) {

    int error; int tmpRes=false; jboolean res;
    CHECK(mapi_wait_for_events(timeout));

     res = (jboolean) MR_SUCCEEDED(tmpRes);

    BEGIN_EXCEPT
    printf("Got error ");
    print_exception(error);

    res = (jboolean) false;
    END_EXCEPT

    return res;

}

/* **** INSOLES FUNCTIONS **** */

extern "C"
JNIEXPORT jintArray
JNICALL
Java_com_aislab_moticonapitest_Mapi_createInsoles(
        JNIEnv *env,
        jobject, /* this */
        insoleid_t leftID,
        insoleid_t rightID) {

    jintArray insoles_array = env-> NewIntArray(2);
    if(insoles_array == NULL){
        return NULL; // out of memory error thrown
    }

    pinsole_t insole_l;
    pinsole_t insole_r;
    int error, id_l, id_r, sns_l, sns_r, size_l, size_r;
    bool isConnected_l, isConnected_r;
    char side_l, side_r;
    uchar_t status_l, status_r;
    CHECK(insole_create(&insole_l, leftID));
    CHECK(insole_create(&insole_r, rightID));

    INSOLEID_CREATE(3422, 4, 'l');
    INSOLEID_CREATE(3423, 4, 'r');

    id_l = insole_get_id(insole_l);
    isConnected_l = insole_is_connected(insole_l);
    status_l = insole_get_status(insole_l);
    sns_l = INSOLEID_ID(leftID);
    side_l = INSOLEID_SIDE(leftID);
    size_l = INSOLEID_SIZE(leftID);

    id_r = insole_get_id(insole_r);
    isConnected_r = insole_is_connected(insole_r);
    status_r = insole_get_status(insole_r);
    sns_r = INSOLEID_ID(rightID);
    side_r = INSOLEID_SIDE(rightID);
    size_r = INSOLEID_SIZE(rightID);

    CHECK(insole_connect(insole_l));
    CHECK(insole_connect(insole_r));

    jint tmp_insoles[2];
    tmp_insoles[0] = (jint) insole_l;
    tmp_insoles[1] = (jint) insole_r;

    env->SetIntArrayRegion(insoles_array, 0, 2, tmp_insoles);

    BEGIN_EXCEPT
    print_exception(error);

    insoles_array = NULL;

    END_EXCEPT
    return insoles_array;

}

extern "C"
JNIEXPORT jintArray
JNICALL
Java_com_moticon_MoticonAPI_createInsoles(
        JNIEnv *env,
        jobject, /* this */
        insoleid_t leftID,
        insoleid_t rightID) {

    mapi_init("");

    jintArray insoles_array = env-> NewIntArray(2);
    if(insoles_array == NULL){
        return NULL; // out of memory error thrown
    }

    pinsole_t insole_l;
    pinsole_t insole_r;
    int error, id_l, id_r, sns_l, sns_r, size_l, size_r;
    bool isConnected_l, isConnected_r;
    char side_l, side_r;
    uchar_t status_l, status_r;
    int x = mapi_init("");
    int resLeft, resRight;
    CHECK(resLeft = insole_create(&insole_l, leftID));
    CHECK(resRight = insole_create(&insole_r, rightID));

    //INSOLEID_CREATE(3422, 4, 'l');
    //INSOLEID_CREATE(3423, 4, 'r');

    id_l = insole_get_id(insole_l);
    isConnected_l = insole_is_connected(insole_l);
    status_l = insole_get_status(insole_l);
    sns_l = INSOLEID_ID(leftID);
    side_l = INSOLEID_SIDE(leftID);
    size_l = INSOLEID_SIZE(leftID);

    id_r = insole_get_id(insole_r);
    isConnected_r = insole_is_connected(insole_r);
    status_r = insole_get_status(insole_r);
    sns_r = INSOLEID_ID(rightID);
    side_r = INSOLEID_SIDE(rightID);
    size_r = INSOLEID_SIZE(rightID);

    CHECK(insole_connect(insole_l));
    CHECK(insole_connect(insole_r));

    jint tmp_insoles[2];
    tmp_insoles[0] = (jint) insole_l;
    tmp_insoles[1] = (jint) insole_r;

    env->SetIntArrayRegion(insoles_array, 0, 2, tmp_insoles);

    BEGIN_EXCEPT
    print_exception(error);

    insoles_array = NULL;

    END_EXCEPT
    return insoles_array;

}

extern "C"
JNIEXPORT jboolean
JNICALL
Java_com_aislab_moticonapitest_Mapi_connectInsoles(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole_l,
        pinsole_t insole_r) {

    int error, leftRes, rightRes; jboolean res;

    CHECK(leftRes = insole_connect(insole_l));
    CHECK(rightRes = insole_connect(insole_r));

    res = (jboolean) (MR_SUCCEEDED(leftRes) && MR_SUCCEEDED(rightRes));

    BEGIN_EXCEPT
    printf("Got error ");
    print_exception(error);

    res = NULL;

    END_EXCEPT

    return res;
}

extern "C"
JNIEXPORT void
JNICALL
Java_com_aislab_moticonapitest_Mapi_disconnectInsoles(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole_l,
        pinsole_t insole_r) {

    insole_disconnect(insole_l);
    insole_disconnect(insole_r);

}


extern "C"
JNIEXPORT void
JNICALL
Java_com_aislab_moticonapitest_Mapi_destroyInsoles(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole_l,
        pinsole_t insole_r) {

    insole_destroy(insole_l);
    insole_destroy(insole_r);
}

extern "C"
JNIEXPORT jboolean
JNICALL
Java_com_aislab_moticonapitest_MainActivity_insoleIsConnected(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole) {

    return (jboolean) (insole_is_connected(insole) == INSOLE_CONNECTED);

}

extern "C"
JNIEXPORT jboolean
JNICALL
Java_com_aislab_moticonapitest_MainActivity_insoleIsDisconnected(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole) {

    return (jboolean) (insole_is_connected(insole) == INSOLE_DISCONNECTED);
}
extern "C"
JNIEXPORT jboolean
JNICALL
Java_com_aislab_moticonapitest_MainActivity_insoleIsSearching(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole) {

    return (jboolean) (insole_is_connected(insole) == INSOLE_SEARCHING);
}

/* Da rivedere:
 * Returns the current insole status. One of INSOLE_CONNECTED, INSOLE_SEARCHING or INSOLE_DISCONNECTED.
 * */
extern "C"
JNIEXPORT jbyte
JNICALL
Java_com_aislab_moticonapitest_MainActivity_insoleGetStatus(
        JNIEnv *env,
        jobject, /* this */
        pinsole_t insole) {

    return insole_get_status(insole);

}



