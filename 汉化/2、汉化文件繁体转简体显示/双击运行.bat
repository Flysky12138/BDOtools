@echo off
set /p="ʱ��ϳ������ĵȴ� "<nul
set /p="1����ʼ ���� "<nul
"bin/BDO_decrypt" languagedata_en.loc languagedata_tw.txt
"bin/BDO_decrypt" languagedata_en.loc.bak languagedata_en.txt
del languagedata_en.loc
echo.
set /p="2����ʼ ת�� "<nul
"bin/iconv" -f UTF-16LE -t UTF-8 languagedata_en.txt > languagedata_en_utf8.txt
"bin/iconv" -f UTF-16LE -t UTF-8 languagedata_tw.txt > languagedata_tw_utf8.txt
del languagedata_en.txt languagedata_tw.txt
echo.
set /p="3����ʼ ���� "<nul
"bin/opencc-1.0.1-win64/opencc" -i languagedata_tw_utf8.txt -o languagedata_tw_t2s.txt -c "bin/opencc-1.0.1-win64/t2s.json"
del languagedata_tw_utf8.txt
echo.
echo 4����ʼ ���
echo.
"bin/ReplaceLanguage" languagedata_tw_t2s.txt languagedata_en_utf8.txt languagedata_tw_mx.txt
del languagedata_tw_t2s.txt languagedata_en_utf8.txt
echo.
set /p="4����ʼ ת�� "<nul
"bin/iconv" -f UTF-8 -t UTF-16LE languagedata_tw_mx.txt > languagedata_tw_utf16.txt
del languagedata_tw_mx.txt
echo.
set /p="5����ʼ ���� "<nul
"bin/BDO_encrypt.exe" languagedata_tw_utf16.txt languagedata_en.loc
copy /y languagedata_en.loc "%USERPROFILE%/Desktop/languagedata_en.loc"
del languagedata_tw_utf16.txt
echo.
echo.
pause
