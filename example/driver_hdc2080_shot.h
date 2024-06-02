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
 * @file      driver_hdc2080_shot.h
 * @brief     driver hdc2080 shot header file
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

#ifndef DRIVER_HDC2080_SHOT_H
#define DRIVER_HDC2080_SHOT_H

#include "driver_hdc2080_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup hdc2080_example_driver
 * @{
 */

/**
 * @brief hdc2080 shot example default definition
 */
#define HDC2080_SHOT_DEFAULT_TEMPERATURE_RESOLUTION         HDC2080_RESOLUTION_14_BIT              /**< 14bits */
#define HDC2080_SHOT_DEFAULT_HUMIDITY_RESOLUTION            HDC2080_RESOLUTION_14_BIT              /**< 14bits */
#define HDC2080_SHOT_DEFAULT_HEATER                         HDC2080_BOOL_FALSE                     /**< disable heater */
#define HDC2080_SHOT_DEFAULT_INTERRUPT_POLARITY             HDC2080_INTERRUPT_POLARITY_LOW         /**< active low */
#define HDC2080_SHOT_DEFAULT_INTERRUPT_MODE                 HDC2080_INTERRUPT_MODE_COMPARATOR      /**< comparator mode */
#define HDC2080_SHOT_DEFAULT_TEMPERATURE_OFFSET             0.0f                                   /**< 0.0 deg */
#define HDC2080_SHOT_DEFAULT_HUMIDITY_OFFSET                0.0f                                   /**< 0.0f % */

/**
 * @brief     shot example init
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t hdc2080_shot_init(hdc2080_address_t addr_pin);

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t hdc2080_shot_deinit(void);

/**
 * @brief      shot example read
 * @param[out] *temperature points to a converted temperature buffer
 * @param[out] *humidity points to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t hdc2080_shot_read(float *temperature, float *humidity);

/**
 * @brief      shot get temperature max
 * @param[out] *temperature points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t hdc2080_shot_get_temperature_max(float *temperature);

/**
 * @brief      shot get humidity max
 * @param[out] *percent points to a converted percent buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t hdc2080_shot_get_humidity_max(float *percent);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
