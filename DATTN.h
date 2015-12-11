#ifndef DATTN_h
#define DATTN_h

#define DATTN_SPI4      0x01
#define DATTN_I2C       0x02

class DATTN
{
  public:
    DATTN(int8_t SID, int8_t SCLK, int8_t CS);
    void InitDAttn(void);
    void SendCommand(unsigned char com);
    void SetVolume(unsigned int v);
    unsigned int SetRelay(unsigned char relayNo, bool set);

  private:
    int8_t _sid;    // MOSI
    int8_t _sclk;   // SCK
    int8_t _cs;     // SS
    bool _isSPI;
    bool _fastSpi;

};




#endif
