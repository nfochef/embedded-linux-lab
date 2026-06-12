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

Lager modellen är mer el mindra att man rör aldrig någon annans kod. Man lägger på ett lager åvanpå istället så orginalet blir orört så ändringen man gjort ligger ovanpå. Vid uppdatering så byter man ut de undre lagren, tillskillnad mot buildroot. 


