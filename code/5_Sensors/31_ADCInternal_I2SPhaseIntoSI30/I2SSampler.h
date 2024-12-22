#ifndef __sampler_base_h__
#define __sampler_base_h__


#ifdef USE_MODULE_SENSORS_ADC_I2S_INTERNAL

#include <Arduino.h>
#include "driver/i2s.h"
#include "freertos/ringbuf.h"


extern uint8_t extern_flag_swap_ringbuffers_before_writting_is2_data;

/**
 * Base Class for both the ADC and I2S sampler
 **/
class I2SSampler
{
private:
    // I2S reader task
    TaskHandle_t m_readerTaskHandle;
    // writer task
    TaskHandle_t m_writerTaskHandle;
    // i2s reader queue
    QueueHandle_t m_i2sQueue;
    // i2s port
    i2s_port_t m_i2sPort;

protected:

    virtual void configureI2S() = 0;
    
    i2s_port_t getI2SPort()
    {
        return m_i2sPort;
    }

public:

    #define ring_buffer_size_rx1 60000
    RingbufHandle_t ringbuffer_handle1;
    QueueHandle_t event_queue_handle1;
    RingbufHandle_t ringbuffer_handle2;
    QueueHandle_t event_queue_handle2;

    /**
     * Pointer to "written into buffer"
     * Pointer to "last frame, results to be read from"
     * */
    // RingbufHandle_t ringbuffer_handle_writer_ptr = nullptr;
    // RingbufHandle_t ringbuffer_handle_reader_ptr = nullptr;

    bool flag_sampler_initialized = false;
    void SwapReaderWritersRingbuffers();

    uint16_t item_id_counter = 0;
    uint16_t active_ringbuffer_writting_index = 0;

    // bool flag_enabled_store_captured_results = false;
    
    void start(i2s_port_t i2sPort, i2s_config_t &i2sConfig, int32_t bufferSizeInSamples, TaskHandle_t writerTaskHandle);

    friend void i2sReaderTask(void *param);
};

#endif

#endif // USE_MODULE_SENSORS_ADC_I2S_INTERNAL

// gpio_num_t pin = (gpio_num_t)(your_pin_number_0to31 & 0x1F);
// int state=0;
// if (GPIO_REG_READ(GPIO_ENABLE_REG) & BIT(pin)){
// 	//pin is output - read the GPIO_OUT_REG register
// 	state = (GPIO_REG_READ(GPIO_OUT_REG)  >> pin) & 1U;
// }
// else
// {
// 	//pin is input - read the GPIO_IN_REG register
// 	state = (GPIO_REG_READ(GPIO_IN_REG)  >> pin) & 1U;
// }
