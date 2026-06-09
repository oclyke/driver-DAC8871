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

#include "dac8871.h"

dac8871_status_e dac8871_init_dev(dac8871_dev_t* pdev, dac8871_if_t* pif, void* arg){
  if(pdev == NULL)          { return DAC8871_STAT_ERR_INVALID_ARG; }
  if(pif == NULL)           { return DAC8871_STAT_ERR_INVALID_ARG; }
  pdev->_if = pif;
  pdev->_arg = arg;
  if(pdev->_if->set_rst != NULL) {
    pdev->_if->set_rst(false, pdev->_arg); // assert RESET (active low) to clear all registers
    pdev->_if->set_rst(true, pdev->_arg);  // deassert RESET — minimum pulse width is 10 ns per datasheet;
  }                                        // two sequential function calls satisfy this on any MCU
  if(pdev->_if->set_ldac != NULL) { pdev->_if->set_ldac(false, pdev->_arg); } // hold LDAC low so writes are automatically latched
  return DAC8871_STAT_OK;
}

dac8871_status_e dac8871_set_code(dac8871_dev_t* pdev, uint16_t code){
  if(pdev == NULL){ return DAC8871_STAT_ERR_INVALID_ARG; }
  if(pdev->_if == NULL){ return DAC8871_STAT_ERR_INVALID_ARG; }
  if(pdev->_if->write_16b == NULL){ return DAC8871_STAT_ERR_INVALID_ARG; }
  return pdev->_if->write_16b(code, pdev->_arg);
}

dac8871_status_e dac8871_latch(dac8871_dev_t* pdev){
  if(pdev == NULL){ return DAC8871_STAT_ERR_INVALID_ARG; }
  if(pdev->_if == NULL){ return DAC8871_STAT_ERR_INVALID_ARG; }
  if(pdev->_if->set_ldac == NULL){ return DAC8871_STAT_OK; } // LDAC tied low externally; latch is always active
  pdev->_if->set_ldac(true, pdev->_arg);  // deassert LDAC — holds input register stable
  pdev->_if->set_ldac(false, pdev->_arg); // assert LDAC — transfers input register to DAC register
  return DAC8871_STAT_OK;
}
