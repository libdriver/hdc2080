[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver HDC2080

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/hdc2080/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The HDC2080 device is an integrated humidity and temperature sensor that provides high accuracy measurements with very low power consumption in a small DFN package. The capacitive-based sensor includes new integrated digital features and a heating element to dissipate condensation and moisture. The HDC2080 digital features include programmable interrupt thresholds to provide alerts and system wake-ups without requiring a microcontroller to be continuously monitoring the system. Combined
with programmable sampling intervals, a low power consumption, and a support for a 1.8-V supply voltage, the HDC2080 is designed for battery operated systems.The HDC2080 provides high accuracy measurement capability for a wide range of environmental monitoring and Internet of Things (IoT) applications such as smart thermostats and smart home assistants. For designs where printed-circuit board (PCB) area is critical, a smaller CSP package option is available thru the HDC2010 with complete software compatibility with the HDC2080.

LibDriver HDC2080 is the full function driver of HDC2080 launched by LibDriver.It provides the function of reading temperature , relative humidity and interrupt function. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver HDC2080 source files.

/interface includes LibDriver HDC2080 IIC platform independent template.

/test includes LibDriver HDC2080 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver HDC2080 sample code.

/doc includes LibDriver HDC2080 offline document.

/datasheet includes HDC2080 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
uint8_t res;
uint32_t i;
float temperature;
float humidity;

/* basic init */
res = hdc2080_basic_init(HDC2080_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
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
    hdc2080_interface_debug_print("hdc2080: %d/%d.\n", (uint32_t)(i + 1), 3);
    hdc2080_interface_debug_print("hdc2080: temperature is %0.2fC.\n", temperature);
    hdc2080_interface_debug_print("hdc2080: humidity is %0.2f%%.\n", humidity); 
 
    ...
}

...
    
/* deinit */
(void)hdc2080_basic_deinit();

return 0;
```
#### example shot

```C
uint8_t res;
uint32_t i;
float temperature;
float humidity;

/* shot init */
res = hdc2080_shot_init(HDC2080_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
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
    hdc2080_interface_debug_print("hdc2080: %d/%d.\n", (uint32_t)(i + 1), 3);
    hdc2080_interface_debug_print("hdc2080: temperature is %0.2fC.\n", temperature);
    hdc2080_interface_debug_print("hdc2080: humidity is %0.2f%%.\n", humidity); 
    
    ...
}

...
    
/* deinit */
(void)hdc2080_shot_deinit();

return 0;
```
#### example interrupt

```C
uint8_t res;
uint32_t i;
float temperature;
float humidity;

/* interrupt init */
res = hdc2080_interrupt_init(HDC2080_ADDRESS_0,
                             28.0f,
                             20.0f,
                             50.0f,
                             30.0f);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
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
    hdc2080_interface_debug_print("hdc2080: %d/%d.\n", (uint32_t)(i + 1), 3);
    hdc2080_interface_debug_print("hdc2080: temperature is %0.2fC.\n", temperature);
    hdc2080_interface_debug_print("hdc2080: humidity is %0.2f%%.\n", humidity); 
    
    ...
        
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
    
    ...
}

...
    
/* deinit */
(void)hdc2080_interrupt_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/hdc2080/index.html](https://www.libdriver.com/docs/hdc2080/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.