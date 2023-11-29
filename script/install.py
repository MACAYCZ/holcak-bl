#!/bin/python3
from sys import argv
from os import path

if len(argv) < 3:
	print(f"Usage: {argv[0]} <image> <build>")
	print("    <image> - Existing raw file with size")
	print("    <build> - Build directory with binaries")
	print("Error: Insufficient number of arguments were provided!")
	exit(1)

with open(argv[1], "rb+") as f:
	f.write(open(f"{argv[2]}/stage1.bin", "rb").read(0x0200))
	f.seek(0x01B0)
	f.write(int(0x01).to_bytes(6, 'little'))	
	size = path.getsize(f"{argv[2]}/stage2.bin")
	f.write(int((size + 0x01FF) // 0x0200).to_bytes(2, 'little'))
	f.seek(0x0200)
	f.write(open(f"{argv[2]}/stage2.bin", "rb").read())
