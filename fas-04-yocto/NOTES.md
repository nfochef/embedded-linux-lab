# Yocto

Anteckningar under inlärning av Yocto. Fel, lösningar och insikter.

Yocto är ett ekosystem som bygger embedded Linux-distributioner. löser samma problem som Buildroot gör dvs bygger linux från källkod för en specifik hårdvara, men är designat för industrin i större skala, stora team, produkter som lever länge, och hara strikta krav på spårbarhet.

Yocto::<br>
Poky är referensdistributionen. Det mab klonar när man "installerar Yocto". Det innehåller;

BitBake bygg-motorn, python-baserad och läser recept och kör tasks.<br>
Recipe (.bb-fil) ger instruktioner för ett paket, var källkoden finns, hur den byggs, vilken licens.Den motsvarar Buildroots hello.mk typ.<br>
Layer (metan) mappen med recept och konfigurationer. Hela Yoctos arkitektur byggs på lager.<br>
Machine - målhårdvaran (qemuarm64, raspberrypi0-2w-64). Motsvarar Buildroots defconfig-val.<br>
Image = slutprodukten (core-image minimal). Motsvarar min rootfs.ext2.<br>
sstate cache som en "delbar" bygg cache. typ kraften för yotco.<br>

Lager modellen är mer el mindra att man rör aldrig någon annans kod. Man lägger på ett lager åvanpå istället så orginalet blir orört så ändringen man gjort ligger ovanpå. Vid uppdatering så byter man ut de undre lagren, tillskillnad mot buildroot.<br>

sstate cache är tasks och signaturer, bitbake byter varje paket i tasks, varje task får en signatur, dvs den chachas och nr man kör en task så finns det redan en chach av dem då hämtas paketet på sekunden istället för att det ska kompileras om.
nackdelen är att det tar lång tid första gången det ska byggas men vid om byggnad tar det väldigt kort tid.  man kan även dela chachen mellan utvecklare via något som heter NFS/S3 då min kollegas kompilering blir min.

<img width="3468" height="4624" alt="20260621_103438" src="https://github.com/user-attachments/assets/da3ac3ae-5a37-4e15-93df-7f03ce37bf25" /><br>

# Scarthgap (Yocto 5.0 med LTS till 2028)
Scarthgap är min valda Yocto-release då min bok Mastering Embedded Linux Development bygger på.


<img width="629" height="119" alt="Skärmbild 2026-06-12 153648" src="https://github.com/user-attachments/assets/ce87f6e0-aabb-4d5a-a0e1-1b2521e65d69" /><br>


<img width="911" height="549" alt="Skärmbild 2026-06-12 153919" src="https://github.com/user-attachments/assets/08568ac0-31fb-445d-87a9-f218a7a1c7ba" /><br>

<img width="683" height="242" alt="Skärmbild 2026-06-12 173000" src="https://github.com/user-attachments/assets/1d1e0f15-25a4-4e92-a0ba-e3796eb1b11e" /><br>


Första bygget lyckades 4060 tasks , 42 varningar - do_fetch som föll tillbaka på mirrors när URL missade, vilket är normalt , Yocto har sina primära källor med speglar för ev fel. 
Output i tmp/deploy/images/quemuarm64 motsvarar Buildroots output/images och boots med **runqemu qemuarm64 nographic**.


 ### Distro: Poky 5.0.18 (Scarthgap LTS), kernel 6.6.127-yocto-standard.<br>

I denna Build så finns det vissa skillnader mot Buildroot:<br>
- CPU: Cortex-A57 (0xd07) mot Buildroots A53 (0xd03), matchar TUNE_FEATURES<br>
- 4 kärnor + 256 MB mot Buildroots 1 kärna + 128 MB<br>
- K-bygginformation: "oe-user@oe-host" Yocto gjorde mitt namn anonymiserat till oe-user..<br>
  Yocto vill ha bit för bit identiska byggen oavsett vem som bygger. För reproducerbarhet<br>
- Init: sysvinit + udev + populate-volatile.sh + read-only-rootfs-hook<br>
  Yocto har strukturerade lösningar för saker jag löste manuellt i<br>
  Buildroot (t.ex persistent logging via volatile-hantering).<br>

Använd /etc/issue för distro-id.<br>

Tänker att jag behöver kolla upp lite hur strukturen är Poky rooten: <br>

<img width="548" height="443" alt="Skärmbild 2026-06-21 104505" src="https://github.com/user-attachments/assets/3ca7e0a1-ce8a-4ebe-9075-3505bfdae182" /> <br>

jag ser Poky rootens lager (det yocto byggs av) dvs meta katalogerna.<br>

meta/ oe-core, hjärtat. Alla grundrecept.<br>
meta-poky/ poky-distrons policy och konfiguration<br>
meta-yocto-bsp/ board support (hårdvarustöd)<br>
meta-skeleton/ mallar för egna recept<br>
meta-selftest/ Yoctos testrecept <br>

Verktyg (motorn):<br>
bitbake/ själva bygg-motorn (Python)<br>
scripts/ runqemu, oe-* (det source la i PATH och se till att den är ren)<br>
oe-init-build-env skriptet jag source:ar<br>

Mina enga genererade kataloger som inte är en  del av poky-källan:<br>
build-qemuarm64/ min build-katalog (conf/, tmp/)<br>
downloads/ DL_DIR, nedladdad källkod...<br>
sstate-cache/ cachade task resultat..<br>

Poky binder ihop fristående lager, README symlinkarna pekar in i lagrena som är ännu ett bevis på den modulära designen. 
Rreceptet som är mitt första riktiga yocto-recept. Det jag byggde ifrån är inte många rader<br>

<img width="764" height="208" alt="Skärmbild 2026-06-21 115536" src="https://github.com/user-attachments/assets/3aff78a6-23ef-4167-805e-ed1da0661b5a" /><br>

 IMAGE_INSTALL          = vad som hamnar i imagen (HUVUDSPAKEN)<br>
                           packagegroup-core-boot = minsta bootbara<br>
                           ${CORE_IMAGE_EXTRA_INSTALL} = sätt i local.conf<br>
  inherit core-image     = ärver HUR man bygger image från klassen<br>
                           Receptet säger VAD, klassen gör HUR<br>
  IMAGE_ROOTFS_SIZE ?=    = ?= svag tilldelning (om ej satt)<br>
  VARIABLE:append =       = LÄGG TILL i variabel (override-syntax)<br>
  ${@python...}           = inline Python vid parsning<br>
  bb.utils.contains(...)  = villkorlig logik (om systemd → +4MB)<br>

Det jag ser som mest av värde att försöka komma ihåg är IMAGE_INSTALL styr image innehållet, inherit ger receptet kraft, :append och ${@python} är overide och logik mekaniken.<br>
Yocto har mycket rikare DSL (inheritance, :append och inline python) som ger mer kraft, tunt recept + tung klass = DRY.
<br>

# sstate-cache 
(sstate-cache (shared state cache) är Yoctos minne av redan utfört arbete)
jag bygger om med **bitbake core-image-minimal**

<img width="1235" height="367" alt="Skärmbild 2026-06-29 162631" src="https://github.com/user-attachments/assets/7730668c-6d19-4cfe-b4a4-7283d6d146e0" />

Det gick väldigt fort tillskillnad från första bygget <br>
Jämnför jag det med det tidigare bygget så ser jag att Missed gick från 1849 till 0 , pga att allt finns i cachen , alla resultat hämtas ifrån sstate istället för att byggas.<br>
Alla 4060 tasks behövde inte köras om de vara verifierades emot att chachen var giltlig och satte ihop resultatet.<br>

Varje task har sin signatur sk hash av inputs.<br>
Skalning till team är delad cache = NFS/S3 en kollega får tex mina cache dem bygger inte om det jag har byggt. <br>
Därav ligger sstate_dir utanför build katalogen local.conf och om jag raderar tillexempel builden överlever cachen.<br>
Ändrar jag något så får bara påverkade tasks nya signaturer men resterande byggs från cache.<br>






  

  
