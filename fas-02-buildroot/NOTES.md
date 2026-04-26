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

Buildroot vägrar bygga med skadat PATH så den logiska lösningen för spaces i PATH är att de bryter sönder Make-byggen... 
I WSL2 ärver Windows PATH automatiskt, och flera Windows-paths innehåller spaces detta blir väldigt visuellt när man kör<br>
echo "$PATH"<br>
echo "$PATH" | cat -A<br>
Min PATH är dublicerad fem gånger och startar en ny shell varje gång, utan att kontrollera om den redan finns..<br>
Detta måste jag fixa innan jag kan börja bygga Buildroot.<br>
Lösning på problemet blir att skapa en ren PATH bara för Buildroot bygget.<br>
