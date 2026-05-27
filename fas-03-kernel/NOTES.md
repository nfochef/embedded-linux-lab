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
Kerneln byggdes om via make linux-rebuild. Vilket gick ganska snabbt. <br>
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


### Kernel-randomness och seed-filer

Noterade att vid poweroff i QEMU så står det att "Saving 256 bits of creditable seed for next boot" och jag vistte inte vad det var som den sparade. Sökning gav:<br>
Kerneln behöver entropi för säkerhetsfunktioner (kryptering, ASLR).<br>
Vid boot är entropi-poolen tom och ger problem på embedded-enheter utan mus/tangentbord/nätverkstrafik etc..<br>

Lösningen är spara slump över boots.<br>
 - Shutdown: kerneln skriver 256 bitar till /var/lib/seedrng/<br>
 - Boot:     S01seedrng-script seedar kerneln med samma 256 bitar<br>
  
Vid varje boot som börjar med ackumulerad entropi från tidigare sessions.<br>
Seed-filen ändras efter användning. Om du klonar ett SD-kort-image så har båda kopiorna samma seed till samma slump vid första boot. Det är en sårbarhet i embedded produktion då den ska generera ny seed vid första boot per enhet.<br>

SEED = 
Entropi = 







