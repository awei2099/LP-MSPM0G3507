/* Key.c
 * Students put your names here
 * Modified: October 2, 2023
 * 4-bit keyboard connected to PB19-PB16
 * Piano key 3 PB19 (note with highest frequency)
 * Piano key 2 PB18
 * Piano key 1 PB17
 * Piano key 0 PB16 (note with lowest frequency)
 *
 */
#include <ti/devices/msp/msp.h>
#define PB16INDEX 32 // UART2_RX  SPI1_SCK  UART3_RTS TIMG8_C1  TIMG7_C1
#define PB17INDEX 42 // UART2_TX  SPI0_PICO SPI1_CS1  TIMA1_C0  TIMA0_C2
#define PB18INDEX 43 // UART2_RX  SPI0_SCK  SPI1_CS2  TIMA1_C1  TIMA0_C2N
#define PB19INDEX 44 // COMP2_OUT SPI0_POCI TIMG8_C1  UART0_CTS TIMG7_C1
#include "../inc/LaunchPad.h"

// **************Key_Init*********************
// Initialize piano key inputs on PB19-PB16
// Input: none
// Output: none
void Key_Init(void){
// Assumes LaunchPad_Init has been called
// I.e., PortB has already been reset and activated (do not reset PortB here again)
    // regular inputs
//  IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081; // regular GPIO input
//  IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00040081; // regular GPIO input
//  IOMUX->SECCFG.PINCM[PB18INDEX] = 0x00040081; // regular GPIO input
//  IOMUX->SECCFG.PINCM[PB19INDEX] = 0x00040081; // regular GPIO input
  // PCB camp board
  IOMUX->SECCFG.PINCM[PA28INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081; // input, no pull
  // red board
  IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA15INDEX] = 0x00040081; // input, no pull
  // green board
//  IOMUX->SECCFG.PINCM[PA17INDEX] = 0x00040081; // input, no pull
//  IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00040081; // input, no pull
}

// **************Key_In*********************
// Input from piano key inputs on PB19-PB16
// Input: none
// Output: 0 to 15 depending on keys
//   0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
uint32_t Key_In(void){
    // regular Lab5
 // return (GPIOB->DIN31_0>>16)&0x000F;
    uint32_t data = GPIOA->DIN31_0;

    // red board
    return ((data>>15)&0x03) | ((data&((1<<28)|(1<<27)))>>25);
    // green board
   // return ((data>>16)&0x03) | ((data&((1<<28)|(1<<27)))>>25);

}
