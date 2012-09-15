/*
 * Toyotomi.h - Toyotomi HVAC Remote Control Library
 * 
 * Created by: Kontodimas G. Konstantinos, May 30, 2012
 * Department of Computer Engineering and Informatics
 * University of Patras, Greece
 * 
 * Release into the public domain.
*/

#ifndef TOYOTOMI_H
#define TOYOTOMI_H

#include <Arduino.h>
#include <avr/pgmspace.h>

#define CLK_8MHZ

#ifdef  CLK_8MHZ
#define _VAR_DELAY            2
#else
#define _VAR_DELAY            8
#endif


#define IR_CLOCK_RATE    38000L

#define DEFAULT_MASK   0xFF8000
#define TEMP_MASK      0x00000F
#define MODE_MASK      0x000030
#define FANSPEED_MASK  0x000700
#define COMMAND_MASK   0xFFFFFF
#define LASTKEY_MASK   0x0000E0
#define TIMENCOM_MASK  0x000001
#define TIMONTIM_MASK  0x0000FE
#define TIMOFFTIM_MASK 0x0078C0
#define ONTIMER_MASK   0x000080
#define INVERTED_MASK  0xFFFF00

#define DEFAULT_HEAD   0x4D8000
#define POWER_OFF      0x4DDE07
#define AIR_DIRECTION  0x4DF007
#define SWING          0x4DD607
#define CLEAN_AIR      0xADAFC5
#define LED_DISPLAY    0xADAFA5
#define TURBO          0xADAF45
#define SLEEP          0x647EA0

#define ONTIMERVAL     0x000080
#define NOTIMONVAL     0x0000FE


#define NOTEMP         0

enum Mode      { AUTO, COOL, DRY, HEAT, FAN };
enum FanSpeed  { NONE_SP, DEFAULT_SP, LOW_SP, MED_SP, HIGH_SP };
enum TimerTime { HOUR000, HOUR005, HOUR010, HOUR015, HOUR020, HOUR025, HOUR030, HOUR035, HOUR040,
                 HOUR045, HOUR050, HOUR055, HOUR060, HOUR065, HOUR070, HOUR075, HOUR080, HOUR085,
                 HOUR090, HOUR095, HOUR100, HOUR110, HOUR120, HOUR130, HOUR140, HOUR150, HOUR160,
                 HOUR170, HOUR180, HOUR190, HOUR200, HOUR210, HOUR220, HOUR230, HOUR240 };

const prog_uint32_t tempMap[] PROGMEM     = { 0x000000, 0x000008, 0x00000C, 0x000004, 0x000006,
                                              0x00000E, 0x00000A, 0x000002, 0x000003, 0x00000B,
                                              0x000009, 0x000001, 0x000005, 0x00000D, 0x000007 };
const prog_uint32_t modeMap[] PROGMEM     = { 0x000010, 0x000000, 0x000020, 0x000030, 0x000020 };
const prog_uint32_t fanSpeedMap[] PROGMEM = { 0x000000, 0x000500, 0x000100, 0x000200, 0x000400 };
const prog_uint32_t timerOnMap[] PROGMEM  = { 0x000040, 0x000000, 0x000040, 0x000020, 0x000060,
                                              0x000010, 0x000050, 0x000030, 0x000070, 0x000008,
                                              0x000048, 0x000028, 0x000068, 0x000018, 0x000058,
                                              0x000038, 0x000078, 0x000004, 0x000044, 0x000024,
                                              0x000064, 0x000054, 0x000074, 0x00004c, 0x00006c,
                                              0x00005c, 0x00007c, 0x000042, 0x000062, 0x000052,
                                              0x000072, 0x00004a, 0x00006a, 0x00005a, 0x00007a };     
const prog_uint32_t timerOffMap[] PROGMEM = { 0x007800, 0x000000, 0x004000, 0x002000, 0x006000,
                                              0x001000, 0x005000, 0x003000, 0x007000, 0x000800,
                                              0x004800, 0x002800, 0x006800, 0x001800, 0x005800,
                                              0x003800, 0x007800, 0x000080, 0x004080, 0x002080,
                                              0x006080, 0x005080, 0x007080, 0x004880, 0x006880,
                                              0x005880, 0x007880, 0x004040, 0x006040, 0x005040,
                                              0x007040, 0x004840, 0x006840, 0x005840, 0x007840 };

#define DEFAULT_TEMP     20
#define DEFAULT_MODE     AUTO
#define DEFAULT_FANSPEED DEFAULT_SP
#define DEFAULT_TIMER    HOUR000
#define DEFAULT_POWER    false
#define DEFAULT_SLEEP    false

#define MIN_TEMP         17
#define MAX_TEMP         30


#define DEFAULT_LED_PIN  8
#define DEFAULT_DATA_LEN 48
#define CYCLE_TIME       26
#define PULSE_CYCLES     21

class Toyotomi
{
    public:
        Toyotomi(uint8_t _temp = DEFAULT_TEMP, Mode _mode = AUTO,
                 FanSpeed _fanSpeed = DEFAULT_SP, TimerTime _timerOn = DEFAULT_TIMER,
                 TimerTime _timerOff = DEFAULT_TIMER, bool = DEFAULT_POWER);
        ~Toyotomi(void);
        uint8_t buttonTempUp(uint8_t _times = 1);
        uint8_t buttonTempDown(uint8_t _times = 1);
        void buttonOn(void);
        void buttonOff(void);
        void buttonOnOff(void);
        void buttonMode(void);
        void buttonFanSpeed(void);
        void buttonSwing(void);
        //void buttonSleep(void);
        void buttonTimerOn(void);
        void buttonTimerOff(void);
        void buttonAirDirection(void);
        void buttonCleanAir(void);
        void buttonLedDisplay(void);
        void buttonTurbo(void);
        
        uint8_t setTemperature(uint8_t _temperature = DEFAULT_TEMP);
        Mode setMode(Mode _mode = DEFAULT_MODE);
        FanSpeed setFanSpeed(FanSpeed _fanSpeed = DEFAULT_FANSPEED);
        TimerTime setTimerOn(TimerTime _time = DEFAULT_TIMER);
        TimerTime setTimerOff(TimerTime _time = DEFAULT_TIMER);
        void powerOn(void);
        void powerOff(void);
        //bool setSleep(bool _sleep = DEFAULT_SLEEP);
        void setState(uint8_t _temperature, Mode _mode, FanSpeed _fanSpeed);

        bool isPoweredOn(void);
        uint8_t getTemperature(void);
        Mode getMode(void);
        FanSpeed getFanSpeed(void);
        TimerTime getTimerOn(void);
        TimerTime getTimerOff(void);
        bool isSleepOn(void);
        
    private:
        uint8_t _setTemperature(uint8_t _temperature = DEFAULT_TEMP);
        Mode _setMode(Mode _mode = DEFAULT_MODE);
        FanSpeed _setFanSpeed(FanSpeed _fanSpeed = DEFAULT_FANSPEED);
        TimerTime _setTimerOn(TimerTime _time = DEFAULT_TIMER);
        TimerTime _setTimerOff(TimerTime _time = DEFAULT_TIMER);
        bool _setActive(const bool = false);
        bool _setSleep(const bool = DEFAULT_SLEEP);
        
        uint8_t _getTemperature(void);
        Mode _getMode(void);
        FanSpeed _getFanSpeed(void);
        TimerTime _getTimerOn(void);
        TimerTime _getTimerOff(void);
        
        bool _timerOnIsOn(void);
        bool _timerOffIsOn(void);
        
        uint32_t _tempMap(const uint8_t = MIN_TEMP - 1);
        uint32_t _modeMap(const Mode = AUTO);
        uint32_t _timerOffMap(const TimerTime = HOUR000);
        uint32_t _timerOnMap(const TimerTime = HOUR000);
        uint32_t _fanSpeedMap(const FanSpeed = DEFAULT_FANSPEED);
        void _createByteArray(const uint32_t, const uint32_t, uint8_t [], const uint8_t = DEFAULT_DATA_LEN);
        uint8_t _setIRLEDPin(uint8_t = DEFAULT_LED_PIN);
        uint8_t _getIRLEDPin(void);
        void _pulsesIR(long int, uint8_t);
        void _sendHIGH(uint8_t = DEFAULT_LED_PIN);
        void _sendLOW(uint8_t = DEFAULT_LED_PIN);
        void sendData(const uint8_t [], uint8_t = DEFAULT_DATA_LEN, const bool = true);
        void sendDataNoHeaders(const uint8_t [], uint8_t = DEFAULT_DATA_LEN);
        
        uint8_t dataInBuf[DEFAULT_DATA_LEN];
        uint8_t _temperature;
        Mode _mode;
        TimerTime _timerOff;
        TimerTime _timerOn;
        FanSpeed _fanSpeed;
        bool _active;
        bool _sleepState;
        uint8_t _IRLEDPin;
};

#endif
