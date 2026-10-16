# HACKPAD — COMPLETE PROJECT BRIEF FOR CLAUDE CODE

## 1. Your role

Act as my technical co-founder, embedded-systems engineer, PCB designer, firmware developer, desktop-app developer, CAD design assistant, and project mentor.

Help me build a real, functional, well-documented custom macropad project for the Hack Club Hackpad Challenge.

I am a student learning electronics, programming, PCB design, embedded development, CAD, and desktop software. I want to understand the engineering behind the project rather than blindly copy code.

Your job is to help me turn the idea into a working physical product through a clear, incremental engineering workflow.

**Important working rules:**
- Explain the reasoning behind important decisions, including pin allocation, circuit design, communication protocols, firmware architecture, and mechanical tolerances.
- Do not assume that a component, pin, library, or firmware feature works without checking its documentation.
- Do not generate an entire complicated codebase before we establish the architecture.
- Work in small, testable milestones.
- Before making major changes, explain what will change and why.
- Prefer simple, reliable implementations over unnecessary complexity.
- Keep track of completed work, unresolved issues, assumptions, and the next task.
- Ask me a focused question when a decision genuinely blocks progress; otherwise, make a sensible provisional recommendation and clearly label it.
- Do not pretend that hardware, firmware, or CAD has been tested when it has not.

---

## 2. Project vision

I am building a custom 12-key programmable macropad called **HackPad**.

It is intended primarily for developers, makers, students, and productivity-focused users.

The device should have:

- 12 mechanical keys arranged in a 3 × 4 grid.
- Two rotary encoders with push-button functionality.
- A 0.91-inch OLED display.
- Individually addressable RGB LEDs under or around the keys, where practical.
- A USB-C connection to the computer.
- A custom 3D-printed enclosure.
- Custom PCB and firmware.
- A companion desktop application for configuring the device.

The device should function as a useful standalone USB keyboard/macropad, with the desktop application providing a richer configuration experience.

The main design principle is:

**Minimalist, functional, customizable, and developer-oriented — not flashy or overloaded with effects.**

I want the project to be both a useful device and a strong demonstration of my engineering ability.

---

## 3. Final visual design direction

Use the following design language throughout the PCB layout, enclosure, firmware UI, and desktop app.

### Physical appearance

- Main enclosure: matte charcoal or near-black.
- Keycaps: white blank DSA-profile keycaps.
- Lighting accents: restrained cyan and purple.
- Overall shape: compact rectangular enclosure with rounded corners.
- Surface: clean, understated, modern.
- No excessive RGB animations or aggressive gaming aesthetics.
- USB-C port positioned accessibly on an exterior edge.
- Two metal rotary encoders positioned along the upper edge, one on each side of the OLED.
- OLED centered above the keys.
- Twelve keys in a symmetrical 3-column × 4-row arrangement.
- Hidden or unobtrusive fasteners where mechanically sensible.
- Two-piece enclosure: upper case and lower case, secured using screws and heat-set inserts if compatible with the actual dimensions.

### OLED appearance

The OLED should display information such as:
- Active profile.
- Current layer or mode.
- Encoder function.
- Brief feedback after a profile switch or configuration change.

Example:

Dev Mode
Layer 1

The exact layout must be designed for the actual 0.91-inch display resolution and controller. Do not assume the controller or resolution until verified.

### RGB appearance

RGB should be configurable but subtle by default.

Preferred effects:
- Static accent color.
- Low-brightness key illumination.
- Gentle profile-based colors.
- Optional restrained animations.

Provide brightness control and an option to turn lighting off.

### Industrial design target

The final device should look like a small, polished developer tool rather than a toy.

The original visual concept is only a reference. Do not treat any dimensions shown in a concept rendering as verified engineering measurements.

---

## 4. Confirmed hardware inventory

These are the parts I currently have or expect to use.

| Component | Quantity | Notes |
|---|---:|---|
| Seeed Studio XIAO RP2040 | 1 | Unsoldered board |
| 1N4148 through-hole diodes | 20 | Key-matrix isolation |
| MX-style mechanical switches | 16 | Use 12 initially |
| EC11E rotary encoders with push switch | 2 | Rotation and press functionality |
| 0.91-inch OLED display | 1 | Pin order: GND, VCC, SCL, SDA |
| White blank DSA keycaps | 16 | Use 12 initially |
| SK6812 MINI-E RGB LEDs | 16 | Individually addressable LEDs |
| M3 × 16 mm screws | 6 | Verify actual case requirements |
| M3 × 5 mm × 4 mm heat-set inserts | 6 | Verify insert geometry and mounting holes |
| Custom PCB | 1 | To be designed |
| 3D-printed enclosure | 1 set | To be designed |

I have spare switches, keycaps, diodes, and LEDs for potential replacements or future expansion.

Do not automatically assume all 16 LEDs should be used in the 12-key version. Evaluate whether using 12 key LEDs is appropriate and decide what to do with the remaining LEDs.

Do not add components casually. First verify the Hackpad Challenge rules and approved-parts policy.

If an extra component is necessary, explain:
1. Why it is needed.
2. Whether it is permitted.
3. What alternative exists.
4. What it costs in complexity and space.

Potential additional components may include a GPIO expander, resistors, decoupling capacitors, connectors, or other essential circuitry. These must be confirmed against the actual design and competition requirements.

---

## 5. Important technical constraint: GPIO budget

The controller is a standard Seeed Studio XIAO RP2040, not a different XIAO variant.

The standard board exposes a limited number of GPIO pins. The design must accommodate:

- A 3 × 4 key matrix.
- Two rotary encoders, each with two quadrature signals.
- Both encoder push switches.
- OLED communication.
- RGB LED data.
- Any additional signals needed by the selected architecture.

A conventional 3 × 4 matrix uses 3 row lines and 4 column lines, for 7 GPIO signals.

The two encoders require four quadrature signals and two push-button inputs if all six signals are connected directly.

The OLED normally uses two I²C signals, and the RGB chain normally uses one data signal.

This is approximately 14 GPIO signals before considering any extra functions. The exact requirements depend on the firmware, electrical design, and whether some signals can be shared or expanded.

**Do not begin PCB routing until the pin budget is solved.**

Investigate these options:

1. Use a permitted I²C GPIO expander for some inputs.
2. Investigate a suitable I/O expansion or shift-register architecture.
3. Evaluate whether a supported firmware architecture can reduce the number of direct GPIO connections.
4. Simplify the feature set only if necessary.

If an I²C GPIO expander is used, investigate whether the OLED and expander can safely share the I²C bus, including address conflicts, voltage compatibility, and pull-up requirements.

Check the official XIAO RP2040 documentation for the correct pin mappings and electrical limits.

Never assume that the XIAO RP2040 has the extra GPIO pins available on other board variants.

---

## 6. Key matrix and input hardware

The main key layout is a 3-column × 4-row matrix.

Design goals:
- Correct row/column wiring.
- One diode per key if using a conventional diode-isolated matrix.
- Consistent diode orientation.
- Clear switch footprints and pin numbering.
- Suitable footprints for the exact switches.
- Reliable debounce handling in firmware.
- No ghosting problems under supported simultaneous key presses.

There are 12 keys in the initial version.

The two encoders must support:
- Clockwise rotation.
- Counterclockwise rotation.
- Push-button press.
- Debouncing.
- Independent configuration.

Possible default assignments:
- Encoder 1: system volume and mute.
- Encoder 2: scrolling, zoom, or another configurable action.
- Encoder presses: mute, change mode, or switch layer.

These defaults must be configurable through the desktop app.

Do not hardcode these behaviors so deeply that future changes require rewriting the firmware.

---

## 7. OLED subsystem

The OLED module has the physical pin order:

GND → VCC → SCL → SDA

Verify the actual module's voltage requirements, controller, resolution, and communication protocol before selecting a library or drawing the PCB footprint.

The firmware should:
- Initialize the display reliably.
- Show the active profile.
- Show the current layer or mode.
- Provide brief visual feedback for user actions.
- Recover sensibly from configuration or display initialization errors.

Keep display rendering separate from key scanning and encoder processing.

Avoid unnecessarily frequent full-screen updates if they introduce flickering or slow input handling.

The desktop app should eventually be able to configure the information shown on the OLED, within the limitations of the display.

---

## 8. RGB subsystem

Use SK6812 MINI-E LEDs, after confirming the exact variant and its electrical requirements.

The design should support:
- Per-key RGB if the physical placement permits it.
- A consistent LED data chain.
- Configurable brightness.
- Static colors.
- Profile-based lighting.
- A small set of tasteful effects.
- Lighting disabled or dimmed when requested.

Investigate power consumption, signal integrity, data timing, power distribution, and required passive components.

Do not assume the USB supply can safely provide maximum brightness to all LEDs without calculating the load.

Implement a sensible default brightness limit.

If per-key LEDs are used, map each LED index to its corresponding key in a documented, maintainable way.

---

## 9. USB and firmware requirements

The XIAO RP2040 should connect to the computer through USB-C and behave as a USB HID keyboard where appropriate.

The device should ideally:
- Work without requiring the desktop app to remain open.
- Provide default key mappings.
- Support common keyboard shortcuts.
- Handle encoder actions.
- Remember its configuration across power cycles.
- Expose a documented method for changing configuration.
- Recover safely from invalid configuration data.

Evaluate appropriate firmware choices before committing.

Possible approaches include:
- QMK, if the exact controller, features, and configuration architecture are supported.
- KMK, if its RP2040 support, USB behavior, and required display/LED features meet the project needs.
- Custom firmware, if necessary, with a clear explanation of the extra engineering cost.

Do not choose a framework based only on popularity.

Compare options based on:
- RP2040 compatibility.
- HID keyboard support.
- Matrix scanning.
- Encoder support.
- OLED support.
- SK6812 support.
- Configuration storage.
- USB configuration communication.
- Ease of development and debugging.

Document the chosen framework and its limitations before implementation.

---

## 10. Companion desktop application

This is a major feature, not an optional afterthought.

I want to create a desktop app through which users can customize the macropad without editing firmware source code.

### Preferred first implementation

Investigate Python with PySide6 for a desktop application.

A web-based desktop framework is also possible if there is a compelling reason, but avoid choosing a more complex stack unnecessarily.

### Visual style

The app should match the physical device:
- Dark interface.
- Charcoal background.
- Cyan and purple accent colors.
- Clean, restrained developer-oriented design.
- Clear navigation.
- A visual representation of the 12-key layout.

### Main screens

#### A. Home / Device status
- Connection status.
- Device name and firmware version, if available.
- Active profile.
- Basic device information.
- Connection or troubleshooting messages.

#### B. Keymap editor
- Display a visual 3 × 4 key grid.
- Let users select a key.
- Assign a keyboard key or shortcut.
- Support common combinations such as Ctrl+C and Ctrl+V.
- Support modifier keys.
- Provide an understandable representation of assigned actions.
- Validate unsupported actions before sending configuration.

#### C. Profiles
Provide several editable profiles, such as:
- Developer.
- General productivity.
- Creative.
- Custom.

Users should be able to:
- Create a profile.
- Rename a profile.
- Edit its key mappings.
- Save changes.
- Switch profiles.
- Duplicate a profile.
- Restore defaults.

#### D. Lighting
- Select a color.
- Adjust brightness.
- Choose supported effects.
- Choose profile-based lighting.
- Turn LEDs off.
- Preview settings before applying them where practical.

#### E. OLED settings
- Choose which information is displayed.
- Configure the profile label.
- Configure supported status indicators.
- Preview the display layout where practical.

#### F. Settings
- Device connection settings.
- Configuration backup and restore.
- Firmware version information.
- Reset-to-default functionality.
- Troubleshooting and diagnostics.

### Important limitation: shortcuts versus launching programs

Sending normal keyboard shortcuts through USB HID is different from launching arbitrary programs on the computer.

Implement standard key mappings first.

If users need actions such as opening applications, running scripts, or executing custom commands, investigate whether these require a separate desktop-side helper process. Treat this as a later feature and design it with clear permission boundaries.

Do not claim that a keyboard-only firmware can launch arbitrary desktop programs by itself.

---

## 11. Desktop app and device communication protocol

Design a reliable configuration protocol between the app and the firmware.

Investigate possible approaches, including:
- USB CDC serial.
- Another suitable USB interface.
- A supported HID-based configuration channel.

Select the simplest robust approach compatible with the chosen firmware and operating systems.

The protocol should eventually support operations such as:
- Detecting the device.
- Requesting device information.
- Reading the current configuration.
- Updating key assignments.
- Updating profile information.
- Updating lighting settings.
- Updating OLED settings.
- Saving configuration.
- Switching profiles.
- Reporting errors.
- Confirming successful updates.

Define a versioned message format before implementing the protocol.

Include:
- Message types.
- Configuration schema.
- Validation rules.
- Error responses.
- Compatibility/version handling.
- Timeouts and recovery behavior.

The app must not assume a configuration update succeeded until it receives an appropriate acknowledgement.

### Configuration persistence

Investigate storage limits on the RP2040 and selected firmware.

Decide which configuration data lives:
- On the device.
- In the desktop app.
- In both locations.

The device should retain a usable default configuration and, if feasible, user settings after being unplugged.

Plan how the app and device resolve conflicting configurations.

---

## 12. PCB design

Use KiCad unless another tool is justified.

The PCB should include:
- Twelve mechanical switch footprints.
- Appropriate diode footprints.
- Two encoder footprints.
- OLED connector or compatible mounting arrangement.
- RGB LED footprints.
- XIAO RP2040 mounting arrangement.
- Required GPIO expansion circuitry, if selected.
- USB connection compatible with the controller's actual USB implementation.
- Power distribution.
- Grounding and required passive components.
- Mounting holes aligned with the enclosure.
- Test points where useful.
- Clear silkscreen labels.

Before routing:
1. Create a complete schematic.
2. Validate the schematic electrically.
3. Create and verify a pin-allocation table.
4. Confirm footprints against real component dimensions.
5. Run the electrical rules check.
6. Place components with the enclosure in mind.
7. Route the PCB.
8. Run design-rule checks.
9. Export manufacturing files.
10. Review the PCB visually and prepare assembly documentation.

Do not design a custom USB-C connector circuit unnecessarily if the XIAO board already provides the intended USB connection. Verify the physical mounting and cable-access strategy first.

The final PCB must be compatible with the real XIAO board and all physical components.

---

## 13. CAD and 3D-printed enclosure

Use Autodesk Fusion or another suitable CAD tool, depending on availability and licensing.

The enclosure should have:
- Upper case.
- Lower case.
- Twelve key openings.
- Correct mounting support for the selected switches or plate.
- Two encoder openings.
- OLED opening.
- USB-C access.
- PCB mounting points.
- Screw and heat-set insert locations.
- Adequate clearance for wiring and components.
- Appropriate wall thickness and printability.

The design must be derived from actual component measurements, footprints, and PCB geometry.

Do not use visual estimates from the concept rendering as manufacturing dimensions.

### Mechanical workflow

1. Confirm all physical component dimensions.
2. Design or import the PCB.
3. Establish a coordinate system shared by the PCB and case.
4. Model the case around the real PCB.
5. Verify keycap and switch clearances.
6. Verify encoder shaft and nut clearances.
7. Verify OLED viewing area.
8. Verify USB-C cable access.
9. Check screw lengths, insert dimensions, and engagement.
10. Export a preliminary STL and inspect the fit.
11. Print a prototype.
12. Test the actual assembly and revise tolerances.

The initial design may use approximate overall dimensions, but those dimensions must be revised after the actual PCB layout is established.

The enclosure should be serviceable: I should be able to open it for repair without damaging the PCB or wiring.

---

## 14. Hackpad Challenge requirements

Official starting point:

https://hackpad.hackclub.com/

Submission guide:

https://hackpad.hackclub.com/submitting

Read the current official documentation and applicable rules before finalizing the design.

Verify:
- Approved components and parts restrictions.
- Whether extra integrated circuits or GPIO expanders are permitted.
- Required CAD and PCB source files.
- Required production exports.
- Firmware requirements.
- Repository and README requirements.
- Submission process and current deadlines.

Do not assume that a component is allowed simply because it is inexpensive or technically suitable.

Prepare a clean project repository containing appropriate folders, for example:

HackPad/
- README.md
- CAD/
- PCB/
- Firmware/
- DesktopApp/
- Production/
- Documentation/
- Tests/

Adapt this structure to the actual tools and competition requirements.

The submission documentation should include:
- A photograph or rendering of the completed device.
- A schematic.
- PCB views.
- CAD views and assembly details.
- A bill of materials with source links.
- Assembly instructions.
- Firmware instructions.
- Desktop app instructions.
- Manufacturing exports where required.

The official guide may require specific formats. Follow the current guide rather than relying on this example structure alone.

Keep editable source files separate from manufacturing exports.

---

## 15. Development strategy: build in milestones

Do not attempt to develop all subsystems simultaneously.

### Milestone 1 — Feasibility and architecture
- Review the official challenge rules.
- Verify every component.
- Create the GPIO budget.
- Resolve the GPIO expansion question.
- Choose the firmware framework.
- Create a system block diagram.
- Create a preliminary BOM.

**Completion criterion:** We have a technically feasible design before committing to PCB routing.

### Milestone 2 — Schematic and PCB
- Draw the schematic.
- Validate all connections.
- Create footprints.
- Lay out the 12-key matrix.
- Place encoders, OLED, RGB and controller.
- Complete routing and design checks.

**Completion criterion:** A reviewable PCB design with documented pin assignments and manufacturing exports.

### Milestone 3 — Minimal firmware
- Set up the development environment.
- Confirm the controller is detected.
- Test one key.
- Implement the matrix.
- Add encoder rotation and presses.
- Add the OLED.
- Add RGB.
- Test USB keyboard output.

**Completion criterion:** The macropad works as a physical USB input device with basic feedback.

### Milestone 4 — Configuration system
- Define a keymap data structure.
- Implement configuration validation.
- Define profile representation.
- Decide on storage.
- Implement the USB configuration protocol.
- Add device acknowledgement and error handling.

**Completion criterion:** A configuration can be changed, saved, and restored without rewriting the firmware.

### Milestone 5 — Desktop app MVP
Build only the essential features first:
- Device connection.
- Visual keymap.
- Key assignment editor.
- Read and write configuration.
- Save and switch profiles.
- Basic lighting settings.

**Completion criterion:** Changing one key in the app changes the actual key behavior on the connected device.

### Milestone 6 — CAD and assembly
- Finalize the PCB outline.
- Model the enclosure.
- Check component clearances.
- Print the prototype.
- Assemble and test the device.
- Fix mechanical issues.

**Completion criterion:** The components fit, the enclosure closes correctly, and the device can be used comfortably.

### Milestone 7 — Polish
- Add richer OLED customization.
- Add more lighting effects.
- Improve app visuals.
- Add configuration backup and restore.
- Test reconnection and power cycling.
- Write documentation.
- Prepare final manufacturing files.

**Completion criterion:** A reliable, presentable and well-documented project.

---

## 16. Testing requirements

Create a checklist and record the results.

### Hardware
- All 12 keys register.
- No unintended key presses.
- Diodes are correctly oriented.
- Both encoders rotate in both directions.
- Both encoder buttons work.
- OLED initializes reliably.
- RGB data chain works.
- No unexpected resets or power issues.
- USB connection is mechanically accessible.
- PCB and enclosure fit together.

### Firmware
- Keyboard shortcuts behave correctly.
- Key debouncing works.
- Encoder actions are responsive.
- OLED updates correctly.
- RGB brightness is controlled.
- Configuration is validated.
- Device recovers from invalid configuration.
- Settings persist if supported.
- The macropad remains usable without the app.

### Desktop application
- Detects the correct device.
- Displays connection state accurately.
- Reads the current configuration.
- Applies key mappings correctly.
- Handles unsupported mappings.
- Receives and displays errors.
- Saves and switches profiles.
- Changes lighting settings.
- Restores defaults safely.
- Handles unplugging and reconnecting.

### Integration
The most important end-to-end test is:

1. Connect the macropad.
2. Open the desktop app.
3. Change one key's assignment.
4. Apply the change.
5. Receive confirmation from the device.
6. Press the physical key.
7. Confirm that the new action occurs.
8. Unplug and reconnect the device.
9. Check that the configuration remains available if persistence is implemented.

---

## 17. My learning preferences

I want to understand the engineering, not just obtain a finished result.

When explaining a concept:
- Start with the underlying idea.
- Explain why it matters in this project.
- Show the relevant calculations when appropriate.
- Give a practical example.
- Explain likely failure modes.
- Then show how to implement or test it.

For example, when discussing GPIO allocation, explain why the matrix reduces the number of pins needed. When discussing USB, explain the distinction between HID keyboard output and configuration communication. When discussing PCB routing, explain the purpose of ground planes, decoupling capacitors, trace widths and design-rule checks where relevant.

Do not overwhelm me with unrelated theory. Teach what I need at the point where I need it.

When generating code:
- Use readable names.
- Add meaningful comments.
- Separate responsibilities into modules.
- Include setup and testing instructions.
- Explain dependencies and installation.
- Provide small test cases.
- Do not invent test results.
- Keep secrets and personal credentials out of the repository.

When generating CAD or PCB guidance, distinguish clearly between conceptual instructions and actual editable files. Do not claim to have created a PCB, STEP model, STL, or working app unless that artifact actually exists.

---

## 18. How I want you to begin

Do not immediately generate the entire firmware, desktop app, and PCB.

Start with these tasks:

1. Inspect the existing project directory and any files already present.
2. Review the current official Hackpad Challenge documentation and submission requirements.
3. Verify the exact XIAO RP2040 pinout and the OLED and RGB component requirements.
4. Create a component inventory and mark which components are confirmed, uncertain, or missing.
5. Build a complete GPIO allocation table for the 12 keys, two encoders including their push buttons, OLED, and RGB.
6. Identify the simplest permitted solution to any GPIO shortage.
7. Compare suitable firmware approaches for the chosen hardware and desktop configuration requirements.
8. Recommend a complete system architecture.
9. Write a short implementation roadmap and identify the first concrete task.

Present your findings before making irreversible design decisions.

**First priority: feasibility, correct pin allocation, and competition compliance.**

**Second priority: a working physical macropad.**

**Third priority: the customizable desktop app.**

**Fourth priority: mechanical refinement, polish, testing, and submission documentation.**

Help me build this properly, explain your reasoning, and keep the implementation grounded in verifiable hardware and software constraints.