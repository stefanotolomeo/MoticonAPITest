/* Managed Devices */
#ifndef __MDEVICE_H__
#define __MDEVICE_H__

#include "mtypes.h"
#include "mresult.h"

/*@rstart(pmdevice_t)
.. type:: pmdevice_t

   The public type as anonymous handle.

   @code*/
struct ManagedDevice;
typedef struct ManagedDevice* pmdevice_t;
/*@*/

int MSLAVE_API mdevice_is_recovering(pmdevice_t device);
int MSLAVE_API mdevice_get_status(pmdevice_t device);
int MSLAVE_API mdevice_is_connected(pmdevice_t device);
int MSLAVE_API mdevice_get_connection_quality(pmdevice_t device);
void MSLAVE_API mdevice_reset_connection_quality(pmdevice_t device, int value);


/* Event Handling */
/*@start(message-codes)*/
#define MDEVICE_DISCONNECTED     MR_MAKE_CODE(MR_T_SUCCESS, 0, 1)
#define MDEVICE_CONNECTED        MR_MAKE_CODE(MR_T_SUCCESS, 0, 2)
#define MDEVICE_SEARCHING        MR_MAKE_CODE(MR_T_SUCCESS, 0, 3)
#define MDEVICE_PAIRING_COMPLETE MR_MAKE_CODE(MR_T_SUCCESS, 0, 4)
/*@*/

#endif /* __MDEVICE_H__ */
