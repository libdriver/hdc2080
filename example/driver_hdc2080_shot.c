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
 * @file      driver_hdc2080_shot.c
 * @brief     driver hdc2080 shot source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2025-05-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/05/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_hdc2080_shot.h"

static hdc2080_handle_t gs_handle;        /**< hdc2080 handle */

/**
 * @brief     shot example init
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t hdc2080_shot_init(hdc2080_address_t addr_pin)
{
    uint8_t res;
    uint8_t reg;
    int8_t offset;
    
    /* link interface function */
    DRIVER_HDC2080_LINK_INIT(&gs_handle, hdc2080_handle_t); 
    DRIVER_HDC2080_LINK_IIC_INIT(&gs_handle, hdc2080_interface_iic_init);
    DRIVER_HDC2080_LINK_IIC_DEINIT(&gs_handle, hdc2080_interface_iic_deinit);
    DRIVER_HDC2080_LINK_IIC_READ(&gs_handle, hdc2080_interface_iic_read);
    DRIVER_HDC2080_LINK_IIC_WRITE(&gs_handle, hdc2080_interface_iic_write);
    DRIVER_HDC2080_LINK_DELAY_MS(&gs_handle, hdc2080_interface_delay_ms);
    DRIVER_HDC2080_LINK_DEBUG_PRINT(&gs_handle, hdc2080_interface_debug_print);
    
    /* set addr pin */
    res = hdc2080_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set addr pin failed.\n");
        
        return 1;
    }
    
    /* hdc2080 init */
    res = hdc2080_init(&gs_handle);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: init failed.\n");
       
        return 1;
    }
    
    /* set default temperature resolution */
    res = hdc2080_set_temperature_resolution(&gs_handle, HDC2080_SHOT_DEFAULT_TEMPERATURE_RESOLUTION);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default humidity resolution */
    res = hdc2080_set_humidity_resolution(&gs_handle, HDC2080_SHOT_DEFAULT_HUMIDITY_RESOLUTION);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set humidity and temperature */
    res = hdc2080_set_mode(&gs_handle, HDC2080_MODE_HUMIDITY_TEMPERATURE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default heater */
    res = hdc2080_set_heater(&gs_handle, HDC2080_SHOT_DEFAULT_HEATER);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set heater failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable interrupt pin */
    res = hdc2080_set_interrupt_pin(&gs_handle, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt pin failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interrupt polarity */
    res = hdc2080_set_interrupt_polarity(&gs_handle, HDC2080_SHOT_DEFAULT_INTERRUPT_POLARITY);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt polarity failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default interrupt mode */
    res = hdc2080_set_interrupt_mode(&gs_handle, HDC2080_SHOT_DEFAULT_INTERRUPT_MODE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 0.0% */
    res = hdc2080_humidity_convert_to_register(&gs_handle, 0.0f, &reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: humidity convert to register failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set humidity high threshold */
    res = hdc2080_set_humidity_high_threshold(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity high threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set humidity low threshold */
    res = hdc2080_set_humidity_low_threshold(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity low threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 0.0 */
    res = hdc2080_temperature_convert_to_register(&gs_handle, 0.0f, &reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: temperature convert to register failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature high threshold */
    res = hdc2080_set_temperature_high_threshold(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature high threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature low threshold */
    res = hdc2080_set_temperature_low_threshold(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature low threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default offset */
    res = hdc2080_humidity_offset_convert_to_register(&gs_handle, HDC2080_SHOT_DEFAULT_TEMPERATURE_OFFSET, &offset);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: humidity offset convert to register failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set humidity offset adjustment */
    res = hdc2080_set_humidity_offset_adjustment(&gs_handle, offset);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity offset adjustment failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default offset */
    res = hdc2080_temperature_offset_convert_to_register(&gs_handle, HDC2080_SHOT_DEFAULT_HUMIDITY_OFFSET, &offset);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: temperature offset convert to register failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature offset adjustment */
    res = hdc2080_set_temperature_offset_adjustment(&gs_handle, offset);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature offset adjustment failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable data ready */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_DRDY, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable temperature high threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_HIGH_THRESHOLD, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable temperature low threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_LOW_THRESHOLD, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable humidity high threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_HIGH_THRESHOLD, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable humidity low threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_LOW_THRESHOLD, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable auto measurement */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_DISABLED);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      shot example read
 * @param[out] *temperature points to a converted temperature buffer
 * @param[out] *humidity points to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t hdc2080_shot_read(float *temperature, float *humidity)
{
    uint8_t res;
    uint16_t temperature_raw;
    uint16_t humidity_raw;
    
    /* read poll */
    res = hdc2080_read_poll(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    /* read temperature and humidity */
    if (hdc2080_read_temperature_humidity(&gs_handle, (uint16_t *)&temperature_raw, temperature, 
                                         (uint16_t *)&humidity_raw, humidity) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      shot get temperature max
 * @param[out] *temperature points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t hdc2080_shot_get_temperature_max(float *temperature)
{
    uint8_t res;
    uint8_t max;
    
    /* get temperature max */
    res = hdc2080_get_temperature_max(&gs_handle, &max);
    if (res != 0)
    {
        return 1;
    }
    
    /* temperature convert to data */
    res = hdc2080_temperature_convert_to_data(&gs_handle, max, temperature);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      shot get humidity max
 * @param[out] *percent points to a converted percent buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t hdc2080_shot_get_humidity_max(float *percent)
{
    uint8_t res;
    uint8_t max;
    
    /* get humidity max */
    res = hdc2080_get_humidity_max(&gs_handle, &max);
    if (res != 0)
    {
        return 1;
    }
    
    /* humidity convert to data */
    res = hdc2080_humidity_convert_to_data(&gs_handle, max, percent);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t hdc2080_shot_deinit(void)
{
    /* deinit hdc2080 and close bus */
    if (hdc2080_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
