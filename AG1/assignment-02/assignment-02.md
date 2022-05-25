Po radikálních ztrátách území, které přinesla dohoda vedoucích mocností, byla politická mapa našeho státu drasticky překreslena. Mnoho lidí došlo k názoru, že jednou z hlavních příčin této nešťastné události byla velká roztříštěnost politických stran.

Na to konto dal předseda vlády okamžitě vzniknout Úřadu pro politické záležitosti. U něj se nově musí každá politická strana a straník zaregistrovat. Registrací strana získává unikátní číslo, pod kterým následně vystupuje v oficiálních dokumentech, například toto číslo určuje pořadí volebních lístků[1](#fn1). Pokud by se náhodou stalo, že na stranu již žádné číslo nezbývá, musí její představitelé počkat až do doby, než nějaká jiná strana zanikne. Obdobná pravidla pak platí i pro straníky, kteří mají ovšem vlastní číselnou řadu.

Na koho ale v celé této situaci nikdo nepomyslel, to jsou novináři, hlídací psi demokracie. Jednak s omezením politické soutěže již nelze tak úplně mluvit o demokracii, ale hlavně, jak lze nyní hlídat, kdo je členem jaké strany, které strany existují, které strany byly sloučeny, kdo ukončil členství a podobně? Vždyť se všechny zažité pořádky drasticky mění! Navíc, již se dávno nepíše rok 1938. Technika pokročila a lidé chtějí zprávy okamžitě. Nejlépe v reálném čase. A pochopitelně také dostatečně krátké, konec konců, kdo má dnes čas číst cokoliv delšího, než co se vejde do jednoho tweetu?

Sešli jste se proto na redakční poradě a řešíte, co dál. Již v tak těžce zkoušeném odvětví by toto mohl být poslední hřebíček do rakve vaší nezávislé redakce. Ve chvíli, kdy byla debata na svém vrcholu, se najednou otevřeli dveře zasedací místnosti a vešel, v teplákové soupravě a s kelímkem kafe, sporťák Vojta. S nepříliš chápajícím výrazem se začal omlouvat a hledat potítko, které zapomněl při poradě sportovní redakce. Přestali jste si ho všímat a dál se snažíte zjistit, jak utéct čertovi z lopaty.

V tu chvíli se odněkud ozve radostné „Mám to! Našel jsem ho!“ Vojta se zvedá z pod stolu, do kterého stačí ještě drknout a převrátit tak skleničku s vodou, která ti polije většinu poznámek. Když ale vidíš ty jeho psí oči a rozzářený výraz, nedokážeš se na něj zlobit. Všechny v místnosti navíc Vojta ohromí následující větou „Já vim, že jsem jenom hloupej sporťák, kopačka, ale pro nás ITčkáři napsali program, kam jenom zadáváme výsledky a on už ty články píše za nás. Tak s nima hoďte řeč, esli by něco takovýho nešlo udělat i pro tu vaší politiku.“ A na odchodu ještě dodal „Jo a nezapomeňte, že v pátek je ten redakční volejbal, tak vás všechny očekávám v 6 na hřišti. Čágo, bélo, šílenci!“

Stojíte jako opaření, vzduch v zasedací místnosti je tak hustý, že by ho bylo možné krájet. Proč Vás to nenapadlo samotné a mnohem dříve? Na co jste mysleli? Šéf politické divize novin ihned rozdává úkoly. Jakožto nejmladší z týmu jsi pochopitelně ty ta osoba, která má zajít na IT oddělení vykomunikovat výrobu nového programu. Navíc za sebou máš i jednosemestrální kurz programování pro žurnalisty, kdo by tak mohl být povolanější, než ty?

Míříš tak k výtahu, na ovládacím panelu zadáváš -3. patro a nebojácně se vydáváš vstříc vašemu IT oddělení. Asi za 2 minuty již stojíš opět ve výtahu, tentokrát ovšem nejedeš dolu, ale nahoru. Programátoři nemají čas, manželka majitele potřebuje nutně spustit nový e-shop, což je pochopitelně mnohem prioritnější úkol. Zároveň ale nechceš zklamat svého šéfa, rozhoduješ se tedy pustit se do výroby robota vlastní silou…

_Cílem je tedy napsat program, který si bude udžovat databázi členů politických stran a pro konkrétní akce nad touto databází generovat automatické zpravodajství._

Rozhraní programu
-----------------

Vaším úkolem je implementovat základ bota pro automatické generování novinových zpráv. Tento bot se bude starat o udržování aktuálních informací o politických stranách, jejich členech a existujících koalicích. O samotnou tvorbu novinových článků se bude starat javascriptový front-end, který se na Váš kód napojí a který je použitý již pro generování sportovních zpráv. Jádro systému musí splňovat následující rozhraní:

    class CPulitzer {
        public:
        
            CPulitzer ( size_t N, size_t P );
            
            bool register_politician ( uint32_t id_party, uint32_t id_politician, const string & name, uint32_t popularity, uint8_t gender );
            
            bool politician_name ( uint32_t id_politician, string & name ) const; 
            
            bool politician_gender ( uint32_t id_politician, uint8_t & gender ) const;
            
            bool politician_popularity ( uint32_t id_politician, uint32_t & popularity ) const;
            
            bool deregister_politician ( uint32_t id_politician );
            
            bool party_leader ( uint32_t id_party, uint32_t & id_leader ) const;
            
            bool change_popularity ( uint32_t id_politician, uint32_t popularity );
            
            bool sack_leader ( uint32_t id_party );
            
            bool merge_parties ( uint32_t dest_party, uint32_t src_party );
            
            bool create_coalition ( uint32_t id_party1, uint32_t id_party2 );
            
            bool leave_coalition ( uint32_t id_party );
            
            bool coalition_leader ( uint32_t id_party, uint32_t & id_leader ) const;
            
            bool scandal_occured ( uint32_t id_party );
            
    };

`CPulitzer::CPulitzer`

Konstruktor implementace bota. Parametry jsou popořadě počet povolených politických stran a počet povolených politiků. Platí 1 ≤ _N_, _P_ ≤ 106.

`CPulitzer::register_politician`

Metoda slouží k registraci nového politika a jeho přidání do zadané politické strany. Jestliže je id politika již obsazeno, metoda nic neprovede a vrátí hodnotu `false`. V opačném případě bude návratová hodnota `true`.

`Cpulitzer::deregister_politician`

Metoda slouží k zrušení registrace zadaného politika. Pokud žádný politik se zadaným ID neexistuje, vrátí metoda hodnotu `false`. V opačném případě je politik odebrán ze strany, které je členem, jeho ID je vráceno úřadu k dalšímu použití a metoda vrátí hodnotu `true`.

`CPulitzer::party_leader`

Metoda slouží k zjištění, který politik je aktuálním předsedou strany. Pokud zadaná strana neexistuje, vrací metoda hodnotu `false`. V opačném případě vrací hodnotu `true` a id předsedy je předáno přes výstupní parametr `id_leader`. Vždy jde o toho politika, který má v rámci strany největší popularitu. Jestliže mají dva politici v jedné straně stejnou popularitu, získává post předsedy ten, kdo potřebnou hodnotu popularity získal později.

`CPulitzer::change_popularity`

Metoda změní zadanému politikovi jeho popularitu. Pokud žádný politik se zadaným ID neexistuje, vrátí metoda hodnotu `false`. V opačném případě je politikova popularita změněna a metoda vrátí hodnotu `true`.

`CPulitzer::sack_leader`

Zadaná politická strana utrpěla v posledních volbách debakl, ze kterého současný předseda této strany vyvodil osobní zodpovědnost a ukončil své politické působení. Jestliže strana se zadaným id neexistuje, metoda nic neprovede a vrátí hodnotu `false`. V opačném případě bude návratová hodnota `true`.

`CPulitzer::merge_parties`

Metoda slouží ke sloučení dvou politických stran. Dosavadní členové strany s id `src_party` tak přecházejí do `dest_party` a původní `src_party` zaniká. Jestliže některá ze zadaných stran neexistuje, metoda nic neprovede a vrátí hodnotu `false`. V opačném případě bude návratová hodnota `true`.

`CPulitzer::create_coalition`

Metoda slouží k vytvoření koalice více politických stran. Jestliže je některá ze stran již součástí nějaké koalice, jsou všechny koalice sloučeny do jedné. Jestliže některá ze zadaných stran neexistuje, metoda nic neprovede a vrátí hodnotu `false`. V opačném případě bude návratová hodnota `true`.

`CPulitzer::leave_coalition`

Zadaná strana opouští koalici, které je členem. Pokud členem žádné koalice není, nebo strana se zadaným id vůbec neexistuje, vrátí metoda hodnotu `false`. V opačném případě je návratová hodnota `true`.

`CPulitzer::scandal_occured`

Zadaná politická strana byla stižena skandálem. Za tento skandál musí přijmout odpovědnost jak předseda zadané strany, tak případný lídr koalice, které je strana členem. Jestliže strana se zadaným id neexistuje, metoda nic neprovede a vrátí hodnotu `false`. V opačném případě bude návratová hodnota `true`.

`CPulitzer::coalition_leader`

Metoda slouží k zjištění aktuálního lídra koalice, tj. osobu s největší popularitou přes všechny strany koalice, které je zadaná strana členem. Jestliže strana není členem žádné koalice, program vrátí skrz výstupní parametr `id_leader` informaci o předsedovi této strany a končí s návratovou hodnotou `true`. Pokud strana neexistuje, je návratová hodnota `false`.

* Metody `politician_name`, `politician_popularity` a `politician_gender` slouží ke zjištění informací o zadaném politikovi. Metody vracejí hodnotu `false` v případě, že politik se zadaným id neexistuje, v opačném případě vrací hodnotu `true` a zjišťovanou vlastnost ukládají do výstupního parametru.
* Id stran jsou vždy z rozsahu \[0; _N_ − 1\]. Id politiků jsou vždy z rozsahu \[0; _P_ − 1\].
* Jestliže některou z politických stran opustí i poslední člen, tato strana zaniká. Tedy je její ID uvolněno pro další použití.

Ukázky práce programu
---------------------

### Ukázka 1

    uint8_t gender;
    uint32_t popularity, id_leader;
    std::string name;
    
    CPulitzer bot( 3, 10 );
    bot.party_leader( 1, id_leader ); // false
    bot.register_politician( 1, 5, "VK", 1000, 77 ); // true
    bot.register_politician( 2, 4, "MZ", 1000, 77 ); // true
    bot.register_politician( 2, 7, "VS", 500, 77 ); // true
    bot.party_leader( 1, id_leader ); // true, 5
    bot.party_leader( 2, id_leader ); // true, 4
    bot.change_popularity( 7, 2000 ); // true
    bot.party_leader( 2, id_leader ); // true, 7
    bot.register_politician( 1, 2, "MT", 500, 77 ); // true
    bot.register_politician( 2, 2, "JP", 500, 77 ); // false
    bot.register_politician( 2, 9, "JP", 500, 77 ); // true
    bot.deregister_politician( 5 ); // true
    bot.party_leader( 1, id_leader ); // true, 2
    bot.sack_leader( 2 ); // true
    bot.change_popularity( 9, 200 ); // true
    bot.sack_leader( 1 ); // true

### Ukázka 2

    uint8_t gender;
    uint32_t popularity, id_leader;
    std::string name;
    
    CPulitzer bot( 5, 5 );
    bot.register_politician( 0, 0, "RS", 150, 77 ); // true
    bot.register_politician( 1, 1, "RS", 50, 77 ); // true
    bot.register_politician( 2, 2, "RS", 60, 77 ); // true
    bot.register_politician( 3, 3, "VKml", 100, 77 ); // true
    bot.register_politician( 3, 4, "ZMZ", 50, 70 ); // true
    bot.deregister_politician( 3 ); // true
    bot.merge_parties( 3, 2 ); // true
    bot.merge_parties( 3, 1 ); // true
    bot.party_leader( 0, id_leader ); // true, 0
    bot.party_leader( 1, id_leader ); // false
    bot.party_leader( 2, id_leader ); // false
    bot.party_leader( 3, id_leader ); // true, 2

### Ukázka 3

    uint8_t gender;
    uint32_t popularity, id_leader;
    std::string name;
    
    CPulitzer bot( 10, 10 );
    bot.register_politician( 9, 1, "MK", 100, 77 ); // true
    bot.register_politician( 0, 0, "IB", 150, 77 ); // true
    bot.register_politician( 1, 2, "VR", 50, 77 ); // true
    bot.create_coalition( 9, 1 ); // true
    bot.leave_coalition( 1 ); // true
    bot.create_coalition( 0, 1 ); // true
    bot.coalition_leader( 0, id_leader ); // true, 0
    bot.coalition_leader( 1, id_leader ); // true, 0
    bot.coalition_leader( 9, id_leader ); // true, 1
    bot.change_popularity( 2, 200 ); // true
    bot.coalition_leader( 0, id_leader ); // true, 2
    bot.leave_coalition( 9 ); // false

Bodové podmínky
---------------

* Pro zisk 2 bodů je potřeba správně a rychle vyřešit instance, kde nedochází k volání metod `merge_parties` a `create_coalition`.
* Pro zisk 5 bodů je potřeba správně a rychle vyřešit instance, kde nedochází k volání metody `create_coalition`.
* Pro zisk 10 bodů je třeba správně vyřešit libovolné instance splňující zadání.

Poznámky
--------

* Před volbou datových struktur vhodných pro udržování informací o politicích doporučujeme pečlivé přečtení celého zadání.
* Knihovna STL není pro řešení problému k dispozici. Při jejím použití nepůjde program zkompilovat a bude hodnocen 0 body. Pokud vytváříte třídu/funkci, která svým názvem koliduje s STL, vložte ji do vlastního namespace.
* Ač se může zdát příběh smyšlený, vězte, že automatizovaný žurnalismus je v dnešní době opravdu tématem. Pro zájemce doporučujeme jako výchozí bod například heslo _Automated Jurnalism_ na [Wikipedii](https://en.wikipedia.org/wiki/Automated_journalism).

* * *

1.  V reálných volbách jsou čísla přiřazená politickým stranám pro účely voleb náhodně losována.[↩](#fnref1)
