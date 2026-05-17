<img width="609" height="83" alt="Skärmbild 2026-05-17 161555" src="https://github.com/user-attachments/assets/87c67fae-cfa4-452a-9d13-d9cb0e40bd6f" />
<img width="609" height="83" alt="Skärmbild 2026-05-17 161555" src="https://github.com/user-attachments/assets/1e722578-de65-47d8-8b97-24dd70432d14" />
# Fas 2 Buildroot
Källkod , konfiguration in , image ut.
Reproducerbarhet är minst lika viktigt som minimalism som den erhåller, produkten ska kunna byggas om identiskt om ~10 år.<br>

Innan jag börjar med att bygga Buildrots så vill jag klargöra att,
jag har inte byggt en Kconfig tidigare och jag kommer använda mig av material som boken. 
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
Busybox är vekligen minimalt och tar inte stor plats (en enda kompakt binär som ersätter många klassiska Unix-verktyg) Samt BusyBox startar snabbare eftersom systemet bara behöver slå upp och läsa in en enda binär istället för många små filer, vilket minskar I/O- och filsystem-overhead.<br>

**Image** Linux Kernel ARM64                    
**bcm2710-rpi-zero-2-w.dtb** = Device tree blob och beskriver hårdvaru modellen  
**boot.vfat** = Dess FAT system på SD kort har Pi:n alltid en FAT-partition först. Vilket är den enda typ av filsystem som Pi firmware direkt kan läsa vid uppstart.     **genimage.cfg** = Är ett "recept" som beskriver hur sdcard.img ska sättas ihop av de andra delarna.              
**rootfs.ext2** = Där hela Linux systemet bor dvs /bin/user/bin/etc/home/var/lib/usr/lib/boot/dev/proc/sys etc. man kan kalla det för rotfilsystem.              
**rootfs.ext4 - rootfs.ext2** = är en symlink till samma fil, två namn bara en fil.
**rpi-firmware/** = Behövs för boot och är GPU firmware och den är inte open-source "Broadcom."              
**sdcard.img** = Allt ihopsatt till en slutproduct som är en flashbar fil. 
<br>
Buildroot-default använder BusyBox<br>
Systemd är alternativet <br>
När ska man välja vad?<br>
Man väljer kanske Systemd när flera tjänster måste startas i rätt ordning, övervakas och automatiskt återstartas. Där väger systemd:s beroendehantering, loggning och service-övervakning tyngre än nackdelen i lagringsstorlek.<br>

Jämnförelse:<br>
Systemet ligger på kanske runt 50-100 binärer i hela systemet.. och jämnför man mot ubuntu som har ca 3000 binärer för att täcka ett "all around" system så får man en väldigt tydlig bild av att Buildrot är överlägs minimal i storlek.
Vad som saknas är ett grafiskt gränsnitt som X-server el Wayland + GUI bibliotek vilket en embedded enhet inte behöver.<br>


# **Boot i QEMU**
QEMU emulerar maskiner i mjukvara. För embedded-arbete betyder det att jag kan testa kernel- och rootfs-ändringar utan att flasha hårdvara genom snabb iteration. Och om ett team har "50" enheter ute i fält kan vi reproducera buggar lokalt utan att skicka enheter fram och tillbaka.<br>
Detta är ju väldigt bra tidsbesparing.<br>
Qemu , tidigare okänt för mig man simulerar en hel maskin, kör hel kernel + rootfs sen kan man göra en User-mode emulation då man kör enskild binär från annan arkitektur. <br>
Installerade med **sudo apt install -y qemu-system-arm qemu-util**<br>
<img width="510" height="64" alt="Skärmbild 2026-04-28 115648" src="https://github.com/user-attachments/assets/cbd524fd-2e47-46c8-814f-5d20750d18cf" /><br>

Innan jag börjar så upptäckte jag att QEMU inte stödjer Pi Zero 2W specifikt och Generisk emulering räcker inte,.
Lösning:<br>
En lösning är att jag skippar PI specifika delar så QEMU tar bara Image och rootfs.ext2 och använder dem på en generisk virtuell maskin , det kommer funka och jag får login. men blir inte en Pi emulering.<br>

Kommando för att starta QEMU med min image och setup:
qemu-system-aarch64 \
  -M virt \
  -cpu cortex-a53 \
  -smp 2 \
  -m 1024 \
  -kernel Image \
  -drive file=rootfs-qemu.ext2,if=none,format=raw,id=hd0 \
  -device virtio-blk-device,drive=hd0 \
  -append "root=/dev/vda console=ttyAMA0 rw" \
  -nographic

Vilket gav ett felmeddelande : **Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(0,0)**
den verkar inte kunna hitta blockenheten, dvs den har inget os att ladda. 
Varför kan den ej hitta enheten som jag bad den om i flaggorna?

-append "root=/dev/vda console=ttyAMA0 rw" — kernel-parametrar:
root=/dev/vda, kerneln ska mounta /dev/vda (vår virtio-disk) som /
console=ttyAMA0 och visa kernel-loggar på serielle UART (ARM-standard)

Problemet är som jag nämde innan att min Image är gjord för Pi Zero 2W och den har inte virtio-stöd och när man tar den till en virtuell maskin som bara har virtio-hårdvara hittar den ingen disk. Det jag kan göra nu är att Ge kerneln en disk den känner igen,
eller ladda om rootfs som en initramfs istället dvs, att den laddas in i RAM av kernel direkt vid boot , ingen blockenhet behövs då.
Jag behövde rådfråga ai om hur jag ska gå till väga och den föreslog att jag skulle bygga en paralell qemu-image.
Då fördelarna var att jag faktiskt få se att min egna distro boota och kunna logga in i den och då vet jag om den funkar, inte att jag tror det funkar.
Det gav mig insikten att en kernel-image är bunden till sina hårdvaruval i konfig genom att flytta den till annan hårdvara kräver matchande drivers eller en helt annan kernel.


Buildroots output/ innehåller både byggcache och artefakter. När man flyttar output/ förlorar man toolchain-byggcache och måste bygga om från noll. För att spara tid mellan parallella builds: behåll output/ per defconfig, växla med mv. För riktigt parallell hantering: använd Buildroots O=path för att specificera output-katalog per bygge.

Då kör vi igång bygget <br>
cd ~/ws/buildroot
tail -f build-qemu.log<br>
<img width="313" height="20" alt="Skärmbild 2026-04-28 154459" src="https://github.com/user-attachments/assets/da0da305-a4eb-478f-8008-2afb91ed5341" /><br>
<br>
<img width="902" height="261" alt="Skärmbild 2026-04-28 154849" src="https://github.com/user-attachments/assets/5ffe99e0-07e2-415c-928c-46e4104bc9b4" />
Så Buildroot utvecklarna har gjort ett start script med rätt QEMU flaggor för denna defconfig "start-qemu.sh". <br>
<img width="548" height="59" alt="Skärmbild 2026-04-28 155611" src="https://github.com/user-attachments/assets/a9c565a7-e1c8-4fa4-b6e4-9787f43c4825" />

Vid inlogg så blev det väl inget wow intryck då det ser ut som vanlig Linux-shell, men när jag tänker på vad jag har skapat så ändras bilden lite jag har byggt ett komplett Linux-system från källkod, från noll. Toolchain, kernel, BusyBox, init, libc, allt och kompilerat med egen Makefile config. Då förstår jag hur stort det är (iallafall för mig).<br>

# Customization
Mål att ändra Buildroot imagen från mall till min image.
 - ändra användarnamn.
 - skapa nytt lösenord.
 - lägga till paket (htop som är ett interaktivt process övervakings verktyg)
 - Bygga om imagen efter detta. 

Detta steg görs allt genom terminalen med kommandot: make menuconfig
<img width="894" height="421" alt="Skärmbild 2026-04-29 084312" src="https://github.com/user-attachments/assets/13e99a73-004a-47f9-b539-c1610872dbba" /><br>

.config är bara en textfil med KEY=value. Hela menuconfig är bara ett gränssnitt för att redigera den. man kunde teoretiskt redigera .config direkt med vim det hade fungerat men menuconfig hanterar dependencies (vissa paket kräver att andra också är aktiverade)

Bygger om efter ändringarna.
**make 2>&1 | tee build-custom.log**

Loggar in och testar att köra # htop:   
<img width="633" height="343" alt="Skärmbild 2026-04-29 091051" src="https://github.com/user-attachments/assets/36caf46d-6e90-4c5b-8f1c-c7f32a295d53" />
Det funkade nu ser vi alla processer..<br>
det jag nu behöver göra är att spara konfigurationen som en egen defconfig vilket är viktigt steg för reproducerbarhet. Mina ändrignar finns bara i output/.config verkar det som om jag nu gör en **make clean** så raderar den bara output/, inte dl/ eller configs/. <br>
Eftersom jag sparade min defconfig till configs/qemu_aarch64_lab_defconfig innan clean kunde jag återskapa konfigurationen direkt. Disciplin: spara som defconfig så fort en customization är klar, sen är clean ofarligt.<br>
<br>
**make savedefconfig BR2_DEFCONFIG=configs/qemu_aarch64_lab_defconfig** <br>
jag behöver bryta ner detta kommando för att förstå det helt:<br>
**savedefconfig** = spara minimal version av min nuvarande konfiguration. den tar min .config och rensar bort allt som är standardvärden så bara ändringar blir kvar.<br>
**BR2_DEFCONFIG** = detta är en variabel som jag skickar till make, den säger till make vart konfigurationsfilen ska sparas. 
**Detta är standard metod i Bildroots när man skapar egna TARGETS..** 

Jag skapade ett mindre problem för mig själv då jag ville se om min config sparades så gjorde jag en make clean, vilket slog mig att nu försvinner min build och att jag hade kunnat kolla om mina konfigurationer hade sparats utan att göra en make clean. detta gjorde att jag förlorade 25 min på att bygga om. men jag lärde mig en sak till efter det och det var att jag insåg vinsten att ha **dl/**cachat så Toolchain bygget tog merparten av tiden men inget behövde laddas ner.<br>

Buildroot kunde inte generera start-qemu.sh automatiskt för min custom defconfig (något i board/qemu/post-image.sh-logiken hoppade över min qemu_aarch64_lab_defconfig, namngivning matchade en exit 0-condition).
Noterade även att medlevererade hjälpscript är ofta knutna till specifika defconfig-namn eller readme.txt-konventioner. När jag gör custom defconfig kan jag inte räkna med att alla helper-scripts genereras. Det jag gjorde av bekvämlighet var att skapa mitt egna start-script och commita det med min defconfig.<br>

cd ~/ws/buildroot/output/images/<br>
cat > start-lab.sh << 'EOF'<br>
#!/bin/bash<br>
exec qemu-system-aarch64 \
  M virt \
  cpu cortex-a53 \
  nographic \
  smp 1 \
  kernel Image \
  append "rootwait root=/dev/vda console=ttyAMA0" \
  netdev user,id=eth0 \
  device virtio-net-device,netdev=eth0 \
  drive file=rootfs.ext4,if=none,format=raw,id=hd0 \
  device virtio-blk-device,drive=hd0
EOF<br>

chmod +x start-lab.sh

Nu kan jag starta med ./start-lab.sh<br>



# Baka in min mjukvara i imagen "Package recipe"
Hur tar jag min hello.c till ett Buildroot paket?
Begrepp att förstå: 
Package recipe är en samling filer i package/<paketnamn>/ och innehåller minimul tre filer.
 - config.in - Kconfig del som lägger in mitt paket i make menuconfig menyn.
 - <paketnamn>.mk - Makefile del som beskriver hur paketet byggs
 - hello.c el vad man nu vill ha, kan ligga i paketet el hämtas från extern URL.
Det behövs även registrera paketet i package/Config.in så Buildroot vet att det finns. 

Jag skapar paket katalogen **mkdir -p package/hello/src** jag tar och kopierar in min källkod från tidigare fas. 
och skapar en ny Makefile för paketet.

cat > ~/ws/buildroot/pagage/hello/src/Makefile << 'EOF'<br>
CC ?= gcc
CFLAGS ?= -Wall -O2

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

clean:
  rm -f hello

install: 
  install -D -m 0755 hello $(DESTDIR)/usr/bin/hello

.PHONY: clean install
EOF
((-m 0755 = sätt permissions till rwxr-xr-x))<br>

Sen behöver jag skapa andra behvliga delen i recipe (Config.in)<br>
cat > ~/ws/buildroot/package/hello/Config.in << 'EOF'<br>
config BR2_PACKAGE_HELLO<br>
bool "hello"<br>
help<br>
	  A simple hello program for learning Buildroot package recipes.<br>

	  https://github.com/nfochef/embedded-linux-lab
EOF<br>

Och sista filen **.MK**

cat > ~/ws/buildroot/package/hello/hello.mk << 'EOF'


 hello



HELLO_VERSION = 1.0
HELLO_SITE = ./package/hello/src
HELLO_SITE_METHOD = local
HELLO_LICENSE = MIT

define HELLO_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define HELLO_INSTALL_TARGET_CMDS
	$(MAKE) -C $(@D) install DESTDIR=$(TARGET_DIR)
endef

$(eval $(generic-package))
EOF
$(eval $(generic-package)) Aktiverar Buildroots generiska paket-infrastruktur som hanterar nedladdning, extrahering, patching, build-step, install-step, etc.

Behöver lägga in paket-recepten på disk och behöver lägga till dem i den globala menyn så Buildroot vet att de finns. <br>
nano ~/ws/buildroot/package/Config.in<br>
läggs in under den kategori som faller in på firmwaret, denna la jag under misc.<br>
source "package/hello/Config.in"<br>

Aktivering av paketet via make menuconfig
[] hello<br>
Markerar hello med space och missade steget att bygga om imagen med make 2>1 | tee build-hello.log<br>

<img width="344" height="151" alt="Skärmbild 2026-05-02 085951" src="https://github.com/user-attachments/assets/eb3e4d6c-380e-4da6-92a7-6740da99bef9" /><br>

Min första paket integration som ett Buildroot recept! resultatet blir då en distro där hello är förinstallerad och redo att köras. KUL!


# Overlay filesystems
Tanken är att lägga till tre egna filer (motd, sysinfo-script, README) i imagen via en rootfs overlay. Bygga om, boota och se dem på plats.<br>
För statiska filer är package recipe overkill. Buildroot har en enklare mekanism: rootfs overlay.<br>
En **overlay** är en mapp i ditt projekt som speglar root-filsystemets struktur. Vid bygget kopieras allt från overlayen rakt in i imagen.<br>

**Overlay** passar när man bara vill lägga in enkla filer rakt in i rootfs, till exempel ett litet script eller en statisk konfigurationsfil som inte har några beroenden och inte behöver byggas.<br> 
Ett **package** är däremot mer lämpligt när innehållet kräver någon form av hantering: om det ska kompileras, har beroenden till andra komponenter, behöver versionsstyras eller uppdateras på ett mer kontrollerat sätt.<br>

Script som rapporterar sys info :<br>
cat > ~/ws/embedded-linux-lab/fas-02-buildroot/rootfs-overlay/usr/local/bin/sysinfo.sh << 'EOF'<br>
#!/bin/sh<br>
echo "=== System Info ==="<br>
echo "Hostname:     $(hostname)"<br>
echo "Kernel:       $(uname -r)"<br>
echo "Architecture: $(uname -m)"<br>
echo "Uptime:       $(uptime)"<br>
echo ""<br>
echo "Memory:"<br>
free -h | head -2<br>
echo ""<br>
echo "Disk:"<br>
df -h / | head -2<br>
EOF<br>

chmod +x ~/ws/embedded-linux-lab/fas-02-buildroot/rootfs-overlay/usr/local/bin/sysinfo.sh<br>

Jag har stött på en del PATH relaterade errors under bygget:<br>
PATH är en lista av kataloger som shellen söker igenom när ett kommando skrivs in. Första matchen vinner. Om en binär finns men inte hittas, kontrollera om dess katalog är i PATH.<br>
echo $PATH är diagnoskommandot.<br>

Buildroot-default har minimal target-PATH (/bin:/sbin:/usr/bin:/usr/sbin). /usr/local/bin är inte med. Workaround: /etc/profile.d/*.sh-fil som exporterar utökad PATH, levererad via overlay.<br>
PATH-buggar är vanligt i scripts, cron, CI, container-images. echo $PATH är värt att ha med sig samt minnas konventionerna. 

# Init-scripts och autostart

Tre stora init-system existerar:
systemd modern, dominerande på desktop/server
SysV-init, klassisk, fortfarande standard på **BusyBox**
OpenRC,  minimalistisk, populär på **Alpine och Gentoo**

Numreringen styr ordningen: S01logging --> S02network --> S99myapp. Tjänster som behöver nätverk har högre nummer än nätverkstjänsten själv.<br>
Så S = "Start" (vid shutdown körs K-prefixade scripts "Kill")<br>
99 = ordningsnummer (00-99) - Kan det vara högre än 99?<br>
myapp = Beskrivningsnamn<br>
Init-systemet bryr sig bara om de två första delarna<br>
Icke kritiska högre nummer , det verkar finnas en konventation för detta som är: <br>
S00-S30 - systemets egna tjänster (filsystem, nätverk, syslog)<br>
S30-S70 - paketerade applikationer (Buildroot/Yocto-paket)<br>
S80-S99 - egna applikationer, custom-tjänster (icke kritiska)<br>

Vissa av dessa kräver beroenden från andra tjänster som har startat först, för att kunna köras.<br>
vid felsökning läs boot-loggen och kolla extra på ordningen tjänsterna startade i.<br>

Man gör SXX numreringar i init systemen för hur dependencies styr ordning. Mental modell...<br>

Init script i overlay:<br>
#!/bin/sh<br>
#<br>
S99hello runs the hello binary at boot, logs output to /var/log/hello.log<br>
#<br>
NAME=hello<br>
DAEMON=/usr/bin/hello<br>
LOGFILE=/var/log/hello.log<br>

start() {<br>
    printf "Starting %s: " "$NAME"<br>
    mkdir -p /var/log<br>
    {<br>
        echo "$(date) booted "<br>
        $DAEMON<br>
        echo ""<br>
    } >> $LOGFILE 2>&1<br>
    echo "OK"<br>
}<br>

stop() {<br>
    printf "Stopping %s: " "$NAME"<br>
    echo "OK"<br>
}<br>

case "$1" in<br>
    start)<br>
        start<br>
        ;;<br>
    stop)<br>
        stop<br>
        ;;<br>
    restart|reload)<br>
        stop<br>
        start<br>
        ;;<br>
    *)<br>
        echo "Usage: $0 {start|stop|restart}"<br>
        exit 1<br>
        ;;<br>
esac<br>

exit 0<br>
EOF<br>

chmod +x ~/ws/embedded-linux-lab/fas-02-buildroot/rootfs-overlay/etc/init.d/S99hello<br>

<img width="410" height="331" alt="Skärmbild 2026-05-08 101305" src="https://github.com/user-attachments/assets/f7e18e12-8031-4089-a704-ab7299ecc8a3" /><br>

#### Problem , när jag kör systemet två gånger så registreras inte det i loggen.
#### Buildroot har Buildroot-default att spara skrivningar mot SD-kort (förlänger livslängd)
#### Embedded-enheter förväntas inte ha persistent state mellan boots.
Hur undgår man detta?
 - Mounta en persistent partition på /var/log/<br>
 - Skicka loggar till annan host via syslog.<br>
 - Använder journald med persistent storage.<br>
 - Skriv kritiska loggar till en separat overlay som överlever reboots.<br>

Reflektion: jag har nu byggt en image som bootar, autostartar min egen "app", loggar och kan stängas ned korrekt.<br>

Man gör SXX numreringar i init systemen för hur dependencies styr ordning. Mental modell...<br>


# Felsökning och introspektion

Relektera över vad som egentligen finns i imagen. Utforska alla paket som är i imagen
tänka på hur jag kan itirera utan att bygga om imagen när jag vill jobba med custom recipe
vad finns i imagen: <br>
cd ~/ws/buildroot<br>
make show-info | head -30<br>
Detta ger mig vad som ser ut att vara ett ernormt JSON svar.<br>
<img width="904" height="422" alt="Skärmbild 2026-05-12 191815" src="https://github.com/user-attachments/assets/b5d31b4f-cdfe-4acb-b545-4ecce9f0f18b" /><br>
inspektion av varje paket i imagen med exakta metadata: version, licens, källa, patches, dependencies.<br>
ake legal-info (licensrapport), och make <paket>-show-info (per-paket-info). Detta känns som ett mindre måste om man behöver kunna redovisa licenser och beroenden i projekt.<br>


Det man behövde för att kunna få ut var hela licens rapporten ligger så är kommandot:<br>
make legal-info 2>&1 | tail -10<br>
ls output/legal-info/<br>
head -20 output/legal-info/manifest.csv<br>

Skapade en MIT-licens<br>
Jag missade att deklarera MIT-licensen i hello.mk Att lägga en fil i "package/hello/src/" räcker inte för Buildroot.<br>
Paketet måste veta om filen via en variabel i .mk-filen i detta fall<br>
"HELLO_LICENSE_FILES = LICENSE" Filer existerar inte för byggsystemet förrän de är deklarerade.<br>

**Buildroots cache är aggressiv**<br>
Source-katalogen "package/<X>/src/" och byggkatalogen<br>
"output/build/<X>/" är två separata platser. Ändringar i source kopieras
inte automatiskt till build.<br>
Löste med "make <paket>-dirclean" raderar<br>
Byggkatalogen och tvingar omstart från source. "make <paket>-rebuild" är
mjukare. <br>
Detta är samma precis cache-fenomen som i Docker<br>

**Mental Note:** Läs felmeddelandena lite extra noga.<br>

##### För varje paket i Buildroot finns tre platser där "filen finns":
**Source-träd**, där du editerar (package/hello/src/)<br>
**Bygg-träd**, där Buildroot kompilerar (output/build/hello-1.0/)<br>
**Target-träd**, där den installerade artefakten hamnar (output/target/usr/bin/)<br>

När jag behöver ändra något i **Source-trädet** så behöver Buildroot triggas att synca det till **bygg-trädet.** Ett lyckat bygge syncar från **Bygg-trädet** till **Target-trädet.**<br>
**Fel ordning och man får man massa fel**
Om jag är osäker på förändringen i source el recept fil:erna och vill undvika mystiska cache fel så vinner jag på att använda mig av kommandot : **<paket>-dirclean**<br>
**Förenkald bild:**<br>
**Buildroot per-paket cache-hantering:**<br>
  **<paket>-rebuild -  mjuk, source-ändring räcker**<br>
  **<paket>-dirclean - hård, raderar build-katalog**<br>
  
  **Recipe-ändring (.mk-fil) eller nya filer → dirclean**<br>
  **Bara kod-ändring                          → rebuild**<br>

  För  varje paket genererar Buildroot en rad uppsättning med targets, ett mönster som är värt att kunna för daglig hantering:<br>
make <paket>-source          bara ladda ner källkod<br>
make <paket>-extract         extrahera tarball + applicera patches<br>
make <paket>-configure       kör configure-steget<br>
make <paket>-build           bygg (inte install)<br>
make <paket>-install         installera till target<br>
make <paket>-rebuild         mjuk omstart<br>
make <paket>-reconfigure     kör om configure + bygg + install<br>
make <paket>-dirclean        hård omstart<br>
make <paket>-show-info       visa metadata<br>
make <paket>-show-depends    visa beroenden<br>
make <paket>-show-rdepends   visa vad som beror på paketet<br>

<img width="609" height="83" alt="Skärmbild 2026-05-17 161555" src="https://github.com/user-attachments/assets/58bdd8b5-4ae0-436c-a811-018cbdd48757" /><br>




