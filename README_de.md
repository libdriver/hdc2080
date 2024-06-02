[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver HDC2080
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/hdc2080/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Das HDC2080-Gerät ist ein integrierter Feuchtigkeits- und Temperatursensor, der hochgenaue Messungen bei sehr geringem Stromverbrauch in einem kleinen DFN-Gehäuse ermöglicht. Der kapazitive Sensor enthält neue integrierte digitale Funktionen und ein Heizelement zum Ableiten von Kondensation und Feuchtigkeit. Zu den digitalen Funktionen des HDC2080 gehören programmierbare Interrupt-Schwellenwerte, um Warnungen und Systemaktivierungen bereitzustellen, ohne dass ein Mikrocontroller das System kontinuierlich überwachen muss. In Kombination mit programmierbaren Abtastintervallen, einem geringen Stromverbrauch und einer Unterstützung für eine Versorgungsspannung von 1,8 V ist der HDC2080 für batteriebetriebene Systeme konzipiert. Der HDC2080 bietet hochgenaue Messfunktionen für eine breite Palette von Umweltüberwachungs- und Internet of Things (IoT)-Anwendungen wie intelligente Thermostate und Smart Home-Assistenten. Für Designs, bei denen die Leiterplattenfläche (PCB) kritisch ist, ist über den HDC2010 eine kleinere CSP-Gehäuseoption mit vollständiger Softwarekompatibilität mit dem HDC2080 verfügbar.

LibDriver HDC2080 ist der voll funktionsfähige Treiber von HDC2080, der von LibDriver eingeführt wurde. Er bietet die Funktion zum Lesen von Temperatur, relativer Luftfeuchtigkeit und eine Interrupt-Funktion. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver HDC2080-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver HDC2080 IIC.

/test enthält den Testcode des LibDriver HDC2080-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver HDC2080-Beispielcode.

/doc enthält das LibDriver HDC2080-Offlinedokument.

/Datenblatt enthält HDC2080-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/hdc2080/index.html](https://www.libdriver.com/docs/hdc2080/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.