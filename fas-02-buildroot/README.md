# Buildroot

Egen embedded Linux-image byggd med Buildroot 2026.02.1, riktad mot ARM64<br>
(qemu_aarch64_virt). Projekt för embedded Linux-utveckling,<br>
separerat från min produktions-LoRaWAN-gateway.<br>

## Vad imagen faktiskt innehåller<br>

- **Kernel:** Linux 6.18.7, ARM64 (aarch64)<br>
- **C-bibliotek:** glibc 2.42<br>
- **Init:** SysV via BusyBox<br>
- **Custom package:** `hello` minimal C-binär byggd via egen Buildroot-recipe<br>
- **Overlay-filer:** custom MOTD, sysinfo-script, init-script för autostart<br>
- **Persistent logging:** `/var/log` som riktig katalog (post-build-script<br>
  modifierar default-symlink till /tmp)<br>
- **Legal-info-spårning:** komplett licens-manifest via `make legal-info`<br>

## Byggandet

Klona Buildroot 2026.02.1, kopiera över defconfigen och bygg:<br>

\`\`\`bash
git clone https://github.com/buildroot/buildroot<br>
cd buildroot<br>
git checkout 2026.02.1<br>
cp /path/to/this/qemu_aarch64_lab_defconfig configs/<br>
make qemu_aarch64_lab_defconfig<br>
make<br>
\`\`\`<br>

Uppskattning av byggandet ~30-90 minuter första gången kompilerar toolchain + kernel + paket (beror på din host).<br>

## Start / kör<br>

\`\`\`bash<br>
cd output/images<br>
./start-lab.sh<br>
\`\`\`<br>

Login: `root` / `dev`<br>

## Strukturen hittils<br>

\`\`\`<br>
fas-02-buildroot/<br>
├── README.md<br>
├── NOTES.md                           detaljerade arbetsanteckningar<br>
├── qemu_aarch64_lab_defconfig         Buildroot-konfiguration<br>
├── package-hello/                     custom paket recipe<br>
│   ├── Config.in<br>
│   ├── hello.mk<br>
│   └── src/<br>
│       ├── hello.c<br>
│       ├── Makefile<br>
│       └── LICENSE<br>
├── rootfs-overlay/                    statiska filer till imagen<br>
│   ├── etc/<br>
│   │   ├── motd<br>
│   │   ├── init.d/S99hello            autostart-script<br>
│   │   └── profile.d/usrlocal.sh      PATH-fix<br>
│   ├── usr/local/bin/sysinfo.sh<br>
│   └── root/README.txt<br>
└── scripts/<br>
    └── post-build.sh                  modifierar rootfs efter paket-install<br>
\`\`\`<br>

## Vad jag har lärt mig<br>

- Cross-compilation principles (ARM64 från ARM-host i WSL2)<br>
- Buildroots arkitektur: paket, overlay, post-build-scripts<br>
- SysV init och autostart via /etc/init.d/SXX-scripts<br>
- Persistent vs ephemeral filsystem-design i embedded<br>
- Felsökning via dmesg, /proc/cmdline, stamp-filer<br>
- Legal-info och GPL-compliance för kommersiella produkter<br>
- Praktisk kernel boot-tid: ~500ms från Booting Linux till /sbin/init<br>

## Vad jag kommmer jobba vidare på<br>

- Kernel-konfiguration och egen LKM (loadable kernel module)<br>
- Migration till Yocto<br>
- Networking-säkerhet och systemd<br>
- Applicera allt på LoRaWAN-gateway-projektet (Open Habitat)<br>
