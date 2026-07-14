/*
 * mssa.c
 * Description:
 * This is start/stop system memory, for the PIC16F684 or PIC16F676 or PIC12F683 or PIC12F675 chips.
 * Author: LZ2HV, Christo
 * Creation date: 2025 for VW T-Cross 2025
 */
//#define DEBUG_WRITE
//#define DEBUG_EXCEP
#define DEBUG_MAINN

#define PIC16F684X
//#define PIC16F676X
//#define PIC12F683X
//#define PIC12F675X

#if defined PIC16F684X
#include <16f684.h>
#endif
#if defined PIC16F676X
#include <16f676.h>
#endif
#if defined PIC12F683X
#include <12f683.h>
#endif
#if defined PIC12F675X
#include <12f675.h>
#endif

/* Microcontroller configuration bits */
//#byte OSCCON=0x8F
//#use delay(internal=4MHz)
#fuses INTRC_IO,NOPROTECT,NOWDT,BROWNOUT,PUT,NOMCLR,NOCPD
#use delay(clock=4000000)
//#use fixed_io(a_outputs=PIN_A3)
//INTRC_IO, NOWDT, NOPROTECT, BROWNOUT, NOMCLR, NOCPD
//#BYTE OSCCON = 0xFd3
//#use delay(clock=4000000)
//#byte OSCTUNE = 0x90
//#bit PLLEN=OSCTUNE.6 
 
#if (defined(PIC16F684X)||defined(PIC16F676X))
#define BUT_SEC         (PIN_A4)
#define SSA_LED         (PIN_A1)
#define SWI_OUT         (PIN_C0)
#define DEB_WRI         (PIN_C4)
#define DEB_EXE         (PIN_C3)
#define DEB_EXC_SAV_MIN (PIN_C2)
#define A05_MMM         (PIN_A5)
#endif

#if (defined(PIC12F683X)||defined(PIC12F675X))
#define BUT_SEC         (PIN_A2)
#define SSA_LED         (PIN_A0)
#define SWI_OUT         (PIN_A1)
//#define DEB_EXE       (PIN_A3)
#define DEB_WRI         (PIN_A4)//error PIN_A4=PIN_A3 ???
#define DEB_EXC_SAV_MIN (PIN_A5)//error PIN -2=PIN_A5 ???
#endif

#define COU_SAVE_SECL 15  //16-1=4s
#define COU_SAVE_SECH 2   //3=750ms
/*#define MAX_SSEC 24 //24=6s
#define MIN_SSEC 4  //4=1s
#define DEF_SSEC 16 //16=4s*/

#define MAX_SEC 40 //40=40*200=8s
#define MIN_SEC 10 //10=10*200=2s
#define DEF_SEC 30 //30=30*200=6s

/*#define MAX_ESEC 16 //160*17=2720ms = 4-flashing
//     14           //160*15=2400ms = 3-flashing
//     12           //160*13=2080ms = 2-flashing
#define MIN_ESEC 10 //160*11=1760ms = 1-flashing*/
#define DEF_ESEC 12 

#define TRY_002 14 //14=3500ms
#define TRY_003 24 //24=6000ms
#define TRY_END 34 //34=8500ms

BOOLEAN is_on_ssa_led(void)
{
    BOOLEAN res = false;
    int c_bssa = 0;
    int to = 0;
    for (int i=0; i<20; ++i)//200ms
    {
        delay_ms(10);    
        if (!input(SSA_LED)) c_bssa++;
        else c_bssa = 0;
        if (c_bssa > 16)//160ms
        {
            res = true;
            break;
        }
    }
    if (!res && c_bssa > 0)
    {
        res = true;
        to = 17 - c_bssa;
        for (int i=0; i<to; ++i)
        {
            delay_ms(10);        
            if (input(SSA_LED))
            {
               res = false;
               break;
            }
        }
    }
    return res;
}
int read_sec = 0;
BOOLEAN wait1234sec(void)
{
    BOOLEAN res = true;
    int to = 0;
    int count_except = 0;
    for (int i=0; i<read_sec; ++i)
    {
        if (is_on_ssa_led())//full=200ms
        {
            count_except++;
            if (count_except > DEF_ESEC)//160*13=2080ms
            {
                res = false;
                break;
            }
        }
        else count_except = 0;
        if (count_except > 0) output_high(DEB_EXC_SAV_MIN);
        else output_low(DEB_EXC_SAV_MIN);
    }
    if (res && count_except > 0)
    {
        res = false;
        to = (DEF_ESEC+1)-count_except;
        for (int i=0; i<to; ++i)
        {
            if (!is_on_ssa_led()) 
            {
               res = true;
               break;
            }
        }
    }
    output_low(DEB_EXC_SAV_MIN);
    return res;
}
void try_to_on(void)
{
    output_high(DEB_EXC_SAV_MIN);
    output_high(SWI_OUT);
    delay_ms(200);
    delay_ms(200);
    output_low(SWI_OUT);
    output_low(DEB_EXC_SAV_MIN);
}
void led_flash(int z)
{
    for (int i=0; i<z; ++i)
    {
        output_low(DEB_EXC_SAV_MIN);
        delay_ms(200);
        output_high(DEB_EXC_SAV_MIN);
        delay_ms(200);
    }
    output_low(DEB_EXC_SAV_MIN);
    delay_ms(200);
}
int read_ssa = 0;
int save_ssa = 0;
#if defined DEBUG_MAINN
int f_main = 0;
#endif
BOOLEAN f_b_sec = false;//button is up logic
void refresh_read_button(void)
{
    int tread_ssa = 2;
    if (is_on_ssa_led()) tread_ssa = 3;
    if (tread_ssa != read_ssa)
    {
        save_ssa=1;
        read_ssa = tread_ssa;
        delay_ms(20);
    } 
    delay_ms(10);
    if (!input(BUT_SEC))
    {
        f_b_sec = true;
#if defined DEBUG_MAINN        
        f_main = 1;
#endif        
    }
    else
    {
        if (f_b_sec)
        {
            f_b_sec = false;
            if (read_sec>=MAX_SEC) read_sec = MIN_SEC;
            else read_sec += 5;
            write_eeprom(1,read_sec);
            int cflash = read_sec/5;
            led_flash(cflash);
#if defined DEBUG_MAINN
            f_main = 1;
#endif        
#if defined DEBUG_WRITE
            output_high(DEB_WRI);
            delay_ms(200);
            output_low(DEB_WRI);
#endif
        }
    }
}
void main(void)
{
    /* Initialization */
#if (defined(PIC16F684X)||defined(PIC16F676X))
#if defined(PIC16F684X)
    setup_oscillator(OSC_4MHZ);
#endif
    set_tris_a(0b00010010);// 1 -> vhod     0-> izhod
    set_tris_c(0b00000000);
    output_c(0);
    output_low(A05_MMM);
#endif

#if (defined(PIC12F683X)||defined(PIC12F675X))  // inputs=1 output=0 (bitove-> 0b_76543210_ )
    //setup_adc_ports(NO_ANALOGS);
    //setup_comparator(NC_NC);
    //OSCTUNE = 0x00;
#if defined(PIC12F683X)
    setup_oscillator(OSC_4MHZ);//setup_oscillator(OSC_INTRC | OSC_4MHZ);
#endif  
    //OSCTUNE = 0x00;
    //setup_oscillator(OSC_INTRC|OSC_4MHz,0);
    set_tris_a(0b00000101);
    output_low(SWI_OUT);
    output_low(DEB_WRI);
    //output_low(DEB_EXE);
    output_low(DEB_EXC_SAV_MIN);
#endif
    /*write_eeprom(0,0xff);//reset
    write_eeprom(1,0xff);
    return;*/
    delay_ms(200);
    delay_ms(200);
    read_ssa = read_eeprom(0);
    delay_ms(20);
    read_sec = read_eeprom(1);
    delay_ms(20);
    if (read_sec<MIN_SEC || read_sec>MAX_SEC)
    {
        write_eeprom(1,DEF_SEC);//20=1s 40=2s 60=3s 80=4s 100=5s 120=6s
        delay_ms(50);
        read_sec = DEF_SEC;
    }      
    int c_try_on = 0;
    int c_ss = 0;
    if (read_ssa == 3)
    {
        delay_ms(100);
        if (wait1234sec())
        {
            try_to_on();
            c_try_on=1;
        } 
    }
    if (read_ssa != 2 && read_ssa != 3)//first time 0xFF
    {
        write_eeprom(0,2);
        delay_ms(50);
        read_ssa = 2;
#if defined DEBUG_WRITE
        output_high(DEB_WRI);
        delay_ms(100);
        output_low(DEB_WRI);
#endif
    }
    //BOOLEAN h=false;//test cycle=250ms every time
    while (TRUE)
    {
        /*if (h)
        {
           h=false; output_low(DEB_EXE);
        }
        else
        {
           h=true; output_high(DEB_EXE);
        }*/
        delay_ms(50);//50+200=250ms
        if (c_try_on==0) refresh_read_button();//160 or 200
        c_ss = COU_SAVE_SECL;
        if (read_ssa==3)
        {
            c_ss = COU_SAVE_SECH;
            if (c_try_on==0) delay_ms(40);//50+160+40=250ms
        }
        if (save_ssa > c_ss)
        {
            save_ssa=0;
            delay_ms(100);//old=100
            write_eeprom(0,read_ssa);
            delay_ms(50);
#if defined DEBUG_WRITE
            output_high(DEB_WRI);
            delay_ms(200);
            output_low(DEB_WRI);
#endif
        }
        if (save_ssa > 0) save_ssa++;
#if defined DEBUG_MAINN
        if (save_ssa > 0 || c_try_on > 0) f_main = 1;
        if (f_main < 1 || save_ssa > 0 || c_try_on > TRY_END) output_high(DEB_EXC_SAV_MIN);
        else output_low(DEB_EXC_SAV_MIN);
        if (f_main > 9) f_main = 0;
        else f_main++;
#else
        if (save_ssa > 0 || c_try_on > TRY_END) output_high(DEB_EXC_SAV_MIN);
        else output_low(DEB_EXC_SAV_MIN);
#endif
        if (c_try_on > 0)
        {
            if (is_on_ssa_led())//first check 200ms or 160ms
            {
                if (is_on_ssa_led())//second check 160+160=320ms
                {
                    if (is_on_ssa_led())//third check 160+160+160ms=480ms
                    {
                        c_try_on=0;
#if defined DEBUG_MAINN
                        f_main = 0;
                        output_low(DEB_EXC_SAV_MIN);
#endif                
                    }
                }
            }
            if (c_try_on > 0 && c_try_on < (TRY_END+1))
            {
                if (c_try_on==TRY_002) try_to_on();
                if (c_try_on==TRY_003) try_to_on();
                c_try_on++;
            }
        }
    }
}
