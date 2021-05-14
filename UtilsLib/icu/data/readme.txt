Файл convrtrs.txt используется для компиляции в cnvalias.icu: gencnval.exe convrtrs.txt

Включение cnvalias.icu в icudt55l.dat: icupkg.exe -a cnvalias.icu icudt55l.dat

Если требуется включить в icudt55l.dat новые конвертеры (обычно это ucm-файлы), то:

1) компиляция ucm в cnv: makeconv.exe <имя>.ucm
2) включение cnv в icudt55l.dat: icupkg.exe -a <имя>.cnv icudt55l.dat

Утилиты и исходники конвертеров можно взять с официального сайта ICU