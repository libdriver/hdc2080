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
 * @file      driver_hdc2080_interrupt_test.c
 * @brief     driver hdc2080 interrupt test source file
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

#include "driver_hdc2080_interrupt_test.h"

static hdc2080_handle_t gs_handle;        /**< hdc2080 handle */

/**
 * @brief     interrupt test
 * @param[in] addr_pin is the address pin
 * @param[in] high_deg is the temperature high threshold
 * @param[in] low_deg is the temperature low threshold
 * @param[in] high_percent is the humidity high threshold
 * @param[in] low_percent is the humidity low threshold
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 read failed
 * @note      none
 */
uint8_t hdc2080_interrupt_test(hdc2080_address_t addr_pin, float high_deg, float low_deg,
                               float high_percent, float low_percent, uint32_t times)
{
    uint8_t res;
    uint8_t reg;
    int8_t offset;
    uint32_t i = 0;
    hdc2080_info_t info;

    /* link interface function */
    DRIVER_HDC2080_LINK_INIT(&gs_handle, hdc2080_handle_t);
    DRIVER_HDC2080_LINK_IIC_INIT(&gs_handle, hdc2080_interface_iic_init);
    DRIVER_HDC2080_LINK_IIC_DEINIT(&gs_handle, hdc2080_interface_iic_deinit);
    DRIVER_HDC2080_LINK_IIC_READ(&gs_handle, hdc2080_interface_iic_read);
    DRIVER_HDC2080_LINK_IIC_WRITE(&gs_handle, hdc2080_interface_iic_write);
    DRIVER_HDC2080_LINK_DELAY_MS(&gs_handle, hdc2080_interface_delay_ms);
    DRIVER_HDC2080_LINK_DEBUG_PRINT(&gs_handle, hdc2080_interface_debug_print);

    /* get hdc2080 information */
    res = hdc2080_info(&info);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: get info failed.\n");

        return 1;
    }
    else
    {
        /* print hdc2080 information */
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

    /* start interrupt test */
    hdc2080_interface_debug_print("hdc2080: start interrupt test.\n");

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

    /* set temperature resolution 14bits */
    res = hdc2080_set_temperature_resolution(&gs_handle, HDC2080_RESOLUTION_14_BIT);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set temperature resolution failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* set humidity resolution 14bits */
    res = hdc2080_set_humidity_resolution(&gs_handle, HDC2080_RESOLUTION_14_BIT);
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

    /* disable heater */
    res = hdc2080_set_heater(&gs_handle, HDC2080_BOOL_FALSE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set heater failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* enable interrupt pin */
    res = hdc2080_set_interrupt_pin(&gs_handle, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt pin failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* set interrupt polarity low */
    res = hdc2080_set_interrupt_polarity(&gs_handle, HDC2080_INTERRUPT_POLARITY_LOW);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt polarity failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* set comparator mode */
    res = hdc2080_set_interrupt_mode(&gs_handle, HDC2080_INTERRUPT_MODE_COMPARATOR);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* set high threshold */
    res = hdc2080_humidity_convert_to_register(&gs_handle, high_percent, &reg);
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

    /* set low threshold */
    res = hdc2080_humidity_convert_to_register(&gs_handle, low_percent, &reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: humidity convert to register failed.\n");
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

    /* set high threshold */
    res = hdc2080_temperature_convert_to_register(&gs_handle, high_deg, &reg);
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

    /* set low threshold */
    res = hdc2080_temperature_convert_to_register(&gs_handle, low_deg, &reg);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: temperature convert to register failed.\n");
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

    /* 0.0 */
    res = hdc2080_humidity_offset_convert_to_register(&gs_handle, 0.0f, &offset);
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

    /* 0.0 */
    res = hdc2080_temperature_offset_convert_to_register(&gs_handle, 0.0f, &offset);
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

    /* enable temperature high threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_HIGH_THRESHOLD, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* enable temperature low threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_TEMPERATURE_LOW_THRESHOLD, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* enable humidity high threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_HIGH_THRESHOLD, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* enable humidity low threshold */
    res = hdc2080_set_interrupt(&gs_handle, HDC2080_INTERRUPT_HUMIDITY_LOW_THRESHOLD, HDC2080_BOOL_TRUE);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set interrupt failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* disable auto measurement mode */
    res = hdc2080_set_auto_measurement_mode(&gs_handle, HDC2080_AUTO_MEASUREMENT_MODE_DISABLED);
    if (res != 0)
    {
        hdc2080_interface_debug_print("hdc2080: set auto measurement mode failed.\n");
        (void)hdc2080_deinit(&gs_handle);

        return 1;
    }

    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;

        /* read poll */
        res = hdc2080_read_poll(&gs_handle);
        if (res != 0)
        {
            hdc2080_interface_debug_print("hdc2080: read poll failed.\n");
            (void)hdc2080_deinit(&gs_handle);

            return 1;
        }

        /* read temperature humidity */
        res = hdc2080_read_temperature_humidity(&gs_handle, &temperature_raw, &temperature_s, &humidity_raw, &humidity_s);
        if (res != 0)
        {
            hdc2080_interface_debug_print("hdc2080: read temperature humidity failed.\n");
            (void)hdc2080_deinit(&gs_handle);

            return 1;
        }

        /* output */
        hdc2080_interface_debug_print("hdc2080: temperature[%d] is %0.2fC.\n", i, temperature_s);
        hdc2080_interface_debug_print("hdc2080: humidity[%d] is %0.2f%%.\n", i, humidity_s);
        if (temperature_s > high_deg)
        {
            hdc2080_interface_debug_print("hdc2080: temperature is over high threshold and check the interrupt pin.\n");
        }
        if (temperature_s < low_deg)
        {
            hdc2080_interface_debug_print("hdc2080: temperature is less than low threshold and check the interrupt pin.\n");
        }
        if (humidity_s > high_percent)
        {
            hdc2080_interface_debug_print("hdc2080: humidity is over high threshold and check the interrupt pin.\n");
        }
        if (humidity_s < low_percent)
        {
            hdc2080_interface_debug_print("hdc2080: humidity is less than low threshold and check the interrupt pin.\n");
        }

        /* delay 1000ms */
        hdc2080_interface_delay_ms(1000);
    }

    /* finish interrupt test and exit */
    hdc2080_interface_debug_print("hdc2080: finish interrupt test.\n");
    (void)hdc2080_deinit(&gs_handle);

    return 0;
}
