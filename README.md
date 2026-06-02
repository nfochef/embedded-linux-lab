# embedded-linux-lab

Ett lärprojekt för embedded Linux-stacken. Lära mig förståelsen för
toolchains, Buildroot, Yocto, kernel, device tree och bootloaders. Under mitt lärande så skriver jag i NOTES.md's och det är generellt "raw notes" som är stökiga för andra men logiska för mig.<br>
Använder QEMU för säker iteration innan jag rör fysisk hårdvara.

## Vem jag är

Jens YH-student inom embedded systems. Har sedan tidigare ett 
LoRaWAN-gateway-projekt (Open Habitat som öppnas publikt vid senare tillfälle) som körs på Raspberry Pi Zero 2W. 
Det här repot är separat, enbart fokuserat på att fördjupa förståelse för 
Linux-stacken på systemnivå och detta är riktigt kul och utmanande.

## Struktur

Varje "fas" är ett självständigt arbetsblock med egen README och NOTES.

- **fas-01-toolchain/** = cross-compilation, ELF-format, statisk vs dynamisk länkning
- **fas-02-buildroot/** = minimal embedded Linux från källkod, custom paket, 
  rootfs-overlay, persistent logging, image-storlek-analys
- **fas-03-kernel/** = kernel-konfiguration, egen loadable kernel module (LKM), 
  /proc /sys /dev, device tree

Kommande:
- **fas-04-yocto/** att migrera till Yocto, layers, sstate-cache
- **fas-05-networking/** addera networking, säkerhet, systemd
- **fas-06-application/** tillämpa allt på Open Habitat-projektet (Gatewayn)

## Hur jag arbetar
Jag använder mig främst av "Mastering Embedded Linux Development" -boken [Länk till bok](https://www.adlibris.com/sv/bok/mastering-embedded-linux-development-9781803232591) som Cookbook och Ai (Perplex samt Claude) för struktur och guidning
NOTES.md används som tankar och guide för mig själv, varje "fas" innehåller arbetsanteckningar, beslut, fel/problem jag stött på samt lösningar. 
README.md är översikten och mer strukturerad och inte "wall of text".

## Min Utvecklar miljö för detta projekt

- Host: WSL2 Ubuntu på Snapdragon X ARM64
- Buildroot 2026.02.1
- QEMU för aarch64-virt
- Linux kernel 6.18.7

## Kontakt

LinkedIn: [Jens L](https://www.linkedin.com/in/jens-l-035132298/)
