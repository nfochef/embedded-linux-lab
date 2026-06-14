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

# Scarthgap (Yocto 5.0 med LTS till 2028)
Scarthgap är min valda Yocto-release då min bok Mastering Embedded Linux Development bygger på.


<img width="629" height="119" alt="Skärmbild 2026-06-12 153648" src="https://github.com/user-attachments/assets/ce87f6e0-aabb-4d5a-a0e1-1b2521e65d69" /><br>


<img width="911" height="549" alt="Skärmbild 2026-06-12 153919" src="https://github.com/user-attachments/assets/08568ac0-31fb-445d-87a9-f218a7a1c7ba" /><br>

<img width="683" height="242" alt="Skärmbild 2026-06-12 173000" src="https://github.com/user-attachments/assets/1d1e0f15-25a4-4e92-a0ba-e3796eb1b11e" /><br>


Första bygget lyckades 4060 tasks , 42 varningar - do_fetch som föll tillbaka på mirrors när URL missade, vilket är normalt , Yocto har sina primära källor med speglar för ev fel. 
Output i tmp/deploy/images/quemuarm64 motsvarar Buildroots output/images och boots med **runqemu qemuarm64 nographic**.


