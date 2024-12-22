
#include "ADCSampler.h"

#ifdef USE_MODULE_SENSORS_ADC_I2S_INTERNAL

#include <Arduino.h>
#include "driver/i2s.h"
#include "driver/adc.h"

ADCSampler::ADCSampler(adc_unit_t adcUnit, adc1_channel_t adcChannel)
{
    m_adcUnit = adcUnit;
    m_adcChannel = adcChannel;
}

void ADCSampler::configureI2S()
{
    //init ADC pad
    i2s_set_adc_mode(m_adcUnit, m_adcChannel);
    // enable the adc
    i2s_adc_enable(getI2SPort());
}

#endif // USE_MODULE_SENSORS_ADC_I2S_INTERNAL