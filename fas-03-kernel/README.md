## /proc, /sys och /dev

Vad som fasnar hos mig:

1. **Kernel modifikation är hanterbart.** Att lägga till CONFIG_PRINTK_TIME
   tillsynes var en menuconfig-toggle med en snabb rebuild.
   Brytningen mellan "användare av kerneln" och "modifierare
   av kerneln" var inte så omfattande som jag trodde.

2. **LKM är portten till driver utveckling.** Att skriva en ".ko" som
   laddas och kör i kernel-space samt avlastas är det fundamentet för
   all driver skrivning. När jag senare ska skriva en SX1302 LoRa-driver
   för Open Habitat är arbetsflödet exakt detsamma men bara med riktigt
   hårdvaru interaktion den gången.

3. **Felsökning för embedded:**
   - Hårdvarurelaterat fel bör man kolla device tree-noden, dmesg, /sys
   - Modul-relaterat se över /proc/modules, dmesg, taint-status
   - Userspace pratar inte med kerneln utan kontrollera att /dev-nod existerar

(/proc, /sys, /dev) var luddigt till en början innan jag fick den mentala bilder över hur allt hängde ihop." men det var mycket
applicerbart. /sys eller /proc. kommer jag nog jobba mest med när man har fått mer kött på benen , det känns som senoirer jobbar mycket med detta. 

Den mentala builden för Yocto börjar falla på plats samttidigt som jag reflekterar över " Kommer jag gilla arbetsuppgifterna"?? 

