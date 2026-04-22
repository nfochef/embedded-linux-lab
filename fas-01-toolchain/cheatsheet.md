Prefix-logiken:
När jag jobbar med Makefiler och build-system sätter de en variabel CROSS_COMPILE=aarch64-linux-gnu- och prependar den till verktygsnamnen.
${CROSS_COMPILE}gcc blir aarch64-linux-gnu-gcc. Det är så kerneln, U-Boot, Buildroot och Yocto fungerar.
