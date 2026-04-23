
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


# 2:<br>
sudo apt install -y gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu file<br>
aarch64-linux-gnu är min toolchain...<br>
aarch64-linux-gnu-gcc --version  (Vilken version jag har och sedan vad min target har : <br>
aarch64-linux-gnu-gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0<br>
<br>
byggde hello.c för att ha en fil att testa och förstå hur filer beteer sig och vad dom säger.<br>
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

<img width="1068" height="145" alt="Skärmbild 2026-04-23 100544" src="https://github.com/user-attachments/assets/32cbc660-df66-422c-92bc-9b7240009aaf" /><br>
<img width="1072" height="118" alt="Skärmbild 2026-04-23 100513" src="https://github.com/user-attachments/assets/6f6cdc3e-e47a-46d5-83a9-564e98a517c2" /><br>

Problemet är att jag inte kan riktigt få en corss-komliering på min snapdragon X(ARM64) Då jag använder WSL2 för virutalisering och ARM windows kan endast köra ARM64-distributioner natively i WSL2. MS stödjer inte en x86_64-WSL2-distribution för ARM-Windows. <br>
Just nu har jag två val:
Docker med x86-emulation<br>
Fördelar med Docker: Att jag kan köra en riktig x86->ARM utan att behöva röra windows installationen.<br>
Nackdel med Docker: Den blir lite långsammare.<br>

Det andra valet jag har att göra är att skippa x86 helt och fokusera på att gå vidare med Buildroot där cross erfarenheten kommer naturlig via Buildroots egna toolchain.<br>
Detta är det jag kommer att fokusera på istället för att jobba med Docker.<br>

# 3:<br>
Statisk vs dynamisk länkning, tanken är att förstå vad en ELF-binär faktiskt är, vad den beror på och varför jag har sett att den dyker ofta upp i embedded projekt.
testar att bygga aarch64-linux-gnu-gcc -o hello-dyn hello.c<br>
aarch64-linux-gnu-gcc -static -o hello-static hello.c <br> -static säger till linkern att ta med alla bibliotek i binären, för att jämnföra storleken på filen:<br>

<img width="1131" height="350" alt="Skärmbild 2026-04-23 105234" src="https://github.com/user-attachments/assets/3a272b2b-39f7-417e-9215-09bee86e7d99" /><br>

-d = dynamic section. Den innehåller instruktioner till den dynamiska länkaren vid start, vilka .so filer att ladda , symboler att resolva etx etc..<br>
ldd - Listar dynamiska bibliotek en binär behöver vid runtime<br>ldd visar den statiska binären som 'not a dynamic executable' vilket är ganska rimligt, för den har inga dynamiska beroenden, allt är ju inbakat.<br>
vad säger då outputen?<br>
linux-vdso.so.1, Virtuellt bibliotek från kärnan (för snabba systemanrop)<br>
libc.so.6 ,Standard C-biblioteket (printf, malloc osv.) <br>
Laddas från /lib/aarch64-linux-gnu/libc.so.6<br> 
/lib/ld-linux-aarch64.so.1 ,Dynamiska länkaren (loadern) som startar programmet<br>

Så hello-dyn är dynamiskt länkad och behöver externa bibliotek MEN hello-static är statiskt länkat och alla bibliotek är redan inbakade i binären så inga externa .so filer behövs. DETTA är lite ögonöppnande och ökar mitt förstående för dependencies!<br>

#### ELF = Executable and Linkable Format. Detta är binärformatet på Linux. Varje Linux-binär är ELF.
De används för :<br>
körbara program (executables)<br>
delade bibliotek (.so)<br>
objektfiler (.o)<br>
core dumps<br>

Verktygen som kan vara bra att kunna kring ELF listas som<br>
file - ser vad det är för typ av fil
ldd - listar dynamiska beroenden
readelf - visar ELF-strukturen (headers, sektioner)
objdump - disassembler + info
nm - listar symboler

Jag körde med -readelf i mitt hello-dyn och det kom ut intressant information kring själva ELF-headern det så kallade "ID'Kortet" för binären: <br>

<img width="1348" height="619" alt="Skärmbild 2026-04-23 112608" src="https://github.com/user-attachments/assets/0f4f64b8-2534-40a8-9b0f-c5b7d4755f81" />
<br>


Som vi kan se här är så bekräftar det att det är en ELF fil Klass EFL64 menas med att det är en 64 bitars binär.<br>
Data: little endian som har lägsta byte först vilket är standard för ARM64/x86_64.<br>
Machine: AArch64 visar att den är Kompilerad för ARM 64-bit (tex. Raspberry Pi 64-bit, ARM-servrar)<br>
Type: DYN (Position-Independent Executable file) är en viktig punkt som man ska titta lite extra på den visar att det är ett program (inte ett bibliotek) det står DYN, det betyder att det är en PIE (Position Independent Executable)<br>
Vilket menas med att den:<br>
Laddas på slumpad adress (ASLR)<br>
Ser ut som ett delat bibliotek internt<br>
Standard i moderna Linux (säkerhetsskäl)<br>
Startpunkten entry point 0x780 säger att här börjar exekveringen och att det inte är min main() direkt utan runtime-startkod(crt, loader setup)
Headers beskriver hur filen är organiserad. Start of program headers 64 bytes, direkt efter ELF-headern. Antal nummer av headers 9 st det beskriver vad som ska laddas i minnet(segment)
Start of section headers ligger lmycket längre bak i filen och antal sektions headers är 28 sektioner som .text .data .bss .rodata etc..<br>
Skillnaden mellan sektion och program headers är att program headers används av OS när programmet körs och säger ladda detta i minnet.<br>
Sektion headers används av verktyg tex linker, debugger osv dom säger el pekar lite på att detta är kod detta är data och detta är symboler.. <br>

ELF headern är på 64 bytes <br>
Programheadern är på 56 bytes Styck<br>
Sections headers är på 64 bytes styck<br>

Om man vill veta exakt vad som laddas i minnet av filen kan man används sig av readelf -l eller om man vill se alla sektioner så fungerar readelf -S med.<br>
detta gav mig en koppling till en större förståelse för ldd då readelf -h visar vad filen är och ldd visar ju vilka bibliotek som behövs vid körning och då blir det en koppling via ELF-strukturen<br>

Kör jag, aarch64-linux-gnu-readelf -h hello-static så säger skillnaden något vitigt:<br>
<img width="683" height="416" alt="Skärmbild 2026-04-23 125822" src="https://github.com/user-attachments/assets/034f7f65-8419-4ff5-9d0d-b38a8ca74f87" /><br>

EXEC = statiskt program<br>
Allt finns i filen<br>
Det körs direkt<br>
DYN - dynamiskt program (PIE)<br>
Den behöver ladda bibliotek (libc.so.6)<br>
Och startas via dynamisk länkare<br>
'I ELF betyder alltså:'<br>
EXEC = fristående<br>
DYN = beroenden behövs<br>


Om jag nu väljer att köra aarch64-linux-gnu-readelf -V hello-dyn | head -40    så får jag fram versions kravpå bibliotek i min ELF (dvs vilken version av libc och loader som krävs.)<br>
<img width="717" height="324" alt="Skärmbild 2026-04-23 165156" src="https://github.com/user-attachments/assets/2c4f670f-2058-41d9-9d1d-e6baece4ef87" /><br>

Detta är bra information att lära sig då detta gör att du kan se om man får fel vid körning som att programmet inte startar el när man bygger program för andra maskiner.<br>
då kan man använda detta för att se minsta glibc-version den binären kräver och avgöra om den kommer fungera på målmaskinen.<br>
el om den funkar i docker containern men krashar i production då kan det vara skillnad i libc-versionen.<br>








