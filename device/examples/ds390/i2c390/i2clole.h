#ifndef _I2C_H
#define _I2C_H

#define I2C_BUFSIZE     128

extern char I2CReset(void);
extern char I2CStart(void);
extern char I2CStop(void);
extern char I2CSendStop(char addr, char count, 
				 char send_stop);
extern char I2CReceive(char addr, char count);
extern char I2CSendReceive(char addr, char tx_count, char rx_count);
extern char I2CByteOut(char);
extern void I2CDumpError(char);
extern void I2CDelay(volatile long);

/*
 * Macro for normal send transfer ending with a stop condition
 */

#define I2CSend(addr, count)   I2CSendStop(addr, count, 1)

/*
 * Global variables in i2clole.c
 */

extern char i2cTransmitBuffer[];     /* Transmit buffer */
extern char i2cReceiveBuffer[];     /* Receive buffer */
extern char i2cError;          /* Set to last error value, see below */

/*
 * I2C error values
 */

#define I2CERR_OK       0       /* No error */
#define I2CERR_NAK      1       /* No ACK from slave */
#define I2CERR_LOST     2       /* Arbitration lost */
#define I2CERR_BUS      3       /* Bus is stuck (not used yet) */
#define I2CERR_TIMEOUT  4       /* Timeout on bus */

#endif







