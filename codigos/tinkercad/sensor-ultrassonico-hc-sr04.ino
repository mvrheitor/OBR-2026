void setup()
{
  Serial.begin(9600);
  DDRD = 0B10000000;
  // TRIG --> Pino 7
  // ECHO --> Pino 6

  DDRB = 0B00100000;
  // Led do arduino
}

int estado_sensor = 0;
// 0: já retornou. precisa disparar/medir de novo
// 1: disparou. verifica se o echo ligou
// 2: echo ligou. verifica se o echo desligou

float disparo = 0;
float retorno_disparo = 0;
float tempo_sinal = 0;

float distancia = 0;

void loop()
{
  if(estado_sensor==0){
    PORTD |= 0B10000000;
    delayMicroseconds(10);
    PORTD &= 0B01111111;

    estado_sensor = 1;
  }

  else if(estado_sensor==1){
    if((PIND & 0B01000000) != 0){
      disparo = micros();

      estado_sensor = 2;
    }
  }

  else if(estado_sensor==2){
    if((PIND & 0B01000000) == 0){
      retorno_disparo = micros();

        tempo_sinal = retorno_disparo - disparo;
  
        // O som viaja 0.034cm a cada microsegundos
        // Distância = tempo do sinal / 2 pois o tempo é de ida e volta
        distancia = tempo_sinal/2 * 0.034;

        Serial.print("Distância: ");
        Serial.print(distancia);
        Serial.println("cm");

        // Captação máxima do sensor do tinkercad = 314,5cm
        // Se distância for menor --> detectou algo --> liga o led
        if(distancia < 314){
          PORTB |= 0B00100000;
        }
        else{
          PORTB &= 0B11011111;
        }
      estado_sensor = 0;
    }
  }
}
