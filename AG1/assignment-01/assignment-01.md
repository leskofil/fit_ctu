## Turnaj tří kouzelnických škol
# Zadani


Všichni soutěžící jsou již po více či méně úspěšně splněných dvou úkolech Turnaje vyčerpáni jak po psychické, tak fyzické stránce. Na to se ale historie neptá, vyhrát může jen ten/ta nejinteligentnější, nejschopnější a nejkurážnější. Tedy zpět do práce, ať už to máš za sebou. Oblékáš si hábit, hůlku ukládáš do pouzdra na noze a vydáváš se vstříc poslednímu dobrodružství.

Když vyjdeš z hradu, již z dálky začínáš tušit, kde se bude poslední úkol odehrávat. Tvé kroky tedy vedou neomylně až na samou hranici Zapovězeného lesa. Ostatní soutěžící jsou již na místě, tudíž se profesor Moody ihned ujímá slova.

„Posledním úkolem, který také rozhodne o tom, kdo se stane vítězem Turnaje tří kouzelnických škol, bude ten, komu se povede obstát ve všech nástrahách, které na vás číhají v připraveném bludišti.“ K překvapení všech zúčastněných vám profesor Moody také předává mapu s přesným popisem celého bludiště s jasně zaznačenou polohou Ohnivého poháru. Kde je tedy ten vtip, vždyť se stačí vydat nejkratší cestou bludištěm přímo k poháru?

Ač už to vypadalo, že vysvětlování končí a budete moci vyrazit, profesor znenadání ve vysvětlování pokračuje „Než se do bludiště vydáte, máte možnost ovlivnit jeho podobu. Jak si můžete všimnout na plánku, okolo bludiště je rozmístěno 8 pák. Všechny jsou momentálně vypnuté a ke každé přísluší nějaký vektor změn, který si lze představit jako posloupnost prázdných a vyplněných polí. Tento vektor má délku rovnou velikosti bludiště. Jako aia_iai​ označme jeho iii. člen. Každá z pák po aktivaci změní podobu každého řádku bludiště zvlášť. Pro řádek jjj a jeho iii. pole bude ve změněném bludišti překážka pouze tehdy, je-li již překážka na této pozici přítomna, ale ve vektoru změny představuje člen aia_iai​ prázdné pole, nebo naopak, představuje-li aia_iai​ překážku a v bludišti je prázdné pole. Po zvednutí páky se změna projeví na vašich plánech bludiště. Hodně štěstí, můžete vyrazit.“

Trochu se ti z toho popisu fungování pák točí hlava, ale přeci jen se ti daří přemluvit své nohy, aby vyrazily ke vstupu do bludiště. Těsně před tím, než se v bludišti zcela ztratíš se za tvými zády ještě ozve „Zapomněl jsem vám říci, že v bludišti nebudou fungovat žádná kouzla, která jste se naučili v předmětech Studium starodávných run, Transfigurace a Lektvary.“

_Tvým úkolem je tedy najít nejkratší cestu bludištěm přes všechny možné konfigurace bludiště zvolené pomocí zapínání a vypínání pák. Ovšem pozor, po cestě k pákám se můžeš zdržet natolik, že se jejich použití vůbec nevyplatí. Dokážeš se k poháru dostat jako první?_

Formát vstupu
-------------

* Vstup začíná čísly nnn a kkk, které popořadě značí velikost jedné strany čtvercového bludiště a počet pák. Platí 1≤n≤10001 \\leq n \\leq 10001≤n≤1000 a 0≤k≤100 \\leq k \\leq 100≤k≤10.

* Poté následuje kkk řádek představujících konfiguraci jednotlivých pák. Na každém z kkk řádků je nejdříve číslo yiy_iyi​, 0≤yi≤n+1-1 \\leq y_i \\leq n-1≤yi​≤n, představující pozici páky na vertikální ose vzhledem k počáteční poloze kouzelníka. Za pozicí páky vždy následuje posloupnost nnn čísel z množiny {0,1}\\{0,1\\}{0,1} představující změnu bludiště při použití páky, tzv. vektor změny.

* Následovat bude definice bludiště, která se skládá z posloupnosti n2n^2n2 znaků z abecedy {0,1}\\{0,1\\}{0,1}, kde 000 značí volné pole a 111 značí překážku (neprůchozí pole).

* Na poslední řádce je dvojice čísel xxx a yyy představujících polohu Ohnivého poháru, 1≤x,y≤n1 \\leq x,y \\leq n1≤x,y≤n.

* Vstup nemusíte nijak kontrolovat, bude vždy validní.

Formát výstupu
--------------

* Pokud se k Ohnivému poháru nelze dostat při žádné konfiguraci pák, na výstupu se objeví číslo −1-1−1.

* Jestliže řešení existuje, vypiš na první řádce délku nejkratší cesty mezi vstupem do bludiště a Ohnivým pohárem.

* Toto číslo bude na další řádce následováno posloupností kkk čísel z množiny {0,1}\\{0,1\\}{0,1} představující konfiguraci pák (000 = vypnuto, 111 = zapnuto) pro tuto nejkratší cestu.

* Na poslední řádce výstupu bude posloupnost souřadnic políček tvořících nějakou nejkratší cestu z počátku až k Ohnivému poháru.

Příklad
-------

Uvažujme následující bludiště s jednou pákou nacházející se ve vzdálenosti di=3y_0 = 3y0​=3 a odpovídajícím vektorem změny 001000010000100. Ohnivý pohár se nachází na souřadnicích \[5;5\]\[5;5\]\[5;5\]. Kouzelník se nachází nalevo od počátku bludiště.

![](?X=ImgT&UID=1699&N=1)

Snadno nahlédneme, že s iniciálním nastavením bludiště se k poháru nedostaneme. Bude tedy potřeba vyzkoušet nějakou kombinaci pák. V našem případě máme pouze jednu páku s y=3y = 3y=3. Pokud ji zmáčkneme, je na každý řádek bludiště (kromě ohraničující zdi) a vektor změny platný pro tuto páku aplikována operace XOR. Po přepnutí bude mít tedy bludiště následující podobu.

![](?X=ImgT&UID=1699&N=2)

Cesta k páce a zpět na počáteční pozici nám zabrala 666 jednotek času. Nyní je třeba se pomocí zaklínadla „_Apparate_“ přenést do bludiště (vždy na souřadnice \[1;1\]\[1;1\]\[1;1\]):

![](?X=ImgT&UID=1699&N=3)

Dále již kouzelník nemá přístup k pákám a musí se nejkratší cestou dostat k Ohnivému poháru. Snadno nahlédneme, že nejkratší cesta má délku 888. Společně s cestou potřebnou pro přepnutí pák je tedy nejkratší cesta délky 141414, protože již žádná další kombinace pák k vyzkoušení neexistuje.

Ukázková data
-------------

**Vstup 1**

    5 1
    3 00100
    00001 00010 00100 01000 10000
    5 5

**Výstup 1**

    14
    1
    [1;1],[1;2],[1;3],[2;3],[3;3],[4;3],[4;4],[4;5],[5;5]

**Vstup 2**

    5 0
    01000 00000 00000 00000 00000
    3 1

**Výstup 2**

    4
    [1;1],[1;2],[2;2],[3;2],[3;1]

Bodové podmínky
---------------

* Pro získání 222 bodů je potřeba správně vyřešit instance s k=0k = 0k=0.

* Pro získání 555 bodů je potřeba správně vyřešit instance s n≤100n \\leq 100n≤100 a k≤5k \\leq 5k≤5.

* Pro získání 101010 bodů je potřeba správně vyřešit libovolné instance odpovídající zadání.

Poznámky
--------

* Kouzelník nejdříve nastavuje páky. Po zapnutí libovolné kombinace pák (může jich být aktivních i více naráz, poté se operace XOR aplikuje na bludiště od největší hodnoty yiy_iyi​ po nejmenší) se musí vrátit na počáteční pozici, přenést se do bludiště a dojít nejkratší cestou k Ohnivému poháru.

* Pokud je aktivováno více pák, pak jich lze v jednom směru aktivovat na jeden pohyb více. Konkrétně, máme-li dvě páky s y0=2y_0=2y0​=2 a y1=4y_1 = 3y1​=3 a chceme-li aktivovat obě, bude cena aktivace pák pouze 666, nikoliv 6+2=86+2 = 86+2=8.

* Jak je patrné z ukázky, kouzelník se v bludišti může pohybovat pouze nahoru, dolů, doleva a doprava. Diagonální pohyb možný není.

* Obvodová zeď bludiště je přítomna vždy a není součástí vstupu. Zároveň při přepnutí páky nedochází k její modifikaci, operace XOR se aplikuje pouze na vnitřní část bludiště.

* Knihovna STL a `std::string` není pro řešení problému k dispozici. Při jejím použití nepůjde program zkompilovat a bude hodnocen 000 body.

* **Na políčko s překážkou se nelze přenést.**

* **UPDATE:** Pro lepší srozumitelnost orientace bludiště došlo k aktualizaci ukázkových vstupů a výstupů.
