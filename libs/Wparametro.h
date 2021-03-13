//ATENÇÃO DEIXE DESABILITADO AVISO DE COMPILADOR EVITA MUITOS PROBLEMAS 
//TIVE PROBLEMAS DE REGRAS C++ FOI NECESSÁRIO DESABILITAR PARA VOLTA A FUNCIONA O PROJETO
//configuração de constantes para trabalha com ds18b20
//LIB DS18B20 COMPLETO 
//DATA: 03.03.2021
//DESENVOLDOR: MARCOS ANTÔNIO SIMAS DRABACH
//MACRO DE DEFINIÇÃO PARA OPERAÇÃO DS18B20
#ifndef Wparametro_H
#define Wparametro_H

//Os comandos ROM são os seguintes:
#define SEARCH_ROM 0xF0//////////////
#define READ_ROM 0x33////////////////
#define MATCH_ROM 0x55///////////////
#define SKIP_ROM 0xCC////////////////
#define ALARM_SEARCH 0xEC////////////
/////////////////////////////////////
//Os Comandos de Função são os seguintes:
#define CONVERT_T 0x44////////////////
#define WRITE_SCRATCHPAD 0x4E/////////
#define READ_SCRATCHPAD 0xBE//////////
#define COPY_SCRATCHPAD 0x48//////////
#define RECALL 0xB8///////////////////
#define READ_POWER_SUPPLY 0xB4////////
//////////////////////////////////////
//Código definido pelo fabricante para cada familia de chip
#define MODEL_DS1820 0x10/////////////
#define MODEL_DS18S20 0x10////////////
#define MODEL_DS1822 0x22/////////////
#define MODEL_DS18B20 0x28////////////
//////////////////////////////////////
//definições de constantes para operações com os registradores do chip ds18xxxx
#define SIZE_SCRATCHPAD 9////////////tamanho de bytes da ROM que seria 64bits
#define TEMP_LSB 0///////////////////byte 0 do registrador LSB
#define TEMP_MSB 1///////////////////byte 1 do registrador MSB
#define ALARM_HIGH 2/////////////////byte  TH  nível alto 
#define ALARM_LOW 3//////////////////byte TL nível baixo 
#define CONFIGURATION 4//////////////mascara de filtro para definir a resolução da configuração para conversão AD
#define CRC8 8//////////////////////Constante que define o tamnaho do crc para validação da temperatura 
//definição de constante para inicializar a configuração das resoluções defino pelo fabricante do chip 
#define RES_9_BIT 0x1F///////////////constante hex que define a resolução 9 bits
#define RES_10_BIT 0x3F//////////////constante hex que define a resolução 10 bits
#define RES_11_BIT 0x5F//////////////constante hex que define a resolução 11 bits
#define RES_12_BIT 0x7F//////////////constante hex que define a resolução 12 bits
//definição de constante para tempo para conversão para cada resolução no seu projeto 
#define CONV_TIME_9_BIT 94/////////// tempo converção 93.75ms
#define CONV_TIME_10_BIT 188/////////tempo converção 187.55ms
#define CONV_TIME_11_BIT 375/////////tempo converção 375ms
#define CONV_TIME_12_BIT 750/////////tempo converção 750ms
//definições de parametros para msg sistema 
#define MSG_MODEL_DS18S20  "Modelo: DS18S20/DS1820"
#define MSG_MODEL_DS1822  "Modelo: DS1822"
#define MSG_MODEL_DS18B20 "Modelo: DS18B22"
#define MSG_MODEL_DS1820_DEFAUL "Despositivo Desconhecido"
#define MSG_MODO_POWER_EXTERNO "Externo Alimentação";
#define MSG_MODO_POWER_PARASITA "Parasita Alimentação";
///padroes de configurações 
#define MinValue  9/////////////////////
#define  MaxValue 12/////////////
#endif
/////////////////////////////////////
/*
Inicialização do DS18B20
Todas as comunicações sobre o barramento 1-Wire iniciam com uma sequência de inicialização. 
A sequência de inicialização é constituída de um impulso de reset transmitido pelo mestre e seguido de um impulso de presença transmitido pelos escravos. 
O impulso de presença permite ao mestre saber que dispositivos escravos presentes no barramento estão prontos a operar.
O acesso ao DS18B20
A sequência para o acesso é a seguinte:

Inicialização;
Comando ROM (seguido de uma troca qualquer de dados quando solicitados);
Comando Função (seguido de uma troca qualquer de dados quando solicitados).
É muito importante seguir esta sequência em cada comunicação, visto que o DS18B20 não responde se um passo da sequência faltar. 
As exceções a esta regra são os comando Busca ROM [F0h] e Busca Alarme [ECh]. Após a emissão de um desses comandos, o mestre deve retornar ao passo 1 da sequência.

Medição de temperatura com o DS18B20
A funcionalidade básica do DS18B20 é a medição de temperatura com saída diretamente em digital. 
A resolução do sensor de temperatura é configurada pelo usuário em 9, 10, 11 e 12 bits, correspondendo 
a incrementos 0,5 °C, 0.25 °C, 0.125 °C, e 0,0625 °C por bit respectivamente.
Comandos ROM
Após o mestre liberar o impulso de presença, poderá enviar um comando ROM.
Esses comandos permitem identificar um dispositivo específico (caso estejam presentes mais de um no barramento) determinando
também quantos e quais dispositivos estão ligados ou se um dispositivo possui registro de alarme ativo. Existem cinco comandos ROM, e cada comando possui 8 bits.
O mestre deve enviar um comando ROM apropriado antes de um comando de função. Um diagrama de fluxo do funcionamento dos comandos ROM é mostrado no anexo A.

Os comandos ROM são os seguintes:
Busca ROM [F0h]
Quando um sistema é acessado inicialmente, o mestre deve identificar os códigos ROM de todos os dispositivos escravos no barramento,
para determinar o número de escravos e tipos de dispositivos. O mestre descobre os códigos ROM através de um processo
de eliminação que exige um ciclo de Busca ROM (isso é, de um comando Busca ROM seguido de uma troca de dados)
o número de vezes necessário para identificar todos os dispositivos presentes.

Se apenas um dispositivo está presente no barramento, o comando Lê ROM pode ser utilizado no lugar do comando Busca ROM.

Uma explicação completa do procedimento de Busca ROM está contido no documento iButton® no Standards em www.maxim-ic.com/ibuttonbook.

Após cada ciclo de Busca ROM, o mestre deve retornar ao passo 1 (inicialização) na sequência da transação.

Lê ROM [33h]
Este comando pode ser utilizado apenas quando existe apenas um escravo no barramento. Permite ao mestre do barramento ler o código ROM de 64 bits
do escravo sem utilizar o procedimento de Busca ROM.
Se este comando é utilizado quando estão presentes mais de um dispositivo no barramento, uma colisão
de dados irá acontecer pois todos os dispositivos irão responder ao mesmo tempo.

Match ROM [55h]
O comando seguido de uma sequência de 64 bits de código ROM permite ao mestre comunicar com um dispositivo específico quando se tem um barramento multidrop  ou single-loop.
Apenas o escravo que corresponde exatamente a sequência transmitida responderá ao comando função enviado
pelo mestre; todos os outros dispositivos no barramento aguardarão um impulso de reset.

Skip ROM [CCh]
O mestre pode utilizar este comando para acessar todos os dispositivos do barramento simultaneamente sem enviar nenhuma informação de código ROM.
Por exemplo, o mestre pode ordenar a todos DS18B20 do barramento a executar uma conversão
de temperatura simultaneamente enviando um comando SKIP ROM seguido de um comando Converter T [44h].

Observe-se que o comando Lê Scratchpad [BEh] pode seguir o comando Skip ROM apenas se existir apenas um dispositivo escravo no barramento. 
Nesse caso, se poupa tempo permitindo ao mestre ler do escravo sem ter de enviar um código ROM de 64 bits. Um comando Skip ROM seguido de um
comando de leitura scratchpad causará uma colisão de dados no barramento se existirem mais de um dispositivo escravo transmitindo dados ao mesmo tempo.

Busca Alarme [ECh]
O funcionamento deste comando é idêntico ao funcionamento do comando Busca ROM, salvo ao fato que responderão apenas os escravos com um flag de alarme ativo.
Este comando permite ao mestre mapear os eventuais DS18B20 que tenham sinalizado uma condição de alarme seguido a conversão de temperatura mais recente. 
Após cada ciclo de Busca Alarme, o mestre deve retornar ao passo 1 (inicialização) na sequência de transações.

Comandos de Função
Após o mestre do barramento ter utilizado um Comando ROM para acessar um DS18B20 com o qual deseja comunicar-se, o mestre pode enviar um comando função.
Esses comandos permitem ao mestre escrever e ler da memória scratchpad do DS18B20, iniciar conversões de temperatura e determinar o modo de alimentação.
Os comandos função descritos a seguir tem o diagrama de fluxo apresentado no anexo B.
O registro de configuração do DS18B20
O byte 4 da memória scratchpad contém o registro de configuração, que é organizado como segue:
O usuário pode ajustar a resolução de conversão utilizando os bits R0 e R1 neste registro.
Os Comandos de Função são os seguintes:

Convert T [44h]
Este comando inicia uma conversão única de temperatura. Após a conversão, o dado resultante é memorizado 
no registro de temperatura da memória scratchpad e o DS18B20 retorna ao estado de inatividade em baixo consumo.

Cuidados com a alimentação:

Se o dispositivo estiver utilizando a modalidade de alimentação “parasita”, dentro de  10μs (máx) após o envio deste comando, 
o mestre deve ativar um pull-up forte no barramento 1-Wire pelo tempo que durar a conversão (tCONV).
Se o DS18B20 é alimentado por uma fonte externa, o mestre pode enviar em intervalos de tempo, impulsos de leitura após o comando Convert T e o 
DS18B20 responderá transmitindo 0 se a conversão da temperatura estiver em curso e 1 quando a conversão for concluída.
Em modalidade de alimentação “parasita” esta técnica de comunicação não pode ser utilizada enquanto o barramento está ligado ao VCC pelo pull-up forte durante a conversão.
Write Scratchpad [4Eh]
Este comando permite ao mestre escrever 3 bytes de dados no scratchpad do DS18B20.
O primeiro byte dos dados é escrito no registro TH (byte 2 do Scratchpad), o segundo byte é escrito no registro TL (byte 3),
e o terceiro byte é escrito no registro de configuração (byte 4).
Os bytes devem ser transmitidos iniciando pelo bit menos significativo. A escrita dos três bytes
deve ser completada antes de um reset do barramento ou os dados poderão ser corrompidos.

Read Scratchpad [BEh]
Este comando permite ao mestre ler o conteúdo do scratchpad.
A transferência dos dados inicia con o bit menos significativo do byte 0 e continua até o nono byte ( 8 bytes + CRC). 
o mestre pode enviar um reset para interromper a leitura a qualquer momento, se for necessária apenas parte dos dados.

Copy Scratchpad [48h]
Este comando copia o conteúdo dos bytes TH, TL e registro de configuração (byte 2, 3 e 4) do scratchpad para a memória EEPROM.
Se o dispositivo utiliza a alimentação “parasita”, dentro de 10μs (máx) após a emissão deste comando, o mestre deve ativar o pull-up forte sobre o barramento 1-Wire por 10 ms (min).

Recall E2 [B8h]
Este comando solicita os valores de alarme (TH e TL) e dados de configuração da memória EEPROM para o scratchpad. O dispositivo mestre
pode enviar a intervalos de tempo, solicitações de leitura, e o DS18B20 indica o estado transmitindo 0 enquanto a solicitação estiver em curso e 1 quando a mesma for concluída.
A operação de solicitação acontece automaticamente na energização, então os dados validos são disponíveis no scratchpad assim que o dispositivo é alimentado.

Read Power Supply [B4H]
Este comando permite ao mestre saber se algum DS18B20 no barramento utiliza alimentação “parasita”. Durante o intervalo de tempo de leitura,
os DS18B20 alimentados pelo modo “parasita” levarão o barramento para nível baixo, enquanto que aqueles alimentados com fonte externa deixarão
o barramento em nível alto. Isto serve para identificar quando é necessário ativar o pull-up forte.
O bit 7 e os bits 0 a 4 do registro de configuração são reservados para uso interno do dispositivo.
Exemplos de comunicação entre mestre e DS18B20
Este é o procedimento no caso de existirem mais de um DS18B20 no barramento:


Mestre      Dado  Função 
Tx  Reset O mestre envia um impulso de reset
Rx  Presença  DS18B20 responde com um impulso de presença
Tx  55h O mestre envia o comando Match ROM
Tx  64 bit ROM code O mestre envia o código ROM do dispositivo desejado
Tx  44h O mestre envia o comando Convert T
Tx  DQ alto E’ solicitado o pull-up se existirem dispositivos com alimentação parasita
Tx  Reset O mestre envia um impulso de reset
Rx  Presença  DS18B20 responde com um impulso de presença
Tx  55h O mestre envia o comando Match ROM
Tx  64 bit ROM code O mestre envia o código ROM do dispositivo desejado
Tx  BEh O mestre envia o comando Read Scratchpad
Rx  9 data bytes  O escravo envia o conteúdo do scratchpad + CRC
É tarefa do mestre verificar se o CRC recebido está correto.

Se existe apenas um DS18B20 no barramento e são solicitados os registros internos:

Mestre    Dado  Função
Tx  Reset O mestre envia um impulso de reset
Rx  Presença  DS18B20 responde com um impulso de presença
Tx  CCh O mestre envia um comando Skip ROM
Tx  4Eh O mestre envia um comando Write Scratchpad
Tx  3 bytes O mestre envia os 3 bytes para o scratchpad
Tx  Reset O mestre envia impulso de reset
Rx  Presença  DS18B20 responde com o impulso de presença
Tx  CCh O mestre envia o comando Skip ROM
Tx  BEh O mestre envia o comando Read Scratchpad
Rx  9 data bytes  O escravo envia o conteúdo do scratchpad junto com o CRC
Tx  Reset O mestre envia o impulso de reset
Rx  Presença  DS18B20 responde com o impulso de presença
Tx  CCh O mestre envia o comando Skip ROM
Tx  48h O mestre envia o comando Copy Scratchpad
Tx  DQ alto É necessário o pull-up o DS18B20 a alimentação é “parasita”
Nota:

É tarefa do mestre verificar se o CRC recebido está correto e tomar a ação necessária.
Durante o ajuste dos registros internos, no seguimento do comando Write Scratchpad devem ser transmitidos os três bytes, mesmo que os alarmes não sejam necessários a aplicação.
CRC
O código ROM CRC é calculado sobre os primeiros 56 bits do código ROM e é armazenado no byte mais significativo da memória ROM. O código CRC scratchpad é calculado sobre os dados memorizados no scratchpad e, por consequência, é alterado quando os dados no scratchpad  mudam.
*/

//definição mais sismples 
/*
 * DS18B20 interno = ROM de 64 bits (código de série do endereço) + scratchpad de 9 bytes.

O registro de 9 bytes inclui: sensor de temperatura, alarme de alta temperatura TH de disparo de limite superior, alarme de temperatura baixa TL de limite inferior, registro de disparo de alta velocidade, gerador CRC de 8 bits.

O diagrama de estrutura de registro de 9 bytes é mostrado abaixo,

byte 0: temperatura LSB (50h)

byte 1: temperatura MSB (05h) EEPROM

byte 2: TH byte 1 definido pelo usuário <---> TH byte 1 do usuário

byte 3: byte 2 definido pelo usuário TL <---> byte 2 do usuário TL

byte 4: Registro de configuração <---> Registro de configuração

byte 5: bit reservado (FFh)

byte 6: bit reservado (0Ch)

byte 7: bit reservado (10h)

byte 8: bit de verificação CRC

Estrutura da ROM de 64 bits = CRC de 8 bits + número de série de 48 bits + código de série de 8 bits (gerado pelo gerador CRC).

5. Precisão de conversão

9 bits ： 0,5 ℃

10 bits ： 0,25

11 bits ： 0,125

12 bits: 0,0625 ° C (padrão)

Em segundo lugar, o uso de DS18B20

1. Etapas para iniciar a conversão de temperatura: redefinir, emitir o comando pular ROM (CCH), emitir o comando de conversão de temperatura inicial (44H).

2. O código usado para DS18B20 é o seguinte: (apenas para referência)
 */ 

