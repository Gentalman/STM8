/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


typedef struct {
  GPIO_TypeDef * GPIOx;
  GPIO_Pin_TypeDef GPIO_Pin; 
} Port;

#define CLK_High()  GPIO_WriteHigh(GPIOB, GPIO_PIN_3)
#define CLK_Low()  GPIO_WriteLow(GPIOB, GPIO_PIN_3)

#define LE_High()  GPIO_WriteHigh(GPIOB, GPIO_PIN_4)
#define LE_Low()  GPIO_WriteLow(GPIOB, GPIO_PIN_4)

#define OE_High()  GPIO_WriteHigh(GPIOB, GPIO_PIN_5)
#define OE_Low()  GPIO_WriteLow(GPIOB, GPIO_PIN_5)

void Delay(uint16_t ncount);

void JXI5020_Init();

void main(void)
{ 
  int i, j;
  
  JXI5020_Init();
  
  GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST); //A0
  GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);  //A1
  GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST); //A2
  
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_SLOW);  //1
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);  //2
  GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);  //3  
//  GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);  //4
//  GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);  //5
//  GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);  //6
  
    uint32_t fir_line_data;
    uint32_t sec_line_data;
    uint16_t color = 0x0001;
    
    for (j = 16; j > 0; j--)
    { 
      OE_High();
      
      if (color & 0x01) 
      {
        fir_line_data = 0x08000000;
        sec_line_data = 0x00000000;
      }
      else
      {
        fir_line_data = 0x00000000;
        sec_line_data = 0x00000000;
      }
      
      color >>= 1;
      
      for (i = 0; i < 32; i++) 
      {   
        CLK_Low();
        if (sec_line_data & 0x01) 
        {
          GPIO_WriteHigh(GPIOD, GPIO_PIN_3);
        }
        else
        {
          GPIO_WriteLow(GPIOD, GPIO_PIN_3);
        }
        
        CLK_High();
        
        sec_line_data >>= 1;
        
        Delay(0x000F);
      } 
      
      for (i = 0; i < 32; i++) 
      {   
        CLK_Low();
        if (fir_line_data & 0x01) 
        {
          GPIO_WriteHigh(GPIOD, GPIO_PIN_3);
        }
        else
        {
          GPIO_WriteLow(GPIOD, GPIO_PIN_3);
        }
        
        CLK_High();
        
        fir_line_data >>= 1;
        
        Delay(0x000F);
      } 
      
      LE_High();
      LE_Low();  
      OE_Low();
    }
    
    
    while (1) {
      CLK_High();
      Delay(0x00FF);     
      CLK_Low();
    }

}

/**
  * @brief Delay
  * @param ncount
  * @retval None
  */
void Delay(uint16_t ncount)
{
  /* Decrement nCount value */
  while (ncount != 0)
  {
    ncount--;
  }
}

void JXI5020_Init()
{
  GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);  //CLK
  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);  //LE
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST); //OE    
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
