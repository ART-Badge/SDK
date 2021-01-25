/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    app_define.h
  * @brief
  * @author
  * @date
  * @version
  ***************************************************************************************
    * @attention
  * <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
  ***************************************************************************************
  */

#ifndef APP_DEFINE_H
#define APP_DEFINE_H

/** @defgroup APP_DEFINE    APP Define
  * @{
  */

/*============================================================================*
  *                                   Macros
  *============================================================================*/
/** @defgroup APP_DEFINE_Exported_Macros App Define Exported Macros
    * @brief
    * @{
    */
#ifndef UNUSED
#define UNUSED(expr) do { (void)(expr); } while (0)
#endif

//#pragma diag_suppress 174  // expression has no effect
//#pragma diag_suppress 223  // function declared implicitly

///////////////////////////////////////////////////////////////////
//#define  SUPPORT_ALONE_UPPERSTACK_IMG
#define SPIFLASH0IntrHandler SPI_Flash0_Handler
#define SPIFLASH1IntrHandler SPI_Flash1_Handler
#define Gpio2IntrHandler  GPIO2_Handler
#define Gpio3IntrHandler  GPIO3_Handler
#define Gpio4IntrHandler  GPIO4_Handler
#define Gpio5IntrHandler  GPIO5_Handler
#define Timer3IntrHandler Timer3_Handler
#define Timer4IntrHandler Timer4_Handler
#define Timer5IntrHandler Timer5_Handler
#define Timer6IntrHandler Timer6_Handler
#define Timer7IntrHandler Timer7_Handler
#define Gpio6IntrHandler  GPIO6_Handler
#define Gpio7IntrHandler  GPIO7_Handler
#define Gpio8IntrHandler  GPIO8_Handler
#define Gpio9IntrHandler  GPIO9_Handler
#define Gpio10IntrHandler GPIO10_Handler
#define Gpio11IntrHandler GPIO11_Handler
#define Gpio12IntrHandler GPIO12_Handler
#define Gpio13IntrHandler GPIO13_Handler
#define Gpio14IntrHandler GPIO14_Handler
#define Gpio15IntrHandler GPIO15_Handler
#define Gpio16IntrHandler GPIO16_Handler
#define Gpio17IntrHandler GPIO17_Handler
#define Gpio18IntrHandler GPIO18_Handler
#define Gpio19IntrHandler GPIO19_Handler
#define Gpio20IntrHandler GPIO20_Handler
#define Gpio21IntrHandler GPIO21_Handler
#define Gpio22IntrHandler GPIO22_Handler
#define Gpio23IntrHandler GPIO23_Handler
#define Gpio24IntrHandler GPIO24_Handler
#define Gpio25IntrHandler GPIO25_Handler
#define Gpio26IntrHandler GPIO26_Handler
#define Gpio27IntrHandler GPIO27_Handler
#define Gpio28IntrHandler GPIO28_Handler
#define Gpio29IntrHandler GPIO29_Handler
#define Gpio30IntrHandler GPIO30_Handler
#define Gpio31IntrHandler GPIO31_Handler
/** End of APP_DEFINE_Exported_Macros
    * @}
    */

/** @} */ /* End of group APP_DEFINE */

#endif // APP_DEFINE_H
