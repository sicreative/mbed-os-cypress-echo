/*
 * sound.cpp
 *
 *  Created on: 21 Apr 2020
 *      Author: slee
 *      
 *      
 *      For handling pcm mic and i2s sound
 */

#include "sound.h"
#include "mbed.h"
#include "cy_pdl.h"

#include "cyhal.h"
#include "ak4954a.h"
#include "hal/static_pinmap.h"



extern serial_t stdio_uart;
extern int stdio_uart_inited;
bool UART_OUTPUT = true;






/* DMA config */
const cy_stc_dma_descriptor_config_t CYBSP_DMA_PDM_Descriptor_0_config =
{
	.retrigger = CY_DMA_RETRIG_16CYC,
	.interruptType = CY_DMA_DESCR,
	.triggerOutType = CY_DMA_DESCR,
	.channelState = CY_DMA_CHANNEL_ENABLED,
	.triggerInType = CY_DMA_1ELEMENT,
	.dataSize = CY_DMA_HALFWORD,
	.srcTransferSize = CY_DMA_TRANSFER_SIZE_WORD,
	.dstTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.descriptorType = CY_DMA_1D_TRANSFER,
	.srcAddress = NULL,
	.dstAddress = NULL,
	.srcXincrement = 0,
	.dstXincrement = 1, //1
	.xCount = 256, //256
	.srcYincrement = 0, //0
	.dstYincrement = 0, //256
	.yCount = 256, //256
	.nextDescriptor = &CYBSP_DMA_PDM_Descriptor_0,
};
cy_stc_dma_descriptor_t CYBSP_DMA_PDM_Descriptor_0 =
{
	.ctl = 0UL,
	.src = 0UL,
	.dst = 0UL,
	.xCtl = 0UL,
	.yCtl = 0UL,
	.nextPtr = 0UL,
};
const cy_stc_dma_channel_config_t CYBSP_DMA_PDM_channelConfig =
{
	.descriptor = &CYBSP_DMA_PDM_Descriptor_0,
	.preemptable = false,
	.priority = 3,
	.enable = false,
	.bufferable = false,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_DMA_PDM_obj =
	{
		.type = CYHAL_RSC_DMA,
		.block_num = 0U,
		.channel_num = CYBSP_DMA_PDM_CHANNEL,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_dma_descriptor_config_t CYBSP_DMA_I2S_Descriptor_0_config =
{
	.retrigger = CY_DMA_RETRIG_16CYC,
	.interruptType = CY_DMA_DESCR,
	.triggerOutType = CY_DMA_1ELEMENT,
	.channelState = CY_DMA_CHANNEL_DISABLED,
	.triggerInType = CY_DMA_1ELEMENT,
	.dataSize = CY_DMA_HALFWORD,
	.srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.dstTransferSize = CY_DMA_TRANSFER_SIZE_WORD,
	.descriptorType = CY_DMA_2D_TRANSFER,
	.srcAddress = NULL,
	.dstAddress = NULL,
	.srcXincrement = 0,
	.dstXincrement = 0,
	.xCount = 2,
	.srcYincrement = 1,
	.dstYincrement = 0,
	.yCount = 256,
	.nextDescriptor = &CYBSP_DMA_I2S_Descriptor_0,
};
cy_stc_dma_descriptor_t CYBSP_DMA_I2S_Descriptor_0 =
{
	.ctl = 0UL,
	.src = 0UL,
	.dst = 0UL,
	.xCtl = 0UL,
	.yCtl = 0UL,
	.nextPtr = 0UL,
};
const cy_stc_dma_channel_config_t CYBSP_DMA_I2S_channelConfig =
{
	.descriptor = &CYBSP_DMA_I2S_Descriptor_0,
	.preemptable = false,
	.priority = 3,
	.enable = false,
	.bufferable = false,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_DMA_I2S_obj =
	{
		.type = CYHAL_RSC_DMA,
		.block_num = 0U,
		.channel_num = CYBSP_DMA_I2S_CHANNEL,
	};
#endif //defined (CY_USING_HAL)


#define ioss_0_port_10_pin_4_HSIOM P10_4_AUDIOSS_PDM_CLK
#define ioss_0_port_10_pin_5_HSIOM P10_5_AUDIOSS_PDM_DATA

#define ioss_0_port_10_pin_4_ENABLED 1U
#define ioss_0_port_10_pin_4_PORT GPIO_PRT10
#define ioss_0_port_10_pin_4_PORT_NUM 10U
#define ioss_0_port_10_pin_4_PIN 4U
#define ioss_0_port_10_pin_4_NUM 4U
#define ioss_0_port_10_pin_4_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_10_pin_4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_4_HSIOM
	#define ioss_0_port_10_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_10_pin_4_IRQ ioss_interrupts_gpio_10_IRQn
#if defined (CY_USING_HAL)
	#define ioss_0_port_10_pin_4_HAL_PORT_PIN P10_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_10_pin_4_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_10_pin_4_HAL_DIR CYHAL_GPIO_DIR_OUTPUT
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_10_pin_4_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define ioss_0_port_10_pin_5_ENABLED 1U
#define ioss_0_port_10_pin_5_PORT GPIO_PRT10
#define ioss_0_port_10_pin_5_PORT_NUM 10U
#define ioss_0_port_10_pin_5_PIN 5U
#define ioss_0_port_10_pin_5_NUM 5U
#define ioss_0_port_10_pin_5_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ioss_0_port_10_pin_5_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_5_HSIOM
	#define ioss_0_port_10_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_10_pin_5_IRQ ioss_interrupts_gpio_10_IRQn
#if defined (CY_USING_HAL)
	#define ioss_0_port_10_pin_5_HAL_PORT_PIN P10_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_10_pin_5_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_10_pin_5_HAL_DIR CYHAL_GPIO_DIR_INPUT
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_10_pin_5_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)

#define CYBSP_I2S_ENABLED 1U
#define CYBSP_I2S_HW I2S
#define CYBSP_I2S_IRQ audioss_interrupt_i2s_IRQn
#define ioss_0_port_5_pin_1_HSIOM P5_1_AUDIOSS_TX_SCK
#define ioss_0_port_5_pin_2_HSIOM P5_2_AUDIOSS_TX_WS
#define ioss_0_port_5_pin_3_HSIOM P5_3_AUDIOSS_TX_SDO

#define CYBSP_DMA_I2S_tr_in_0_TRIGGER_OUT TRIG0_OUT_CPUSS_DW0_TR_IN3
#define CYBSP_DMA_PDM_tr_in_0_TRIGGER_OUT TRIG1_OUT_CPUSS_DW1_TR_IN2
#define CYBSP_I2S_tr_i2s_tx_req_0_TRIGGER_IN_0 TRIG0_IN_TR_GROUP13_OUTPUT1
#define CYBSP_I2S_tr_i2s_tx_req_0_TRIGGER_IN_1 TRIG13_IN_AUDIOSS_TR_I2S_TX_REQ
#define CYBSP_PDM_tr_pdm_rx_req_0_TRIGGER_IN_0 TRIG0_IN_TR_GROUP13_OUTPUT0
#define CYBSP_PDM_tr_pdm_rx_req_0_TRIGGER_IN_1 TRIG13_IN_AUDIOSS_TR_PDM_RX_REQ




#define CYBSP_PDM_ENABLED 1U
#define CYBSP_PDM_HW PDM
#define CYBSP_PDM_IRQ audioss_interrupt_pdm_IRQn






#define ioss_0_port_5_pin_1_ENABLED 1U
#define ioss_0_port_5_pin_1_PORT GPIO_PRT5
#define ioss_0_port_5_pin_1_PORT_NUM 5U
#define ioss_0_port_5_pin_1_PIN 1U
#define ioss_0_port_5_pin_1_NUM 1U
#define ioss_0_port_5_pin_1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_5_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
	#define ioss_0_port_5_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_5_pin_1_IRQ ioss_interrupts_gpio_5_IRQn
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_1_HAL_PORT_PIN P5_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_1_HAL_DIR CYHAL_GPIO_DIR_OUTPUT
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define ioss_0_port_5_pin_2_ENABLED 1U
#define ioss_0_port_5_pin_2_PORT GPIO_PRT5
#define ioss_0_port_5_pin_2_PORT_NUM 5U
#define ioss_0_port_5_pin_2_PIN 2U
#define ioss_0_port_5_pin_2_NUM 2U
#define ioss_0_port_5_pin_2_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_5_pin_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_2_HSIOM
	#define ioss_0_port_5_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_5_pin_2_IRQ ioss_interrupts_gpio_5_IRQn
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_2_HAL_PORT_PIN P5_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_2_HAL_DIR CYHAL_GPIO_DIR_OUTPUT
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define ioss_0_port_5_pin_3_ENABLED 1U
#define ioss_0_port_5_pin_3_PORT GPIO_PRT5
#define ioss_0_port_5_pin_3_PORT_NUM 5U
#define ioss_0_port_5_pin_3_PIN 3U
#define ioss_0_port_5_pin_3_NUM 3U
#define ioss_0_port_5_pin_3_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_5_pin_3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_3_HSIOM
	#define ioss_0_port_5_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_5_pin_3_IRQ ioss_interrupts_gpio_5_IRQn
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_3_HAL_PORT_PIN P5_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_3_HAL_DIR CYHAL_GPIO_DIR_OUTPUT
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define ioss_0_port_5_pin_3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)


	const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_4_config =
	{
		.outVal = 1,
		.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
		.hsiom = ioss_0_port_10_pin_4_HSIOM,
		.intEdge = CY_GPIO_INTR_DISABLE,
		.intMask = 0UL,
		.vtrip = CY_GPIO_VTRIP_CMOS,
		.slewRate = CY_GPIO_SLEW_FAST,
		.driveSel = CY_GPIO_DRIVE_1_2,
		.vregEn = 0UL,
		.ibufMode = 0UL,
		.vtripSel = 0UL,
		.vrefSel = 0UL,
		.vohSel = 0UL,
	};
	#if defined (CY_USING_HAL)
		const cyhal_resource_inst_t ioss_0_port_10_pin_4_obj =
		{
			.type = CYHAL_RSC_GPIO,
			.block_num = ioss_0_port_10_pin_4_PORT_NUM,
			.channel_num = ioss_0_port_10_pin_4_PIN,
		};
	#endif //defined (CY_USING_HAL)
	const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_5_config =
	{
		.outVal = 1,
		.driveMode = CY_GPIO_DM_HIGHZ,
		.hsiom = ioss_0_port_10_pin_5_HSIOM,
		.intEdge = CY_GPIO_INTR_DISABLE,
		.intMask = 0UL,
		.vtrip = CY_GPIO_VTRIP_CMOS,
		.slewRate = CY_GPIO_SLEW_FAST,
		.driveSel = CY_GPIO_DRIVE_1_2,
		.vregEn = 0UL,
		.ibufMode = 0UL,
		.vtripSel = 0UL,
		.vrefSel = 0UL,
		.vohSel = 0UL,
	};
	#if defined (CY_USING_HAL)
		const cyhal_resource_inst_t ioss_0_port_10_pin_5_obj =
		{
			.type = CYHAL_RSC_GPIO,
			.block_num = ioss_0_port_10_pin_5_PORT_NUM,
			.channel_num = ioss_0_port_10_pin_5_PIN,
		};
	#endif //defined (CY_USING_HAL)





	const cy_stc_pdm_pcm_config_t CYBSP_PDM_config =
	{
		.clkDiv = CY_PDM_PCM_CLK_DIV_1_2,
		.mclkDiv = CY_PDM_PCM_CLK_DIV_1_2,
		.ckoDiv = 7U,
		.ckoDelay = 0U,
		.sincDecRate = 32U,
		.chanSelect = CY_PDM_PCM_OUT_CHAN_LEFT,
		.chanSwapEnable = false,
		.highPassFilterGain = 8U,
		.highPassDisable = false,
		.softMuteCycles = CY_PDM_PCM_SOFT_MUTE_CYCLES_96,
		.softMuteFineGain = 1UL,
		.softMuteEnable = false,
		.wordLen = CY_PDM_PCM_WLEN_16_BIT,
		.signExtension = false,
		.gainLeft = CY_PDM_PCM_GAIN_10_5_DB,
		.gainRight = CY_PDM_PCM_GAIN_10_5_DB,
		.rxFifoTriggerLevel = 0U,
		.dmaTriggerEnable = true,
		.interruptMask = 0UL,



	};



	const cy_stc_i2s_config_t CYBSP_I2S_config =
	{
		.txEnabled = true,
		.rxEnabled = false,
		.txDmaTrigger = true,
		.rxDmaTrigger = false,
		.clkDiv = 8,
		.extClk = false,
		.txMasterMode = 1,
		.txAlignment = CY_I2S_I2S_MODE,
		.txWsPulseWidth = CY_I2S_WS_ONE_CHANNEL_LENGTH,
		.txWatchdogEnable = false,
		.txWatchdogValue = 4294967295,
		.txSdoLatchingTime = 0,
		.txSckoInversion = 0,
		.txSckiInversion = 0,
		.txChannels = 2,
		.txChannelLength = CY_I2S_LEN16,
		.txWordLength = CY_I2S_LEN16,
		.txOverheadValue = CY_I2S_OVHDATA_ONE,
		.txFifoTriggerLevel = 128,
		.rxMasterMode = 1,
		.rxAlignment = CY_I2S_I2S_MODE,
		.rxWsPulseWidth = CY_I2S_WS_ONE_CHANNEL_LENGTH,
		.rxWatchdogEnable = false,
		.rxWatchdogValue = 4294967295,
		.rxSdiLatchingTime = 0,
		.rxSckoInversion = 0,
		.rxSckiInversion = 0,
		.rxChannels = 2,
		.rxChannelLength = CY_I2S_LEN16,
		.rxWordLength = CY_I2S_LEN16,
		.rxSignExtension = 0,
		.rxFifoTriggerLevel = 128,
	};







		const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_1_config =
		{
			.outVal = 1,
			.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
			.hsiom = ioss_0_port_5_pin_1_HSIOM,
			.intEdge = CY_GPIO_INTR_DISABLE,
			.intMask = 0UL,
			.vtrip = CY_GPIO_VTRIP_CMOS,
			.slewRate = CY_GPIO_SLEW_FAST,
			.driveSel = CY_GPIO_DRIVE_1_2,
			.vregEn = 0UL,
			.ibufMode = 0UL,
			.vtripSel = 0UL,
			.vrefSel = 0UL,
			.vohSel = 0UL,
		};
		#if defined (CY_USING_HAL)
			const cyhal_resource_inst_t ioss_0_port_5_pin_1_obj =
			{
				.type = CYHAL_RSC_GPIO,
				.block_num = ioss_0_port_5_pin_1_PORT_NUM,
				.channel_num = ioss_0_port_5_pin_1_PIN,
			};
		#endif //defined (CY_USING_HAL)
		const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_2_config =
		{
			.outVal = 1,
			.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
			.hsiom = ioss_0_port_5_pin_2_HSIOM,
			.intEdge = CY_GPIO_INTR_DISABLE,
			.intMask = 0UL,
			.vtrip = CY_GPIO_VTRIP_CMOS,
			.slewRate = CY_GPIO_SLEW_FAST,
			.driveSel = CY_GPIO_DRIVE_1_2,
			.vregEn = 0UL,
			.ibufMode = 0UL,
			.vtripSel = 0UL,
			.vrefSel = 0UL,
			.vohSel = 0UL,
		};
		#if defined (CY_USING_HAL)
			const cyhal_resource_inst_t ioss_0_port_5_pin_2_obj =
			{
				.type = CYHAL_RSC_GPIO,
				.block_num = ioss_0_port_5_pin_2_PORT_NUM,
				.channel_num = ioss_0_port_5_pin_2_PIN,
			};
		#endif //defined (CY_USING_HAL)
		const cy_stc_gpio_pin_config_t ioss_0_port_5_pin_3_config =
		{
			.outVal = 1,
			.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
			.hsiom = ioss_0_port_5_pin_3_HSIOM,
			.intEdge = CY_GPIO_INTR_DISABLE,
			.intMask = 0UL,
			.vtrip = CY_GPIO_VTRIP_CMOS,
			.slewRate = CY_GPIO_SLEW_FAST,
			.driveSel = CY_GPIO_DRIVE_FULL,
			.vregEn = 0UL,
			.ibufMode = 0UL,
			.vtripSel = 0UL,
			.vrefSel = 0UL,
			.vohSel = 0UL,
		};
		#if defined (CY_USING_HAL)
			const cyhal_resource_inst_t ioss_0_port_5_pin_3_obj =
			{
				.type = CYHAL_RSC_GPIO,
				.block_num = ioss_0_port_5_pin_3_PORT_NUM,
				.channel_num = ioss_0_port_5_pin_3_PIN,
			};
		#endif //defined (CY_USING_HAL)











/* AK4954a I2C */

I2C i2c(P6_1 , P6_0 );

uint32_t ak4954a_transmit(uint8_t reg_addr, uint8_t data){

	uint32_t result;
	char buffer[AK4954A_PACKET_SIZE];

    buffer[0] = reg_addr;
    buffer[1] = data;

    /* Send the data over the I2C */
    result = i2c.write(AK4954A_I2C_ADDR << 1, buffer,AK4954A_PACKET_SIZE);

    return result;


}



int16_t record_buffer[2][NUM_CHANNELS][BUFFER_SIZE];




/* interrupt cfg struct */

const cy_stc_sysint_t intPDM_cfg = {		.intrSrc      = CYBSP_DMA_PDM_IRQ,
											.intrPriority = 3u
	                          	      };

const cy_stc_sysint_t intI2SDma_cfg = {		 .intrSrc      = CYBSP_DMA_I2S_IRQ,
		                          	    	         .intrPriority = 3u
		                          	    	    };



/* pdm interrupt for  buffer flipping*/
void pdm_dma_int(){


	Cy_DMA_Channel_ClearInterrupt(CYBSP_DMA_PDM_HW, CYBSP_DMA_PDM_CHANNEL);


	/*Flip Buffer*/

	if (Cy_DMA_Descriptor_GetDstAddress(&CYBSP_DMA_PDM_Descriptor_0)==(void*)record_buffer[0][0])
		Cy_DMA_Descriptor_SetDstAddress(&CYBSP_DMA_PDM_Descriptor_0, (void*)record_buffer[1][0]);
	else
		Cy_DMA_Descriptor_SetDstAddress(&CYBSP_DMA_PDM_Descriptor_0, (void*)record_buffer[0][0]);


	/* Enable channel for start next cycle */

	Cy_DMA_Channel_Enable(CYBSP_DMA_PDM_HW, CYBSP_DMA_PDM_CHANNEL);



}

/* i2s interrupt for  buffer flipping */
void i2s_dma_int(){


	Cy_DMA_Channel_ClearInterrupt(CYBSP_DMA_I2S_HW, CYBSP_DMA_I2S_CHANNEL);

	if (Cy_DMA_Descriptor_GetSrcAddress(&CYBSP_DMA_I2S_Descriptor_0)==(void*)record_buffer[0][0])
		Cy_DMA_Descriptor_SetSrcAddress(&CYBSP_DMA_I2S_Descriptor_0, (void*)record_buffer[1][0]);
	else
		Cy_DMA_Descriptor_SetSrcAddress(&CYBSP_DMA_I2S_Descriptor_0, (void*)record_buffer[0][0]);

	/* Enable channel for start next cycle */

	Cy_DMA_Channel_Enable(CYBSP_DMA_I2S_HW, CYBSP_DMA_I2S_CHANNEL);



}




void sound_init(){
	
	UART_OUTPUT = true;

	APP_INFO(( "\nStart Sound card..."));

	Cy_SysLib_Delay(500);

	/* Config Trigger Mux*/

	Cy_TrigMux_Connect(TRIG1_IN_TR_GROUP13_OUTPUT0, TRIG1_OUT_CPUSS_DW1_TR_IN2, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT1, TRIG0_OUT_CPUSS_DW0_TR_IN3, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG13_IN_AUDIOSS_TR_I2S_TX_REQ, TRIG13_OUT_TR_GROUP0_INPUT28, false, TRIGGER_TYPE_LEVEL);
	Cy_TrigMux_Connect(TRIG13_IN_AUDIOSS_TR_PDM_RX_REQ, TRIG13_OUT_TR_GROUP1_INPUT27, false, TRIGGER_TYPE_LEVEL);


	/* Config PDM port*/

	Cy_GPIO_Pin_Init(ioss_0_port_10_pin_4_PORT, ioss_0_port_10_pin_4_PIN, &ioss_0_port_10_pin_4_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&ioss_0_port_10_pin_4_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(ioss_0_port_10_pin_5_PORT, ioss_0_port_10_pin_5_PIN, &ioss_0_port_10_pin_5_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&ioss_0_port_10_pin_5_obj);
#endif //defined (CY_USING_HAL)



	/* Config I2S port*/


	 Cy_GPIO_Pin_Init(ioss_0_port_5_pin_2_PORT, ioss_0_port_5_pin_2_PIN, &ioss_0_port_5_pin_2_config);
	#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&ioss_0_port_5_pin_2_obj);
	#endif //defined (CY_USING_HAL)

	 Cy_GPIO_Pin_Init(ioss_0_port_5_pin_3_PORT, ioss_0_port_5_pin_3_PIN, &ioss_0_port_5_pin_3_config);
	#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&ioss_0_port_5_pin_3_obj);
	#endif //defined (CY_USING_HAL)





		/* temp stop UART console output for playback */
        UART_OUTPUT = false;



        stdio_uart_inited = false;

        serial_clear(&stdio_uart);
        serial_free(&stdio_uart);


        /*Start GPIO Pin1*/
        Cy_GPIO_Pin_Init(ioss_0_port_5_pin_1_PORT, ioss_0_port_5_pin_1_PIN, &ioss_0_port_5_pin_1_config);



		 /* Initialize PDM/PCM block */
			cy_en_pdm_pcm_status_t result = Cy_PDM_PCM_Init(CYBSP_PDM_HW, &CYBSP_PDM_config);
				if (result==CY_PDM_PCM_BAD_PARAM)
					APP_INFO(( "\nBAD PDM setup"));


	    /* Initialize the DMAs and their descriptor addresses */
		cy_en_dma_status_t reslt =  Cy_DMA_Descriptor_Init(&CYBSP_DMA_PDM_Descriptor_0, &CYBSP_DMA_PDM_Descriptor_0_config);

		if (reslt!=CY_DMA_SUCCESS)
			APP_INFO(( "PDM DMA Init ERROR " ));




	    /* Initialize PDM DMAs and their descriptor addresses */
		Cy_DMA_Descriptor_Init(&CYBSP_DMA_PDM_Descriptor_0, &CYBSP_DMA_PDM_Descriptor_0_config);
		Cy_DMA_Descriptor_SetSrcAddress(&CYBSP_DMA_PDM_Descriptor_0, (void *) &CYBSP_PDM_HW->RX_FIFO_RD);
		Cy_DMA_Descriptor_SetDstAddress(&CYBSP_DMA_PDM_Descriptor_0, (void *) &record_buffer[0][0]);
		Cy_DMA_Channel_Init(CYBSP_DMA_PDM_HW, CYBSP_DMA_PDM_CHANNEL, &CYBSP_DMA_PDM_channelConfig);
		Cy_DMA_Enable(CYBSP_DMA_PDM_HW);

		reslt = Cy_DMA_Descriptor_Init(&CYBSP_DMA_I2S_Descriptor_0, &CYBSP_DMA_I2S_Descriptor_0_config);
		           if (reslt!=CY_DMA_SUCCESS)
		                          	         	APP_INFO(( "I2S  Init ERROR  " ));

		   /* Initialize I2S DMAs and their descriptor addresses */

		Cy_DMA_Descriptor_SetSrcAddress(&CYBSP_DMA_I2S_Descriptor_0, (void *) &record_buffer[1][0]);
		Cy_DMA_Descriptor_SetDstAddress(&CYBSP_DMA_I2S_Descriptor_0, (void *) &CYBSP_I2S_HW->TX_FIFO_WR);

		reslt =  Cy_DMA_Channel_Init(CYBSP_DMA_I2S_HW, CYBSP_DMA_I2S_CHANNEL, &CYBSP_DMA_I2S_channelConfig);
			if (reslt!=CY_DMA_SUCCESS)
				APP_INFO(( " I2S Channel Init ERROR " ));


		Cy_DMA_Enable(CYBSP_DMA_I2S_HW);

		/* Initalize the DMAs Interrupt */




		Cy_SysInt_Init(&intPDM_cfg, &pdm_dma_int);
		NVIC_EnableIRQ((IRQn_Type)intPDM_cfg.intrSrc);
		Cy_DMA_Channel_SetInterruptMask (CYBSP_DMA_PDM_HW, CYBSP_DMA_PDM_CHANNEL, CY_DMA_INTR_MASK);
		Cy_DMA_Channel_SetInterrupt (CYBSP_DMA_PDM_HW, CYBSP_DMA_PDM_CHANNEL);
		Cy_DMA_Channel_SetPriority (CYBSP_DMA_PDM_HW,CYBSP_DMA_PDM_CHANNEL,3);


		Cy_SysInt_Init(&intI2SDma_cfg, &i2s_dma_int);
		NVIC_EnableIRQ((IRQn_Type)intI2SDma_cfg.intrSrc);
		Cy_DMA_Channel_SetInterruptMask (CYBSP_DMA_I2S_HW, CYBSP_DMA_I2S_CHANNEL, CY_DMA_INTR_MASK);
		Cy_DMA_Channel_SetPriority (CYBSP_DMA_I2S_HW,CYBSP_DMA_I2S_CHANNEL,3);



		/* Initialize the I2S block */

					Cy_I2S_Init(CYBSP_I2S_HW, &CYBSP_I2S_config);
					Cy_I2S_ClearTxFifo(CYBSP_I2S_HW);

		/* Put at least one frame into the Tx FIFO */
	               Cy_I2S_WriteTxData(CYBSP_I2S_HW, 0UL);
	               Cy_I2S_WriteTxData(CYBSP_I2S_HW, 0UL);


	               Cy_I2S_EnableTx(CYBSP_I2S_HW);
	    /* Init AK4954A */


		 ak4954a_init(ak4954a_transmit);
		 ak4954a_activate();


			/* Startup PDM and I2S */
		Cy_PDM_PCM_Enable(CYBSP_PDM_HW);


		Cy_SysLib_Delay(100);
		/*Start DMA channel */
		Cy_DMA_Channel_Enable(CYBSP_DMA_PDM_HW, CYBSP_DMA_PDM_CHANNEL);
		Cy_DMA_Channel_Enable(CYBSP_DMA_I2S_HW, CYBSP_DMA_I2S_CHANNEL);













        /* Delay 60s to switch off echo */

        Cy_SysLib_Delay(60*1000);




        /* close codec */
	     ak4954a_deactivate();

	     Cy_I2S_DisableTx(CYBSP_I2S_HW);

	     Cy_PDM_PCM_Disable(CYBSP_PDM_HW);

	    /* disable DMA */
	    Cy_DMA_Channel_Disable(CYBSP_DMA_PDM_HW, CYBSP_DMA_PDM_CHANNEL);
	    Cy_DMA_Channel_Disable(CYBSP_DMA_I2S_HW, CYBSP_DMA_I2S_CHANNEL);


	    Cy_DMA_Disable(CYBSP_DMA_PDM_HW);
	    Cy_DMA_Disable(CYBSP_DMA_I2S_HW);


	    /* free resource */
	    cyhal_utils_disconnect_and_free(P5_0);
	    cyhal_utils_disconnect_and_free(P5_1);


	    /* Recovery UART console */

	    wait_us (10*1000);

	    const serial_pinmap_t pinmap = get_uart_pinmap(STDIO_UART_TX,STDIO_UART_RX );
	    serial_init_direct(&stdio_uart, &pinmap);
	    serial_baud(&stdio_uart, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);


	    stdio_uart_inited = true;

	    UART_OUTPUT = true;

	    wait_us (100*1000);

		APP_INFO(( "\nClosed Sound card..."));


	
}



