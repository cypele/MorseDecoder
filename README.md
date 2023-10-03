# MorseDecoder
# 1. Opis projektu
Projekt umożliwia dekodowanie kodu Morse’a wysyłanego za pomocą światła mikrokontrolerem KL05Z,
oraz wyświetlanie zdekodowanej wiadomości.

# 2. Instrukcja użytkownika
Wyświetlacz po uruchomieniu płytki, wyświetli komunikat powitalny oraz po kilku sekundach pusty ekran,
który oznacza, że można zacząć nadawać kod Morse’a za pomocą latarki w stronę czujnika światła:
kropka<0.1s, kreska>0.1s && kreska<0.2s. Jeśli podany znak nie istnieje w alfabecie Morse’a wyświetli się
komunikat „Wrong letter!”, jeśli sygnał świetlny będzie dłuższy niż 0.2s to do naszego słowa zostanie dopisany
znak przerwy (‘ ‘) oraz całe słowo zostanie zapisane w tablicy całej wiadomości którą można wyświetlić za
pomocą nadania pięciu kropek („…..”).

# 3. Schemat połączeń
Do poprawnego działania programu będziemy potrzebować :
• Wyświetlacz LCD1602A
FRDM-KL05 - LCD:
PTB3-SCL
PTB4-SDA
+5V-VCC
GND-GND

• Czujnik światła ALS-PT19
FRDM-KL05: ALS-PT19:
3.3V-„+”
GND-„-”
PTB11-OUT

# 4. Inne
Program został napisany w środowisku Keil uVision5 oraz został skompilowany przy pomocy Arm Compiler
version 6
