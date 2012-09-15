/*
 * Toyotomi.cpp - Toyotomi HVAC Remote Control Library
 * 
 * Created by: Kontodimas G. Konstantinos, May 30, 2012
 * Department of Computer Engineering and Informatics
 * University of Patras, Greece
 * 
 * Release into the public domain.
*/


#include <Arduino.h>
#include <Toyotomi.h>

Toyotomi::Toyotomi(uint8_t _temperature, Mode _mode, FanSpeed _fanSpeed,
                   TimerTime _timerOn, TimerTime _timerOff, bool _active)
{
    this->_setIRLEDPin(DEFAULT_LED_PIN);
    this->_setTemperature(_temperature);
    this->_setMode(_mode);
    this->_setFanSpeed(_fanSpeed);
    this->_setTimerOn(_timerOn);
    this->_setTimerOff(_timerOff);
    this->_setActive(_active);
    this->_setSleep(DEFAULT_SLEEP);
}

Toyotomi::~Toyotomi(){}

uint8_t Toyotomi::_setTemperature(uint8_t _temperature)
{
    if (_temperature < MIN_TEMP)
        this->_temperature = MIN_TEMP;
    else if (_temperature > MAX_TEMP)
        this->_temperature = MAX_TEMP;
    else
        this->_temperature = _temperature;
    
    return this->_temperature;
}


uint8_t Toyotomi::setTemperature(uint8_t _temperature)
{
    long unsigned sendValNor, sendValInv;
    
    if (this->getMode() == FAN)
        return this->_temperature;
    this->_setTemperature(_temperature);
    if (!this->isPoweredOn())
        return this->_temperature;
    
    sendValNor = (TEMP_MASK & this->_tempMap(this->getTemperature())) |
                 (MODE_MASK & this->_modeMap(this->getMode())) |
                 (FANSPEED_MASK & this->_fanSpeedMap(this->getFanSpeed())) |
                 (TIMOFFTIM_MASK & this->_timerOffMap(this->getTimerOff())) |
                 (DEFAULT_MASK & DEFAULT_HEAD);

    if (this->getTimerOn() == HOUR000 && this->getTimerOff() == HOUR000)
        sendValInv = ~sendValNor;
    else
        sendValInv = (~sendValNor & INVERTED_MASK) |
                     (sendValNor & TIMENCOM_MASK) |
                     (ONTIMER_MASK & ONTIMERVAL) |
                     (TIMONTIM_MASK & this->_timerOnMap(this->getTimerOn())) |
                     (this->getTimerOn() == HOUR000 ? TIMONTIM_MASK & NOTIMONVAL : 0);
                     
    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);

    return this->_temperature;
}

Mode Toyotomi::_setMode(Mode _mode)
{
    if (_mode >= AUTO && _mode <= FAN)
        this->_mode = _mode;
    else
        this->_mode = DEFAULT_MODE;

    return this->_mode;
}


Mode Toyotomi::setMode(Mode _mode)
{
    long unsigned sendValNor, sendValInv;
    
    this->_setMode(_mode);
    if (!this->isPoweredOn())
        return this->_mode;
    
    sendValNor = (TEMP_MASK & this->_tempMap(this->getTemperature())) |
                 (MODE_MASK & this->_modeMap(this->getMode())) |
                 (FANSPEED_MASK & this->_fanSpeedMap(this->getFanSpeed())) |
                 (TIMOFFTIM_MASK & this->_timerOffMap(this->getTimerOff())) |
                 (DEFAULT_MASK & DEFAULT_HEAD);

    if (this->getTimerOn() == HOUR000 && this->getTimerOff() == HOUR000)
        sendValInv = ~sendValNor;
    else
        sendValInv = (~sendValNor & INVERTED_MASK) |
                     (sendValNor & TIMENCOM_MASK) |
                     (ONTIMER_MASK & ONTIMERVAL) |
                     (TIMONTIM_MASK & this->_timerOnMap(this->getTimerOn())) |
                     (this->getTimerOn() == HOUR000 ? TIMONTIM_MASK & NOTIMONVAL : 0);

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);
    
    return this->_mode;
}


TimerTime Toyotomi::_setTimerOff(TimerTime _timerOff)
{
    if (_timerOff >= HOUR000 && _timerOff <= HOUR240)
        this->_timerOff = _timerOff;
    else
        this->_timerOff = DEFAULT_TIMER;
    
    if (this->_timerOff != HOUR000)
    {
        _setActive(true);
        
        if (this->_timerOff == this->getTimerOn())
        {
            if (this->getTimerOff() < HOUR240)
                this->setTimerOff(static_cast<TimerTime>(this->getTimerOff() + 1));
            else
                this->setTimerOff(static_cast<TimerTime>(this->getTimerOff() - 1));
        }
    }
    
    return this->_timerOff;
}


TimerTime Toyotomi::setTimerOff(TimerTime _timerOff)
{
    long unsigned sendValNor, sendValInv;

    this->_setTimerOff(_timerOff);
    sendValNor = (TEMP_MASK & this->_tempMap(this->getTemperature())) |
                 (MODE_MASK & this->_modeMap(this->getMode())) |
                 (FANSPEED_MASK & this->_fanSpeedMap(this->getFanSpeed())) |
                 (TIMOFFTIM_MASK & this->_timerOffMap(this->getTimerOff())) |
                 (DEFAULT_MASK & DEFAULT_HEAD);

    if (this->getTimerOn() == HOUR000 && this->getTimerOff() == HOUR000)
        sendValInv = ~sendValNor;
    else
        sendValInv = (~sendValNor & INVERTED_MASK) |
                     (sendValNor & TIMENCOM_MASK) |
                     (ONTIMER_MASK & ONTIMERVAL) |
                     (TIMONTIM_MASK & this->_timerOnMap(this->getTimerOn())) |
                     (this->getTimerOn() == HOUR000 ? TIMONTIM_MASK & NOTIMONVAL : 0);

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);
    
    return this->_timerOff;
}


TimerTime Toyotomi::_setTimerOn(TimerTime _timerOn)
{
    if (_timerOn >= HOUR000 && _timerOn <= HOUR240)
        this->_timerOn = _timerOn;
    else
        this->_timerOn = DEFAULT_TIMER;
    
    if (this->_timerOn != HOUR000)
    {
        _setActive(true);
        
        if (this->_timerOn == this->getTimerOff())
        {
            if (this->getTimerOff() < HOUR240)
                this->setTimerOff(static_cast<TimerTime>(this->getTimerOff() + 1));
            else
                this->setTimerOff(static_cast<TimerTime>(this->getTimerOff() - 1));
        }
    }
    
    return this->_timerOn;
}


TimerTime Toyotomi::setTimerOn(TimerTime _timerOn)
{
    long unsigned sendValNor, sendValInv;
    bool _timerOnOn, _timerOffOn;
    
    _timerOnOn = this->_timerOnIsOn();
    _timerOffOn = this->_timerOffIsOn();
    
    this->_setTimerOn(_timerOn);
    if (_timerOnOn && !_timerOffOn && this->_timerOn == HOUR000) //also powered on
    {
        this->powerOff();
        return this->_timerOn;
    }
        
    sendValNor = (TEMP_MASK & this->_tempMap(this->getTemperature())) |
                 (MODE_MASK & this->_modeMap(this->getMode())) |
                 (FANSPEED_MASK & this->_fanSpeedMap(this->getFanSpeed())) |
                 (TIMOFFTIM_MASK & this->_timerOffMap(this->getTimerOff())) |
                 (DEFAULT_MASK & DEFAULT_HEAD);

    if (this->getTimerOn() == HOUR000 && this->getTimerOff() == HOUR000)
        sendValInv = ~sendValNor;
    else
        sendValInv = (~sendValNor & INVERTED_MASK) |
                     (sendValNor & TIMENCOM_MASK) |
                     (ONTIMER_MASK & ONTIMERVAL) |
                     (TIMONTIM_MASK & this->_timerOnMap(this->getTimerOn())) |
                     (this->getTimerOn() == HOUR000 ? TIMONTIM_MASK & NOTIMONVAL : 0);

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);
    
    return this->_timerOn;
}


FanSpeed Toyotomi::_setFanSpeed(FanSpeed _fanSpeed)
{  
    if (_fanSpeed < NONE_SP || _fanSpeed > HIGH_SP)
        _fanSpeed = DEFAULT_SP;
        
    switch (this->_mode)
    {
        case COOL:
        case HEAT:
        case FAN:
            if (_fanSpeed != NONE_SP)
                this->_fanSpeed = _fanSpeed;
            else
                this->_fanSpeed = DEFAULT_SP;
            break;
        case AUTO:
        case DRY:
        default:
            break;
    }
    
    return this->_fanSpeed;
}

FanSpeed Toyotomi::setFanSpeed(FanSpeed _fanSpeed)
{
    long unsigned sendValNor, sendValInv;
    FanSpeed _curFanSpeed, _prevFanSpeed = this->_fanSpeed;
    
    if (!this->isPoweredOn()|| this->_mode == AUTO || this->_mode == DRY)
        return this->_fanSpeed;
    
    this->_setFanSpeed(_fanSpeed);
    sendValNor = (TEMP_MASK & this->_tempMap(this->getTemperature())) |
                 (MODE_MASK & this->_modeMap(this->getMode())) |
                 (FANSPEED_MASK & this->_fanSpeedMap(this->getFanSpeed())) |
                 (TIMOFFTIM_MASK & this->_timerOffMap(this->getTimerOff())) |
                 (DEFAULT_MASK & DEFAULT_HEAD);

    if (this->getTimerOn() == HOUR000 && this->getTimerOff() == HOUR000)
        sendValInv = ~sendValNor;
    else
        sendValInv = (~sendValNor & INVERTED_MASK) |
                     (sendValNor & TIMENCOM_MASK) |
                     (ONTIMER_MASK & ONTIMERVAL) |
                     (TIMONTIM_MASK & this->_timerOnMap(this->getTimerOn())) |
                     (this->getTimerOn() == HOUR000 ? TIMONTIM_MASK & NOTIMONVAL : 0);

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);
    
    return this->_fanSpeed;
}


void Toyotomi::setState(uint8_t _temperature, Mode _mode, FanSpeed _fanSpeed)
{
    this->_setActive(true);
    this->_setMode(_mode);
    this->_setFanSpeed(_fanSpeed);
    this->_setTemperature(_temperature);
    
    this->powerOn();
    
    return;
}

uint8_t Toyotomi::_getTemperature()
{
    return this->_temperature;
}

uint8_t Toyotomi::getTemperature()
{
    if (this->getMode() == FAN)
        return NOTEMP;
        
    return this->_temperature;
}

Mode Toyotomi::_getMode()
{
    return this->_mode;
}

Mode Toyotomi::getMode()
{
    return this->_mode;
}

TimerTime Toyotomi::_getTimerOff()
{
    return this->_timerOff;
}

TimerTime Toyotomi::getTimerOff()
{
    return this->_timerOff;
}

TimerTime Toyotomi::_getTimerOn()
{
    return this->_timerOn;
}

TimerTime Toyotomi::getTimerOn()
{
    return this->_timerOn;
}

FanSpeed Toyotomi::_getFanSpeed()
{
    return this->_fanSpeed;
}

FanSpeed Toyotomi::getFanSpeed()
{
    FanSpeed _fanSpeed;
    switch (this->_mode)
    {
        case COOL:
        case HEAT:
        case FAN:
            _fanSpeed = this->_getFanSpeed();
            break;
        case DRY:
        case AUTO:
        default:
            _fanSpeed = NONE_SP;
            break;
    }
    
    return _fanSpeed;
}

bool Toyotomi::isPoweredOn()
{
    return this->_active;
}

uint8_t Toyotomi::buttonTempUp(uint8_t _times)
{
    uint8_t _newTemp = this->setTemperature(this->getTemperature() + 1);
    
    return _newTemp;
}
    
uint8_t Toyotomi::buttonTempDown(uint8_t _times)
{
    uint8_t _newTemp = this->setTemperature(this->getTemperature() - 1);    
    
    return _newTemp;
}

void Toyotomi::buttonOn()
{
    this->powerOn();
    
    return;
}

void Toyotomi::buttonOff()
{
    this->powerOff();
    
    return;
}

void Toyotomi::buttonOnOff()
{
    if (this->isPoweredOn())
        this->powerOff();
    else
        this->powerOn();

    return;
}

void Toyotomi::buttonMode()
{
    Mode _mode = this->getMode();
    
    switch (_mode)
    {
        case AUTO:
            this->setMode(COOL);
            break;
        case COOL:
            this->setMode(DRY);
            break;
        case DRY:
            this->setMode(HEAT);
            break;
        case HEAT:
            this->setMode(FAN);
            break;
        case FAN:
            this->setMode(AUTO);
            break;
    }
    
    return;
}

void Toyotomi::buttonFanSpeed()
{
    FanSpeed _fanSpeed = this->getFanSpeed();
      
    switch (_fanSpeed)
    {
        case DEFAULT_SP:
            this->setFanSpeed(LOW_SP);
            break;
        case LOW_SP:
            this->setFanSpeed(MED_SP);
            break;
        case MED_SP:
            this->setFanSpeed(HIGH_SP);
            break;
        case HIGH_SP:
            this->setFanSpeed(DEFAULT_SP);
            break;
    }
     
    return;
}

void Toyotomi::buttonSwing()
{
    long unsigned sendValNor, sendValInv;
    
    if (!this->isPoweredOn())
        return;
    
    sendValNor = SWING;
    sendValInv = ~sendValNor;
    
    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);

    return;
}

/*
void Toyotomi::buttonSleep()
{   
    this->setSleep(!this->isSleepOn());

    return;
}
*/

void Toyotomi::buttonTimerOn()
{
    TimerTime _timerOn = this->getTimerOn();
    if (_timerOn == HOUR240)
        this->setTimerOn(HOUR000);
    else
        this->setTimerOn(static_cast<TimerTime>(_timerOn + 1));
 
    return;
}

void Toyotomi::buttonTimerOff()
{
    TimerTime _timerOff = this->getTimerOff();
    if (_timerOff == HOUR240)
        this->setTimerOff(HOUR000);
    else
        this->setTimerOff(static_cast<TimerTime>(_timerOff + 1));
 
    return;
}


void Toyotomi::buttonAirDirection(void)
{
    long unsigned sendValNor, sendValInv;
    
    if (!this->isPoweredOn())
        return;
    
    sendValNor = AIR_DIRECTION;
    sendValInv = ~sendValNor;

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN, false);

    return;
}

void Toyotomi::buttonCleanAir(void)
{
    long unsigned sendValNor, sendValInv;
    
    if (!this->isPoweredOn())
        return;
    
    sendValNor = CLEAN_AIR;
    sendValInv = ~sendValNor;

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);

    return;
}


void Toyotomi::buttonLedDisplay(void)
{
    long unsigned sendValNor, sendValInv;
    
    sendValNor = LED_DISPLAY;
    sendValInv = ~sendValNor;

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);

    return;
}

void Toyotomi::buttonTurbo(void)
{
    long unsigned sendValNor, sendValInv;
    
    if (!this->isPoweredOn())
        return;
    
    sendValNor = TURBO;
    sendValInv = ~sendValNor;

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);

    return;
}
        

uint32_t Toyotomi::_tempMap(const uint8_t _temperature)
{
    uint32_t _rawData;
    if (_temperature == NOTEMP)
        _rawData = pgm_read_word(&tempMap[14]);
    if (_temperature >= MIN_TEMP && _temperature <= MAX_TEMP)
        _rawData = pgm_read_word(&tempMap[_temperature - MIN_TEMP]);
    else //_temperature == NOTEMP
        _rawData = pgm_read_word(&tempMap[0]);
    
    return _rawData;
}


uint32_t Toyotomi::_modeMap(const Mode _mode)
{
    uint32_t _rawData;
    if (_mode >= AUTO && _mode <= FAN)
        _rawData = pgm_read_word(&modeMap[_mode]);
    else
        _rawData = pgm_read_word(&modeMap[AUTO]);
            
    return _rawData;
}

uint32_t Toyotomi::_timerOffMap(const TimerTime _timerOff)
{
    uint32_t _rawData;
    if (_timerOff >= HOUR000 && _timerOff <= HOUR240)
        _rawData = pgm_read_word(&timerOffMap[_timerOff]);
    else
        _rawData = pgm_read_word(&timerOffMap[HOUR000]);
    
    return _rawData;
}

uint32_t Toyotomi::_timerOnMap(const TimerTime _timerOn)
{
    uint32_t _rawData;
    if (_timerOn >= HOUR000 && _timerOn <= HOUR240)
        _rawData = pgm_read_word(&timerOnMap[_timerOn]);
    else
        _rawData = pgm_read_word(&timerOnMap[HOUR000]);
    
    return _rawData;
}


uint32_t Toyotomi::_fanSpeedMap(const FanSpeed _fanSpeed)
{
    uint32_t _rawData;
    if (_fanSpeed >= NONE_SP && _fanSpeed <= HIGH_SP)
        _rawData = pgm_read_word(&fanSpeedMap[_fanSpeed]);
    else
        _rawData = pgm_read_word(&fanSpeedMap[DEFAULT_SP]);
    
    return _rawData;
}


uint8_t Toyotomi::_getIRLEDPin()
{
    return this->_IRLEDPin;
}


void Toyotomi::_pulsesIR(long microsecs, uint8_t _IRLEDPin)
{
    while (microsecs > 0)
    {
        // 38 kHz is about 13 microseconds high and 13 microseconds low
        digitalWrite(_IRLEDPin, HIGH);  // this takes about 5 microseconds to happen
        delayMicroseconds(_VAR_DELAY);         // hang out for 8 microseconds
        digitalWrite(_IRLEDPin, LOW);   // this also takes about 5 microseconds
        delayMicroseconds(_VAR_DELAY);         // hang out for 8 microseconds
 
        // so 26 microseconds altogether
        microsecs -= CYCLE_TIME;
    }
    
    return;
}


void Toyotomi::_sendHIGH(uint8_t _IRLEDPin)
{
    this->_pulsesIR(CYCLE_TIME * PULSE_CYCLES, _IRLEDPin);
    delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 3);
}


void Toyotomi::_sendLOW(uint8_t _IRLEDPin)
{
    this->_pulsesIR(CYCLE_TIME * PULSE_CYCLES, _IRLEDPin);
    delayMicroseconds(CYCLE_TIME * PULSE_CYCLES);    
}


uint8_t Toyotomi::_setIRLEDPin(uint8_t _IRLEDPin)
{
    if (_IRLEDPin >= 8 && _IRLEDPin <= 13)
        this->_IRLEDPin = _IRLEDPin;
    else
        this->_IRLEDPin = DEFAULT_LED_PIN;
    
    pinMode(this->_IRLEDPin, OUTPUT);
    return this->_IRLEDPin;
}



void Toyotomi::sendData(const uint8_t dataIn[], const uint8_t dataLength, const bool repeat)
{
    uint8_t _IRLEDPin = _getIRLEDPin();
    
    cli();
    
//     delayMicroseconds(50);
    if (repeat)
    {
        this->_pulsesIR(CYCLE_TIME * PULSE_CYCLES * 8, _IRLEDPin);
        delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 8);
    
        for (unsigned i = 0; i < dataLength; i++)
        {
            if (!dataIn[i])
                this->_sendLOW(_IRLEDPin);
            else
                this->_sendHIGH(_IRLEDPin);
        }

//         this->_pulsesIR(CYCLE_TIME * PULSE_CYCLES, _IRLEDPin);
        this->_sendLOW(_IRLEDPin);
        delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 9);
        
        this->_pulsesIR(CYCLE_TIME * PULSE_CYCLES * 8, _IRLEDPin);
        delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 8);
    
        for (unsigned i = 0; i < dataLength; i++)
        {
            if (!dataIn[i])
                this->_sendLOW(_IRLEDPin);
            else
                this->_sendHIGH(_IRLEDPin);
        }

        this->_sendLOW(_IRLEDPin);
//         this->_pulsesIR(CYCLE_TIME * PULSE_CYCLES, _IRLEDPin);
        delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 9);
    }
    else
    {
        this->_pulsesIR(CYCLE_TIME * PULSE_CYCLES * 8, _IRLEDPin);
        delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 8);
    
        for (unsigned i = 0; i < dataLength; i++)
        {
            if (!dataIn[i])
                this->_sendLOW(_IRLEDPin);
            else
                this->_sendHIGH(_IRLEDPin);
        }

        this->_sendLOW(_IRLEDPin);
//         this->_pulsesIR(CYCLE_TIME * PULSE_CYCLES, _IRLEDPin);
        delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 9);
    }
//     delay(65);
    
    sei();
#ifdef SERIAL_DEBUG
    this->sendToSerial(dataIn, dataLength, repeat);
#endif
}


void Toyotomi::sendDataNoHeaders(const uint8_t dataIn[], const uint8_t dataLength)
{
    uint8_t _IRLEDPin = _getIRLEDPin();
    
    cli();
    
    //delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 8);
    
    for (unsigned i = 0; i < dataLength; i++)
    {
        if (!dataIn[i])
            this->_sendLOW(_IRLEDPin);
        else
            this->_sendHIGH(_IRLEDPin);
    }
    
    this->_sendLOW(_IRLEDPin);
    delayMicroseconds(CYCLE_TIME * PULSE_CYCLES * 9);
    
    sei();
}



void Toyotomi::_createByteArray(const uint32_t inputHex, const uint32_t inputHexInv, uint8_t dataIn[], const uint8_t dataLength)
{
    for (unsigned i = 0; i < dataLength/2; i++)
    {
         dataIn[i % 8 + 16*(i / 8)] = (inputHex >> ((dataLength / 2) - ((i / 8) * 8) - (8 - i % 8))) & 1;
         dataIn[i % 8 + 16*(i / 8) + 8] = (inputHexInv >> ((dataLength / 2) - ((i / 8) * 8) - (8 - i % 8))) & 1;
    }
}

void Toyotomi::powerOn()
{
    long unsigned sendValNor, sendValInv;
    this->_setActive(true);
    
    sendValNor = (TEMP_MASK & this->_tempMap(this->getTemperature())) |
                 (MODE_MASK & this->_modeMap(this->getMode())) |
                 (FANSPEED_MASK & this->_fanSpeedMap(this->getFanSpeed())) |
                 (TIMOFFTIM_MASK & this->_timerOffMap(this->getTimerOff())) |
                 (DEFAULT_MASK & DEFAULT_HEAD);

    sendValInv = ~sendValNor;

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);

    return;
}

void Toyotomi::powerOff()
{
    long unsigned sendValNor, sendValInv;
    
    this->_setActive(false);
    
    sendValNor = POWER_OFF;
    sendValInv = ~sendValNor;

    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
    this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);

    return;
}


bool Toyotomi::_setActive(const bool _active)
{
    if (!_active)
    {
        this->_setTimerOff(HOUR000);
        this->_setTimerOn(HOUR000);
    }
    this->_active = _active;
    
    return this->_active;
}

bool Toyotomi::_setSleep(const bool _sleepState)
{
    this->_sleepState = _sleepState;
    
    return this->_sleepState;
}

/*
bool Toyotomi::setSleep(bool _sleepState)
{
    long unsigned sendValNor, sendValInv;
    uint8_t tempVal;
    unsigned i;
    
    if (!this->isPoweredOn())
        return this->_sleepState;
    
    this->_setSleep(_sleepState);
    
    sendValNor = (TEMP_MASK & this->_tempMap(this->getTemperature())) |
                 (MODE_MASK & this->_modeMap(this->getMode())) |
                 (FANSPEED_MASK & this->_fanSpeedMap(this->getFanSpeed())) |
                 (TIMOFFTIM_MASK & this->_timerOffMap(this->getTimerOff())) |
                 (DEFAULT_MASK & DEFAULT_HEAD);

    if (this->getTimerOn() == HOUR000 && this->getTimerOff() == HOUR000)
        sendValInv = ~sendValNor;
    else
        sendValInv = (~sendValNor & INVERTED_MASK) |
                     (sendValNor & TIMENCOM_MASK) |
                     (ONTIMER_MASK & ONTIMERVAL) |
                     (TIMONTIM_MASK & this->_timerOnMap(this->getTimerOn())) |
                     (this->getTimerOn() == HOUR000 ? TIMONTIM_MASK & NOTIMONVAL : 0);
                         
    this->_createByteArray(sendValNor, sendValInv, this->dataInBuf, DEFAULT_DATA_LEN);
     
    if (!this->_sleepState)
        this->sendData(this->dataInBuf, DEFAULT_DATA_LEN);
    else
        this->sendDataNoHeaders(&this->dataInBuf[1], DEFAULT_DATA_LEN - 1);
      
    return this->_sleepState;
}
*/

bool Toyotomi::isSleepOn(void)
{
    return this->_sleepState;
}


bool Toyotomi::_timerOnIsOn()
{
    if (this->getTimerOn() == HOUR000 && this->getTimerOff() == HOUR000)
        return false;
    else
        return true;
}


bool Toyotomi::_timerOffIsOn()
{
    if (this->getTimerOff() == HOUR000)
        return false;
    else
        return true;
}

#ifdef SERIAL_DEBUG

void Toyotomi::sendToSerial(const uint8_t dataIn[], const uint8_t dataLength, const bool repeat)
{
    Serial.print("int IRsignal[] = {\n");
    Serial.print("// ON, OFF (in 10's of microseconds)\n");
    uint8_t repeatCount = (repeat == true ? 2 : 1);
    for (uint8_t j = 0; j < repeatCount; j++)
    {
        Serial.print("\t"); // tab
        Serial.print(8 * 53, DEC);
        Serial.print(", ");
        Serial.print(8 * 53, DEC);
        Serial.print(",\n");
        for (uint8_t i = 0; i < dataLength; i++)
        {
            Serial.print("\t"); // tab
            Serial.print(1 * 53, DEC);
            Serial.print(", ");
            if (dataIn[i])
                Serial.print(3 * 53, DEC);
            else
                Serial.print(1 * 53, DEC);
            Serial.print(",\n");
        }
        if (j < repeatCount - 1)
        {
            Serial.print("\t"); // tab
            Serial.print(1 * 53, DEC);
            Serial.print(", ");
            Serial.print(10 * 53, DEC);
            Serial.print(",\n");
        }
    }
    Serial.print("\t53, 0};\n");
}

#endif