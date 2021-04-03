#include "led.h"

#define TRANSITION_TIME_MS 100 // How long should a colour tranisiton take?

class Nanoclone; 

class Tile
{
public:
    Tile();
    Tile(Nanoclone* parent, uint8_t i, SRGB *pRGB, uint8_t uLedsPerTile); 
    ~Tile();

    void vQueueColour(SRGB rgb); // Queue a colour to be updated when vTasks() is called
    void vSetColour(SRGB rgb);   // Set my colour right now   

    void vTasks(uint32_t uTimeDelta); // Execute my pending tasks

private:
    Nanoclone* _poParent; // Pointer to the parent Nanoclone

    uint8_t _uIndex; // My index in the panel array
    uint8_t _uLedsPerTile;
    SRGB*   _psRGB;  // Pointer to my RGB values in the array

    // Variables for colour changing
    bool     _bTask;                      // Does this tile have any work queued?
    uint32_t _uCurrentStep;              // How many steps have we completed? Used to allow colour to be changed mid-transition
    SRGB     _sStartRGB, _sEndRGB;        // The RGB value of the tile at the start and end of the transition
    double   _dDeltaR, _dDeltaG, _dDeltaB; // Colour deltas to indicate how much each channel should change for each step
    

};

class Nanoclone
{
public:
    Nanoclone();
    Nanoclone(uint8_t uNumPanels, uint8_t uLedsPerPanel, TaskFunction_t xTask);
    ~Nanoclone();

    void vQueueColour(uint8_t i, SRGB rgb); // Queue a colour change for tile i
    void vSetColour(uint8_t i, SRGB rgb);

    void vSetBrightness(uint8_t uBrightness);

    void vTasks(uint32_t uTimeDelta);
private:
    Tile* _poTile;  // Array of attached tiles

    SRGB* _psRGB;

    uint8_t _uNumTiles;
    uint8_t _uLedsPerTile;

    uint8_t _uBrightness;

    // FreeRTOS Variables
    TaskFunction_t _xTask; // Pointer to task function
    TaskHandle_t _xHandle; // Task handler
    uint8_t _ucParam;
};
