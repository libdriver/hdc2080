[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver HDC2080

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/hdc2080/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

HDC2080 器件是一款采用小型 DFN 封装的集成式湿度和温度传感器，能够以超低功耗提供高准确度测量。 这款电容式传感器包括新的集成数字功能和用于消散冷凝和湿气的加热元件。HDC2080 数字功能包括可编程中断阈值，能够提供警报和系统唤醒，无需微控制器持续对系统进行监控。HDC2080 还具有可编程采样间隔，功耗较低，并且支持 1.8V 电源电压， 非常适合电池供电型系统。HDC2080 为各种环境监测和物联网 (IoT) 应用（如智 能恒温器和智能家居助理）提供高准确度测量功能。对于印刷电路板 (PCB) 区域较小的设计，可以通过使用 与 HDC2080 完全软件兼容的 HDC2010 获得较小的芯 片级封装 (CSP) 选项。

LibDriver HDC2080是LibDriver推出的HDC2080全功能驱动，该驱动提供温度、相对湿度读取和中断等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver HDC2080的源文件。

/interface目录包含了LibDriver HDC2080与平台无关的IIC总线模板。

/test目录包含了LibDriver HDC2080驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver HDC2080编程范例。

/doc目录包含了LibDriver HDC2080离线文档。

/datasheet目录包含了HDC2080数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/hdc2080/index.html](https://www.libdriver.com/docs/hdc2080/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。