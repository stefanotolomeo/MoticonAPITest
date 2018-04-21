#include <jni.h>
#include <string>

#include "../../../libs/include/insole.h"
#include "../../../libs/include/mdevice.h"
#include "../../../libs/include/mmacros.h"
#include "../../../libs/include/moticonapi.h"
#include "../../../libs/include/mresult.h"
#include "../../../libs/include/mtypes.h"
#include "../../../libs/include/pstdint.h"

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_aislab_moticonapitest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
