/*!
 *  @file   CTS_HAL.h
 *
 *  @brief       
 *
 *  @par    Project:
 *             MSP430 Capacitive Touch Library 
 *
 *  @par    Developed using:
 *             CCS Version : 5.2.1.00018, w/support for GCC extensions (--gcc)
 *  \n         IAR Version : 5.51.2 [Kickstart]
 *
 *  @author    C. Sterzik
 *  @author    T. Hwang
 *
 *  @version     1.2 Added HALs for new devices.
 *
 *  @par    Supported Hardware Configurations:
 *              - TI_CTS_RO_COMPAp_TA0_WDTp_HAL()
 *              - TI_CTS_fRO_COMPAp_TA0_SW_HAL()
 *              - TI_CTS_fRO_COMPAp_SW_TA0_HAL()
 *              - TI_CTS_RO_COMPAp_TA1_WDTp_HAL()
 *              - TI_CTS_fRO_COMPAp_TA1_SW_HAL()
 *              - TI_CTS_RC_PAIR_TA0_HAL()
 *              - TI_CTS_RO_PINOSC_TA0_WDTp_HAL()
 *              - TI_CTS_RO_PINOSC_TA0_HAL()
 *              - TI_CTS_fRO_PINOSC_TA0_SW_HAL()
 *              - TI_CTS_RO_COMPB_TA0_WDTA_HAL()
 *              - TI_CTS_RO_COMPB_TA1_WDTA_HAL()
 *              - TI_CTS_fRO_COMPB_TA0_SW_HAL()
 *              - TI_CTS_fRO_COMPB_TA1_SW_HAL()
 *           /n  (1.1)
 *              - TI_CTS_fRO_PINOSC_TA0_TA1_HAL()
 *              - TI_CTS_RO_PINOSC_TA0_TA1_HAL()
 *              - TI_CTS_RO_CSIO_TA2_WDTA_HAL()
 *              - TI_CTS_RO_CSIO_TA2_TA3_HAL()
 *              - TI_CTS_fRO_CSIO_TA2_TA3_HAL()
 *              - TI_CTS_RO_COMPB_TB0_WDTA_HAL()
 *              - TI_CTS_RO_COMPB_TA1_TA0_HAL()
 *              - TI_CTS_fRO_COMPB_TA1_TA0_HAL()
 *           /n  (1.2)
 *              - TI_CTS_RO_PINOSC_TA1_WDTp_HAL()
 *              - TI_CTS_RO_PINOSC_TA1_TB0_HAL()
 *              - TI_CTS_fRO_PINOSC_TA1_TA0_HAL()
 *              - TI_CTS_fRO_PINOSC_TA1_TB0_HAL()
 *
 */

#ifndef CTS_HAL_H_
#define CTS_HAL_H_

#include "structure.h"

void TI_CTS_RO_CSIO_TA2_WDTA_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_RO_CSIO_TA2_TA3_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_fRO_CSIO_TA2_TA3_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_RO_COMPB_TB0_WDTA_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_RO_PINOSC_TA0_TA1_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_fRO_PINOSC_TA0_TA1_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_RO_COMPAp_TA0_WDTp_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_COMPAp_TA0_SW_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_COMPAp_SW_TA0_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_RO_COMPAp_TA1_WDTp_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_COMPAp_TA1_SW_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_RC_PAIR_TA0_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_RO_PINOSC_TA0_WDTp_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_RO_PINOSC_TA0_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_PINOSC_TA0_SW_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_RO_COMPB_TA0_WDTA_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_COMPB_TA0_SW_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_RO_COMPB_TA1_WDTA_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_COMPB_TA1_SW_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_RO_COMPB_TA1_TA0_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_COMPB_TA1_TA0_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_RO_PINOSC_TA1_WDTp_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_RO_PINOSC_TA1_TB0_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_PINOSC_TA1_TA0_HAL(const struct Sensor *, uint16_t *);

void TI_CTS_fRO_PINOSC_TA1_TB0_HAL(const struct Sensor *, uint16_t *);


#endif /* CTS_HAL_H_ */
