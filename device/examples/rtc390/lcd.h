extern void LcdInit(void);
extern void LcdOn(void);
extern void LcdOff(void);
extern void LcdClear(void);
extern void LcdHome(void);
extern void LcdGoto(unsigned int collumnRow);
extern void LcdPutChar(char c);
extern void LcdPutString(char *string);
extern void LcdLPutString(unsigned int collumnRow, char *string);
extern void LcdPrintf(xdata const char *format, ...) reentrant;
extern void LcdLPrintf(unsigned int collumnRow, xdata const char *format, ...) reentrant;

