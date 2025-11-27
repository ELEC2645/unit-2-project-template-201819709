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
void device_component_selection_calculations(void)
{
    clear_screen();

    printf("\n>> Device Component Selection Calculations\n");
    print_device_parameters(2);      // show Section 2 values

    // Warn user if Ron cannot be computed with current data

    float overdrive = device.VGS - device.VTH;

    if (device.mu_n <= 0 || device.Cox <= 0 || device.W <= 0 || device.L <= 0 || overdrive <= 0)
    {
        printf("\nWARNING: Current MOSFET parameters are incomplete or invalid.\n");
        printf("Ron cannot be computed with the existing values.\n");
        printf("You may want to update device parameters first.\n\n");
        return;
    }
    // ask user whether to create a new situation
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

    const float k = 1.380649e-23f;

    float vn_rms = 0.0f;  // kt/c noise
    if (device.Cs > 0 && device.Temperature > 0) {
        vn_rms = sqrtf(k * device.Temperature / device.Cs);
        printf("kT/C noise (vn_rms) = %.6e V\n", vn_rms);
    } else {
        printf("kT/C noise cannot be computed (Cs or Temperature <= 0)\n");
    }

    // target sampling capacitor
    if (device.vrms > 0) {
        float Cs_required = k * device.Temperature / (device.vrms * device.vrms);
        printf("Required sampling capacitance Cs = %.6e F\n", Cs_required);
    } else {
        printf("Required Cs cannot be computed (vrms <= 0)\n");
    }

    // capacitor mismatch
    if (device.Area > 0) {
        float sigmaC_over_C = device.AC / sqrtf(device.Area);
        printf("Capacitor mismatch (sigmaC/C) = %.6e\n", sigmaC_over_C);
    } else {
        printf("Capacitor mismatch cannot be computed (Area <= 0)\n");
    }

    // switch on resistance
        float denom = device.mu_n * device.Cox * (device.W / device.L) * overdrive;
        device.Ron = 1.0f / denom;
        printf("Estimated MOS switch on-resistance Ron = %.6e ohms\n", device.Ron);

    printf("====================================================================\n\n");
}

// Function 3
void performance_calculation(void)
{
    clear_screen();

    printf("\n>> Performance Calculation\n");
    print_device_parameters(3);     // Show values for Section 3

    // Check if all parameters are valid to proceed
    if (device.Cs <= 0 || device.Temperature <= 0)
    {
        printf("\nWARNING: Sampling noise cannot be computed.\n");
        printf("Cs and Temperature must both be > 0.\n");
        printf("Please update these values first in Section 2.\n\n");
        return;
    }

    // Ask user whether to reuse existing values or enter new ones
    int confirmation = confirm_new_input_or_use_existing();

    if (confirmation)
    {
        clear_screen();
        printf("\nEnter performance parameters (127 digits MAX!):\n");

        // These two overwrite Section 2 values if user chooses so 
        device.Cs          = get_confirmed_value("Sampling capacitor Cs (F)");
        device.Temperature = get_confirmed_value("Temperature (K)");

        device.VFS   = get_confirmed_value("Full-scale voltage VFS (V)");
        device.Nbits = (int)get_confirmed_value("ADC resolution Nbits");
    }
    else
    {
        // If user keeps existing values then VFS and Nbits must be available
        if (device.VFS <= 0)
            device.VFS = get_confirmed_value("Full-scale voltage VFS (V)");

        if (device.Nbits <= 0)
            device.Nbits = (int)get_confirmed_value("ADC resolution Nbits");
    }

    clear_screen();

    printf("\n================= Performance Calculations =================\n");

    const float k = 1.380649e-23f;     // Boltzmann constant

    // Sampling noise power & vn_rms
    float Psampling = k * device.Temperature / device.Cs;
    float vn_rms = sqrtf(Psampling);

    printf("Sampling noise power (Psampling) = %.6e V^2\n", Psampling);
    printf("Sampling noise (vn_rms) = %.6e V\n", vn_rms);

    // Quantization noise power
    float LSB = device.VFS / (float)(1u << device.Nbits);
    float Pq  = (LSB * LSB) / 12.0f;

    printf("Quantization noise power (Pq) = %.6e V^2\n", Pq);

    // Overall SNR
    float SNR = (device.VFS * device.VFS / 8.0f) / (Pq + Psampling);
    float SNRdB = 10.0f * log10f(SNR);

    printf("Overall SNR = %.6e\n", SNR);
    printf("Overall SNR (dB) = %.6f dB\n", SNRdB);

    // Effective number of bits (ENOB)
    float ENOB = (SNRdB - 1.76f) / 6.02f;
    printf("Effective Number of Bits (ENOB) = %.6f bits\n", ENOB);

    printf("================================================================\n\n");
}



void menu_item_4(void) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}
