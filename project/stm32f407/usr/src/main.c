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
 * @file      main.c
 * @brief     main source file
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
#include "driver_hdc2080_register_test.h"
#include "driver_hdc2080_read_test.h"
#include "driver_hdc2080_basic.h"
#include "driver_hdc2080_shot.h"
#include "driver_hdc2080_interrupt.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
volatile uint16_t g_len;   /**< uart buffer length */

/**
 * @brief     hdc2080 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t hdc2080(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"humidity-high", required_argument, NULL, 2},
        {"humidity-low", required_argument, NULL, 3},
        {"temperature-high", required_argument, NULL, 4},
        {"temperature-low", required_argument, NULL, 5},
        {"times", required_argument, NULL, 6},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    hdc2080_address_t addr_pin = HDC2080_ADDRESS_0;
    uint32_t times = 3;
    float humidity_high = 0.0f;
    float humidity_low = 0.0f;
    float temperature_high = 0.0f;
    float temperature_low = 0.0f;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* address */
            case 1 :
            {
                if (strcmp("0", optarg) == 0)
                {
                    addr_pin = HDC2080_ADDRESS_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr_pin = HDC2080_ADDRESS_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* humidity high */
            case 2 :
            {
                /* convert humidity high */
                humidity_high = atof(optarg);
                
                break;
            }
            
            /* humidity low */
            case 3 :
            {
                /* convert humidity low */
                humidity_low = atof(optarg);
                
                break;
            }
            
            /* temperature high */
            case 4 :
            {
                /* convert temperature high */
                temperature_high = atof(optarg);
                
                break;
            }
            
            /* temperature low */
            case 5 :
            {
                /* convert temperature low */
                temperature_low = atof(optarg);
                
                break;
            }
            
            /* running times */
            case 6 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (hdc2080_register_test(addr_pin) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (hdc2080_read_test(addr_pin, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        /* run interrupt test */
        if (hdc2080_interrupt_test(addr_pin,
                                   temperature_high,
                                   temperature_low,
                                   humidity_high,
                                   humidity_low,
                                   times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temperature;
        float humidity;
        
        /* basic init */
        res = hdc2080_basic_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 2000ms */
            hdc2080_interface_delay_ms(2000);
            
            /* read data */
            res = hdc2080_basic_read((float *)&temperature, (float *)&humidity);
            if (res != 0)
            {
                (void)hdc2080_basic_deinit();
                
                return 1;
            }
            
            /* output */
            hdc2080_interface_debug_print("hdc2080: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            hdc2080_interface_debug_print("hdc2080: temperature is %0.2fC.\n", temperature);
            hdc2080_interface_debug_print("hdc2080: humidity is %0.2f%%.\n", humidity); 
        }
        
        /* deinit */
        (void)hdc2080_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temperature;
        float humidity;
        
        /* shot init */
        res = hdc2080_shot_init(addr_pin);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 2000ms */
            hdc2080_interface_delay_ms(2000);
            
            /* read data */
            res = hdc2080_shot_read((float *)&temperature, (float *)&humidity);
            if (res != 0)
            {
                (void)hdc2080_shot_deinit();
                
                return 1;
            }
            
            /* output */
            hdc2080_interface_debug_print("hdc2080: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            hdc2080_interface_debug_print("hdc2080: temperature is %0.2fC.\n", temperature);
            hdc2080_interface_debug_print("hdc2080: humidity is %0.2f%%.\n", humidity); 
        }
        
        /* deinit */
        (void)hdc2080_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temperature;
        float humidity;
        
        /* interrupt init */
        res = hdc2080_interrupt_init(addr_pin,
                                     temperature_high,
                                     temperature_low,
                                     humidity_high,
                                     humidity_low);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 2000ms */
            hdc2080_interface_delay_ms(2000);
            
            /* read data */
            res = hdc2080_interrupt_read((float *)&temperature, (float *)&humidity);
            if (res != 0)
            {
                (void)hdc2080_interrupt_deinit();
                
                return 1;
            }
            
            /* output */
            hdc2080_interface_debug_print("hdc2080: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            hdc2080_interface_debug_print("hdc2080: temperature is %0.2fC.\n", temperature);
            hdc2080_interface_debug_print("hdc2080: humidity is %0.2f%%.\n", humidity); 
            
            if (temperature > temperature_high)
            {
                hdc2080_interface_debug_print("hdc2080: temperature is over high threshold and check the interrupt pin.\n");
            }
            if (temperature < temperature_low)
            {
                hdc2080_interface_debug_print("hdc2080: temperature is less than low threshold and check the interrupt pin.\n");
            }
            if (humidity > humidity_high)
            {
                hdc2080_interface_debug_print("hdc2080: humidity is over high threshold and check the interrupt pin.\n");
            }
            if (humidity < humidity_low)
            {
                hdc2080_interface_debug_print("hdc2080: humidity is less than low threshold and check the interrupt pin.\n");
            }
        }
        
        /* deinit */
        (void)hdc2080_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        hdc2080_interface_debug_print("Usage:\n");
        hdc2080_interface_debug_print("  hdc2080 (-i | --information)\n");
        hdc2080_interface_debug_print("  hdc2080 (-h | --help)\n");
        hdc2080_interface_debug_print("  hdc2080 (-p | --port)\n");
        hdc2080_interface_debug_print("  hdc2080 (-t reg | --test=reg) [--addr=<0 | 1>]\n");
        hdc2080_interface_debug_print("  hdc2080 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]\n");
        hdc2080_interface_debug_print("  hdc2080 (-t int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--temperature-high=<deg>] "); 
        hdc2080_interface_debug_print("[--temperature-low=<deg>] [--humidity-high=<percent>] [--humidity-low=<percent>]\n");
        hdc2080_interface_debug_print("  hdc2080 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]\n");
        hdc2080_interface_debug_print("  hdc2080 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]\n");
        hdc2080_interface_debug_print("  hdc2080 (-e int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--temperature-high=<deg>] ");
        hdc2080_interface_debug_print("[--temperature-low=<deg>] [--humidity-high=<percent>] [--humidity-low=<percent>]\n");
        hdc2080_interface_debug_print("\n");
        hdc2080_interface_debug_print("Options:\n");
        hdc2080_interface_debug_print("      --addr=<0 | 1>             Set the iic address.([default: 0])\n");
        hdc2080_interface_debug_print("  -e <read | shot | int>, --example=<read | shot | int>\n");
        hdc2080_interface_debug_print("                                 Run the driver example.\n");
        hdc2080_interface_debug_print("  -h, --help                     Show the help.\n");
        hdc2080_interface_debug_print("      --humidity-high=<percent>  Set the humidity high threshold.([default: 0.0])\n");
        hdc2080_interface_debug_print("      --humidity-low=<percent>   Set the humidity low threshold.([default: 0.0])\n");
        hdc2080_interface_debug_print("  -i, --information              Show the chip information.\n");
        hdc2080_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        hdc2080_interface_debug_print("  -t <reg | read | int>, --test=<reg | read | int>\n");
        hdc2080_interface_debug_print("                                 Run the driver test.\n");
        hdc2080_interface_debug_print("      --temperature-high=<deg>   Set the temperature high threshold.([default: 0.0])\n");
        hdc2080_interface_debug_print("      --temperature-low=<deg>    Set the temperature low threshold.([default: 0.0])\n");
        hdc2080_interface_debug_print("      --times=<num>              Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        hdc2080_info_t info;
        
        /* print hdc2080 information */
        hdc2080_info(&info);
        hdc2080_interface_debug_print("hdc2080: chip is %s.\n", info.chip_name);
        hdc2080_interface_debug_print("hdc2080: manufacturer is %s.\n", info.manufacturer_name);
        hdc2080_interface_debug_print("hdc2080: interface is %s.\n", info.interface);
        hdc2080_interface_debug_print("hdc2080: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        hdc2080_interface_debug_print("hdc2080: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        hdc2080_interface_debug_print("hdc2080: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        hdc2080_interface_debug_print("hdc2080: max current is %0.2fmA.\n", info.max_current_ma);
        hdc2080_interface_debug_print("hdc2080: max temperature is %0.1fC.\n", info.temperature_max);
        hdc2080_interface_debug_print("hdc2080: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        hdc2080_interface_debug_print("hdc2080: SCL connected to GPIOB PIN8.\n");
        hdc2080_interface_debug_print("hdc2080: SDA connected to GPIOB PIN9.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register hdc2080 function */
    shell_init();
    shell_register("hdc2080", hdc2080);
    uart_print("hdc2080: welcome to libdriver hdc2080.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("hdc2080: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("hdc2080: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("hdc2080: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("hdc2080: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("hdc2080: param is invalid.\n");
            }
            else
            {
                uart_print("hdc2080: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
