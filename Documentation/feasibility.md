# HackPad: Feasibility and Architecture (Milestone 1)

Status: **draft for review**. Nothing here has been built or tested yet.
Confidence tags: **[Certain]** checked against a source, **[Likely]** strong engineering judgement, **[Guessing]** needs confirmation.

## Confirmed spec (2026-10-16)

- 12 MX keys, **4 across × 3 down**
- **1** EC11 rotary encoder with push-click (knob 2 dropped)
- 0.91" OLED (pins GND-VCC-SCL-SDA)
- SK6812 MINI-E RGB, subtle by default
- Seeed XIAO RP2040, USB-C
- **Kit parts only**, no I/O expander

---

## 1. Hackpad rules that affect this design

Sources: [Blueprint submission guide](https://blueprint.hackclub.com/hackpad/submitting), [Blueprint parts list](https://blueprint.hackclub.com/hackpad/parts), [Stardance guide](https://stardance.hackclub.com/missions/hackpad/guide).

| Rule | Our design | Status |
|---|---|---|
| Through-hole Seeed XIAO RP2040 as the MCU | XIAO RP2040 | OK [Certain] |
| PCB ≤ 100 × 100 mm, 2 layers | Target < 95 × 90 mm, 2 layers | OK [Certain] |
| Case fits in 200 × 200 × 100 mm, 3D-printed only | ~100 × 95 × 33 mm, printed | OK [Certain] |
| "You have less than 16 inputs (switches, encoders, etc)" | 12 keys + 1 encoder (+ its click) = 13–14 | OK [Certain] |
| "Approved parts only" | Every electronic part is from the kit | OK [Certain] |
| Repo needs `CAD/`, `PCB/`, `Firmware/`, `production/`, and a README with render, schematic, PCB and case screenshots plus a BOM | Created as each milestone produces files | Planned |
| Deadline | The Stardance banner says "the new deadline is october 31". Unclear whether it applies to Hackpad. | **Confirm in #hackpad** [Guessing] |

Case rules from the Stardance guide: heat-set insert holes 4.7 mm, screw clearance holes 3.4 mm, 0.2 mm tolerance on mating surfaces, export as .STEP.
Kit hardware: the Blueprint parts page lists **4** M3 screws and 4 inserts, so the case is designed for 4.

---

## 2. Component check

| Part | Qty used / in kit | Key facts | Status |
|---|---|---|---|
| Seeed XIAO RP2040 | 1 / 1 | 11 GPIO (D0–D10), 3.3 V logic, **not 5 V tolerant**, I²C0 on D4/D5. No extra back pads (those exist only on the "Plus"). | Confirmed [Certain] ([Seeed wiki](https://wiki.seeedstudio.com/XIAO-RP2040/)) |
| MX switches | 12 / 16 | | Confirmed |
| EC11 encoder with push | 1 / 2 | A/B quadrature + common, plus a separate push switch | Confirmed. 1 spare. |
| 0.91" OLED | 1 / 1 | Pin order GND-VCC-SCL-SDA. Almost always 128 × 32, SSD1306 controller, I²C address 0x3C. | Controller and resolution **unverified** until it arrives [Likely] |
| SK6812 MINI-E | 12 / 16 | Reverse-mount, shines up through the switch. Powered from 5 V. | Confirmed. See §5. 4 spare (could become an underglow strip later). |
| 1N4148 diodes | 13 / 20 | One per switch in the duplex matrix (§3) | Confirmed |
| DSA keycaps | 12 / 16 | | Confirmed |
| M3×16 screws + inserts | 4 / 4 | | Confirmed |
| Knob cap | 1 | Not in the kit, so **3D-print it** with the case (6 mm D-shaft). Printed parts are allowed. | Planned |

---

## 3. The GPIO budget

### The problem

| Function | Pins |
|---|---|
| Encoder turn (A + B) | 2 |
| OLED (I²C SDA + SCL) | 2 |
| RGB data | 1 |
| **Left for switches** | **11 − 5 = 6** |

There are 13 switches: 12 keys plus the knob click.

A normal matrix with 6 pins is 3 × 3 = **9 switches**. That's 4 short. [Certain]

### The fix: a duplex matrix (the "Japanese duplex")

**The idea.** In a normal matrix, each row/column crossing has one switch and one diode, so current can flow only one way. A duplex matrix puts **two** switches at each crossing, with their diodes pointing in **opposite directions**. The firmware scans twice:

1. Drive each L line low in turn and read the M lines. This sees only the switches whose diode lets current flow M → L.
2. Drive each M line low in turn and read the L lines. This sees only the switches whose diode lets current flow L → M.

So 3 + 3 = 6 pins give 3 × 3 × 2 = **18 switch positions**. We need 13. [Certain]

**Cost:** one diode per switch (13 of the kit's 20), and a custom scanning routine in firmware (§4). There are no extra chips.

**Failure modes:**
- A diode soldered backwards puts the switch in the other scan pass, so the wrong key fires. The firmware's position map in §3 makes this easy to diagnose, and silkscreen arrows on the PCB prevent it.
- Every pin must float (high-impedance input) when it isn't being driven, or the two passes interfere. That's handled in firmware.
- There's no ghosting with one diode per switch, same as a normal matrix.

### Pin allocation (provisional: pins may be swapped during PCB routing)

| XIAO pin | RP2040 GPIO | Function |
|---|---|---|
| D0 | GPIO26 | Encoder A |
| D1 | GPIO27 | Encoder B |
| D2 | GPIO28 | Duplex line L0 |
| D3 | GPIO29 | Duplex line L1 |
| D4 | GPIO6 | I²C SDA (OLED) |
| D5 | GPIO7 | I²C SCL (OLED) |
| D6 | GPIO0 | Duplex line L2 |
| D7 | GPIO1 | SK6812 data in (12 LEDs chained) |
| D8 | GPIO2 | Duplex line M0 |
| D9 | GPIO4 | Duplex line M1 |
| D10 | GPIO3 | Duplex line M2 |

All 11 pins are used. The encoder common pin and the 5 V and GND pins are not GPIO.

### Switch positions

Keys are numbered by row and column: K(row, col), with row 0 at the top and col 0 on the left.

| Crossing | Pass 1 (L driven, diode cathode → L) | Pass 2 (M driven, diode cathode → M) |
|---|---|---|
| L0 × M0 | K(0,0) | K(0,3) |
| L0 × M1 | K(0,1) | Knob click |
| L0 × M2 | K(0,2) | spare |
| L1 × M0 | K(1,0) | K(1,3) |
| L1 × M1 | K(1,1) | spare |
| L1 × M2 | K(1,2) | spare |
| L2 × M0 | K(2,0) | K(2,3) |
| L2 × M1 | K(2,1) | spare |
| L2 × M2 | K(2,2) | spare |

The 5 spare positions could take 5 more switches later without new pins.

### Other wiring

- **Encoder:** the common pin goes to GND, and A/B go to D0/D1 with the RP2040's internal pull-ups. The encoder is read directly, not through the matrix, so fast turns don't skip steps. [Likely]
- **OLED:** VCC goes to **3V3**, not 5 V, so the I²C lines stay at the XIAO's safe 3.3 V. Most of these modules already have I²C pull-up resistors on board. [Likely] Check yours when it arrives.

### An alternative to note, not recommended

Knob 2 could come back by wiring its A/B contacts into the spare duplex positions and decoding the rotation in firmware. That works in theory but is unproven on this hardware and makes the firmware harder. [Guessing] The brief says to prefer simple and reliable, so it's left out.

---

## 4. Firmware choice

| Need (from the brief) | QMK (C) | KMK (CircuitPython) |
|---|---|---|
| RP2040 support | Yes | Yes |
| Duplex matrix | "Lite" custom matrix: we write `matrix_scan_custom()`, about 50 lines, and QMK handles debounce and the rest ([docs](https://docs.qmk.fm/custom_matrix)) [Certain] | Needs a custom scanner class |
| Encoder, OLED, SK6812 | Built in | Built in, slower |
| **Saving config across power cycles** | Built in (emulated EEPROM in flash) | Hard: the flash drive is read-only to code while plugged into a computer |
| **Channel for your desktop app** | Raw HID, and the documented VIA protocol already handles keymap read/write | USB serial is easy, but you'd write the whole protocol and storage yourself |
| Works without the app | Yes | Yes |
| Learning curve | Steeper (C, build toolchain) | Easier (Python) |

**Recommendation: QMK with VIA enabled.** [Likely]
- The PySide6 app talks to the pad over **raw HID**, using the VIA protocol for keymaps. That gives it a tested way to read and write keys from day one.
- Extra features (profiles, OLED text, lighting presets) use VIA's custom-command channel with our own versioned message format, as the brief asks (§11).
- VIA or Vial remains a fallback configurator if the app isn't finished.
- Why not KMK: saving settings and talking to an app are your two hardest requirements, and KMK makes both harder.

---

## 5. Power and RGB

- 12 × SK6812 at full white ≈ 12 × 60 mA = **720 mA**. That's more than a basic USB port's 500 mA. [Likely, from about 20 mA per colour channel]
- Firmware will cap brightness at about 40%, which is about 290 mA worst case. The default is a dim static cyan/purple, as the brief asks.
- **Keys glow when pressed** (requested 2026-10-16): QMK's RGB Matrix has built-in reactive effects (for example `SOLID_REACTIVE_SIMPLE`, which lights the pressed key and fades it out). [Certain] They need a table mapping each LED to its key (`g_led_config`), so the LED chain order gets documented on the schematic. The 40% brightness cap still applies.
- **Data-voltage caveat:** the LEDs run on 5 V, but the XIAO sends 3.3 V data. The usual logic-high threshold is 0.7 × 5 V = 3.5 V, so 3.3 V is slightly below spec. Most Hackpad builds work like this in practice. [Likely] If the first LED misbehaves, the usual fix is to feed that LED's power through a diode, which lowers its threshold. A spare 1N4148 can do that.

---

## 6. System block diagram

```
USB-C (on XIAO) ──► XIAO RP2040 ── 5V ──► 12 × SK6812 (data from D7)
                       │
                       ├── 3V3 + I²C (D4/D5) ── OLED 128×32 (0x3C)
                       ├── D0/D1 ── EC11 encoder (turn)
                       └── D2,D3,D6 (L) × D8,D9,D10 (M) ── duplex matrix
                                                    12 keys + knob click, 13 diodes

Computer:  HID keyboard (always works)  +  raw HID ◄──► PySide6 app
```

---

## 7. Open questions

1. **Deadline:** is October 31 the Hackpad deadline? Ask in #hackpad.
2. **OLED:** confirm SSD1306, 128 × 32 and on-board pull-ups when it arrives.

## 8. Next concrete task

Milestone 2: draw the KiCad schematic from the pin table in §3, using the Hack Club footprint library. The PCB is laid out only after you've reviewed the schematic.
