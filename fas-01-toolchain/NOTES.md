
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


2:
sudo apt install -y gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu file
aarch64-linux-gnu är min toolchain...
aarch64-linux-gnu-gcc --version  (Vilken version jag har och sedan vad min target har : 
aarch64-linux-gnu-gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
byggde hello.c
<img width="796" height="309" alt="Skärmbild 2026-04-23 100158" src="https://github.com/user-attachments/assets/38bfd836-d5b9-4f5b-b5e2-9ac9b3d0b073" />
testar att köra:
<img width="1613" height="561" alt="Skärmbild 2026-04-22 172612" src="https://github.com/user-attachments/assets/12f8feb3-dba5-46a5-8a96-00cbd03e83b2" />




Paket som finns vid körning: dpkg -l | grep -E "aarch64|arm64-cross" | awk '{print $2}' ::
binutils-aarch64-linux-gnu
cpp-13-aarch64-linux-gnu
cpp-aarch64-linux-gnu
g++-13-aarch64-linux-gnu
g++-aarch64-linux-gnu
gcc-13-aarch64-linux-gnu
gcc-aarch64-linux-gnu    = Kompilatorn

notering: Jag saknar verifiering på att det är en cross kompilation genom kommandot åvan samt så finns några dubletter av paket (-13), : md5sum gav identiska hashar för hello-native och hello-pi. gcc och aarch64-linux-gnu-gcc är i praktiken samma kompilator på min ARM64-WSL2.
Det är inte riktig cross-kompilering, det är native ARM64-bygge med två olika kommandonamn som råkar ge samma resultat.

<img width="1068" height="145" alt="Skärmbild 2026-04-23 100544" src="https://github.com/user-attachments/assets/32cbc660-df66-422c-92bc-9b7240009aaf" />
<img width="1072" height="118" alt="Skärmbild 2026-04-23 100513" src="https://github.com/user-attachments/assets/6f6cdc3e-e47a-46d5-83a9-564e98a517c2" />
