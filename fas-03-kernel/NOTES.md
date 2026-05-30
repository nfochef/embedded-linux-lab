# Kernel

### menuconfig tolka och sök i options

cd ~/ws/buildroot<br>
make linux-menuconfig<br>

<img width="1070" height="386" alt="Skärmbild 2026-05-26 182133" src="https://github.com/user-attachments/assets/2affd6ae-5f2f-4c32-97c0-e9a1fc72e2bb" />

<img width="1166" height="228" alt="Skärmbild 2026-05-26 181452" src="https://github.com/user-attachments/assets/f22a61fd-5413-4395-a1d0-175bf1da11be" />

Tryck "/" i menuconfig för att söka på CONFIG namn.
<img width="709" height="208" alt="Skärmbild 2026-05-26 182332" src="https://github.com/user-attachments/assets/179ab2e1-3b58-40ba-af6d-6fb51263828f" />






### Buildroots qemu/aarch64-virt/linux.config och vad seed:en gör<br>

76 rader som medvetna kernel-options för hela QEMU-virt-användning.<br>
Allt annat ärvs som default från kerneln.<br>
Kritiska virtio-options utan dessa så går det ej att boota QEMU.<br>
 - CONFIG_VIRTIO_BLK     läs rootfs.ext2<br>
 - CONFIG_VIRTIO_NET      nätverk<br>
 - CONFIG_VIRTIO_PCI     PCI transport<br>
 - CONFIG_VIRTIO_MMIO     MMIO transport<br>
 - CONFIG_VIRTIO_CONSOLE   serial console<br>

Det här är precis varför min pi-defconfig inte bootade i QEMU förra kapitlet min pi-konfiger saknade virtio (pi-hardvaran har inte<br>
virtio). När jag bytte till qemu_aarch64_virt blev allt fixat.<br>

Andra som är värda att nämna.<br>
 - CONFIG_MODULES=y      kan ha LKM:er<br>
 - CONFIG_OVERLAY_FS=y   container stöd<br>
 - CONFIG_BRIDGE=m     som modul men inte inbyggt<br>

PL011 är PrimeCell-namnet på ARM:s standard UART-controller. När jag loggar in i QEMU så pratar den med en virtuell PL011.Däremot på en riktig pi Zero 2W finns där en liknande UART, men inte den PL011-versionen.<br>
Exempel på Hårdvara som visar PL011.<br>
 - CONFIG_SERIAL_AMBA_PL011=y    ARM PrimeCell UART (min konsoll)<br>
 - CONFIG_SERIAL_AMBA_PL011_CONSOLE=y   och den konsol-funktionalitet<br>
 - CONFIG_RTC_DRV_PL031=y       ARM PrimeCell Real-Time Clock<br>
 - CONFIG_ARM_SMMU_V3=y     IOMMU för ARM"<br>

Kerneln behöver veta vilka hårdvarustubbar den ska prata med. Saknas en drivrutin så går det inte att kommunicera med den!.<br>
Min Seed-fil, 76 rader, 4200 rader expanderat och en kernel-Image på 13 MB.<br>
76 rader medvetna kernel-options för QEMU-virt användning.<br>
Allt annat ärvs som default från kerneln.<br>

### Aktivering av PRINTK_TIME<br>
kernel modifikation

Slog på CONFIG_PRINTK_TIME i menuconfig (Kernel hacking -> printk and dmesg options -> Show timing information on printks).<br>
Kerneln byggdes om via make linux-rebuild. Vilket gick snabbt. <br>
Verifiering vid boot:<br>
- dmesg har [X.XXXXXX]-tidsstämplar from start<br>
- INTE behövdes printk.time=1 på command line<br>
- /proc/cmdline visar bara rootwait root=/dev/vda console=ttyAMA0<br>

Nu har jag kompilerat IN funktionaliteten.<br>
Detta är skillnaden mellan "konfigurera" och "modifiera" kerneln.<br>

WSL2:s egna kernel har också PRINTK_TIME=y<br>
På desktop/server är de flesta säkerhets/debug-options PÅ.<br>
I embedded måste man välja varje en medvetet och man väljer ofta AV för att spara plats och boot-tid.<br>

<img width="887" height="425" alt="Skärmbild 2026-05-27 075737" src="https://github.com/user-attachments/assets/e965187f-2aee-4924-9e62-1e9fc4b08d8d" /><br>


Verifiering av lyckad aktivering:<br>
<img width="536" height="49" alt="Skärmbild 2026-05-27 081309" src="https://github.com/user-attachments/assets/5bfe3ed3-0998-4a10-b4fe-e03ca2b82eb9" /><br>

Sparat tillbaka till Buildroot-trädet med "make linux-update-defconfig".<br>
Även har jag kopierat linux.config till mitt repo som snapshot.<br>
"make linux-update-defconfig" la även till en rad om CONFIG_GCC_PLUGINS den savedefconfig-mekaniken är petig med att<br>
fånga exakt nuvarande state, även för options som behöver explicit "is not set" deklaration.<br>



### Kernel-randomness och seed-filer

Noterade att vid poweroff i QEMU så står det att "Saving 256 bits of creditable seed for next boot" och jag vistte inte vad det var som den sparade. Sökning gav:<br>
Kerneln behöver entropi för säkerhetsfunktioner (kryptering, ASLR).<br>
Vid boot är entropi-poolen tom och ger problem på embedded-enheter utan mus/tangentbord/nätverkstrafik etc..<br>

Lösningen är spara slump över boots.<br>
 - Shutdown: kerneln skriver 256 bitar till /var/lib/seedrng/<br>
 - Boot:     S01seedrng-script seedar kerneln med samma 256 bitar<br>
  
Vid varje boot som börjar med ackumulerad entropi från tidigare sessions.<br>
Seed-filen ändras efter användning. Om du klonar ett SD-kort-image så har båda kopiorna samma seed till samma slump vid första boot. Det är en sårbarhet i embedded produktion då den ska generera ny seed vid första boot per enhet.<br>

SEED = En template, startkonfiguration eller en basconfig. <br>
Entropi = hur mycket oförutsägbar slumpmässighet som finns tex . Bra entropi = 5f3a9Xc2d4Ye7b... , Dålig = 1234..<br>

Kernel-crash-felsökning. cat /proc/sys/kernel/tainted visar en numerisk flagga. dmesg | grep -i taint visar alla taint-orsaker.<br>

# kernel-modul (LKM)
Tanken att lära mig att skriva en .ko-fil som ska bli en del av kerneln vid runtime. jag ska loada in den, ser meddelandet i dmesg, och kan ta ut den igen.<br>
Det är en kernel-modul är C-kod som körs i kernel-space istället för userspace. Den har kerneln's privilegier, direkt access till hårdvara och kan crasha systemet om man gör fel...<br>
Verkligen stor skillnad mot applications kod..<br>


När man installerar nya enheter eller felsöker befintliga är **insmod, rmmod, lsmod, modinfo och dmesg** dom dagliga verktygen.<br>
Min modul är en del av kerneln när den är laddad. Med precis samma privilegier som kerneln själv. Det är därför moduler kan crash:a hela systemet om man gör fel och det ger en anledning till varför vi alltid testar i QEMU innan<br>
den hamnar på fysisk hårdvara.<br>

Väl användbara Buildroot kommandon vid bygge av kernel: <br>
 - make linux-menuconfig, öppna kernel-menuconfig<br>
 - make linux-rebuild, bygg om kerneln efter konfig-ändring<br>
 - make linux-update-defconfig, spara nuvarande konfig som ny defconfig<br>
 - make linux-dirclean, radera kernel-byggkatalogen, börja om<br>
 - make linux-source, bara ladda ner källkoden<br>


 # /proc, /sys och /dev
 Jag behöver förstå tre virtuella filsystem som är kernels gränssnitt mot userspace. De tre proc, sys och dev alla är virutella och tar ingen plats på rootfs kernel genererar innehållet dynamiskt.<br>
 Linux har en designprincip "allt är en fil". Kernel information och hårdvara exponeras som filer, Tre virtuella filsystem gör detta:<br>

 /proc: Processer och kernel information.<br>
  - förut var det för process information.<br>
  - innehåler också kernel parametrar, minne, CPU info.<br>
  - Filer finns ej på Disk, Kernel genererar innehållet när man läser.<br>

 /sys: hårdvara och drivrutiner(sysfs)<br>
  - En mer modern strukturerad översikt av enheter, drivisar och kernel objekt.<br>
     - hierarkisk<br>

 /dev: enhetsnoder dvs devtmpfs<br>
  - Filer som reprensenterar hårdvaruenheter.<br>
  - Man läser och skriver till dem för att kunna prata med hårdvaran.<br>
  - /dev/dva är min disk, /dev/null är papperskorgen, /dev/zero ger nollor.<br>

 Sensor svarar inte gå till /dev/i2c-*, /sys/class/, dmesg<br>
 Driver laddad?  kolla /proc/modules, /sys/module/<br>
 Temperatur kolla /sys/class/thermal/.../temp<br>
 Nätverk uppe? se /sys/class/net/<br>




  
---
cat /proc/1/ ger mig init processen, då varje filmapp i /proc/1/ är ett fönster in i initprocessen .  Detta finns för varje process. verkty som ps, top och htop fungerar så att de läser /proc/pid/ katalogerna och sammanställer dem i det här är det bara filer inget "API som processerar".<br>

/sys/class/ ger enheter efter typ , kernels taxonomi över hårdvarutyper<br>

Klassiska /dev experiment dök upp vid sökning.<br>
echo "detta försvinner" > /dev/null<br>
head -c 16 /dev/zero | hexdump -C<br>
head -c 16 /dev/urandom | hexdump -C<br>

Enhetsnoder med deras struktur;<br>
<img width="649" height="297" alt="Skärmbild 2026-05-28 112822" src="https://github.com/user-attachments/assets/721e4408-aafa-45d6-a40f-a63ab37c3e33" /><br>

Tolkning av bokstäver, C = character device , b = block device , s = socket, l = symlink<br>
Siffror i stället för storlekar | 1, 3 = null | 1, 5 = zero | 1,1 = mem | 1, 11 = kmsg Dessa är Kallade MAJOR 1 och är "memory devices" Minor identifierar vilken.<br>
MAJOR 5 är "TTY relaterade" 5, 1 = console.<br>
På så vis vet kernel vart en operation ska skickas skriver jag till /dev/null så skickar kernel "major 1, minor 3 och dirigerar til minnes drivrutinen's null hantering tillex.<br>


# Device Tree basics
Kernal använder sig av en datastruktur som kallas device tree även känd som "flattend device tree or FDT"<br>
QEMU genererar device tree dynamiskt baserat på mina -device-flaggor. Device tree har fil som beskriver hårdvaran tex att det finns en UART på adress 0x80000000 osv.<br>
samma kernel kan köras på tusentals boards men device tree filen skiljer sig åt. utan device tree skulle tex ARM kernel behöva hårdkoda varje hårdvarukombination. På ARM kan hårdvara inte upptäckas automatiskt (till skillnad från x86) <br>
Det finns tre filformat;<br>
 - .dts - Device Tree Source. Mänskligt läsbar text. Det du editerar.<br>
 - .dtb - Device Tree Blob. Kompilerad binär. Det kerneln läser.<br>
 - .dtsi - Source Include. Delas mellan flera boards.<br>
Kompilatorn heter dtc (device tree compiler)<br>

Stdout-path i chosen-noden bestämmer default-konsol.
rng-seed och kaslr-seed = säkerhetsdetaljer från QEMU/bootloader.





