#include "config.h"

//variaveis

byte nArdStatusPrt    = 2;     //Valor Porta Arduino
byte nIotStatusPrt    = 2;     //Valor Porta Iot

byte nArdStatusFg     = 2;     //Valor Fogo Arduino
byte nIotStatusFg     = 2;     //Valor Fogo Iot

byte nArdStatusLp1    = 2;     //Valor Fogo Arduino
byte nIotStatusLp1    = 2;     //Valor Fogo Iot

byte nArdStatusLp2    = 2;     //Valor Fogo Arduino
byte nIotStatusLp2    = 2;     //Valor Fogo Iot

byte nArdStatusLp3    = 2;     //Valor Fogo Arduino
byte nIotStatusLp3    = 2;     //Valor Fogo Iot

byte nArdStatusLp4    = 2;     //Valor Fogo Arduino
byte nIotStatusLp4    = 2;     //Valor Fogo Iot

byte nArdStatusLp5    = 2;     //Valor Fogo Arduino
byte nIotStatusLp5    = 2;     //Valor Fogo Iot

byte nArdStatusLp6    = 2;     //Valor Fogo Arduino
byte nIotStatusLp6    = 2;     //Valor Fogo Iot

byte nIotStatusAlr    = 2;
byte nOldIotStatusAlr = 3;
byte nOldIotStatusLp1 = 3;
byte nOldIotStatusLp2 = 3;
byte nOldIotStatusLp3 = 3;
byte nOldIotStatusLp4 = 3;
byte nOldIotStatusLp5 = 3;
byte nOldIotStatusLp6 = 3;

//Variaveis adicionais
int n1 = 0;
String Dados;
String Resultado[15];
int contador = 0;
int desvio = 0;

//instanciando objetos
AdafruitIO_Feed *alarme = io.feed("alarme");
AdafruitIO_Feed *porta = io.feed("porta");
AdafruitIO_Feed *fogo = io.feed("fogo");
AdafruitIO_Feed *lampada1 = io.feed("lampada1");
AdafruitIO_Feed *lampada2 = io.feed("lampada2");
AdafruitIO_Feed *lampada3 = io.feed("lampada3");
AdafruitIO_Feed *lampada4 = io.feed("lampada4");
AdafruitIO_Feed *lampada5 = io.feed("lampada5");
AdafruitIO_Feed *lampada6 = io.feed("lampada6");




void setup() {
    Serial.begin(9600);

    io.connect();

    alarme->onMessage(getStatusAlarme);
    porta->onMessage(getStatusPorta);
    fogo->onMessage(getStatusFogo);
    lampada1->onMessage(getStatusLp1);
    lampada2->onMessage(getStatusLp2);
    lampada3->onMessage(getStatusLp3);
    lampada4->onMessage(getStatusLp4);
    lampada5->onMessage(getStatusLp5);
    lampada6->onMessage(getStatusLp6);

    while (io.status() < AIO_CONNECTED) {
        delay(100);
    }

    alarme->get();
    porta->get();
    fogo->get();
    lampada1->get();
    lampada2->get();
    lampada3->get();
    lampada4->get();
    lampada5->get();
    lampada6->get();

}

void loop() {

    io.run();

    if( ((nIotStatusAlr != nOldIotStatusAlr) and (nIotStatusAlr != 0)) or 
        ((nIotStatusLp1 != nOldIotStatusLp1) and (nIotStatusLp1 != 0)) or
        ((nIotStatusLp2 != nOldIotStatusLp2) and (nIotStatusLp2 != 0)) or
        ((nIotStatusLp3 != nOldIotStatusLp3) and (nIotStatusLp3 != 0)) or
        ((nIotStatusLp4 != nOldIotStatusLp4) and (nIotStatusLp4 != 0)) or
        ((nIotStatusLp5 != nOldIotStatusLp5) and (nIotStatusLp5 != 0)) or
        ((nIotStatusLp6 != nOldIotStatusLp6) and (nIotStatusLp6 != 0))
        ){

        Serial.println( String(nIotStatusAlr) + ';' + String(nIotStatusLp1) + ';' + String(nIotStatusLp2) + ';' + String(nIotStatusLp3) + ';' + String(nIotStatusLp4) + ';' + String(nIotStatusLp5) + ';' + String(nIotStatusLp6) + ';' );
                      
        nOldIotStatusAlr = nIotStatusAlr;
        nOldIotStatusLp1 = nIotStatusLp1;
        nOldIotStatusLp2 = nIotStatusLp2;
        nOldIotStatusLp3 = nIotStatusLp3;
        nOldIotStatusLp4 = nIotStatusLp4;
        nOldIotStatusLp5 = nIotStatusLp5;
        nOldIotStatusLp6 = nIotStatusLp6;
    }

    if(Serial.available()){

        String Dados = Serial.readString();
        for(int i =0; i < Dados.length(); i++){
            if(Dados.substring(i, i+1) == ";"){
                if (contador == 0){
                    Resultado[contador] = Dados.substring(0, i);
                    Resultado[contador+1] = Dados.substring(i + 1);
                }else{
                    Resultado[contador] = Dados.substring(desvio + 1, i);
                }
            desvio = 1;
            contador = contador + 1;
            }
            if(contador > 0){
                Resultado[contador + 1] = Dados.substring(i + 2);
            }
        }
        
        contador = 0;
        desvio = 0;
        
        nArdStatusPrt = Resultado[0].toInt();
        nArdStatusFg  = Resultado[1].toInt();
        if((nArdStatusPrt == 1) or (nArdStatusPrt == 2)){
            if((nArdStatusPrt != nIotStatusPrt) and nArdStatusPrt != 0){
                porta->save(nArdStatusPrt);
                nIotStatusPrt = nArdStatusPrt;
            }
        }
        if((nArdStatusFg == 1) or (nArdStatusFg == 2)){
            if((nArdStatusFg != nIotStatusFg) and nArdStatusFg != 0){
                fogo->save(nArdStatusFg);
                nIotStatusFg = nArdStatusFg;
            }
        }
    }
}

//Funcoes que buscam valores na adafruit

//Status alarme
void getStatusAlarme(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusAlr = valRecebido.toInt();
    if (nOldIotStatusAlr == 3) {
        nOldIotStatusAlr = nIotStatusPrt;
    }
}

//Status da porta
void getStatusPorta(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusPrt = valRecebido.toInt();
}


//Status da fogo
void getStatusFogo(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusFg = valRecebido.toInt();
}

//Status da lampada 1
void getStatusLp1(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusLp1 = valRecebido.toInt();
    if (nOldIotStatusLp1 == 3) {
        nOldIotStatusLp1 = nIotStatusLp1;
    }
}


//Status da lampada 2
void getStatusLp2(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusLp2 = valRecebido.toInt();
    if (nOldIotStatusLp2 == 3) {
        nOldIotStatusLp2 = nIotStatusLp2;
    }
}


//Status da lampada 3
void getStatusLp3(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusLp3 = valRecebido.toInt();
    if (nOldIotStatusLp3 == 3) {
        nOldIotStatusLp3 = nIotStatusLp3;
    }
}

//Status da lampada 1
void getStatusLp4(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusLp4 = valRecebido.toInt();
    if (nOldIotStatusLp4 == 3) {
        nOldIotStatusLp4 = nIotStatusLp4;
    }
}

//Status da lampada 1
void getStatusLp5(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusLp5 = valRecebido.toInt();
    if (nOldIotStatusLp5 == 3) {
        nOldIotStatusLp5 = nIotStatusLp5;
    }
}

//Status da lampada 6
void getStatusLp6(AdafruitIO_Data *data) {
    String valRecebido = data->value();
    nIotStatusLp6 = valRecebido.toInt();
    if (nOldIotStatusLp6 == 3) {
        nOldIotStatusLp6 = nIotStatusLp6;
    }
}
