
#include "I2SSampler.h"

#ifdef USE_MODULE_SENSORS_ADC_I2S_INTERNAL

#include <Arduino.h>
#include "driver/i2s.h"

#define DMA_BYTE_SIZE 1000
#define PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM 32 //temp fix to get in scope


uint8_t extern_flag_swap_ringbuffers_before_writting_is2_data = 0;


void i2sReaderTask(void *param)
{
    I2SSampler *sampler = (I2SSampler *)param;
    while (true)
    {


        // wait for some data to arrive on the queue
        i2s_event_t evt;
        if (xQueueReceive(sampler->m_i2sQueue, &evt, portMAX_DELAY) == pdPASS)
        {
            if (evt.type == I2S_EVENT_RX_DONE)
            {
                size_t bytesRead = 0;
                do
                {
                    //    Serial.println("do SET");
                    // read data from the I2S peripheral
                    uint8_t i2sData[DMA_BYTE_SIZE+3]; // 2 extra for number
                    // read from i2s DMA
                    i2s_read(sampler->getI2SPort(), i2sData, DMA_BYTE_SIZE, &bytesRead, pdMS_TO_TICKS(1));      //Read from DMA, then pass the pointer to be processed

                    /**
                     * To get around ISR issue, check a flag here before commiting to the new buffer, and swap if flag was set
                     * */
                    if(extern_flag_swap_ringbuffers_before_writting_is2_data)
                    {
                        extern_flag_swap_ringbuffers_before_writting_is2_data = false;
                        // std::swap(sampler->ringbuffer_handle_writer_ptr, sampler->ringbuffer_handle_reader_ptr);
                        sampler->active_ringbuffer_writting_index ^= 1;
                        // Serial.printf("extern_flag_swap_ringbuffers_before_writting_is2_data SET %d\n\r", millis());
                    }

                    // Push latest DMA read into ringbuffer, itemized
                    //if(sampler->flag_enabled_store_captured_results)
                    if(digitalRead(PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM)==LOW)
                    {
                        // uint16_t* rxon_write = &i2sData[1000];
                        i2sData[1000] = (sampler->item_id_counter) & 0xff;//;
                        i2sData[1001] = (sampler->item_id_counter >> 8) & 0xff;//;
                        sampler->item_id_counter++;
                        i2sData[1002] = sampler->active_ringbuffer_writting_index;//;
                        // i2sData[1002] = (sampler->active_ringbuffer_writting_index) & 0xff;//;
                        // i2sData[1003] = (sampler->active_ringbuffer_writting_index >> 8) & 0xff;//;
                        
                        if(sampler->active_ringbuffer_writting_index){
                            xRingbufferSend(sampler->ringbuffer_handle1, i2sData, DMA_BYTE_SIZE+3, pdMS_TO_TICKS(1));
                        }else{
                            xRingbufferSend(sampler->ringbuffer_handle2, i2sData, DMA_BYTE_SIZE+3, pdMS_TO_TICKS(1));
                        }
                    }
                    // Inform writer task that new data has been added
                    // xTaskNotify(sampler->m_writerTaskHandle, 1, eIncrement);
                } while (bytesRead > 0);
            }
        }
    }
}

void I2SSampler::start(i2s_port_t i2sPort, i2s_config_t &i2sConfig, int32_t bufferSizeInBytes, TaskHandle_t writerTaskHandle)
{
    m_i2sPort = i2sPort;

    ringbuffer_handle1 = xRingbufferCreate(ring_buffer_size_rx1, RINGBUF_TYPE_NOSPLIT);
    // if(ringbuffer_handle1 == NULL)
    // {
    //     Serial.println("if ringbuffer_handle1 = xRingbufferCreate"); 
    // }else{
    //     Serial.println("else ringbuffer_handle1 = xRingbufferCreate");
    // }
    ringbuffer_handle2 = xRingbufferCreate(ring_buffer_size_rx1, RINGBUF_TYPE_NOSPLIT);

    /**
     * starting with 1 as writer, 2 as reader then swap
     * */
    // ringbuffer_handle_writer_ptr = ringbuffer_handle1;
    // ringbuffer_handle_reader_ptr = ringbuffer_handle2;

    flag_sampler_initialized = true;

    m_writerTaskHandle = writerTaskHandle;
    //install and start i2s driver
    i2s_driver_install(m_i2sPort, &i2sConfig, 4, &m_i2sQueue);
    // set up the I2S configuration from the subclass
    configureI2S();
    // start a task to read samples from the ADC
    TaskHandle_t readerTaskHandle;
    xTaskCreatePinnedToCore(i2sReaderTask, "i2s Reader Task", 4096, this, 1, &readerTaskHandle, 0);
}

void I2SSampler::SwapReaderWritersRingbuffers()
{
  if(!flag_sampler_initialized){ return; }
//   std::swap(ringbuffer_handle_writer_ptr, ringbuffer_handle_reader_ptr);
  active_ringbuffer_writting_index ^= 1;
}

#endif // USE_MODULE_SENSORS_ADC_I2S_INTERNAL