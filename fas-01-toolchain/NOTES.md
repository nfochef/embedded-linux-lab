embedded-linux-lab on  main
❯ uname -m
uname -a
gcc --version
ldd --version | head -1
cat /etc/os-release | head -3
aarch64
Linux "XVX" 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Thu Jun  5 18:31:42 UTC 2025 aarch64 aarch64 aarch64 GNU/Linux
gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

ldd (Ubuntu GLIBC 2.39-0ubuntu8.7) 2.39
PRETTY_NAME="Ubuntu 24.04.4 LTS"
NAME="Ubuntu"
VERSION_ID="24.04"

Tankar kring Host och Target:
Pi Zero 2W = ARMv8-A, Cortex-A53, 64-bit (aarch64). Host har nyare glibc. Pi har gcc installerad.

Cross-kompilering är bra för många orsaker , då cross kompilering är bara byggprocessen , när binären körs på zeron beteer sig den identiskt oavsätt vart jag byggde den, det blir samma maskinkod cross-kompilering handlar om vart bygget sker. 

En faktor som spelar roll här är vart ben byggs Hosten är naturligtvis snabbare i detta fallet  , att bygga en hel distro (Yocto) på zeron är kanske omöjligt pga av sitt minne.
Även om GCC finns på Target så kommer den att väljas bort senare i bygget.
cross-komplimering ger även en möjlighet för reproducerbarhet/CI.
Skrapade på Ytan om QUEMU kunskap som används för att testa embedded-images utan fysisk hårdvara. 
