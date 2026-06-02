# embedded-linux-lab

A learning project for the embedded Linux stack. The goal is to build a solid understanding of toolchains, Buildroot, Yocto, the kernel, device trees, and bootloaders.  
During the process, I write in `NOTES.md` files—these are mostly raw notes, often messy for others but logical for me.  
I use QEMU for safe iteration before working with physical hardware.

## About Me

Jens, a YH student in embedded systems.  
I previously worked on a LoRaWAN gateway project (Open Habitat, to be made public later) running on a Raspberry Pi Zero 2W.  
This repository is separate and focuses purely on deepening my understanding of the Linux stack at the system level which is both fun and challenging.

## Structure

Each "phase" is a self-contained work unit with its own README and NOTES.
- **phase-01-toolchain/** = cross-compilation, ELF format, static vs dynamic linking  
- **phase-02-buildroot/** = minimal embedded Linux from source, custom packages, rootfs overlay, persistent logging, image size analysis  
- **phase-03-kernel/** = kernel configuration, custom loadable kernel module (LKM), `/proc`, `/sys`, `/dev`, device tree  

Upcoming:
- **phase-04-yocto/** = migrating to Yocto, layers, sstate cache  
- **phase-05-networking/** = adding networking, security, systemd  
- **phase-06-application/** = applying everything to the Open Habitat project (gateway)  

## How I Work

I primarily use *Mastering Embedded Linux Development* as a cookbook:  
[Link to book](https://www.adlibris.com/sv/bok/mastering-embedded-linux-development-9781803232591)
I also use AI tools (Perplexity and Claude) for structure and guidance.  
`NOTES.md` serves as my personal thinking space and guide—each phase includes working notes, decisions, issues encountered, and their solutions and they are in Swedish.  
`README.md` provides a structured overview and avoids becoming a wall of text.

## Development Environment

- Host: WSL2 Ubuntu on Snapdragon X (ARM64)  
- Buildroot 2026.02.1  
- QEMU for aarch64-virt  
- Linux kernel 6.18.7  

## Contact

[LinkedIn:](https://www.linkedin.com/in/jens-l-035132298/)
