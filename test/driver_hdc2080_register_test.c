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
 * @file      driver_hdc2080_register_test.c
 * @brief     driver hdc2080 register test source file
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
 
#include "driver_hdc2080_register_test.h"
#include <stdlib.h>

static hdc2080_handle_t gs_handle;        /**< hdc2080 handle */

/**
 * @brief     register test
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t hdc2080_register_test(hdc2080_address_t addr_pin)
{
    uint8_t res;
    uint8_t reg;
    uint8_t reg_check;
    int8_t offset;
    int8_t offset_check;
    float f;
    float f_check;
    hdc2080_address_t addr_pin_check;
    hdc2080_resolution_t resolution;
    hdc2080_mode_t mode;
    hdc2080_auto_measurement_mode_t auto_mode;
    hdc2080_bool_t enable;
    hdc2080_info_t info;
    hdc2080_interrupt_polarity_t polarity;
    hdc2080_interrupt_mode_t interrupt_mode;

    /* link interface function */
    DRIVER_HDC2080_LINK_INIT(&gs_handle, hdc2080_handle_t); 
    DRIVER_HDC2080_LINK_IIC_INIT(&gs_handle, hdc2080_interface_iic_init);
    DRIVER_HDC2080_LINK_IIC_DEINIT(&gs_handle, hdc2080_interface_iic_deinit);
    DRIVER_HDC2080_LINK_IIC_READ(&gs_handle, hdc2080_interface_iic_read);
    DRIVER_HDC2080_LINK_IIC_WRITE(&gs_handle, hdc2080_interface_iic_write);
    DRIVER_HDC2080_LINK_DELAY_MS(&gs_handle, hdc2080_interface_delay_ms);
    DRIVER_HDC2080_LINK_DEBUG_PRINT(&gs_handle, hdc2080_interface_debug_print);
    
    /* get information */
    res = hdc2080_info(&info);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        hdc2080_interface_debug_print("hdc2080: chip is %s.\n", info.chip_name);
        hdc2080_interface_debug_print("hdc2080: manufacturer is %s.\n", info.manufacturer_name);
        hdc2080_interface_debug_print("hdc2080: interface is %s.\n", info.interface);
        hdc2080_interface_debug_print("hdc2080: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        hdc2080_interface_debug_print("hdc2080: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        hdc2080_interface_debug_print("hdc2080: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        hdc2080_interface_debug_print("hdc2080: max current is %0.2fmA.\n", info.max_current_ma);
        hdc2080_interface_debug_print("hdc2080: max temperature is %0.1fC.\n", info.temperature_max);
        hdc2080_interface_debug_print("hdc2080: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    hdc2080_interface_debug_print("hdc2080: start register test.\n");
    
    /* hdc2080_set_addr_pin/hdc2080_get_addr_pin test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_addr_pin/hdc2080_get_addr_pin test.\n");
    
    /* set addr pin 0 */
    res = hdc2080_set_addr_pin(&gs_handle, HDC2080_ADDRESS_0);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set addr pin failed.\n");
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set addr pin 0.\n");
    res = hdc2080_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get addr pin failed.\n");
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check addr pin %s.\n", addr_pin_check == HDC2080_ADDRESS_0 ? "ok" : "error");
    
    /* set addr pin 1 */
    res = hdc2080_set_addr_pin(&gs_handle, HDC2080_ADDRESS_1);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set addr pin failed.\n");
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set addr pin 1.\n");
    res = hdc2080_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get addr pin failed.\n");
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check addr pin %s.\n", addr_pin_check == HDC2080_ADDRESS_1 ? "ok" : "error");
    
    /* set addr pin */
    res = hdc2080_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set addr pin failed.\n");
        
        return 1;
    }
    
    /* init */
    res = hdc2080_init(&gs_handle);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: init failed.\n");
        
        return 1;
    }
    
    /* hdc2080_set_temperature_resolution/hdc2080_get_temperature_resolution test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_temperature_resolution/hdc2080_get_temperature_resolution test.\n");
    
    /* set temperature resolution 14bit */
    res = hdc2080_set_temperature_resolution(&gs_handle, HDC2080_RESOLUTION_14_BIT);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature resolution 14bit.\n");
    res = hdc2080_get_temperature_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get temperature resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature resolution %s.\n", resolution == HDC2080_RESOLUTION_14_BIT ? "ok" : "error");
    
    /* set temperature resolution 11bit */
    res = hdc2080_set_temperature_resolution(&gs_handle, HDC2080_RESOLUTION_11_BIT);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature resolution 11bit.\n");
    res = hdc2080_get_temperature_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get temperature resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature resolution %s.\n", resolution == HDC2080_RESOLUTION_11_BIT ? "ok" : "error");
    
    /* set temperature resolution 9bit */
    res = hdc2080_set_temperature_resolution(&gs_handle, HDC2080_RESOLUTION_9_BIT);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature resolution 9bit.\n");
    res = hdc2080_get_temperature_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get temperature resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature resolution %s.\n", resolution == HDC2080_RESOLUTION_9_BIT ? "ok" : "error");
    
    /* hdc2080_set_humidity_resolution/hdc2080_get_humidity_resolution test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_humidity_resolution/hdc2080_get_humidity_resolution test.\n");
    
    /* set humidity resolution 14bit */
    res = hdc2080_set_humidity_resolution(&gs_handle, HDC2080_RESOLUTION_14_BIT);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity resolution 14bit.\n");
    res = hdc2080_get_humidity_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get humidity resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature resolution %s.\n", resolution == HDC2080_RESOLUTION_14_BIT ? "ok" : "error");
    
    /* set humidity resolution 11bit */
    res = hdc2080_set_humidity_resolution(&gs_handle, HDC2080_RESOLUTION_11_BIT);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity resolution 11bit.\n");
    res = hdc2080_get_humidity_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get humidity resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature resolution %s.\n", resolution == HDC2080_RESOLUTION_11_BIT ? "ok" : "error");
    
    /* set humidity resolution 9bit */
    res = hdc2080_set_humidity_resolution(&gs_handle, HDC2080_RESOLUTION_9_BIT);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity resolution 9bit.\n");
    res = hdc2080_get_humidity_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get humidity resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature resolution %s.\n", resolution == HDC2080_RESOLUTION_9_BIT ? "ok" : "error");
    
    /* hdc2080_set_mode/hdc2080_get_mode test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_mode/hdc2080_get_mode test.\n");
    
    /* set humidity temperature mode */
    res = hdc2080_set_mode(&gs_handle, HDC2080_MODE_HUMIDITY_TEMPERATURE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity temperature mode.\n");
    res = hdc2080_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check mode %s.\n", mode == HDC2080_MODE_HUMIDITY_TEMPERATURE ? "ok" : "error");
    
    /* set temperature only mode */
    res = hdc2080_set_mode(&gs_handle, HDC2080_MODE_TEMPERATURE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature only mode.\n");
    res = hdc2080_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check mode %s.\n", mode == HDC2080_MODE_TEMPERATURE ? "ok" : "error");
    
    /* hdc2080_set_measurement/hdc2080_get_measurement test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_measurement/hdc2080_get_measurement test.\n");
    
    /* enable measurement */
    res = hdc2080_set_measurement(&gs_handle, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set measurement failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: enable measurement.\n");
    res = hdc2080_get_measurement(&gs_handle, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get measurement failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check measurement %s.\n", res == 0 ? "ok" : "error");
    
    /* disable measurement */
    res = hdc2080_set_measurement(&gs_handle, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set measurement failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: disable measurement.\n");
    res = hdc2080_get_measurement(&gs_handle, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get measurement failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check measurement %s.\n", res == 0 ? "ok" : "error");
    
    /* hdc2080_soft_reset test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_soft_reset test.\n");
    
    /* soft reset */
    res = hdc2080_soft_reset(&gs_handle);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: soft reset failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check soft reset %s.\n", res == 0 ? "ok" : "error");
    
    /* hdc2080_set_auto_measurement_mode/hdc2080_get_auto_measurement_mode test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_auto_measurement_mode/hdc2080_get_auto_measurement_mode test.\n");
    
    /* set auto measurement mode disable */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_DISABLED);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set auto mode disable.\n");
    res = hdc2080_get_auto_measurement_mode(&gs_handle, &auto_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check auto mode %s.\n", auto_mode == HDC2080_AUTO_MEASUREMENT_MODE_DISABLED ? "ok" : "error");
    
    /* set auto measurement mode 1/120 hz */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_120_HZ);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set auto mode 1/120 hz.\n");
    res = hdc2080_get_auto_measurement_mode(&gs_handle, &auto_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check auto mode %s.\n", auto_mode == HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_120_HZ ? "ok" : "error");
    
    /* set auto measurement mode 1/60 hz */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_60_HZ);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set auto mode 1/60 hz.\n");
    res = hdc2080_get_auto_measurement_mode(&gs_handle, &auto_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check auto mode %s.\n", auto_mode == HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_60_HZ ? "ok" : "error");
    
    /* set auto measurement mode 1/10 hz */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_10_HZ);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set auto mode 1/10 hz.\n");
    res = hdc2080_get_auto_measurement_mode(&gs_handle, &auto_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check auto mode %s.\n", auto_mode == HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_10_HZ ? "ok" : "error");
    
    /* set auto measurement mode 1/5 hz */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_5_HZ);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set auto mode 1/5 hz.\n");
    res = hdc2080_get_auto_measurement_mode(&gs_handle, &auto_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check auto mode %s.\n", auto_mode == HDC2080_AUTO_MEASUREMENT_MODE_1_DIV_5_HZ ? "ok" : "error");
    
    /* set auto measurement mode 1 hz */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_1_HZ);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set auto mode 1 hz.\n");
    res = hdc2080_get_auto_measurement_mode(&gs_handle, &auto_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check auto mode %s.\n", auto_mode == HDC2080_AUTO_MEASUREMENT_MODE_1_HZ ? "ok" : "error");
    
    /* set auto measurement mode 2 hz */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_2_HZ);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set auto mode 2 hz.\n");
    res = hdc2080_get_auto_measurement_mode(&gs_handle, &auto_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check auto mode %s.\n", auto_mode == HDC2080_AUTO_MEASUREMENT_MODE_2_HZ ? "ok" : "error");
    
    /* set auto measurement mode 5 hz */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_5_HZ);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set auto mode 5 hz.\n");
    res = hdc2080_get_auto_measurement_mode(&gs_handle, &auto_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check auto mode %s.\n", auto_mode == HDC2080_AUTO_MEASUREMENT_MODE_5_HZ ? "ok" : "error");
    
    /* hdc2080_set_heater/hdc2080_get_heater test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_heater/hdc2080_get_heater test.\n");

    /* enable heater */
    res = hdc2080_set_heater(&gs_handle, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set heater failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: enable heater.\n");
    res = hdc2080_get_heater(&gs_handle, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get heater failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check heater %s.\n", enable == HDC2080_BOOL_TRUE ? "ok" : "error");
    
    /* disable heater */
    res = hdc2080_set_heater(&gs_handle, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set heater failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: disable heater.\n");
    res = hdc2080_get_heater(&gs_handle, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get heater failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check heater %s.\n", enable == HDC2080_BOOL_FALSE ? "ok" : "error");
    
    /* hdc2080_set_interrupt_pin/hdc2080_get_interrupt_pin test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_interrupt_pin/hdc2080_get_interrupt_pin test.\n");
    
    /* enable interrupt pin */
    res = hdc2080_set_interrupt_pin(&gs_handle, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt pin failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: enable interrupt pin.\n");
    res = hdc2080_get_interrupt_pin(&gs_handle, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt pin failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt pin %s.\n", enable == HDC2080_BOOL_TRUE ? "ok" : "error");
    
    /* disable interrupt pin */
    res = hdc2080_set_interrupt_pin(&gs_handle, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt pin failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: disable interrupt pin.\n");
    res = hdc2080_get_interrupt_pin(&gs_handle, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt pin failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt pin %s.\n", enable == HDC2080_BOOL_FALSE ? "ok" : "error");
    
    /* hdc2080_set_interrupt_polarity/hdc2080_get_interrupt_polarity test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_interrupt_polarity/hdc2080_get_interrupt_polarity test.\n");
    
    /* set interrupt polarity low */
    res = hdc2080_set_interrupt_polarity(&gs_handle, HDC2080_INTERRUPT_POLARITY_LOW);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt polarity failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set interrupt polarity low.\n");
    res = hdc2080_get_interrupt_polarity(&gs_handle, &polarity);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt polarity failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt polarity pin %s.\n", polarity == HDC2080_INTERRUPT_POLARITY_LOW ? "ok" : "error");
    
    /* set interrupt polarity high */
    res = hdc2080_set_interrupt_polarity(&gs_handle, HDC2080_INTERRUPT_POLARITY_HIGH);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt polarity failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set interrupt polarity high.\n");
    res = hdc2080_get_interrupt_polarity(&gs_handle, &polarity);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt polarity failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt polarity pin %s.\n", polarity == HDC2080_INTERRUPT_POLARITY_HIGH ? "ok" : "error");
    
    /* hdc2080_set_interrupt_mode/hdc2080_get_interrupt_mode test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_interrupt_mode/hdc2080_get_interrupt_mode test.\n");
    
    /* set interrupt mode level */
    res = hdc2080_set_interrupt_mode(&gs_handle, HDC2080_INTERRUPT_MODE_LEVEL);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set interrupt mode level.\n");
    res = hdc2080_get_interrupt_mode(&gs_handle, &interrupt_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt mode %s.\n", interrupt_mode == HDC2080_INTERRUPT_MODE_LEVEL ? "ok" : "error");
    
    /* set interrupt mode comparator */
    res = hdc2080_set_interrupt_mode(&gs_handle, HDC2080_INTERRUPT_MODE_COMPARATOR);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set interrupt mode comparator.\n");
    res = hdc2080_get_interrupt_mode(&gs_handle, &interrupt_mode);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt mode %s.\n", interrupt_mode == HDC2080_INTERRUPT_MODE_COMPARATOR ? "ok" : "error");
    
    /* hdc2080_set_humidity_high_threshold/hdc2080_get_humidity_high_threshold test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_humidity_high_threshold/hdc2080_get_humidity_high_threshold test.\n");
    
    reg = rand() % 256;
    res = hdc2080_set_humidity_high_threshold(&gs_handle,reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity high threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity high threshold 0x%02X.\n", reg);
    res = hdc2080_get_humidity_high_threshold(&gs_handle, &reg_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get humidity high threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check humidity high threshold %s.\n", reg_check == reg ? "ok" : "error");
    
    /* hdc2080_set_humidity_low_threshold/hdc2080_get_humidity_low_threshold test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_humidity_low_threshold/hdc2080_get_humidity_low_threshold test.\n");
    
    reg = rand() % 256;
    res = hdc2080_set_humidity_low_threshold(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity low threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity low threshold 0x%02X.\n", reg);
    res = hdc2080_get_humidity_low_threshold(&gs_handle, &reg_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get humidity low threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check humidity low threshold %s.\n", reg_check == reg ? "ok" : "error");
    
    /* hdc2080_set_temperature_high_threshold/hdc2080_get_temperature_high_threshold test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_temperature_high_threshold/hdc2080_get_temperature_high_threshold test.\n");
    
    reg = rand() % 256;
    res = hdc2080_set_temperature_high_threshold(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature high threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature high threshold 0x%02X.\n", reg);
    res = hdc2080_get_temperature_high_threshold(&gs_handle, &reg_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get temperature high threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature high threshold %s.\n", reg_check == reg ? "ok" : "error");
    
    /* hdc2080_set_temperature_low_threshold/hdc2080_get_temperature_low_threshold test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_temperature_low_threshold/hdc2080_get_temperature_low_threshold test.\n");
    
    reg = rand() % 256;
    res = hdc2080_set_temperature_low_threshold(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature low threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature low threshold 0x%02X.\n", reg);
    res = hdc2080_get_temperature_low_threshold(&gs_handle, &reg_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get temperature low threshold failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature low threshold %s.\n", reg_check == reg ? "ok" : "error");
    
    /* hdc2080_set_humidity_offset_adjustment/hdc2080_get_humidity_offset_adjustment test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_humidity_offset_adjustment/hdc2080_get_humidity_offset_adjustment test.\n");
    
    offset = rand() % 128;
    res = hdc2080_set_humidity_offset_adjustment(&gs_handle, offset);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity offset adjustment failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity offset adjustment %d.\n", offset);
    res = hdc2080_get_humidity_offset_adjustment(&gs_handle, &offset_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get humidity offset adjustment failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check humidity offset adjustment %s.\n", offset_check == offset ? "ok" : "error");
    
    /* hdc2080_set_temperature_offset_adjustment/hdc2080_get_temperature_offset_adjustment test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_temperature_offset_adjustment/hdc2080_get_temperature_offset_adjustment test.\n");
    
    offset = -(rand() % 128);
    res = hdc2080_set_temperature_offset_adjustment(&gs_handle, offset);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature offset adjustment failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature offset adjustment %d.\n", offset);
    res = hdc2080_get_temperature_offset_adjustment(&gs_handle, &offset_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get temperature offset adjustment failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature offset adjustment %s.\n", offset_check == offset ? "ok" : "error");
    
    /* hdc2080_set_interrupt/hdc2080_get_interrupt test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_interrupt/hdc2080_get_interrupt test.\n");
    
    /* enable data ready */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_DRDY, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: enable data ready.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_DRDY, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_TRUE ? "ok" : "error");
    
    /* disable data ready */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_DRDY, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: disable data ready.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_DRDY, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_FALSE ? "ok" : "error");
    
    /* enable temperature high threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_HIGH_THRESHOLD, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: enable temperature high threshold.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_HIGH_THRESHOLD, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_TRUE ? "ok" : "error");
    
    /* disable temperature high threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_HIGH_THRESHOLD, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: disable temperature high threshold.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_HIGH_THRESHOLD, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_FALSE ? "ok" : "error");
    
    /* enable temperature low threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_LOW_THRESHOLD, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: enable temperature low threshold.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_LOW_THRESHOLD, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_TRUE ? "ok" : "error");
    
    /* disable temperature low threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_LOW_THRESHOLD, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: disable temperature low threshold.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_LOW_THRESHOLD, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_FALSE ? "ok" : "error");
    
    /* enable humidity high threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_HIGH_THRESHOLD, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: enable humidity high threshold.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_HIGH_THRESHOLD, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_TRUE ? "ok" : "error");
    
    /* disable humidity high threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_HIGH_THRESHOLD, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: disable humidity high threshold.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_HIGH_THRESHOLD, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_FALSE ? "ok" : "error");
    
    /* enable humidity low threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_LOW_THRESHOLD, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: enable humidity low threshold.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_LOW_THRESHOLD, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_TRUE ? "ok" : "error");
    
    /* disable humidity low threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_LOW_THRESHOLD, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: disable humidity low threshold.\n");
    res = hdc2080_get_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_LOW_THRESHOLD, &enable);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check interrupt %s.\n", enable == HDC2080_BOOL_FALSE ? "ok" : "error");
    
    /* hdc2080_set_humidity_max/hdc2080_get_humidity_max test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_humidity_max/hdc2080_get_humidity_max test.\n");
    
    reg = 0;
    res = hdc2080_set_humidity_max(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set humidity max failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity max 0x%02X.\n", reg);
    res = hdc2080_get_humidity_max(&gs_handle, &reg_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get humidity max failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check humidity max %s.\n", reg_check == reg ? "ok" : "error");
    
    /* hdc2080_set_temperature_max/hdc2080_get_temperature_max test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_set_temperature_max/hdc2080_get_temperature_max test.\n");
    
    reg = 0;
    res = hdc2080_set_temperature_max(&gs_handle, reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature max failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature max 0x%02X.\n", reg);
    res = hdc2080_get_temperature_max(&gs_handle, &reg_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get temperature max failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature max %s.\n", reg_check == reg ? "ok" : "error");
    
    /* hdc2080_get_interrupt_status test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_get_interrupt_status test.\n");
    
    /* get interrupt status */
    res = hdc2080_get_interrupt_status(&gs_handle, &reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get interrupt status failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: status is 0x%02x.\n", reg);
    
    /* hdc2080_humidity_convert_to_register/hdc2080_humidity_convert_to_data test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_humidity_convert_to_register/hdc2080_humidity_convert_to_data test.\n");
    
    f = (float)(rand() % 300) / 10.0f;
    res = hdc2080_humidity_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: humidity convert to register failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity %0.2f.\n", f);
    res = hdc2080_humidity_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: humidity convert to data failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check humidity %0.2f.\n", f_check);
    
    /* hdc2080_temperature_convert_to_register/hdc2080_temperature_convert_to_data test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_temperature_convert_to_register/hdc2080_temperature_convert_to_data test.\n");
    
    f = (float)(rand() % 100) / 10.0f;
    res = hdc2080_temperature_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: temperature convert to register failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature %0.2f.\n", f);
    res = hdc2080_temperature_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: temperature convert to data failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature %0.2f.\n", f_check);
   
    /* hdc2080_humidity_offset_convert_to_register/hdc2080_humidity_offset_convert_to_data test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_humidity_offset_convert_to_register/hdc2080_humidity_offset_convert_to_data test.\n");
    
    f = (float)(rand() % 100) / 10.0f;
    res = hdc2080_humidity_offset_convert_to_register(&gs_handle, f, &offset);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: humidity offset convert to register failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set humidity offset %0.2f.\n", f);
    res = hdc2080_humidity_offset_convert_to_data(&gs_handle, offset, &f_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: humidity offset convert to data failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check humidity offset %0.2f.\n", f_check);
    
    /* hdc2080_temperature_offset_convert_to_register/hdc2080_temperature_offset_convert_to_data test */
    hdc2080_interface_debug_print("hdc2080: hdc2080_temperature_offset_convert_to_register/hdc2080_temperature_offset_convert_to_data test.\n");
    
    f = (float)(rand() % 100) / 10.0f;
    res = hdc2080_temperature_offset_convert_to_register(&gs_handle, f, &offset);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: temperature offset convert to register failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    hdc2080_interface_debug_print("hdc2080: set temperature offset %0.2f.\n", f);
    res = hdc2080_temperature_offset_convert_to_data(&gs_handle, offset, &f_check);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: temperature offset convert to data failed.\n");
        (void)hdc2080_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    hdc2080_interface_debug_print("hdc2080: check temperature offset %0.2f.\n", f_check);
    
    /* finish register */
    hdc2080_interface_debug_print("hdc2080: finish register test.\n");
    (void)hdc2080_deinit(&gs_handle);
    
    return 0;
}
