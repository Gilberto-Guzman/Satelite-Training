#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>


File Archivo;
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();


const uint8_t DetectaSD = 9;   //Pin para detectar tarjeta SD (CS)
const uint8_t DetectaVo = 7;   // Pin analógico para detectar voltaje


//**********************************************************************************************************************Agregar línea
const uint8_t ID = 10;


uint8_t i;
uint8_t j;
uint8_t x=0;
uint8_t k=1;


float tm = 1;                 //Tiempo de Muestreo(seg)


char fileName[] = "CuboZat.txt";
char Encab[] = "CuboZat_v1";


//-----------------------------------Función de Inicialización-------------------------------------------------------


void setup() {
 
  Serial.begin(115200); // Apertura Puerto Serial
  while (!Serial);  // Espera a que el puerto serial se conecte
 
  pinMode(DetectaSD, INPUT); // Pone Pin 9 como Salida


  //encabezado();   //Imprime Encabezados


  inicializaSD();  //Inicializa Tarjeta SD


  inicializaSensor();  //Inicializa Sensores


  tm=tm*1000;           //Calcula valor para tiempo de Muestreo
}


//-----------------------------------Función Main-------------------------------------------------------


void loop() {
  // put your main code here, to run repeatedly:


//**********************************************************************************************************************Cambiar a 11
float VecDat[11];


datSens(VecDat);  // Lectura de Sensores
LeeVoltaje(VecDat); // Lectura de Voltaje


if(x>=1){      
mandaSerial(VecDat);  //Manda Datos a Puerto Serial


Archivo = SD.open(fileName, FILE_WRITE);  //Abre Archivo de Texto    
escribeSD(VecDat);  //Escribe Datos en Archivo de Texto
}


x=x+1;
delay(tm);  //Tiempo de Muestreo
}


//---------------------------------Función Para Obtener Datos del Sensor---------------------------------


void datSens(float *VecDatos){


  lsm.read();  /* ask it to read in the data */


  /* Get a new sensor event */
  sensors_event_t a, m, g, temp;


  lsm.getEvent(&a, &m, &g, &temp);


  VecDatos[0]=a.acceleration.x;
  VecDatos[1]=a.acceleration.y;
  VecDatos[2]=a.acceleration.z;
  VecDatos[3]=m.magnetic.x;
  VecDatos[4]=m.magnetic.y;
  VecDatos[5]=m.magnetic.z;
  VecDatos[6]=g.gyro.x;
  VecDatos[7]=g.gyro.y;
  VecDatos[8]=g.gyro.z;
}




//-----------------------------------Función Para Leer Voltaje-----------------------------------


void LeeVoltaje(float *VecDatos){


float V = analogRead(DetectaVo);    
float Volt = ((V * 5.0)/1024);


VecDatos[9]=Volt;  
}


//---------------------------------Función Para Manadar Datos al Puerto Serial----------------------------


void mandaSerial(float *VecDatos){
String Paq="";


  //**********************************************************************************************************************Cambiar a 10
  for(i=0;i<10;i++){
      Paq += String(VecDatos[i]);
      Paq += String("\t");
  }
  //**********************************************************************************************************************Agregar línea
  VecDatos[10] = ID;
  Paq += VecDatos[10];


  Paq += String("\n");
 
Serial.print(Paq);
}


//-----------------------------------Función Para Escribir en Trajeta SD-----------------------------------


void escribeSD(float *VecDatos){
String Paq="";


Paq += String("P");
Paq += String(x); //Número de Paquete
Paq += String(":\t");


//**********************************************************************************************************************Cambiar a 10
  for(i=0;i<10;i++){
      Paq += String(VecDatos[i]);
      Paq += String("\t");
  }
 
  Archivo.println(Paq);
  Archivo.close();
}


//---------------------------------Función Para imprimir Encabezados---------------------------------


//void encabezado(){


// Serial.println(Encab);
 //Serial.print("\n");


//}


//-----------------------------------Función Inicialización Trajeta SD-----------------------------------


void inicializaSD()  {
 //Serial.print("Inicializando Tarjeta SD...\n");  
 
 // while (!SD.begin(DetectaSD)) {            //Determinando si la SD está conectada
  //  Serial.println("Esperando Tarjeta...!");
 //   delay(250);
 // }
  //Serial.println("Inicialización Terminada.");




if (SD.exists(fileName)) {   //Elimina archivo CubeZat.txt si ya existe alguno
    SD.remove(fileName);
}


  Archivo = SD.open(fileName, FILE_WRITE);  //Creando archivo .txt
  if (Archivo)
    {
   // Archivo.println(Encab);
    Archivo.close();
    Serial.println("Archivo Listo.");
    }
 // else
 // {
  //  Serial.println("Error abriendo archivo");
 // }
}


//-----------------------------------Función Inicializar Sensores-----------------------------------


void  inicializaSensor()
{
  if (!lsm.begin())
  {
    //Serial.println("No es posible inicializar el LSM9DS1.");
    while (1);
  }
  //Serial.println("LSM9DS1 Encontrado");
 // Serial.print("\n");
 
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
 
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);


  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}
