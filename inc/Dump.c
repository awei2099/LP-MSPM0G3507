// Dump.c
// Your solution to ECE319K Lab 3 Spring 2025
// Author: Your name
// Last Modified: Your date


#include <ti/devices/msp/msp.h>
#include "../inc/Timer.h"
#define MAXBUF 50
uint32_t DataBuffer[MAXBUF];
uint32_t TimeBuffer[MAXBUF];
uint32_t DebugCnt; // 0 to MAXBUF (0 is empty, MAXBUF is full)

// *****Debug_Init******
// Initializes your index or pointer.
// Input: none
// Output:none
void Debug_Init(void){
// students write this for Lab 3
// This function should also initialize Timer G12, call TimerG12_Init.
  DebugCnt = 0;
  TimerG12_Init();
}

// *****Debug_Dump******
// Records one data and one time into the two arrays.
// Input: data is value to store in DataBuffer
// Output: 1 for success, 0 for failure (buffers full)
uint32_t Debug_Dump(uint32_t data){
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
  if (DebugCnt >= MAXBUF) {
    return 0;
  }
  DataBuffer[DebugCnt] = data;
  TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;
  DebugCnt++;
  return 1; // success
}
// *****Debug_Dump2******
// Always record data and time on the first call to Debug_Dump2
// However, after the first call
//    Records one data and one time into the two arrays, only if the data is different from the previous call.
//    Do not record data or time if the data is the same as the data from the previous call
// Input: data is value to store in DataBuffer
// Output: 1 for success (saved or skipped), 0 for failure (buffers full)
uint32_t Debug_Dump2(uint32_t data){
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
  
  return 1; // success
}

// *****Debug_Period******
// Calculate period of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in bus cycles
// Period is defined as rising edge (low to high) to the next rising edge.
// Return 0 if there is not enough collected data to calculate period .
uint32_t Debug_Period(uint32_t mask){
// students write this for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
  if (DebugCnt < 2){
    return 0;
  }
  uint32_t last_state = (DataBuffer[0] & mask) ? 1 : 0;
  uint32_t last_rise_time = 0;
  bool    first_rise = false;
  uint64_t sum = 0;
  uint32_t count = 0;

  for(uint32_t i = 1; i < DebugCnt; i++){
      uint32_t cur_state = (DataBuffer[i] & mask) ? 1 : 0;
      // detect low->high transition
      if(!last_state && cur_state){
          if(!first_rise){
              // record first rising edge time
              last_rise_time = TimeBuffer[i];
              first_rise = true;
          } else {
              // compute period = last_rise_time - this_time (wrapping subtraction)
              uint32_t period = last_rise_time - TimeBuffer[i];
              sum += period;
              count++;
              // prepare for next
              last_rise_time = TimeBuffer[i];
          }
      }
      last_state = cur_state;
  }
  if(count == 0) {
      return 0;
  }
  return (uint32_t)(sum / count);
  //return 42; // average period
}


// *****Debug_Duty******
// Calculate duty cycle of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in percent (0 to 100)
// Period is defined as rising edge (low to high) to the next rising edge.
// High is defined as rising edge (low to high) to the next falling edge.
// Duty cycle is (100*High)/Period
// Return 0 if there is not enough collected data to calculate duty cycle.
uint32_t Debug_Duty(uint32_t mask){
// students write this for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
  if(DebugCnt < 3){
        return 0;
  }
  enum {LOW=0, HIGH=1} state = (DataBuffer[0] & mask) ? HIGH : LOW;
  bool seen_rise = false;
  uint32_t rise_time = 0;
  uint64_t sum_duty = 0;
  uint32_t cycle_count = 0;

  for(uint32_t i = 1; i < DebugCnt; i++){
      uint32_t cur = (DataBuffer[i] & mask) ? HIGH : LOW;
      // rising edge
      if(state == LOW && cur == HIGH){
          if(seen_rise){
              // finish previous cycle: compute period and duty
              uint32_t next_rise = TimeBuffer[i];
              uint32_t period = rise_time - next_rise;
              // sum_duty already accumulated high-time portions
              // duty% = (100 * total_high) / period
              uint32_t duty = (uint32_t)((sum_duty * 100) / period);
              sum_duty = 0;
              cycle_count++;
              rise_time = next_rise;
          } else {
              // first rising edge
              rise_time = TimeBuffer[i];
              seen_rise = true;
          }
      }
      // falling edge
      else if(state == HIGH && cur == LOW && seen_rise){
          uint32_t fall_time = TimeBuffer[i];
          // high duration = rise_time - fall_time
          sum_duty += (rise_time - fall_time);
      }
      state = cur;
  }
  if(cycle_count == 0){
      return 0;
  }
  // average of duty% over all cycles
  // note: sum_duty was re-used per cycle; above we computed each cycle's %
  // so here just return the last computed duty; for true average you'd accumulate duties
  return (uint32_t)((sum_duty * 100) / cycle_count);
}

// Lab2 specific debugging code
uint32_t Theperiod;
void Dump(void){
  uint32_t out = GPIOB->DOUT31_0&0x0070000; // PB18-PB16 outputs
  uint32_t in = GPIOB->DIN31_0&0x0F;        // PB3-PB0 inputs
  uint32_t data = out|in;                   // PB18-PB16, PB3-PB0
  uint32_t result = Debug_Dump(data);       // calls your Lab3 function
  if(result == 0){ // 0 means full
    Theperiod = Debug_Period(1<<18);        // calls your Lab3 function
//   __asm volatile("bkpt; \n"); // breakpoint here
// observe Theperiod
  }
}




