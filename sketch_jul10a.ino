//через дифайн мы создаем константы
//без точки с запятой
#define SCLK 7 // white 
#define RCLK 6 //red
#define DIO 5  //green

byte digitBuffer[4];

void setup() {
  // put your setup code here, to run once:
  pinMode(RCLK, OUTPUT);  //ставим номера пинов на вывод
  pinMode(SCLK, OUTPUT);
  pinMode(DIO, OUTPUT);

  Serial.begin(9600); //нужно подключать, если хотим получать потом вывод в терминал 
}

void loop() {
  // заполняем массив  
  while (true) {
    long int set0 = 1600;
    bool dot = true;
    for(long int j = set0; j >= 0; j--){
        long int HH = j / 3600;
        long int mins = j/60;
        digitBuffer[0] = mins / 10 % 10;
        if(dot == true){ // для постановки точки
          digitBuffer[1] = (mins % 10)+10;
          dot = false;
        }
        else{
          digitBuffer[1] = mins % 10;
          dot = true;
        }
        long int secs = j - ((HH * 3600) + (((j - (HH*3600))/60)*60));
        digitBuffer[2] = secs / 10 % 10;
        digitBuffer[3] = secs % 10;
        showDisplay();
      }
  }

  // однократно выводим данные из массива на 7сигмент
  // тоесть функция должна постоянно крутится
}

void showDisplay() {

  const byte digit[20] = { // маска для 7 сигментного индикатора
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000, // 9 
    0b01000000, // 0.
    0b01111001, // 1.
    0b00100100, // 2.
    0b00110000, // 3.
    0b00011001, // 4.
    0b00010010, // 5.
    0b00000010, // 6.
    0b01111000, // 7.
    0b00000000, // 8.
    0b00010000 // 9.
  };

  const byte chr[5] = { // маска для разряда
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001,
    0b00000000
  };

  // отправляем в цикле по два байта в сдвиговые регистры
  int a = 0;
  while (a <= 30) {
    for (byte i = 0; i <= 4; i++) {
      digitalWrite(RCLK, LOW); // открываем защелку
      if (i < 4) {
        if(i == 1){
          
        }
        shiftOut(DIO, SCLK, MSBFIRST, digit[digitBuffer[i]]); // отправляем байт с "числом"
      }
      shiftOut(DIO, SCLK, MSBFIRST, chr[i]); // включаем разряд
      digitalWrite(RCLK, HIGH); // защелкиваем регистры
      delay(1); // ждем немного перед отправкой следующего "числа"
    }
    a++;
  }
}
