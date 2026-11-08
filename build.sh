#!/usr/bin/env bash
# Build Master and Slave AVR firmware with avr-gcc.
# Used by GitHub Actions and for local builds. Exits non-zero on any error
# or warning (-Werror), so CI fails if either firmware does not build cleanly.
set -euo pipefail

MCU=atmega168          # from <avrdevice> in both .cppproj files
OUT=build

CXXFLAGS=(
  -mmcu="$MCU"
  -Os
  -funsigned-char
  -funsigned-bitfields
  -ffunction-sections
  -fdata-sections
  -fpack-struct
  -fshort-enums
  -Wall
  -Wextra
  -Werror
)

build() {
  local name="$1" srcdir="$2"; shift 2
  local dir="$OUT/$name"
  mkdir -p "$dir"
  echo "=== Building $name ($MCU) ==="
  local objs=()
  for src in "$@"; do
    local obj="$dir/$(basename "${src%.*}").o"
    echo "avr-g++ -c $srcdir/$src"
    avr-g++ -x c++ "${CXXFLAGS[@]}" -I "$srcdir" -c "$srcdir/$src" -o "$obj"
    objs+=("$obj")
  done
  avr-g++ -mmcu="$MCU" -Wl,--gc-sections -Wl,-Map="$dir/$name.map" \
    "${objs[@]}" -lm -o "$dir/$name.elf"
  avr-objcopy -O ihex -R .eeprom "$dir/$name.elf" "$dir/$name.hex"
  test -s "$dir/$name.hex"
  avr-size --format=avr --mcu="$MCU" "$dir/$name.elf"
}

build master AVR_Application_Master \
  AVR8574.cpp \
  AVRI2C.cpp \
  AVRIO_I2C_Master.cpp \
  AVR_Application_Master.CPP \
  AVRKeyboard_NOTAUS.CPP \
  AVRLCD.CPP \
  AVRTimer0.CPP \
  AVRUSART.CPP \
  FIFO.CPP \
  Schrittmotor.cpp

build slave AVRIO1_Slave_I2C \
  AVRIO1_Slave_I2C.cpp \
  AVRI2C_Master_Slave.cpp \
  AVRLEDMatrix.cpp \
  AVRTimer0.cpp

echo "=== Both firmware images built successfully ==="
