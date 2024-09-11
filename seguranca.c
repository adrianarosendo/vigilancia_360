#include <LiquidCrystal.h>
#include <Keypad.h>

LiquidCrystal lcd(13,12,14,15,16,17);
const byte ROWS = 4; 
const byte COLS = 4;
char keys[ROWS][COLS] = { 
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {8,7,6,9}; 
byte colPins[COLS] = {5,4,3,2}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char* password = "5938"; 
char nova_senha[4];
char senha[4] = {'5','9','3','8'}; // cria um vetor char de nome senha de 4 colunas com caracteres definidos

const int pinoPIR = 1; //PINO DIGITAL UTILIZADO PELO SENSOR DE PRESENÇA
const int pinoLED = 11; //PINO DIGITAL UTILIZADO PELO LED
const int pinoBUZZER = 10;
int position = 0;
int sensor = 0;
int leitura_sensor; // cria uma variavel int de nome leitura_sensor
int desliga_alarme; // cria uma variavel int de nome desliga_alarme
char vetor[4]; // cria um vetor char para armazenar os digitos do teclado
int i; // cria uma variavel de nome i

void setup(){
  Serial.begin(9600);
  pinMode(pinoLED, OUTPUT); //DEFINE O PINO COMO SAÍDA
  pinMode(pinoPIR, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(pinoBUZZER, OUTPUT);
  lcd.begin(16,2); // inicia a comunicação com o display lcd
  lcd.setCursor(0,0); 
  lcd.print("ALARME"); // escreve ALARME no display na posição (0,0)

}


void loop(){
  lcd.setCursor(8,0); 
  lcd.print("OFF"); // indica que o alarme está desabiitado no display

  char tecla_pressionada = keypad.getKey(); //  verifica se alguma tecla foi pressionada
 //código troca de senha aqui
 if(tecla_pressionada == 'A'){
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Troca de senha");  // Indica no display que a senha será trocada
    lcd.setCursor(0, 1); 
    lcd.print("Senha atual: ");   // Solicita a senha atual

    i = 0;  // Reseta o índice para garantir que comece do início
    while (i < 4) {
      tecla_pressionada = keypad.getKey();  // Captura a tecla pressionada

      if (tecla_pressionada) {
        vetor[i] = tecla_pressionada;  // Armazena a tecla pressionada no vetor
        lcd.setCursor(i + 12, 1);  // Mostra a tecla no LCD (após "Senha atual: ")
        lcd.print("*");  // Exibe um asterisco para ocultar a senha
        i++;  // Incrementa o contador para o próximo dígito
      }
    }

    // Verifica se a senha digitada corresponde à senha armazenada
    if (vetor[0] == senha[0] && vetor[1] == senha[1] && vetor[2] == senha[2] && vetor[3] == senha[3]) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha correta");
      delay(1000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nova senha:");  // Solicita nova senha
      i = 0;
      
      while (i < 4) {
        tecla_pressionada = keypad.getKey();  // Captura nova tecla pressionada

        if (tecla_pressionada) {
          nova_senha[i] = tecla_pressionada;  // Armazena a nova senha
          lcd.setCursor(i + 12, 1);  // Exibe a nova senha com asterisco
          lcd.print("*");
          i++;
        }
      }

      // Atualiza a senha com a nova
      for (int j = 0; j < 4; j++) {
        senha[j] = nova_senha[j];
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha alterada!");  // Confirma a alteração
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("ALARME");
      i = 0;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha incorreta!");
      delay(2000);
      lcd.clear();
    }
  }
     if (tecla_pressionada == '#' ) { // se # for pressionado executa o codigo abaixo
        lcd.setCursor(8,0); lcd.print("ON "); // indica no display que o alarme está ativo
        digitalWrite (pinoBUZZER, HIGH); delay(50);  // sonorização
              while ( tecla_pressionada != '*') { // enquanto * não for pressionado o programa executa no laço abaixo
        leitura_sensor = digitalRead(pinoPIR); // realiza a leitura de sinal advindo do sensor de vibrações
        tecla_pressionada = keypad.getKey(); // verifica se alguma tecla foi pressionada
        //******************************************ALARME
       if (leitura_sensor == HIGH) { // se receber sinais do sensor de vibração, executa o codigo abaixo
          analogWrite (pinoBUZZER, 130); delay(100); // sonorizaão alarme inicial
          analogWrite (pinoBUZZER, 150); delay(200); // sonorização alarme inicial
          analogWrite (pinoBUZZER, 255); delay(100); // sonorização alarme inicial
          analogWrite (pinoBUZZER, 100); delay(300); // sonorização alarme inicial
       while ( desliga_alarme != HIGH){ // enquanto o botão de desativador alternativo não for ativo, executa o laço abaixo
        digitalWrite (pinoLED ,HIGH); // liga o led rgb de alarme
        digitalWrite (pinoBUZZER, HIGH); delay(100); // Alarme
        digitalWrite (pinoBUZZER, LOW); delay(100); // Alarme
        tecla_pressionada = keypad.getKey(); // verifica se alguma tecla foi pressionada
        //******************************************SENHA
       if (tecla_pressionada){ 
        // se alguma tecla for pressionada executa abaixo
        vetor[i] = tecla_pressionada; // armazena no vetor[4] as teclas pressionadas
        lcd.setCursor(i,1); lcd.print(tecla_pressionada); // indica no display a tecla pressionada
        i++; // contador para definir que a senha seja de 4 digitos
          if(i==4){ // se o ultimo digito da senha for pressionado, executa abaixo
            if(vetor[0] == senha[0] &&
                vetor[1] == senha[1] &&
                vetor[2] == senha[2] &&
                vetor[3] == senha[3]){ // compara o vetor das teclas pressionadas com a senha pré definida
                lcd.setCursor(7,1); lcd.print("OK"); delay(1000); // indica que a senha digitada foi correta
                lcd.setCursor(0,1); lcd.print("                "); // limpa o display
                lcd.setCursor(6,0); lcd.print("                "); // limpa o display
                i=0; // zera o contador
                return 0;} // desativa o alarme retornando o codigo para o inicio
          else { // se a senha não for correta, executa abaixo
            lcd.setCursor(0,1); lcd.print("SENHA INVALIDA  "); digitalWrite (pinoBUZZER, HIGH); delay(1000); // indica no display que a senha digitada não é correta
            lcd.setCursor(0,1); lcd.print("                "); //limpa o display
            i=0;} //zera o contador
          }
}}}
//******************************************FIM DO ALARME
else { analogWrite(pinoBUZZER,0); // caso o alarme não estiver ativo, garante o buzzer desativado
}}}}
