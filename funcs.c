#include <stdio.h>
#include "funcs.h"
#include <string.h>

void Calculate_MOS_device_parameter(void) {

    char variable_name[64];     // storage is local, modifiable
    strcpy(variable_name, "electron mobility (m^2/(V·s)");

    float electron_mobility;
    float Oxide_Capacitance;
    float channel_width;
    float channel_length;
    float gate_source_Voltage;
    float threshold_voltage;
    float drain_source_voltage;

    printf("\n>> Calculation of single MOS device parameter\n");
    printf("\nEnter the following device parameters:\n");

    // Calling custom function to take user input one by one

    electron_mobility = get_user_input_custom(variable_name);

    strcpy(variable_name, "Oxide Capacitance: F/m^2");
    electron_mobility = get_user_input_custom(variable_name);

    strcpy(variable_name, "channel width (m)");
    channel_width = get_user_input_custom(variable_name);

    strcpy(variable_name, "channel length (m)");
    channel_length = get_user_input_custom(variable_name);

    strcpy(variable_name, "gate-to-source voltage (V)");
    gate_source_Voltage = get_user_input_custom(variable_name);

    strcpy(variable_name, "threshold voltage (V)");
    threshold_voltage = get_user_input_custom(variable_name);

    strcpy(variable_name, "drain-to-source voltage (V)");
    drain_source_voltage = get_user_input_custom(variable_name);

    
}

void menu_item_2(void) {
    printf("\n>> Menu 2\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 2 */
}

void menu_item_3(void) {
    printf("\n>> Menu 3\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 3 */
}

void menu_item_4(void) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}
