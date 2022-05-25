# Válcovací stolice
## Zadání
 
 Předpokládáme, že máme k dispozici válcovací stolice, které vyrábějí ocelové plechy. Plech vyjíždějící ze stolice má obdélníkový tvar, známe jeho šířku, délku a v pravidelném čtvercovém rastru je měříme tloušťku plechu. Např, pro rozměr šířka=10 a délka=20 máme k dispozici 200 naměřených tlouštěk plechu.

Pro vyválcovaný plech chceme najít co nejlepší využití. Potřebujeme z něj vyříznout co největší obdélník tak, aby splňoval zadaná kritéria. Kritéria jsou tří druhů:

* relDev \- chceme najít co největší obdélníkový výřez takový, že relativní odchylka jeho tlouštěk je menší nebo rovná zadané mezi,
* volume \- chceme najít co největší obdélníkový výřez takový, že jeho objem je menší nebo roven zadané mezi,
* minMax \- chceme najít co největší obdélníkový výřez takový, že v něm obsažené tloušťky plechu jsou v rozmezí zadaného intervalu hodnot.

Vyrobený plech je reprezentován instancí CSheet. Třída obsahuje členské proměnné popisující jeho velikost, naměřené tloušťky a problémy k vyřešení (3 pole/mapy, kde má být doplněná vypočtená velikost).

Vyválcované plechy jsou načítané z instance válcovací stolice (CProductionLine). Tato třída poskytuje rozhraní pro načítání plechů (metoda getSheet) a předávání vypočtených výsledků (metoda doneSheet). Vaše implementace vytvoří 2 pomocná vlákna pro **každou** válcovací stolici. Jedno vlákno bude v cyklu načítat nové plechy a předávat je dále ke zpracování. Druhé vlákno bude vyřešené problémy vracet (bude v cyklu volat doneSheet). Tato pomocná vlákna zajišťují komunikaci s válcovací stolicí. Nejsou ale určena pro provádění vlastních výpočtů (řešené problémy jsou výpočetně náročné, komunikační vlákna by nemusela včas předat data z/do válcovací stolice). Protože válcovací stolice nemůže změnit pořadí plechů ve výrobě, musíte vypočtené výsledky pro jednotlivé plechy vracet ve stejném pořadí v jakém jste je načetli. V jednom okamžiku ale lze načíst více plechů a mít je rozpracované (je to dokonce žádoucí). Důležité je ale dodržet pořadí při jejich odevzdání.

Celý výpočet zapouzdřuje instance CQualityControl. Tato třída obsahuje odkazy na jednotlivé válcovací stolice, řídí spouštění a pozastavení výpočtu a řídí činnost výpočetních vláken. Výpočetní vlákna jsou použita pro řešení zadávaných problémů a jsou použita pro řešení problémů ze všech obsluhovaných válcovacích stolic. Výpočetní vlákna převezmou problém od komunikačního vlákna, provedou vlastní výpočty a vyřešený problém předají zpět komunikačnímu vláknu (té správné stolice), aby jej toto ve vhodný okamžik předalo zpět válcovací stolici (je potřeba dodržet pořadí). Výpočetních vláken bude existovat právě a pouze zadaný počet, to umožní vhodně využít hardwarové vybavení počítače (počet CPU).

Třída CQualityControl bude použita podle následujícího scénáře:

* vytvoří se instance CQualityControl,
* zaregistrují se válcovací stolice (voláním metody addLine),
* spustí se vlastní výpočet (voláním metody start). Metoda start dostane v parametru počet výpočetních vláken (workThreads). Tato vlákna vytvoří a nechá je čekat na příchozí požadavky. Zároveň pro každou válcovací stolici vytvoří dvě komunikační vlákna, jedno pro příjem požadavků a druhé pro odevzdávání výsledků. Po spuštění vláken se metoda start vrátí zpět do volajícího,
* komunikační vlákna válcovacích stolic průběžně přebírají požadavky (v cyklu volají odpovídající metody getSheet). Vlákno přebírající požadavky se ukončí v okamžiku, kdy načtete prázdný požadavek (smart pointer obsahuje nullptr),
* výpočetní vlákna přebírají problémy od komunikačních vláken a řeší je. Po vyřešení problém předají odevzdávacímu vláknu odpovídající válcovací stolice,
* odevzdávací vlákna čekají na vyřešený problém a po jeho převzetí problémy ve správném pořadí vrací (metoda doneSheet). Vyřešené problémy musí odevzdat okamžitě, jak to je (vzhledem k pořadí) možné (vyřešené problémy nelze ukládat a odevzdat až na konci výpočtu). Odevzdávací vlákno skončí, pokud již žádné další problémy z dané válcovací stolice nepřijdou (getSheet dříve vrátil nullptr) a všechny problémy této válcovací stolice byly vyřešené a odevzdané,
* testovací prostředí v nějakém okamžiku zavolá metodu stop. Volání počká na dokončení obsluhy všech válcovacích stolic, ukončí pracovní vlákna a vrátí se do volajícího,
* je uvolněna instance CQualityControl.

Použité třídy a jejich rozhraní:

* CSheet je třída reprezentující jeden kus ocelového plechu. Jedná se o velmi jednoduchou třídu, která zapouzdřuje velikost, naměřenou tloušťku a požadované problémy k vyřešení. Třída je implementovaná v testovacím prostředí, její rozhraní nesmíte nijak měnit. Rozhraní obsahuje:
    * m_Width udává šířku plechu,
    * m_Length udává délku plechu,
    * m_Thickness je pole naměřených tlouštěk plechu. Obsahuje celkem m\_Width × m\_Length hodnot, hodnoty jsou ukládané po řádcích (row-major order). Tedy pořadí ukládaných hodnot odpovídají indexům \[0\]\[0\] \[0\]\[1\] \[0\]\[2\] ... \[0\]\[m\_Width-1\] \[1\]\[0\] \[1\]\[1\] \[1\]\[2\] ... \[1\]\[m\_Width - 1\] ... \[m\_Length-1\]\[0\] ... \[m\_Length-1\]\[m_Width-1\].
    * m_RelDev seznam dvojic s problémy typu relDev. Zadaných problémů může být mnoho, každý problém je zadaný jednou dvojicí. První složka dvojice (first) je nastavena na hodnotu maximální přípustné relativní odchylky, druhá složka (second) je nastavena na nulu. Po vyřešení problému nastavte druhou složku na velikost největšího obdélníku takového, že jeho relativní směrodatná odchylka je menší nebo rovná první složce,
    * m_Volume mapa hodnot s problémy typu volume. Klíč udává maximální požadovaný objem, hodnota je na vstupu nastavena na 0. Po vyřešení přepište hodnotu na velikost největšího obdélníkového výřezu plechu, který má objem menší nebo roven klíči,
    * m_MinMax mapa hodnot s problémy typu minMax. Klíč udává interval přípustné tloušťky (má dvě složky, m_Lo a m_Hi), hodnota je na vstupu nastavena na 0. Po vyřešení přepište hodnotu na velikost největšího obdélníkového výřezu plechu, který má všechny tloušťky v zadaném intervalu,
    * metody usnadňující nastavování členských proměnných (viz přiložený zdrojový kód).
* CProductionLine je třída reprezentující jednu válcovací stolici. Třída pouze definuje rozhraní, faktická implementace v testovacím prostředí je potomkem CProductionLine. Rozhraní třídy tedy nemůžete měnit. K dispozici jsou metody:
    * getSheet pro načtení dalšího plechu z výrobní linky. Metoda vrátí instanci ke zpracování nebo neplatný ukazatel (smart pointer obsahuje nullptr) pokud již nejsou na této válcovací stolici další instance problémů ke zpracování. Volání metody může trvat dlouho (čeká se na výrobu plechu), proto pro obsluhu musíte vyčlenit oddělené vlákno, které bude metodu v cyklu volat. Vlákno nesmí provádět žádnou výpočetně náročnou činnost, musí získanou instanci CSheet předat ke zpracování pracovním vláknům. Kontroluje se, že v jedné instanci válcovací stolice volá tuto metodu vždy jedno (stejné) vlákno,
    * doneSheet pro předání vyřešené instance CSheet. Parametrem je vyřešená instance problému dříve získaná z volání getSheet. Protože odevzdání může trvat dlouho, musíte pro odevzdávání vytvořit vyhrazené vlákno. Vlákno bude přebírat od pracovních vláken vyřešené instance problémů a zavolá na ně metodu doneSheet. Vyřešené instance problémů musí být vracené ve stejném pořadí, ve kterém byly z getSheet převzaté. Předávací vlákno nesmí provádět žádnou výpočetně náročnou činnost. Kontroluje se, že v jedné instanci válcovací stolice volá tuto metodu vždy jedno (stejné) vlákno.
* CQualityControl je třída zapouzdřující celý výpočet. Třídu budete vytvářet vy, musíte ale dodržet následující veřejné rozhraní:
    * konstruktor bez parametrů inicializuje novou instanci třídy. Zatím nevytváří žádná vlákna,
    * metodu addLine (x), tato metoda zaregistruje válcovací stolici x,
    * metodu start ( workThr ), tato metoda vytvoří komunikační vlákna pro všechny zaregistrované válcovací stolice a spustí workThr pracovních vláken. Po spuštění vláken se metoda start vrátí zpět do volajícího,
    * metodu stop, která počká na dokončení obsluhy válcovacích stolic a ukončení vytvořených vláken. Po tomto se volání stop vrátí zpět do volajícího,
    * třídní metodu checkAlgorithm(sheet). Metoda slouží k otestování správnosti vlastního algoritmu. Parametrem volání je jedna instance CSheet, která může obsahovat několik požadavků na výpočet relDev/volume/minMax. Kód metody zajistí potřebné výpočty a vyplní odpovídající složky v instanci sheet. Kromě kontroly správnosti implementovaných algoritmů se metoda používá ke kalibraci rychlosti vašeho řešení. Rychlosti se přizpůsobí velikost zadávaných problémů, aby testování trvalo rozumně dlouhou dobu.
* funkce maxRectByRelDev (thickness, width, height, relDevMax) je hotová implementace algoritmu pro výpočet největšího obdélníku, který má relativní odchylku tloušťky menší nebo rovnou mezi relDevMax. Dalšími parametry jsou 2D pole naměřených tlouštěk (prvním indexem je délka, druhým šířka) a velikost plechu. Návratovou hodnotou je velikost největšího nalezeného obdélníkového výřezu. Vaše implementace může tuto funkci použít pro vlastní výpočet, případně tuto funkci nemusíte používat a výpočet si implementovat sami. V bonusových testech je implementace této funkce v testovacím prostředí úmyslně nefunkční (vrací hodnotu 0).
* funkce maxRectByVolume (thickness, width, height, volumeMax) je hotová implementace algoritmu pro výpočet největšího obdélníku, který má objem menší nebo rovný mezi volumeMax. Dalšími parametry jsou 2D pole naměřených tlouštěk (prvním indexem je délka, druhým šířka) a velikost plechu. Návratovou hodnotou je velikost největšího nalezeného obdélníkového výřezu. Vaše implementace může tuto funkci použít pro vlastní výpočet, případně tuto funkci nemusíte používat a výpočet si implementovat sami. V bonusových testech je implementace této funkce v testovacím prostředí úmyslně nefunkční (vrací hodnotu 0).
* funkce maxRectByMinMax (thickness, width, height, min, max) je hotová implementace algoritmu pro výpočet největšího obdélníku, který má všechny tloušťky v zadaném intervalu hodnot &lt; min ; max &gt;. Dalšími parametry jsou 2D pole naměřených tlouštěk (prvním indexem je délka, druhým šířka) a velikost plechu. Návratovou hodnotou je velikost největšího nalezeného obdélníkového výřezu, který splňuje zadaná omezení tloušťky. Vaše implementace může tuto funkci použít pro vlastní výpočet, případně tuto funkci nemusíte používat a výpočet si implementovat sami. V bonusových testech je implementace této funkce v testovacím prostředí úmyslně nefunkční (vrací hodnotu 0).

Vlastní řešení zadaných problémů můžete přenechat na dodaných funkcích nebo si jej můžete implementovat sami. Před implementací vlastního algoritmu zvažte následující:

* Problém relDev:
    * řešení znamená identifikovat všechny existující obdélníkové výřezy zadaného plechu (je jich řádově (m\_Width × m\_Length)2),
    * pro každý takový obdélníkový výřez musíme spočítat průměr a směrodatnou odchylku tlouštěk v něm obsažených (to bude trvat čas (m\_Width × m\_Length)),
    * následně vydělíte směrodatnou odchylku průměrem, získáte relativní odchylku a porovnáte ji s mezí. Pozor, porovnávají se desetinná čísla, je potřeba myslet na toleranci. Porovnává se _relativní_ odchylka, tedy toleranci lze nastavit fixně. Referenční implementace používá hodnotu 1e-5,
    * naivní řešení problému má časovou složitost (m\_Width × m\_Length)3,
    * poskytnutá funkce maxRectByRelDev má složitost (m\_Width × m\_Length)2 (n4 pro čtvercové plechy).
* Problém volume:
    * naivní řešení nalezne všechny existující obdélníkové výřezy zadaného plechu (je jich řádově (m\_Width × m\_Length)2),
    * pro každý takový obdélníkový výřez sečteme tloušťky v něm obsažené (to bude trvat čas (m\_Width × m\_Length)),
    * součet porovnáme s mezí,
    * naivní řešení problému má časovou složitost (m\_Width × m\_Length)3,
    * poskytnutá funkce maxRectByVolume má složitost (m\_Width × m\_Length × min (m\_Width, m\_Length)) (n3 pro čtvercové plechy).
* Problém minMax:
    * naivní řešení nalezne všechny existující obdélníkové výřezy zadaného plechu (je jich řádově (m\_Width × m\_Length)2),
    * pro každý takový obdélníkový výřez zkontrolujeme tloušťky, zda se vejdou do zadaného intervalu (to bude trvat čas (m\_Width × m\_Length)),
    * naivní řešení problému má časovou složitost (m\_Width × m\_Length)3,
    * poskytnutá funkce maxRectByMinMax má složitost (m\_Width × m\_Length) (n2 pro čtvercové plechy),
    * optimalizace je založená na myšlence tohoto [algoritmu](https://www.kuniga.me/blog/2021/01/09/max-area-under-histogram.html).
* Pokud se rozhodnete implementovat výpočty vlastní, měli byste dosahovat podobné časové složitosti jako mají poskytnuté funkce. Testovací prostředí sice měří rychlost vašeho řešení a upravuje velikosti zadávaných problémů, ale toto funguje pouze v nějakých mezích (např. si poradí se skrytými multiplikativními konstantami). Nedá se očekávat, že regulace bude dobře fungovat i pro naivní implementace (m\_Width × m\_Length)3).
* Všimněte si, že řešení jednotlivých druhů problémů má velmi odlišnou časovou složitost. Dá se očekávat, že v jedné instanci CSheet bude většinou více méně problémů relDev a více problémů minMax, nemusí to ale být pravda vždy. Vaše řešení by mělo být schopné vhodně plánovat přidělování problémů tak, aby maximálně využilo všechna dostupná pracovní vlákna.

* * *

Odevzdávejte zdrojový kód s implementací požadované třídy CQualityControl s požadovanými metodami. Můžete samozřejmě přidat i další podpůrné třídy a funkce. Do Vaší implementace nevkládejte funkci main ani direktivy pro vkládání hlavičkových souborů. Funkci main a hlavičkové soubory lze ponechat pouze v případě, že jsou zabalené v bloku podmíněného překladu.

Využijte přiložený ukázkový soubor. Celá implementace patří do souboru solution.cpp. Pokud zachováte bloky podmíněného překladu, můžete soubor solution.cpp odevzdávat jako řešení úlohy.

Při řešení lze využít pthread nebo C++11 API pro práci s vlákny (viz vložené hlavičkové soubory). Dostupný kompilátor g++ verze 10.3, tato verze kompilátoru zvládá většinu C++11, C++14 a C++17 konstrukcí. C++20 není podporované plně, doporučujeme se mu vyhnout.

* * *

**Doporučení:**  

* Začněte se rovnou zabývat vlákny a synchronizací, nemusíte se zabývat vlastními algoritmy řešení zadaných problémů. Využijte dodané řešení - funkce maxRectByXXXXXX. Až budete mít hotovou synchronizaci, můžete doplnit i vlastní algoritmické řešení.
* Abyste zapojili co nejvíce jader, obsluhujte co nejvíce instancí CSheet najednou. Je potřeba zároveň přebírat problémy, řešit je a odevzdávat je. Nepokoušejte se tyto činnosti nafázovat (například nejdříve pouze počkat na všechny CSheet, pak začít řešit akumulované problémy, ...), takový postup nebude fungovat. Testovací prostředí je nastaveno tak, aby takové "serializované" řešení vedlo k uváznutí (deadlock).
* Instance CQualityControl je vytvářená opakovaně, pro různé vstupy. Nespoléhejte se na inicializaci globálních proměnných - při druhém a dalším zavolání budou mít globální proměnné hodnotu jinou. Je rozumné případné globální proměnné vždy inicializovat v konstruktoru nebo na začátku metody start. Ještě lepší je nepoužívat globální proměnné vůbec.
* Nepoužívejte mutexy a podmíněné proměnné inicializované pomocí PTHREAD\_MUTEX\_INITIALIZER, důvod je stejný jako v minulém odstavci. Použijte raději pthread\_mutex\_init() nebo C++11 API.
* Instance problémů (CSheet) a válcovacích stolic (CProductionLine) alokovalo testovací prostředí při vytváření příslušných smart pointerů. K uvolnění dojde automaticky po zrušení všech odkazů. Uvolnění těchto instancí tedy není Vaší starostí, stačí zapomenout všechny takto předané smart pointery. Váš program je ale zodpovědný za uvolnění všech ostatních prostředků, které si sám alokoval.
* Neukončujte metodu stop pomocí exit, pthread_exit a podobných funkcí. Pokud se funkce stop nevrátí do volajícího, bude Vaše implementace vyhodnocena jako nesprávná.
* Využijte přiložená vzorová data. V archivu jednak naleznete ukázku volání rozhraní a dále několik testovacích vstupů a odpovídajících výsledků.
* V testovacím prostředí je k dispozici STL. Myslete ale na to, že ten samý STL kontejner nelze najednou zpřístupnit z více vláken. Více si o omezeních přečtěte např. na [C++ reference - thread safety.](http://en.cppreference.com/w/cpp/container)
* Testovací prostředí je omezené velikostí paměti. Není uplatňován žádný explicitní limit, ale VM, ve které testy běží, je omezena 4 GiB celkové dostupné RAM. Úloha může být dost paměťově náročná, zejména pokud se rozhodnete pro jemné členění úlohy na jednotlivá vlákna. Pokud se rozhodnete pro takové jemné rozčlenění úlohy, možná budete muset přidat synchronizaci běhu vláken tak, aby celková potřebná paměť v žádný okamžik nepřesáhla rozumný limit. Pro běh máte garantováno, že Váš program má k dispozici nejméně 1 GiB pro Vaše data (data segment + stack + heap). Pro zvídavé - zbytek do 4GiB je zabraný běžícím OS, dalšími procesy, zásobníky Vašich vláken a nějakou rezervou.
* Pokud se rozhodnete pro všechny bonusy, je potřeba velmi pečlivě nastavovat granularitu řešeného problému. Pokud řešený problém rozdělíte na příliš mnoho drobných podproblémů, začne se příliš mnoho uplatňovat režie. Dále, pokud máte najednou rozpracováno příliš mnoho problémů (a každý je rozdělen na velké množství podproblémů), začne se výpočet dále zpomalovat (mj. se začnou hůře využívat cache CPU). Aby se tomu zabránilo, řídí referenční řešení počet najednou rozpracovaných úloh (navíc dynamicky podle velikosti rozpracované úlohy).
* Výpočetně náročné operace musí provádět pracovní vlákna. Počet pracovních vláken je určen parametrem metody start. Testovací prostředí kontroluje, zda Vaše implementace neprovádí výpočetně náročné operace v ostatních vláknech. Pokud to zjistí, Vaše řešení bude odmítnuto.
* Explicitní nastavení počtu pracovních vláken má dobré praktické důvody. Volbou rozumného počtu pracovních vláken můžeme systém zatížit dle naší volby (tedy například podle počtu jader, která můžeme úloze přidělit). Pokud by časově náročné výpočty probíhaly i v jiných vláknech (komunikační vlákna), pak by bylo možné systém snadno zahltit.
