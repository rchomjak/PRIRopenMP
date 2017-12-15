Zadanie 06 - OpenMP

Idea zadania 6 to “eksperymentalne” rozwiązanie pewnego problemu z dziedziny rachunku prawdopodobieństwa.

Niech badanym problemem będzie takie oto zadanie: W urnie znajduje się pewna liczba kul (N) ponumerowanych od 1 wzwyż do N włącznie. Losowo wybieramy część z nich (L). Losowanie wykonujemy bez zwracania, czyli wylosowane kulki nie wracają do urny i każda wybierana jest tylko raz. Aby samo losowanie nie były takie proste, kulki wyciągane są z różnym prawdopodobieństwem. Prawdopodobieństwo wybrania kulki zależne jest od numeru kulki i od tego- jako która jest ta kulka wybierana.

Jako rozwiązanie zadania interesuje nas najczęściej i średnio pojawiająca się suma numerow wylosowanych kulek.

Program który państwo otrzymali jest poprawną, ale sekwencyjną, wersją kodu, który symuluje wiele eksperymentów-losowań. Wyniki poszczególnych eksperymentów (czyli sumy numerów wybranych kulek) trafiają do histogramu. Po pewnej liczbie prób histogram jest przeszukiwany i prezentowane są wyniki obliczeń.

Państwa zadanie polega na tym aby wykorzystując wątki (OpenMP) zwiększyć liczbę eksperymentów przeprowadzanych w czasie trwania pracy programu. Proszę zwrócić uwagę na to, że zmiana liczby kulek, liczby losowań oraz prawdopodobieństwa wybrania kulki w znaczący sposób może zmienić czas trwania poszczególnych części kodu. Dane wejściowe mogą spowodować, że samo przeglądanie histogram będzie krótkie (np. wtedy gdy losujemy mało kulek z niewielkiej ich ilości), jednak gdy zwiększymy liczby kulek/liczbę losowań to rozmiar histogramu znacząco wzrośnie. Podobnie zwiększenie prawdopodobieństwa wybrania każdej z kulek doprowadzi do tego, że czas trwania pojedynczego eksperymentu będzie krótki; jeśli jednak prawdopodobieństwo wybrania kulki spadnie, to należy się spodziewać, że czas jednego eksperymentu wielokrotnie się wydłuży.

Państwa zadaniem jest takie zmodyfikowanie kodu (nie chodzi optymalizacji samego algorytmu liczenia, lecz tylko jego zrównoleglenie) aby program działał efektywnie w różnych warunkach losowania. Zwracam też uwagę na to, że program zawsze będzie kończyć pracę po tym samym okresie czasu, chodzi więc o to aby w histogramie znajdowało się możliwie jak najwięcej wyników pojedynczych eksperymentów.

Rozwiązania testowane będą na komputerach posiadających jeden wielordzeniowy procesor. Można się spodziewać testów w zakresie od 1 do 6 wątków - liczba watków ustalana bedzie poprzez zmienną srodowiskowa OMP_NUM_THREADS - proszę nie zmieniać ustawien liczby watków we wlasnym kodzie!

Koniecznie! nalezy w rozwiazaniu zadania wymienic generator liczb losowych z rand() na drand48_r. Wymieniajac generator trzeba zadbać o

    poprawne zainicjowanie struktury kontrolnej generatora i oto, aby struktura ta byla zainicjowana inaczej dla różnych watków.
    efektywną inicjację - niedopuszczalne jest inicjowanie generatora przed niemal każdym użyciem
    aby watki nie powielaly swojej pracy (ponowne i identyczne zainicjowanie generatora prowadzi do tej samej sekwencji losowanych liczb)
    aby watki nie powielaly pracy innych watków (zainicjowanie genetratora tak jak wczesniej w innym watku prowadzi ponownie do pojawienia się tej samej sekwencji liczb i takich samych wyników jak już otrzymano) 

Kod musi dać się skompilować i działać na maszynach dostepowych (spk-ssh, spk-k04 czy ssh-k05) i komputerach pracowni studenckich.

Państwa praca wiąże się wyłącznie z modyfikacją klasy Experiment - pozostałe części kodu są w mojej gestii.

Podobnie jak w zadaniu z MPI możliwe jest zmodyfikowanie pliku nagłówkowego klasy Experiment, proszę jednak nie zmieniać części "publicznej" klasy - kod nadal ma się kompilować jako całość. Sposób przesłania projektu wraz z uwagami o nazwach plików - jak w zadaniu MPI
Kompilacja kodu

W aktualnej (sekwencyjnej) wersji kod kompilowany jest następująco:

c++ Distribution.cpp Experiment.cpp Result.cpp main.cpp

Wkrywanie problemów z wątkami za pomocą Intel Inspector XE

Uwaga: aby móc używać tego programu trzeba zalogować się na komputery spk-k04.if.uj.edu.pl lub spk-k05.if.uj.edu.pl i wpisać:

        source /app/intel/bin/iccvars.sh intel64
        source /app/intel/inspector_xe/inspxe-vars.sh
    

Aby uniknąć problemów ze zdalną pracą w trybie graficznym, test można wykonać w trybie terminalowym.

icc -g -O -openmp Distribution.cpp Experiment.cpp Result.cpp main.cpp
            
inspxe-cl -collect ti3 -- ./a.out
inspxe-cl -report summary > SUMMARY.txt
inspxe-cl -report problems -format text -report-output myThreadingStatus.txt

