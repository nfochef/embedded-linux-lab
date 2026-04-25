# Fas 2 Buildroot
Källkod , konfiguration in , image ut och det ska vara repruducerbart..

Tankar innan jag börjar med att bygga Buildrots:
Jag har inte byggt en Kconfig tidigare, jag kommer använda mig av material som boken "Mastering Embedded Linux Development" och Dels Ai för stöd och struktur.<br>

Klonat  från repo https://gitlab.com/buildroot.org/buildroot.git<br>
buildroot on  HEAD (0141ca3)<br>
❯ git describe<br>
ls configs/raspberrypizero2w*<br>
2026.02.1<br>
configs/raspberrypizero2w_64_defconfig  configs/raspberrypizero2w_defconfig<br>

Valde raspberrypizero2w_64_defconfig för konsistens med min nuvarande 64-bit-setup. Skillnaden mellan defconfigs är target-arkitektur (32 vs 64-bit ARM), trots att det samma fysiska hårdvara.<br>
