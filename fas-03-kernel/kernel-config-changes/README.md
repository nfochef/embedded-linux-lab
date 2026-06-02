# Kernel config ändringar

Snapshot av min anpassade `board/qemu/aarch64-virt/linux.config`
efter modifikationer från Buildroot default.

## Modifikationer

1. **CONFIG_PRINTK_TIME=y** kernel-tidsstämplar är nu inbyggda vilket dom bör vara.
   Tidigare löst med runtime flagga `printk.time=1` i command line
   (se fas-02/NOTES.md, pass 2.8.6). Nu kompilerat in i den.

2. **CONFIG_GCC_PLUGINS not set** automatiskt tillagd av
   `make linux-update-defconfig`, ingen aktiv ändring från min sida.

## Applicering

För att återskapa min image:
```bash
cp linux.config ~/ws/buildroot/board/qemu/aarch64-virt/linux.config
cd ~/ws/buildroot
make linux-rebuild
```
