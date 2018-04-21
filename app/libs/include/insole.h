/*
  insole.h
*/


#ifndef __MAPI_INSOLE_H__
#define __MAPI_INSOLE_H__

#include <time.h>
#include "mtypes.h"
#include "mmacros.h"
#include "mdevice.h"


_BEGIN_CPP


#define PSENSOR_COUNT 13

/*@start(pinsole_t)
.. type:: pinsole_t

   The public handle to an insole object.
   @subst(Code)*/
struct Insole;
typedef struct Insole* pinsole_t;
/*@*/

/*@start(insole capabilities)
Insole capabilities
===================

With insole capabilities some special features of the insole can be enabled or
disabled.
@subst(Code)*/
#define CAPABILITY_RECORDING         0x00000001
#define CAPABILITY_SMART_RECORDING   0x00000002
#define CAPABILITY_TF_COP            0x00000004
#define CAPABILITY_USB_DOWNLOAD      0x00000008
/*@*/


/*@cstart(operation modes)*/
#define INS_NO_MODE       255
#define INS_CONTROL_MODE    0
#define INS_FIND_MODE       1

#define INS_DATA_MODE_100  11
#define INS_DATA_MODE_50   12
#define INS_DATA_MODE_10   13
#define INS_DATA_MODE_20   14
/*@*/

/*@cstart(recording modes)*/
#define INS_REC_MODE_STOPPED    0

#define INS_REC_MODE_100       21
#define INS_REC_MODE_50        22
#define INS_REC_MODE_20        24
#define INS_REC_MODE_10        23

#define INS_REC_CLEARING_FLASH 28
#define INS_REC_AWAIT_TIMESTAMP 29

#define INS_REC_TIMEOUT        254
#define INS_REC_MEMORY_FULL    255 /* not yet used */
/*@*/

/* @rstart(acc_range_t)

.. _accrange-t:

acc_range_t
===========

.. type:: acc_range_t

   The valid values for the acceleration G-range
@code */

typedef enum AccelerationRange{
  ACC_2_G = 0,
  ACC_4_G = 1,
  ACC_8_G = 2,
}acc_range_t;

/* @edoc
@rstart(acc_bw_t)

.. _accbw-t:

acc_bw_t
========

.. type:: acc_bw_t

   The valid values for the acceleration Bandwidth
   @code*/

typedef enum AccelerationBandwidth{
  ACC_BW_25   = 0,
  ACC_BW_50   = 1,
  ACC_BW_100  = 2,
  ACC_BW_190  = 3,
  ACC_BW_375  = 4,
  ACC_BW_750  = 5,
  ACC_BW_1500 = 6,
}acc_bw_t;

/*@edoc
@*/

/*

The Sensors are arranged in the following order (the picture shows
the top of the insoles):

.. figure:: sensor-pos.png
   :width: 30 %
   :align: center


The insole has no switch. It wakes up when shaken and enters a sleep
mode when kept still for a certain time.

*/


/*@start(insoleid_t)
insoleid_t
==========

.. type:: insoleid_t

   The type of an insole id.

   .. note::

      Insoleids are given as hex numbers,
      i.e. if your insole has insoleid 00021511 set insoleid to 0x00021511

   @subst(Code)*/
typedef size32_t insoleid_t;

/* @start(INSOLEID_CREATE)

INSOLEID_CREATE
===============

.. macro:: INSOLEID_CREATE(sernum, size, side)

   Creates a soleid from the its parts.

   :param sernum: The unique identifier (serial number) of the insole.
   :param size: The insole size (see :macro:`INSOLEID_SIZE`).
   :param side: The insole side: can be 'l' (left foot) or 'r' (right foot).

@subst(Code) */
#define INSOLEID_CREATE(sernum, size, side)		\
  ((insoleid_t)(((insoleid_t)(sernum) << 16)		\
		| (((MAX(size, 1)) & 0x0F) << 8)	\
		| ((side) == 'r' ? 0x1000 : 0x0000)))

/*@*/

/* @start(INSOLEID_ID)

INSOLEID_ID
===========

.. macro:: INSOLEID_ID(soleid)

   Returns the unique number of the sole id.

@code */
#define INSOLEID_ID(soleid) ((ushort_t)((soleid) >> 16))


/*@start(INSOLEID_SIZE)

INSOLEID_SIZE
=============

.. macro:: INSOLEID_SIZE(soleid)

   Returns the insole size encoded inside the sole id.
   Possible value are:

   ===========  ========  ======== =========  ===========
   SOLEID_SIZE  European     US       UK       Mondopoint
   ===========  ========  ======== =========  ===========
   1            36/37
   2            38/39
   3            40/41
   4            42/43
   5            44/45
   6            46/47
   ===========  ========  ======== =========  ===========
   @subst(Code)*/

#define INSOLEID_SIZE(soleid) ((uchar_t)(((soleid)&0x0F00)>>8))

/*@start(INSOLEID_SIDE)

INSOLEID_SIDE
=============

.. macro:: INSOLEID_SIDE(soleid)

   Returns the insole side encoded inside the sole id.

   :return: 'l' for a left sided insole, 'r' for a right sided insole.

   @subst(Code)*/

#define INSOLEID_SIDE(soleid) (((soleid & 0x1000)) ? 'r' : 'l')
/*@*/



int MSLAVE_API insole_create(pinsole_t* pself, insoleid_t insoleid);
void MSLAVE_API insole_destroy(pinsole_t self);
int MSLAVE_API insole_connect(pinsole_t self);
void MSLAVE_API insole_disconnect(pinsole_t self);

insoleid_t MSLAVE_API insole_get_id(pinsole_t self);
void MSLAVE_API insole_set_id(pinsole_t self, insoleid_t soleid);

int MSLAVE_API insole_connect_bootloader(pinsole_t self);

void MSLAVE_API insole_set_channel_frequencies(pinsole_t self, uchar_t frequencies[3]);
uchar_t MSLAVE_API insole_get_frequency(pinsole_t self);

uchar_t MSLAVE_API insole_create_datachannel(void);
int MSLAVE_API insole_set_datachannel(pinsole_t self, uchar_t data_channel);
int MSLAVE_API insole_set_num_frequencies(pinsole_t self, uchar_t num_frequencies);


/*
Pairing
=======

The sole object can find insoles automatically, without knowing their unique sole number. To
do this, set the sole object's soleid to 0, and turn the desired insole upside down (top face
must be down) in order to bring the insole into a "pairing mode". After calling
:func:`deses_connect` the sole object tries to find the insole in pairing mode. If sole finds
the insole, it emits a :macro:`DESES_PAIRING_COMPLETE` event. To change to data sending mode
you have to reset the connection by calling :func:`mdevice_reset_connection`, and turn the
insole back top face up.

.. note:: For Experts:
   You can restrict the search to specific insole types by setting only the unique sole number
   to 0, and specify the rest of the soleid according to your desired insole types.
*/


/* @start(insole_event_t)
@subst(Code)*/

typedef struct InsoleEvent {
  int code;
  union {
    struct {
      size32_t frame;
      size32_t changed;
    };
    struct {
      uchar_t executed_msg_id;
      uchar_t token;
      size32_t result_value;    /* 0 if ok, 1 if error */
    };
  };
} insole_event_t;

/*@*/


/* @rstart(LONGTERM_TF_MSG_x)

.. macro:: LONGTERM_TF_MSG_x

   Special sensor setups for longterm recording.

   @subst(Code) */

#define LONGTERM_TF_MSG_0 0x80000001
#define LONGTERM_TF_MSG_1 0x80000002

#define LONGTERM_SENS_SETUP ((size32_t)INSOLE_ACCELERATION_BIT(1) \
                                     | INSOLE_TF_BIT \
                                     | INSOLE_COP_BIT(0) \
                                     | INSOLE_COP_BIT(1))



/* @rstart(INSOLE_PRESSURE_BIT)

.. macro:: INSOLE_PRESSURE_BIT(sensor_id)

   Returns the test bit for a pressure sensor.

   :param sensor_id: The pressure sensor id (0..12)

   @subst(Code) */

#define INSOLE_PRESSURE_BIT(sensor_id) (1 << (sensor_id + 3))

/* @rstart(INSOLE_ACCELERATION_BIT)

.. macro:: INSOLE_ACCELERATION_BIT(sensor_id)

   Returns the test bit for an acceleration sensor.

   :param sensor_id: The acceleration sensor id (0..2)

   @subst(Code) */

#define INSOLE_ACCELERATION_BIT(sensor_id) (1 << sensor_id)


/* @rstart(INSOLE_TEMPERATURE_BIT)

.. macro:: INSOLE_TEMPERATURE_BIT

   Returns the test bit for the temperature sensor.

   @subst(Code) */

#define INSOLE_TEMPERATURE_BIT (1L << 16)

/* @rstart(INSOLE_VOLTAGE_BIT)

.. macro:: INSOLE_VOLTAGE_BIT

   Returns the test bit for the battery voltage.

   @subst(Code) */

#define INSOLE_VOLTAGE_BIT (1L << 17)

/* @rstart(INSOLE_CLUSTER_FORCE_BIT)

.. macro:: INSOLE_CLUSTER_FORCE_BIT

   Returns the test bit for the CLUSTER_FORCE (cluster 0-3)

   @subst(Code) */

#define INSOLE_CLUSTER_FORCE_BIT(cluster_id) (1L << (cluster_id + 21))

/* @rstart(INSOLE_COP_BIT)

.. macro:: INSOLE_COP_BIT

   Returns the test bit for the Center of Pressure (0 = x, 1 = y)

   @subst(Code) */

#define INSOLE_COP_BIT(x_y) (1L << (x_y + 25))

/* @rstart(INSOLE_TF_BIT)

.. macro:: INSOLE_TF_BIT

   Returns the test bit for the total force

   @subst(Code) */

#define INSOLE_TF_BIT (1L << 20)

/* @start(INSOLE_NEW_DATA)

.. macro:: INSOLE_NEW_DATA

   The event code for new sole data.

   @subst(Code)*/

#define INSOLE_NEW_DATA      MR_MAKE_CODE(MR_T_SUCCESS, 3, 0)

/* @start(INSOLE_NEW_STATUS_DATA)

.. macro:: INSOLE_NEW_STATUS_DATA

   The event code when new status data arrived.

   @subst(Code)*/
#define INSOLE_NEW_STATUS_DATA      MR_MAKE_CODE(MR_T_SUCCESS, 3, 1)

/* @start(INSOLE_EXECUTION_DONE)

.. macro:: INSOLE_EXECUTION_DONE

   The event code for a confirmed execution.

   @subst(Code)*/
#define INSOLE_EXECUTION_DONE MR_MAKE_CODE(MR_T_SUCCESS, 3, 3)

/* @start(INSOLE_BURST_RECEIVED)

.. macro:: INSOLE_BURST_RECEIVED

   The event code when new download data was received with a burst transfer.

   @subst(Code)*/
#define INSOLE_BURST_RECEIVED MR_MAKE_CODE(MR_T_SUCCESS, 3, 4)

/* @start(INSOLE_RECORDING_STARTED)

.. macro:: INSOLE_RECORDING_STARTED

   The message when recording was started on an insole.

   @subst(Code)*/
#define INSOLE_RECORDING_STARTED MR_MAKE_CODE(MR_T_SUCCESS, 3, 5)

/* @start(INSOLE_RECORDING_STOPPED)

.. macro:: INSOLE_RECORDING_STOPPED

   The message when recording was stopped on an insole.

   @subst(Code)*/
#define INSOLE_RECORDING_STOPPED MR_MAKE_CODE(MR_T_SUCCESS, 3, 6)

/* @start(INSOLE_DISCONNECTED)

.. macro:: INSOLE_DISCONNECTED

   The event code to signalize the insole was disconnected form its peer.

   @subst(Code)*/

#define INSOLE_DISCONNECTED      MDEVICE_DISCONNECTED

/* @start(INSOLE_CONNECTED)

.. macro:: INSOLE_CONNECTED

   The event code to signalize the insole was connected to its peer.

   @subst(Code)*/

#define INSOLE_CONNECTED         MDEVICE_CONNECTED

/* @start(INSOLE_SEARCHING)

.. macro:: INSOLE_SEARCHING

   The event code to signalize the insole is searching for its peer.

   @subst(Code)*/

#define INSOLE_SEARCHING         MDEVICE_SEARCHING


/* @start(INSOLE_PAIRING_COMPLETE)

.. macro:: INSOLE_PAIRING_COMPLETE

   The event code to signalize the insole completed a pairing successfully.

   @subst(Code)*/

#define INSOLE_PAIRING_COMPLETE  MDEVICE_PAIRING_COMPLETE

/*@*/


// 0 or 1 are allowed for debug_id
#define INSOLE_DEBUG_BIT(debug_id) (1L << ((debug_id) + 18))


int MSLAVE_API insole_get_event(pinsole_t self, insole_event_t* msg);

void MSLAVE_API insole_decode_data_message(pinsole_t self, uint8_t *buffer,
                                    size32_t sensors);

void MSLAVE_API insole_set_recording_decode_mode(pinsole_t self, uchar_t rec_mode);

/* Configuration */

int MSLAVE_API insole_stamp_id(pinsole_t self, insoleid_t soleid, time_t timeout);


#if ! (defined(__C30) || defined(_PIC18))
#pragma pack(push, 1) /* causes nasty warnings on C30 compiler */
#endif

#define BURST_UPLOAD    0x01 /* from the peer to the insole */
#define BURST_DOWNLOAD  0x02 /* from the insole to the peer */
#define BURST_UART_DOWNLOAD  0x03  /* data transfer via serial cable from insole to peer*/

#define MEM_EXTERNAL_FLASH        0x01
#define MEM_PIC_FLASH             0x03
#define MEM_PIC_EEPROM            0x04

/*@cstart(burst_data_t)*/
typedef struct BurstData {
  uchar_t seq_num PACKED;
  ushort_t payload_len PACKED;
  uchar_t payload[508] PACKED;
  uchar_t checksum PACKED;
} burst_data_t;
/*@*/

/*
.. type:: direct_data_t

  this type is used to read or write data to/from the different pic memory
  spaces and is transmitted in burst messages

@code*/
typedef struct directAccessData {
  uchar_t mem_type PACKED;
  size32_t address PACKED;
  ushort_t length PACKED;
  uchar_t payload[501] PACKED;
} direct_data_t;
/* @edoc */


#if ! (defined(__C30) || defined(_PIC18))
#pragma pack(pop)   /* causes nasty warnings on C30 compiler */
#endif

int MSLAVE_API insole_transmit_burst(pinsole_t self, burst_data_t* data, time_t timeout, uchar_t* result_value, uchar_t* result_seq_num);
int MSLAVE_API insole_close_burst_connection(pinsole_t self, uchar_t connection_type, uchar_t memory_type);
int MSLAVE_API insole_open_burst(pinsole_t self, uchar_t connection_type, uchar_t memory_type, size32_t num_bursts, uchar_t* data, size_t size);
int MSLAVE_API insole_request_burst_data(pinsole_t self, uchar_t memory_type, size32_t address, ushort_t size);
int MSLAVE_API insole_get_mem_size(pinsole_t self, uchar_t mem_type, size32_t* size);
int MSLAVE_API insole_get_max_mem_size(pinsole_t self, uchar_t memtype, size32_t* size);

int MSLAVE_API insole_sleep(pinsole_t self, uchar_t sleep_time);
int MSLAVE_API insole_zeroize(pinsole_t self, uchar_t zeroize, uchar_t zoc_auto);
int MSLAVE_API insole_get_zoc_auto(pinsole_t self, uchar_t* zoc_auto);
int MSLAVE_API insole_zoc_zeroing_busy(pinsole_t self, uchar_t* busy);

int MSLAVE_API insole_get_error_counter(pinsole_t self, uchar_t* weak, uchar_t* defect);


void MSLAVE_API insole_switch_mode(pinsole_t self, uchar_t mode);

int MSLAVE_API insole_rec_start(pinsole_t self, uchar_t rec_mode, uchar_t record_smart, ulong_t rec_timeout);
void MSLAVE_API insole_rec_stop(pinsole_t self);
int MSLAVE_API insole_rec_clear_flash(pinsole_t self);
int MSLAVE_API insole_is_recording(pinsole_t self, uchar_t *result);
int MSLAVE_API insole_rec_synced(pinsole_t self, uchar_t *result);
int MSLAVE_API insole_get_rec_mode(pinsole_t self, uchar_t *mode);
int MSLAVE_API insole_get_smartrec_mode(pinsole_t self, uchar_t *smart_rec_mode);

void MSLAVE_API insole_get_timestamp(size32_t *timestamp, ushort_t *ms);


uchar_t MSLAVE_API insole_get_mode(pinsole_t self);

uchar_t MSLAVE_API insole_get_status(pinsole_t self);
uchar_t MSLAVE_API insole_is_connected(pinsole_t self);


/* @start(DEFAULT_MSG1)

.. macro:: DEFAULT_MSG1

   The first message for the default sensor set.

@code */
#define DEFAULT_MSG1 ((size32_t)0       \
            |INSOLE_ACCELERATION_BIT(0) \
            |INSOLE_ACCELERATION_BIT(1) \
            |INSOLE_PRESSURE_BIT(2)     \
            |INSOLE_PRESSURE_BIT(4)     \
            |INSOLE_PRESSURE_BIT(6)     \
            |INSOLE_PRESSURE_BIT(7)     \
            |INSOLE_PRESSURE_BIT(10)    \
            |INSOLE_PRESSURE_BIT(12))

/* @edoc

@start(DEFAULT_MSG2)

.. macro:: DEFAULT_MSG2

   The second message for the default sensor set.

@code */
#define DEFAULT_MSG2 ((size32_t)0       \
            |INSOLE_ACCELERATION_BIT(2) \
            |INSOLE_PRESSURE_BIT(0)     \
            |INSOLE_PRESSURE_BIT(1)     \
            |INSOLE_PRESSURE_BIT(3)     \
            |INSOLE_PRESSURE_BIT(5)     \
            |INSOLE_PRESSURE_BIT(8)     \
            |INSOLE_PRESSURE_BIT(9)     \
            |INSOLE_PRESSURE_BIT(11))

/*@*/

/* @start(DEFAULT_DATA_MESSAGES)

.. macro:: DEFAULT_DATA_MESSAGES

   The default sensor set

@code */

#define DEFAULT_DATA_MESSAGES DEFAULT_MSG1, DEFAULT_MSG2, 0, 0

/*@*/

#define ACCELERATION_MESSAGES ((size32_t)0   \
              | INSOLE_ACCELERATION_BIT(0)   \
              | INSOLE_ACCELERATION_BIT(1)   \
              | INSOLE_ACCELERATION_BIT(2)), \
              0, 0, 0




#define INSOLE_ACC_GET_G_RANGE(acc_settings) (acc_settings & 0x03)
#define INSOLE_ACC_GET_BANDWIDTH(acc_settings) ((acc_settings >> 2) & 0x0F)
#define INSOLE_ACC_SETTINGS(acc_g_range, acc_bw) (((acc_bw & 0x0F) << 2) | (acc_g_range & 0x03))

#define DEFAULT_ACC_SETTINGS INSOLE_ACC_SETTINGS(ACC_8_G, ACC_BW_25)

/* a row of sensor_bits  followed by 0 */
int MSLAVE_API insole_set_data_messages(pinsole_t self, size32_t m0, size32_t m1,
                                            size32_t m2, size32_t m3);
int MSLAVE_API insole_get_data_messages(pinsole_t self, size32_t sensors[4]);
int MSLAVE_API insole_get_acc_g_range(pinsole_t self, uchar_t* acc_g_range);
void MSLAVE_API insole_set_acc_g_range(pinsole_t self, uchar_t acc_g_range);

int MSLAVE_API insole_get_acc_bw(pinsole_t self, uchar_t* acc_bw);
void MSLAVE_API insole_set_acc_bw(pinsole_t self, uchar_t acc_bw);

int MSLAVE_API insole_get_restarts(pinsole_t self, ushort_t* restarts);
int MSLAVE_API insole_get_clock(pinsole_t self, size32_t* clock, ushort_t* clock_32k);

int MSLAVE_API insole_get_version(pinsole_t self, ushort_t* version);
int MSLAVE_API insole_get_version_modified(pinsole_t self, uchar_t* version_modified);
int  MSLAVE_API insole_get_serial_number(pinsole_t self, size32_t *id);

int MSLAVE_API insole_get_bl_hw_info(pinsole_t self, ushort_t* hw_version, ushort_t* flash_size);


int MSLAVE_API insole_get_uptime(pinsole_t self, size32_t* uptime);
int MSLAVE_API insole_get_operating_time(pinsole_t self, size32_t* operating_time);


/* Sensor Values  */
size32_t MSLAVE_API insole_get_frame_count(pinsole_t self);
int MSLAVE_API insole_get_pressure(pinsole_t self, ushort_t data[PSENSOR_COUNT]);
int MSLAVE_API insole_get_force(pinsole_t self, float data[PSENSOR_COUNT]);
int MSLAVE_API insole_get_cop(pinsole_t self, int16_t* cop_x, int16_t* cop_y);
int MSLAVE_API insole_get_total_force(pinsole_t self, float* total_force);
int MSLAVE_API insole_get_cluster_force(pinsole_t self, ushort_t cluster_force[4]);
int MSLAVE_API insole_get_acceleration(pinsole_t self, ushort_t data[3]);
int MSLAVE_API insole_get_voltage(pinsole_t self, ushort_t* voltage);
int MSLAVE_API insole_get_battery_type(pinsole_t self, ushort_t* battery_type);
int MSLAVE_API insole_get_power_consumption(pinsole_t self, float* consumption);
int MSLAVE_API insole_get_battery_level(pinsole_t self, uchar_t* battery_level);
int MSLAVE_API insole_get_temperature(pinsole_t self, short* temperature);
int MSLAVE_API insole_get_debug(pinsole_t self, ushort_t debug[2]);

int MSLAVE_API insole_get_sensor_values(pinsole_t self,
                                       uchar_t* acc_g_range,
                                       ushort_t pressure[PSENSOR_COUNT],
                                       ushort_t acc[3],
                                       float* total_force,
                                       short cop[2],
                                       ushort_t* voltage,
                                       short* temperature);

void MSLAVE_API insole_set_pressure_value(pinsole_t self, uchar_t sensor, ushort_t n_cm);

#define insole_is_recovering(x)          mdevice_is_recovering((pmdevice_t)(x))
//#define insole_get_status(x)             mdevice_get_status((pmdevice_t)(x))
//#define insole_is_connected(x)           mdevice_is_connected((pmdevice_t)(x))
#define insole_get_connection_quality(x) mdevice_get_connection_quality((pmdevice_t)(x))


/*
.. _sgeometry:

********
Geometry
********

For advanced analysis the exact sole data is neccessary. The following functions provide
the insole geometry data shown in the following figure. All functions works with *insole coordinates*
which are relative to the origin marked in the figure. The unit of all coordinates are millimeter.
*/

void MSLAVE_API insole_get_sensor_coords(insoleid_t soleid, float x[PSENSOR_COUNT],
                            float y[PSENSOR_COUNT], float square[PSENSOR_COUNT]);

#if ! defined(TERMINAL)
void MSLAVE_API insole_get_outline(insoleid_t soleid, float x[100], float y[100]);
void MSLAVE_API insole_get_dimension(insoleid_t soleid, float* length, float* width);
void MSLAVE_API insole_get_interpolation_matrix(insoleid_t soleid,
                                        size_t count, float x[], float y[],
                                        float result[]);
#endif

_END_CPP

#endif /* __MAPI_INSOLE_H__ */
