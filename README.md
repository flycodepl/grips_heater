Regulator podgrzewanych manetek do motocykla (Grips Heater)
============

Sercem regulatora jest attiny44. Generuje on sygnał prostokątny o różnym wypełnieniu (zależnie od oczekiwanej mocy grzania manetek), którym steruje tranzystor mosfetowy (IRL540N - datasheat: http://www.uni-kl.de/elektronik-lager/413916).

## Schemat ideowy:
![schemat] (https://raw.github.com/flycodepl/grips_heater/master/eagle_schema/schemat.png)

Wartości elemantów:
```
IC1         - attiny44
IC2         - 78M05
Q1          - IRL540N
C1,C3,C5,C6 - 100nF
R1-R4       - 100 Ohm
R5,R6       - 220Ohm
```

## Płytka PCB:
![pcb] (https://raw.github.com/flycodepl/grips_heater/master/eagle_schema/pcb.png)

## Uwagi i błędy:
Jest kilka szczegółów które wyszły w trakcie składania i nie chciało mi się już nanosić tego na schemat.

* Zaraz za stabilizatorem należy dać kondensator elektrolityczny ponieważ bez niego na wyjściu PWM pod czas stanu wysokiego pojawiają się 'szpilki' (tylko kiedy diody są w stanie 'standby'). Ja dałem 10uF 25V + 47uF 10V. Można dać jeden większy ale trzeba pilnować żeby całość zmieściła się w obudowie. W przyszłości dam go na samej płytce.

* Wyjście na manetki jest błędne - na schematcie i projekcie PCB jest to masa i dren (środkowa nóżka). Jednak powinno być dren i +12V całego układu.

* Ponieważ jest to pierwsze wersja i jest dopiero w testach, dodałem wejście na programator - dzięki temu można łatwo i szybko zrobić update softu.

## Gotowy układ:
![pcb] (https://raw.github.com/flycodepl/grips_heater/master/eagle_schema/pcb_finish.png)
![pcb] (https://raw.github.com/flycodepl/grips_heater/master/eagle_schema/real_top.png)
![pcb] (https://raw.github.com/flycodepl/grips_heater/master/eagle_schema/real_bottom.png)

