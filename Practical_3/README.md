# EEE3096S Practical 3 — Voltage Mirror

University of Cape Town · Department of Electrical Engineering · 2026

Build a system that reads an input voltage with the ADC and reproduces it with
the DAC. Use SW0 to switch between **FULL12** and **COARSE6**, then measure how
discarding six bits changes the output. Both hardware converters remain 12-bit.

Assessment is **80% live demonstration and 20% report**. Start with the
[practical manual](docs/prac_03.md), also available as a
[printable PDF](docs/prac_03.pdf).

## Where you write your code

**Complete all four numbered TODOs in
[starter/Core/Src/voltage_mirror.c](starter/Core/Src/voltage_mirror.c).**
This is the only firmware file you need to edit.

In STM32CubeIDE's Project Explorer, open
**Practical3 → Core → Src → voltage_mirror.c**.

| TODO | Function | What you implement |
| --- | --- | --- |
| 1 | `voltage_mirror_init()` | Configure the ADC and DAC |
| 2 | `voltage_mirror_read()` | Start a conversion and return the acquired ADC code |
| 3 | `voltage_mirror_write()` | Limit the command to the valid range and write it to the DAC |
| 4 | `voltage_mirror_map()` | Implement the FULL12 and COARSE6 mappings |

The supplied `main.c`, `board_io.c` and `sampling.c` handle startup, the LCD,
buttons and sampling timing, and call your completed functions.

## Where everything is

| File or folder | What you use it for |
| --- | --- |
| [docs/prac_03.md](docs/prac_03.md) / [prac_03.pdf](docs/prac_03.pdf) | Task, connections, measurement procedure and demo marking criteria |
| [starter/](starter/README.md) | Complete STM32CubeIDE project, GNU Arm Makefile, supplied drivers and build instructions |
| [starter/Core/Src/voltage_mirror.c](starter/Core/Src/voltage_mirror.c) | Code skeleton: the four TODOs you complete |
| [docs/results.md](docs/results.md) | Record your reference voltage, predictions, DC readings and scope observations |
| [docs/report_template.tex](docs/report_template.tex) / [report_template.pdf](docs/report_template.pdf) | LaTeX report template and blank preview |
| [references/Board.md](references/Board.md) | Board pinout and connection reference; the folder also contains lecture slides |

## Get started

1. Read the manual and [starter instructions](starter/README.md). Use
   **STM32CubeIDE**, or **GNU Arm GCC and Make** for the command-line build.
2. Import and build the project before editing it. The supplied skeleton builds,
   but shows `Check TODOs` and does not acquire measurements until the converter
   functions are completed.
3. Complete the four TODOs in `starter/Core/Src/voltage_mirror.c`: configure the
   ADC/DAC, acquire an ADC reading, write a bounded DAC command and map the code
   for both modes. This is the only firmware file you need to edit. Clock setup,
   GPIO, LCD, button handling and the fixed 2 kHz sampling timer are supplied.
4. Connect the input to **PA5 (ADC channel 5)** and measure the output at
   **PA4 (DAC1)**, with a common ground. Follow the manual's checks for supply
   voltage, generator limits and probe settings before applying a signal.
5. Build, program and run your completed code. Measure **three DC input levels
   in both modes** (six table rows), then take **two labelled scope captures**:
   FULL12 over approximately one triangle cycle, and COARSE6 with adjacent
   plateaus and a cursor measurement of the step.

The target is **STM32F051C6: 32 KiB Flash and 8 KiB SRAM**. Keep the supplied C6
startup and linker configuration. The board reference also covers the
pin-compatible C8 variant.

## Report and demonstration

Submit your completed converter code and a report containing your measured
reference, predictions, six DC rows, two real scope captures and **at most one
page of written explanation**. Use the results sheet or the LaTeX template.
For LaTeX, copy the template to `report.tex`, place your own `full12.png` and
`coarse6.png` beside it, and compile with your LaTeX editor or
`pdflatex report.tex`. The template PDF shows the layout with blank entries.

For the **5–8 minute live demonstration**, prepare all three DC levels. Show
one safe input selected by the tutor, the actual ADC/DAC codes in both modes,
both triangle traces and a measured coarse step. Be ready to explain your
register choices, conversion handling, quantisation, and resolution versus
accuracy. The [manual's demonstration checklist](docs/prac_03.md#hand-in-and-demonstrate)
gives the sequence and the 80-mark breakdown; the report contributes 20 marks.
