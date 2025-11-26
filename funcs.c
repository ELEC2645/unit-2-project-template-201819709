#include <stdio.h>
#include "funcs.h"
#include <string.h>

DeviceData device = {0};    // global device state

void Calculate_MOS_device_parameter(void) {

    printf("\n>> Calculation of single MOS device parameter\n");
    print_device_parameters();   // show current stored values

    int confimration = confirm_new_input_or_use_existing();

    if (confimration) {          // user wants to enter new values
        printf("\nEnter the following device parameters (127 digits MAX!):\n");
        device.mu_n        = get_confirmed_value("electron mobility (m^2/(V*s))");
        device.Cox         = get_confirmed_value("Oxide Capacitance (F/m^2)");
        device.W           = get_confirmed_value("channel width (m)");
        device.L           = get_confirmed_value("channel length (m)");
        device.VGS         = get_confirmed_value("gate-to-source voltage (V)");
        device.VTH         = get_confirmed_value("threshold voltage (V)");
        device.VDS         = get_confirmed_value("drain-to-source voltage (V)");
        device.lambda      = get_confirmed_value("lambda (1/V)");
    }

    printf("\n================= MOSFET Calculations =================\n");

    float VOV = device.VGS - device.VTH;   // overdrive voltage
    printf("VOV (overdrive voltage) = %.6f V\n", VOV);

    // ID in linear region
    float ID_linear = device.mu_n * device.Cox * (device.W / device.L) * ((device.VGS - device.VTH) * device.VDS - (device.VDS * device.VDS) / 2.0f);

    printf("ID (linear region) = %.6f A\n", ID_linear);

    // ID in saturation
    float ID_saturation = 0.5f * device.mu_n * device.Cox * (device.W / device.L) * (VOV * VOV);

    printf("ID (saturation region) = %.6f A\n", ID_saturation);

    float gm;
    if (VOV != 0)                // avoid division by 0
        gm = 2.0f * ID_saturation / VOV;
    else
        gm = 0.0f;

    printf("Transconductance gm = %.6f S (A/V)\n", gm);

    float ro;
    if (ID_saturation > 0)       // avoid division by 0
        ro = 1.0f / (device.lambda * ID_saturation);
    else
        ro = 0.0f;

    printf("Output resistance ro = %.6f ohms\n", ro);

    float Av = gm * ro;          // intrinsic gain
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
