#include <stdio.h>
#include "funcs.h"
#include <string.h>

void Calculate_MOS_device_parameter(void) {

    char variable_name[64];     // show user what's prompted currently

    float electron_mobility;
    float Oxide_Capacitance;
    float channel_width;
    float channel_length;
    float gate_source_Voltage;
    float threshold_voltage;
    float drain_source_voltage;
    float lambda;


    printf("\n>> Calculation of single MOS device parameter\n");
    printf("\nEnter the following device parameters (127 digits max!!!):\n");

    // Calling custom function to take user input one by one

    electron_mobility = get_confirmed_value("electron mobility (m^2/(V*s))");
    Oxide_Capacitance = get_confirmed_value("Oxide Capacitance (F/m^2)");
    channel_width = get_confirmed_value("channel width (m)");
    channel_length = get_confirmed_value("channel length (m)");
    gate_source_Voltage = get_confirmed_value("gate-to-source voltage (V)");
    threshold_voltage = get_confirmed_value("threshold voltage (V)");
    drain_source_voltage = get_confirmed_value("drain-to-source voltage (V)");
    lambda = get_confirmed_value("lambda (1/V)");

    printf("\n================= MOSFET Calculations =================\n");

    float VOV = gate_source_Voltage - threshold_voltage;
    printf("VOV (overdrive voltage) = %.6f V\n", VOV);

    // ID linear = μn Cox (W/L) [ (VGS − VTH) VDS − VDS^2 / 2 ]

    float ID_linear = electron_mobility * Oxide_Capacitance * (channel_width / channel_length) *
    ((gate_source_Voltage - threshold_voltage) * drain_source_voltage - (drain_source_voltage * drain_source_voltage) / 2.0f);
    printf("ID (linear region) = %.6f A\n", ID_linear);

    // ID Saturation = (1/2) μn Cox (W/L) (VOV)^2

    float ID_saturation = 0.5f * electron_mobility * Oxide_Capacitance * (channel_width / channel_length) * (VOV * VOV);
    printf("ID (saturation region) = %.6f A\n", ID_saturation);

    //gm = ∂ID/∂VGS = μn Cox (W/L) VOV = 2ID / VOV
    float gm;
    if (VOV != 0) {                         // Avoid division by 0
        gm = 2.0f * ID_saturation / VOV;
    } else {
        gm = 0.0f;
    }
    printf("Transconductance gm = %.6f S (A/V)\n", gm);

    // output resistance
    float ro;

    if (ID_saturation > 0) {                    // avoid division by 0 again
        ro = 1.0f / (lambda * ID_saturation);
    } else {
        ro = 0.0f;
    }
    printf("Output resistance ro = %.6f ohms\n", ro);

    // Intrinsic gain: Av = gm * ro
    float Av = gm * ro;
    printf("Intrinsic gain Av = %.6f V/V\n", Av);
    printf("=======================================================\n\n");
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
