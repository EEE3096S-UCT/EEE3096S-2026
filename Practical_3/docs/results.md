# Practical 3 — Results

Names/student numbers: ____________________  Date: ____________________

Use actual instrument readings. The suggested voltages are setup targets.
Complete the [single task](prac_03.md) and keep your explanations to one page.
Assessment is **live demonstration 80 marks; report 20 marks**. Use this sheet
or the [LaTeX report template](report_template.tex)
([blank PDF](report_template.pdf)). Add your two real scope captures.

## Reference, predictions and measurements — 8 report marks

- Measured analogue reference \(V_R\): __________ V
- DMM range/resolution: ____________________
- ADC nominal interval \(V_R/4096\): __________ mV
- DAC nominal interval \(V_R/4096\): __________ mV
- Predicted coarse output step \(V_R/64\): __________ mV

## Three DC inputs, two modes

Measure PA5 and PA4 independently with the DMM. Record the LCD's actual codes.
Calculate the nominal output prediction \(D V_R/4096\) using your recorded DAC
command. Include units; do not replace measured values with predictions.

| Input target | Mode | PA5 DMM (V) | ADC code C | DAC code D | Predicted PA4 (V) | PA4 DMM (V) |
| --- | --- | --- | --- | --- | --- | --- |
| ~0.800 V | FULL12 | | | | | |
| ~0.800 V | COARSE6 | | | | | |
| ~1.650 V | FULL12 | | | | | |
| ~1.650 V | COARSE6 | | | | | |
| ~2.500 V | FULL12 | | | | | |
| ~2.500 V | COARSE6 | | | | | |

## Triangle and captures — 6 report marks

Generator settings: __________ Hz; __________ Vpp; __________ V offset

Verified PA5 minimum/maximum: __________ / __________ V

1. FULL12 capture filename: ____________________
2. COARSE6 capture filename: ____________________

Label PA5/PA4, modes, voltage scales and time scales. Show adjacent settled
plateaus and voltage cursors in the COARSE6 capture.

Measured adjacent coarse step: __________ mV

Difference from prediction: __________ mV (__________ %)

## Explain in one page or less — 6 report marks

1. Show your FULL12 and COARSE6 mapping. How many distinct coarse commands exist,
   what is their spacing, and why is the largest one 4032?
2. Compare your measured coarse step with your prediction. Use your instrument
   readings to discuss any difference.
3. Explain why a smooth FULL12 trace does not establish continuous voltage
   resolution, and why 12-bit resolution does not guarantee voltage accuracy.
4. Choose one DC result. Compare its ADC estimate \(C V_R/4096\), measured input,
   nominal DAC output \(D V_R/4096\) and measured output. Explain why copying
   the ADC code to the DAC does not guarantee identical input and output voltage.

## Live demonstration — 80 marks, 5–8 minutes

Prepare the three DC measurements in advance; show one tutor-selected safe DC
input live, followed by the triangle. The tutor checks setup/SW0 (**10**),
independent DC input/output and real LCD codes/mapping (**25**), both live
triangle modes and cursor step measurement versus \(V_R/64\) (**25**), and your
register/quantisation explanations (**20**). See the exact sequence in the
[practical](prac_03.md#hand-in-and-demonstrate).

Tutor demonstration completed: ____________________
