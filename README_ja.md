[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver HDC2080

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/hdc2080/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

HDC2080 デバイスは、小型の DFN パッケージで非常に低い消費電力で高精度の測定を提供する統合湿度および温度センサーです。静電容量ベースのセンサーには、新しい統合デジタル機能と、結露や湿気を消散させる加熱要素が含まれています。HDC2080 のデジタル機能には、マイクロコントローラがシステムを継続的に監視しなくても警告やシステムのウェイクアップを提供するためのプログラム可能な割り込みしきい値が含まれています。プログラム可能なサンプリング間隔、低消費電力、および 1.8 V 電源電圧のサポートを組み合わせた HDC2080 は、バッテリー駆動システム向けに設計されています。HDC2080 は、スマート サーモスタットやスマート ホーム アシスタントなどの幅広い環境監視およびモノのインターネット (IoT) アプリケーションに高精度の測定機能を提供します。プリント回路基板 (PCB) 領域が重要な設計の場合、HDC2080 との完全なソフトウェア互換性を備えた HDC2010 を通じて、より小型の CSP パッケージ オプションを利用できます。

LibDriver HDC2080 は、LibDriver がリリースした HDC2080 のフル機能ドライバーです。温度、相対湿度の読み取り機能と割り込み機能を提供します。LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver HDC2080のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver HDC2080用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver HDC2080ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver HDC2080プログラミング例が含まれています。

/ docディレクトリには、LibDriver HDC2080オフラインドキュメントが含まれています。

/ datasheetディレクトリには、HDC2080データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/hdc2080/index.html](https://www.libdriver.com/docs/hdc2080/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。