[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver HDC2080

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/hdc2080/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

HDC2080器件是一款採用小型DFN封裝的集成式濕度和溫度感測器，能够以超低功耗提供高準確度量測。 這款電容式感測器包括新的集成數位功能和用於消散冷凝和濕氣的加熱元件。 HDC2080數位功能包括可程式設計中斷閾值，能够提供警報和系統喚醒，無需微控制器持續對系統進行監控。 HDC2080還具有可程式設計採樣間隔，功耗較低，並且支持1.8V電源電壓，非常適合電池供電型系統。 HDC2080為各種環境監測和物聯網（IoT）應用（如智慧恒溫器和智能家居助理）提供高準確度量測功能。 對於印刷電路板（PCB）區域較小的設計，可以通過使用與HDC2080完全軟件相容的HDC2010獲得較小的晶片級封裝（CSP）選項。

LibDriver HDC2080是LibDriver推出的HDC2080全功能驅動，該驅動提供溫度、相對濕度讀取和中斷等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver HDC2080的源文件。

/interface目錄包含了LibDriver HDC2080與平台無關的IIC總線模板。

/test目錄包含了LibDriver HDC2080驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver HDC2080編程範例。

/doc目錄包含了LibDriver HDC2080離線文檔。

/datasheet目錄包含了HDC2080數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/hdc2080/index.html](https://www.libdriver.com/docs/hdc2080/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
