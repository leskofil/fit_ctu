Vedení dopravního podniku se rozhodlo, že využije dobu datovou, kdy jsou vozy dopravního podniku plné kamer, senzorů a jiných chytrých zařízení, a připraví lepší strategii pro kontroly jízdenek. Lépe řečeno tímto úkolem pověřilo útvar Přepravní kontroly.

Pracovníci útvaru se rozhodli nejdříve zjistit, která všechna data jsou schopni získat. Analýza ukázala, že pro každou dvojici stanic `A` a `B` má dopravní podnik informaci o tom, kolik lidí průměrně ze stanice `A` do stanice `B` jede. Tým analytiků z dat také zjistil zajímavé informace týkající se samotných kontrolorů. Počet pasažérů bez platné jízdenky sice rok od roku klesá, nicméně u těch chycených výrazně stoupá jejich agresivita.

Suma sumárum, výsledek je následující. Kvůli narůstajícímu počtu incidentů bude při odhalení černého pasažéra tento legitimován a bude mu uložena pokuta. V jízdě ale bude moci pokračovat. Díky útržku s cílovou stanicí se mu také nestane, že by při další kontrole při stejné cestě dostal pokutu opakovaně. Z důvodu malého množství revizorů by dopravní podnik pro každou linku chtěl vytipovat malé množství úseků (vždy mezi dvěma po sobě jdoucími stanicemi) takových, že při přepravní kontrole provedené na tomto úseku bude zkontrolováno co největší množství pasažérů. Poslední novinky pak říká, že každý revizor může provést kontrolu na právě jednom úseku trasy. To zejména z důvodu ochrany zdraví revizorů.

Poslední věcí, kterou je před spuštěním nového systému třeba udělat, je vytvořit algoritmus, který takové úseky najde. Naštěstí se dalo pár chytrých hlav dohromady a vymysleli, že by to mohli zadat jako projekt spolupráce s průmyslem na FIT ČVUT. Prý to tam studenti naprogramují skoro zadarmo...

_Vaším cílem je tedy pro zadanou linku navrhnout místa kontrol tak, aby došlo ke zkontrolování co největšího počtu přepravovaných osob._

Formát vstupu
-------------

* Vstup začíná trojicí čísel _C_, _S_, _P_, které popořadě představují počet kontrolorů, počet zastávek a počet záznamů o přepravených pasažérech. Platí 1 ≤ _C_, _P_ ≤ 10000 a 2 ≤ _S_ ≤ 10000.
* Následuje _P_ řádků s informacemi o přepravených pasažérech. Každý z těchto řádků ma tvar `from to count`, kde `from` představuje nástupní stanici, `to` představuje výstupní stanici a `count` je počet pasažérů, u kterých očekáváme, že se mezi těmito zastávkami budou přepravovat. Platí `0 ≤ from < to ≤ S-1` a `count ≤ 10⁹`.
* Vstup je vždy validní a není třeba jej kontrolovat.

Formát výstupu
--------------

* Na první řádce výstupu je maximální počet pasažéru, které lze pomocí _C_ revizorů zkontrolovat.
* Na druhé řádce následuje posloupnost úseků popisující, mezi kterými zastávkami má ke kontrolám dojít. Každý úsek má tvar \[_x_; _y_\] a jednotlivé úseky jsou oddělené čárkou.

Bodové podmínky
---------------

* Pro získání 1 bodu je potřeba správně vyřešit instance s _C_ = 1 a _S_ ≤ 15.
* Pro získání 3 bodů je potřeba správně vyřešit instance s _C_ ≤ 5 a _S_ ≤ 15.
* Pro získání 5 bodů je potřeba správně a rychle vyřešit instance s _C_ ≤ 15 a _S_ ≤ 50.
* Pro získání 10 bodů je potřeba správně a rychle vyřešit instance s _C_ ≤ 1000 a _S_ ≤ 1000.
* Pro získání 13 bodů je potřeba správně a rychle vyřešit libovolné instance odpovídající zadání.

Ukázková data
-------------

**Vstup 1**

    1 5 5
    1 4 2
    1 2 2
    2 3 6
    2 4 3
    3 4 5

**Výstup 1**

    11
    [2;3]

**Vstup 2**

    2 5 5
    1 4 4
    1 2 2
    2 3 6
    2 4 3
    3 4 5

**Výstup 2**

    18
    [2;3],[3;4]

Poznámky
--------

* Linku, pro kterou problém řešíte, si můžete představit jako přímku. První zastávku na trase označujeme `0`, druhou `1`, až poslední zastávka bude označována jako `S-1`
* Ve všech testech budete omezeni dobou běhu. Limity můžete vidět u referenčního řešení.
* Úloha nabízí bonusový test rychlosti, za který je možné získat až 3 body navíc. Pro jeho splnění je třeba dobře implementovat efektivní algoritmus řešící zadaný problém. Bonusový test rychlosti není možné konzultovat s vyučujícími.

Reference
---------

* \[1\] DOPRAVNÍ PODNIK HL. M. PRAHY. Výroční zpráva 2020 \[online\]. Praha: Dopravní podnik hl. m. Prahy, duben 2021. Dostupné z: [https://www.dpp.cz/cs/data/Výroční zprávy/DPP\_VYROCNI\_ZPRAVA_2020.pdf](https://www.dpp.cz/cs/data/V%C3%BDro%C4%8Dn%C3%AD%20zpr%C3%A1vy/DPP_VYROCNI_ZPRAVA_2020.pdf)
* \[2\] DOPRAVNÍ PODNIK HL. M. PRAHY. Výroční zpráva 2019 \[online\]. Praha: Dopravní podnik hl. m. Prahy, duben 2020. Dostupné z: [https://www.dpp.cz/cs/data/Výroční zprávy/DPP\_VYROCNI ZPRAVA\_2019.pdf](https://www.dpp.cz/cs/data/V%C3%BDro%C4%8Dn%C3%AD%20zpr%C3%A1vy/DPP_VYROCNI%20ZPRAVA_2019.pdf)
* \[3\] DOPRAVNÍ PODNIK HL. M. PRAHY. Výroční zpráva 2018 \[online\]. Praha: Dopravní podnik hl. m. Prahy, duben 2019. Dostupné z: [https://www.dpp.cz/cs/data/Výroční zprávy/DPP\_VYROCNI\_ZPRAVA_2018.pdf](https://www.dpp.cz/cs/data/V%C3%BDro%C4%8Dn%C3%AD%20zpr%C3%A1vy/DPP_VYROCNI_ZPRAVA_2018.pdf)
* \[4\] DOPRAVNÍ PODNIK HL. M. PRAHY. Výroční zpráva 2017 \[online\]. Praha: Dopravní podnik hl. m. Prahy, duben 2018. Dostupné z: [https://www.dpp.cz/cs/data/Výroční zprávy/DPP\_VYROCNI\_ZPRAVA_2017.pdf](https://www.dpp.cz/cs/data/V%C3%BDro%C4%8Dn%C3%AD%20zpr%C3%A1vy/DPP_VYROCNI_ZPRAVA_2017.pdf)
* \[5\] ŠINDELÁŘ, Jan. Černých pasažérů v Praze ubývá, stále jich je ale dvakrát více než ve Vídni. Zdopravy.cz \[online\]. Praha: Avizer Z, 2019, 19. ledna 2019. ISSN 2570-7868. Dostupné z: [https://zdopravy.cz/cernych-pasazeru-v-praze-ubyva-stale-jich-je-ale-dvakrat-vice-nez-ve-vidni-22277/](https://zdopravy.cz/cernych-pasazeru-v-praze-ubyva-stale-jich-je-ale-dvakrat-vice-nez-ve-vidni-22277/)
