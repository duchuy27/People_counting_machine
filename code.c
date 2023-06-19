#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

#pragma config FOSC = HS     // Oscillator Selection bits (EC oscillator, CLKO function on RA6 (EC))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

#pragma config CCP2MX = OFF      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

#pragma config EBTRB = OFF

#include <xc.h>
#include<pic18f4550.h>
#define _XTAL_FREQ 20000000

void URAT(){
    TRISC6=0;
    TRISC7=1;
    TXSTA=0x20;
    RCSTA=0x90;
    BAUDCON=0x00; 
}

void ngoai_ngat(){
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    INTCONbits.INT0IE=1; 
    INTCONbits.INT0IF=0;
    INT1IE=1; 
    INT1IF=0;
    INTCON2bits.INTEDG0=0;
    INTCON2bits.INTEDG1=0;
    URAT();
}

void URAT_SPBRG(unsigned int brd){                 // ham toc do
    SPBRG=_XTAL_FREQ/64/brd-1;
}

void URAT_SEND_1byte(char kt){                     // gui 1 ki tu
    while(TXIF==0);
    TXREG=kt;
}

void URAT_Send_String(char *str, int leng){                  // gui nhieu ki tu
    for (int i=0;i<leng;i++){
        URAT_SEND_1byte(str[i]);
    }
}

unsigned int vao,ra,t=0;int x=0;
char vao1[2];
char ra1[2];

void main(void) {
    ngoai_ngat();
    URAT_SPBRG(9600);
    TRISDbits.RD1=0;
    TRISB=0x03;
    while(1)
    {
		if(vao-ra!=0) LATDbits.LD1=1;
		else LATDbits.LD1=0;
		URAT_Send_String("so nguoi vao phong:",19); 
		URAT_Send_String(vao1,2);
		URAT_Send_String("\r\n",2);
		URAT_Send_String("so nguoi ra phong:",21); 
		URAT_Send_String(ra1,2);
		URAT_Send_String("\r\n",2);
		__delay_ms(1000);
    }
    return;
}

void __interrupt isr(){
    if(PORTBbits.RB0==0){
        x=x*10+1;
        INT0IF=0;
    }
    if(PORTBbits.RB1==0){
        x=x*10+2;
        INT1IF=0;
    }
    if((x==12)==1)
    {
        vao++;
        x=0;
		vao1[0]=vao/10+48;
		vao1[1]=vao%10+48;
		ra1[0]=ra/10+48;
		ra1[1]=ra%10+48;
    }
    if((x==21)==1)
    {
        ra++;
        x=0;
		vao1[0]=vao/10+48;
		vao1[1]=vao%10+48;
		ra1[0]=ra/10+48;
		ra1[1]=ra%10+48;
    }
}
