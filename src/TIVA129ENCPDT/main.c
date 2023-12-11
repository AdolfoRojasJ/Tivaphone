#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"
#include <math.h>

//Variables de Control
uint32_t door=1;
uint32_t light1=0;
uint32_t light2=0;
uint32_t light3=0;
uint32_t light4=0;
uint32_t food=0;
uint32_t alarm=0;
uint32_t result;
uint8_t segundos, minutos, horas;

//Variables auxiliares
float temp;
int TPR = 7;
uint8_t error;
uint32_t i;
uint8_t d_uint8Dato;
uint32_t result;
float voltage;

//**Direcciones del DS1307
int AdreDS1307 =0x068;///Dirección del RTC DS1307
int AdreSec= 0x00;
int AdreMin=0x01;

//Loop
uint32_t ui32Loop;

//Funciones
void INIPORTS(void);
void GPIOPortA_Handler(void);
void OPENDOOR(void);
void CLOSEDOOR(void);
void FOODSERVE(void);
void TEMPREAD(void);
void FIREALARM(void);
int esperar();
void CargarFecha();
void LeerFecha();
void RTCREAD(void);
char UART6_Lee_dato(void);

//Programa principal
main(void)
{
        INIPORTS();
       // while(I2C0_MCS_R&0x00000001){};

        //TIMER 0
        TIMER0_TAILR_R= 0xE200; // VALOR DE RECARGA 50 Hz
        TIMER0_TAPR_R= 0X04; // PREESCALADOR
        TIMER0_CTL_R |= 0X00000041;

        //TIMER 1
        TIMER1_TAILR_R= 0x1200; // VALOR DE RECARGA 2 Hz
        TIMER1_TAPR_R= 0X7A; // PREESCALADOR
        TIMER1_CTL_R |= 0X00000041;

        //TIMER 4
        TIMER4_TAILR_R= 0xE200; // VALOR DE RECARGA 50 Hz
        TIMER4_TAPR_R= 0X04; // PREESCALADOR
        TIMER4_CTL_R |= 0X00000041;

         while(1)
         {
             //RTCREAD();
           // UART6_Lee_dato();
             TEMPREAD();

             switch(d_uint8Dato)
             {
             case 23:
                 door=0;
                 break;
             case 24:
                 light1=1;
                 break;
             case 25:
                 light2=1;
                 break;
             case 26:
                 light3=1;
                 break;
             case 27:
                 light4=1;
                 break;
             case 28:
                 food=1;
                 break;
             case 36:
                 alarm=0;
                 break;
             }

            if(light1!=0)
            {
                GPIO_PORTK_DATA_R ^= 0x01;
                light1=0;
                SysCtlDelay(350000);
                d_uint8Dato=0;
            }

            if(light2!=0)
            {
                GPIO_PORTK_DATA_R ^= 0x02;
                light2=0;
                SysCtlDelay(350000);
                d_uint8Dato=0;
            }

            if(light3!=0)
            {
                GPIO_PORTK_DATA_R ^= 0x04;
                light3=0;
                SysCtlDelay(350000);
                d_uint8Dato=0;
            }

            if(light4!=0)
            {
               GPIO_PORTK_DATA_R ^= 0x08;
               light4=0;
               SysCtlDelay(350000);
               d_uint8Dato=0;
            }

            if(temp>35)
            {
                FIREALARM();
                alarm = 1;
                d_uint8Dato=0;
            }

            if(temp<=35 && alarm == 0)
            {
               TIMER1_TAMATCHR_R = 0X0000;
            }

            if(door==1)
            {
                OPENDOOR();
            }

            if(door==0)
            {
                CLOSEDOOR();
                d_uint8Dato=0;
            }

            if(food==1)
            {
                FOODSERVE();
                d_uint8Dato=0;
            }

        }
}

//Inicialización de Puertos
void INIPORTS(void)
{
    //TIMERS
    SYSCTL_RCGCGPIO_R |= 0x00002A1B; //A,B,D,E,K,M,P
    SYSCTL_RCGCTIMER_R |= 0X13; //TIMER 0 & 1 & 4
    SYSCTL_RCGCADC_R  = 0x01; //ADC0
    SYSCTL_RCGCI2C_R |= 0x0001;//I2C
    SYSCTL_RCGCUART_R |=0X40;//UART

    ui32Loop = SYSCTL_RCGCGPIO_R;

    //PUERTO A
    GPIO_PORTA_AHB_DIR_R &= ~0x80;
    GPIO_PORTA_AHB_DEN_R |= 0x80;
    GPIO_PORTA_AHB_PUR_R |= 0x80;
    GPIO_PORTA_AHB_IS_R &= ~0x80;
    GPIO_PORTA_AHB_IBE_R &= ~0x01;
    GPIO_PORTA_AHB_IEV_R &= ~0x80;
    GPIO_PORTA_AHB_ICR_R = 0x80;
    GPIO_PORTA_AHB_IM_R |= 0x80;

    //PUERTO B
    GPIO_PORTB_AHB_AFSEL_R |= 0x0C;
    GPIO_PORTB_AHB_ODR_R |= 0x08;
    GPIO_PORTB_AHB_DIR_R |= 0x0C;
    GPIO_PORTB_AHB_DEN_R |= 0x0C;
    GPIO_PORTB_AHB_PCTL_R|=0x00002200;

    //PUERTO D
    GPIO_PORTD_AHB_DEN_R |= 0x45;
    GPIO_PORTD_AHB_DIR_R |= 0x45;
    GPIO_PORTD_AHB_DATA_R = 0x00;
    GPIO_PORTD_AHB_AFSEL_R = 0x45;
    GPIO_PORTD_AHB_PCTL_R = 0x03000303;

    //PUERTO E
    GPIO_PORTE_AHB_DIR_R = 0x00;
    GPIO_PORTE_AHB_AFSEL_R |= 0x10;
    GPIO_PORTE_AHB_DEN_R = 0x00;
    GPIO_PORTE_AHB_AMSEL_R |= 0x10;

    //PUERTO K
    GPIO_PORTK_DEN_R |= 0x0F;
    GPIO_PORTK_DIR_R |= 0x0F;
    GPIO_PORTK_DATA_R = 0x00;

    //PUERTO M
    GPIO_PORTM_DIR_R|=0xFF;
    GPIO_PORTM_DEN_R|=0XFF;
    GPIO_PORTM_DATA_R=0x00;

    //PUERTO P
    GPIO_PORTP_PCTL_R = (GPIO_PORTP_PCTL_R&0XFFFFFF00)+0X00000011;
    GPIO_PORTP_DEN_R|=0X03;
    GPIO_PORTP_AMSEL_R &= ~0X03;
    GPIO_PORTP_AFSEL_R |= 0X03;

    //TIMER 0
    TIMER0_CTL_R=0X00000000;
    TIMER0_CFG_R= 0X00000004;
    TIMER0_TAMR_R= 0X0000000A;

    //TIMER 1
    TIMER1_CTL_R=0X00000000;
    TIMER1_CFG_R= 0X00000004;
    TIMER1_TAMR_R= 0X0000000A;

    //TIMER 4
    TIMER4_CTL_R=0X00000000;
    TIMER4_CFG_R= 0X00000004;
    TIMER4_TAMR_R= 0X0000000A;

    //I2C0
    I2C0_MCR_R = 0x00000010;
    I2C0_MTPR_R = TPR;

    //ADC0
    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;
    while((SYSCTL_PLLSTAT_R&0x01)==0);
    SYSCTL_PLLFREQ0_R &= ~SYSCTL_PLLFREQ0_PLLPWR;
    ADC0_ISC_R = 0x0008;                // Se recomienda Limpia la bandera RIS del ADC0
    ADC0_ACTSS_R = 0x0000;

    ADC0_PC_R = 0x01;
    ADC0_SSPRI_R = 0x0123;
    ADC0_EMUX_R = 0x0000;
    ADC0_SSEMUX3_R = 0x00;
    ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0) + 9;
    ADC0_SSCTL3_R = 0x0006;
    ADC0_IM_R = 0x0000;
    ADC0_ACTSS_R |= 0x0008;

    //UART 6
    UART6_CTL_R &=~0X0001;
    UART6_IBRD_R = 8 ; //115200
    UART6_FBRD_R =43 ;
    UART6_LCRH_R =0X0070; //8 BITS, HABILITAR FIFO
    UART6_CTL_R= 0X0301 ;

    //INTERRUPCIONES
    NVIC_EN0_R= 0x01;  //habilita la interrupción en puerto A
}

//INTERRUPCIÓN PUERTO A
void GPIOPortA_Handler(void)
{
    GPIO_PORTA_AHB_ICR_R = 0x80;
    door=1;
}

//ABRIR PUERTA
void OPENDOOR(void)
{
    TIMER0_TAMATCHR_R = 31500;
    SysCtlDelay(8000000);
}

//CERRAR PUERTA
void CLOSEDOOR(void)
{
    TIMER0_TAMATCHR_R = 16500;
    SysCtlDelay(8000000);
}

//SERVIR COMIDA
void FOODSERVE(void)
{
    TIMER4_TAMATCHR_R = 31500;
    SysCtlDelay(23000000);
    TIMER4_TAMATCHR_R = 16500;
    food=0;

}

//LEER TEMPERATURA
void TEMPREAD(void)
{
    ADC0_ISC_R = 0x0008;
    ADC0_PSSI_R = 0x0008;
    while ((ADC0_RIS_R & 0x08)==0);
    result = (ADC0_SSFIFO3_R & 0xFFF)*100;
    ADC0_ISC_R = 0x0008;
    temp=(result*3.3)/4095;
}

//ALARMA INCENDIOS
void FIREALARM(void)
{
    TIMER1_TAMATCHR_R = 0X0900;
    TIMER1_TAPR_R = 0X00;
    SysCtlDelay(4000000);
    while(alarm == 1)
    {
        TEMPREAD();
        GPIO_PORTM_DATA_R=0b00001000;
        SysCtlDelay(30000);
        GPIO_PORTM_DATA_R=0b00000100;
        SysCtlDelay(30000);
        GPIO_PORTM_DATA_R=0b00000010;
        SysCtlDelay(30000);
        GPIO_PORTM_DATA_R=0b00000001;
        SysCtlDelay(30000);
    }
}

//ESPERAR
int esperar(){
      while(I2C0_MCS_R&0x00000001){}; //Espero a que la transmisión acabe
      if(I2C0_MCS_R&0x00000002==1){ //¿Hubo error?
          error=1;
          return error;
      };
      return 0;
}

//CARGAR FECHA
void CargarFecha(){

    int segundos=0x00, minutos=0x11, horas=0x08;
    while(I2C0_MCS_R&0x00000001){};
    I2C0_MSA_R=(AdreDS1307<<1)&0xFE;
    I2C0_MDR_R=AdreSec&0x0FF;
    I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_START);
    for(i=0;i<300;i++){}
    esperar();

    //SEGUNDOS
    I2C0_MDR_R=segundos;
    I2C0_MCS_R=(I2C_MCS_RUN);
    for(i=0;i<300;i++){} //Delay
    esperar();

    //MINUTOS
    I2C0_MDR_R=minutos;
    I2C0_MCS_R=(I2C_MCS_RUN);
    for(i=0;i<300;i++){}
    esperar();

    //HORAS
    I2C0_MDR_R=horas;
    I2C0_MCS_R=(I2C_MCS_RUN);
    for(i=0;i<300;i++){}
    esperar();

}

//LEER FECHA
void LeerFecha(){
        while(I2C0_MCS_R&0x00000001){};

        I2C0_MSA_R=(AdreDS1307<<1)&0xFE;
        I2C0_MDR_R=AdreSec&0x0FF;
        I2C0_MCS_R=(I2C_MCS_START|I2C_MCS_RUN);
        for(i=0;i<300;i++){}
        esperar();

        I2C0_MSA_R=(AdreDS1307<<1)&0xFE;
        I2C0_MSA_R|=0x01;
        I2C0_MCS_R=(I2C_MCS_START|I2C_MCS_RUN|I2C_MCS_ACK);
        for(i=0;i<300;i++){}
        esperar();
        segundos=(I2C0_MDR_R&0xFF);

        I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_ACK);
        for(i=0;i<300;i++){}
        esperar();
        minutos=(I2C0_MDR_R&0xFF);

         I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_ACK);
         for(i=0;i<300;i++){}
         esperar();
         horas=(I2C0_MDR_R&0xFF);

         I2C0_MCS_R=(I2C_MCS_STOP|I2C_MCS_RUN);
}

//ENVIAR DATOS DEL RTC
void RTCREAD()
{
    CargarFecha(); // Función para configurar al esclavo RTC DS1307
    do{
       LeerFecha();
       }while(error!=1);
}

//LEER DATO DE TIVA 1
char UART6_Lee_dato(void)
{
    //while((UART6_FR_R&0X0010)!=0); //ESPERAR A QUE RXFE SEA CERO
    d_uint8Dato=((char)(UART6_DR_R&0xff));//lecturayescritura
    return d_uint8Dato;
}
