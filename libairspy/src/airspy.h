/*
Copyright (c) 2012, Jared Boone <jared@sharebrained.com>
Copyright (c) 2013, Michael Ossmann <mike@ossmann.com>
Copyright (c) 2013, Benjamin Vernoux <bvernoux@airspy.com>
Copyright (C) 2013, Youssef Touil <youssef@airspy.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the 
	documentation and/or other materials provided with the distribution.
    Neither the name of Great Scott Gadgets nor the names of its contributors may be used to endorse or promote products derived from this software
	without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __AIRSPY_H__
#define __AIRSPY_H__

#include <stdint.h>

#ifdef _WIN32
   #define ADD_EXPORTS
   
  /* You should define ADD_EXPORTS *only* when building the DLL. */
  #ifdef ADD_EXPORTS
    #define ADDAPI __declspec(dllexport)
  #else
    #define ADDAPI __declspec(dllimport)
  #endif

  /* Define calling convention in one place, for convenience. */
  #define ADDCALL __cdecl

#else /* _WIN32 not defined. */

  /* Define with no value on non-Windows OSes. */
  #define ADDAPI
  #define ADDCALL

#endif

enum airspy_error
{
	AIRSPY_SUCCESS = 0,
	AIRSPY_TRUE = 1,
	AIRSPY_ERROR_INVALID_PARAM = -2,
	AIRSPY_ERROR_NOT_FOUND = -5,
	AIRSPY_ERROR_BUSY = -6,
	AIRSPY_ERROR_NO_MEM = -11,
	AIRSPY_ERROR_LIBUSB = -1000,
	AIRSPY_ERROR_THREAD = -1001,
	AIRSPY_ERROR_STREAMING_THREAD_ERR = -1002,
	AIRSPY_ERROR_STREAMING_STOPPED = -1003,
	AIRSPY_ERROR_OTHER = -9999,
};

enum airspy_board_id
{
	AIRSPY_BOARD_ID_PROTO_AIRSPY  = 0,
	AIRSPY_BOARD_ID_INVALID = 0xFF,
};

enum airspy_sample_type
{
	AIRSPY_SAMPLE_FLOAT32_IQ = 0, /* 2*32bits(float) per sample */
	AIRSPY_SAMPLE_FLOAT32_REAL = 1, /* 1*32bits(float) per sample */
	AIRSPY_SAMPLE_INT16_IQ = 2, /* 2*16bits per sample */
	AIRSPY_SAMPLE_INT16_REAL = 3 /* 1*16bits per sample */
};

struct airspy_device;

typedef struct {
	struct airspy_device* device;
	void* ctx;
	void* samples;
	int sample_count;
	enum airspy_sample_type sample_type;
} airspy_transfer_t, airspy_transfer;

typedef enum {
	RECEIVER_MODE_OFF = 0,
	RECEIVER_MODE_RX = 1
} receiver_mode_t;

typedef struct {
	uint32_t part_id[2];
	uint32_t serial_no[4];
} airspy_read_partid_serialno_t;

typedef int (*airspy_sample_block_cb_fn)(airspy_transfer* transfer);

#ifdef __cplusplus
extern "C"
{
#endif

extern ADDAPI int ADDCALL airspy_init();
extern ADDAPI int ADDCALL airspy_exit();
 
extern ADDAPI int ADDCALL airspy_open(struct airspy_device** device);
extern ADDAPI int ADDCALL airspy_close(struct airspy_device* device);
 
extern ADDAPI int ADDCALL airspy_start_rx(struct airspy_device* device, airspy_sample_block_cb_fn callback, void* rx_ctx);
extern ADDAPI int ADDCALL airspy_stop_rx(struct airspy_device* device);

/* return AIRSPY_TRUE if success */
extern ADDAPI int ADDCALL airspy_is_streaming(struct airspy_device* device);
 
extern ADDAPI int ADDCALL airspy_si5351c_write(struct airspy_device* device, uint8_t register_number, uint8_t value);
extern ADDAPI int ADDCALL airspy_si5351c_read(struct airspy_device* device, uint8_t register_number, uint8_t* value);

extern ADDAPI int ADDCALL airspy_r820t_write(struct airspy_device* device, uint8_t register_number, uint8_t value);
extern ADDAPI int ADDCALL airspy_r820t_read(struct airspy_device* device, uint8_t register_number, uint8_t* value);
 
extern ADDAPI int ADDCALL airspy_spiflash_erase(struct airspy_device* device);
extern ADDAPI int ADDCALL airspy_spiflash_write(struct airspy_device* device, const uint32_t address, const uint16_t length, unsigned char* const data);
extern ADDAPI int ADDCALL airspy_spiflash_read(struct airspy_device* device, const uint32_t address, const uint16_t length, unsigned char* data);
		
extern ADDAPI int ADDCALL airspy_board_id_read(struct airspy_device* device, uint8_t* value);
extern ADDAPI int ADDCALL airspy_version_string_read(struct airspy_device* device, char* version, uint8_t length);

extern ADDAPI int ADDCALL airspy_board_partid_serialno_read(struct airspy_device* device, airspy_read_partid_serialno_t* read_partid_serialno);

extern ADDAPI int ADDCALL airspy_set_sample_type(struct airspy_device* device, enum airspy_sample_type sample_type);

extern ADDAPI int ADDCALL airspy_set_freq(struct airspy_device* device, const uint32_t freq_hz);
extern ADDAPI int ADDCALL airspy_set_lna_gain(struct airspy_device* device, uint8_t value);
extern ADDAPI int ADDCALL airspy_set_mixer_gain(struct airspy_device* device, uint8_t value);
extern ADDAPI int ADDCALL airspy_set_vga_gain(struct airspy_device* device, uint8_t value);
extern ADDAPI int ADDCALL airspy_set_lna_agc(struct airspy_device* device, uint8_t value);
extern ADDAPI int ADDCALL airspy_set_mixer_agc(struct airspy_device* device, uint8_t value);

extern ADDAPI const char* ADDCALL airspy_error_name(enum airspy_error errcode);
extern ADDAPI const char* ADDCALL airspy_board_id_name(enum airspy_board_id board_id);

#ifdef __cplusplus
} // __cplusplus defined.
#endif

#endif//__AIRSPY_H__