# Fas 2 Buildroot
Källkod , konfiguration in , image ut.
Reproducerbarhet är minst lika viktigt som minimalism som den erhåller, produkten ska kunna byggas om identiskt om ~10 år.<br>

Tankar innan jag börjar med att bygga Buildrots:
Jag har inte byggt en Kconfig tidigare, jag kommer använda mig av material som boken 
**Mastering Embedded Linux Development** <br>
**https://buildroot.org/docs.html**<br>
**https://buildroot.org/downloads/manual/manual.html**<br>
och Dels Ai för stöd och struktur.<br>

Klonat  från repo https://gitlab.com/buildroot.org/buildroot.git<br>
buildroot on  HEAD (0141ca3)<br>
❯ git describe<br>
ls configs/raspberrypizero2w*<br>
2026.02.1<br>
configs/raspberrypizero2w_64_defconfig  configs/raspberrypizero2w_defconfig<br>

Valde raspberrypizero2w_64_defconfig för konsistens med min nuvarande 64-bit-setup. Skillnaden mellan defconfigs är target-arkitektur (32 vs 64-bit ARM), trots att det samma fysiska hårdvara.<br>

Nyfikenheten tog mig mot att kolla in Kconfig för att navigera runt i menyerna för att greppa lite på hur det är gjort, noterade även att det finns väldigt många tillängliga paket som var i kategorier.<br>
Körde make menuconfig, resultatet blir:<br>
<img width="890" height="419" alt="Skärmbild 2026-04-26 073047" src="https://github.com/user-attachments/assets/ed2ce965-47f3-4f3c-95ed-23b55a9c8b6d" /><br>
<br>

Bygget startas genom kommando som:
make - startar bygget
2>&1 - slår ihop stdout och stderr
| tee build.log - visar output i terminalen och sparar till fil.

make 2>&1 | tee build.log<br>

<img width="795" height="128" alt="Skärmbild 2026-04-26 080347" src="https://github.com/user-attachments/assets/f782195d-173f-46d0-9eab-40e25bbca648" /><br>
<br>
Så Buildroot i korthet:<br>
Extremt liten footprint <br>
Flexible konfiguration<br>
Snabb uppstart<br>
Mindre Beroenden <br>
Standaliserad Unix känsla<br>
Beprövat i hela ekosystemet<br>
<br>
Buildroot vägrar bygga med skadat PATH så den logiska lösningen för spaces i PATH är att de bryter sönder Make-byggen... 
I WSL2 ärver Windows PATH automatiskt, och flera Windows-paths innehåller spaces detta blir väldigt visuellt när man kör<br>
echo "$PATH"<br>
echo "$PATH" | cat -A<br>
Min PATH är dublicerad fem gånger och startar en ny shell varje gång, utan att kontrollera om den redan finns..<br>
Detta måste jag fixa innan jag kan börja bygga Buildroot.<br>
Jag gör en tillfällig lösning på problemet genom att skapa en ren PATH bara för Buildroot bygget. export PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:$HOME/.local/bin"<br>

Första bygget kraschade efter 2h35m på SSL-fel mot github.com vid kernel-nedladdning.. jag bygger om och hoppas att det går denna gången , denna gång bör det inte ta lika lång tid då Buildroot är inkrementellt.<br>
<img width="311" height="17" alt="Skärmbild 2026-04-26 110752" src="https://github.com/user-attachments/assets/1df3b061-258b-4210-9c88-482e96700907" /><br>
Tur att alla redan byggda dependencies behålls.<br>
Build klar!<br>
<img width="525" height="62" alt="Skärmbild 2026-04-26 120315" src="https://github.com/user-attachments/assets/ca23e5b0-2be4-4b4a-ac4a-6b716a697928" /><br>
ARM64 den har rätt arkitektur : Check!
boot executable ..det är inte en vanlig ELF utan den är en specifik bootbar.<br>


Genom att mounta image-filen via sudo mount -o loop,ro rootfs.ext2 ~/mnt/buildroot-rootfs så kan jag kolla in Linux-distron<br>
<img width="653" height="658" alt="Skärmbild 2026-04-27 083915" src="https://github.com/user-attachments/assets/83d5f438-3f7c-4ab3-81fc-f8631f6c2e73" /><br><br>

Så varje rad med -> är en symlink till busybox det vill säga att det är en fil som pekar på en annan fil själva Busybox är ett program som samlar flera Unix-verktyg i en enda exekverbar fil.<br>
Om en symlink som heter ls finns och pekar på busybox, så innebär det att när du skriver ls i terminalen så körs faktiskt busybox ls..<br>
Jag trodde den komprimerar filerna vilket var fel för det är en enda C-binär-fil och används där det inte fins utrymme el behovet av att ha många seperata program.<br>
Busybox är vekligen minimalt och tar inte stor plats (en enda kompakt binär som ersätter många klassiska Unix-verktyg) <br>

**Image** Linux Kernel ARM64                    
**bcm2710-rpi-zero-2-w.dtb** = Device tree blob och beskriver hårdvaru modellen  
**boot.vfat** = Dess FAT system på SD kort har Pi:n alltid en FAT-partition först. Vilket är den enda typ av filsystem som Pi firmware direkt kan läsa vid uppstart.     **genimage.cfg** = Är ett "recept" som beskriver hur sdcard.img ska sättas ihop av de andra delarna.              
**rootfs.ext2** = Där hela Linux systemet bor dvs /bin/user/bin/etc/home/var/lib/usr/lib/boot/dev/proc/sys etc. man kan kalla det för rotfilsystem.              
**rootfs.ext4 - rootfs.ext2** = är en symlink till samma fil, två namn bara en fil.
**rpi-firmware/** = Behövs för boot och är GPU firmware och den är inte open-source "Broadcom."              
**sdcard.img** = Allt ihopsatt till en slutproduct som är en flashbar fil. 


