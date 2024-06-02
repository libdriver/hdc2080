/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_hdc2080.c
 * @brief     driver hdc2080 source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-05-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/05/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_hdc2080.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Texas Instruments HDC2080"        /**< chip name */
#define MANUFACTURER_NAME         "Texas Instruments"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.62f                              /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                               /**< chip max supply voltage */
#define MAX_CURRENT               90.0f                              /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                             /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                             /**< chip max operating temperature */
#define DRIVER_VERSION            1000                               /**< driver version */

/**
 * @brief chip register definition
 */
#define HDC2080_REG_TEMPERATURE_LOW        0x00        /**< temperature low register */
#define HDC2080_REG_TEMPERATURE_HIGH       0x01        /**< temperature high register */
#define HDC2080_REG_HUMIDITY_LOW           0x02        /**< humidity low register */
#define HDC2080_REG_HUMIDITY_HIGH          0x03        /**< humidity high register */
#define HDC2080_REG_INTERRUPT_DRDY         0x04        /**< interrupt drdy register */
#define HDC2080_REG_TEMPERATURE_MAX        0x05        /**< temperature max register */
#define HDC2080_REG_HUMIDITY_MAX           0x06        /**< humidity max register */
#define HDC2080_REG_INTERRUPT_ENABLE       0x07        /**< interrupt enable register */
#define HDC2080_REG_TEMP_OFFSET_ADJUST     0x08        /**< temp offset adjust register */
#define HDC2080_REG_HUM_OFFSET_ADJUST      0x09        /**< hum offset adjust register */
#define HDC2080_REG_TEMP_THR_L             0x0A        /**< temp thr low register */
#define HDC2080_REG_TEMP_THR_H             0x0B        /**< temp thr high register */
#define HDC2080_REG_RH_THR_L               0x0C        /**< rh thr low register */
#define HDC2080_REG_RH_THR_H               0x0D        /**< rh thr high register */
#define HDC2080_REG_CONF                   0x0E        /**< conf register */
#define HDC2080_REG_MEASUREMENT            0x0F        /**< measurement register */
#define HDC2080_REG_MANUFACTURER_ID_LOW    0xFC        /**< manufacturer id low register */
#define HDC2080_REG_MANUFACTURER_ID_HIGH   0xFD        /**< manufacturer id high register */
#define HDC2080_REG_DEVICE_ID_LOW          0xFE        /**< device id low register */
#define HDC2080_REG_DEVICE_ID_HIGH         0xFF        /**< device id high register */

/**
 * @brief      read data
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  reg is the read register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_hdc2080_iic_read(hdc2080_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)    /* read the register */
    {
        return 1;                                                  /* return error */
    }
    else
    {
        return 0;                                                  /* success return 0 */
    }
}

/**
 * @brief     write data
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] reg is the write register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_hdc2080_iic_write(hdc2080_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_write(handle->iic_addr, reg, buf, len) != 0)    /* write the register */
    {
        return 1;                                                   /* return error */
    }
    else
    {
        return 0;                                                   /* success return 0 */
    }
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t hdc2080_set_addr_pin(hdc2080_handle_t *handle, hdc2080_address_t addr_pin)
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;      /* set pin */
    
    return 0;                                  /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t hdc2080_get_addr_pin(hdc2080_handle_t *handle, hdc2080_address_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (hdc2080_address_t)(handle->iic_addr);        /* get pin */
    
    return 0;                                                 /* success return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an hdc2080 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic init failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 read id failed
 *            - 5 id is invalid
 *            - 6 reset failed
 * @note      none
 */
uint8_t hdc2080_init(hdc2080_handle_t *handle)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t id;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->debug_print == NULL)                                           /* check debug_print */
    {
        return 3;                                                              /* return error */
    }
    if (handle->iic_init == NULL)                                              /* check iic_init */
    {
        handle->debug_print("hdc2080: iic_init is null.\n");                   /* iic_init is null */
        
        return 3;                                                              /* return error */
    }
    if (handle->iic_deinit == NULL)                                            /* check iic_deinit */
    {
        handle->debug_print("hdc2080: iic_deinit is null.\n");                 /* iic_deinit is null */
        
        return 3;                                                              /* return error */
    }
    if (handle->iic_read == NULL)                                              /* check iic_read */
    {
        handle->debug_print("hdc2080: iic_read is null.\n");                   /* iic_read is null */
        
        return 3;                                                              /* return error */
    }
    if (handle->iic_write == NULL)                                             /* check iic_write */
    {
        handle->debug_print("hdc2080: iic_write is null.\n");                  /* iic_write is null */
        
        return 3;                                                              /* return error */
    }
    if (handle->delay_ms == NULL)                                              /* check delay_ms */
    {
        handle->debug_print("hdc2080: delay_ms is null.\n");                   /* delay_ms is null */
        
        return 3;                                                              /* return error */
    }
    
    if (handle->iic_init() != 0)                                               /* iic init */
    {
        handle->debug_print("hdc2080: iic init failed.\n");                    /* iic init failed */
        
        return 1;                                                              /* return error */
    }
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MANUFACTURER_ID_LOW, buf, 2); /* read manufacturer id */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("hdc2080: read manufacturer id failed.\n");        /* read manufacturer id failed */
        (void)handle->iic_deinit();                                            /* iic deinit */
        
        return 4;                                                              /* return error */
    }
    id = buf[0] | (uint16_t)buf[1] << 8;                                       /* set id */
    if (id != 0x5449)                                                          /* check id */
    {
        handle->debug_print("hdc2080: manufacturer id is invalid.\n");         /* read manufacturer id failed */
        (void)handle->iic_deinit();                                            /* iic deinit */
        
        return 5;                                                              /* return error */
    }
    res = a_hdc2080_iic_read(handle, HDC2080_REG_DEVICE_ID_LOW, buf, 2);       /* read device id */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("hdc2080: read device id failed.\n");              /* read device id failed */
        (void)handle->iic_deinit();                                            /* iic deinit */
        
        return 4;                                                              /* return error */
    }
    id = buf[0] | (uint16_t)buf[1] << 8;                                       /* set id */
    if (id != 0x07D0)                                                          /* check id */
    {
        handle->debug_print("hdc2080: device id is invalid.\n");               /* read device id failed */
        (void)handle->iic_deinit();                                            /* iic deinit */
        
        return 5;                                                              /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, buf, 1);                /* read config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                 /* read config failed */
        (void)handle->iic_deinit();                                            /* iic deinit */
        
        return 6;                                                              /* return error */
    }
    buf[0] &= ~(1 << 7);                                                       /* clear settings */
    buf[0] |= 1 << 7;                                                          /* software reset */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_CONF, buf, 1);               /* write config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                /* write config failed */
        (void)handle->iic_deinit();                                            /* iic deinit */
        
        return 6;                                                              /* return error */
    }
    handle->delay_ms(100);                                                     /* delay 100ms */
    handle->inited = 1;                                                        /* flag finish initialization */
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to an hdc2080 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_deinit(hdc2080_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    prev &= ~(7 << 4);                                                   /* clear settings */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_CONF, &prev, 1);       /* write config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");          /* write config failed */
        
        return 1;                                                        /* return error */
    }
    if (handle->iic_deinit() != 0)                                       /* iic deinit */
    {
        handle->debug_print("hdc2080: iic deinit failed.\n");            /* iic deinit failed */
        
        return 1;                                                        /* return error */
    }
    handle->inited = 0;                                                  /* set closed flag */
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     set temperature resolution
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] resolution is the set resolution
 * @return    status code
 *            - 0 success
 *            - 1 set temperature resolution failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_temperature_resolution(hdc2080_handle_t *handle, hdc2080_resolution_t resolution)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
        
        return 1;                                                               /* return error */
    }
    prev &= ~(3 << 6);                                                          /* clear settings */
    prev |= resolution << 6;                                                    /* set resolution */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_MEASUREMENT, &prev, 1);       /* write config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                 /* write config failed */
        
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief      get temperature resolution
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *resolution points to a resolution buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature resolution failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_temperature_resolution(hdc2080_handle_t *handle, hdc2080_resolution_t *resolution)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
        
        return 1;                                                               /* return error */
    }
    *resolution = (hdc2080_resolution_t)((prev >> 6) & 0x3);                    /* get resolution */
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief     set humidity resolution
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] resolution is the set resolution
 * @return    status code
 *            - 0 success
 *            - 1 set humidity resolution failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_humidity_resolution(hdc2080_handle_t *handle, hdc2080_resolution_t resolution)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
        
        return 1;                                                               /* return error */
    }
    prev &= ~(3 << 4);                                                          /* clear settings */
    prev |= resolution << 4;                                                    /* set resolution */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_MEASUREMENT, &prev, 1);       /* write config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                 /* write config failed */
        
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief      get humidity resolution
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *resolution points to a resolution buffer
 * @return     status code
 *             - 0 success
 *             - 1 get humidity resolution failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_humidity_resolution(hdc2080_handle_t *handle, hdc2080_resolution_t *resolution)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
        
        return 1;                                                               /* return error */
    }
    *resolution = (hdc2080_resolution_t)((prev >> 4) & 0x3);                    /* get resolution */
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief     set the chip mode
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_mode(hdc2080_handle_t *handle, hdc2080_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
        
        return 1;                                                               /* return error */
    }
    prev &= ~(3 << 1);                                                          /* clear settings */
    prev |= mode << 1;                                                          /* set mode */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_MEASUREMENT, &prev, 1);       /* write config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                 /* write config failed */
        
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief      get the chip mode
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_mode(hdc2080_handle_t *handle, hdc2080_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
        
        return 1;                                                               /* return error */
    }
    *mode = (hdc2080_mode_t)((prev >> 1) & 0x03);                               /* get mode */
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief     enable or disable measurement
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_measurement(hdc2080_handle_t *handle, hdc2080_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
        
        return 1;                                                               /* return error */
    }
    prev &= ~(1 << 0);                                                          /* clear settings */
    prev |= enable << 0;                                                        /* set bool */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_MEASUREMENT, &prev, 1);       /* write config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                 /* write config failed */
        
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief      get measurement status
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *enable points to a bool value
 * @return     status code
 *             - 0 success
 *             - 1 get measurement failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_measurement(hdc2080_handle_t *handle, hdc2080_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
        
        return 1;                                                               /* return error */
    }
    *enable = (hdc2080_bool_t)((prev >> 0) & 0x01);                             /* get bool */
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief     soft reset
 * @param[in] *handle points to an hdc2080 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_soft_reset(hdc2080_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << 7);                                                   /* clear settings */
    prev |= 1 << 7;                                                      /* set soft reset */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_CONF, &prev, 1);       /* write config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");          /* write config failed */
        
        return 1;                                                        /* return error */
    }
    handle->delay_ms(100);                                               /* delay 100ms */
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     set auto measurement mode
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] mode is the set mode
 * @return    status code
 *            - 0 success
 *            - 1 set auto measurement mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_auto_measurement_mode(hdc2080_handle_t *handle, hdc2080_auto_measurement_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    prev &= ~(7 << 4);                                                   /* clear settings */
    prev |= mode << 4;                                                   /* set mode */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_CONF, &prev, 1);       /* write config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");          /* write config failed */
        
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief      get auto measurement mode
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *mode points to a mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto measurement mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_auto_measurement_mode(hdc2080_handle_t *handle, hdc2080_auto_measurement_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    *mode = (hdc2080_auto_measurement_mode_t)((prev >> 4) & 0x07);       /* get mode */
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     enable or disable heater
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set heater failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_heater(hdc2080_handle_t *handle, hdc2080_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << 3);                                                   /* clear settings */
    prev |= enable << 3;                                                 /* set bool */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_CONF, &prev, 1);       /* write config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");          /* write config failed */
        
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief      get heater status
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get heater failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_heater(hdc2080_handle_t *handle, hdc2080_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    *enable = (hdc2080_bool_t)((prev >> 3) & 0x01);                      /* get bool */
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     enable or disable interrupt pin
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_interrupt_pin(hdc2080_handle_t *handle, hdc2080_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << 2);                                                   /* clear settings */
    prev |= enable << 2;                                                 /* set bool */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_CONF, &prev, 1);       /* write config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");          /* write config failed */
        
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief      get interrupt pin status
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_interrupt_pin(hdc2080_handle_t *handle, hdc2080_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    *enable = (hdc2080_bool_t)((prev >> 2) & 0x01);                      /* get bool */
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     set interrupt polarity
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] polarity is the set interrupt polarity
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt polarity failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_interrupt_polarity(hdc2080_handle_t *handle, hdc2080_interrupt_polarity_t polarity)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << 1);                                                   /* clear settings */
    prev |= polarity << 1;                                               /* set bool */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_CONF, &prev, 1);       /* write config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");          /* write config failed */
        
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief      get interrupt polarity
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *polarity points to a polarity buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt polarity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_interrupt_polarity(hdc2080_handle_t *handle, hdc2080_interrupt_polarity_t *polarity)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    *polarity = (hdc2080_interrupt_polarity_t)((prev >> 1) & 0x01);      /* get bool */
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     set interrupt mode
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] mode is the set interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_interrupt_mode(hdc2080_handle_t *handle, hdc2080_interrupt_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << 0);                                                   /* clear settings */
    prev |= mode << 0;                                                   /* set bool */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_CONF, &prev, 1);       /* write config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");          /* write config failed */
        
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief      get interrupt mode
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *mode points to an interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_interrupt_mode(hdc2080_handle_t *handle, hdc2080_interrupt_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_CONF, &prev, 1);        /* read config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");           /* read config failed */
        
        return 1;                                                        /* return error */
    }
    *mode = (hdc2080_interrupt_mode_t)((prev >> 0) & 0x01);              /* get mode */
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     set humidity high threshold
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] threshold is the set high threshold
 * @return    status code
 *            - 0 success
 *            - 1 set humidity high threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_humidity_high_threshold(hdc2080_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    prev = threshold;                                                        /* set threshold */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_RH_THR_H, &prev, 1);       /* write config */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");              /* write config failed */
        
        return 1;                                                            /* return error */
    }
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief      get humidity high threshold
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *threshold points to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get humidity high threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_humidity_high_threshold(hdc2080_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_RH_THR_H, &prev, 1);       /* read config */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");              /* read config failed */
        
        return 1;                                                           /* return error */
    }
    *threshold = prev;                                                      /* set threshold */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief     set humidity low threshold
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] threshold is the set low threshold
 * @return    status code
 *            - 0 success
 *            - 1 set humidity low threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_humidity_low_threshold(hdc2080_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    prev = threshold;                                                        /* set threshold */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_RH_THR_L, &prev, 1);       /* write config */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");              /* write config failed */
        
        return 1;                                                            /* return error */
    }
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief      get humidity low threshold
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *threshold points to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get humidity low threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_humidity_low_threshold(hdc2080_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_RH_THR_L, &prev, 1);       /* read config */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");              /* read config failed */
        
        return 1;                                                           /* return error */
    }
    *threshold = prev;                                                      /* set threshold */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief     set temperature high threshold
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] threshold is the set high threshold
 * @return    status code
 *            - 0 success
 *            - 1 set temperature high threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_temperature_high_threshold(hdc2080_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    prev = threshold;                                                        /* set threshold */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_TEMP_THR_H, &prev, 1);     /* write config */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");              /* write config failed */
        
        return 1;                                                            /* return error */
    }
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief      get temperature high threshold
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *threshold points to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature high threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_temperature_high_threshold(hdc2080_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_TEMP_THR_H, &prev, 1);     /* read config */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");              /* read config failed */
        
        return 1;                                                           /* return error */
    }
    *threshold = prev;                                                      /* set threshold */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief     set temperature low threshold
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] threshold is the set low threshold
 * @return    status code
 *            - 0 success
 *            - 1 set temperature low threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_temperature_low_threshold(hdc2080_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    prev = threshold;                                                        /* set threshold */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_TEMP_THR_L, &prev, 1);     /* write config */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");              /* write config failed */
        
        return 1;                                                            /* return error */
    }
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief      get temperature low threshold
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *threshold points to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature low threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_temperature_low_threshold(hdc2080_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_TEMP_THR_L, &prev, 1);     /* read config */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");              /* read config failed */
        
        return 1;                                                           /* return error */
    }
    *threshold = prev;                                                      /* set threshold */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief     set humidity offset adjustment
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] offset is the set offset
 * @return    status code
 *            - 0 success
 *            - 1 set humidity offset adjustment failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_humidity_offset_adjustment(hdc2080_handle_t *handle, int8_t offset)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    prev = (uint8_t)(offset);                                                       /* set offset */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_HUM_OFFSET_ADJUST, &prev, 1);     /* write config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                     /* write config failed */
        
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      get humidity offset adjustment
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *offset points to an offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get humidity offset adjustment failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_humidity_offset_adjustment(hdc2080_handle_t *handle, int8_t *offset)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_HUM_OFFSET_ADJUST, &prev, 1);      /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                      /* read config failed */
        
        return 1;                                                                   /* return error */
    }
    *offset = (int8_t)(prev);                                                       /* set offset */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     set temperature offset adjustment
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] offset is the set offset
 * @return    status code
 *            - 0 success
 *            - 1 set temperature offset adjustment failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_temperature_offset_adjustment(hdc2080_handle_t *handle, int8_t offset)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    prev = (uint8_t)(offset);                                                       /* set offset */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_TEMP_OFFSET_ADJUST, &prev, 1);    /* write config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                     /* write config failed */
        
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      get temperature offset adjustment
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *offset points to an offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature offset adjustment failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_temperature_offset_adjustment(hdc2080_handle_t *handle, int8_t *offset)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_TEMP_OFFSET_ADJUST, &prev, 1);     /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                      /* read config failed */
        
        return 1;                                                                   /* return error */
    }
    *offset = (int8_t)(prev);                                                       /* set offset */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     enable or disable interrupt
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] interrupt is the set interrupt
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_interrupt(hdc2080_handle_t *handle, hdc2080_interrupt_t interrupt, hdc2080_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_INTERRUPT_ENABLE, &prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                       /* read config failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << interrupt);                                                       /* clear settings */
    prev |= enable << interrupt;                                                     /* set bool */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_INTERRUPT_ENABLE, &prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                      /* write config failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get interrupt status
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  interrupt is the set interrupt
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_interrupt(hdc2080_handle_t *handle, hdc2080_interrupt_t interrupt, hdc2080_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_INTERRUPT_ENABLE, &prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                       /* read config failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (hdc2080_bool_t)((prev >> interrupt) & 0x01);                          /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     set humidity max
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] max is the set humidity max
 * @return    status code
 *            - 0 success
 *            - 1 set humidity max failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_humidity_max(hdc2080_handle_t *handle, uint8_t max)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    prev = max;                                                                   /* set max */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_HUMIDITY_MAX, &prev, 1);        /* write config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                   /* write config failed */
        
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

/**
 * @brief      get humidity max
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *max points to a humidity max buffer
 * @return     status code
 *             - 0 success
 *             - 1 get humidity max failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_humidity_max(hdc2080_handle_t *handle, uint8_t *max)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_HUMIDITY_MAX, &prev, 1);        /* read config */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                   /* read config failed */
        
        return 1;                                                                /* return error */
    }
    *max = prev;                                                                 /* get max */
    
    return 0;                                                                    /* success return 0 */
}

/**
 * @brief     set temperature max
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] max is the set temperature max
 * @return    status code
 *            - 0 success
 *            - 1 set temperature max failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_temperature_max(hdc2080_handle_t *handle, uint8_t max)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    prev = max;                                                                      /* set max */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_TEMPERATURE_MAX, &prev, 1);        /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                      /* write config failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get temperature max
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *max points to a temperature max buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature max failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_temperature_max(hdc2080_handle_t *handle, uint8_t *max)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_TEMPERATURE_MAX, &prev, 1);        /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                      /* read config failed */
        
        return 1;                                                                   /* return error */
    }
    *max = prev;                                                                    /* get max */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      get interrupt status
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *status points to an interrupt status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_interrupt_status(hdc2080_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_INTERRUPT_DRDY, &prev, 1);        /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                     /* read config failed */
        
        return 1;                                                                  /* return error */
    }
    *status = prev;                                                                /* get status */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      convert the humidity to the register raw data
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  percent is the set percent
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_humidity_convert_to_register(hdc2080_handle_t *handle, float percent, uint8_t *reg)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    
    *reg = (uint8_t)(percent / 100.0f * (float)(256.0f));        /* convert real data to register data */
    
    return 0;                                                    /* success return 0 */
}

/**
 * @brief      convert the register raw data to the humidity
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *percent points to a percent buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_humidity_convert_to_data(hdc2080_handle_t *handle, uint8_t reg, float *percent)
{
    if (handle == NULL)                               /* check handle */
    {
        return 2;                                     /* return error */
    }
    if (handle->inited != 1)                          /* check handle initialization */
    {
        return 3;                                     /* return error */
    }
    
    *percent = (float)(reg) / 256.0f * 100.0f;        /* convert raw data to real data */
    
    return 0;                                         /* success return 0 */
}

/**
 * @brief      convert the temperature to the register raw data
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  deg is the set deg
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_temperature_convert_to_register(hdc2080_handle_t *handle, float deg, uint8_t *reg)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    *reg = (uint8_t)((deg + 40.5f) / 165.0f * (float)(256.0f));        /* convert real data to register data */
    
    return 0;                                                          /* success return 0 */
}

/**
 * @brief      convert the register raw data to the temperature
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *deg points to a deg buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_temperature_convert_to_data(hdc2080_handle_t *handle, uint8_t reg, float *deg)
{
    if (handle == NULL)                                   /* check handle */
    {
        return 2;                                         /* return error */
    }
    if (handle->inited != 1)                              /* check handle initialization */
    {
        return 3;                                         /* return error */
    }
    
    *deg = (float)(reg) / 256.0f * 165.0f - 40.5f;        /* convert raw data to real data */
    
    return 0;                                             /* success return 0 */
}

/**
 * @brief      convert the humidity offset to the register raw data
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  percent is the set percent
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_humidity_offset_convert_to_register(hdc2080_handle_t *handle, float percent, int8_t *reg)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->inited != 1)                /* check handle initialization */
    {
        return 3;                           /* return error */
    }
    
    *reg = (int8_t)(percent / 0.2f);        /* convert real data to register data */
    
    return 0;                               /* success return 0 */
}

/**
 * @brief      convert the register raw data to the humidity offset
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *percent points to a percent buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_humidity_offset_convert_to_data(hdc2080_handle_t *handle, int8_t reg, float *percent)
{
    if (handle == NULL)                    /* check handle */
    {
        return 2;                          /* return error */
    }
    if (handle->inited != 1)               /* check handle initialization */
    {
        return 3;                          /* return error */
    }
    
    *percent = (float)(reg) * 0.2f;        /* convert raw data to real data */
    
    return 0;                              /* success return 0 */
}

/**
 * @brief      convert the temperature offset to the register raw data
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  deg is the set deg
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_temperature_offset_convert_to_register(hdc2080_handle_t *handle, float deg, int8_t *reg)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    *reg = (int8_t)(deg / 0.16f);        /* convert real data to register data */
    
    return 0;                            /* success return 0 */
}

/**
 * @brief      convert the register raw data to the temperature offset
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *deg points to a deg buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_temperature_offset_convert_to_data(hdc2080_handle_t *handle, int8_t reg, float *deg)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *deg = (float)(reg) * 0.16f;        /* convert raw data to real data */
    
    return 0;                           /* success return 0 */
}

/**
 * @brief      read the temperature and humidity data
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *temperature_raw points to a raw temperature buffer
 * @param[out] *temperature_s points to a converted temperature buffer
 * @param[out] *humidity_raw points to a raw humidity buffer
 * @param[out] *humidity_s points to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature humidity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_read_temperature_humidity(hdc2080_handle_t *handle, uint16_t *temperature_raw, float *temperature_s,
                                          uint16_t *humidity_raw, float *humidity_s)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_TEMPERATURE_LOW, buf, 2);             /* read temperature */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("hdc2080: read temperature failed.\n");                    /* read temperature failed */
        
        return 1;                                                                      /* return error */
    }
    *temperature_raw = buf[0] | (uint16_t)(buf[1]) << 8;                               /* get temperature */
    *temperature_s = (float)(*temperature_raw) / 65536.0f * 165.0f - 40.5f;            /* convert temperature */
    res = a_hdc2080_iic_read(handle, HDC2080_REG_HUMIDITY_LOW, buf, 2);                /* read humidity */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("hdc2080: read humidity failed.\n");                       /* read humidity failed */
        
        return 1;                                                                      /* return error */
    }
    *humidity_raw = buf[0] | (uint16_t)(buf[1]) << 8;                                  /* get humidity */
    *humidity_s = (float)(*humidity_raw) / 65536.0f * 100.0f;                          /* convert humidity */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief      read the temperature
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *temperature_raw points to a raw temperature buffer
 * @param[out] *temperature_s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_read_temperature(hdc2080_handle_t *handle, uint16_t *temperature_raw, float *temperature_s)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_TEMPERATURE_LOW, buf, 2);             /* read temperature */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("hdc2080: read temperature failed.\n");                    /* read temperature failed */
        
        return 1;                                                                      /* return error */
    }
    *temperature_raw = buf[0] | (uint16_t)(buf[1]) << 8;                               /* get temperature */
    *temperature_s = (float)(*temperature_raw) / 65536.0f * 165.0f - 40.5f;            /* convert temperature */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief      read the humidity data
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *humidity_raw points to a raw humidity buffer
 * @param[out] *humidity_s points to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read humidity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_read_humidity(hdc2080_handle_t *handle, uint16_t *humidity_raw, float *humidity_s)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_HUMIDITY_LOW, buf, 2);                /* read humidity */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("hdc2080: read humidity failed.\n");                       /* read humidity failed */
        
        return 1;                                                                      /* return error */
    }
    *humidity_raw = buf[0] | (uint16_t)(buf[1]) << 8;                                  /* get humidity */
    *humidity_s = (float)(*humidity_raw) / 65536.0f * 100.0f;                          /* convert humidity */
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief     read poll
 * @param[in] *handle points to an hdc2080 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 read poll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 read timeout
 * @note      none
 */
uint8_t hdc2080_read_poll(hdc2080_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    uint32_t i;
    uint32_t timeout = 500;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);            /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("hdc2080: read config failed.\n");                      /* read config failed */
        
        return 1;                                                                   /* return error */
    }
    prev &= ~(1 << 0);                                                              /* clear settings */
    prev |= 1 << 0;                                                                 /* set bool */
    res = a_hdc2080_iic_write(handle, HDC2080_REG_MEASUREMENT, &prev, 1);           /* write config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("hdc2080: write config failed.\n");                     /* write config failed */
        
        return 1;                                                                   /* return error */
    }
    for (i = 0; i < timeout; i++)                                                   /* wait */
    {
        res = a_hdc2080_iic_read(handle, HDC2080_REG_MEASUREMENT, &prev, 1);        /* read config */
        if (res != 0)                                                               /* check result */
        {
            handle->debug_print("hdc2080: read config failed.\n");                  /* read config failed */
            
            return 1;                                                               /* return error */
        }
        if (((prev >> 0) & 0x01) == 0)                                              /* check flag */
        {
            break;                                                                  /* break */
        }
        handle->delay_ms(10);                                                       /* delay 10ms */
    }
    if (i >= timeout)                                                               /* check timeout */
    {
        handle->debug_print("hdc2080: read timeout.\n");                            /* read timeout */
        
        return 4;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     set chip register
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] reg is the write register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t hdc2080_set_reg(hdc2080_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    if (handle->inited != 1)                                 /* check handle initialization */
    {
        return 3;                                            /* return error */
    } 
    
    if (a_hdc2080_iic_write(handle, reg, buf, len) != 0)     /* write data */
    {
        return 1;                                            /* return error */
    }
    else
    {
        return 0;                                            /* success return 0 */
    }
}

/**
 * @brief      get chip register
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[in]  reg is the read register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t hdc2080_get_reg(hdc2080_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    } 
    
    if (a_hdc2080_iic_read(handle, reg, buf, len) != 0)     /* read data */
    {
        return 1;                                           /* return error */
    }
    else
    {
        return 0;                                           /* success return 0 */
    }
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an hdc2080 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t hdc2080_info(hdc2080_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(hdc2080_info_t));                        /* initialize hdc2080 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
