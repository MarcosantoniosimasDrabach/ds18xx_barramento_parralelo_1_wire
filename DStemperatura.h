//ATENÇÃO DEIXE DESABILITADO AVISO DE COMPILADOR EVITA MUITOS PROBLEMAS 
//TIVE PROBLEMAS DE REGRAS C++ FOI NECESSÁRIO DESABILITAR PARA VOLTA A FUNCIONA O PROJETO
//define para o compilador se é válido a definição do cabeçalho
#ifndef DStemperatura_H
#define DStemperatura_H
//define as libs necessária para integrar as funcionalidade 
#include "Arduino.h"///////////////////////////////////////////
#include <OneWire.h>///////////////////////////////////////////
#include "Wparametro.h"////////////////////////////////////////
///////////////////////////////////////////////////////////////
//definição da class ds18b20 é definido na min()


class DStemperatura {
    public://são metodos públicos 
	//metodos construtor  DS18B20(uint8_t pin);
        DStemperatura(uint8_t pin);
		         ~DStemperatura();
		//definição de cabeçalho de métodos público 
        uint8_t SelecionarEndereco(uint8_t address[]);
        uint8_t SelecionaProxSensor();
        uint8_t SelecionarProxAlarme();
        void   RedefinirPesquisa();
        float ObterTemperaturaCelcius();
        float ObterTemperaturaFahrenheit();
        uint8_t ObterResolucao();
        void SetarResolucaes(uint8_t resolucao);
        uint8_t ObterModoPower();
        uint8_t ObterCodigoFamilia();
        void ObterEnderecoFisico(uint8_t Endereco[]);
        uint8_t*  ArrayObterEnderecoFisico(uint8_t Endereco[]);
        String  DecimalObterEnderecoFisico(uint8_t Endereco[]);
        void  PrintObterEnderecoFisico(uint8_t Endereco[]);
        void ObterEndereco(uint8_t address[]);
        void TConvercao();
        uint8_t ObterNumeroDespositos();
        uint8_t hasAlarm();
        void setAlarms(int8_t alarmLow, int8_t alarmHigh);
        int8_t getAlarmLow();
        void setAlarmLow(int8_t alarmLow);
        int8_t getAlarmHigh();
        void setAlarmHigh(int8_t alarmHigh);
        void setRegisters(int8_t lowRegister, int8_t highRegister);
        int8_t getLowRegister();
        void setLowRegister(int8_t lowRegister);
        int8_t getHighRegister();
        void setHighRegister(int8_t highRegister);
        uint8_t ObterEnderecos[8];
        uint8_t  ObterQtdDespositivos();
        String ObterNumerodeDespositos();
        String  ObterCodigoFamiliaString();
        String ObterModoPowerString();
    

     
        
		
    private:
    //variaveis internas do sistema
    //metodos ou rotinas internas 
	 //referencia de class de comunicação wire-1
        OneWire umfio;//definido a class padrão 1-wire para declarar os metodos 
		//cabelhos de metodos de uso privado 
        //uint8_t é igaul a um byte ou byte 
        uint8_t globalResolucao;
        uint8_t globalPowerMode;    
        uint8_t QtdDespositivos;
        uint8_t selectedAddress[8];
        uint8_t selectedScratchpad[SIZE_SCRATCHPAD];
        uint8_t selectedResolution;
        uint8_t selectedPowerMode;
        uint8_t searchAddress[8];
        uint8_t lastDiscrepancy;
        uint8_t lastDevice;
        uint8_t readScratchpad();
        void writeScratchpad();
        uint8_t sendCommand(uint8_t romCommand);
        uint8_t sendCommand(uint8_t romCommand, uint8_t functionCommand, uint8_t power = 0);
        uint8_t oneWireSearch(uint8_t romCommand);
        uint8_t isConnected(uint8_t address[]);
        void TempDeConversao(uint8_t resolution, uint8_t powerMode);

        
};

#endif
/*
 * DS18B20 internal = 64-bit ROM (address serial code) + 9-byte scratchpad.

The 9-byte register includes: temperature sensor, upper limit trigger TH high temperature alarm, lower limit trigger TL low temperature alarm, high speed register, 8-bit CRC generator.

The 9-byte register structure diagram is shown below,

byte 0: temperature LSB (50h)

byte 1: temperature MSB (05h) EEPROM

byte 2: TH user defined byte 1 <---> TH user byte 1

byte 3: TL user-defined byte 2 <---> TL user byte 2

byte 4: Configuration register <---> Configuration register

byte 5: reserved bit (FFh)

byte 6: reserved bit (0Ch)

byte 7: reserved bit (10h)

byte 8: CRC check bit

64-bit ROM structure = 8-bit CRC + 48-bit serial number + 8-bit serial code (generated by CRC generator).

5. Conversion accuracy

9-bit：  0.5 ℃

10-bit：0.25 ℃

11-bit： 0.125 ℃

12-bit: 0.0625 °C (default)

Second, the use of DS18B20

1. Steps to start temperature conversion: reset, issue skip ROM command (CCH), issue start temperature conversion command (44H).

2. The code used for DS18B20 is as follows: (for reference only)


 */ 
 
