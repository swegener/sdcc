del small\*.lib
set SDCCLIB_CC=sdcc --use-stdout --model-small -c
sdcclib -l small\libfloat.lib libfloat.lib
sdcclib -l small\libint.lib libint.lib
sdcclib -l small\liblong.lib liblong.lib
sdcclib -l small\libsdcc.lib libsdcc.lib
cd mcs51
@for %%I in (*.asm) do sdas8051 -plosgff %%I
@for %%I in (*.rel) do sdcclib ..\small\mcs51.lib %%I
cd..
del *.rel *.sym *.asm *.lst *.adb

del medium\*.lib
set SDCCLIB_CC=sdcc --use-stdout --model-medium -c
sdcclib -l medium\libfloat.lib libfloat.lib
sdcclib -l medium\libint.lib libint.lib
sdcclib -l medium\liblong.lib liblong.lib
sdcclib -l medium\libsdcc.lib libsdcc.lib
cd mcs51
@for %%I in (*.asm) do sdas8051 -plosgff %%I
@for %%I in (*.rel) do sdcclib ..\medium\mcs51.lib %%I
cd..
del *.rel *.sym *.asm *.lst *.adb

del large\*.lib
set SDCCLIB_CC=sdcc --use-stdout --model-large -DUSE_FLOATS -c
sdcclib -l large\libfloat.lib libfloat.lib
sdcclib -l large\libint.lib libint.lib
sdcclib -l large\liblong.lib liblong.lib
sdcclib -l large\libsdcc.lib libsdcc.lib
cd mcs51
@for %%I in (*.asm) do sdas8051 -plosgff %%I
@for %%I in (*.rel) do sdcclib ..\large\mcs51.lib %%I
cd..
del *.rel *.sym *.asm *.lst *.adb

set SDCCLIB_CC=