#if ! defined(__MOTICONAPI_H__)
#define __MOTICONAPI_H__

#include "mtypes.h"
#include <time.h>

_BEGIN_CPP;
version_t MSLAVE_API mapi_version();
ushort_t MSLAVE_API mapi_revision();

int MSLAVE_API mapi_init(const char* license);
void MSLAVE_API mapi_finish();
size_t MSLAVE_API mapi_get_sticks(size32_t rsticks[20]);
void MSLAVE_API mapi_add_stick(const char* id);
int MSLAVE_API mapi_wait_for_events(time_t timeout);
void MSLAVE_API mapi_reset();

_END_CPP;

#if defined(MSVC) && ! defined(MSLAVE_EXPORTS) && ! defined(TERMINAL)
#pragma comment(lib, "moticonapi.lib")
#endif


extern size32_t start_timestamp;
extern size32_t start_ms;

#endif /*__MOTICONAPI_H__*/
