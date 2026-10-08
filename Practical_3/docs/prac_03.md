# Practical 3 — Voltage Mirror: ADC and DAC

**University of Cape Town**  
**Department of Electrical Engineering**  
**EEE3096S · 2026**

Build a voltage mirror: read an analogue input with the ADC and reproduce it
with the DAC. Press SW0 to switch between a full-resolution output and a coarse,
64-level output. Use measurements to explain the steps you see.

By the end you should be able to configure the ADC and DAC through registers,
map converter codes correctly, and distinguish resolution from voltage accuracy.

## What is supplied

The [starter project](../starter/README.md) supplies the clock setup, board GPIO,
LCD, debounced buttons, and a fixed **2 kHz sampling timer**. ADC clock startup,
self-calibration and enable helpers are supplied too. The ADC and DAC both stay
at **12-bit hardware resolution**. You implement four short functions in
[voltage_mirror.c](../starter/Core/Src/voltage_mirror.c):

| Function | Your work |
| --- | --- |
| `voltage_mirror_init` | Configure the ADC fields and enable the buffered DAC |
| `voltage_mirror_read` | Start one ADC conversion and return its actual result |
| `voltage_mirror_write` | Write a bounded 12-bit DAC command |
| `voltage_mirror_map` | Convert the ADC code into the selected output command |

Use direct register access for the ADC and DAC. Read the TODO comments and use
CMSIS register names. Consult the ADC and DAC chapters of
[RM0091](https://www.st.com/resource/en/reference_manual/rm0091-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf).
Keep clock, LCD and button infrastructure as supplied.

## Equipment and connections

Use the UCT STM32F051 board, STM32CubeIDE or the supplied Makefile, a signal
generator, a digital multimeter, a two-channel oscilloscope and jumper leads.
The confirmed target is an **STM32F051C6 with 32 KiB flash and 8 KiB RAM**.

| Connection | Board signal |
| --- | --- |
| Generator output and scope CH1 | PA5, ADC channel 5, expansion header |
| DMM or scope CH2 | PA4, DAC1 output, HAT contact 1 |
| Instrument returns and scope grounds | Board GND |
| Mode selection | SW0 |
| Programming/debugging | Preserve PA13 and PA14 |

Check labels against the [board reference](../references/Board.md). MCU package
pin numbers are not header-contact numbers. Keep the EEPROM deselected and
remove the P2 loopback bridges; board support handles the other GPIO settings.

Wire with power off. Measure the board's 3V3 analogue reference with the DMM and
record it as \(V_R\). Verify the reference measurement point before probing it.
Keep PA5 between GND and the measured analogue supply; use interior voltages
away from both rails. Configure the generator for **High Z**, then verify its
actual amplitude and offset on the scope before connecting it to PA5. Use
high-impedance scope inputs, common ground, and matched 10× probe settings.

## Predict the two outputs

Let \(C\) be the acquired ADC code and \(D\) the DAC command, both in 0–4095.

| Mode | Mapping | Output commands |
| --- | --- | --- |
| FULL12 | \(D=C\) | Every integer from 0 to 4095 |
| COARSE6 | \(D=64\lfloor C/64\rfloor\) | 0, 64, 128, …, 4032 |

COARSE6 discards the six least significant bits **in software**. The DAC still
accepts 12-bit commands; its hardware resolution has not changed.

Use these nominal models for predictions:

\[
V_{\mathrm{ADC,estimate}}=C\frac{V_R}{4096},\qquad
V_{\mathrm{DAC,ideal}}=D\frac{V_R}{4096}.
\]

The predicted coarse output step is

\[
\Delta V_{\mathrm{coarse}}=64\frac{V_R}{4096}=\frac{V_R}{64}.
\]

For an illustrative 3.300 V reference, this is **51.5625 mV**. Calculate your
prediction with your measured reference. A converter's number of levels and
highest code differ: there are 4096 ADC codes, but the highest code is 4095.
Both nominal converter intervals use 4096, so copying the code gives a nominal
unity-voltage transfer. Real converter offsets, gain and loading errors, noise
and reference uncertainty can produce differences. A shared-reference mirror
alone cannot establish either converter's absolute accuracy.

Record your prediction before taking the output measurements in the
[results sheet](results.md).

## Implement and run

1. Read the four function TODOs and complete them. Select channel 5, right-aligned
   12-bit single conversions and a fixed 239.5-cycle acquisition setting. Follow
   the supplied helper's initialization order. Enable DAC channel 1 with its
   output buffer and software writes.
2. In the conversion functions, handle the relevant status flags correctly and
   keep returned codes and written commands within 0–4095. In the mapping
   function, implement the two relationships above without changing ADC resolution.
3. Build and program using the [starter instructions](../starter/README.md).
   Keep the C6 linker configuration. Reset and check that the LCD shows
   `FULL12` or `COARSE6`, plus the actual `Cxxxx` and `Dxxxx` codes.
4. Press and release SW0 to change mode. If the LCD control mapping is reversed
   on your board, hold SW3 during reset to select the alternate mapping. Check
   LCD contrast before changing driver code.

The supplied timer invokes the conversion path every 0.5 ms. LCD updates happen
separately. You do not need to develop a timer, menu system or calibration routine.

## Measure and explain

**DC check.** Apply approximately 0.800, 1.650 and 2.500 V, provided each lies
inside your measured supply and the DAC's usable interior range. Measure the
actual input at PA5 with the DMM. For each input, record the LCD's actual ADC code
and DAC command, then measure PA4 with the DMM, in both modes. Fill the six rows
in the results sheet. Nominal settings are examples; enter your own readings.

Check that FULL12 gives \(D=C\), and COARSE6 gives a multiple of 64 no greater
than \(C\). Compare measured output voltages with the nominal prediction. Small
code variations are possible even with a steady input; record representative
readings after they settle.

**Triangle check.** Set a **2 Hz triangle, 1.7 V peak-to-peak, 1.65 V offset**.
Its nominal limits are 0.8–2.5 V. Verify the real limits before connecting PA5
and reduce them if needed for your measured supply. Put CH1 on PA5 and CH2 on
PA4, both DC coupled.

Save one FULL12 capture with both traces over about one cycle; 50 ms/div is a
useful starting point. Switch to COARSE6, zoom until adjacent output plateaus
are visible, and save a second capture with both channels labelled. Use voltage
cursors on settled adjacent plateaus to measure the output step. Compare it
with your prediction. FULL12's individual steps may be too small for the scope
to resolve; a visually smooth trace still comes from discrete codes.

## Hand in and demonstrate

Assessment is **80 marks for the live demonstration and 20 marks for the report**.
Submit your four completed functions and a short report using the
[results sheet](results.md) or [LaTeX template](report_template.tex)
([blank PDF](report_template.pdf)). The report contains the measurement table,
two labelled scope captures and **at most one page of written explanation**.

Prepare all three DC levels beforehand. The **5–8 minute demonstration** uses
one representative safe DC input selected by the tutor, followed by the triangle:

1. **Setup and switching (10 marks):** identify PA5, PA4 and common ground;
   show verified safe generator limits and use SW0 to switch modes.
2. **DC mirror (25 marks):** independently measure PA5 and PA4 with the DMM.
   Show the real LCD codes in both modes: FULL12 has \(D=C\); COARSE6 has a
   multiple of 64 no greater than \(C\). Relate the output to the input.
3. **Triangle and steps (25 marks):** show live PA5/PA4 traces first in FULL12,
   then COARSE6. Use cursors on adjacent settled plateaus and compare their
   spacing with your measured-reference prediction \(V_R/64\).
4. **Understanding (20 marks):** explain ADC channel/resolution/acquisition and
   DAC enable/buffer/trigger fields, conversion start/result, bounded wait and
   write-one-to-clear flags. Explain
   software quantisation, 12-bit hardware versus 6-bit effective output,
   and resolution versus accuracy.

The **20 report marks** cover measured reference, six DC rows, predictions and
step comparison (**8**); two real, labelled scope captures (**6**); and the
concise explanations in the results sheet (**6**).
