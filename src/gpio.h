// Copyright (c) 2023 Ziga Miklosic
// All Rights Reserved
////////////////////////////////////////////////////////////////////////////////
/**
*@file      gpio.h
*@brief     GPIO LL Driver based on STM32 HAL library
*@author    Ziga Miklosic
*@email		ziga.miklosic@gmail.si
*@date      21.04.2023
*@version   V0.1.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup GPIO_API
* @{ <!-- BEGIN GROUP -->
*
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __GPIO_H
#define __GPIO_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../gpio_cfg.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 *  Module version
 */
#define GPIO_VER_MAJOR          ( 0 )
#define GPIO_VER_MINOR          ( 1 )
#define GPIO_VER_DEVELOP        ( 0 )

/**
 *  GPIO status
 */
typedef enum
{
    eGPIO_OK        = 0x00U,    /**<Normal operation */
    eGPIO_ERROR     = 0x01U,    /**<General error code */
} gpio_status_t;

/**
 *  GPIO state
 */
typedef enum
{
    eGPIO_LOW = 0,      /**<Logic low state */
    eGPIO_HIGH,         /**<Logic high state */
    eGPIO_UKNOWN,       /**<Uknown state - during startup time */
} gpio_state_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
gpio_status_t   gpio_init                   (void);
gpio_status_t   gpio_deinit                 (void);
gpio_status_t   gpio_is_init                (bool * const p_is_init);
gpio_status_t   gpio_get                    (const gpio_pin_t pin, gpio_state_t * const p_state);
gpio_status_t   gpio_set                    (const gpio_pin_t pin, const gpio_state_t state);
gpio_status_t   gpio_toggle                 (const gpio_pin_t pin);
gpio_status_t   gpio_enable_port_clock      (const GPIO_TypeDef * p_port);
gpio_status_t   gpio_disable_port_clock     (const GPIO_TypeDef * p_port);

#endif // __GPIO_H

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
