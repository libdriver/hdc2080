### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. HDC2080

#### 3.1 Command Instruction

1. Show hdc2080 chip and driver information.

   ```shell
   hdc2080 (-i | --information)
   ```

2. Show hdc2080 help.

   ```shell
   hdc2080 (-h | --help)
   ```

3. Show hdc2080 pin connections of the current board.

   ```shell
   hdc2080 (-p | --port)
   ```

4. Run hdc2080 register test.

    ```shell
    hdc2080 (-t reg | --test=reg) [--addr=<0 | 1>]
    ```

5. Run hdc2080 read test, num means test times.

    ```shell
    hdc2080 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
    ```

6. Run hdc2080 interrupt test, deg is the temperature threshold, percent is the humidity percent threshold, num means test times.

   ```shell
   hdc2080 (-t int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--temperature-high=<deg>] [--temperature-low=<deg>] [--humidity-high=<percent>] [--humidity-low=<percent>]
   ```

7. Run hdc2080 read function, num means test times.

   ```shell
   hdc2080 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
   ```

8. Run hdc2080 shot function, num means test times.

   ```shell
   hdc2080 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]
   ```
   
9. Run hdc2080 interrupt function, deg is the temperature threshold, percent is the humidity percent threshold, num means test times.

   ```shell
   hdc2080 (-e int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--temperature-high=<deg>] [--temperature-low=<deg>] [--humidity-high=<percent>] [--humidity-low=<percent>]
   ```

#### 3.2 Command Example

```shell
hdc2080 -i

hdc2080: chip is Texas Instruments HDC2080.
hdc2080: manufacturer is Texas Instruments.
hdc2080: interface is IIC.
hdc2080: driver version is 1.0.
hdc2080: min supply voltage is 1.6V.
hdc2080: max supply voltage is 3.6V.
hdc2080: max current is 90.00mA.
hdc2080: max temperature is 125.0C.
hdc2080: min temperature is -40.0C.
```

```shell
hdc2080 -p

hdc2080: SCL connected to GPIOB PIN8.
hdc2080: SDA connected to GPIOB PIN9.
```

```shell
hdc2080 -t reg --addr=0

hdc2080: chip is Texas Instruments HDC2080.
hdc2080: manufacturer is Texas Instruments.
hdc2080: interface is IIC.
hdc2080: driver version is 1.0.
hdc2080: min supply voltage is 1.6V.
hdc2080: max supply voltage is 3.6V.
hdc2080: max current is 90.00mA.
hdc2080: max temperature is 125.0C.
hdc2080: min temperature is -40.0C.
hdc2080: start register test.
hdc2080: hdc2080_set_addr_pin/hdc2080_get_addr_pin test.
hdc2080: set addr pin 0.
hdc2080: check addr pin ok.
hdc2080: set addr pin 1.
hdc2080: check addr pin ok.
hdc2080: hdc2080_set_temperature_resolution/hdc2080_get_temperature_resolution test.
hdc2080: set temperature resolution 14bit.
hdc2080: check temperature resolution ok.
hdc2080: set temperature resolution 11bit.
hdc2080: check temperature resolution ok.
hdc2080: set temperature resolution 9bit.
hdc2080: check temperature resolution ok.
hdc2080: hdc2080_set_humidity_resolution/hdc2080_get_humidity_resolution test.
hdc2080: set humidity resolution 14bit.
hdc2080: check temperature resolution ok.
hdc2080: set humidity resolution 11bit.
hdc2080: check temperature resolution ok.
hdc2080: set humidity resolution 9bit.
hdc2080: check temperature resolution ok.
hdc2080: hdc2080_set_mode/hdc2080_get_mode test.
hdc2080: set humidity temperature mode.
hdc2080: check mode ok.
hdc2080: set temperature only mode.
hdc2080: check mode ok.
hdc2080: hdc2080_set_measurement/hdc2080_get_measurement test.
hdc2080: enable measurement.
hdc2080: check measurement ok.
hdc2080: disable measurement.
hdc2080: check measurement ok.
hdc2080: hdc2080_soft_reset test.
hdc2080: check soft reset ok.
hdc2080: hdc2080_set_auto_measurement_mode/hdc2080_get_auto_measurement_mode test.
hdc2080: set auto mode disable.
hdc2080: check auto mode ok.
hdc2080: set auto mode 1/120 hz.
hdc2080: check auto mode ok.
hdc2080: set auto mode 1/60 hz.
hdc2080: check auto mode ok.
hdc2080: set auto mode 1/10 hz.
hdc2080: check auto mode ok.
hdc2080: set auto mode 1/5 hz.
hdc2080: check auto mode ok.
hdc2080: set auto mode 1 hz.
hdc2080: check auto mode ok.
hdc2080: set auto mode 2 hz.
hdc2080: check auto mode ok.
hdc2080: set auto mode 5 hz.
hdc2080: check auto mode ok.
hdc2080: hdc2080_set_heater/hdc2080_get_heater test.
hdc2080: enable heater.
hdc2080: check heater ok.
hdc2080: disable heater.
hdc2080: check heater ok.
hdc2080: hdc2080_set_interrupt_pin/hdc2080_get_interrupt_pin test.
hdc2080: enable interrupt pin.
hdc2080: check interrupt pin ok.
hdc2080: disable interrupt pin.
hdc2080: check interrupt pin ok.
hdc2080: hdc2080_set_interrupt_polarity/hdc2080_get_interrupt_polarity test.
hdc2080: set interrupt polarity low.
hdc2080: check interrupt polarity pin ok.
hdc2080: set interrupt polarity high.
hdc2080: check interrupt polarity pin ok.
hdc2080: hdc2080_set_interrupt_mode/hdc2080_get_interrupt_mode test.
hdc2080: set interrupt mode level.
hdc2080: check interrupt mode ok.
hdc2080: set interrupt mode comparator.
hdc2080: check interrupt mode ok.
hdc2080: hdc2080_set_humidity_high_threshold/hdc2080_get_humidity_high_threshold test.
hdc2080: set humidity high threshold 0xB0.
hdc2080: check humidity high threshold ok.
hdc2080: hdc2080_set_humidity_low_threshold/hdc2080_get_humidity_low_threshold test.
hdc2080: set humidity low threshold 0xCB.
hdc2080: check humidity low threshold ok.
hdc2080: hdc2080_set_temperature_high_threshold/hdc2080_get_temperature_high_threshold test.
hdc2080: set temperature high threshold 0x49.
hdc2080: check temperature high threshold ok.
hdc2080: hdc2080_set_temperature_low_threshold/hdc2080_get_temperature_low_threshold test.
hdc2080: set temperature low threshold 0x68.
hdc2080: check temperature low threshold ok.
hdc2080: hdc2080_set_humidity_offset_adjustment/hdc2080_get_humidity_offset_adjustment test.
hdc2080: set humidity offset adjustment 7.
hdc2080: check humidity offset adjustment ok.
hdc2080: hdc2080_set_temperature_offset_adjustment/hdc2080_get_temperature_offset_adjustment test.
hdc2080: set temperature offset adjustment -69.
hdc2080: check temperature offset adjustment ok.
hdc2080: hdc2080_set_interrupt/hdc2080_get_interrupt test.
hdc2080: enable data ready.
hdc2080: check interrupt ok.
hdc2080: disable data ready.
hdc2080: check interrupt ok.
hdc2080: enable temperature high threshold.
hdc2080: check interrupt ok.
hdc2080: disable temperature high threshold.
hdc2080: check interrupt ok.
hdc2080: enable temperature low threshold.
hdc2080: check interrupt ok.
hdc2080: disable temperature low threshold.
hdc2080: check interrupt ok.
hdc2080: enable humidity high threshold.
hdc2080: check interrupt ok.
hdc2080: disable humidity high threshold.
hdc2080: check interrupt ok.
hdc2080: enable humidity low threshold.
hdc2080: check interrupt ok.
hdc2080: disable humidity low threshold.
hdc2080: check interrupt ok.
hdc2080: hdc2080_set_humidity_max/hdc2080_get_humidity_max test.
hdc2080: set humidity max 0x00.
hdc2080: check humidity max ok.
hdc2080: hdc2080_set_temperature_max/hdc2080_get_temperature_max test.
hdc2080: set temperature max 0x00.
hdc2080: check temperature max ok.
hdc2080: hdc2080_get_interrupt_status test.
hdc2080: status is 0x00.
hdc2080: hdc2080_humidity_convert_to_register/hdc2080_humidity_convert_to_data test.
hdc2080: set humidity 6.00.
hdc2080: check humidity 5.86.
hdc2080: hdc2080_temperature_convert_to_register/hdc2080_temperature_convert_to_data test.
hdc2080: set temperature 6.70.
hdc2080: check temperature 6.55.
hdc2080: hdc2080_humidity_offset_convert_to_register/hdc2080_humidity_offest_convert_to_data test.
hdc2080: set humidity offset 0.70.
hdc2080: check humidity offset 0.60.
hdc2080: hdc2080_temperature_offset_convert_to_register/hdc2080_temperature_offest_convert_to_data test.
hdc2080: set temperature offset 6.40.
hdc2080: check temperature offset 6.40.
hdc2080: finish register test.
```

```shell
hdc2080 -t read --addr=0 --times=3

hdc2080: chip is Texas Instruments HDC2080.
hdc2080: manufacturer is Texas Instruments.
hdc2080: interface is IIC.
hdc2080: driver version is 1.0.
hdc2080: min supply voltage is 1.6V.
hdc2080: max supply voltage is 3.6V.
hdc2080: max current is 90.00mA.
hdc2080: max temperature is 125.0C.
hdc2080: min temperature is -40.0C.
hdc2080: start read test.
hdc2080: shot read test.
hdc2080: temperature[0] is 28.47C.
hdc2080: humidity[0] is 48.47%.
hdc2080: temperature[1] is 28.47C.
hdc2080: humidity[1] is 48.48%.
hdc2080: temperature[2] is 28.46C.
hdc2080: humidity[2] is 48.46%.
hdc2080: humidity max is 48.44%.
hdc2080: temperature max is 28.46C.
hdc2080: set auto measure 5 hz.
hdc2080: temperature[0] is 28.46C.
hdc2080: humidity[0] is 48.54%.
hdc2080: temperature[1] is 28.45C.
hdc2080: humidity[1] is 48.54%.
hdc2080: temperature[2] is 28.46C.
hdc2080: humidity[2] is 48.51%.
hdc2080: set auto measure 2 hz.
hdc2080: temperature[0] is 28.46C.
hdc2080: humidity[0] is 48.47%.
hdc2080: temperature[1] is 28.46C.
hdc2080: humidity[1] is 48.39%.
hdc2080: temperature[2] is 28.45C.
hdc2080: humidity[2] is 48.35%.
hdc2080: set auto measure 1 hz.
hdc2080: temperature[0] is 28.45C.
hdc2080: humidity[0] is 48.30%.
hdc2080: temperature[1] is 28.45C.
hdc2080: humidity[1] is 48.25%.
hdc2080: temperature[2] is 28.45C.
hdc2080: humidity[2] is 48.20%.
hdc2080: set auto measure 1/5 hz.
hdc2080: temperature[0] is 28.45C.
hdc2080: humidity[0] is 48.38%.
hdc2080: temperature[1] is 28.45C.
hdc2080: humidity[1] is 48.38%.
hdc2080: temperature[2] is 28.45C.
hdc2080: humidity[2] is 48.38%.
hdc2080: set auto measure 1/10 hz.
hdc2080: temperature[0] is 28.45C.
hdc2080: humidity[0] is 48.38%.
hdc2080: temperature[1] is 28.45C.
hdc2080: humidity[1] is 48.38%.
hdc2080: temperature[2] is 28.45C.
hdc2080: humidity[2] is 48.38%.
hdc2080: set auto measure 1/60 hz.
hdc2080: temperature[0] is 28.45C.
hdc2080: humidity[0] is 48.38%.
hdc2080: temperature[1] is 28.45C.
hdc2080: humidity[1] is 48.38%.
hdc2080: temperature[2] is 28.45C.
hdc2080: humidity[2] is 48.38%.
hdc2080: set auto measure 1/120 hz.
hdc2080: temperature[0] is 28.45C.
hdc2080: humidity[0] is 48.38%.
hdc2080: temperature[1] is 28.45C.
hdc2080: humidity[1] is 48.38%.
hdc2080: temperature[2] is 28.45C.
hdc2080: humidity[2] is 48.38%.
hdc2080: set resolution 11bits.
hdc2080: temperature[0] is 28.45C.
hdc2080: humidity[0] is 48.38%.
hdc2080: temperature[1] is 28.42C.
hdc2080: humidity[1] is 48.42%.
hdc2080: temperature[2] is 28.42C.
hdc2080: humidity[2] is 48.53%.
hdc2080: set resolution 9bits.
hdc2080: temperature[0] is 28.42C.
hdc2080: humidity[0] is 51.07%.
hdc2080: temperature[1] is 28.42C.
hdc2080: humidity[1] is 51.09%.
hdc2080: temperature[2] is 28.42C.
hdc2080: humidity[2] is 51.04%.
hdc2080: finish read test.
```

```shell
hdc2080 -t int --addr=0 --times=3 --temperature-high=30.0 --temperature-low=25.0 --humidity-high=45.0 --humidity-low=30.0

hdc2080: chip is Texas Instruments HDC2080.
hdc2080: manufacturer is Texas Instruments.
hdc2080: interface is IIC.
hdc2080: driver version is 1.0.
hdc2080: min supply voltage is 1.6V.
hdc2080: max supply voltage is 3.6V.
hdc2080: max current is 90.00mA.
hdc2080: max temperature is 125.0C.
hdc2080: min temperature is -40.0C.
hdc2080: start interrupt test.
hdc2080: temperature[0] is 28.55C.
hdc2080: humidity[0] is 48.18%.
hdc2080: humidity is over high threshold and check the interrupt pin.
hdc2080: temperature[1] is 28.55C.
hdc2080: humidity[1] is 48.19%.
hdc2080: humidity is over high threshold and check the interrupt pin.
hdc2080: temperature[2] is 28.55C.
hdc2080: humidity[2] is 48.18%.
hdc2080: humidity is over high threshold and check the interrupt pin.
hdc2080: finish interrupt test.
```

```shell
hdc2080 -e read --addr=0 --times=3

hdc2080: 1/3.
hdc2080: temperature is 30.25C.
hdc2080: humidity is 45.90%.
hdc2080: 2/3.
hdc2080: temperature is 30.21C.
hdc2080: humidity is 45.77%.
hdc2080: 3/3.
hdc2080: temperature is 30.19C.
hdc2080: humidity is 45.80%.
```

```shell
hdc2080 -e shot --addr=0 --times=3

hdc2080: 1/3.
hdc2080: temperature is 29.79C.
hdc2080: humidity is 47.55%.
hdc2080: 2/3.
hdc2080: temperature is 29.77C.
hdc2080: humidity is 47.57%.
hdc2080: 3/3.
hdc2080: temperature is 29.74C.
hdc2080: humidity is 47.63%.
```

```shell
hdc2080 -e int --addr=0 --times=3 --temperature-high=30.0 --temperature-low=25.0 --humidity-high=45.0 --humidity-low=30.0

hdc2080: 1/3.
hdc2080: temperature is 28.88C.
hdc2080: humidity is 48.33%.
hdc2080: humidity is over high threshold and check the interrupt pin.
hdc2080: 2/3.
hdc2080: temperature is 28.89C.
hdc2080: humidity is 48.01%.
hdc2080: humidity is over high threshold and check the interrupt pin.
hdc2080: 3/3.
hdc2080: temperature is 28.88C.
hdc2080: humidity is 47.85%.
hdc2080: humidity is over high threshold and check the interrupt pin.
```

```shell
hdc2080 -h

Usage:
  hdc2080 (-i | --information)
  hdc2080 (-h | --help)
  hdc2080 (-p | --port)
  hdc2080 (-t reg | --test=reg) [--addr=<0 | 1>]
  hdc2080 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
  hdc2080 (-t int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--temperature-high=<deg>] [--temperature-low=<deg>] [--humidity-high=<percent>] [--humidity-low=<percent>]
  hdc2080 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
  hdc2080 (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]
  hdc2080 (-e int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--temperature-high=<deg>] [--temperature-low=<deg>] [--humidity-high=<percent>] [--humidity-low=<percent>]

Options:
      --addr=<0 | 1>             Set the iic address.([default: 0])
  -e <read | shot | int>, --example=<read | shot | int>
                                 Run the driver example.
  -h, --help                     Show the help.
      --humidity-high=<percent>  Set the humidity high threshold.([default: 0.0])
      --humidity-low=<percent>   Set the humidity low threshold.([default: 0.0])
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
  -t <reg | read | int>, --test=<reg | read | int>
                                 Run the driver test.
      --temperature-high=<deg>   Set the temperature high threshold.([default: 0.0])
      --temperature-low=<deg>    Set the temperature low threshold.([default: 0.0])
      --times=<num>              Set the running times.([default: 3])
```

