# GAL - Grafové algoritmy - Porovnání - Hledání párování maximální kardinality

### Členové týmu:

- Jan Vybíral, xvybir05
- Jiří Chromečka, xchrom12

### Zadání:

Úkolem projektu bylo implementovat a porovnat dva různé algoritmy pro hledání párování maximální kardinality.

Zvolené algoritmy:

- Edmonds-Karp
- Hopcroft-Karp

Zvolené grafy jsou nevážené, neorientované a bipartitní.

### Ovládání programu:

Program má celkem 6 přepínačů, které mohou být zadávány v libovolném pořadí:

    -h: zobrazí nápovědu
    -c B G E: vygeneruje bipartitní graf s počty uzlů B a G a počtem hran E
    -l F: načte bipartitní graf ze souboru F
    -f M: spočítá maximální párování metodou M (h pro Hopcroft-Karp, e pro Edmonds-Karp)
    -g: vytiskne vstupní graf
    -m: vytiskne maximální párování

Soubor se vstupním grafem musí být ve specifickém  formátu: na každém řádku jsou jména uzlů oddělená středníky. 
První řádek obsahuje všechny uzly z první množiny, druhý řádek všechny uzly z druhé množiny a na ostatních řádcích jsou seznamy sousedů (vždy jako uzel následovaný svými sousedy). Viz. přiložený vzorový vstupní soubor [input.txt](input.txt).

### Dokumentace:

[doc/maxMatch.pdf](doc/maxMatch.pdf)

### Hodnocení:

**Komentář učitele k hodnocení:**

Porovnání ok. Výsledky potvrzují předpoklad, dokumentované a zdůvodněné. Testy ok. Bylo by vhodné porovnat také prostorové složitosti obou algoritmů.

**24/25**