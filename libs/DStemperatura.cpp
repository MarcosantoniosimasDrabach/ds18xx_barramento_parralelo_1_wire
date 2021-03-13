//Re: Duvida sobre a diferença entre " " e <> no #INCLUDE.
//Regra geral o uso das <> indica que a biblioteca está na pasta das bibliotecas definida pelo compilador.
//Quando se usa "", a biblioteca está na pasta raíz do projecto.
//ATENÇÃO DEIXE DESABILITADO AVISO DE COMPILADOR EVITA MUITOS PROBLEMAS 
//TIVE PROBLEMAS DE REGRAS C++ FOI NECESSÁRIO DESABILITAR PARA VOLTA A FUNCIONA O PROJETO
#include "DStemperatura.h"
#include "Wparametro.h"

/*O acesso ao DS18B20
A sequência para o acesso é a seguinte:
Inicialização;
Comando ROM (seguido de uma troca qualquer de dados quando solicitados);
Comando Função (seguido de uma troca qualquer de dados quando solicitados).
*/
//class::Metodo(uint8_t pin) ":
DStemperatura::~DStemperatura(){/*destrutor*/}
DStemperatura::DStemperatura(uint8_t pin) : // métrodos definos como erança do onewire wire-1
    //OneWire="em Português! Um fio" chamado de MicroLan 
    umfio(OneWire(pin)),//inicia a instância wire-1 passa por parametro no construtor c++
     QtdDespositivos(0),//primero mettodo herdado do um-fio 
    globalResolucao(0),//segundo metodo herdado 
    selectedResolution(0),//terceiro metodo herdado 
    selectedPowerMode(0)//ultimo meto herdado 
{
	//carrega automaticamento ao chamar o metodo construtor 
    resetSearch();//reseta o slave 
    sendCommand(SKIP_ROM, READ_POWER_SUPPLY);//lê rom e checa o tipo de alimentação no barramento 
    globalPowerMode = umfio.read_bit();//retona os tipos de alimentação no barramento 

    while (SelecionaProxSensor()) {//seleciona todos os despositos no barramento 
        uint8_t resolucao = ObterResolucao();//carrega a resolução 

        if (resolucao > globalResolucao) {//compara se a resolução global e menor então atualiza
            globalResolucao = resolucao;
        }

         QtdDespositivos++;
    }
}
//metodos declarado como publicos 
uint8_t DStemperatura::select(uint8_t address[]) {
    if (isConnected(address)) {//verefica se esta conectado 
      //conversor de ponto flutuante  memcpy (20,0) em formato hexadecimal:
        memcpy(selectedAddress, address, 8);//copia para vetor esse selectedAddress para address

        if (readScratchpad()) {
            selectedResolution = ObterResolucao();

            sendCommand(MATCH_ROM, READ_POWER_SUPPLY);
            selectedPowerMode = umfio.read_bit();

            return 1;
        }
    }

    return 0;
}
String DStemperatura::ObterNumeroDespositos(){
  return  String(QtdDespositivos);
}
uint8_t DS18xx::SelecionaProxSensor(){
    if (oneWireSearch(SEARCH_ROM)) {
        return select(searchAddress);
    }

    return 0;
}

uint8_t DStemperatura::selectNextAlarm() {
    if (oneWireSearch(ALARM_SEARCH)) {
        return select(searchAddress);
    }

    return 0;
}

void DStemperatura::resetSearch() {
    lastDiscrepancy = 0;
    lastDevice = 0;
}

float DStemperatura::ObterTemperaturaCelcius() {
    sendCommand(MATCH_ROM, CONVERT_T, !selectedPowerMode);
    delayForConversion(selectedResolution, selectedPowerMode);
    readScratchpad();
    uint8_t lsb = selectedScratchpad[TEMP_LSB];
    uint8_t msb = selectedScratchpad[TEMP_MSB];

    switch (selectedResolution) {
        case 9:
            lsb &= 0xF8;
            break;
        case 10:
            lsb &= 0xFC;
            break;
        case 11:
            lsb &= 0xFE;
            break;
    }

    uint8_t sign = msb & 0x80;
    int16_t temp = (msb << 8) + lsb;

    if (sign) {
        temp = ((temp ^ 0xffff) + 1) * -1;
    }

    return temp / 16.0;
}

float DStemperatura::ObterTemperaturaFahrenheit() {
  return ObterTemperaturaCelcius() * 1.8 + 32;
}

uint8_t DStemperatura::ObterResolucao() {
    switch (selectedScratchpad[CONFIGURATION]) {
        case RES_9_BIT:
            return 9;
        case RES_10_BIT:
            return 10;
        case RES_11_BIT:
            return 11;
        case RES_12_BIT:
            return 12;
    }
}

void DStemperatura::setResolution(uint8_t resolucao) {
  //limita p renge de valor constrain(resolucao, 9, 12); entre 9 e 12
    resolucao= constrain(resolucao, MinValue, MaxValue);

    switch (resolucao) {
        case 9:
            selectedScratchpad[CONFIGURATION] = RES_9_BIT;
            break;
        case 10:
            selectedScratchpad[CONFIGURATION] = RES_10_BIT;
            break;
        case 11:
            selectedScratchpad[CONFIGURATION] = RES_11_BIT;
            break;
        case 12:
            selectedScratchpad[CONFIGURATION] = RES_12_BIT;
            break;
    }

    if (resolucao > globalResolucao) {
       globalResolucao  = resolucao;
    }

    writeScratchpad();
}
//metodo que retorna um uint8_t (byte)
uint8_t DStemperatura::ObterModoPower(){
    return selectedPowerMode;
}//metodo que rtorna uma string 

String DStemperatura::ObterModoPowerString(){
  //variavel local é importante para economizar memoria 
   uint8_t Modo_Power = this->ObterModoPower();
   if( Modo_Power)
   {
    return MSG_MODO_POWER_EXTERNO;
   }else
   {
   return MSG_MODO_POWER_PARASITA;
   }
}//metodo que rtorna uma string 
//obeter codigo da familia do chip
uint8_t DStemperatura::ObterCodigoFamilia(){
    return selectedAddress[0];
}

 
String  DStemperatura::ObterCodigoFamiliaString() {
   uint8_t Modelo_ds18xx = this->ObterCodigoFamilia();
    switch (Modelo_ds18xx) {//seleciona pelo código número da familia 
      case MODEL_DS18S20://Model_ds18b20 é  uma constante pre-definido no projeto da lib 
         return   MSG_MODEL_DS18S20; ////Contante definido na parametrods
        break;
      case MODEL_DS1822://Model_ds18b20 é  uma constante pre-definido no projeto da lib 
         return  MSG_MODEL_DS1822;////Contante definido na parametrods
        break;
      case MODEL_DS18B20://Model_ds18b20 é  uma constante pre-definido no projeto da lib 
         return  MSG_MODEL_DS18B20;//Contante definido na parametrods
        break;
      default://se não for verdadadeiros as condisões acima então passa aqui 
         return MSG_MODEL_DS1820_DEFAUL;
        break;
    }
 
}
void DStemperatura::ObterEnderecoFisico(uint8_t Endereco[]) {
    memcpy(Endereco, selectedAddress, 8);//copia o vetor para o outro 
}
uint8_t* DStemperatura::ArrayObterEnderecoFisico(uint8_t Endereco[]) {
    memcpy(Endereco, selectedAddress, 8);//copia o vetor para o outro 
   return Endereco;
}
String  DStemperatura::DecimalObterEnderecoFisico(uint8_t Endereco[]) {
  String Array="";
    memcpy(Endereco, selectedAddress, 8);//copia o vetor para o outro 
   for(int i=0; i <8;i++){ 
     Array+=" ";
     Array+= Endereco[i];
    }
    return Array;
}
void  DStemperatura::PrintObterEnderecoFisico(uint8_t Endereco[]) {
    memcpy(Endereco, selectedAddress, 8);//copia o vetor para o outro 
      for (uint8_t i = 0; i < 8; i++) {
      Serial.print(" ");
      Serial.print(Endereco[i],HEX);//imprime o código do chip no terminal 
    }
    Serial.println();
}


 

void DStemperatura::getAddress(uint8_t address[]) {
    memcpy(address, selectedAddress, 8);//copia o vetor para o outro 
}


void DStemperatura::TConvercao() {
    sendCommand(SKIP_ROM, CONVERT_T, !globalPowerMode);
    delayForConversion(globalResolucao, globalPowerMode);
}
//lê o número de despositos código identificador único para cada chip no barramento 1-wire 
uint8_t DStemperatura::getNumberOfDevices() {
    return  QtdDespositivos;
}
uint8_t DStemperatura::ObterQtdDespositivos()
{
  return  this->getNumberOfDevices();
}
//rotina que verifica se ha estouro do alarme 
uint8_t DStemperatura::hasAlarm() {
    uint8_t oldResolution = selectedResolution;
    setResolution(9);
    float temp = ObterTemperaturaCelcius();
    setResolution(oldResolution);
    return ((temp <= selectedScratchpad[ALARM_LOW]) || (temp >= selectedScratchpad[ALARM_HIGH]));
}
//setar o valor de setpoint de renge minimo e maximo do alarme exemplo para refrigeração
//liga gando cai a temperatura e desliga quando atinge o maximo da temperatura
void DStemperatura::setAlarms(int8_t alarmLow, int8_t alarmHigh) {
    setAlarmLow(alarmLow);
    setAlarmHigh(alarmHigh);
    writeScratchpad();
}
//lê o valor do byte TL do registrador de alarme do chip ds18xxx
int8_t DStemperatura::getAlarmLow() {
    return selectedScratchpad[ALARM_LOW];
}
//seta  o valor do byte TL do registrador de alarme do chip ds18xxx
void DStemperatura::setAlarmLow(int8_t alarmLow) {
    alarmLow = constrain(alarmLow, -55, 125);
    selectedScratchpad[ALARM_LOW] = alarmLow;
    writeScratchpad();
}
//lê o valor do byte TH do registrador de alarme do chip ds18xxx
int8_t DStemperatura::getAlarmHigh() {
    return selectedScratchpad[ALARM_HIGH];
}
//seta o valor do byte TH do registrador de alarme do chip ds18xxx
void DStemperatura::setAlarmHigh(int8_t alarmHigh) {
    alarmHigh = constrain(alarmHigh, -55, 125);
    selectedScratchpad[ALARM_HIGH] = alarmHigh;
    writeScratchpad();
}
//setar  o valor do byte TLe byte tH do registrador de alarme do chip ds18xxx
void DStemperatura::setRegisters(int8_t lowRegister, int8_t highRegister) {
    setAlarms(lowRegister, highRegister);
}
//lê o valor do byte TL do registrador de alarme do chip ds18xxx lsb 
int8_t DStemperatura::getLowRegister() {
    return getAlarmLow();
}
//setar o valor do byte TL do registrador de alarme do chip ds18xxx LSB 
void DStemperatura::setLowRegister(int8_t lowRegister) {
    setAlarmLow(lowRegister);
}
//lê o valor do byte TH do registrador de alarme do chip ds18xxx LSM
int8_t DStemperatura::getHighRegister() {
    return getAlarmHigh();
}
//SETA  o valor do byte TH do registrador de alarme do chip ds18xxx LSM 
void DStemperatura::setHighRegister(int8_t highRegister) {
    setAlarmHigh(highRegister);
}
//lê o registrador de trabalho Scratchpad
uint8_t DStemperatura::readScratchpad() {
    sendCommand(MATCH_ROM, READ_SCRATCHPAD);

    for (uint8_t i = 0; i < SIZE_SCRATCHPAD; i++) {
        selectedScratchpad[i] = umfio.read();
    }
    //chamda da class nativa onewire ( onewire = (um fio) "MicroLan")UmFio
    return OneWire::crc8(selectedScratchpad, 8) == selectedScratchpad[CRC8];
}
//escreve no registrador de trabalho Scratchpad
void DStemperatura::writeScratchpad() {
    sendCommand(MATCH_ROM, WRITE_SCRATCHPAD);
  umfio.write(selectedScratchpad[ALARM_HIGH]);
   umfio.write(selectedScratchpad[ALARM_LOW]);
    umfio.write(selectedScratchpad[CONFIGURATION]);
    sendCommand(MATCH_ROM, COPY_SCRATCHPAD, !selectedPowerMode);

    if (!selectedPowerMode) {
        delay(10);
    }
}
//envia somente o comando 
uint8_t DStemperatura::sendCommand(uint8_t romCommand) {
    if (!umfio.reset()) {
        return 0;
    }

    switch (romCommand) {
        case SEARCH_ROM:
        case SKIP_ROM:
        case ALARM_SEARCH:
          umfio.write(romCommand);
            break;
        case MATCH_ROM:
            umfio.select(selectedAddress);
            break;
        default:
            return 0;
    }

    return 1;
}
//envia comando função e modo de aliemtação  para despositivo metodo  sobre caregado 
uint8_t DStemperatura::sendCommand(uint8_t romCommand, uint8_t functionCommand, uint8_t power) {
    if (!sendCommand(romCommand)) {
        return 0;
    }

    switch (functionCommand) {
        case CONVERT_T:
        case COPY_SCRATCHPAD:
            umfio.write(functionCommand, power);
            break;
        case WRITE_SCRATCHPAD:
        case READ_SCRATCHPAD:
        case READ_POWER_SUPPLY:
            umfio.write(functionCommand);
            break;
        default:
            return 0;
    }

    return 1;
}
//procura um despositivo para enviar comando 
uint8_t DStemperatura::oneWireSearch(uint8_t romCommand) {
    if (lastDevice || !sendCommand(romCommand)) {
        resetSearch();
        return 0;
    }

    uint8_t lastZero = 0;
    uint8_t direction, byteNumber, bitNumber, currentBit, currentBitComp;

    for (uint8_t bitPosition = 0; bitPosition < 64; bitPosition++) {
        currentBit = umfio.read_bit();
        currentBitComp = umfio.read_bit();

        if (currentBit && currentBitComp) {
            lastDiscrepancy = 0;
            return 0;
        }

        byteNumber = bitPosition / 8;
        bitNumber = bitPosition % 8;

        if (!currentBit && !currentBitComp) {
            if (bitPosition == lastDiscrepancy) {
                direction = 1;
            } else if (bitPosition > lastDiscrepancy) {
                direction = 0;
                lastZero = bitPosition;
            } else {
                direction = bitRead(searchAddress[byteNumber], bitNumber);

                if (!direction) {
                    lastZero = bitPosition;
                }
            }
        } else {
            direction = currentBit;
        }

        bitWrite(searchAddress[byteNumber], bitNumber, direction);
        umfio.write_bit(direction);
    }

    lastDiscrepancy = lastZero;

    if (!lastDiscrepancy) {
        lastDevice = 1;
    }

    return 1;
}
//verifica se está conecta  ao  barramento 
uint8_t DStemperatura::isConnected(uint8_t address[]) {
    if (!sendCommand(SEARCH_ROM)) {
        return 0;
    }

    uint8_t currentBit, currentBitComp, byteNumber, bitNumber;

    for (uint8_t bitPosition = 0; bitPosition < 64; bitPosition++) {
        currentBit = umfio.read_bit();
        currentBitComp =umfio.read_bit();

        if (currentBit && currentBitComp) {
            return 0;
        }

        byteNumber = bitPosition / 8;
        bitNumber = bitPosition % 8;
      umfio.write_bit(bitRead(address[byteNumber], bitNumber));
    }

    return 1;
}
//tempo necessário para cada conversão a realizar conforme o tipo de alimentação no barramento
void DStemperatura::delayForConversion(uint8_t resolucao, uint8_t powerMode) {
    if (powerMode) {
        while (!umfio.read_bit());
    } else {
        switch (resolucao) {
            case 9:
                delay(CONV_TIME_9_BIT);
                break;
            case 10:
                delay(CONV_TIME_10_BIT);
                break;
            case 11:
                delay(CONV_TIME_11_BIT);
                break;
            case 12:
                delay(CONV_TIME_12_BIT);
                break;
        }
    }
}


