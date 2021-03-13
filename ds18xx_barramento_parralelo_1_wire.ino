
//ATENÇÃO DEIXE DESABILITADO AVISO DE COMPILADOR EVITA MUITOS PROBLEMAS 
//TIVE PROBLEMAS DE REGRAS C++ FOI NECESSÁRIO DESABILITAR PARA VOLTA A FUNCIONA O PROJETO

/*PROJETO DE USO EDUCACIONAL PARA FINS DE CONHECIMENTO 
1-weri =(um fio ) == MicroLan varios chips família ds usa esse  
são padrões de comuncação so de um fio para comunição bi direcional entre master e escravos 
seria semelhante ao padrão I2C por em I2C é mais rapído em comunicação
exemplo:de aplicação de barramento em rede microlan topologia tipo T
essa framework visa facilitar o uso dos recursos do chip inteligente de temperatura 
já calibrado de fábrica.Isso lhe tras varios beneficios em aplicação profissional, pois o mesmo 
calibrado pronto para uso em projetos na vida real em sistemas embarcados e Internet das Coisa 
DESENVOLVIDO POR:MARCOOS ANTONIO SIMAS DRABACH
Data:05/03/2021*/
/*
Modelo: DS18B20
Endereço Físico: 28 E0 45 BC 13 19 1 21
Resolução: 12
Modo Alimetação: Externa
Temperatura: 27.12 ºC / 80.82 F

Modelo: DS18B20
Endereço Físico: 28 44 5 AA 13 19 1 79
Resolução: 12
Modo Alimetação: Externa
Temperatura: 28.00 ºC / 82.40 F

Modelo: DS18B20
Endereço Físico: 28 1C 62 A7 13 19 1 2B
Resolução: 12
Modo Alimetação: Externa
Temperatura: 26.56 ºC / 79.81 F

Modelo: DS18B20
Endereço Físico: 28 DD 82 A6 13 19 1 D6
Resolução: 12
Modo Alimetação: Externa
Temperatura: 27.12 ºC / 80.94 F

Modelo: DS18B20
Endereço Físico: 28 EB 54 B3 13 19 1 F2
Resolução: 12
Modo Alimetação: Externa
Temperatura: 27.00 ºC / 80.60 F
*/



//lib class de chip temperatura atende as regras de comunicação dos protocolos do chip 
#include "DStemperatura.h"
//INSTÂNCIA DE OBJETO QUE CARREGA NO METODO CONSTRUTOR O 
//GPIO DO BARRAMENTO CLASS=DS18B20 REFERENCIA OBJETO ds(pin 2)
DStemperatura ds(2);//a class em c++ é referenciada  instânciada inicializando o pin gpio  
uint8_t Endereco[8];//define um vetor de oito indice para carregar o código serial número do chip na rede 1-wire
//rotina que carrega toda inicialização do programa
void setup() {
  Serial.begin(115200);//inicia a configuração do usart com taxa counicação 960kps
  Serial.print("Despositivos: ");//ipreme a string no terminal 
  //obtem todos os depositivos no barramento identifica quantidades em números 
  Serial.println(ds.ObterQtdDespositivos());//imprime o número serial único de identificação do chip ds18xxx
  Serial.println();//próxima linha 
  //interface de operação facilita o usuário alto nível 
  //herança de DS18xx vs ds();
  
}


void loop() {
 // fica no loop até carregar todos os despositivos  do barramento repete infinitamente 
  uint8_t nDespositivos=0;
  while (ds.SelecionaProxSensor()) {//rotina que lista os chips no barramento  um fio MicroLan
    nDespositivos++;
    //metodo que apresenta modole de despositivos em barramento um fio MicroLan
    Serial.println(ds.ObterCodigoFamiliaString());
   //lê o código serial número de identificação único em cada chip ds18xxx
   Serial.print("Despositivo:");
    Serial.println(nDespositivos);
    Serial.print("Endereço Físico:");
    ds.PrintObterEnderecoFisico(Endereco);
    Serial.print("Resolução: ");//imprime  apresentação 
    Serial.println(ds.ObterResolucao());//Lê a resolução da memoria e2prom do chip ds18xxx

    Serial.print("Modo Alimetação: ");//imprime  apresentação 
    Serial.println(ds.ObterModoPowerString());
    Serial.print("Temperatura: ");
    Serial.print(ds.ObterTemperaturaCelcius());//Lê em grau celsius e retorna em ponto flutuante 
    Serial.print(" ºC / ");
    Serial.print(ds.ObterTemperaturaFahrenheit());//Lê e retorna para fahrenheit
    Serial.println(" F");
    Serial.println();
  }//fim do loop sai aguarda 1 segundo e retorna a fazer o clico novamente 

  delay(1000);//aguarda 1 segundo e retoma tudo novamente infinatamente 
}

