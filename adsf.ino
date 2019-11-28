#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(11,12); // 
Servo Servoalto;
Servo Servobajo;

char colors[6];
 int banda1;
 int banda2;
 int banda3;
 int banda4;
 int banda5;
 int banda6;
 int noreconocido=0;
 bool enviar=false;
 
 int rojas = 0;
 int naranjas = 0;
 int verdes = 0;
 int amarillas = 0;
 int cafes = 0;
 int azules = 0;
 int noreconocidas = 0;
 
 
 
  
 int bandasalidarojo;
 int bandasalidanaranja;
 int bandasalidaverde;
 int bandasalidaamarillo;
 int bandasalidacafe;
 int bandasalidaazul;

 int gradobanda1 = 18;
 int gradobanda2 = 40;
 int gradobanda3 = 65;
 int gradobanda4 = 93;
 int gradobanda5 = 118;
 int gradobanda6 = 140;
 int gradobanda7 = 170;
 
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
int frequency = 0;
int color=0;
char detener='a';

void setup() {
  Servoalto.attach(7);
  Servobajo.attach(8);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);
  BT.begin(9600);

  pinMode(13, OUTPUT);
  
}

char led; // Led es nuestro led conectado a Arduino
void loop() {
  if(BT.readBytes(colors,6) == 6){
    noreconocido=0;
    detener='a';
    banda1 = colors[0];
    banda2 = colors[1];
    banda3 = colors[2];
    banda4 = colors[3];
    banda5 = colors[4];
    banda6 = colors[5];
    enviar=true;
    
    configurar();
    Serial.print("Color Rojo va salir por la banda que tiene grado: ");
    Serial.println(bandasalidarojo);
    
    Serial.print("Color Naranja va salir por la banda que tiene grado: ");
    Serial.println(bandasalidanaranja);
    
    Serial.print("Color Verde va salir por la banda que tiene grado: ");
    Serial.println(bandasalidaverde);
    
    Serial.print("Color Amarillo va salir por la banda que tiene grado: ");
    Serial.println(bandasalidaamarillo);
    
    Serial.print("Color Cafe va salir por la banda que tiene grado: ");
    Serial.println(bandasalidacafe);
    
    Serial.print("Color Azul va salir por la banda que tiene grado: ");
    Serial.println(bandasalidaazul);

    while(noreconocido<5 | detener=='a'){
      procedimiento();            
      detener=BT.read();      
      if(detener=='b'){
        Serial.print("Se detendra el servicio");
        enviar=true;
        break;        
      }            
    }    
  }
  if(enviar){    
    BT.println("Reportes de Botonetas");
    BT.print("Botonetas rojas: ");
    BT.println(rojas);
    
    BT.print("Botonetas naranjas: ");    
    BT.println(naranjas);
    
    BT.print("Botonetas verdes: ");
    BT.println(verdes);
    
    BT.print("Botonetas amarillas: ");
    BT.println(amarillas);
    
    BT.print("Botonetas cafes: ");
    BT.println(cafes);
    
    BT.print("Botonetas azules: ");
    BT.println(azules);
    
    BT.print("Botonetas No Reconocidas: ");
    BT.println(noreconocidas);
    
    enviar=false;
  }
  
}

void procedimiento(){
  Servoalto.write(128);
  delay(300);
  
  for(int i = 128; i > 70; i--) {
    Servoalto.write(i);
    delay(3);
  }
  delay(500);

  color = readColor();
  Serial.println("Identificando Color");
  delay(500);  
  
  switch (color) {
    case 1:{
      Serial.println("Color Rojo");
      Servobajo.write(bandasalidarojo);
      noreconocido=0;
      break;
    }
    
    case 2:{
      Serial.println("Color Anaranjado");
      Servobajo.write(bandasalidanaranja);
      noreconocido=0;
      break;
    }
    
    case 3:{
      Serial.println("Color Verde");
      Servobajo.write(bandasalidaverde);
      noreconocido=0;
      break;
    }
    
    case 4:{
      Serial.println("Color Amarillo");
      Servobajo.write(bandasalidaamarillo);
      noreconocido=0;
      break;
    }
    
    case 5:{
      Serial.println("Color Cafe");
      Servobajo.write(bandasalidacafe);
      noreconocido=0;
      break;
    }
    
    case 6:{
      Serial.println("Color Azul");
      Servobajo.write(bandasalidaazul);
      noreconocido=0;
      break;
    }
    
    case 0:{
      Serial.print("Color No reconocido");
      Servobajo.write(gradobanda7);
      noreconocido++;
      noreconocidas++;
      break;
    }
  }
  delay(300);
  color=0;

  Serial.println("  ");    
    
  for(int i = 70; i > 38; i--) {
    Servoalto.write(i);
    delay(3);
  } 
  
  delay(200);
  
  for(int i = 38; i < 129; i++) {
    Servoalto.write(i);
    delay(1);
  }
}




// Custom Function - readColor()
int readColor() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int R = frequency;
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(50);
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int G = frequency;
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(50);
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int B = frequency;
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  delay(50);
  if(R<70 & R>46 & G<91 & G>76 & B<77 & B>64){
    color = 1; // Red
    rojas++;
  }
  if(R<47 & R>34 & G<84 & G>67 & B<80 & B>64){
    color = 2; // Orange
    naranjas++;
  }
  if(R<70 & R>50 & G<65 & G>46 & B<73 & B>61){
    color = 3; // Green
    verdes++;
  }
  if(R<46 & R>28 & G<59 & G>38 & B<64 & B>51 ){
    color = 4; // Yellow
    amarillas++;
  }
  if(R<75 & R>65 & G<93 & G>82 & B<80  & B>67){
    color = 5; // Brown
    cafes++;
  }
  if (R<82 & R>70 & G<73 & G>55 & B<53 & B>41){
    color = 6; // Blue
    azules++;
  }
  return color;  
}

void configurar(){
  switch (colors[0]){
    case 1:{
      bandasalidarojo=gradobanda1;
      break;
    }
    case 2:{
      bandasalidanaranja=gradobanda1;
      break;
    }
    case 3:{
      bandasalidaverde=gradobanda1;
      break;
    }
    case 4:{
      bandasalidaamarillo=gradobanda1;
      break;
    }
    case 5:{
      bandasalidacafe=gradobanda1;
      break;
    }
    case 6:{
      bandasalidaazul=gradobanda1;
      break;
    }    
  }

  switch (colors[1]){
    case 1:{
      bandasalidarojo=gradobanda2;
      break;
    }
    case 2:{
      bandasalidanaranja=gradobanda2;
      break;
    }
    case 3:{
      bandasalidaverde=gradobanda2;
      break;
    }
    case 4:{
      bandasalidaamarillo=gradobanda2;
      break;
    }
    case 5:{
      bandasalidacafe=gradobanda2;
      break;
    }
    case 6:{
      bandasalidaazul=gradobanda2;
      break;
    }    
  }

  switch (colors[2]){
    case 1:{
      bandasalidarojo=gradobanda3;
      break;
    }
    case 2:{
      bandasalidanaranja=gradobanda3;
      break;
    }
    case 3:{
      bandasalidaverde=gradobanda3;
      break;
    }
    case 4:{
      bandasalidaamarillo=gradobanda3;
      break;
    }
    case 5:{
      bandasalidacafe=gradobanda3;
      break;
    }
    case 6:{
      bandasalidaazul=gradobanda3;
      break;
    }    
  }

  switch (colors[3]){
    case 1:{
      bandasalidarojo=gradobanda4;
      break;
    }
    case 2:{
      bandasalidanaranja=gradobanda4;
      break;
    }
    case 3:{
      bandasalidaverde=gradobanda4;
      break;
    }
    case 4:{
      bandasalidaamarillo=gradobanda4;
      break;
    }
    case 5:{
      bandasalidacafe=gradobanda4;
      break;
    }
    case 6:{
      bandasalidaazul=gradobanda4;
      break;
    }    
  }

  switch (colors[4]){
    case 1:{
      bandasalidarojo=gradobanda5;
      break;
    }
    case 2:{
      bandasalidanaranja=gradobanda5;
      break;
    }
    case 3:{
      bandasalidaverde=gradobanda5;
      break;
    }
    case 4:{
      bandasalidaamarillo=gradobanda5;
      break;
    }
    case 5:{
      bandasalidacafe=gradobanda5;
      break;
    }
    case 6:{
      bandasalidaazul=gradobanda5;
      break;
    }
        
  }
  
  switch (colors[5]){
    case 1:{
      bandasalidarojo=gradobanda6;
      break;
    }
    case 2:{
      bandasalidanaranja=gradobanda6;
      break;
    }
    case 3:{
      bandasalidaverde=gradobanda6;
      break;
    }
    case 4:{
      bandasalidaamarillo=gradobanda6;
      break;
    }
    case 5:{
      bandasalidacafe=gradobanda6;
      break;
    }
    case 6:{
      bandasalidaazul=gradobanda6;
      break;
    }    
  }
}
