/*!
 *  @file   CTS_Layer.h
 *
 *  @brief       
 *
 *  @par    Project:
 *             MSP430 Capacitive Touch Library 
 *
 *  @par    Developed using:
 *             CCS Version : 5.2.1.00018, w/support for GCC extensions (--gcc)
 *  \n         IAR Version : 5.51.2
 *
 *  @author    C. Sterzik
 *  @author    T. Hwang
 *  @version     1.2 
 *                 Updated HALs.
 *
 *  @par    Supported API Calls:
 *              - TI_CAPT_Init_Baseline()
 *              - TI_CAPT_Update_Baseline()
 *              - TI_CAPT_Reset_Tracking()
 *              - TI_CAPT_Update_Tracking_DOI()
 *              - TI_CAPT_Update_Tracking_Rate()
 *              - TI_CAPT_Update_Baseline()
 *              - TI_CAPT_Raw()
 *              - TI_CAPT_Custom()
 *              - TI_CAPT_Button()
 *              - TI_CAPT_Buttons()
 *              - TI_CAPT_Slider()
 *              - TI_CAPT_Wheel()
 */

#ifndef CTS_LAYER_H_
#define CTS_LAYER_H_

#include "CTS_HAL.h"

//! \name Status Register Definitions
//! @{
//
//! (bit 0): Event Flag: this indicates that a threshold crossing occured
#define EVNT            0x01 
//! (bit 1): Direction of Interest: This indicates if the measurement is looking
//! for an increasing (set) or decreasing (clr) capacitance.
#define DOI_MASK        0x02 
#define DOI_INC         0x02  
#define DOI_DEC         0x00  
//! (bit 2): Past Event Flag: this indicates that a prior element within the 
//! sensor group has detected a threshold crossing
#define PAST_EVNT       0x04  
//! (bits 4-5): Tracking Rate in Direction of Interest: this indicates at what  
//! rate the baseline will adjust to the current measurement when the 
//! when the measurement is changing in the direction of interst but does not
//! result in a threshold crossing:
//!     \n Very Slow
//!     \n Slow
//!     \n Medium
//!     \n Fast
#define TRIDOI_VSLOW    0x00   
#define TRIDOI_SLOW     0x10   
#define TRIDOI_MED      0x20  
#define TRIDOI_FAST     0x30  
//! (bits 5-6): Tracking Rate Against Direction of Interest: this indicates at   
//! what rate the baseline will adjust to the current measurement when the 
//! when the measurement is changing against the direction of interst:
//!     \n Fast
//!     \n Medium
//!     \n Slow
//!     \n Very Slow
#define TRADOI_FAST     0x00   
#define TRADOI_MED      0x40   
#define TRADOI_SLOW     0x80  
#define TRADOI_VSLOW    0xC0  

//! @}


// API Calls
void TI_CAPT_Init_Baseline(const struct Sensor*);
void TI_CAPT_Update_Baseline(const struct Sensor*, uint8_t);

void TI_CAPT_Reset_Tracking(void);
void TI_CAPT_Update_Tracking_DOI(uint8_t);
void TI_CAPT_Update_Tracking_Rate(uint8_t);

void TI_CAPT_Raw(const struct Sensor*, uint16_t*);

void TI_CAPT_Custom(const struct Sensor *, uint16_t*);

uint8_t TI_CAPT_Button(const struct Sensor *);
const struct Element * TI_CAPT_Buttons(const struct Sensor *);
uint16_t TI_CAPT_Slider(const struct Sensor*);
uint16_t TI_CAPT_Wheel(const struct Sensor*);

// Internal Calls
uint8_t Dominant_Element (const struct Sensor*, uint16_t*);

#endif /* CTS_LAYER_H_ */
