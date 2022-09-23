// adc.h
#ifndef ADC_H
#define ADC_H

struct ADCChannel
{
	uint32_t channelID { 0xffffffff };
	uint16_t filteredData { 0 };
	uint32_t summValue { 0 };
};



class AnalogConverter
{

	static uint32_t channelIndex;
	static uint16_t countChannels;

	static constexpr uint16_t countSumm { 100 };


	static constexpr uint16_t ADC_NODATA { 0xffff };
	static constexpr uint16_t MAX_CHANNELS { 19 };
	static ADCChannel channels[MAX_CHANNELS];
	volatile static uint16_t adcData[MAX_CHANNELS];// = {0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000};//adc data in DMA mode

public:
    AnalogConverter() {};

	enum Channels { ADC_0 = 0x00000001, ADC_1 = 0x00000002, ADC_2 = 0x00000004, ADC_3 = 0x00000008, ADC_4 = 0x00000010,
		ADC_5 = 0x00000020, ADC_6 = 0x00000040, ADC_7 = 0x00000080, ADC_8 = 0x00000100, ADC_9 = 0x00000200, ADC_10 = 0x00000400,
		ADC_11 = 0x00000800, ADC_12 = 0x00001000, ADC_13 = 0x00002000, ADC_14 = 0x00004000, ADC_15 = 0x00008000,
		ADC_16 = 0x00010000, ADC_17 = 0x00020000, ADC_18 = 0x00040000};

    void init();

    void initChannels(uint32_t chns);
    void start();
    static void DMA_Handler(void);
    uint16_t getData(uint32_t channel)
    {
    	int i = 0;
    	while(i< countChannels)
    	{
    		if(channels[i].channelID == channel) return adcData[i];//channels[i].data;
    		i++;
    	}
    	return ADC_NODATA;
    }

    uint16_t getDataFiltered(uint32_t channel)
    {
    	int i = 0;
    	while(i< countChannels)
    	{
    		if(channels[i].channelID == channel) return channels[i].filteredData;//channels[i].data;
    		i++;
    	}
    	return ADC_NODATA;
    }

    uint16_t getDataN(uint32_t channel)
    {
    	if(channel<MAX_CHANNELS)
    		return adcData[channel];
    	else
    	return ADC_NODATA;
    }
    uint16_t chnCnt(){ return countChannels; }
private:
    //friend void ADC_Handler(void);

};
#endif //ADC_H
