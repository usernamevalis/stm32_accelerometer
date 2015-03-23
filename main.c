/**
  ******************************************************************************
  * @file    main.c
  * @author  J.Shankarappa
  * @version V0.0.1
  * @date    5-April-2013
  * @brief   Blink LED Program for STM32L-Discovery Board
  ******************************************************************************
  */

#include "stm32l1xx.h"
#include "discover_board.h"
#include <stm32l1xx_rcc.h>
#include <stm32l1xx_gpio.h>
#include <stm32l1xx_tim.h>

void TimingDelay_Decrement(void);  // used in SysTick_Handler function
void Delay(__IO uint32_t nTime);
uint16_t adc_read(ADC_TypeDef* ADCx, uint8_t channel, uint8_t ADC_SampleTime);

static __IO uint32_t TimingDelay;


/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;
DAC_InitTypeDef DAC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;

/* Private function prototypes -----------------------------------------------*/
void ADC_Config(void);
void DAC_Config(void);
void ADC1_IRQHandler(void);

uint16_t ADCVal=0;

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l1xx_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l1xx.c file
     */

  /* DAC configuration ---------------------------------------------*/
  DAC_Config();

  /* ADC configuration ---------------------------------------------*/
  ADC_Config();

  while (1)
  {
	  uint16_t temp = adc_read(ADC1, ADC_Channel_18, ADC_SampleTime_16Cycles);
	  uint16_t temp2 = adc_read(ADC1, ADC_Channel_1, ADC_SampleTime_16Cycles);

  }

}

void DAC_Config(void)
{
  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  /* Configure PA.04 (DAC_OUT1) in analog mode -------------------------*/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable DAC clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

  /* DAC Channel1 Init */
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

/**
  * @brief  Configures the ADC1 channel18 (PB12) with software trigger.
  *         End Of Conversion interrupt enabled.
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{
  /* Enable the HSI oscillator */
  RCC_HSICmd(ENABLE);
  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Configure PB.12 (ADC Channel18) in analog mode -------------------------*/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  /* Check that HSI oscillator is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* ADC1 configuration ------------------------------------------------------*/
   /*
    This is set to ADC_Prescaler_Div1 by default and for a reason lost on me, does not work
    without these two lines the adc will not work.
    It might have to do with the actual clock speed being 2mhz and not 32mhz
  */
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInit(&ADC_CommonInitStructure);

  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel18 configuration */
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_18, 1, ADC_SampleTime_16Cycles);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_16Cycles);


  /* Enable End Of Conversion interrupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Start ADC1 Software Conversion */
//  ADC_SoftwareStartConv(ADC1);
//  /* Configure and enable ADC1 interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
}

uint16_t adc_read(ADC_TypeDef* ADCx, uint8_t channel, uint8_t ADC_SampleTime) {
 /* Configure Channel */
 ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime);
 
 /* check if conversion was started, if not start */
 ADC_SoftwareStartConv(ADCx);
 
 /* wait for end of conversion */
 while((ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET));
 
 return ADC_GetConversionValue(ADCx);
}

//void ADC1_IRQHandler(void)
//{
//  if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
//  {
//    /* Get converted value */
//    ADCVal = ADC_GetConversionValue(ADC1);
//    /* Output converted value on DAC_OUT1 */
//    DAC_SetChannel1Data(DAC_Align_12b_R, ADCVal);
//    /* Clear EOC Flag */
//    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
//  }
//}







  void TimingDelay_Decrement(void)
  {
   if( TimingDelay != 0x00 ) {
    TimingDelay--;
  }
}

void Delay(__IO uint32_t nTime)
{
 TimingDelay = nTime;

 while(TimingDelay != 0)
  ;
}




