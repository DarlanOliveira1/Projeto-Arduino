//Bibliotecas
#include <IRremote.h>

//sensor temperatura
#include <OneWire.h> //INCLUSÃO DE BIBLIOTECA
#include <DallasTemperature.h> //INCLUSÃO DE BIBLIOTECA

//####################################
//##    Definicao pinos             ##
//####################################

//Sensor Ir
#define receptor    2   //Pino sensor ir
#define sensorPorta 3   //Pino sensor de porta
#define pinLed      4   //Pino do led
#define pinBip      5   //Pino sirene
#define pinoSenFogo 6   //Pino sensor fogo
#define pinoSenTemp 7   //Pino sensor temperatura

#define pinoLp1   8   //Pino lampada 1
#define pinoLp2   9   //Pino lampada 1
#define pinoLp3   10  //Pino lampada 1
#define pinoLp4   11  //Pino lampada 1
#define pinoLp5   12  //Pino lampada 1
#define pinoLp6   13  //Pino lampada 1


//####################
//##    Variaveis   ##
//####################
#define liga    0xFFA25D //Liga
#define desliga 0xFF629D //desliga

byte nStatusAlarme      = 0;    //0 = StandBy; 1 = sim; 2 = nao
byte nStatusAlarmeOld   = 0;    //0 = StandBy; 1 = sim; 2 = nao
byte nDispara           = 2;    //1 = sim; 2 = nao
byte nPortaAberta       = 2;    //1 = sim; 2 = nao
byte nPortaAbertaOld    = 2;    //1 = sim; 2 = nao
byte nFgoAmb            = 2 ;   //1= Sim / 2 = Nao
byte nFgoAmbOld         = 2 ;   //1= Sim / 2 = Nao
//variaveis mills
long tempoAntBip200     = 0;
long tempo2             = 200;
long tempo3             = 100;
int  nBipsTocDelig      = 0;
int  nBipsTocLig        = 0;
int  numBipsLigar       = 5;    //numeros de bip ao ligar
int  numBipsDelig       = 3;    //numeros de bip ao desligar
long tempoAntLed300     = 300;
float temperatura       = 0;

//recebida esp
byte nEspAlarme         = 0;
byte nEspLp1            = 0;
byte nEspLp2            = 0;
byte nEspLp3            = 0;
byte nEspLp4            = 0;
byte nEspLp5            = 0;
byte nEspLp6            = 0;

String nEspAlrSub       = "";
String nEspLp1Sub       = "";
String nEspLp2Sub       = "";
String nEspLp3Sub       = "";
String nEspLp4Sub       = "";
String nEspLp5Sub       = "";
String nEspLp6Sub       = "";

//auxiliares
String Dados;
String Resultado[15];
int contador = 0;
int desvio = 0;

//sensor de temperatura
OneWire ourWire(2); //CONFIGURA UMA INSTÂNCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
DallasTemperature sensors(&ourWire); //BIBLIOTECA DallasTemperature UTILIZA A OneWire

IRrecv recIR(receptor);
decode_results resultado;


//############################
//##    Inicio programa     ##
//############################
void setup(){

    Serial.begin(9600); //Inicializa Serial
    recIR.enableIRIn(); //Inicializa receptor
    //Serial.println("Sensor IR Habilitado");

    pinMode(sensorPorta,INPUT_PULLUP);   //Pino sensor de porta

    //Led Status
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, HIGH);

    //Bip
    pinMode(pinBip, OUTPUT);
    digitalWrite(pinBip, LOW);
    
    //Lampada 1
    pinMode(pinoLp1, OUTPUT);
    digitalWrite(pinoLp1, LOW);

    //Lampada 2
    pinMode(pinoLp2, OUTPUT);
    digitalWrite(pinoLp2, LOW);

    //Lampada 3
    pinMode(pinoLp3, OUTPUT);
    digitalWrite(pinoLp3, LOW);

    //Lampada 4
    pinMode(pinoLp4, OUTPUT);
    digitalWrite(pinoLp4, LOW);

    //Lampada 5
    pinMode(pinoLp5, OUTPUT);
    digitalWrite(pinoLp5, LOW);

    //Lampada 6
    pinMode(pinoLp6, OUTPUT);
    digitalWrite(pinoLp6, LOW);

    //Sensor temperatura
    sensors.begin();    //INICIA O SENSOR
    //sensor de temperatura
    OneWire ourWire(pinoSenTemp); //CONFIGURA UMA INSTÂNCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
    DallasTemperature sensors(&ourWire); //BIBLIOTECA DallasTemperature UTILIZA A OneWire
    //Pega a temperatura a primeira vez
    sensors.requestTemperatures();//SOLICITA QUE A FUNÇÃO INFORME A TEMPERATURA DO SENSOR
    temperatura = sensors.getTempCByIndex(0);
}


void loop(){


    //Dados recebidos Esp
    if (Serial.available()) {

        Dados = Serial.readString();
        
        contador        = 0;
        desvio          = 0;
        nEspAlarme      = Dados.substring(1).toInt();

        nEspAlrSub         = Dados.substring(0,1);
        nEspLp1Sub         = Dados.substring(2,3);
        nEspLp2Sub         = Dados.substring(4,5);
        nEspLp3Sub         = Dados.substring(6,7);
        nEspLp4Sub         = Dados.substring(8,9);
        nEspLp5Sub         = Dados.substring(10,11);
        nEspLp6Sub         = Dados.substring(12,13);

        nEspAlarme      = nEspAlrSub.toInt();
        nEspLp1         = nEspLp1Sub.toInt();   
        nEspLp2         = nEspLp2Sub.toInt();
        nEspLp3         = nEspLp3Sub.toInt();
        nEspLp4         = nEspLp4Sub.toInt();
        nEspLp5         = nEspLp5Sub.toInt();
        nEspLp6         = nEspLp6Sub.toInt();
        
        //Serial.println("valor 3");
        Serial.println(nEspLp3);

        if((nEspAlarme == 1) and (nStatusAlarme != 1)){
            
            //Serial.println("Ligar acionado");
            nStatusAlarme   = 1;//Seta pra ligado
            nDispara        = 2; //Disparo habilitado
            nPortaAberta    = 2; //seta a porta pra fechada novamente
            nFgoAmb         = 2; //Seta o sensor de fogo para negativo
            nBipsTocLig     = 0;
            digitalWrite(pinBip, LOW);  //Sempre desliga a sirene

        }

        if((nEspAlarme == 2) and (nStatusAlarme != 2)){
            
            //Serial.println("Desligar acionado");
            nStatusAlarme   = 2; //Seta pra desligado
            nDispara        = 2; //Disparo habilitado
            nPortaAberta    = 2; //seta a porta pra fechada novamente
            nFgoAmb         = 2; //Seta o sensor de fogo para negativo
            nBipsTocDelig   = 0;
            digitalWrite(pinBip, LOW);  //Sempre desliga a sirene

        }


    }

    
    if(recIR.decode(&resultado)){
        
        //Se verdadeiro Liga
        if(resultado.value == liga){

            //Serial.println("Ligar acionado");
            nStatusAlarme   = 1;//Seta pra ligado
            nDispara        = 2; //Disparo habilitado
            nPortaAberta    = 2; //seta a porta pra fechada novamente
            nFgoAmb         = 2; //Seta o sensor de fogo para negativo
            nBipsTocLig     = 0;
            digitalWrite(pinBip, LOW);  //Sempre desliga a sirene


        }

        //Se verdadeiro Desliga
        if(resultado.value == desliga){
          
            //Serial.println("Desligar acionado");
            nStatusAlarme   = 2; //Seta pra desligado
            nDispara        = 2; //Disparo habilitado
            nPortaAberta    = 2; //seta a porta pra fechada novamente
            nFgoAmb         = 2; //Seta o sensor de fogo para negativo
            nBipsTocDelig   = 0;
            digitalWrite(pinBip, LOW);  //Sempre desliga a sirene



        }

        recIR.resume();
    }

    //verifica as luzes independente do status do alarme
    //Lampada 1
    if(nEspLp1 == 1){
        digitalWrite(pinoLp1, HIGH);
    } else if(nEspLp1 == 2){

        digitalWrite(pinoLp1, LOW);
    }

    //Lampada 2
    if(nEspLp2 == 1){
        digitalWrite(pinoLp2, HIGH);
    } else if(nEspLp2 == 2){

        digitalWrite(pinoLp2, LOW);
    }

    //Lampada 3
    if(nEspLp3 == 1){
        digitalWrite(pinoLp3, HIGH);
    } else if(nEspLp3 == 2){

        digitalWrite(pinoLp3, LOW);
    }

    //Lampada 4
    if(nEspLp4 == 1){
        digitalWrite(pinoLp4, HIGH);
    } else if(nEspLp4 == 2){

        digitalWrite(pinoLp4, LOW);
    }

    //Lampada 5
    if(nEspLp5 == 1){
        digitalWrite(pinoLp5, HIGH);
    } else if(nEspLp5 == 2){

        digitalWrite(pinoLp5, LOW);
    }

    //Lampada 1
    if(nEspLp6 == 1){
        digitalWrite(pinoLp6, HIGH);
    } else if(nEspLp6 == 2){

        digitalWrite(pinoLp6, LOW);
    }


    //########################################
    //##    Acoes apos ligar e desligar     ##
    //########################################
    //Se ligado
    if(nStatusAlarme == 1){

        LedAlarmeLigado();
        somLigaAlarme();

        //##########################
        //## verifica porta       ##
        //##########################
        if ((digitalRead(sensorPorta)) and (nPortaAberta != 1)){
            
            nPortaAberta    = 1;
            nDispara        = 1; //Habilita a variavel de dispara pois a porta foi violada

        }


        //##########################
        //## verifica se tem fogo ##
        //##########################
        if((digitalRead(pinoSenFogo) == LOW)and (nFgoAmb != 1)) { //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
            //Serial.println("Fogo no ambiente");
            nDispara    = 1;//Disparo habilitado
            nFgoAmb     = 1;
        }


        if(nDispara == 1){
            disparaAlarme();
        }
    }
    //Se desligado
    if(nStatusAlarme == 2){
        somDesligaAlarme();
        digitalWrite(pinLed, HIGH);

    }

    //Vica verificando as diferencas e mandando para o esp
    //if ((nPortaAberta != nPortaAbertaOld) or (nFgoAmb != nFgoAmbOld) or (nStatusAlarme != nStatusAlarmeOld)  ){
    if ((nPortaAberta != nPortaAbertaOld) or (nFgoAmb != nFgoAmbOld)){
        
        //Serial.println("manda informacao porta para esp");
        
        nPortaAbertaOld     = nPortaAberta;
        nFgoAmbOld          = nFgoAmb;
        nStatusAlarmeOld    = nStatusAlarme;

        Serial.println( String(nPortaAberta) + ';'+ String(nFgoAmbOld) + ';' + String(temperatura) );
    }

    //########################################################
    //##    Dalay para nao ficar rodando 100% do tempo      ##
    //########################################################
    delay(10);
}


//Pisca o led quando o alarme estiver ligado
void LedAlarmeLigado() {

    if(millis() >= (tempoAntLed300 + 300)){
        tempoAntLed300 = millis();
        digitalWrite(pinLed, !digitalRead(pinLed));
    }

}

//Ligar alarme
void somLigaAlarme() {
    
    if(millis() >= (tempoAntBip200 + tempo2)){

        tempoAntBip200 = millis();
        if(nBipsTocLig < numBipsLigar){
            digitalWrite(pinBip, !digitalRead(pinBip));
        }else{
            digitalWrite(pinBip, LOW);
        }
        nBipsTocLig ++;
    }

}

//Desligar alarme
void somDesligaAlarme() {
    
    if(millis() >= (tempoAntBip200 + tempo2)){

        tempoAntBip200 = millis();
        if(nBipsTocDelig < numBipsDelig){
            digitalWrite(pinBip, !digitalRead(pinBip));
        }else{
            digitalWrite(pinBip, LOW);
        }
        nBipsTocDelig ++;
    }

}

//Dispara alarme
void disparaAlarme() {
    
    if(millis() >= (tempoAntBip200 + tempo3)){
        tempoAntBip200 = millis();
        digitalWrite(pinBip, !digitalRead(pinBip));

    }

}



