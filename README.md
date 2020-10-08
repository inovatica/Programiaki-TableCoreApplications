### Programiaki

##### Opis
system jest podzielony na dwie części - backend i frontend

1. Backend składa się z:
1.1 Aplikacji działającej na Arduino
1.2 Aplikacji agregującej dane z Arduono i przesyłającej do serwera Websocket
1.3 Serwera Websocket


1.1 Aplikacja działająca na mikrokontrolerze Arduono ma za zadanie zczytać z podłączonych do mikrokontrolera czytników RFID. Po zaczytaniu wartości czytnika wpisuje go do tablicy z kartami - jeśli na czytniku nie jest położona żadna karta wówczas podstawia wartość 404 oznaczającą "brak karty". Aplikacja używa protokołu komunikacji i2c w trybie slave - gdy dostanie wskazane zadanie (tabela poniżej) od urządzenia pracującego w trybie "master" następne żądanie danych będzie odpowiedzią na żądane zadanie. 

1.2 Aplikacja napisana w języku Python w wersji języka 3.4. Aplikacja działa na urządzeniu komunikujące się w protokole i2c w trybie master. W pętli odpytuje się podłączone do urządzenia (i skonfigurowane w aplikacji) mikrokontrolery Arduino. Jeśli na pytanie "czy jest coś nowego" dostanie odpowiedź twierdzącą wówczas uruchamia żądanie zwracające nową kartę. Otrzymaną informację przekazuje do serwera websocketowego.

1.3 Serwer websocket obsługuje połączenia przychodzące i rozpropagowuje przychodzące treści do wszystkich podłączonych klientów.


|KOD ZADANIA|OPIS ZADANIA|
|:-------------|:--------------------------|
|68|Reset Wysłanych Kart|
|70|Ile jest podłączonych czytników RFID|
|72|Czy pośród aktualnych kart istnieją karty jeszcze nie wysłane|
|74|Który czytnik jeszcze nie został wysłany|
|76|Jak długa będzie odpowiedź|
|78|Podaj kolejną cyfrę z numeru karty. Żądanie powtażane tyle razy ile wyniesie odpowiedź na zadanie 76|



|KOD ZADANIA|KODY_ODPOWIEDZI|OPIS|
|:-------------|:--------------------------|:--------------------------|
|70|int|Liczba czytników podłączona pod miktokontroler|
|72|20|Tak istnieją|
|72|22|Nie, nie istnieją|
|74|int|Numer czytnika który nie został jeszcze przesłany|
|76|int|Po zczytaniu karty mają one różne długości - zwraca długość numeru karty|
|78|int|Zwraca jeden numer z ciągu znaków i ustawia index na koleną pozycję|
