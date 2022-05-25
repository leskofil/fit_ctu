 Dotazy
------

-- D1

-- Zoznam zakaznikov, ktori dali hodnotenie 5.

A F2

-- RA

{ZAKAZNIK*hodnotenie}(vyska_hodnotenia = '10')\[meno\]

-- SQL

SELECT DISTINCT meno
FROM ZAKAZNIK
NATURAL JOIN HODNOTENIE
WHERE vyska_hodnotenia = '10';

-- D2

-- Zoznam mien zakaznikov byvajucich v state 'Japan'.

A F2

-- RA

{zakaznik * adresa}(krajina = 'Japan')

-- SQL

SELECT DISTINCT *
FROM ZAKAZNIK
NATURAL JOIN ADRESA
WHERE krajina = 'Japan';

-- D3

-- Zoznam letov nesmerujuce do 'Argentina'

A F2

-- RA

{destinaciav1*destinacia}(stat = 'Argentina')

-- SQL

SELECT DISTINCT *
FROM DESTINACIAV1
NATURAL JOIN DESTINACIA
WHERE stat = 'Argentina';

-- D4

-- Letenky s datumom odletu '22.11.2014'

-- RA

letenka(datum_odletu = '22.11.2014')

-- SQL

SELECT DISTINCT *
FROM LETENKA
WHERE datum\_odletu = TO\_DATE('22.11.2014','dd.mm.yyyy');

-- D5

-- Adresa ku ktorej nie je priradeny zakaznik

A C F2 H2

-- RA

zakaznik !*> adresa

-- SQL

SELECT DISTINCT *
FROM ADRESA
MINUS
SELECT DISTINCT id_adresa,
                supisne\_cislo\_domu,
                ulica,
                mesto,
                krajina,
                postove\_smerovacie\_cislo
FROM ADRESA
NATURAL JOIN ZAKAZNIK;

-- D6

-- Mena vsetkych zakaznikov, ktori nedali hodnotenie 6.

A B C F2 G2 H2

-- RA

{hodnotenie(vyska_hodnotenia='6')!*>zakaznik}\[meno\]

-- SQL

SELECT DISTINCT meno
FROM (
    SELECT DISTINCT *
    FROM ZAKAZNIK
    MINUS
    SELECT DISTINCT id_zakaznik,
                    meno,
                    priezvisko,
                    datum_narodenia,
                    rodne_cislo,
                    email,
                    telefonne_cislo,
                    dovod_cesty,
                    id_adresa
    FROM ZAKAZNIK
    NATURAL JOIN (
        SELECT DISTINCT *
        FROM HODNOTENIE
        WHERE vyska_hodnotenia = '6'
    ) R1
) R2;

-- D7

-- Zoznam len tych batozin ktore maju vahu '20'

A C F2 H1

-- RA

{batozina<\*tazka\_batozina(vaha\_tazka = '20')} ∪ {batozina<\*prirucna(vaha_prirucna = '20')}

-- SQL

SELECT DISTINCT id_batozina,
                id_letenka,
                batozina_typ
FROM BATOZINA
NATURAL JOIN (
    SELECT DISTINCT *
    FROM TAZKA_BATOZINA
    WHERE vaha_tazka = '20'
) R1
UNION
SELECT DISTINCT id_batozina,
                id_letenka,
                batozina_typ
FROM BATOZINA BATOZINA1
NATURAL JOIN (
    SELECT DISTINCT *
    FROM PRIRUCNA
    WHERE vaha_prirucna = '20'
) R2;

-- D8

-- Zoznam zakaznikov, ktori dali len hodnotenie '4'

C F2 G2 H2

-- RA

{hodnotenie(vyska_hodnotenia = '4') *> zakaznik}\[meno\]
\
{hodnotenie(vyska_hodnotenia != '4') *> zakaznik}\[meno\] 

-- SQL

SELECT DISTINCT meno
FROM (
    SELECT DISTINCT id_zakaznik,
                    meno
    FROM ZAKAZNIK
    NATURAL JOIN (
        SELECT DISTINCT *
        FROM HODNOTENIE
        WHERE vyska_hodnotenia = '4'
    ) R1
) R2
MINUS
SELECT DISTINCT meno
FROM (
    SELECT DISTINCT id_zakaznik,
                    meno
    FROM ZAKAZNIK ZAKAZNIK1
    NATURAL JOIN (
        SELECT DISTINCT *
        FROM HODNOTENIE HODNOTENIE1
        WHERE vyska_hodnotenia != '4'
    ) R3
) R4;

-- D9

-- Lety ktore, smeruju do vsetkych destinacii

D1 G1 G4

-- RA

destinaciav1\[id\_let, id\_destinacia\] ÷ destinacia\[id_destinacia\] 

-- SQL

select id_let
from let l
where not exists
(select id_let
from destinacia d
where not exists ( select id_let
                    from destinaciav1 v
                    where l.id\_let = v.id\_let and v.id\_destinacia = d.id\_destinacia)

                )

-- D10

-- Vsetky prirucne batoziny sirky '30'

-- RA

prirucna(sirka\_prirucna = '30')\[id\_batozina\]

-- SQL

SELECT DISTINCT id_batozina
FROM PRIRUCNA
WHERE sirka_prirucna = '30';

-- D11

-- Hodnotenia zakaznikov

A F1

-- SQL

select hodnotenie.*
from zakaznik join hodnotenie on zakaznik.id\_zakaznik = hodnotenie.id\_zakaznik 

-- D12

-- Zakaznici, ktori nikdy nenapisali hodnotenie, maju id mensie ako 10, zoradeni zostupne

G1 G4

-- SQL

select * from zakaznik z where not exists (
    select * from hodnotenie h where z.id\_zakaznik=h.id\_zakaznik
) and id_zakaznik <= 10
order by id_zakaznik desc

;

-- D13

-- Vsetci zakaznici ktori, sedia na sedadle 200 alebo byvaju v Nigerii.

A F1 H1

-- SQL

select z.*
from zakaznik z join letenka l on (l.id\_zakaznik=z.id\_zakaznik)
where sedadlo = 200
union
select z.*
from zakaznik z join adresa a on (a.id\_adresa=z.id\_adresa)
where krajina = 'Nigeria'
; 

-- D14

-- Vsetci zakaznici viac ako 1 letenka

A F2 I1 I2 K

-- SQL

select id\_zakaznik, meno, count(id\_letenka) as letenka_count
from zakaznik join letenka using (id_zakaznik)
where meno like 'D%'
group by id_zakaznik, meno
having count(id_letenka) > 1
order by id_zakaznik asc

-- D15

-- Zvysenie ceny prestupnych letov 1.5x

A F2 G1 O

-- SQL

(select id_letenka, cena 
from letenka join let using(id_let)
where prestupnost = 'prestupny')
;
update letenka
set cena = cena * 1.5
where id\_letenka in (select id\_letenka 
from letenka join let using(id_let)
where prestupnost = 'prestupny')
;
(select id_letenka, cena 
from letenka join let using(id_let)
where prestupnost = 'prestupny')
;
rollback;

-- D16

-- O kolko by stupol pocet letov, keby kazdy let smeroval do kazdej destinacie?

F3 G2 H2 I1

-- SQL

select count(*) as potencialne_lety
from (select id\_let, id\_destinacia
from let cross join destinacia
minus
select *
from destinaciav1) 

-- D17

-- View Prirucna batozina, ktora je tazsia ako 5.

L

-- SQL

create or replace view tazka\_prirucna\_batozina as
    select *
    from prirucna
    where vaha_prirucna > 5
with check option;

-- D18

-- View Prirucna batozina, ktora je tazsia ako 5 a vysia ako 20.

L M

-- SQL

create or replace view tazka\_prirucna\_batozina as
    select *
    from prirucna
    where vaha_prirucna > 5
with check option;

select * 
from tazka\_prirucna\_batozina
where vyska_prirucna > 20 

-- D19

-- 3 krat zakaznici co nenapisali hodnotenie

A B F1 F2 G1 G4 H2 J

-- SQL

select id\_hodnotenie, vyska\_hodnotenia
from hodnotenie
minus
select id\_hodnotenie, vyska\_hodnotenia
from hodnotenie join zakaznik using (id_zakaznik)
;

select id\_hodnotenie, vyska\_hodnotenia
from hodnotenie
where id\_hodnotenie not in ( select id\_hodnotenie
                                from zakaznik join hodnotenie using (id_zakaznik)
                             )
;

select id\_hodnotenie, vyska\_hodnotenia
from hodnotenie h
where not exists ( select id_hodnotenie
                     from zakaznik z join hodnotenie on h.id\_zakaznik=z.id\_zakaznik
                    )
order by id_hodnotenie asc
; 

-- D20

-- Kontrola D1 dotazu

D2 G1 G4

-- SQL

select * 
from destinacia d 
where id_destinacia not in (
    select id_destinacia 
    from destinaciav1 v
    where id_let in 
            (select id_let
            from let l 
            where not exists
            (select * 
            from destinacia 
            where not exists ( select * 
                                from destinaciav1
                                where l.id\_let = v.id\_let and v.id\_destinacia = d.id\_destinacia)
    )
    )
    ) 

-- D21

-- Id cinskych zakaznikov, ktory maju dvovod cesty praca.

G1 G3

-- SQL

select z.id_zakaznik, meno, priezvisko, (
    select id_adresa from adresa a
    where a.krajina='China' and z.id\_adresa = a.id\_adresa) as id\_adresa\_cinski_zakaznici
from zakaznik z
where  dovod_cesty='praca' and (
    select id_adresa from adresa a
    where a.krajina='China' and z.id\_adresa = a.id\_adresa) is not null

-- D22

-- Zakaznici, ktori hodnotili viac ako 2krat.

A F4 I1 I2

-- SQL

select  z.id\_zakaznik, meno, count (z.id\_zakaznik) as pocet_hodniteni
from zakaznik z left join hodnotenie h on z.id\_zakaznik=h.id\_zakaznik
group by z.id_zakaznik, meno
having count (z.id_zakaznik) >2

-- D23

-- Id zakaznikov , vcetne tych ktore nemaju batozinu

A F4 F5

-- SQL

select z.id_zakaznik
from letenka l full join batozina b on l.id\_letenka=b.id\_letenka
 left join zakaznik z on l.id\_zakaznik = z.id\_zakaznik 

-- D24

-- Pridanie novej destinacie do ktorej nikto neleti a jej nasledne odstanenie.

G1 I1 P

-- SQL

insert into destinacia values (51,'Kosice','Kassa','Slovakia');
delete destinacia
where id_destinacia in
    (select id_destinacia 
    from destinacia 
    where id_destinacia not in 
        (select id_destinacia from destinaciav1)
    );
select count(id\_destinacia ) as Pocet\_destinacii 
from destinacia 
where id_destinacia 
not in (select id_destinacia from destinaciav1
        )
;
rollback;

-- D25

-- Spocitanie vsetkych moznosti letov, nasledne insertovanie tych ktore este neprebehli a znova spocitanie.

F3 G1 I1 N

-- SQL

select count(*) 
from let cross 
join destinacia
where (id\_let, id\_destinacia) not in 
    (select id\_let, id\_destinacia 
    from destinaciav1);

insert into destinaciav1(id\_destinacia,id\_let)
select id\_destinacia,id\_let from let cross join destinacia
where (id\_let, id\_destinacia) not in 
    (select id\_let, id\_destinacia 
    from destinaciav1);


select count(*) 
from let cross join destinacia
where (id\_let, id\_destinacia) not in 
(select id\_let, id\_destinacia from destinaciav1);
rollback;

-- D26

-- Lety ktore leteli do Konklux a zaroven leteli aj do mesta Borovany.

A F2 H3

-- SQL

select *
from destinaciav1 join destinacia using (id_destinacia)
where nazov_destinacia = 'Konklux'
intersect
select *
from destinaciav1 join destinacia using (id_destinacia)
where mesto = 'Borovany'

-- D27

-- Batoziny s vyskou mensou ako 20

A F2

-- SQL

select * 
from batozina join letenka using (id_letenka)
    join prirucna using (id_batozina)
where vyska_prirucna < '20'

-- D28

D2 G1 G4

-- SQL

select * 
from destinacia 
where id_destinacia not in (    
    select id_destinacia 
    from destinaciav1 
    where id_let in 
            (select id_let l
            from let l 
            where not exists
            (select * 
            from destinacia d
            where not exists ( select * 
                                from destinaciav1 v
                                where l.id\_let = v.id\_let and v.id\_destinacia = d.id\_destinacia)
    )
    )
    )

-- D29

A F1 F2

-- SQL

select * from zakaznik join hodnotenie using (id_zakaznik);
select * from zakaznik z join hodnotenie h on (z.id\_zakaznik=h.id\_zakaznik);
select * from zakaznik natural join hodnotenie;

-- D30

G3

-- RA

select z.id_zakaznik, meno, priezvisko, (
    select id_adresa from adresa a
    where a.krajina='China' and z.id\_adresa = a.id\_adresa) as id\_adresa\_cinski_zakaznici
from zakaznik z
where  dovod_cesty='praca' and (
    select id_adresa from adresa a
    where a.krajina='China' and z.id\_adresa = a.id\_adresa) is not null

-- SQL

select z.id_zakaznik, meno, priezvisko, (
    select id_adresa from adresa a
    where a.krajina='China' and z.id\_adresa = a.id\_adresa) as id\_adresa\_cinski_zakaznici
from zakaznik z 

