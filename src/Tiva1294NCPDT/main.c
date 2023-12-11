//Bibliotecas
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include <stdio.h>
#include <string.h>

//Variables Teclado
unsigned char Valcon,Temp,Temp2;
uint8_t   contador;
int Fila,Columna,val,a, valtec;
char charTecla;
int jumpcount=0;
uint8_t Valcon,Temp,Temp2,i;

//Variables Almacenamiento
int number [10];
char message [32];
char telephonenumber[10];
char messagenumber[10];
uint8_t d_uint8Dato;
uint8_t d_uint8Message;
char messagefull[100];

//Banderas Modo Mensaje
uint32_t keyboardflag=0;
uint32_t spaceflag=0;
uint32_t spaceflag2=0;

//Inicio Mensaje
uint32_t field1=0x80;

//Counters
int j=0;
int a=0;
int x=0;
int messagecounter=0;

//Display
#define bitset(Var,bitno) ((Var) |= 1 << (bitno))
#define bitclr(Var,bitno) ((Var) &= ~(1 << (bitno)))
#define PORTD_AHBIS GPIO_PORTK_DATA_R
#define PORTCON GPIO_PORTM_DATA_R
#define RS   0x00
#define R_W  0x01
#define E    0x02
#define BIT7 0X80
#define DISP_ON 0X0F
#define CLR_DISP 0x01
void INILCD(void);
void BUSY (void);
void BUSY1 (void);
void _E(void);
void ESCDAT (unsigned char c);
void ESCCOM (unsigned char d);
void CLS(void);
void BYTEDIS(unsigned char DByte);
void HOME(void);
void AND491(void);
void LEECON(void);
void ESCCON (uint8_t d);

//Teclado
void keyboard_INI(void);
void LeerTeclas(void);

//Funciones Tiempos
uint32_t ui32SysClkFreq=16000000;//si definimos la frecuencia del reloj con SysCtlClockFreqSet cambiar este valor de 16 MHz
void delayUs(uint32_t Us);
void delayMs(uint32_t Ms);

void delayUs(uint32_t Us) { //funcion para lograr un retardo de tantos microsegundos
    SysCtlDelay(Us*(ui32SysClkFreq/3000000.0));//3/1000000;
}
void delayMs(uint32_t Ms) { //funcion para lograr un retardo de tantos mili
    SysCtlDelay(Ms*(ui32SysClkFreq/3000.0));//3/1000;
}
#define bitset(Var,bitno) ((Var) |= 1 << (bitno)) // |         0000 0100 <-0
#define bitclr(Var,bitno) ((Var) &= ~(1 << (bitno)))  //&      1111 1011

/////Lectura de datos en teclado matricial////
int filas[] = {0x0E, 0x0D, 0x0B, 0x07};
int columnas[] = {0x0E, 0x0D, 0x0B, 0x07};
char teclas[4][4] ={{'1','4','7','*'},{'2','5','8','0'},{'3','6','9','#'},{'A','B','C','D'}};

//FUNCIONES
void INIPORTS(void);
void BOOT(void);
void PHONEMODEINI(void);
void CALL (void);
void ENDCALL (void);
void INSERTMESSAGENUMBER(void);
void KEY2(void);
void KEY3(void);
void KEY4(void);
void KEY5(void);
void KEY6(void);
void KEY7(void);
void KEY8(void);
void KEY9(void);
void KEY0(void);
void MESSAGESENT(void);
char UART6_Escribe_Dato(char dato);
char UART6_Lee_Dato();
char UART0_Escribe_Dato(char dato);
void DEVELOPMENTCREDITS(void);
void DOORCONTROL(void);
void LED1CONTROL(void);
void LED2CONTROL(void);
void LED3CONTROL(void);
void LED4CONTROL(void);
void FOODCONTROL(void);
void MATRIXCONTROL(void);
void POWERCONTROL1(void);
void POWERCONTROL2(void);
void POWERCONTROL3(void);
void POWERCONTROL4(void);
void POWERCONTROL5(void);
void ALARMCONTROL(void);

//SIM808 COMANDOS
#define CR     0x0D
#define LF     0x0A
#define CTRL_Z 0x1A

//FUNCIONES TELEFONO
void ENVIOCOMANDO(void);
void ENVIOMENSAJE(void);
void C_ENTER(void);
////////////PROGRAMA PRINCIPAL//////////////////
void main(void)
{
    BOOT();
    j=0;
    while(1)
    {
        LeerTeclas();
        if (charTecla!=0xff)
        {
            valtec=charTecla;
            ESCDAT(valtec);
            if (valtec == 'A')  //MODO TELEFONO
            {
                UART6_Escribe_Dato ('A');
                UART6_Escribe_Dato ('T');
                C_ENTER();
                PHONEMODEINI();
                while(1)
                {
                    LeerTeclas();
                    if (charTecla!=0xff)
                    {
                        //LLENADO DE ARREGLO
                        if (j<10)
                        {
                            number[j]=charTecla;
                            telephonenumber[j]=number[j]-48;
                            //REGRESO A HOME
                            if (number[j] == '*')
                            {
                                main();
                            }
                            ESCDAT(number[j]);
                            j++;
                        }
                        //FIN LLENADO DE ARREGLO
                        if (charTecla == 'A') // OPCION 1 LLAMAR
                        {
                            ESCCOM(CLR_DISP);
                            UART6_Escribe_Dato ('A');
                            UART6_Escribe_Dato ('T');
                            UART6_Escribe_Dato ('D');
                            UART6_Escribe_Dato (telephonenumber[0]+'0');
                            UART6_Escribe_Dato (telephonenumber[1]+'0');
                            UART6_Escribe_Dato (telephonenumber[2]+'0');
                            UART6_Escribe_Dato (telephonenumber[3]+'0');
                            UART6_Escribe_Dato (telephonenumber[4]+'0');
                            UART6_Escribe_Dato (telephonenumber[5]+'0');
                            UART6_Escribe_Dato (telephonenumber[6]+'0');
                            UART6_Escribe_Dato (telephonenumber[7]+'0');
                            UART6_Escribe_Dato (telephonenumber[8]+'0');
                            UART6_Escribe_Dato (telephonenumber[9]+'0');
                            UART6_Escribe_Dato (';');
                            C_ENTER();
                            CALL();
                        }
                        if (charTecla == 'B') //OPCION 2 COLGAR
                        {
                            UART6_Escribe_Dato ('A');
                            UART6_Escribe_Dato ('T');
                            UART6_Escribe_Dato ('H');
                            C_ENTER();
                            ENDCALL();
                        }
                        if (charTecla == 'C') //OPCION 3 BORRAR NUMERO ESCRITO
                        {
                            ESCCOM(CLR_DISP);
                            j=0;
                        }
                    }
                }
            }

            if (valtec == 'B')  //MODO MENSAJE
            {
                UART6_Escribe_Dato ('A');
                UART6_Escribe_Dato ('T');
                C_ENTER();
                spaceflag=0;
                spaceflag2=0;
                field1=0x80;
                messagecounter=0;
                ESCCOM(CLR_DISP);
                while(1)
                {
                    LeerTeclas();
                    if (charTecla!=0xff)
                    {

                        valtec=charTecla;
                        //REGRESO A HOME
                        if (valtec == '*')
                        {
                            main();
                        }
                        if (charTecla == 'A') //ENVIAR MENSAJE
                        {
                            j=0;
                            ESCCOM(CLR_DISP);
                            INSERTMESSAGENUMBER();
                            ESCCON(0XC0);
                            while(1)
                            {
                                LeerTeclas();
                                if (charTecla!=0xff)
                                {
                                    //LLENADO DE ARREGLO
                                    if (j<11)
                                    {
                                        number[j]=charTecla;
                                        messagenumber[j]=number[j]-48;
                                        //REGRESO A HOME
                                        if (number[j] == '*')
                                        {
                                            main();
                                        }

                                        if(j==9)
                                        {
                                            UART6_Escribe_Dato ('A');
                                            UART6_Escribe_Dato ('T');
                                            UART6_Escribe_Dato (0x2B);
                                            UART6_Escribe_Dato ('C');
                                            UART6_Escribe_Dato ('M');
                                            UART6_Escribe_Dato ('G');
                                            UART6_Escribe_Dato ('S');
                                            UART6_Escribe_Dato (0X3D);
                                            UART6_Escribe_Dato (0X22);
                                            SysCtlDelay(53333);
                                            UART6_Escribe_Dato (messagenumber[0]+'0');
                                            UART6_Escribe_Dato (messagenumber[1]+'0');
                                            UART6_Escribe_Dato (messagenumber[2]+'0');
                                            UART6_Escribe_Dato (messagenumber[3]+'0');
                                            UART6_Escribe_Dato (messagenumber[4]+'0');
                                            UART6_Escribe_Dato (messagenumber[5]+'0');
                                            UART6_Escribe_Dato (messagenumber[6]+'0');
                                            UART6_Escribe_Dato (messagenumber[7]+'0');
                                            UART6_Escribe_Dato (messagenumber[8]+'0');
                                            UART6_Escribe_Dato (messagenumber[9]+'0');
                                            UART6_Escribe_Dato (0X22);
                                            C_ENTER();
                                            SysCtlDelay(53333);
                                            UART6_Escribe_Dato (message[0]);
                                            UART6_Escribe_Dato (message[1]);
                                            UART6_Escribe_Dato (message[2]);
                                            UART6_Escribe_Dato (message[3]);
                                            UART6_Escribe_Dato (message[4]);
                                            UART6_Escribe_Dato (message[5]);
                                            SysCtlDelay(53333);
                                            UART6_Escribe_Dato (message[6]);
                                            UART6_Escribe_Dato (message[7]);
                                            UART6_Escribe_Dato (message[8]);
                                            UART6_Escribe_Dato (message[9]);
                                            UART6_Escribe_Dato (message[10]);
                                            SysCtlDelay(53333);
                                            UART6_Escribe_Dato (message[11]);
                                            UART6_Escribe_Dato (message[12]);
                                            UART6_Escribe_Dato (message[13]);
                                            UART6_Escribe_Dato (message[14]);
                                            UART6_Escribe_Dato (message[15]);
                                            SysCtlDelay(53333);
                                            UART6_Escribe_Dato (message[16]);
                                            UART6_Escribe_Dato (message[17]);
                                            UART6_Escribe_Dato (message[18]);
                                            UART6_Escribe_Dato (message[19]);
                                            UART6_Escribe_Dato (message[20]);
                                            SysCtlDelay(53333);
                                            UART6_Escribe_Dato (message[21]);
                                            UART6_Escribe_Dato (message[22]);
                                            UART6_Escribe_Dato (message[23]);
                                            UART6_Escribe_Dato (message[24]);
                                            UART6_Escribe_Dato (message[25]);
                                            SysCtlDelay(53333);
                                            UART6_Escribe_Dato (message[26]);
                                            UART6_Escribe_Dato (message[27]);
                                            UART6_Escribe_Dato (message[28]);
                                            UART6_Escribe_Dato (message[29]);
                                            UART6_Escribe_Dato (message[30]);
                                            SysCtlDelay(53333);
                                            UART6_Escribe_Dato (message[31]);
                                            UART6_Escribe_Dato (CTRL_Z);
                                            C_ENTER();
                                        }

                                        if (charTecla == 'A') //ENVIAR MENSAJE
                                       {

                                            ESCCOM(CLR_DISP);
                                            MESSAGESENT();
                                       }

                                        ESCDAT(number[j]);
                                        j++;


                                    } //FIN LLENADO DE ARREGLO
                                    if (charTecla == 'C') //BORRAR NUMERO
                                    {
                                            ESCCOM(CLR_DISP);
                                            INSERTMESSAGENUMBER();
                                            ESCCON(0XC0);
                                            j=0;
                                    }

                                }

                            }

                        }
                        if (charTecla == 'C') //LIMPIAR MENSAJE
                        {
                            ESCCOM(CLR_DISP);
                            ESCCON(0x80);
                            field1=0x80;
                            spaceflag=0;
                            spaceflag2=0;
                            messagecounter=0;
                        }
                        LeerTeclas();
                        if (valtec == '2')
                        {
                            KEY2();
                        }
                        if (valtec == '3')
                        {
                            KEY3();
                        }
                        if (valtec == '4')
                        {
                            KEY4();
                        }
                        if (valtec == '5')
                        {
                            KEY5();
                        }
                        if (valtec == '6')
                        {
                            KEY6();
                        }
                        if (valtec == '7')
                        {
                            KEY7();
                        }
                        if (valtec == '8')
                        {
                            KEY8();
                        }
                        if (valtec == '9')
                        {
                            KEY9();
                        }
                        if (valtec == '0')
                        {
                            KEY0();
                        }
                    }
                }
            } //FIN MODO MENSAJE


            if (valtec == 'C')  //MODO CONTROL
            {
                ESCCOM(CLR_DISP);
                x=0;
                while(1)
                {
                    UART6_Lee_Dato(); //[73-76]
                    if(x<100)
                    {
                        messagefull[x]=d_uint8Message;
                        x++;
                    }
                    //PUERTA
                    if (messagefull[73] == 'D' && messagefull[74] == 'O' && messagefull[75] == 'O' && messagefull[76] == 'R')
                    {
                        ESCCOM(CLR_DISP);
                        DOORCONTROL();
                    }

                    //FOCO 1
                    if (messagefull[73] == 'L' && messagefull[74] == 'E' && messagefull[75] == 'D' && messagefull[76] == '1')
                    {
                        ESCCOM(CLR_DISP);
                        LED1CONTROL();
                    }

                    //FOCO 2
                    if (messagefull[73] == 'L' && messagefull[74] == 'E' && messagefull[75] == 'D' && messagefull[76] == '2')
                    {
                        ESCCOM(CLR_DISP);
                        LED2CONTROL();
                    }

                    //FOCO 3
                    if (messagefull[73] == 'L' && messagefull[74] == 'E' && messagefull[75] == 'D' && messagefull[76] == '3')
                    {
                        ESCCOM(CLR_DISP);
                        LED3CONTROL();
                    }

                    //FOCO 4
                    if (messagefull[73] == 'L' && messagefull[74] == 'E' && messagefull[75] == 'D' && messagefull[76] == '4')
                    {
                        ESCCOM(CLR_DISP);
                        LED4CONTROL();
                    }

                    //DISPENSADOR
                   if (messagefull[73] == 'F' && messagefull[74] == 'O' && messagefull[75] == 'O' && messagefull[76] == 'D')
                   {
                       ESCCOM(CLR_DISP);
                       FOODCONTROL();
                   }

                   //MATRIZ LED
                   if (messagefull[73] == 'P' && messagefull[74] == 'L' && messagefull[75] == 'A' && messagefull[76] == 'Y')
                   {
                       ESCCOM(CLR_DISP);
                       MATRIXCONTROL();

                   }

                   //POTENCIA 1
                   if (messagefull[73] == 'P' && messagefull[74] == 'W' && messagefull[75] == 'R' && messagefull[76] == '1')
                  {
                      ESCCOM(CLR_DISP);
                      POWERCONTROL1();
                  }

                   //POTENCIA 2
                    if (messagefull[73] == 'P' && messagefull[74] == 'W' && messagefull[75] == 'R' && messagefull[76] == '2')
                    {
                        ESCCOM(CLR_DISP);
                        POWERCONTROL2();
                    }

                    //POTENCIA 3
                    if (messagefull[73] == 'P' && messagefull[74] == 'W' && messagefull[75] == 'R' && messagefull[76] == '3')
                    {
                        ESCCOM(CLR_DISP);
                        POWERCONTROL3();
                    }

                    //POTENCIA 4
                    if (messagefull[73] == 'P' && messagefull[74] == 'W' && messagefull[75] == 'R' && messagefull[76] == '4')
                    {
                        ESCCOM(CLR_DISP);
                        POWERCONTROL4();
                    }

                    //POTENCIA 5
                    if (messagefull[73] == 'P' && messagefull[74] == 'W' && messagefull[75] == 'R' && messagefull[76] == '5')
                    {
                        ESCCOM(CLR_DISP);
                        POWERCONTROL5();
                    }

                   //APAGAR ALARMA
                   if (messagefull[73] == 'F' && messagefull[74] == 'I' && messagefull[75] == 'R' && messagefull[76] == 'E')
                   {
                       ESCCOM(CLR_DISP);
                       ALARMCONTROL();
                   }


                }

            } //FIN MODO CONTROL

            if (valtec == 'D')  //USO LIBRE
            {
                ESCCOM(CLR_DISP);
                DEVELOPMENTCREDITS();
            } //FIN USO LIBRE

        }

    }
}

////////FUNCIONES/////////////

//FUNCION ARRANQUE
void BOOT(void)
{
    INIPORTS();
    INILCD(); //inicializa el display 16 x 2
    ESCCOM(CLR_DISP);
    ESCDAT('S');
    ESCDAT('E');
    ESCDAT('L');
    ESCDAT('E');
    ESCDAT('C');
    ESCDAT('C');
    ESCDAT('I');
    ESCDAT('O');
    ESCDAT('N');
    ESCDAT('A');
    ESCCON(0xC0);
    ESCDAT('M');
    ESCDAT('O');
    ESCDAT('D');
    ESCDAT('O');
    ESCDAT(':');
    ESCDAT(' ');
}

//INICIALIZACIÓN MODO TELÉFONO
void PHONEMODEINI(void)
{
    spaceflag=0;
    spaceflag2=0;
    field1=0x80;
    ESCCOM(CLR_DISP);
}

//LLAMADA
void CALL (void)
{
    ESCDAT('L');
    ESCDAT('L');
    ESCDAT('A');
    ESCDAT('M');
    ESCDAT('A');
    ESCDAT('N');
    ESCDAT('D');
    ESCDAT('O');
    ESCDAT('.');
    ESCDAT('.');
    ESCDAT('.');
    ESCCON(0XC0);
}

//COLGAR
void ENDCALL (void)
{
    ESCCOM(CLR_DISP);
    ESCDAT('F');
    ESCDAT('I');
    ESCDAT('N');
    SysCtlDelay(4300000);
    ESCCOM(CLR_DISP);
    main();
}

//INSERTAR NUMERO DEL MENSAJE
void INSERTMESSAGENUMBER (void)
{
    ESCDAT('N');
    ESCDAT('U');
    ESCDAT('M');
    ESCDAT('E');
    ESCDAT('R');
    ESCDAT('O');
    ESCDAT(':');
}

//MENSAJE ENVIADO
void MESSAGESENT (void)
{
    ESCDAT('E');
    ESCDAT('N');
    ESCDAT('V');
    ESCDAT('I');
    ESCDAT('A');
    ESCDAT('D');
    ESCDAT('O');
    SysCtlDelay(4300000);
    ESCCOM(CLR_DISP);
    main();
}

//TECLA "2"
void KEY2(void)
{
    keyboardflag++;
    if (keyboardflag == 1){
     ESCCON(field1);
     ESCDAT('A');
     message[messagecounter]='A';
    }
    else if (keyboardflag == 2){
    ESCCON(field1);
    ESCDAT('B');
    message[messagecounter]='B';
    }
    else if (keyboardflag == 3){
    ESCCON(field1);
    ESCDAT('C');
    keyboardflag=0;
    message[messagecounter]='C';
    }
}

//TECLA "3"
void KEY3(void)
{
    keyboardflag++;
    if (keyboardflag == 1){
    ESCCON(field1);
    ESCDAT('D');
    message[messagecounter]='D';
    }
    else if (keyboardflag == 2){
    ESCCON(field1);
    ESCDAT('E');
    message[messagecounter]='E';
    }
    else if (keyboardflag == 3){
    ESCCON(field1);
    ESCDAT('F');
    message[messagecounter]='F';
    keyboardflag=0;
    }
}

//TECLA "4"
void KEY4(void)
{
    keyboardflag++;
    if (keyboardflag == 1){
    ESCCON(field1);
    ESCDAT('G');
    message[messagecounter]='G';
    }
    else if (keyboardflag == 2){
    ESCCON(field1);
    ESCDAT('H');
    message[messagecounter]='H';
    }
    else if (keyboardflag == 3){
    ESCCON(field1);
    ESCDAT('I');
    message[messagecounter]='I';
    keyboardflag=0;
    }
}

//TECLA "5"
void KEY5(void)
{
    keyboardflag++;
    if (keyboardflag == 1){
    ESCCON(field1);
    ESCDAT('J');
    message[messagecounter]='J';
    }
    else if (keyboardflag == 2){
    ESCCON(field1);
    ESCDAT('K');
    message[messagecounter]='K';
    }
    else if (keyboardflag == 3){
    ESCCON(field1);
    ESCDAT('L');
    message[messagecounter]='L';
    keyboardflag=0;
    }
}

//TECLA "6"
void KEY6(void)
{
    keyboardflag++;
    if (keyboardflag == 1){
    ESCCON(field1);
    ESCDAT('M');
    message[messagecounter]='M';
    }
    else if (keyboardflag == 2){
    ESCCON(field1);
    ESCDAT('N');
    message[messagecounter]='N';
    }
    else if (keyboardflag == 3){
    ESCCON(field1);
    ESCDAT('O');
    message[messagecounter]='O';
    keyboardflag=0;
    }
}

//TECLA "7"
void KEY7(void)
{
    keyboardflag++;
    if (keyboardflag == 1){
    ESCCON(field1);
    ESCDAT('P');
    message[messagecounter]='P';
    }
    else if (keyboardflag == 2){
    ESCCON(field1);
    ESCDAT('Q');
    message[messagecounter]='Q';
    }
    else if (keyboardflag == 3){
    ESCCON(field1);
    ESCDAT('R');
    message[messagecounter]='R';
    }
    else if (keyboardflag == 4){
    ESCCON(field1);
    ESCDAT('S');
    message[messagecounter]='S';
    keyboardflag=0;
    }
}

//TECLA "8"
void KEY8(void)
{
    keyboardflag++;
    if (keyboardflag == 1){
    ESCCON(field1);
    ESCDAT('T');
    message[messagecounter]='T';
    }
    else if (keyboardflag == 2){
    ESCCON(field1);
    ESCDAT('U');
    message[messagecounter]='U';
    }
    else if (keyboardflag == 3){
    ESCCON(field1);
    ESCDAT('V');
    message[messagecounter]='V';
    keyboardflag=0;
    }
}

//TECLA "9"
void KEY9(void)
{
    keyboardflag++;
    if (keyboardflag == 1){
    ESCCON(field1);
    ESCDAT('W');
    message[messagecounter]='W';
    }
    else if (keyboardflag == 2){
    ESCCON(field1);
    ESCDAT('X');
    message[messagecounter]='X';
    }
    else if (keyboardflag == 3){
    ESCCON(field1);
    ESCDAT('Y');
    message[messagecounter]='Y';
    }
    else if (keyboardflag == 4){
    ESCCON(field1);
    ESCDAT('Z');
    message[messagecounter]='Z';
    keyboardflag=0;
    }
}

//TECLA "0"
void KEY0(void)
{
    if(spaceflag<16)
    {
    spaceflag++;
    field1=0x80+spaceflag;
    }
    if(spaceflag>15)
    {
    field1=0xC0+spaceflag2;
    spaceflag2++;
    }
    messagecounter++;
}

//FUNCION INICIALIZACIÓN DE PUERTOS
void INIPORTS(void)
{
    SYSCTL_RCGCGPIO_R |= 0X6A81;
    while ((SYSCTL_PRGPIO_R&0x6A81)!=0x6A81);

    SYSCTL_RCGCUART_R |=0X41; //UART 0 Y 6

    //PUERTO A
    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R&0XFFFFFF00)+0X00000011;
    GPIO_PORTA_AHB_DEN_R|=0X03;
    GPIO_PORTA_AHB_AMSEL_R &= ~0X03;
    GPIO_PORTA_AHB_AFSEL_R |= 0X03;

    //PUERTO H
    GPIO_PORTH_AHB_DATA_R=0X00;
    GPIO_PORTH_AHB_DEN_R|=0XFF;
    GPIO_PORTH_AHB_DIR_R=0X0f;

    //PUERTO K
    GPIO_PORTK_DATA_R=0X00;
    GPIO_PORTK_DEN_R|=0XFF;
    GPIO_PORTK_DIR_R|=0Xff;

    //PUERTO M
    GPIO_PORTM_DATA_R=0X00;
    GPIO_PORTM_DEN_R|=0X07;
    GPIO_PORTM_DIR_R|=0x07;

    //PUERTO P
    GPIO_PORTP_PCTL_R = (GPIO_PORTP_PCTL_R&0XFFFFFF00)+0X00000011;
    GPIO_PORTP_DEN_R|=0X03;
    GPIO_PORTP_AMSEL_R &= ~0X03;
    GPIO_PORTP_AFSEL_R |= 0X03;

    //PUERTO Q
    GPIO_PORTQ_DEN_R|=0XFF;
    GPIO_PORTQ_DIR_R=0X00;
    GPIO_PORTQ_PUR_R=0X0F;

    //UART 0
    UART0_CTL_R &=~0X0001;
    UART0_IBRD_R = 8;
    UART0_FBRD_R =43;
    UART0_LCRH_R =0X0070;
    UART0_CTL_R= 0X0301 ;

    //UART 6
    UART6_CTL_R &=~0X0001;
    UART6_IBRD_R = 8 ; //115200
    UART6_FBRD_R =43 ;
    UART6_LCRH_R =0X0070; //8 BITS, HABILITAR FIFO
    UART6_CTL_R= 0X0301 ;
}

//FUNCIÓN INICIALIZACIÓN DISPLAY 2X16
void INILCD(void)
{
    PORTCON=0X00;
    PORTD_AHBIS=0X38;

    SysCtlDelay(533000);
    _E();
    SysCtlDelay (53000);
    _E();
    SysCtlDelay (53000);
    PORTD_AHBIS=0X38;
    _E();
    SysCtlDelay (53000);
    ESCCOM(0X38);
    ESCCOM(DISP_ON);

    ESCCOM(CLR_DISP);

    ESCCOM(0X06);
    ESCCOM(0X38);
}

//FUNCIÓN QUE GENERA UN PULSO DE 1.25 uS EN LA TERMINAL E DEL DISPLAY
void _E(void)
{
    bitset(PORTCON,E);
    SysCtlDelay (3);  //retraso de 500 nS
    bitclr(PORTCON,E);
}

//ESCRITURA DE DATO EN EL DISPLAY
void ESCDAT (unsigned char c)
{
    PORTD_AHBIS = c;
    bitset(PORTCON,RS);
    _E();
    BUSY1();
    bitclr(PORTCON,RS);
}

//COMANDO AL DISPLAY
void ESCCOM (unsigned char d)
{
    PORTD_AHBIS = d;
    bitclr(PORTCON,RS);
    _E();
    BUSY1();
}

//COMANDO AL REGISTRO DE CONTROL DEL LCD
void ESCCON (uint8_t d)
{
    PORTD_AHBIS = d;
    bitclr(PORTCON,R_W);
    bitclr(PORTCON,RS);
    _E();
    BUSY();
}

//CLS
void CLS(void)
{
    ESCCOM(0x01);
}

//HOME
void HOME(void)
{
    ESCCOM(0x02);
}

// GENERA LOS CORRIMIENTOS PARA ALINEAR COMO EN EL AND491 PARA LOS DISPLAYS GENERICOS
void AND491(void)
{
    ESCCOM (0X1C);
    ESCCOM (0X1C);
    ESCCOM (0X1C);
    ESCCOM (0X1C);
}

//BANDERA BUSY
void BUSY (void)
{
    do LEECON( );
    while ((Valcon & BIT7) != 0);
}

//LEE EL VALOR DEL REGISTRO DE CONTROL
void   LEECON(void)
{
    PORTD_AHBIS=0;
    GPIO_PORTK_DIR_R=0x00;
    bitset(PORTCON,R_W);
    bitset(PORTCON,E);
    SysCtlDelay(2); //espera 384 nS
    Temp=PORTD_AHBIS;
    bitclr(PORTCON,E);
    bitclr(PORTCON,RS);
    bitclr(PORTCON,R_W);
    GPIO_PORTK_DIR_R=0XFF;
    Valcon=Temp;
}

//ESCRIBE UN BYTE EN LA PANTALLA
void BYTEDIS(unsigned char DByte)
{
    Temp2=DByte;
    Temp2=Temp2>>4;
    if (Temp2<=0x09)
    Temp2+=0x30;
    else
    Temp2+=0x37;
    ESCDAT(Temp2);
    Temp2=DByte&0x0f;
    if (Temp2<=0x09)
    Temp2+=0x30;
    else
    Temp2+=0x37;
    ESCDAT(Temp2);
}

//BANDERA BUSY1
void BUSY1(void)
{
    SysCtlDelay(10066);
}

//LEER TECLAS DEL TECLADO MATRICIAL

void LeerTeclas(void)
{
    charTecla=0xff;
    if((GPIO_PORTQ_DATA_R&0x0f)!=0x0f)
    {
        delayUs(20000);
        val =  GPIO_PORTQ_DATA_R;
        for (Fila=0; Fila<4; Fila++)
        {
            GPIO_PORTH_AHB_DATA_R = filas[Fila];
            for (Columna=0;Columna<4;Columna++)
            {
                if (GPIO_PORTQ_DATA_R == columnas[Columna])
                {
                    charTecla=teclas [Fila][Columna];
                }
            }
        }
    GPIO_PORTH_AHB_DATA_R &= 0x00;
    while((GPIO_PORTQ_DATA_R&0x0f)!=0x0f);
       delayUs(20000);
    }
}

//ENVIAR DATO A TIVA 2
char UART0_Escribe_Dato(char dato1)
{
while   ((UART0_FR_R&0X0020)!=0);
UART0_DR_R=dato1;
}

//UART TELEFONO
char UART6_Lee_Dato(void)
{
while((UART6_FR_R&0X0010)!=0);
d_uint8Message=((char)(UART6_DR_R&0xff));
}

char UART6_Escribe_Dato(char dato)
{
UART6_DR_R=dato;
}

//OPCIÓN LIBRE
void DEVELOPMENTCREDITS(void)
{
    ESCDAT('2');
    ESCDAT('0');
    ESCDAT('2');
    ESCDAT('4');
    ESCDAT('-');
    ESCDAT('1');
    ESCCON(0xC0);
    ESCDAT('A');
    ESCDAT('D');
    ESCDAT('O');
    ESCDAT('L');
    ESCDAT('F');
    ESCDAT('O');
    ESCDAT(' ');
    ESCDAT('R');
    ESCDAT('O');
    ESCDAT('J');
    ESCDAT('A');
    ESCDAT('S');
    SysCtlDelay(16000000);
    main();
}

//FUNCIONES DE CONTROL
void DOORCONTROL(void)
{
    d_uint8Dato= 23;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('D');
    ESCDAT('O');
    ESCDAT('O');
    ESCDAT('R');
    ESCCON(0xC0);
    ESCDAT('O');
    ESCDAT('P');
    ESCDAT('E');
    ESCDAT('N');
    ESCDAT('E');
    ESCDAT('D');
    SysCtlDelay(16000000);
    main();
}

void LED1CONTROL(void)
{
    d_uint8Dato= 24;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('L');
    ESCDAT('I');
    ESCDAT('G');
    ESCDAT('H');
    ESCDAT('T');
    ESCCON(0xC0);
    ESCDAT('1');
    SysCtlDelay(16000000);
    main();
}

void LED2CONTROL(void)
{
    d_uint8Dato= 25;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('L');
    ESCDAT('I');
    ESCDAT('G');
    ESCDAT('H');
    ESCDAT('T');
    ESCCON(0xC0);
    ESCDAT('2');
    SysCtlDelay(16000000);
    main();
}

void LED3CONTROL(void)
{
    d_uint8Dato= 26;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('L');
    ESCDAT('I');
    ESCDAT('G');
    ESCDAT('H');
    ESCDAT('T');
    ESCCON(0xC0);
    ESCDAT('3');
    SysCtlDelay(16000000);
    main();
}

void LED4CONTROL(void)
{
    d_uint8Dato= 27;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('L');
    ESCDAT('I');
    ESCDAT('G');
    ESCDAT('H');
    ESCDAT('T');
    ESCCON(0xC0);
    ESCDAT('4');
    SysCtlDelay(16000000);
    main();
}

void FOODCONTROL(void)
{
    d_uint8Dato= 28;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('Y');
    ESCDAT('U');
    ESCDAT('M');
    ESCDAT('!');
    SysCtlDelay(16000000);
    main();
}

void MATRIXCONTROL(void)
{
    d_uint8Dato= 29;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('M');
    ESCDAT('A');
    ESCDAT('T');
    ESCDAT('R');
    ESCDAT('I');
    ESCDAT('X');
    SysCtlDelay(16000000);
    main();
}

void POWERCONTROL1(void)
{
    d_uint8Dato= 30;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('M');
    ESCDAT('O');
    ESCDAT('D');
    ESCDAT('U');
    ESCDAT('L');
    ESCDAT('A');
    ESCDAT('T');
    ESCDAT('E');
    SysCtlDelay(16000000);
    main();
}

void POWERCONTROL2(void)
{
    d_uint8Dato= 31;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('M');
    ESCDAT('O');
    ESCDAT('D');
    ESCDAT('U');
    ESCDAT('L');
    ESCDAT('A');
    ESCDAT('T');
    ESCDAT('E');
    SysCtlDelay(16000000);
    main();
}

void POWERCONTROL3(void)
{
    d_uint8Dato= 32;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('M');
    ESCDAT('O');
    ESCDAT('D');
    ESCDAT('U');
    ESCDAT('L');
    ESCDAT('A');
    ESCDAT('T');
    ESCDAT('E');
    SysCtlDelay(16000000);
    main();
}

void POWERCONTROL4(void)
{
    d_uint8Dato= 33;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('M');
    ESCDAT('O');
    ESCDAT('D');
    ESCDAT('U');
    ESCDAT('L');
    ESCDAT('A');
    ESCDAT('T');
    ESCDAT('E');
    SysCtlDelay(16000000);
    main();
}

void POWERCONTROL5(void)
{
    d_uint8Dato= 34;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('M');
    ESCDAT('O');
    ESCDAT('D');
    ESCDAT('U');
    ESCDAT('L');
    ESCDAT('A');
    ESCDAT('T');
    ESCDAT('E');
    SysCtlDelay(16000000);
    main();
}

void ALARMCONTROL(void)
{
    d_uint8Dato= 36;
    UART0_Escribe_Dato(d_uint8Dato);
    ESCDAT('O');
    ESCDAT('F');
    ESCDAT('F');
    SysCtlDelay(16000000);
    main();
}

//FUNCIONES TELEFONO

void C_ENTER(void)
{
    UART6_Escribe_Dato(CR);
    UART6_Escribe_Dato(LF);
}

