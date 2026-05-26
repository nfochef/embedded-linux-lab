# Fas 2 — Buildroot

Egen embedded Linux-image byggd med Buildroot 2026.02.1, riktad mot ARM64
(qemu_aarch64_virt). Pedagogiskt projekt för embedded Linux-utveckling,
separerat från min produktions-LoRaWAN-gateway.

## Vad imagen faktiskt innehåller

- **Kernel:** Linux 6.18.7, ARM64 (aarch64)
- **C-bibliotek:** glibc 2.42
- **Init:** SysV via BusyBox
- **Custom package:** `hello` minimal C-binär byggd via egen Buildroot-recipe
- **Overlay-filer:** custom MOTD, sysinfo-script, init-script för autostart
- **Persistent logging:** `/var/log` som riktig katalog (post-build-script
  modifierar default-symlink till /tmp)
- **Legal-info-spårning:** komplett licens-manifest via `make legal-info`

## Byggandet

Klona Buildroot 2026.02.1, kopiera över defconfigen och bygg:

\`\`\`bash
git clone https://github.com/buildroot/buildroot
cd buildroot
git checkout 2026.02.1
cp /path/to/this/qemu_aarch64_lab_defconfig configs/
make qemu_aarch64_lab_defconfig
make
\`\`\`

Uppskattning av byggandet ~30-90 minuter första gången kompilerar toolchain + kernel + paket (beror på din host).

## Start / kör

\`\`\`bash
cd output/images
./start-lab.sh
\`\`\`

Login: `root` / `dev`

## Strukturen hittils

\`\`\`
fas-02-buildroot/
├── README.md
├── NOTES.md                           detaljerade arbetsanteckningar
├── qemu_aarch64_lab_defconfig         Buildroot-konfiguration
├── package-hello/                     custom paket recipe
│   ├── Config.in
│   ├── hello.mk
│   └── src/
│       ├── hello.c
│       ├── Makefile
│       └── LICENSE
├── rootfs-overlay/                    statiska filer till imagen
│   ├── etc/
│   │   ├── motd
│   │   ├── init.d/S99hello            autostart-script
│   │   └── profile.d/usrlocal.sh      PATH-fix
│   ├── usr/local/bin/sysinfo.sh
│   └── root/README.txt
└── scripts/
    └── post-build.sh                  modifierar rootfs efter paket-install
\`\`\`

## Vad jag har lärt mig

- Cross-compilation principles (ARM64 från ARM-host i WSL2)
- Buildroots arkitektur: paket, overlay, post-build-scripts
- SysV init och autostart via /etc/init.d/SXX-scripts
- Persistent vs ephemeral filsystem-design i embedded
- Felsökning via dmesg, /proc/cmdline, stamp-filer
- Legal-info och GPL-compliance för kommersiella produkter
- Praktisk kernel boot-tid: ~500ms från Booting Linux till /sbin/init

## Vad jag kommmer jobba vidare på

- Kernel-konfiguration och egen LKM (loadable kernel module)
- Migration till Yocto
- Networking-säkerhet och systemd
- Applicera allt på LoRaWAN-gateway-projektet (Open Habitat)
