/*
 * mssa.c
 * Description:
 * This is start/stop system memory, for the PIC16F684 and PIC12F675 chips.
 * Author: LZ2HV, Christo
 * Creation date: 2025 for VW T-Cross 2025
 */
//#define DEBUG_WRITE
//#define DEBUG_EXCEP
#define DEBUG_MAINN

#define PIC16F684X
//#define PIC12F683X  // ok as PIC12F675
//#define PIC12F675X
//#define PIC12F1571X //write_program_eeprom  read_program_eeprom

#if defined PIC16F684X
#include <16f684.h>
#endif
#if defined PIC12F683X
#include <12f683.h> //ok
//#include <12f675.h>
//#include <12f1571.h> //write_program_eeprom
#endif
//#device *=16 /* Allow RAM to expand beyond 256 bytes */
//#device ADC=10  // Use 10 bits 10bit (5volt;correspond;1024) or 8bit(5volt;correspond;256)

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
 

#if defined PIC16F684X
#define BUT_SEC         (PIN_A4)
#define SSA_BUT         (PIN_A1)
#define SWI_OUT         (PIN_C0)
#define DEB_WRI         (PIN_C4)
#define DEB_EXE         (PIN_C3)
#define DEB_EXC_SAV_MIN (PIN_C2)
#define A05_MMM         (PIN_A5)
#endif

//#define COU_SAVE_SECL 16  //17*150+17*100=2550+1700=4250s
#define COU_SAVE_SECH 2   //3=750 4*170+4*80=680+320=1000  8*158+8*92=1264+736=2000s
#define MAX_SSEC 24 //24=6s
#define MIN_SSEC 4  //4=1s
#define DEF_SSEC 16 //16=4s

#define MAX_SEC 40 //40=40*200=8s
#define MIN_SEC 10 //10=10*200=2s
#define DEF_SEC 30 //30=30*200=6s

#define MAX_ESEC 14 //120*15=1800ms = 3-flashing
// MID=12           //120*13=1560ms = 2-flashing
#define MIN_ESEC 10 //120*11=1320ms = 1-flashing
#define DEF_ESEC 10

#define TRY_002 9           //(9*250=2250ms)=9
#define TRY_003 (TRY_002+5) //9+(5*250ms=1250ms)=14
#define TRY_END (TRY_003+5) //14+(5*250ms=1250ms)=19

#if defined PIC12F683X
#define BUT_SEC         (PIN_A2)
#define SSA_BUT         (PIN_A0)
#define SWI_OUT         (PIN_A1)
//#define DEB_EXE       (PIN_A3)
#define DEB_WRI         (PIN_A4)//error PIN_A4=PIN_A3 ????
#define DEB_EXC_SAV_MIN (PIN_A5)//error PIN -2=PIN_A5 ????
#endif

BOOLEAN is_on_bssa(void)
{
    BOOLEAN res = false;
    int c_bssa = 0;
    for (int i=0; i<20; ++i)//200ms
    {
        if (!input(SSA_BUT)) c_bssa++;
        else c_bssa = 0;
        if (c_bssa > 12)//120ms
        {
            res = true;
            break;
        }
        delay_ms(10);
    }
    return res;
}
int read_sec = 0;
int exce_sec = 0;
BOOLEAN wait1234sec(void)
{
    BOOLEAN res = true; //int try = 0;
    int to = 0;
    int count_except = 0;
    for (int i=0; i<read_sec; ++i)
    {
        if (is_on_bssa())//full=200+0=200ms
        {
            count_except++;
            if (count_except > exce_sec)//except=(0+120=120)*11=1320ms //120*15=1800ms
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
        to = (exce_sec+1)-count_except;
        for (int i=0; i<to; ++i)//try times to 1320ms
        {
            if (!is_on_bssa()) 
            {
               res = true;
               break;
            }
        }
    }
    output_low(DEB_EXC_SAV_MIN);
    return res;
}
void tray_to_on(void)
{
    output_high(DEB_EXC_SAV_MIN);
    output_high(SWI_OUT);
    delay_ms(200);
    delay_ms(200);
    output_low(SWI_OUT);
    output_low(DEB_EXC_SAV_MIN);
}
void led_flash(int z,BOOLEAN l)
{
    for (int i=0; i<z; ++i)
    {
        output_low(DEB_EXC_SAV_MIN);
        delay_ms(200);
        output_high(DEB_EXC_SAV_MIN);
        if (l) delay_ms(800);
        else delay_ms(200);
    }
    output_low(DEB_EXC_SAV_MIN);
    delay_ms(200);
}
int read_ssa = 0;
int save_ssa = 0;
#if defined DEBUG_MAINN
int f_main = 0;
#endif
int set_parm = 0;
int c_parm = 0;
int save_secl = 0;
void refresh_read_button(void)
{
    int tread_ssa = 2;
    if (is_on_bssa()) tread_ssa = 3;
    if (tread_ssa != read_ssa)
    {
        save_ssa=1;
        read_ssa = tread_ssa;
        delay_ms(100);
    }
    delay_ms(10);
    if (save_ssa>0) 
    {
        set_parm=0;
        c_parm=0;
        return;
    }
    if (!input(BUT_SEC))
    {
        if (set_parm==0) set_parm=1;
        c_parm=0;
#if defined DEBUG_MAINN
        output_low(DEB_EXC_SAV_MIN);
        f_main = 1;
#endif
        if (set_parm==2)
        {
            if (read_sec>=MAX_SEC) read_sec = MIN_SEC;
            else read_sec += 5;
            write_eeprom(1,read_sec);
            int cflash = read_sec/5;
            led_flash(cflash,false);
        }
        if (set_parm==3)
        {
            if (save_secl>=MAX_SSEC) save_secl = MIN_SSEC;
            else save_secl += 4;
            write_eeprom(2,save_secl);
            int cflash = save_secl/4;
            led_flash(cflash,false);
        }
        if (set_parm==4)
        {
            if (exce_sec>=MAX_ESEC) exce_sec = MIN_ESEC;
            else exce_sec += 2;
            write_eeprom(3,exce_sec);
            int cflash = ((exce_sec/2)-4);
            led_flash(cflash,false);
        }
    }
    else if (set_parm>0)
    {
        if (set_parm==1)
        {
            set_parm = 2;
            c_parm=0;
            led_flash(1,true);
        }
        else if (set_parm>0) c_parm++;
        if (c_parm>5)
        {
            c_parm=0;
#if defined DEBUG_MAINN
            f_main = 1;
#endif            
            if (set_parm==2)
            {
                led_flash(1,true);
                set_parm=3;
            }
            else if (set_parm==3)
            {
                led_flash(1,true);
                set_parm=4;
            }            
            else if (set_parm==4)
            {
                led_flash(1,true);
                set_parm=0;
#if defined DEBUG_MAINN
                f_main = 0;
#endif                 
            }           
        }
    }
}
void main(void)
{
    /* Initialization */
    //OSCCON=0;
#if defined PIC16F684X
    setup_oscillator(OSC_4MHZ);
    //enable_interrupts(GLOBAL);
    set_tris_a(0b00010010);// 1 -> vhod     0-> izhod
    set_tris_c(0b00000000);
    output_c(0);
    output_low(A05_MMM);
#endif

#if defined PIC12F683X   // inputs=1 output=0 (bitove-> 0b_76543210_ )
    //setup_adc_ports(NO_ANALOGS);
    //setup_comparator(NC_NC);
    //OSCTUNE = 0x00;
    setup_oscillator(OSC_4MHZ);//setup_oscillator(OSC_INTRC | OSC_4MHZ);
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
    save_secl = read_eeprom(2);
    delay_ms(20);
    if (save_secl<MIN_SSEC || save_secl>MAX_SSEC)
    {
        write_eeprom(2,DEF_SSEC);
        delay_ms(50);
        save_secl = DEF_SSEC;
    }
    exce_sec = read_eeprom(3);
    delay_ms(20);
    if (exce_sec<MIN_ESEC || exce_sec>MAX_ESEC)
    {
        write_eeprom(3,DEF_ESEC);
        delay_ms(50);
        exce_sec = DEF_ESEC;
    }    
    int c_tray_on = 0;
    int c_ss = 0;
    if (read_ssa == 3)
    {
        delay_ms(100);//old=100 //BOOLEAN res = wait1234sec();
        if (wait1234sec())
        {
            tray_to_on();
            c_tray_on=1;
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
        if (c_tray_on==0) refresh_read_button();//120 or 200
        c_ss = (save_secl-1);
        if (read_ssa==3)
        {
            c_ss = COU_SAVE_SECH;
            if (c_tray_on==0) delay_ms(80);//50+120+80=250ms
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
        if (save_ssa > 0 || c_tray_on > 0) f_main = 1;
        if (f_main < 1 || save_ssa > 0 || c_tray_on > TRY_END) output_high(DEB_EXC_SAV_MIN);//exception=17->c_tray_on > 16
        else output_low(DEB_EXC_SAV_MIN);
        if (f_main > 9) f_main = 0;
        else f_main++;
#else
        if (save_ssa > 0 || c_tray_on > TRY_END) output_high(DEB_EXC_SAV_MIN);//exception=17->c_tray_on > 16
        else output_low(DEB_EXC_SAV_MIN);
#endif
        if (c_tray_on > 0)
        {
            if (is_on_bssa())
            {
                c_tray_on=0;
#if defined DEBUG_MAINN
                f_main = 0;
                output_low(DEB_EXC_SAV_MIN);
#endif
            }
            if (c_tray_on > 0 && c_tray_on < (TRY_END+1))
            {
                if (c_tray_on==TRY_002) tray_to_on();//(9*250ms=2250ms)=9
                if (c_tray_on==TRY_003) tray_to_on();//9+(5*250ms=1250ms)=14
                c_tray_on++;//14+(5*250ms=1250ms)=19 ++ to 20max
            }
        }
    }
}
