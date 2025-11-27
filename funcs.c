#include <stdio.h>
#include "funcs.h"
#include <string.h>
#include <math.h>


DeviceData device = {0};    // global device state

void Calculate_MOS_device_parameter(void) {

    clear_screen(); // clean the output terminal for better viewing

    printf("\n>> Calculation of single MOS device parameter\n");
    print_device_parameters(1);   // show current stored values

    int confimration = confirm_new_input_or_use_existing();

    if (confimration) {          // user wants to enter new values
        clear_screen();
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

    clear_screen();

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


// Function 2
void device_component_selection_calculations(void) {

    clear_screen();

    printf("\n>> Device Component Selection Calculations\n");
    print_device_parameters(2);      // Current values for Section 2

    if (device.mu_n <= 0 || device.Cox <= 0 || device.W <= 0 || device.L <= 0 ||
    (device.VGS - device.VTH) <= 0)
{
    printf("WARNING: MOSFET parameters are incomplete or invalid.\n");
    printf("Ron cannot be computed because one or more required values are zero or negative.\n");
    printf("Missing terms would cause division by zero in the Ron formula.\n\n");
    return;
}

    int confirmation = confirm_new_input_or_use_existing();

    if (confirmation) {
        clear_screen();
        printf("\nEnter component parameters (127 digits MAX!):\n");

        device.Temperature  = get_confirmed_value("Temperature (K)");
        device.Cs           = get_confirmed_value("Sampling capacitor Cs (F)");
        device.vrms         = get_confirmed_value("Target RMS noise (V)");
        device.AC           = get_confirmed_value("Pelgrom mismatch coefficient AC");
        device.Area         = get_confirmed_value("Device area (m^2)");
    }

    clear_screen();

    printf("\n================= Component Selection Calculations =================\n");

    const float k = 1.380649e-23f;   // Boltzmann constant

    // kT/C noise
    float vn_rms = sqrtf(k * device.Temperature / device.Cs);
    printf("kT/C noise (vn_rms) = %.6e V\n", vn_rms);

    // Required Cs for target noise
    float Cs_required = k * device.Temperature / (device.vrms * device.vrms);
    printf("Required sampling capacitance Cs = %.6e F\n", Cs_required);

    // Pelgrom mismatch
    float sigmaC_over_C = device.AC / sqrtf(device.Area);
    printf("Capacitor mismatch (sigmaC/C) = %.6e\n", sigmaC_over_C);

    // MOSFET switch on-resistance
    float overdrive = device.VGS - device.VTH;
    float Ron_calc = 1.0f / (device.mu_n * device.Cox * (device.W / device.L) * overdrive);
    device.Ron = Ron_calc;
    printf("Estimated MOS switch on-resistance Ron = %.6f ohms\n", Ron_calc);

    printf("====================================================================\n\n");
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
