#include "driver/rmt.h"
#include "led.h"

#define LED_PIN GPIO_NUM_18
#define LED_RMT_CH RMT_CHANNEL_0

#define NUM_LEDS 1

// Timing requirements for RGB LED
#define WS2812_T0H_NS (350)
#define WS2812_T0L_NS (1000)
#define WS2812_T1H_NS (1000)
#define WS2812_T1L_NS (350)
#define WS2812_RESET_US (280)
static uint32_t ws2812_t0h_ticks = 0;
static uint32_t ws2812_t1h_ticks = 0;
static uint32_t ws2812_t0l_ticks = 0;
static uint32_t ws2812_t1l_ticks = 0;

static void IRAM_ATTR vToRmtFormat(const void *src, rmt_item32_t *dest, size_t src_size,
        size_t wanted_num, size_t *translated_size, size_t *item_num)
{
    if (src == NULL || dest == NULL) {
        *translated_size = 0;
        *item_num = 0;
        return;
    }
    const rmt_item32_t bit0 = {{{ ws2812_t0h_ticks, 1, ws2812_t0l_ticks, 0 }}}; //Logical 0
    const rmt_item32_t bit1 = {{{ ws2812_t1h_ticks, 1, ws2812_t1l_ticks, 0 }}}; //Logical 1
    size_t size = 0;
    size_t num = 0;
    uint8_t *psrc = (uint8_t *)src;
    rmt_item32_t *pdest = dest;
    while (size < src_size && num < wanted_num) {
        for (int i = 0; i < 8; i++) {
            // MSB first
            if (*psrc & (1 << (7 - i))) {
                pdest->val =  bit1.val;
            } else {
                pdest->val =  bit0.val;
            }
            num++;
            pdest++;
        }
        size++;
        psrc++;
    }
    *translated_size = size;
    *item_num = num;
}

void vSetPixel(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t buf[] = { g, r, b };
    rmt_write_sample(LED_RMT_CH, buf, 3, true);
    rmt_wait_tx_done(LED_RMT_CH, pdMS_TO_TICKS(1000));
}

void vStripInit(void)
{
    rmt_config_t rConfig = RMT_DEFAULT_CONFIG_TX(LED_PIN, LED_RMT_CH);
    rConfig.clk_div = 2;

    rmt_config(&rConfig);
    rmt_driver_install(rConfig.channel, 0, 0);

    uint32_t counter_clk_hz = 0;
    rmt_get_counter_clock(LED_RMT_CH, &counter_clk_hz);

    // ns -> ticks
    float ratio = (float)counter_clk_hz / 1e9;
    ws2812_t0h_ticks = (uint32_t)(ratio * WS2812_T0H_NS);
    ws2812_t0l_ticks = (uint32_t)(ratio * WS2812_T0L_NS);
    ws2812_t1h_ticks = (uint32_t)(ratio * WS2812_T1H_NS);
    ws2812_t1l_ticks = (uint32_t)(ratio * WS2812_T1L_NS);

    rmt_translator_init(LED_RMT_CH, vToRmtFormat);
}

void vTaskLED(void * pvParams)
{
    for (;;)
    {
        for (uint8_t i = 0; i < 255; i++)
        {
            vSetPixel(i,255-i,0);
            vTaskDelay(pdMS_TO_TICKS(10));
        }  
    }
}