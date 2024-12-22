#ifndef __adc_sampler_h__
#define __adc_sampler_h__

#include "I2SSampler.h"

#ifdef USE_MODULE_SENSORS_ADC_I2S_INTERNAL

class ADCSampler : public I2SSampler
{
private:
    adc_unit_t m_adcUnit;
    adc1_channel_t m_adcChannel;

protected:
    void configureI2S();

public:
    ADCSampler(adc_unit_t adc_unit, adc1_channel_t adc_channel);
    
};

#endif // USE_MODULE_SENSORS_ADC_I2S_INTERNAL

#endif