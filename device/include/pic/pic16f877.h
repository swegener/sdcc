/*

Small Device C Compiler (SDCC) - PIC16F877 Device Library Header 2004

Note an alterative header file can be generated from 
inc files using peal script support/scripts/inc2h.pl

*/

#ifndef PIC16F877_H
#define PIC16F877_H


/*****************************************************************************
	RAM
 *****************************************************************************/

#pragma maxram 0x1FF

#pragma memmap 0x0020 0x006f RAM 0x000
#pragma memmap 0x0070 0x007f RAM 0x180
#pragma memmap 0x00a0 0x00ef RAM 0x000
#pragma memmap 0x0110 0x016f RAM 0x000
#pragma memmap 0x0190 0x01ef RAM 0x000


/*****************************************************************************
	Special Function Register Addresses
 *****************************************************************************/

enum {
	indf_addr       = 0x0000,
	tmr0_addr       = 0x0001,
	pcl_addr        = 0x0002,
	status_addr     = 0x0003,
	fsr_addr        = 0x0004,
	porta_addr      = 0x0005,
	portb_addr      = 0x0006,
	portc_addr      = 0x0007,
	portd_addr      = 0x0008,
	porte_addr      = 0x0009,
	pclath_addr     = 0x000a,
	intcon_addr     = 0x000b,
	pir1_addr       = 0x000c,
	pir2_addr       = 0x000d,
	tmr1l_addr      = 0x000e,
	tmr1h_addr      = 0x000f,
	t1con_addr      = 0x0010,
	tmr2_addr       = 0x0011,
	t2con_addr      = 0x0012,
	sspbuf_addr     = 0x0013,
	sspcon_addr     = 0x0014,
	ccpr1l_addr     = 0x0015,
	ccpr1h_addr     = 0x0016,
	ccp1con_addr    = 0x0017,
	rcsta_addr      = 0x0018,
	txreg_addr      = 0x0019,
	rcreg_addr      = 0x001a,
	ccpr2l_addr     = 0x001b,
	ccpr2h_addr     = 0x001c,
	ccp2con_addr    = 0x001d,
	adresh_addr     = 0x001e,
	adcon0_addr     = 0x001f,
	option_reg_addr = 0x0081,
	trisa_addr      = 0x0085,
	trisb_addr      = 0x0086,
	trisc_addr      = 0x0087,
	trisd_addr      = 0x0088,
	trise_addr      = 0x0089,
	pie1_addr       = 0x008c,
	pie2_addr       = 0x008d,
	pcon_addr       = 0x008e,
	sspcon2_addr    = 0x0091,
	pr2_addr        = 0x0092,
	sspadd_addr     = 0x0093,
	sspstat_addr    = 0x0094,
	txsta_addr      = 0x0098,
	spbrg_addr      = 0x0099,
	adresl_addr     = 0x009e,
	adcon1_addr     = 0x009f,
	eedata_addr     = 0x010c,
	eeadr_addr      = 0x010d,
	eedath_addr     = 0x010e,
	eeadrh_addr     = 0x010f,
	eecon1_addr     = 0x018c,
	eecon2_addr     = 0x018d,
	config_addr     = 0x2007
};

/* Special function register memory map - memmap start_addr end_addr type bank_mask */
#pragma memmap 0x001 0x001 SFR 0x080
#pragma memmap 0x002 0x004 SFR 0x180
#pragma memmap 0x005 0x005 SFR 0x000
#pragma memmap 0x006 0x006 SFR 0x080
#pragma memmap 0x007 0x009 SFR 0x000
#pragma memmap 0x00a 0x00b SFR 0x180
#pragma memmap 0x00c 0x01f SFR 0x000
#pragma memmap 0x081 0x081 SFR 0x100
#pragma memmap 0x085 0x085 SFR 0x000
#pragma memmap 0x086 0x086 SFR 0x100
#pragma memmap 0x087 0x089 SFR 0x000
#pragma memmap 0x08c 0x08e SFR 0x000
#pragma memmap 0x091 0x094 SFR 0x000
#pragma memmap 0x098 0x099 SFR 0x000
#pragma memmap 0x09c 0x09f SFR 0x000
#pragma memmap 0x10c 0x10f SFR 0x000
#pragma memmap 0x18c 0x18d SFR 0x000


/*****************************************************************************
	Misc Registers
 *****************************************************************************/

/* ---- STATUS Bits -------------------------------------------------------- */
typedef union {
	struct {
		unsigned c      :1; /* Carry/borrow bit (ADDWF,ADDLW,SUBWF,SUBLW instructions) */
		unsigned dc     :1; /* Digit carry/borrow bit (ADDWF,ADDLW,SUBWF,SUBLW instructions) */
		unsigned z      :1; /* Zero bit */
		unsigned not_pd :1; /* Power down bit */
		unsigned not_to :1; /* Timeout bit */
		unsigned rp0    :1; /* Register bank select bits for direct addressing */
		unsigned rp1    :1;
		unsigned irp    :1; /* Register bank select bits for indirect addressing */
	};
	unsigned char r;
} status_t;

static volatile status_t at status_addr status_b;
#define C      status_b.c
#define DC     status_b.dc
#define Z      status_b.z
#define NOT_PD status_b.not_pd
#define NOT_TO status_b.not_to
#define RP0    status_b.rp0
#define RP1    status_b.rp1
#define IRP    status_b.irp

/* ---- OPTION_REG Bits ----------------------------------------------------- */
typedef union {
	struct {
		unsigned ps0      :1; /* Prescaler rate select bits */
		unsigned ps1      :1;
		unsigned ps2      :1;
		unsigned psa      :1; /* Prescaler assignment bit */
		unsigned t0se     :1; /* TMR0 source edge select bit */
		unsigned t0cs     :1; /* TMR0 clock source select bit */
		unsigned intedg   :1; /* Interrupt edge select bit */
		unsigned not_rbpu :1; /* PortB pullup enable bit */
	};
	struct {
		unsigned ps:3;
	};
	unsigned char r;
} option_reg_t;

static volatile option_reg_t at option_reg_addr option_reg_b;
#define PS0      option_reg_b.ps0
#define PS1      option_reg_b.ps1
#define PS2      option_reg_b.ps2
#define PSA      option_reg_b.psa
#define T0SE     option_reg_b.t0se
#define T0CS     option_reg_b.t0cs
#define INTEDG   option_reg_b.intedg
#define NOT_RBPU option_reg_b.not_rbpu

/* ---- PCON Bits ---------------------------------------------------------- */
typedef union {
	struct {
		unsigned not_bor :1; /* Brown out reset status bit */
		unsigned not_por :1; /* Power on reset bit */
	};
	unsigned char r;
} pcon_t;

static volatile pcon_t at pcon_addr pcon_b;
#define NOT_BOR pcon_b.not_bor
#define NOT_POR pcon_b.not_por

/* ---- Misc Registers ----------------------------------------------------- */
static volatile unsigned char at indf_addr indf;
sfr at status_addr     status;
sfr at pcl_addr        pcl;
sfr at pclath_addr     pclath;
sfr at fsr_addr        fsr;
sfr at option_reg_addr option_reg;
sfr at pcon_addr       pcon;
sfr at pr2_addr        pr2;

#define INDF       indf
#define STATUS     status
#define PCL        pcl
#define PCLATH     pclath
#define FSR        fsr
#define OPTION_REG option_reg
#define PCON       pcon
#define INDF       indf
#define PR2        pr2


/*****************************************************************************
	Ports
 *****************************************************************************/

typedef struct {
	unsigned b0 :1;
	unsigned b1 :1;
	unsigned b2 :1;
	unsigned b3 :1;
	unsigned b4 :1;
	unsigned b5 :1;
	unsigned b6 :1;
	unsigned b7 :1;
} bits_t;

typedef union {
	bits_t;
	unsigned char r;
} port_t;

typedef union {
	bits_t;
	unsigned char r;
} tris_t;

/* --- PORTA Bits ---------------------------------------------------------- */
static volatile port_t at porta_addr porta_b;
#define PA0 porta_b.b0
#define PA1 porta_b.b1
#define PA2 porta_b.b2
#define PA3 porta_b.b3
#define PA4 porta_b.b4
#define PA5 porta_b.b5
#define PA6 porta_b.b6
#define PA7 porta_b.b7

/* --- PORTB Bits ---------------------------------------------------------- */
static volatile port_t at portb_addr portb_b;
#define PB0 portb_b.b0
#define PB1 portb_b.b1
#define PB2 portb_b.b2
#define PB3 portb_b.b3
#define PB4 portb_b.b4
#define PB5 portb_b.b5
#define PB6 portb_b.b6
#define PB7 portb_b.b7

/* --- PORTC Bits ---------------------------------------------------------- */
static volatile port_t at portc_addr portc_b;
#define PC0 portc_b.b0
#define PC1 portc_b.b1
#define PC2 portc_b.b2
#define PC3 portc_b.b3
#define PC4 portc_b.b4
#define PC5 portc_b.b5
#define PC6 portc_b.b6
#define PC7 portc_b.b7

/* --- PORTD Bits ---------------------------------------------------------- */
static volatile port_t at portd_addr portd_b;
#define PD0 portd_b.b0
#define PD1 portd_b.b1
#define PD2 portd_b.b2
#define PD3 portd_b.b3
#define PD4 portd_b.b4
#define PD5 portd_b.b5
#define PD6 portd_b.b6
#define PD7 portd_b.b7

/* --- PORTE Bits ---------------------------------------------------------- */
static volatile port_t at porte_addr porte_b;
#define RE0 porte_b.b0
#define RE1 porte_b.b1
#define RE2 porte_b.b2

/* --- TRISA Bits ---------------------------------------------------------- */
static volatile tris_t at trisa_addr trisa_b;
#define TRISA0 trisa_b.b0
#define TRISA1 trisa_b.b1
#define TRISA2 trisa_b.b2
#define TRISA3 trisa_b.b3
#define TRISA4 trisa_b.b4
#define TRISA5 trisa_b.b5
#define TRISA6 trisa_b.b6
#define TRISA7 trisa_b.b7

/* --- TRISB Bits ---------------------------------------------------------- */
static  tris_t at trisb_addr trisb_b;
#define TRISB0 trisb_b.b0
#define TRISB1 trisb_b.b1
#define TRISB2 trisb_b.b2
#define TRISB3 trisb_b.b3
#define TRISB4 trisb_b.b4
#define TRISB5 trisb_b.b5
#define TRISB6 trisb_b.b6
#define TRISB7 trisb_b.b7

/* --- TRISC Bits ---------------------------------------------------------- */
static volatile tris_t at trisc_addr trisc_b;
#define TRISC0 trisc_b.b0
#define TRISC1 trisc_b.b1
#define TRISC2 trisc_b.b2
#define TRISC3 trisc_b.b3
#define TRISC4 trisc_b.b4
#define TRISC5 trisc_b.b5
#define TRISC6 trisc_b.b6
#define TRISC7 trisc_b.b7

/* --- TRISD Bits ---------------------------------------------------------- */
static volatile tris_t at trisd_addr trisd_b;
#define TRISD0 trisd_b.b0
#define TRISD1 trisd_b.b1
#define TRISD2 trisd_b.b2
#define TRISD3 trisd_b.b3
#define TRISD4 trisd_b.b4
#define TRISD5 trisd_b.b5
#define TRISD6 trisd_b.b6
#define TRISD7 trisd_b.b7

/* --- TRISE Bits ---------------------------------------------------------- */
typedef union {
	struct {
		unsigned bit0    :1; /* Data direction bits */
		unsigned bit1    :1;
		unsigned bit2    :1;
		unsigned         :1;
		unsigned pspmode :1; /* Parrallel slave port mode select bit */
		unsigned ibov    :1; /* Input buffer overflow detect bit */
		unsigned obf     :1; /* Output buffer full status bit */
		unsigned ibf     :1; /* Input buffer full status bit */
	};
	struct {
		unsigned trise:3;
	};
	bits_t;
	unsigned char r;
} trise_t;

static volatile trise_t at trise_addr  trise_b;
#define TRISE0  trise_b.bit0
#define TRISE1  trise_b.bit1
#define TRISE2  trise_b.bit2
#define PSPMODE trise_b.pspmode
#define IBOV    trise_b.ibov
#define OBF     trise_b.obf
#define IBF     trise_b.ibf

/* ---- Port Registers ----------------------------------------------------- */
sfr at porta_addr porta;
sfr at portb_addr portb;
sfr at portc_addr portc;
sfr at portd_addr portd;
sfr at porte_addr porte;
sfr at trisa_addr trisa;
sfr at trisb_addr trisb;
sfr at trisc_addr trisc;
sfr at trisd_addr trisd;
sfr at trise_addr trise;
#define PORTA porta
#define PORTB portb
#define PORTC portc
#define PORTD portd
#define PORTE porte
#define TRISA trisa
#define TRISB trisb
#define TRISC trisc
#define TRISD trisd
#define TRISE trise


/*****************************************************************************
	Interrupts
 *****************************************************************************/

/* ---- INTCON Bits -------------------------------------------------------- */
typedef union {
	struct {
		unsigned rbif   :1; /* RB port change interrupt flag bit */
		unsigned ntf    :1; /* RB0/INT external interrupt flag bit */
		unsigned tmr0if :1; /* TMR0 overflow interrupt flag bit */
		unsigned rbie   :1; /* RB port change interrupt enable bit */
		unsigned inte   :1; /* RB0/INT external interrupt enable */
		unsigned tmr0ie :1; /* TMR0 over flow interrupt enble bit */
		unsigned peie   :1; /* Peripheral interrupt enable bit */
		unsigned gie    :1; /* Global interrupt enable bit */
	};
	unsigned char r;
} intcon_t;

static volatile intcon_t at intcon_addr intcon_b;
#define RBIF intcon_b.rbif
#define NTF  intcon_b.ntf
#define T0IF intcon_b.tmr0if
#define RBIE intcon_b.rbie
#define INTE intcon_b.inte
#define T0IE intcon_b.tmr0ie
#define PEIE intcon_b.peie
#define GIE  intcon_b.gie

/* ---- PIR1 Bits ---------------------------------------------------------- */
typedef union {
	struct {
		unsigned tmr1if :1; /* Tmr1 overflow interrupt flag bit */
		unsigned tmr2if :1; /* TMR2 to PR2 match interrupt flag bit */
		unsigned ccp1if :1; /* CCP1 interrupt flag bit */
		unsigned sspif  :1; /* Synchronous serial port interrupt flag bit */
		unsigned txif   :1; /* USART transmit interrupt flag bit */
		unsigned rcif   :1; /* USART receiver interrupt flag bit */
		unsigned adif   :1; /* A/D converter interrupt flag bit */
		unsigned pspif  :1; /* Parrallel slave port read/write interrupt flag bit */
	};
	unsigned char r;
} pir1_t;

static volatile pir1_t at pir1_addr pir1_b;
#define TMR1IF pir1_b.tmr1if
#define TMR2IF pir1_b.tmr2if
#define CCP1IF pir1_b.ccp1if
#define SSPIF  pir1_b.sspif
#define TXIF   pir1_b.txif
#define RCIF   pir1_b.rcif
#define ADIF   pir1_b.adif
#define PSPIF  pir1_b.pspif

/* ---- PIR2 Bits ---------------------------------------------------------- */
typedef union {
	struct {
		unsigned ccp2if :1; /* CCP2 interrupt enable bit */
		unsigned        :1;
		unsigned        :1;
		unsigned bclif  :1; /* Bus collision interrupt enable bit */
		unsigned eeif   :1; /* EEPROM write operation interrupt enable bit */
		unsigned        :1;
		unsigned cmif   :1; /* Comparitor interrupt flag bit */
	};
	unsigned char r;
} pir2_t;

static volatile pir2_t at pir2_addr pir2_b;
#define CCP2IF pir2_b.ccp2if
#define BCLIF  pir2_b.bclif
#define EEIF   pir2_b.eeif

/* ---- PIE1 Bits ---------------------------------------------------------- */
typedef union {
	struct {
		unsigned tmr1ie :1; /* Tmr1 overflow interrupt enable bit */
		unsigned tmr2ie :1; /* TMR2 to PR2 match interrupt enable bit */
		unsigned ccp1ie :1; /* CCP1 interrupt enable bit */
		unsigned sspie  :1; /* Synchronous serial port interrupt enable bit */
		unsigned txie   :1; /* USART transmit interrupt enable bit */
		unsigned rcie   :1; /* USART receiver interrupt enable bit */
		unsigned adie   :1; /* A/D converter interrupt enable bit */
		unsigned pspie  :1; /* Parrallel slave port read/write interrupt enable bit */
	};
	unsigned char r;
} pie1_t;

static volatile pie1_t at pie1_addr pie1_b;
#define TMR1IE pie1_b.tmr1ie
#define TMR2IE pie1_b.tmr2ie
#define CCP1IE pie1_b.ccp1ie
#define SSPIE  pie1_b.sspie
#define TXIE   pie1_b.txie
#define RCIE   pie1_b.rcie
#define ADIE   pie1_b.adie
#define PSPIE  pie1_b.pspie

/* ---- PIE2 Bits ---------------------------------------------------------- */
typedef union {
	struct {
		unsigned ccp2ie :1; /* CCP2 interrupt enable bit */
		unsigned        :1;
		unsigned        :1;
		unsigned bclie  :1; /* Bus collision interrupt enable bit */
		unsigned eeie   :1; /* EEPROM write operation interrupt enable bit */
		unsigned        :1;
		unsigned cmie   :1; /* Comparitor interrupt enable bit */
	};
	unsigned char r;
} pie2_t;

static volatile pie2_t at pie2_addr pie2_b;
#define CCP2IE pie2_b.ccp2ie
#define BCLIE  pie2_b.bclie
#define EEIE   pie2_b.eeie
#define CMIE   pie2_b.cmie

/* ---- Interrupt Registers ------------------------------------------------ */
sfr at intcon_addr intcon;
sfr at pir1_addr   pir1;
sfr at pir2_addr   pir2;
sfr at pie1_addr   pie1;
sfr at pie2_addr   pie2;

#define INTCON intcon
#define PIR1   pir1
#define PIR2   pir2
#define PIE1   pie1
#define PIE2   pie2

/*****************************************************************************
	Timers
 *****************************************************************************/

/* ---- T1CON Bits --------------------------------------------------------- */
typedef union {
	struct {
		unsigned tmr1on  :1; /* Timer1 on bit */
		unsigned tmr1cs  :1; /* Timer1 clock source select bit */
		unsigned t1sync  :1; /* Timer1 external clock input synchronization control bit */
		unsigned t1oscen :1; /* Timer1 oscilator enable control bit */
		unsigned t1ckps0 :1; /* Timer1 input clock prescale select bits */
		unsigned t1ckps1 :1;
	};
	struct {
		unsigned        :1;
		unsigned        :1;
		unsigned        :1;
		unsigned        :1;
		unsigned t1ckps :2; /* Timer1 input clock prescale select bits */
	};
	bits_t;
	unsigned char r;
} t1con_t;

static volatile t1con_t at t1con_addr t1con_b;
#define TMR1ON     t1con_b.tmr1on
#define TMR1CS     t1con_b.tmr1cs
#define T1SYNC     t1con_b.t1sync
#define NOT_T1SYNC t1con_b.t1sync
#define T1OSCEN    t1con_b.t1oscen
#define T1CKPS0    t1con_b.t1ckps0
#define T1CKPS1    t1con_b.t1ckps1

/* ---- T2CON Bits --------------------------------------------------------- */
typedef union {
	struct {
		unsigned t2ckps0 :1; /* Timer2 clock prescale select bits */
		unsigned t2ckps1 :1;
		unsigned tmr2on  :1; /* Timer2 on bit */
		unsigned toutps0 :1;
		unsigned toutps1 :1; /* Timer2 output postscale selet bits */
		unsigned toutps2 :1;
		unsigned toutps3 :1;
	};
	struct {
		unsigned t2ckps :2; /* Timer2 clock prescale select bits */
		unsigned        :1;
		unsigned toutps :4; /* Timer2 output postscale selet bits */
	};
	unsigned char r;
} t2con_t;

static volatile t2con_t at t2con_addr t2con_b;
#define T2CKPS0 t2con_b.t2ckps0
#define T2CKPS1 t2con_b.t2ckps1
#define TMR2ON  t2con_b.tmr2on
#define TOUTPS0 t2con_b.toutps0
#define TOUTPS1 t2con_b.toutps1
#define TOUTPS2 t2con_b.toutps2
#define TOUTPS3 t2con_b.toutps3

/* ---- Timer Registers ---------------------------------------------------- */
sfr at tmr0_addr   tmr0;
sfr at tmr1l_addr  tmr1l;
sfr at tmr1h_addr  tmr1h;
sfr at tmr2_addr   tmr2;
sfr at t1con_addr  t1con;
sfr at t2con_addr  t2con;

#define TMR0  tmr0
#define TMR1L tmr1l
#define TMR1H tmr1h
#define TMR2  tmr2
#define T1CON t1con
#define T2CON t2con


/*****************************************************************************
	Comparitors
 *****************************************************************************/

/* ----- CCPCON Bits -------------------------------------------------------- */
typedef union {
	struct {
		unsigned ccpm0 :1; /* CCPx mode select bits */
		unsigned ccpm1 :1;
		unsigned ccpm2 :1;
		unsigned ccpm3 :1;
		unsigned ccpy  :1; /* PWM least significant bits */
		unsigned ccpx  :1;
	};
	struct {
		unsigned ccpm :4; /* CCPx mode select bits */
	};
	unsigned char r;
} ccpcon_t;

static volatile ccpcon_t at ccp1con_addr ccp1con_b;
#define CCP1M0 ccp1con_b.ccpm0
#define CCP1M1 ccp1con_b.ccpm1
#define CCP1M2 ccp1con_b.ccpm2
#define CCP1M3 ccp1con_b.ccpm3
#define CCP1Y  ccp1con_b.ccpy
#define CCP1X  ccp1con_b.ccpx

static volatile ccpcon_t at ccp2con_addr ccp2con_b;
#define CCP2M0 ccp2con_b.ccpm0
#define CCP2M1 ccp2con_b.ccpm1
#define CCP2M2 ccp2con_b.ccpm2
#define CCP2M3 ccp2con_b.ccpm3
#define CCP2Y  ccp2con_b.ccpy
#define CCP2X  ccp2con_b.ccpx

/* ---- Timer Registers ---------------------------------------------------- */
sfr at ccpr1l_addr   CCPR1L;
sfr at ccpr1h_addr   CCPR1H;
sfr at ccp1con_addr  CCP1CON;
sfr at ccpr2l_addr   CCPR2L;
sfr at ccpr2h_addr   CCPR2H;
sfr at ccp2con_addr  CCP2CON;


/*****************************************************************************
	SSP
 *****************************************************************************/

/* ---- SSPCON Bits -------------------------------------------------------- */
typedef union {
	struct {
		unsigned sspm0 :1; /* Synchronous serial port mode select bits */
		unsigned sspm1 :1;
		unsigned sspm2 :1;
		unsigned sspm3 :1;
		unsigned ckp   :1; /* Clock polarity select bits */
		unsigned sspen :1; /* Synchronous serial port enable bits */
		unsigned sspov :1; /* Receive overflow indicator bit */
		unsigned wcol  :1; /* Write collision dedect bit */
	};
	struct {
		unsigned sspm :4; /* Synchronous serial port mode select bits */
	};
	unsigned char r;
} sspcon_t;

static volatile sspcon_t at sspcon_addr sspcon_b;
#define SSPM0 sspcon_b.sspm0
#define SSPM1 sspcon_b.sspm1
#define SSPM2 sspcon_b.sspm2
#define SSPM3 sspcon_b.sspm3
#define CKP   sspcon_b.ckp
#define SSPEN sspcon_b.sspen
#define SSPOV sspcon_b.sspov
#define WCOL  sspcon_b.wcol

/* ---- SSPCON2 Bits -------------------------------------------------------- */
typedef union {
	struct {
		unsigned sen     :1; /* START condition enabled/stretch enabled bit */
		unsigned rsen    :1; /* Repeated START condition enabled bit */
		unsigned pen     :1; /* STOP condition enable bit */
		unsigned rcen    :1; /* Receive enable bit */
		unsigned acken   :1; /* Acknowledge sequence enable bit */
		unsigned ackdt   :1; /* Acknowledge data bit (master receive mode only) */
		unsigned ackstat :1; /* Acknowledge status bit (master transmit mode only) */
		unsigned gcen    :1; /* General call enable bit */
	};
	unsigned char r;
} sspcon2_t;

static volatile sspcon2_t at sspcon2_addr sspcon2_b;
#define SEN     sspcon2_b.sen
#define RSEN    sspcon2_b.rsen
#define PEN     sspcon2_b.pen
#define RCEN    sspcon2_b.rcen
#define ACKEN   sspcon2_b.acken
#define ACKDT   sspcon2_b.ackdt
#define ACKSTAT sspcon2_b.ackstat
#define GCEN    sspcon2_b.gcen

/* ---- SSPSTAT Bits ------------------------------------------------------- */
typedef union {
	struct {
		unsigned bf  :1; /* Buffer full status bit (Receive mode only) */
		unsigned ua  :1; /* Update address bit used in I2C mode only */
		unsigned r_w :1; /* Read write bit information used in I2C mode only */
		unsigned s   :1; /* Start bit used in I2C mode only */
		unsigned p   :1; /* Stop bit used in I2C mode only */
		unsigned d_a :1; /* Data address bit used in I2C mode only */
		unsigned cke :1; /* SPI clock select edge bit */
		unsigned smp :1; /* Sample bit */
	};
	unsigned char r;
} sspstat_t;

static volatile sspstat_t at sspstat_addr sspstat_b;
#define BF           sspstat_b.bf
#define UA           sspstat_b.ua
#define R_W          sspstat_b.r_w
#define R            sspstat_b.r_w
#define I2C_READ     sspstat_b.r_w
#define NOT_W        sspstat_b.r_w
#define NOT_WRITE    sspstat_b.r_w
#define READ_WRITE   sspstat_b.r_w
#define S            sspstat_b.s
#define I2C_START    sspstat_b.s
#define P            sspstat_b.p
#define I2C_STOP     sspstat_b.p
#define D_A          sspstat_b.d_a
#define D            sspstat_b.d_a
#define I2C_DATA     sspstat_b.d_a
#define NOT_A        sspstat_b.d_a
#define NOT_ADDRESS  sspstat_b.d_a
#define DATA_ADDRESS sspstat_b.d_a
#define CKE          sspstat_b.cke
#define SMP          sspstat_b.smp

/* ---- SSP Registers ------------------------------------------------------ */
sfr at sspbuf_addr  sspbuf;
sfr at sspcon_addr  sspcon;
sfr at sspcon2_addr sspcon2;
sfr at sspadd_addr  sspadd;
sfr at sspstat_addr sspstat;
sfr at sspstat_addr sspstat;
sfr at sspcon2_addr sspcon2;
sfr at sspcon_addr  sspcon;

#define SSPBUF  sspbuf
#define SSPCON  sspcon
#define SSPCON2 sspcon2
#define SSPADD  sspadd
#define SSPSTAT sspstat

/*****************************************************************************
	USART
 *****************************************************************************/

/* ---- RCSTA Bits --------------------------------------------------------- */
typedef union {
	struct {
		unsigned rx9d  :1; /* 9the bit of received data (can be parity bit but must be calculated by user firmware */
		unsigned oerr  :1; /* Overrun error bit */
		unsigned ferr  :1; /* Framing error bit */
		unsigned adden :1; /* Address detect enable bit */
		unsigned cren  :1; /* Continuous receive enable bit */
		unsigned sren  :1; /* Single receive enable bit */
		unsigned rx9   :1; /* 9th bit receive enable bit */
		unsigned spen  :1; /* Serial port enbale bit */
	};
	unsigned char r;
} rcsta_t;

static volatile rcsta_t at rcsta_addr rcsta_b;
#define SPEN  rcsta_b.spen
#define RX9   rcsta_b.rx9
#define SREN  rcsta_b.sren
#define CREN  rcsta_b.cren
#define ADDEN rcsta_b.adden
#define FERR  rcsta_b.ferr
#define OERR  rcsta_b.oerr
#define RX9D  rcsta_b.rx9d

/* ---- TXSTA Bits --------------------------------------------------------- */
typedef union {
	struct {
		unsigned tx9d :1; /* 9th bit of transmit data, can be parity */
		unsigned trmt :1; /* Transmit shift register status bit */
		unsigned brgh :1; /* High baud rate select bit */
		unsigned      :1;
		unsigned sync :1; /* USART mode select bit */
		unsigned txen :1; /* Transmit enable bit */
		unsigned tx9  :1; /* 9th bit transmit enable */
		unsigned csrc :1; /* Clock source select bit */
	};
	unsigned char r;
} txsta_t;

static volatile txsta_t at txsta_addr txsta_b;
#define CSRC txsta_b.csrc
#define TX9  txsta_b.tx9
#define TXEN txsta_b.txen
#define SYNC txsta_b.sync
#define BRGH txsta_b.brgh
#define TRMT txsta_b.trmt
#define TX9D txsta_b.tx9d

/* ---- USART Registers ---------------------------------------------------- */
sfr at spbrg_addr spbrg;
sfr at rcreg_addr rcreg;
sfr at txreg_addr txreg;
sfr at rcsta_addr rcsta;
sfr at txsta_addr txsta;
#define SPBRG spbrg
#define RCREG rcreg
#define TXREG txreg
#define RCSTA rcsta
#define TXSTA txsta


/*****************************************************************************
	ADC
 *****************************************************************************/

/* ---- ADCON0 Bits -------------------------------------------------------- */
typedef union {
	struct {
		unsigned adon    :1; /* A/D on bit */
		unsigned         :1;
		unsigned go_done :1; /* A/D conversion start bit and not done status bit */
		unsigned chs0    :1; /* Analog chanel select bits */
		unsigned chs1    :1;
		unsigned chs2    :1;
		unsigned adcs0   :1; /* A/D conversion clock select bits */
		unsigned adcs1   :1;
	};
	struct {
		unsigned      :1;
		unsigned      :1;
		unsigned chs  :3; /* Analog chanel select bits */
		unsigned adcs :2; /* A/D conversion clock select bits */
	};
	unsigned char r;
} adcon0_t;

static volatile adcon0_t at adcon0_addr adcon0_b;
#define GO       adcon0_b.go_done
#define NOT_DONE adcon0_b.go_done
#define GO_DONE  adcon0_b.go_done
#define ADON     adcon0_b.adon
#define CHS0     adcon0_b.chs0
#define CHS1     adcon0_b.chs1
#define CHS2     adcon0_b.chs2
#define ADCS0    adcon0_b.adcs0
#define ADCS1    adcon0_b.adcs1

/* ---- ADCON1 Bits -------------------------------------------------------- */
typedef union {
	struct {
		unsigned pcfg0 :1; /* A/D port configuration control bits */
		unsigned pcfg1 :1;
		unsigned pcfg2 :1;
		unsigned pcfg3 :1;
		unsigned       :2;
		unsigned adcs2 :1; /* A/D convertion clock select bit */
		unsigned adfm  :1; /* A/D result format select bit */
	};
	struct {
		unsigned pcfg :4; /* A/D port configuration control bits */
	};
	unsigned char r;
} adcon1_t;

static volatile adcon1_t at adcon1_addr adcon1_b;
#define PCFG0 adcon1_b.pcfg0
#define PCFG1 adcon1_b.pcfg1
#define PCFG2 adcon1_b.pcfg2
#define PCFG3 adcon1_b.pcfg3
#define ADFM  adcon1_b.adfm

/* ---- ADC Registers   ---------------------------------------------------- */
sfr at adresh_addr adresh;
sfr at adresh_addr adresl;
sfr at adcon0_addr adcon0;
sfr at adcon1_addr adcon1;
#define ADRESH adresh
#define ADRESL adresl
#define ADCON0 adcon0
#define ADCON1 adcon1


/*****************************************************************************
	EEPROM
 *****************************************************************************/

/* ---- EECON1 Bits -------------------------------------------------------- */
typedef union {
	struct {
		unsigned rd    :1; /* Read control bit */
		unsigned wr    :1; /* Write control bit */
		unsigned wren  :1; /* EEPROM write enable bit */
		unsigned wrerr :1; /* EEPROM error flag bit */
		unsigned       :3;
		unsigned eepgd :1; /* Program data EEPROM select bit */
	};
	unsigned char r;
} eecon1_t;

static volatile eecon1_t at eecon1_addr eecon1_b;
#define RD    eecon1_b.rd
#define WR    eecon1_b.wr
#define WREN  eecon1_b.wren
#define WRERR eecon1_b.wrerr
#define EEPGD eecon1_b.eepgd

/* ---- EEPROM Registers   ------------------------------------------------- */
sfr at eedata_addr eedata;
sfr at eeadr_addr  eeadr;
sfr at eedath_addr eedath;
sfr at eeadrh_addr eeadrh;
sfr at eecon1_addr eecon1;
sfr at eecon2_addr eecon2;

#define EEDATA eedata
#define EEADR  eeadr
#define EEDATH eedath
#define EEADRH eeadrh
#define EECON1 eecon1
#define EECON2 eecon2


/*****************************************************************************
	Config Bits
 *****************************************************************************/
enum {
	cp_all         = 0x0fcf,
	cp_half        = 0x1fdf,
	cp_upper_256   = 0x2fef,
	cp_off         = 0x3fff,
	debug_on       = 0x37ff,
	debug_off      = 0x3fff,
	wrt_enable_on  = 0x3fff,
	wrt_enable_off = 0x3dff,
	cpd_on         = 0x3eff,
	cpd_off        = 0x3fff,
	lvp_on         = 0x3fff,
	lvp_off        = 0x3f7f,
	boden_on       = 0x3fff,
	boden_off      = 0x3fbf,
	pwrte_off      = 0x3fff,
	pwrte_on       = 0x3ff7,
	wdt_on         = 0x3fff,
	wdt_off        = 0x3ffb,
	lp_osc         = 0x3ffc,
	xt_osc         = 0x3ffd,
	hs_osc         = 0x3ffe,
	rc_osc         = 0x3fff
};

#endif /* PIC16F877_H */
