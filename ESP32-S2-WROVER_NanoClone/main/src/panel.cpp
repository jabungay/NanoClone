#include "panel.h"

Tile::Tile() { }

Tile::Tile(Nanoclone* parent, uint8_t i, SRGB *pRGB, uint8_t uLedsPerTile)
{
    // Set class variables
    this->_poParent     = parent;
    this->_psRGB        = pRGB;
    this->_uIndex       = i;
    this->_uLedsPerTile = uLedsPerTile;
}

Tile::~Tile()
{

}

// Queue a colour change for this tile
// The tile colour will fade over time as the vTasks() function is called
// rgb = the target colour
void Tile::vQueueColour(SRGB rgb)
{
    this->_uCurrentStep = 0;    // Reset the step counter
    this->_sStartRGB    = this->_psRGB[0]; // Set the starting value to the current RGB value
    this->_sEndRGB      = rgb;
    this->_bTask        = true;
}

void Tile::vSetColour(SRGB rgb)
{
    // Set all of my values
    for (uint8_t i = 0; i < this->_uLedsPerTile; i++)
    {
        this->_psRGB[i] = rgb;
    }
}

void Tile::vTasks(uint32_t uTimeDelta)
{
    if (this->_bTask) // Do we have pending tasks?
    {
        // double dDeltaR = 
    }
}

Nanoclone::Nanoclone() { }
Nanoclone::Nanoclone(uint8_t uNumTiles, uint8_t uLedsPerTile)
{
    // Set Class Variables
    this->_uNumTiles    = uNumTiles;
    this->_uLedsPerTile = uLedsPerTile;
    this->_uBrightness  = 0xFF;

    this->_psRGB = new SRGB[uNumTiles * uLedsPerTile]; // Allocate SRGB array
    memset(this->_psRGB, 0, uNumTiles * uLedsPerTile * 3);  // Ensure array is clear

    this->_ptLedStrip = new AddrRGB(LED_PIN, LED_RMT_CH, uNumTiles * uLedsPerTile);

    this->_poTile = new Tile[uNumTiles];    // Allocate Tile array
    for (uint8_t i = 0; i < uNumTiles; i++) // Initialize Tile array
    {
        this->_poTile[i] = Tile(this, i, (this->_psRGB + i*uLedsPerTile), uLedsPerTile);
    }
}

Nanoclone::~Nanoclone()
{
    delete[] this->_psRGB;
    delete[] this->_poTile;
    delete this->_ptLedStrip;
    // vTaskDelete(this->_xHandle);
}

void Nanoclone::vInit(void)
{
    this->_ptLedStrip->vInit();
}

// Set the colour of Tile i immediateley
// i   = The index of the tile that should be set
// rgb = The colour that this tile should be set ot 
void Nanoclone::vSetColour(uint8_t i, SRGB rgb)
{
    this->_poTile[i].vSetColour(rgb);   // Call on the Tile to update its own data
    this->_ptLedStrip->vUpdateLEDs(this->_psRGB);          // Push updated colours to LEDs
}

void Nanoclone::vQueueColour(uint8_t i, SRGB rgb)
{
    
}

void Nanoclone::vSetBrightness(uint8_t uBrightness)
{
    this->_uBrightness = uBrightness;
    uint32_t uNumLeds = this->_uNumTiles * this->_uLedsPerTile;
    SRGB psRGB_Scaled[uNumLeds];

    for(uint8_t i = 0; i < uNumLeds; i++)   // Scale all colours based on brightness value
    {
        double dR = (double) this->_psRGB[i].r * ((double) this->_uBrightness / 255.0d);
        double dG = (double) this->_psRGB[i].g * ((double) this->_uBrightness / 255.0d);
        double dB = (double) this->_psRGB[i].b * ((double) this->_uBrightness / 255.0d);

        // printf("R[%u] G[%u] B[%u]\nR[%u] G[%u] B[%u]\n\n", 
        //     this->_psRGB[i].r, this->_psRGB[i].g, this->_psRGB[i].b,
        //     (uint8_t) dR,      (uint8_t) dG,      (uint8_t) dB);

        psRGB_Scaled[i].r = (uint8_t) dR;
        psRGB_Scaled[i].g = (uint8_t) dG;
        psRGB_Scaled[i].b = (uint8_t) dB;
    }

    this->_ptLedStrip->vUpdateLEDs(psRGB_Scaled);  // Push the scaled colours to the panel
}

// Execute any pending tasks for the Nanoclone
// uTimeDelta = How long it's been since this function was last called. 
//              used to ensure that panel updates are untethered to the speed of the MCU
//              and dependent only on time 
void Nanoclone::vTasks(uint32_t uTimeDelta)
{
    for (uint8_t i = 0; i < this->_uNumTiles; i++)
    {
        this->_poTile[i].vTasks(uTimeDelta); // Handle the tasks of all tiles
    }

    uint32_t uNumLeds = this->_uNumTiles * this->_uLedsPerTile;

    SRGB psRGB_Scaled[uNumLeds];

    for(uint8_t i = 0; i < uNumLeds; i++)   // Scale all colours based on brightness value
    {
        double dR = (double) this->_psRGB[i].r * ((double) this->_uBrightness / 255.0d);
        double dG = (double) this->_psRGB[i].g * ((double) this->_uBrightness / 255.0d);
        double dB = (double) this->_psRGB[i].b * ((double) this->_uBrightness / 255.0d);

        // printf("R[%u] G[%u] B[%u]\nR[%u] G[%u] B[%u]\n\n", 
        //     this->_psRGB[i].r, this->_psRGB[i].g, this->_psRGB[i].b,
        //     (uint8_t) dR,      (uint8_t) dG,      (uint8_t) dB);

        psRGB_Scaled[i].r = (uint8_t) dR;
        psRGB_Scaled[i].g = (uint8_t) dG;
        psRGB_Scaled[i].b = (uint8_t) dB;
    }

    this->_ptLedStrip->vUpdateLEDs(psRGB_Scaled);  // Push the scaled colours to the panel

    // vUpdateLEDs(this->_psRGB); // Refresh the panel
}