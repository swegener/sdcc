set SDCCFLAGS=--debug --stack-after-data --model-large 
cls
cd src
sdcc --version
sdcc %SDCCFLAGS% -c startup_code.c
sdcc %SDCCFLAGS% -c cpu_c515a.c
sdcc %SDCCFLAGS% startup_code.rel cpu_c515a.rel
del c:\tmp\t*.*
cd ..
pause
c.bat
