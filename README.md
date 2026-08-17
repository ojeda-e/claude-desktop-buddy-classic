# Claude Desktop Buddy for M5StickC

[![GitHub Actions Status](https://github.com/ojeda-e/claude-desktop-buddy-M5StickC/workflows/CI/badge.svg)](https://github.com/ojeda-e/claude-desktop-buddy-M5StickC/actions?query=workflow%3ACI)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/ojeda-e/claude-desktop-buddy-M5StickC/blob/main/LICENSE)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-espressif32-orange.svg)](https://platformio.org/)
[![Board](https://img.shields.io/badge/board-M5StickC-blue.svg)](https://docs.m5stack.com/en/core/m5stickc)

A port of [anthropics/claude-desktop-buddy](https://github.com/anthropics/claude-desktop-buddy) to the **original M5StickC** (80 × 160 ST7735S).

<p align="center">
  <img src="docs/device-ported.png" alt="M5StickC running the buddy firmware" width="500">
</p>

The upstream firmware targets the M5StickC Plus. The two boards share most of their silicon (the same MPU6886 and AXP192), so this is a light port with UI compacted for the smaller display, not a rewrite.

| | Original M5StickC | M5StickC Plus |
| --- | --- | --- |
| Display | ST7735S, **80 × 160** | ST7789, 135 × 240 |
| Arduino library | `M5StickC` | `M5StickCPlus` |
| Buzzer | none | present |

## Features

With this firmware you can:

- Run the Claude Hardware Buddy on an original **M5StickC**
- Pick from **18 ASCII pets**, scaled for the 80 × 160 screen
- Approve or deny Claude tool prompts from the stick
- Track mood, fed, energy, level, prompts, and tokens on compact multi-page screens
- Pair over Bluetooth with Claude Desktop / Cowork

## Requirements

- An original **M5StickC** (not Plus / Plus2)
- A **USB-C** cable
- **macOS** (steps below. Linux works with the same `pio` commands and `/dev/ttyUSB*` ports)
- [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/)
- Claude Desktop or Claude Cowork with Developer Mode

## Installation

### 1. Install PlatformIO

On macOS with Homebrew:

```bash
brew install platformio
pio --version
```

Or follow the [PlatformIO install docs](https://docs.platformio.org/en/latest/core/installation/) for other platforms.

### 2. Clone this repository

```bash
git clone https://github.com/ojeda-e/claude-desktop-buddy-M5StickC.git
cd claude-desktop-buddy-M5StickC
```

The M5StickC port is already applied in this fork. No extra patching step.

### 3. Find the serial port

List serial devices **before** plugging in the stick:

```bash
ls /dev/cu.usbserial-*
```

Connect the M5StickC over USB-C, wait a few seconds, and list again. The new entry is your device, for example:

```text
/dev/cu.usbserial-XXXXXXXXXX
```

> [!NOTE]
> PlatformIO accepts either `/dev/cu.usbserial-…` or `/dev/tty.usbserial-…` on macOS. The original M5StickC uses an FTDI chip. Modern macOS includes the driver.

### 4. Build

```bash
pio run
```

The first run downloads the toolchain and libraries. Look for `[SUCCESS]` at the end. A healthy build looks roughly like:

```text
RAM:   [==        ]  22.5% (used 73572 bytes from 327680 bytes)
Flash: [======    ]  64.8% (used 1357989 bytes from 2097152 bytes)
========================= [SUCCESS] Took ~10-20 seconds =========================
```

### 5. Flash

Erase once if you previously flashed a different partition layout, then upload (replace the port with yours):

```bash
pio run -t erase  --upload-port /dev/tty.usbserial-XXXXXXXXXX
pio run -t upload --upload-port /dev/tty.usbserial-XXXXXXXXXX
```

On boot the pet appears **asleep**. That is expected until Claude is connected.

## Usage

> [!NOTE]
> **Buttons on the StickC:** **A** is the big front button (M5 logo face). **B** is the small button on the right edge. Power is on the left edge. Leave it alone for normal use.
>
> Rule of thumb: **A** moves the highlight or changes screen. **B** selects, pages, or confirms.

### Pick a pet

1. Tap **A** to wake the screen.
2. **Hold A** (~1s) to open the menu.
3. Press **B** to enter **settings**.
4. Tap **A** until **ascii pet** is highlighted.
5. Press **B** to cycle species.

| Index | ASCII pet |
|------:|-----------|
| 1 | capybara |
| 2 | duck |
| 3 | goose |
| 4 | blob |
| 5 | cat |
| 6 | dragon |
| 7 | octopus |
| 8 | owl |
| 9 | penguin |
| 10 | turtle |
| 11 | snail |
| 12 | ghost |
| 13 | axolotl |
| 14 | cactus |
| 15 | robot |
| 16 | rabbit |
| 17 | mushroom |
| 18 | chonk |

### Pair with Claude

1. In Claude Desktop or Cowork: **Help → Troubleshooting → Enable Developer Mode**.
2. Open **Developer → Open Hardware Buddy…**.
3. Wake the stick (tap **A**. A dark screen will not advertise over Bluetooth).
4. Click **Connect** and pick your device. Grant the macOS Bluetooth permission if prompted.
5. When the bridge connects, the pet should wake from asleep to idle.

### Screens on the stick

- **Home:** pet animation and transcript HUD.
- **Pet** (**A** to reach it, **B** pages): meters, counters (including prompts), and a short how-to.
- **Info** (**A** again, **B** pages): about, buttons, Claude link stats, device, Bluetooth, credits.

Face-down the stick to nap and refill energy. Flip it back up to wake.

## Troubleshooting

**Nothing in the device list?**  
Tap **A** to wake the stick. Confirm Bluetooth is on: hold **A** → settings → bluetooth.

**macOS never asked for Bluetooth, or you dismissed it?**  
System Settings → Privacy & Security → Bluetooth. Allow Claude, then Connect again.

**Build fails after a git pull?**  
Run `pio run` once more so PlatformIO can refresh libraries. Erase before upload if the partition table changed.

## What changed vs upstream

- Display library, resolution (80 × 160), RTC method names, and buzzer stubs for the original StickC
- ASCII pet canvas fixes (centering / offset) for the smaller panel
- Multi-page pet and info UI with short labels so text fits ~12 characters per line
- `prompts` counter on the stats page (permission prompts received)

For the BLE protocol contract, see [REFERENCE.md](REFERENCE.md).

## Contributing

This is a small board-specific port, not a catch-all firmware tree.

**Welcome here:** fixes that make this StickC build, flash, pair, or render correctly, plus clarifications to docs that are wrong for this board.

**Better as a fork of your own:** new boards, large features, new pets or screens. Same idea as [upstream](https://github.com/anthropics/claude-desktop-buddy/blob/main/CONTRIBUTING.md): keep the BLE contract, build the device you want.

## License

Source code is available under the MIT License (see [LICENSE](LICENSE)).

Ported to the original M5StickC by [Mia Ojeda](https://github.com/ojeda-e), based on [claude-desktop-buddy](https://github.com/anthropics/claude-desktop-buddy) by Felix Rieseberg / Anthropic.

GIF assets under `characters/bufo/` are third-party artwork. See [LICENSE](LICENSE) for details.

[PlatformIO]: https://platformio.org/
[REFERENCE.md]: REFERENCE.md
[LICENSE]: LICENSE
