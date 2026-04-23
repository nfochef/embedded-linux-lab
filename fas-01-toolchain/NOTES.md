
embedded-linux-lab on  main<br>
❯ uname -m<br>
uname -a<br>
gcc --version<br>
ldd --version | head -1<br>
cat /etc/os-release | head -3<br>
aarch64<br>
Linux "XVX" 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Thu Jun  5 18:31:42 UTC 2025 aarch64 aarch64 aarch64 GNU/Linux<br>
gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0<br>
Copyright (C) 2023 Free Software Foundation, Inc.<br>
This is free software; see the source for copying conditions.  There is NO<br>
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.<br>

ldd (Ubuntu GLIBC 2.39-0ubuntu8.7) 2.39<br>
PRETTY_NAME="Ubuntu 24.04.4 LTS"<br>
NAME="Ubuntu"<br>
VERSION_ID="24.04"<br>

Tankar kring Host och Target:<br>
Pi Zero 2W = ARMv8-A, Cortex-A53, 64-bit (aarch64). Host har nyare glibc. Pi har gcc installerad.<br>

Cross-kompilering är bra för många orsaker, då cross kompilering är bara byggprocessen , när binären körs på zeron beteer sig den identiskt oavsätt vart jag byggde den, det blir samma maskinkod cross-kompilering handlar om vart bygget sker. <br>

En faktor som spelar roll här är vart ben byggs Hosten är naturligtvis snabbare i detta fallet  , att bygga en hel distro (Yocto) på zeron är kanske omöjligt pga av sitt minne.<br>
Även om GCC finns på Target så kommer den att väljas bort senare i bygget.<br>
cross-komplimering ger även en möjlighet för reproducerbarhet/CI.<br>
Skrapade på Ytan om QUEMU kunskap som används för att testa embedded-images utan fysisk hårdvara. <br>


2:
sudo apt install -y gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu file<br>
aarch64-linux-gnu är min toolchain...<br>
aarch64-linux-gnu-gcc --version  (Vilken version jag har och sedan vad min target har : <br>
aarch64-linux-gnu-gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0<br>
byggde hello.c<br>
<img width="796" height="309" alt="Skärmbild 2026-04-23 100158" src="https://github.com/user-attachments/assets/38bfd836-d5b9-4f5b-b5e2-9ac9b3d0b073" />
testar att köra:
<img width="1613" height="561" alt="Skärmbild 2026-04-22 172612" src="https://github.com/user-attachments/assets/12f8feb3-dba5-46a5-8a96-00cbd03e83b2" />




Paket som finns vid körning: dpkg -l | grep -E "aarch64|arm64-cross" | awk '{print $2}' ::<br>
binutils-aarch64-linux-gnu<br>
cpp-13-aarch64-linux-gnu<br>
cpp-aarch64-linux-gnu<br>
g++-13-aarch64-linux-gnu<br>
g++-aarch64-linux-gnu<br>
gcc-13-aarch64-linux-gnu<br>
gcc-aarch64-linux-gnu    = Kompilatorn<br>

Notering: Jag saknar verifiering på att det är en cross kompilation genom kommandot åvan samt så finns några dubletter av paket (-13), : md5sum gav identiska hashar för hello-native och hello-pi. gcc och aarch64-linux-gnu-gcc är i praktiken samma kompilator på min ARM64-WSL2.<br>
Det är inte riktig cross-kompilering, det är native ARM64-bygge med två olika kommandonamn som råkar ge samma resultat.<br>

<img width="1068" height="145" alt="Skärmbild 2026-04-23 100544" src="https://github.com/user-attachments/assets/32cbc660-df66-422c-92bc-9b7240009aaf" />
<img width="1072" height="118" alt="Skärmbild 2026-04-23 100513" src="https://github.com/user-attachments/assets/6f6cdc3e-e47a-46d5-83a9-564e98a517c2" />

Problemet är att jag inte kan riktigt få en corss-komliering på min snapdragon X(ARM64) Då jag använder WSL2 för virutalisering och ARM windows kan endast köra ARM64-distributioner natively i WSL2. MS stödjer inte en x86_64-WSL2-distribution för ARM-Windows. <br>
Just nu har jag två val:
Docker med x86-emulation<br>
Fördelar med Docker: Att jag kan köra en riktig x86->ARM utan att behöva röra windows installationen.<br>
Nackdel med Docker: Den blir lite långsammare.<br>

Det andra valet jag har att göra är att skippa x86 helt och fokusera på att gå vidare med Buildroot där cross erfarenheten kommer naturlig via Buildroots egna toolchain.<br>
Detta är det jag kommer att fokusera på istället för att jobba med Docker.<br>


