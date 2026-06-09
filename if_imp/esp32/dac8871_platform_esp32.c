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

#include "dac8871_platform_esp32.h"


dac8871_status_e dac8871_esp32_write_16b( uint16_t dat, void* arg );
dac8871_status_e dac8871_esp32_set_ldac( bool lvl, void* arg );
dac8871_status_e dac8871_esp32_set_rst( bool lvl, void* arg );

dac8871_if_t dac8871_if_esp32 = {
  .write_16b  = dac8871_esp32_write_16b,
  .set_ldac   = dac8871_esp32_set_ldac,
  .set_rst    = dac8871_esp32_set_rst,
};

dac8871_status_e dac8871_esp32_write_16b( uint16_t dat, void* arg ){
  esp_err_t ret = ESP_OK;
  dac8871_if_esp32_arg_t* if_args = (dac8871_if_esp32_arg_t*)arg;
    spi_transaction_t trans = {     // Configure common transaction settings (also ensures that the transfer is zero-initialized in other entries)
    .length = 2*8,
    .flags = SPI_TRANS_USE_TXDATA,
  };
  trans.tx_data[0] = (uint8_t)(((uint16_t)dat & 0xFF00) >> 8);
  trans.tx_data[1] = (uint8_t)(((uint16_t)dat & 0x00FF) >> 0);
  if(!if_args->spi_initialized){
    spi_device_interface_config_t devcfg={
        .clock_speed_hz = if_args->clk_freq,
        .mode = 0,                                // SPI mode 0
        .spics_io_num = if_args->cs_pin,        // Let ESP32 SPI driver handle SYNC line
        .queue_size = if_args->spi_q_size,
    };
    spi_bus_add_device(if_args->host, &devcfg, &(if_args->spi));
    if_args->spi_initialized = true;
  }
  ret = spi_device_transmit(if_args->spi, &trans);
  return (ret == ESP_OK) ? DAC8871_STAT_OK : DAC8871_STAT_ERR;
}

dac8871_status_e dac8871_esp32_set_ldac( bool lvl, void* arg ){
  dac8871_status_e retval = DAC8871_STAT_OK;
  dac8871_if_esp32_arg_t* if_args = (dac8871_if_esp32_arg_t*)arg;
  if(!if_args->ldac_initialized){
    gpio_pad_select_gpio(if_args->ldac_pin);
    gpio_set_direction(if_args->ldac_pin, GPIO_MODE_OUTPUT);
    if_args->ldac_initialized = true;
  }
  gpio_set_level(if_args->ldac_pin, lvl);
  return retval;
}

dac8871_status_e dac8871_esp32_set_rst( bool lvl, void* arg ){
  dac8871_status_e retval = DAC8871_STAT_OK;
  dac8871_if_esp32_arg_t* if_args = (dac8871_if_esp32_arg_t*)arg;
  if(!if_args->rst_initialized){
    gpio_pad_select_gpio(if_args->rst_pin);
    gpio_set_direction(if_args->rst_pin, GPIO_MODE_OUTPUT);
    if_args->rst_initialized = true;
  }
  gpio_set_level(if_args->rst_pin, lvl);
  return retval;
}
