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
 * @file      driver_hdc2080.h
 * @brief     driver hdc2080 header file
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

#ifndef DRIVER_HDC2080_H
#define DRIVER_HDC2080_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup hdc2080_driver hdc2080 driver function
 * @brief    hdc2080 driver modules
 * @{
 */

/**
 * @addtogroup hdc2080_basic_driver
 * @{
 */

/**
 * @brief hdc2080 address enumeration definition
 */
typedef enum
{
    HDC2080_ADDRESS_0 = (0x40 << 1),        /**< addr pin connected to the GND */
    HDC2080_ADDRESS_1 = (0x41 << 1),        /**< addr pin connected to the VCC */
} hdc2080_address_t;

/**
 * @brief hdc2080 bool enumeration definition
 */
typedef enum
{
    HDC2080_BOOL_FALSE = 0x00,        /**< disable */
    HDC2080_BOOL_TRUE  = 0x01,        /**< enable */
} hdc2080_bool_t;

/**
 * @brief hdc2080 resolution enumeration definition
 */
typedef enum
{
    HDC2080_RESOLUTION_14_BIT = 0x00,        /**< 14 bit */
    HDC2080_RESOLUTION_11_BIT = 0x01,        /**< 11 bit */
    HDC2080_RESOLUTION_9_BIT  = 0x02,        /**< 9 bit */
} hdc2080_resolution_t;

/**
 * @brief hdc2080 mode enumeration definition
 */
typedef enum
{
    HDC2080_MODE_HUMIDITY_TEMPERATURE = 0x00,        /**< humidity temperature */
    HDC2080_MODE_TEMPERATURE          = 0x01,        /**< temperature only */
} hdc2080_mode_t;

/**
 * @brief hdc2080 auto measurement mode enumeration definition
 */
typedef enum
{
    HDC2080_AUTO_MEASUREMENT_MODE_DISABLED     = 0x00,        /**< disable */
    HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_120_HZ = 0x01,        /**< 1/120 hz */
    HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_60_HZ  = 0x02,        /**< 1/60 hz */
    HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_10_HZ  = 0x03,        /**< 1/10 hz */
    HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_5_HZ   = 0x04,        /**< 1/5 hz */
    HDC2080_AUTO_MEASUREMENT_MODE_1_HZ         = 0x05,        /**< 1 hz */
    HDC2080_AUTO_MEASUREMENT_MODE_2_HZ         = 0x06,        /**< 2 hz */
    HDC2080_AUTO_MEASUREMENT_MODE_5_HZ         = 0x07,        /**< 5 hz */
} hdc2080_auto_measurement_mode_t;

/**
 * @brief hdc2080 interrupt polarity enumeration definition
 */
typedef enum
{
    HDC2080_INTERRUPT_POLARITY_LOW  = 0x00,        /**< active low */
    HDC2080_INTERRUPT_POLARITY_HIGH = 0x01,        /**< active high */
} hdc2080_interrupt_polarity_t;

/**
 * @brief hdc2080 interrupt enumeration definition
 */
typedef enum
{
    HDC2080_INTERRUPT_DRDY                       = 7,        /**< data ready */
    HDC2080_INTERRUPT_TEMPERATURE_HIGH_THRESHOLD = 6,        /**< temperature high threshold */
    HDC2080_INTERRUPT_TEMPERATURE_LOW_THRESHOLD  = 5,        /**< temperature low threshold */
    HDC2080_INTERRUPT_HUMIDITY_HIGH_THRESHOLD    = 4,        /**< humidity high threshold */
    HDC2080_INTERRUPT_HUMIDITY_LOW_THRESHOLD     = 3,        /**< humidity low threshold */
} hdc2080_interrupt_t;

/**
 * @brief hdc2080 interrupt mode enumeration definition
 */
typedef enum
{
    HDC2080_INTERRUPT_MODE_LEVEL      = 0x00,        /**< level sensitive */
    HDC2080_INTERRUPT_MODE_COMPARATOR = 0x01,        /**< comparator mode */
} hdc2080_interrupt_mode_t;

/**
 * @brief hdc2080 handle structure definition
 */
typedef struct hdc2080_handle_s
{
    uint8_t (*iic_init)(void);                                                         /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                       /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);       /**< point to an iic_write function address */
    void (*delay_ms)(uint32_t ms);                                                     /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                   /**< point to a debug_print function address */
    uint8_t inited;                                                                    /**< inited flag */
    uint8_t iic_addr;                                                                  /**< iic address */
} hdc2080_handle_t;

/**
 * @brief hdc2080 information structure definition
 */
typedef struct hdc2080_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} hdc2080_info_t;

/**
 * @}
 */

/**
 * @defgroup hdc2080_link_driver hdc2080 link driver function
 * @brief    hdc2080 link driver modules
 * @ingroup  hdc2080_driver
 * @{
 */

/**
 * @brief     initialize hdc2080_handle_t structure
 * @param[in] HANDLE points to an hdc2080 handle structure
 * @param[in] STRUCTURE is hdc2080_handle_t
 * @note      none
 */
#define DRIVER_HDC2080_LINK_INIT(HANDLE, STRUCTURE)             memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an hdc2080 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_HDC2080_LINK_IIC_INIT(HANDLE, FUC)              (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an hdc2080 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_HDC2080_LINK_IIC_DEINIT(HANDLE, FUC)            (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to an hdc2080 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_HDC2080_LINK_IIC_READ(HANDLE, FUC)              (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to an hdc2080 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_HDC2080_LINK_IIC_WRITE(HANDLE, FUC)             (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an hdc2080 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_HDC2080_LINK_DELAY_MS(HANDLE, FUC)              (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an hdc2080 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_HDC2080_LINK_DEBUG_PRINT(HANDLE, FUC)           (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup hdc2080_basic_driver hdc2080 basic driver function
 * @brief    hdc2080 basic driver modules
 * @ingroup  hdc2080_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an hdc2080 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t hdc2080_info(hdc2080_info_t *info);

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an hdc2080 handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t hdc2080_set_addr_pin(hdc2080_handle_t *handle, hdc2080_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an hdc2080 handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t hdc2080_get_addr_pin(hdc2080_handle_t *handle, hdc2080_address_t *addr_pin);

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
uint8_t hdc2080_init(hdc2080_handle_t *handle);

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
uint8_t hdc2080_deinit(hdc2080_handle_t *handle);

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
                                          uint16_t *humidity_raw, float *humidity_s);

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
uint8_t hdc2080_read_temperature(hdc2080_handle_t *handle, uint16_t *temperature_raw, float *temperature_s);

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
uint8_t hdc2080_read_humidity(hdc2080_handle_t *handle, uint16_t *humidity_raw, float *humidity_s);

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
uint8_t hdc2080_read_poll(hdc2080_handle_t *handle);

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
uint8_t hdc2080_set_temperature_resolution(hdc2080_handle_t *handle, hdc2080_resolution_t resolution);

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
uint8_t hdc2080_get_temperature_resolution(hdc2080_handle_t *handle, hdc2080_resolution_t *resolution);

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
uint8_t hdc2080_set_humidity_resolution(hdc2080_handle_t *handle, hdc2080_resolution_t resolution);

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
uint8_t hdc2080_get_humidity_resolution(hdc2080_handle_t *handle, hdc2080_resolution_t *resolution);

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
uint8_t hdc2080_set_mode(hdc2080_handle_t *handle, hdc2080_mode_t mode);

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
uint8_t hdc2080_get_mode(hdc2080_handle_t *handle, hdc2080_mode_t *mode);

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
uint8_t hdc2080_set_measurement(hdc2080_handle_t *handle, hdc2080_bool_t enable);

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
uint8_t hdc2080_get_measurement(hdc2080_handle_t *handle, hdc2080_bool_t *enable);

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
uint8_t hdc2080_soft_reset(hdc2080_handle_t *handle);

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
uint8_t hdc2080_set_auto_measurement_mode(hdc2080_handle_t *handle, hdc2080_auto_measurement_mode_t mode);

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
uint8_t hdc2080_get_auto_measurement_mode(hdc2080_handle_t *handle, hdc2080_auto_measurement_mode_t *mode);

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
uint8_t hdc2080_set_heater(hdc2080_handle_t *handle, hdc2080_bool_t enable);

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
uint8_t hdc2080_get_heater(hdc2080_handle_t *handle, hdc2080_bool_t *enable);

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
uint8_t hdc2080_set_interrupt_pin(hdc2080_handle_t *handle, hdc2080_bool_t enable);

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
uint8_t hdc2080_get_interrupt_pin(hdc2080_handle_t *handle, hdc2080_bool_t *enable);

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
uint8_t hdc2080_set_interrupt_polarity(hdc2080_handle_t *handle, hdc2080_interrupt_polarity_t polarity);

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
uint8_t hdc2080_get_interrupt_polarity(hdc2080_handle_t *handle, hdc2080_interrupt_polarity_t *polarity);

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
uint8_t hdc2080_set_interrupt_mode(hdc2080_handle_t *handle, hdc2080_interrupt_mode_t mode);

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
uint8_t hdc2080_get_interrupt_mode(hdc2080_handle_t *handle, hdc2080_interrupt_mode_t *mode);

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
uint8_t hdc2080_set_humidity_high_threshold(hdc2080_handle_t *handle, uint8_t threshold);

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
uint8_t hdc2080_get_humidity_high_threshold(hdc2080_handle_t *handle, uint8_t *threshold);

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
uint8_t hdc2080_set_humidity_low_threshold(hdc2080_handle_t *handle, uint8_t threshold);

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
uint8_t hdc2080_get_humidity_low_threshold(hdc2080_handle_t *handle, uint8_t *threshold);

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
uint8_t hdc2080_set_temperature_high_threshold(hdc2080_handle_t *handle, uint8_t threshold);

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
uint8_t hdc2080_get_temperature_high_threshold(hdc2080_handle_t *handle, uint8_t *threshold);

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
uint8_t hdc2080_set_temperature_low_threshold(hdc2080_handle_t *handle, uint8_t threshold);

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
uint8_t hdc2080_get_temperature_low_threshold(hdc2080_handle_t *handle, uint8_t *threshold);

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
uint8_t hdc2080_set_humidity_offset_adjustment(hdc2080_handle_t *handle, int8_t offset);

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
uint8_t hdc2080_get_humidity_offset_adjustment(hdc2080_handle_t *handle, int8_t *offset);

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
uint8_t hdc2080_set_temperature_offset_adjustment(hdc2080_handle_t *handle, int8_t offset);

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
uint8_t hdc2080_get_temperature_offset_adjustment(hdc2080_handle_t *handle, int8_t *offset);

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
uint8_t hdc2080_set_interrupt(hdc2080_handle_t *handle, hdc2080_interrupt_t interrupt, hdc2080_bool_t enable);

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
uint8_t hdc2080_get_interrupt(hdc2080_handle_t *handle, hdc2080_interrupt_t interrupt, hdc2080_bool_t *enable);

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
uint8_t hdc2080_set_humidity_max(hdc2080_handle_t *handle, uint8_t max);

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
uint8_t hdc2080_get_humidity_max(hdc2080_handle_t *handle, uint8_t *max);

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
uint8_t hdc2080_set_temperature_max(hdc2080_handle_t *handle, uint8_t max);

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
uint8_t hdc2080_get_temperature_max(hdc2080_handle_t *handle, uint8_t *max);

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
uint8_t hdc2080_get_interrupt_status(hdc2080_handle_t *handle, uint8_t *status);

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
uint8_t hdc2080_humidity_convert_to_register(hdc2080_handle_t *handle, float percent, uint8_t *reg);

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
uint8_t hdc2080_humidity_convert_to_data(hdc2080_handle_t *handle, uint8_t reg, float *percent);

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
uint8_t hdc2080_temperature_convert_to_register(hdc2080_handle_t *handle, float deg, uint8_t *reg);

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
uint8_t hdc2080_temperature_convert_to_data(hdc2080_handle_t *handle, uint8_t reg, float *deg);

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
uint8_t hdc2080_humidity_offset_convert_to_register(hdc2080_handle_t *handle, float percent, int8_t *reg);

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
uint8_t hdc2080_humidity_offset_convert_to_data(hdc2080_handle_t *handle, int8_t reg, float *percent);

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
uint8_t hdc2080_temperature_offset_convert_to_register(hdc2080_handle_t *handle, float deg, int8_t *reg);

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
uint8_t hdc2080_temperature_offset_convert_to_data(hdc2080_handle_t *handle, int8_t reg, float *deg);

/**
 * @}
 */

/**
 * @defgroup hdc2080_extern_driver hdc2080 extern driver function
 * @brief    hdc2080 extern driver modules
 * @ingroup  hdc2080_driver
 * @{
 */

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
uint8_t hdc2080_set_reg(hdc2080_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

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
uint8_t hdc2080_get_reg(hdc2080_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
