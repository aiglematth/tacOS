#!/bin/bash

rm *.img 2> /dev/null
rm *.bin 2> /dev/null

nasm -f bin -o bootloader.bin bootloader.asm
nasm -f bin -o protected.bin protected.asm
nasm -f bin -o long.bin long.asm
nasm -f bin -o 64bits.bin 64bits.asm

cat bootloader.bin protected.bin long.bin 64bits.bin > image.bin

dd if=image.bin of=floppy.img

rm *.bin 2> /dev/null
