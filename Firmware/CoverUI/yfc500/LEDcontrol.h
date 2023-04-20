/**
 * @file LEDcontrol.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-04-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YFC500_LEDCONTROL_H
#define YFC500_LEDCONTROL_H

#include <stdint.h>
#include <map>
#include "LEDcontrol.h"
#include "stm32cube/inc/gpio.h"
#include "../BttnCtl.h"

#define NUM_LEDS 19
// Some handy shortcuts used during develope
#define LED_NUM_LIFTED 14
#define LED_NUM_WIRE 15
#define LED_NUM_BAT 16
#define LED_NUM_CHARGE 17
#define LED_NUM_REAR 18

class LEDcontrol
{
private:
    struct Led_pio_def
    {
        GPIO_TypeDef *port;
        uint16_t pin;
    };
    Led_pio_def _leds[NUM_LEDS] = {
        // Order derived from LED_animation() and UIButtonTest, but doesn't match with Buttons
        {LED_2HR_GPIO_Port, LED_2HR_Pin}, // 1st (4 piece) row
        {LED_4HR_GPIO_Port, LED_4HR_Pin},
        {LED_6HR_GPIO_Port, LED_6HR_Pin},
        {LED_8HR_GPIO_Port, LED_8HR_Pin},
        {LED_S1_GPIO_Port, LED_S1_Pin}, // 2nd row
        {LED_S2_GPIO_Port, LED_S2_Pin},
        {LED_LOCK_GPIO_Port, LED_LOCK_Pin},
        {LED_MON_GPIO_Port, LED_MON_Pin}, // 3rd (7 piece) row
        {LED_TUE_GPIO_Port, LED_TUE_Pin},
        {LED_WED_GPIO_Port, LED_WED_Pin},
        {LED_THU_GPIO_Port, LED_THU_Pin},
        {LED_FRI_GPIO_Port, LED_FRI_Pin},
        {LED_SAT_GPIO_Port, LED_SAT_Pin},
        {LED_SUN_GPIO_Port, LED_SUN_Pin},
        {LED_LIFTED_GPIO_Port, LED_LIFTED_Pin}, // 4th row
        {LED_WIRE_GPIO_Port, LED_WIRE_Pin},
        {LED_BAT_GPIO_Port, LED_BAT_Pin},
        {LED_CHARGE_GPIO_Port, LED_CHARGE_Pin},
        {LED_REAR_GPIO_Port, LED_REAR_Pin} // LED 19 = SMD LED which seem not to exist on OM-CoverUI, used as alive

        /* Order derived from schematic, but look fully wrong
        {LED_SUN_GPIO_Port, LED_SUN_Pin},
        {LED_SAT_GPIO_Port, LED_SAT_Pin},
        {LED_FRI_GPIO_Port, LED_FRI_Pin},
        {LED_LOCK_GPIO_Port, LED_LOCK_Pin},
        {LED_S2_GPIO_Port, LED_S2_Pin},
        {LED_4HR_GPIO_Port, LED_4HR_Pin},
        {LED_6HR_GPIO_Port, LED_6HR_Pin},
        {LED_8HR_GPIO_Port, LED_8HR_Pin},
        {LED_CHARGE_GPIO_Port, LED_CHARGE_Pin},
        {LED_BAT_GPIO_Port, LED_BAT_Pin},
        {LED_WIRE_GPIO_Port, LED_WIRE_Pin},
        {LED_LIFTED_GPIO_Port, LED_LIFTED_Pin},
        {LED_MON_GPIO_Port, LED_MON_Pin},
        {LED_TUE_GPIO_Port, LED_TUE_Pin},
        {LED_WED_GPIO_Port, LED_WED_Pin},
        {LED_THU_GPIO_Port, LED_THU_Pin},
        {LED_2HR_GPIO_Port, LED_2HR_Pin},
        {LED_S1_GPIO_Port, LED_S1_Pin},
        {LED_REAR_GPIO_Port, LED_REAR_Pin}*/
    };

    uint64_t _led_states_bin = 0; // Binary representation of all LEDs. Each LED gets three bits (19*3=57) for the current state (see BtnCtrl.h)
    uint32_t _force_led_on = 0;   // Binary representation of a "forced LED on" _led_states_bin overrule
    uint32_t _force_led_off = 0;  // Binary representation of a "forced LED off" _led_states_bin overrule

    void _change_led_states(uint8_t led_num, LED_state state);       // Change _led_states_bin for the given LED num and state
    void _force(uint8_t led_num, bool force, uint32_t *_force_type); // Switch/force LED num on or off, independent on it's running state

public:
    LEDcontrol();

    void animate();                                                                            // A short LED Animation
    void blink_timer_elapsed(LED_state blink_state);                                           // Get called by responsible blink timer
    void force_off(uint8_t led_num, bool force);                                               // Switch/force LED num off, independent on it's running state
    void force_on(uint8_t led_num, bool force);                                                // Switch/force LED num on, independent on it's running state
    LED_state get(uint8_t led_num);                                                            // Get state from _led_states_bin for the given led
    void identify(uint8_t led_num);                                                            // Identify the given LED by a special signalling
    bool has_state(uint8_t led_num, LED_state state);                                          // Comparison if the given LED has the given state (in _led_states_bin)
    void set(uint8_t led_num, LED_state state = LED_state::LED_off, bool change_state = true); // Set any of known LED_state states for a specific LED
    void set(uint64_t all_state);                                                              // Set any of known LED_state states for all LEDs by binary state value
    void toggle(uint8_t led_num);                                                              // Toggle on->off or off->on
};

#endif /* YFC500_LEDCONTROL_H */
