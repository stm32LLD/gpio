// Copyright (c) 2023 Ziga Miklosic
// All Rights Reserved
////////////////////////////////////////////////////////////////////////////////
/**
*@file      gpio.c
*@brief     GPIO LL Driver based on STM32 HAL library
*@author    Ziga Miklosic
*@email     ziga.miklosic@gmail.si
*@date      21.04.2023
*@version   V0.1.0
*/
////////////////////////////////////////////////////////////////////////////////
/*!
* @addtogroup GPIO
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * Initialization guards
 */
static bool gb_is_init = false;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/*
 *  EXTI ISR ON STM32L4/L4P/H7:
 *
 *      - EXTI0_IRQHandler
 *      - EXTI1_IRQHandler
 *      - EXTI2_IRQHandler
 *      - EXTI3_IRQHandler
 *      - EXTI4_IRQHandler
 *      - EXTI9_5_IRQHandler
 *      - EXTI15_10_IRQHandler
 *
 *
 *  EXTI ISR ON STM32G0:
 *
 *      - EXTI0_1_IRQn
 *      - EXTI2_3_IRQn
 *      - EXTI4_15_IRQn
 *
 */


////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup GPIO_API
* @{ <!-- BEGIN GROUP -->
*
* 	Following function are part of GPIO API.
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*       GPIO initialization
*
* @return       status - Status of operation
*/
////////////////////////////////////////////////////////////////////////////////
gpio_status_t gpio_init(void)
{
    gpio_status_t       status      = eGPIO_OK;
    GPIO_InitTypeDef    gpio_init   = {0};

    // Is init
    if ( false == gb_is_init )
    {
        // Init all GPIOs
        for ( uint32_t pin = 0; pin < eGPIO_PIN_NUM_OF; pin++ )
        {
            // Get pin configuration
            const gpio_cfg_t * pin_cfg = gpio_cfg_get_cfg( pin );

            // Enable clock
            gpio_enable_port_clock( pin_cfg->p_port );

            // Prepare init data
            gpio_init.Mode      = pin_cfg->mode;
            gpio_init.Pin       = pin_cfg->pin;
            gpio_init.Speed     = pin_cfg->speed;

            // Set output state
            if  (   ( GPIO_MODE_OUTPUT_PP == pin_cfg->mode )
                ||  ( GPIO_MODE_OUTPUT_OD == pin_cfg->mode ))
            {
                if ( eGPIO_HIGH == pin_cfg->init_state )
                {
                    HAL_GPIO_WritePin((GPIO_TypeDef*) pin_cfg->p_port, pin_cfg->pin, GPIO_PIN_SET );
                }
                else if ( eGPIO_LOW == pin_cfg->init_state  )
                {
                    HAL_GPIO_WritePin((GPIO_TypeDef*) pin_cfg->p_port, pin_cfg->pin, GPIO_PIN_RESET );
                }
                else
                {
                    // No actions...
                }
            }

            // Initialize
            HAL_GPIO_Init((GPIO_TypeDef*) pin_cfg->p_port, &gpio_init );
        }

        // Alles gut - init succeed
        if ( eGPIO_OK == status )
        {
            gb_is_init = true;
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
*       GPIO de-initialization
*
* @return       status - Status of operation
*/
////////////////////////////////////////////////////////////////////////////////
gpio_status_t gpio_deinit(void)
{
    gpio_status_t status = eGPIO_OK;

    if ( true == gb_is_init )
    {
        // Init all GPIOs
        for ( uint32_t pin = 0; pin < eGPIO_PIN_NUM_OF; pin++ )
        {
            // Get pin configuration
            const gpio_cfg_t * pin_cfg = gpio_cfg_get_cfg( pin );

            // De-Initialise
            HAL_GPIO_DeInit((GPIO_TypeDef*) pin_cfg->p_port, pin_cfg->pin );
        }

        // De-init success
        gb_is_init = false;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
*       Get GPIO init flag
*
* @param[out]   p_is_init   - Pointer to GPIO init flag
* @return       state       - Current state of pin
*/
////////////////////////////////////////////////////////////////////////////////
gpio_status_t gpio_is_init(bool * const p_is_init)
{
    gpio_status_t status = eGPIO_OK;

    if ( NULL != p_is_init )
    {
        *p_is_init = gb_is_init;
    }
    else
    {
        status = eGPIO_ERROR;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
*       Get GPIO state
*
* @param[in]    pin     - GPIO pin
* @param[out]   p_state - GPIO pin state
* @return       state   - Current state of pin
*/
////////////////////////////////////////////////////////////////////////////////
gpio_status_t gpio_get(const gpio_pin_t pin, gpio_state_t * const p_state)
{
    gpio_status_t status = eGPIO_OK;

    GPIO_ASSERT( true == gb_is_init );
    GPIO_ASSERT( pin < eGPIO_PIN_NUM_OF );
    GPIO_ASSERT( NULL != p_state );

    if  (   ( true == gb_is_init )
        &&  ( pin < eGPIO_PIN_NUM_OF )
        &&  ( NULL != p_state ))
    {
        // Get pin configuration
        const gpio_cfg_t * pin_cfg = gpio_cfg_get_cfg( pin );

        // Read pin
        if ( GPIO_PIN_SET == HAL_GPIO_ReadPin((GPIO_TypeDef*) pin_cfg->p_port, pin_cfg->pin ))
        {
            *p_state = eGPIO_HIGH;
        }
        else
        {
            *p_state = eGPIO_LOW;
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
*       Set GPIO state
*
* @param[in]    pin     - GPIO pin
* @param[in]    state   - Current state of pin
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
gpio_status_t gpio_set(const gpio_pin_t pin, const gpio_state_t state)
{
    gpio_status_t status = eGPIO_OK;

    GPIO_ASSERT( true == gb_is_init );
    GPIO_ASSERT( pin < eGPIO_PIN_NUM_OF );

    if  (   ( true == gb_is_init )
        &&  ( pin < eGPIO_PIN_NUM_OF ))
    {
        // Get pin configuration
        const gpio_cfg_t * pin_cfg = gpio_cfg_get_cfg( pin );

        // Write pin
        if ( eGPIO_HIGH == state )
        {
            HAL_GPIO_WritePin((GPIO_TypeDef*) pin_cfg->p_port, pin_cfg->pin, GPIO_PIN_SET );
        }
        else if ( eGPIO_LOW == state )
        {
            HAL_GPIO_WritePin((GPIO_TypeDef*) pin_cfg->p_port, pin_cfg->pin, GPIO_PIN_RESET );
        }
        else
        {
            // No actions...
        }
    }
    else
    {
        status = eGPIO_ERROR;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
*       Toggle GPIO state
*
* @param[in]    pin     - GPIO pin
* @return       status  - Status of operation
*/
////////////////////////////////////////////////////////////////////////////////
gpio_status_t gpio_toggle(const gpio_pin_t pin)
{
    gpio_status_t status = eGPIO_OK;

    GPIO_ASSERT( true == gb_is_init );
    GPIO_ASSERT( pin < eGPIO_PIN_NUM_OF );

    if  (   ( true == gb_is_init )
        &&  ( pin < eGPIO_PIN_NUM_OF ))
    {
        // Get pin configuration
        const gpio_cfg_t * pin_cfg = gpio_cfg_get_cfg( pin );

        // Toggle pin
        HAL_GPIO_TogglePin((GPIO_TypeDef*) pin_cfg->p_port, pin_cfg->pin );
    }
    else
    {
        status = eGPIO_ERROR;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Enable GPIO clock
*
* @param[in]    p_port  - Pointer to GPIO address
* @return       status  - Status of operation
*/
////////////////////////////////////////////////////////////////////////////////
gpio_status_t gpio_enable_port_clock(const GPIO_TypeDef * p_port)
{
    gpio_status_t status = eGPIO_OK;

    GPIO_ASSERT( NULL != p_port );

    if ( NULL != p_port )
    {
        if ( GPIOA == p_port )
        {
            __HAL_RCC_GPIOA_CLK_ENABLE();
        }

    #if defined(GPIOB)
        else if ( GPIOB == p_port )
        {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOC)
        else if ( GPIOC == p_port )
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOD)
        else if ( GPIOD == p_port )
        {
            __HAL_RCC_GPIOD_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOE)
        else if ( GPIOE == p_port )
        {
            __HAL_RCC_GPIOE_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOF)
        else if ( GPIOF == p_port )
        {
            __HAL_RCC_GPIOF_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOG)
        else if ( GPIOG == p_port )
        {
            __HAL_RCC_GPIOG_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOH)
        else if ( GPIOH == p_port )
        {
            __HAL_RCC_GPIOH_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOI)
        else if ( GPIOI == p_port )
        {
            __HAL_RCC_GPIOI_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOJ)
        else if ( GPIOJ == p_port )
        {
            __HAL_RCC_GPIOJ_CLK_ENABLE();
        }
    #endif

    #if defined(GPIOK)
        else if ( GPIOK == p_port )
        {
            __HAL_RCC_GPIOK_CLK_ENABLE();
        }
    #endif

        // None of the clock has been enabled --> report error
        else
        {
            status = eGPIO_ERROR;
        }
    }
    else
    {
        status = eGPIO_ERROR;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Disable GPIO clock
*
* @param[in]    p_port  - Pointer to GPIO address
* @return       status  - Status of operation
*/
////////////////////////////////////////////////////////////////////////////////
gpio_status_t gpio_disable_port_clock(const GPIO_TypeDef * p_port)
{
    gpio_status_t status = eGPIO_OK;

    GPIO_ASSERT( NULL != p_port );

    if ( NULL != p_port )
    {
        if ( GPIOA == p_port )
        {
            __HAL_RCC_GPIOA_CLK_DISABLE();
        }

    #if defined(GPIOB)
        else if ( GPIOB == p_port )
        {
            __HAL_RCC_GPIOB_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOC)
        else if ( GPIOC == p_port )
        {
            __HAL_RCC_GPIOC_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOD)
        else if ( GPIOD == p_port )
        {
            __HAL_RCC_GPIOD_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOE)
        else if ( GPIOE == p_port )
        {
            __HAL_RCC_GPIOE_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOF)
        else if ( GPIOF == p_port )
        {
            __HAL_RCC_GPIOF_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOG)
        else if ( GPIOG == p_port )
        {
            __HAL_RCC_GPIOG_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOH)
        else if ( GPIOH == p_port )
        {
            __HAL_RCC_GPIOH_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOI)
        else if ( GPIOI == p_port )
        {
            __HAL_RCC_GPIOI_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOJ)
        else if ( GPIOJ == p_port )
        {
            __HAL_RCC_GPIOJ_CLK_DISABLE();
        }
    #endif

    #if defined(GPIOK)
        else if ( GPIOK == p_port )
        {
            __HAL_RCC_GPIOK_CLK_DISABLE();
        }
    #endif

        // None of the clock has been disabled --> report error
        else
        {
            status = eGPIO_ERROR;
        }
    }
    else
    {
        status = eGPIO_ERROR;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
