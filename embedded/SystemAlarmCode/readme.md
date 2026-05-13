# 📚 LAB 2 – CMPE2250
## 👩‍💻 Author
# Eunice De Grace Fmukam Ngadjou 
## Signal Reconstruction using ADC, DAC, and PWM

---

# 📋 Objective

The purpose of this lab is to capture an analog signal using the STM32 ADC and reconstruct it using two different methods:

* Direct Digital-to-Analog Conversion (DAC)
* Pulse Width Modulation (PWM)

Finally, digital filtering is applied to demonstrate how discrete and modulated signals can be smoothed back into a continuous analog waveform.

---

# 1️⃣ System Overview

The system operates as follows:

* The ADC samples an analog sine wave input
* The 12-bit digital value is:

  * Sent directly to the DAC (direct reconstruction)
  * Scaled and used to control PWM duty cycle

### Data Flow

```
Analog Sine → ADC → ISR/DMA → DAC → Analog Output
                         ↘ PWM Timer → Modulated Output
```

---

# 2️⃣ Part 1: ADC → DAC (Direct Playback)

## ⚙️ Implementation

* ADC configured on AN0 (12-bit resolution)
* DAC configured on output pin (PA4)
* Each ADC conversion triggers:

  * Read ADC value
  * Write directly to DAC register

### Mapping:

```
DAC_value = ADC_value
```

---

## 🔬 Observations

### Input Signal (200 Hz)

* Input: 200 Hz sine wave (3 Vpp, 1.5 V offset)
* Output: Staircase approximation of sine wave

📸 **Screenshot 1: ADC Input vs DAC Output (200 Hz)**
![DAC Output](Capture3.PNG)

---

### Input Signal (1 kHz)

At higher frequency:

* Fewer samples per period
* Output becomes more distorted
* Visible “stepping” increases

📸 **Screenshot 2: ADC Input vs DAC Output (1 kHz)**  
![DAC Output](Capture4.PNG)

### Explanation

Sampling frequency limits reconstruction quality.

The number of samples per period is given by:

N_samples = f_sampling / f_signal

In this lab:

* Sampling frequency ≈ 10 kHz (Timer trigger)
* Signal frequency = 200 Hz or 1 kHz

So:

* At 200 Hz:
  N = 10000 / 200 = 50 samples per period → smooth signal

* At 1 kHz:
  N = 10000 / 1000 = 10 samples per period → visible distortion

This explains why the DAC output becomes more “steppy” at higher frequencies.

### Estimated Limit

The maximum usable frequency depends on sampling rate:

* Good reconstruction requires ~20+ samples per period
* Beyond that → visible distortion

---

# 3️⃣ Part 2: PWM Modulation

## ⚙️ Timer Configuration

* Prescaler = 0
* Timer frequency = 40 MHz
* ARR chosen to generate high-frequency PWM

### Example:

```
ARR = 999 → PWM frequency = 40 kHz
```

---

## 🔄 Duty Cycle Mapping

ADC values must be scaled:

```
CCR = (ADC × ARR) / 4095
```

---

## 🔬 Observation

* PWM signal appears as a high-frequency square wave
* Pulse width varies following sine wave

📸 **Screenshot 2: PWM Output (Duty Cycle Modulation)**
![DAC Output](Capture8.PNG)

---

# 4️⃣ Part 3: Filtering and Analysis

## 🎯 Goal

Convert:

* PWM → smooth sine wave
* DAC staircase → clean sine wave

---

## 🔴 PWM Filtering

* Filter type: Butterworth Low Pass
* Order: 8
* Cutoff: ~2 kHz

### Result:

* High-frequency components removed
* Clean sine wave reconstructed

---

## 🔵 DAC Filtering

* Lower order sufficient (2–4)
* Cutoff: ~1.5–2 kHz

### Result:

* Staircase effect smoothed
* Signal closely matches input

---

📸 **Screenshot 3: Filtered PWM + Filtered DAC + Input Signal**
![DAC Output](Capture7.PNG)

---

## ⚠️ Important Notes

* High-order filter needed for PWM (high-frequency noise)
* Low-order filter sufficient for DAC (already close to analog)
* Filters introduce phase shift (normal behavior)

---

# 5️⃣ Analysis and Discussion

## Key Observations

* ADC + DAC produces discrete approximation
* PWM encodes signal using duty cycle variation
* Filtering reconstructs analog waveform

---

## Signal Behavior

| Method | Raw Output  | Filtered Output |
| ------ | ----------- | --------------- |
| DAC    | Staircase   | Smooth sine     |
| PWM    | Square wave | Smooth sine     |

---

## Challenges Encountered

* Aliasing at low sampling rates
* Signal distortion due to insufficient sampling
* Filter tuning required for stable output

---

## Conclusions

* Accurate signal reconstruction depends on sampling rate
* PWM requires aggressive filtering due to high-frequency components
* DAC requires minimal filtering
* Proper filter tuning is critical for stable output

---

# 6️⃣ Challenge (Optional)

## 🎯 Objective

Generate an inverted signal:

### Method:

For a 12-bit signal:

```
Inverted = 4095 - ADC_value
```

* Output original signal on DAC Channel 1
* Output inverted signal on DAC Channel 2

---

📸 **Screenshot 4: Original vs Inverted Signal**
![DAC Output](Capture6.PNG)

---

# 🧠 Final Conclusion

This lab demonstrates the fundamental concepts of:

* Analog-to-Digital Conversion
* Digital-to-Analog Reconstruction
* PWM Modulation
* Signal Filtering

It highlights the importance of sampling rate, resolution, and filtering in embedded signal processing systems.

---
