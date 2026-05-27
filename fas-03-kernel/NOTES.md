# Kernel

### menuconfig tolka och sök i options

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
"CONFIG_SERIAL_AMBA_PL011=y    ARM PrimeCell UART (min konsoll)<br>
CONFIG_SERIAL_AMBA_PL011_CONSOLE=y   och den konsol-funktionalitet<br>
CONFIG_RTC_DRV_PL031=y       ARM PrimeCell Real-Time Clock<br>
CONFIG_ARM_SMMU_V3=y     IOMMU för ARM"<br>
