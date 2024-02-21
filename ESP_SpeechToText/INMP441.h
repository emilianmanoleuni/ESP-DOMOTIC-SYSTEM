#ifndef __INMP441__
#define __INMP441__

#include <driver/i2s.h>

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32
#define I2S_PORT I2S_NUM_0
#define I2S_CHANNEL_NUM 1

#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define RECORD_TIME       (5) //Seconds
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * RECORD_TIME)


class INMP441 {
private:
    const i2s_config_t i2s_config; 
    const i2s_pin_config_t pin_config;
public:
    INMP441():
        i2s_config{
            .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
            .sample_rate = I2S_SAMPLE_RATE,
            .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
            .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
            .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
            .intr_alloc_flags = 0,
            .dma_buf_count = 64,
            .dma_buf_len = 1024,
            .use_apll = false
        },
        pin_config {
            .bck_io_num = I2S_SCK,
            .ws_io_num = I2S_WS,
            .data_out_num = -1,
            .data_in_num = I2S_SD
        }{};

    void init(void){
        i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
        i2s_set_pin(I2S_PORT, &pin_config);
    };
    void reset(){
      i2s_driver_uninstall(I2S_PORT);
      i2s_driver_install(I2S_PORT, &i2s_config, 0, nullptr);
      i2s_set_pin(I2S_PORT, &pin_config);
      i2s_zero_dma_buffer(I2S_PORT);
    }
};




#endif
