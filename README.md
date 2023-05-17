# **STM32 GPIO Low Level Driver**
Following repository contains STM32 GPIO Low Level (LL) driver based on STM32 HAL library. 

STM32 GPIO LL driver is supporting following STM32 device family:
- STM32G0:      Has ports from GPIOA-GPIOF 
- STM32L4/L4+:  Has ports from GPIOA-GPIOI
- STM32H7:      Has ports from GPIOA-GPIOK


## **Dependencies**

### **1. STM32 HAL library**
STM32 GPIO LL driver module uses STM32 HAL library.


## **API**
| API Functions | Description | Prototype |
| --- | ----------- | ----- |
| **gpio_init** | Initialization of GPIO module | gpio_status_t gpio_init(void) |
| **gpio_deinit** | De-initialization of GPIO module | gpio_status_t gpio_deinit(void) |
| **gpio_is_init** | Get initialization state of GPIO module | gpio_status_t gpio_is_init(bool * const p_is_init) |
| **gpio_get** | Get pin state | gpio_status_t gpio_get(const gpio_pin_t pin, gpio_state_t * const p_state) |
| **gpio_set** | Set pin state | gpio_status_t gpio_set(const gpio_pin_t pin, const gpio_state_t state) |
| **gpio_toggle** | Toggle pin state | gpio_status_t gpio_toggle(const gpio_pin_t pin) |
| **gpio_enable_port_clock** | Enable GPIO port clock | gpio_status_t gpio_enable_port_clock(const GPIO_TypeDef * p_port) |
| **gpio_disable_port_clock** | Disable GPIO port clock | gpio_status_t gpio_disable_port_clock(const GPIO_TypeDef * p_port) |


## **Usage**

**GENERAL NOTICE: Put all user code between sections: USER CODE BEGIN & USER CODE END!**

**1. Copy template files to root directory of the module**

Copy configuration file *gpio_cfg* to root directory and replace file extension (.htmp/.ctmp -> .h/.c).

**2. Change default HAL library include to target microprocessor inside ***gpio_cfg.h***:**

Following example shows HAL library include for STM32L4 family:
```C
// USER INCLUDE BEGIN...

#include "stm32l4xx_hal.h"

// USER INCLUDE END...
```

**3. Configure GPIO module for application needs by changing ***gpio_cfg.h***. Configuration options are following:**

| Configuration | Description |
| --- | --- |
| **GPIO_CFG_ASSERT_EN** 		        | Enable/Disable assertions |
| **GPIO_ASSERT** 		                | Assert definition |

**4. Define needed GPIO pins inside ***gpio_cfg.h*****:
```C
/**
 *  GPIO pins
 *
 *  @note   Must start with enumeration of 0!
 */
typedef enum
{
    // USER CODE BEGIN...

    eGPIO_KEY_1 = 0,    /**<User button 1 */
    eGPIO_KEY_2,        /**<User button 2 */
    eGPIO_KEY_3,        /**<User button 3 */
    eGPIO_KEY_4,        /**<User button 4 */

    eGPIO_BLE_STAT,     /**<BLE module status */
    eGPIO_BLE_RST,      /**<BLE module reset */

    eGPIO_WIFI_STAT,    /**<WIFI module status */
    eGPIO_WIFI_RST,     /**<WIFI module reset */

    eGPIO_LCD_CD,       /**<LCD Command/Data pin */
    eGPIO_LCD_CS,       /**<LCD Chip Select pin */
    eGPIO_LCD_RST,      /**<LCD Reset pin */
    eGPIO_LCD_LED,      /**<LCD Backlight */


    // Add more here...


    // USER CODE END...

    eGPIO_PIN_NUM_OF    /**<Number of all GPIO pins*/
} gpio_pin_t;
```

**5. Configure GPIO pins inside ***gpio_cfg.c***:**
```C
/**
 *  GPIO Pin Configuration
 */
static const gpio_cfg_t g_gpio_cfg[eGPIO_PIN_NUM_OF] =
{
    // USER CODE BEGIN...

    // ===========================================================================================================================================================================
    //          INPUT pins
    //
    //  @note   Do not care for ".init_state" field in input mode!
    // ============================================================================================================================================================================
    [eGPIO_KEY_1]       = { .p_port = KEY1__PORT ,      .pin = KEY1__PIN,       .mode = GPIO_MODE_INPUT, .pull = KEY1__PULL,        .speed = GPIO_SPEED_MEDIUM, .init_state = 0U },
    [eGPIO_KEY_2]       = { .p_port = KEY2__PORT ,      .pin = KEY2__PIN,       .mode = GPIO_MODE_INPUT, .pull = KEY2__PULL,        .speed = GPIO_SPEED_MEDIUM, .init_state = 0U },
    [eGPIO_KEY_3]       = { .p_port = KEY3__PORT ,      .pin = KEY3__PIN,       .mode = GPIO_MODE_INPUT, .pull = KEY3__PULL,        .speed = GPIO_SPEED_MEDIUM, .init_state = 0U },
    [eGPIO_KEY_4]       = { .p_port = KEY4__PORT ,      .pin = KEY4__PIN,       .mode = GPIO_MODE_INPUT, .pull = KEY4__PULL,        .speed = GPIO_SPEED_MEDIUM, .init_state = 0U },

    [eGPIO_BLE_STAT]    = { .p_port = BLE_IRQ__PORT ,   .pin = BLE_IRQ__PIN,    .mode = GPIO_MODE_INPUT, .pull = BLE_IRQ__PULL,     .speed = GPIO_SPEED_MEDIUM, .init_state = 0U },
    [eGPIO_WIFI_STAT]   = { .p_port = WIFI_IRQ__PORT,   .pin = WIFI_IRQ__PIN,   .mode = GPIO_MODE_INPUT, .pull = WIFI_IRQ__PULL,    .speed = GPIO_SPEED_MEDIUM, .init_state = 0U },


    // ============================================================================================================================================================================
    //          OUTPUT pins
    // ============================================================================================================================================================================
    [eGPIO_BLE_RST]     = { .p_port = BLE_RST__PORT,    .pin = BLE_RST__PIN,   .mode = GPIO_MODE_OUTPUT_PP, .pull = BLE_RST__PULL,    .speed = GPIO_SPEED_MEDIUM, .init_state = eGPIO_HIGH },
    [eGPIO_WIFI_RST]    = { .p_port = ESP_RST__PORT,    .pin = ESP_RST__PIN,   .mode = GPIO_MODE_OUTPUT_PP, .pull = ESP_RST__PULL,    .speed = GPIO_SPEED_MEDIUM, .init_state = eGPIO_HIGH },

    [eGPIO_LCD_CD]      = { .p_port = LCD_CD__PORT,     .pin = LCD_CD__PIN,    .mode = GPIO_MODE_OUTPUT_PP, .pull = LCD_CD__PULL,     .speed = GPIO_SPEED_MEDIUM, .init_state = eGPIO_LOW  },
    [eGPIO_LCD_CS]      = { .p_port = SPI2_CS__PORT,    .pin = SPI2_CS__PIN,   .mode = GPIO_MODE_OUTPUT_PP, .pull = SPI2_CS__PULL,    .speed = GPIO_SPEED_MEDIUM, .init_state = eGPIO_HIGH },
    [eGPIO_LCD_RST]     = { .p_port = LCD_RST__PORT,    .pin = LCD_RST__PIN,   .mode = GPIO_MODE_OUTPUT_PP, .pull = LCD_RST__PULL,    .speed = GPIO_SPEED_MEDIUM, .init_state = eGPIO_LOW  },
    [eGPIO_LCD_LED]     = { .p_port = BACKLIGHT__PORT,  .pin = BACKLIGHT__PIN, .mode = GPIO_MODE_OUTPUT_PP, .pull = BACKLIGHT__PULL,  .speed = GPIO_SPEED_MEDIUM, .init_state = eGPIO_LOW  },

    // USER CODE END...
};
```

**6. Initialize GPIO module**
```C
// Init GPIO
if ( eGPIO_OK != gpio_init())
{
    // Initialization failed...

    // Further actions here...
}
```

**7. Set, get and toggle GPIO pin**
```C
gpio_state_t pin_state = eGPIO_UNKNOWN;

// Get GPIO 
gpio_get( eGPIO_BLE_STAT, &pin_state );

if ( eGPIO_LOW == pin_state )
{
    // BLE_STATE pin set to low
}

// Set GPIO
gpio_set( eGPIO_WIFI_RST, eGPIO_LOW );

// Toggle backlight
gpio_toggle( eGPIO_LCD_LED );

```

