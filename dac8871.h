/*
Copyright 2019 Owen Lyke

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, 
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _DAC8871_H_
#define _DAC8871_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
  DAC8871_STAT_OK = 0x00,
  DAC8871_STAT_ERR,
  DAC8871_STAT_ERR_INVALID_ARG,

  DAC8871_STAT_NUM
} dac8871_status_e;

typedef struct _dac8871_if_t {
  dac8871_status_e (*write_16b) ( uint16_t dat, void* arg );  // required. send 16 bits to the DAC8871 using CS, CLK, and MOSI lines. Send MSB first
  dac8871_status_e (*set_ldac)  ( bool lvl, void* arg );      // optional. control the level of the LDAC line. if not implemented LDAC should be pulled LOW
  dac8871_status_e (*set_rst)   ( bool lvl, void* arg );      // optional. control the level of the RESET line. if not implemented RESET should be pulled HIGH
} dac8871_if_t;

typedef struct _dac8871_dev_t {
  dac8871_if_t* _if;            // interface used to control this device
  void*         _arg;           // user assignable pointer to pass into interface functions
} dac8871_dev_t;

dac8871_status_e dac8871_init_dev(dac8871_dev_t* pdev, dac8871_if_t* pif, void* arg);
dac8871_status_e dac8871_set_code(dac8871_dev_t* pdev, uint16_t code);

#endif // _DAC8871_H_