/*
  xplg_apa102.ino - APA102 led string support for Sonoff-Tasmota

  Copyright (C) 2018 Florian Moesch

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_APA102

#include <NeoPixelBus.h>

#ifdef USE_APA102_SPI
#if (USE_APA102_CTYPE == NEO_GRB)
NeoPixelBus<DotStarGrbFeature, DotStarSpiMethod>* strip = NULL;
#elif (USE_APA102_CTYPE == NEO_BRG)
NeoPixelBus<DotStarBrgFeature, DotStarSpiMethod>* strip = NULL;
#elif (USE_APA102_CTYPE == NEO_RBG)
NeoPixelBus<DotStarRbgFeature, DotStarSpiMethod>* strip = NULL;
#elif (USE_APA102_CTYPE == NEO_RGBW)
NeoPixelBus<DotStarRgbwFeature, DotStarSpiMethod>* strip = NULL;
#elif (USE_APA102_CTYPE == NEO_GRBW)
NeoPixelBus<DotStarGrbwFeature, DotStarSpiMethod>* strip = NULL;
#else
NeoPixelBus<DotStarRgbFeature, DotStarSpiMethod>* strip = NULL;
#endif
#else
#if (USE_APA102_CTYPE == NEO_GRB)
NeoPixelBus<DotStarGrbFeature, DotStarMethod>* strip = NULL;
#elif (USE_APA102_CTYPE == NEO_BRG)
NeoPixelBus<DotStarBrgFeature, DotStarMethod>* strip = NULL;
#elif (USE_APA102_CTYPE == NEO_RBG)
NeoPixelBus<DotStarRbgFeature, DotStarMethod>* strip = NULL;
#elif (USE_APA102_CTYPE == NEO_RGBW)
NeoPixelBus<DotStarRgbwFeature, DotStarMethod>* strip = NULL;
#elif (USE_APA102_CTYPE == NEO_GRBW)
NeoPixelBus<DotStarGrbwFeature, DotStarMethod>* strip = NULL;
#else
NeoPixelBus<DotStarRgbFeature, DotStarMethod>* strip = NULL;
#endif
#endif

void Apa102Init()
{
#ifdef USE_APA102_SPI
#if (USE_APA102_CTYPE == NEO_GRB)
    strip = new NeoPixelBus<DotStarGrbFeature, DotStarSpiMethod>(APA102_MAX_LEDS);
#elif (USE_APA102_CTYPE == NEO_BRG)
    strip = new NeoPixelBus<DotStarBrgFeature, DotStarSpiMethod>(APA102_MAX_LEDS);
#elif (USE_APA102_CTYPE == NEO_RBG)
    strip = new NeoPixelBus<DotStarRbgFeature, DotStarSpiMethod>(APA102_MAX_LEDS);
#elif (USE_APA102_CTYPE == NEO_RGBW)
    strip = new NeoPixelBus<DotStarRgbwFeature, DotStarSpiMethod>(APA102_MAX_LEDS);
#elif (USE_APA102_CTYPE == NEO_GRBW)
    strip = new NeoPixelBus<DotStarGrbwFeature, DotStarSpiMethod>(APA102_MAX_LEDS);
#else
    strip = new NeoPixelBus<DotStarRgbFeature, DotStarSpiMethod>(APA102_MAX_LEDS);
#endif
#else
#if (USE_APA102_CTYPE == NEO_GRB)
    strip = new NeoPixelBus<DotStarGrbFeature, DotStarMethod>(APA102_MAX_LEDS, pin[GPIO_APA102]);
#elif (USE_APA102_CTYPE == NEO_BRG)
    strip = new NeoPixelBus<DotStarBrgFeature, DotStarMethod>(APA102_MAX_LEDS, pin[GPIO_APA102]);
#elif (USE_APA102_CTYPE == NEO_RBG)
    strip = new NeoPixelBus<DotStarRbgFeature, DotStarMethod>(APA102_MAX_LEDS, pin[GPIO_APA102]);
#elif (USE_APA102_CTYPE == NEO_RGBW)
    strip = new NeoPixelBus<DotStarRgbwFeature, DotStarMethod>(APA102_MAX_LEDS, pin[GPIO_APA102]);
#elif (USE_APA102_CTYPE == NEO_GRBW)
    strip = new NeoPixelBus<DotStarGrbwFeature, DotStarMethod>(APA102_MAX_LEDS, pin[GPIO_APA102]);
#else
    strip = new NeoPixelBus<DotStarRgbFeature, DotStarMethod>(APA102_MAX_LEDS, pin[GPIO_APA102]);
#endif
#endif
    strip->Begin();
    Apa102Clear();
}

void Apa102Clear()
{
    strip->ClearTo(0);
    strip->Show();
}

void Apa102SetColor(uint16_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
#if (USE_APA102_CTYPE > NEO_3LED)
    RgbwColor lcolor;
    lcolor.W = white;
#else
    RgbColor lcolor;
#endif

    lcolor.R = red;
    lcolor.G = green;
    lcolor.B = blue;
    if (led) {
        strip->SetPixelColor(led - 1, lcolor); // Led 1 is strip Led 0 -> substract offset 1
    } else {
        for (uint16_t i = 0; i < Settings.light_pixels; i++) {
            strip->SetPixelColor(i, lcolor);
        }
    }
    strip->Show();
}

char* Apa102GetColor(uint16_t led, char* scolor)
{
    uint8_t sl_ledcolor[4];

#if (USE_APA102_CTYPE > NEO_3LED)
    RgbwColor lcolor = strip->GetPixelColor(led - 1);
    sl_ledcolor[3] = lcolor.W;
#else
    RgbColor lcolor = strip->GetPixelColor(led - 1);
#endif
    sl_ledcolor[0] = lcolor.R;
    sl_ledcolor[1] = lcolor.G;
    sl_ledcolor[2] = lcolor.B;
    scolor[0] = '\0';
    for (byte i = 0; i < light_subtype; i++) {
        if (Settings.flag.decimal_text) {
            snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", sl_ledcolor[i]);
        } else {
            snprintf_P(scolor, 25, PSTR("%s%02X"), scolor, sl_ledcolor[i]);
        }
    }
    return scolor;
}

#endif
