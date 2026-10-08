# UCT Dev Board — STM32F051C6/C8 Hardware Reference

This markdown document provides a detailed pinout mapping, hardware sub-circuit breakdown, and functional description of the **UCT Dev Board** based on the provided KiCad schematic.

Practical 3 uses the confirmed **STM32F051C6 target: 32 KiB Flash and 8 KiB SRAM**.
The original schematic reference below names the pin-compatible C8 variant;
this does not change the C6 linker memory limits. The attached ST-LINK/V2 does
not provide a virtual COM port. The new practical uses the LCD and buttons.

The LCD table and subsection now both use **PC14 = RS, PC15 = E**. This is the
supplied driver's default; hold SW3 during reset to try the alternate wiring
on a board with reversed control lines. Confirm physical operation at the bench.

## 1. Project Metadata

- **Title:** UCT Dev Board
    
- **Institution:** University of Cape Town
    
- **Original Designer:** James Gowans
    
- **Revision Author:** Justin Pead
    
- **Date:** 2025-01-06
    
- **Revision:** v1.06
    
- **EDA Tool:** KiCad E.D.A. 9.0.7
    

## 2. Comprehensive Pinout Reference Table (Pins 1–48)

The table below describes the function and connections of all 48 pins on the LQFP48 package of the **STM32F051C8** microcontroller.

| **Pin** | **Pin Name**     | **Hardware Subsystem / Net** | **Description & Connections**                                                                                                                                         |
| ------- | ---------------- | ---------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **1**   | `VBAT`           | Power (`3V3`)                | Device backup domain power supply. Tied directly to the board's main `3V3` rail.                                                                                      |
| **2**   | `PC13`           | External Pin Header (`P1`)   | General-purpose I/O broken out to the main expansion pin header.                                                                                                      |
| **3**   | `PC14-OSC32_IN`  | LCD Control (`RS`) / Header  | Connected to **NMOS Level Shifter 2** (`PC14` $\rightarrow$ `PC14_S`) to drive the **LCD Screen Character Register Select (RS)** pin. Also broken out to Header `P1`. |
| **4**   | `PC15-OSC32_OUT` | LCD Control (`E`) / Header   | Connected to **NMOS Level Shifter 1** (`PC15` $\rightarrow$ `PC15_S`) to drive the **LCD Screen Enable (E)** pin. Also broken out to Header `P1`.                     |
| **5**   | `PF0-OSC_IN`     | Main Clock (`OSC_IN`)        | Connected to the 8MHz Main Crystal oscillator circuit (`XTAL`) with a 10pF stability capacitor (`C11`) to GND.                                                        |
| **6**   | `PF1-OSC_OUT`    | Main Clock (`OSC_OUT`)       | Connected to the 8MHz Main Crystal oscillator circuit (`XTAL`) with a 10pF stability capacitor (`C12`) to GND.                                                        |
| **7**   | `NRST`           | System Reset                 | Active-low reset pin connected to the physical tactile **Reset Switch**, the onboard ST-Link programming block (`T_NRST`), and Header `P1`.                           |
| **8**   | `VSSA`           | Analog Ground                | Tied directly to the main system ground (`GND`) plane.                                                                                                                |
| **9**   | `VDDA`           | Analog Power Supply          | Analog circuitry power rail. Tied to the `3V3` system voltage line.                                                                                                   |
| **10**  | `PA0`            | Switches Block (`SW0`)       | Connected to tactile push-button switch `SW0` (pulls to GND when pressed).                                                                                            |
| **11**  | `PA1`            | Switches Block (`SW1`)       | Connected to tactile push-button switch `SW1` (pulls to GND when pressed).                                                                                            |
| **12**  | `PA2`            | Switches Block (`SW2`)       | Connected to tactile push-button switch `SW2` (pulls to GND when pressed).                                                                                            |
| **13**  | `PA3`            | Switches Block (`SW3`)       | Connected to tactile push-button switch `SW3` (pulls to GND when pressed).                                                                                            |
| **14**  | `PA4`            | `DAC1` Output / HAT          | Digital-to-Analog converter channel 1 output. Labeled `DAC1` and routed to Pin 1 of the **HAT Connector**.                                                            |
| **15**  | `PA5`            | External Pin Header (`P1`)   | General-purpose I/O broken out to expansion header `P1`.                                                                                                              |
| **16**  | `PA6`            | External Pin Header (`P1`)   | General-purpose I/O broken out to expansion header `P1`.                                                                                                              |
| **17**  | `PA7`            | Loopback Header (`P2`)       | Routed to Loopback Header `P2` (Pin 5), designed to couple with `PB13`.                                                                                               |
| **18**  | `PB0`            | User LED 0 / Pot Jumper      | Drives individual Red LED `D1` via a 150$\Omega$ resistor. Also routes to the 2x2 Jumper Header to optionally accept analog input from **POT0**.                      |
| **19**  | `PB1`            | User LED 1                   | Drives individual Red LED `D2` via a 150$\Omega$ resistor to GND.                                                                                                     |
| **20**  | `PB2`            | User LED 2                   | Drives individual Red LED `D3` via a 150$\Omega$ resistor to GND.                                                                                                     |
| **21**  | `PB10`           | Red/Green Status LED         | Labeled `PB10`. Controls the **Red** channel of the R/G status LED assembly via a 150$\Omega$ series resistor (`R12`).                                                |
| **22**  | `PB11`           | Red/Green Status LED         | Labeled `PB11`. Controls the **Green** channel of the R/G status LED assembly via a 150$\Omega$ series resistor (`R13`).                                              |
| **23**  | `VSS`            | Digital Ground               | Primary digital return path connected to the global ground plane (`GND`).                                                                                             |
| **24**  | `VDD`            | Digital Power Supply         | Main digital core supply input tied to the filtered `3V3` power rail.                                                                                                 |
| **25**  | `PB12`           | EEPROM Chip Select (`CS`)    | Master-controlled SPI Chip Select line. Hardwired to Pin 1 (`CS`) of the `CAT25040LI-G` EEPROM IC.                                                                    |
| **26**  | `PB13`           | EEPROM `SO` / Loopback       | Connected to Pin 2 (`SO` / MISO) of the SPI EEPROM. Also routes to Loopback Jumper Header `P2` (Pin 6) to connect with `PA7`.                                         |
| **27**  | `PB14`           | EEPROM `SI` / Loopback       | Connected to Pin 5 (`SI` / MOSI) of the SPI EEPROM. Also routes to Loopback Jumper Header `P2` (Pin 4) to connect with `PA11`.                                        |
| **28**  | `PB15`           | EEPROM `SCK` / Loopback      | Connected to Pin 6 (`SCK`) of the SPI EEPROM. Also routes to Loopback Jumper Header `P2` (Pin 2) to connect with `PA8`.                                               |
| **29**  | `PA8`            | Loopback Header (`P2`)       | General-purpose I/O tied to Loopback Header `P2` (Pin 1) to form a pair with `PB15`.                                                                                  |
| **30**  | `PA9`            | ST-Link UART (`VCP_TX`)      | Configured as USART1 Transmit. Interfaces with the ST-Link V2.1 MCU section as the Virtual COM Port TX line (`VCP_TX`).                                               |
| **31**  | `PA10`           | ST-Link UART (`VCP_RX`)      | Configured as USART1 Receive. Interfaces with the ST-Link V2.1 MCU section as the Virtual COM Port RX line (`VCP_RX`).                                                |
| **32**  | `PA11`           | Loopback Header (`P2`)       | General-purpose I/O tied to Loopback Header `P2` (Pin 3) to form a pair with `PB14`.                                                                                  |
| **33**  | `PA12`           | LCD Data Bit 6               | Connected to **NMOS Level Shifter 5** (`PA12` $\rightarrow$ `PA12_S`) to drive character **LCD Data Line 6 (D6)**.                                                    |
| **34**  | `PA13`           | SWD Debug Data (`SWDIO`)     | Hardware Serial Wire Debug Data I/O line connected directly to the onboard **ST-Link V2.1** debugger.                                                                 |
| **35**  | `PF6`            | `HAT Connector` Pin 5        | Low-pin count GPIO variant routed natively to Pin 5 of the 10-pin expansion HAT connector.                                                                            |
| **36**  | `PF7`            | `HAT Connector` Pin 3        | Low-pin count GPIO variant routed natively to Pin 3 of the 10-pin expansion HAT connector.                                                                            |
| **37**  | `PA14`           | SWD Debug Clock (`SWCLK`)    | Hardware Serial Wire Debug Clock line connected directly to the onboard **ST-Link V2.1** debugger.                                                                    |
| **38**  | `PA15`           | LCD Data Bit 7               | Connected to **NMOS Level Shifter 6** (`PA15` $\rightarrow$ `PA15_S`) to drive character **LCD Data Line 7 (D7)**.                                                    |
| **39**  | `PB3`            | Byte of LEDs (`D3`)          | Part of the output byte block; drives individual Red LED `D3` via a 150$\Omega$ resistor.                                                                             |
| **40**  | `PB4`            | Byte of LEDs (`D4`)          | Part of the output byte block; drives individual Red LED `D4` via a 150$\Omega$ resistor.                                                                             |
| **41**  | `PB5`            | Byte of LEDs (`D5`)          | Part of the output byte block; drives individual Red LED `D5` via a 150$\Omega$ resistor.                                                                             |
| **42**  | `PB6`            | Byte of LEDs (`D6`)          | Part of the output byte block; drives individual Red LED `D6` via a 150$\Omega$ resistor.                                                                             |
| **43**  | `PB7`            | Byte of LEDs (`D7`)          | Part of the output byte block; drives individual Red LED `D7` via a 150$\Omega$ resistor.                                                                             |
| **44**  | `BOOT0`          | Boot Mode Configuration      | Boot control pin tied to a 10k$\Omega$ pulldown resistor (`R11`) to ensure the MCU starts execution from main user Flash memory by default.                           |
| **45**  | `PB8`            | LCD Data Bit 4               | Connected to **NMOS Level Shifter 3** (`PB8` $\rightarrow$ `PB8_S`) to drive character **LCD Data Line 4 (D4)**.                                                      |
| **46**  | `PB9`            | LCD Data Bit 5               | Connected to **NMOS Level Shifter 4** (`PB9` $\rightarrow$ `PB9_S`) to drive character **LCD Data Line 5 (D5)**.                                                      |
| **47**  | `VSS`            | Digital Ground               | Core electrical ground return tied to the primary system plane.                                                                                                       |
| **48**  | `VDD`            | Digital Power Supply         | Main digital core supply input tied to the filtered `3V3` power rail.                                                                                                 |

## 3. Hardware Subsystems Deep-Dive

### A. Power Supply & Regulation Block

- **Input Sources:** Power is drawn primarily via the standard **USB Connector** (Molex 61729-0010BLF) which provides a raw `5V_USB` rail. An external power terminal block (`JP2` Header 2) is available for standalone operation.
    
- **Voltage Regulator:** An `MCP1702-3302E/TO` Low-Dropout (LDO) linear regulator down-converts the raw 5V input down to a stabilized `3V3` system rail.
    
- **Decoupling Network:** The board integrates an extensive array of filtering capacitors to guarantee clean voltage supply inputs across all frequencies:
    
    - Bulk Input Filtering: `C1` ($10\,\mu\text{F}$) and `C2` ($1\,\mu\text{F}$).
        
    - Core Output Smoothing: `C3` ($5\,\mu\text{F}$).
        
    - High-Frequency Noise Suppression: Parallel network containing capacitors `C4`, `C5`, `C6`, and `C7` (all $100\,\text{nF}$) distributed across supply rails.
        
- **Visual Indication:** A green **Power LED** (`D8`) is connected in series with a $150\,\Omega$ resistor (`R10`) directly across the `3V3` output track to confirm operational voltage status.
    

### B. User Interface Elements (Buttons & LEDs)

- **The "Byte of LEDs":** A unique design feature of the UCT Dev Board is the provision of an entire continuous 8-bit output byte mapped entirely onto **Port B**.
    
    - Lower Bits (`PB0`, `PB1`, `PB2`) route to individual indicator LEDs (`D1`, `D2`, `D3`).
        
    - Upper Bits (`PB3`, `PB4`, `PB5`, `PB6`, `PB7`) drive the remaining 5 LEDs in the cluster assembly.
        
    - Every individual LED incorporates a $150\,\Omega$ current-limiting resistor to guarantee uniform brightness.
        
- **Push-Button Switches:** Four uncommitted tactile input pushbuttons are grouped as the **Switches** block, mapped to pins `PA0` through `PA3`. These interface lines are hardwired to pull down directly to `GND` when depressed.
    
- **Analog Interface (Pots + Jumper):** An on-board $1\,\text{k}\Omega$ linear potentiometer (**POT0**) serves as a clean adjustable analog voltage source. Using the 2x2 header layout (`Header 2X2`), a user can install a physical jumper block to route the variable analog voltage signal directly to `PB0` for ADC conversion testing.
    

### C. Alphanumeric LCD Character Screen Interface

The board integrates a `WC1602A` character LCD screen module powered by an independent `LCD_PWR` track. Because standard character screens operate at standard 5V logic thresholds while the STM32F0 operates at 3V3, the design includes **six separate bidirectional N-Channel MOSFET Level Shifters**.

- **Shifter Architecture:** Each of the 6 isolated shifters relies on a discrete NMOS transistor (e.g., `Q2` through `Q6` across schematic sheets) matched with dual $1.8\,\text{k}\Omega$ pullup resistors tied between the low-voltage side (`3V3`) and the high-voltage side (`5V`).
    
- **Control Configuration:** `PC14` shifts to `PC14_S` $\rightarrow$ Drives **RS (Register Select)**.
    
    - `PC15` shifts to `PC15_S` $\rightarrow$ Drives **E (Enable Clock)**.
        
    - The **R/W (Read/Write)** track is permanently pulled low (`GND`), locking the module into Write-Only mode to optimize pin constraints.
        
- **Data Configuration:** To maximize system efficiency, the screen runs in compact **4-bit Nibble Mode** using high-speed data lines 4 to 7:
    
    - `PB8` shifts to drive **DB4**
        
    - `PB9` shifts to drive **DB5**
        
    - `PA12` shifts to drive **DB6**
        
    - `PA15` shifts to drive **DB7**
        
- **Contrast Potentiometer:** A dedicated trimmer potentiometer (**POT2**, $10\,\text{k}\Omega$) interfaces with the LCD contrast pin (`V0`) to handle character clarity calibration.
    

### D. Integrated SPI EEPROM Storage

For non-volatile memory development, the board mounts a `CAT25040LI-G` 4-Kilobit SPI EEPROM IC:

- **Pin 1 (`CS`):** Driven by `PB12`.
    
- **Pin 2 (`SO`):** Main MISO out, mapped to `PB13`.
    
- **Pin 5 (`SI`):** Main MOSI in, mapped to `PB14`.
    
- **Pin 6 (`SCK`):** Bus Clock input, mapped to `PB15`.
    
- **Loopback Validation Header (`P2`):** The 2x4 jumper header layout (`Conn_02x04_Odd_Even`) provides quick bus manipulation. A programmer can bridge these contacts to link the active EEPROM SPI pins directly to alternative GPIO nodes (`PA7`, `PA11`, `PA8`) for diagnostic signal tracing or peripheral separation.
    

### E. Onboard ST-Link V2.1 Programmer/Debugger

The topmost section features an integrated **ST-Link V2.1** circuitry subsystem that removes the need for external hardware debug tools.

- **SWD Target Connection:** Connects directly to the target via the standard Serial Wire Debug framework: `T_SWCLK` to Pin 37 (`PA14`) and `T_SWDIO` to Pin 34 (`PA13`).
    
- **USART routing:** The schematic labels PA9/PA10 for serial signals. The attached ST-LINK/V2 has no virtual COM port; serial communication would need a separate adapter. The voltage-mirror practical does not use UART.