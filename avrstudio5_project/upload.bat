@set MK=m2561
@set PRG=usbasp
@set FLASHFILE=board_1\Debug\board_1.hex
@set AVRDUDEPATH=C:\avrdude\

%AVRDUDEPATH%avrdude -p %MK% -c %PRG% -U flash:w:%FLASHFILE%

pause

:: Manual run:
:: c:\avrdude>avrdude -p m2561 -c usbasp -U flash:w:"board_1\Debug\board_1.hex":i