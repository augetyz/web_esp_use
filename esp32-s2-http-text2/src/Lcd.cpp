#include "Lcd.h"
extern const char hanzi[300000];
TFT_eSPI tft = TFT_eSPI();
int16_t x, y;
void LCD_init(void)
{
    pinMode(45, OUTPUT);
    digitalWrite(45, HIGH);
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextColor(TFT_SKYBLUE);
    tft.setCursor(0, 0); // 1.设置游标
}

void LCD_showstring(String TEXT)
{
    u_int16_t num = 0;
    int biu;
    int16_t dx, dy,dx1,dy1;
    u_int8_t m, n;
    while (TEXT[num]!= '\0')
    {
        if (TEXT[num]< 0x80)
        {
            tft.drawChar(TEXT[num],x+dx, y+dy);
            dx += 16;
            dy += 0;
            num++;
        }
        else
        {
            biu = (((TEXT[num] - 0xA0 - 1) * 94) + (TEXT[++num] - 0xA0 - 1)) * 16 * 2;
            for (m = 0; m < 31; m++)
            {
                for (n = 0; n < 7; n++)
                {
                    if ((hanzi[biu+ m] << n) & 0x80)
                        tft.drawPixel(x+dx+dx1, y+dy+dy1, TFT_GOLD );

                    dx1++;
                }
                if ((m % 2) == 1)
                {
                    dy1++;
                    dx1 = 0;
                }   
                
            }
            num++;
            dx += 16;
            dy += 0;
            dx1 = dy1 = 0;
        }
        if(dx>144)
        {
            dy += 16;
            dx = 0;
        }
    }
    
}



