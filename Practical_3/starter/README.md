# Voltage Mirror — student starter

The [handout](../docs/prac_03.md) is the laboratory procedure. The starter
compiles as supplied, but deliberately stops at `Check TODOs` until the converter
functions are implemented. It does not display fabricated ADC measurements.

## Where you write your code

**Complete all four numbered TODOs in
[Core/Src/voltage_mirror.c](Core/Src/voltage_mirror.c). This is the only source
file you need to edit for this practical.**

In STM32CubeIDE's **Project Explorer**, open
**Practical3 → Core → Src → voltage_mirror.c**. Search that file for `TODO` to
find each function you must complete:

| TODO | Function | Your implementation |
|---|---|---|
| 1 | `voltage_mirror_init()` | Configure the ADC and DAC. |
| 2 | `voltage_mirror_read()` | Acquire a real ADC reading, with a bounded wait. |
| 3 | `voltage_mirror_write()` | Limit the DAC command to 0–4095 and write it. |
| 4 | `voltage_mirror_map()` | Implement the FULL12 and COARSE6 mappings. |

`main.c`, `board_io.c` and `sampling.c` are supplied infrastructure: they handle
startup, the LCD, buttons and fixed sampling timing, and call your functions.
You do not need to edit these files.

## STM32CubeIDE

1. Choose **File → Import → General → Existing Projects into Workspace**.
2. Select this `starter` directory and import **Practical3**. If the previous
   Practical3 is already in your workspace, remove that workspace entry first,
   leaving **Delete project contents on disk** unchecked.
3. Build **Debug**, then use the supplied **Practical3 Debug** ST-LINK/SWD
   configuration to program and run the connected board.

CubeIDE produces `Debug/Practical3.elf` or `Release/Practical3.elf`. Import the
project directly; CubeMX regeneration would overwrite supplied infrastructure.
The `.ioc` is a clock/SWD reference, not a generator for the completed practical.

## Command-line build

With GNU Arm GCC and Make installed, run from the practical root:

```sh
make -C starter -j4
make -C starter -j4 CONFIG=Release
```

Outputs are `starter/build/Debug/student/Practical3.{elf,hex,bin}` or the
equivalent Release directory. For a toolchain outside PATH, add
`GCC_PATH=/path/to/bin`. These commands build firmware; they do not program it.

### Optional checks on your computer

After completing the TODOs, run the following from the practical root if a host
C compiler (`cc`) is installed:

```sh
make -C starter test
```

This checks your mapping across all 4096 ADC codes, out-of-range arguments and
bounded DAC writes using mock registers. It is expected to fail for the
uncompleted skeleton. It cannot check physical ADC/DAC behaviour, LCD wiring,
sampling rate or analogue accuracy; verify those on the board during the lab.

## Supplied infrastructure

| File | Purpose |
|---|---|
| `main.c` | 8 MHz crystal → 48 MHz clock; foreground display and mode switch |
| `board_io.c` | LCD, 20 ms button debounce, converter clocks/GPIO and ADC startup |
| `sampling.c` | Fixed 2 kHz TIM2 interrupt and coherent live display snapshot |
| `voltage_mirror.c` | Student ADC/DAC configuration, read/write and coarse mapping |

SW0 switches **FULL12** / **COARSE6**. Both converter peripherals remain 12-bit.
The LCD shows the actual acquired ADC code (`C`) and written DAC command (`D`).
If the LCD is blank, adjust POT2 contrast; holding SW3 during reset tries the
alternate PC14/PC15 control assignment. Release it after reset.

Use the C6 startup and `STM32F051C6TX_FLASH.ld`: **32 KiB Flash, 8 KiB SRAM**.
`STM32F051x8` is ST's shared CMSIS family symbol, not a 64 KiB linker selection.
Keep SWD on PA13/PA14 and EEPROM chip-select PB12 high. This project needs no
UART, DMA, PWM, external filter or calibration menu.
