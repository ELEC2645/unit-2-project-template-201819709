#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"
#include <string.h>
#include <math.h>
#include <time.h>

DeviceData device = {0};    // instance of a device
CalcResults results = {0};

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

    // STORE RESULT
    results.VOV = VOV;

    // ID in linear region
    float ID_linear = device.mu_n * device.Cox * (device.W / device.L) * ((device.VGS - device.VTH) * device.VDS - (device.VDS * device.VDS) / 2.0f);
    printf("ID (linear region) = %.6f A\n", ID_linear);

    // STORE RESULT
    results.ID_linear = ID_linear;

    // ID in saturation
    float ID_saturation = 0.5f * device.mu_n * device.Cox * (device.W / device.L) * (VOV * VOV);
    printf("ID (saturation region) = %.6f A\n", ID_saturation);

    // STORE RESULT
    results.ID_saturation = ID_saturation;

    float gm;
    if (VOV != 0)                // avoid division by 0
        gm = 2.0f * ID_saturation / VOV;
    else
        gm = 0.0f;

    printf("Transconductance gm = %.6f S (A/V)\n", gm);

    // STORE RESULT
    results.gm = gm;

    float ro;
    if (ID_saturation > 0)       // avoid division by 0
        ro = 1.0f / (device.lambda * ID_saturation);
    else
        ro = 0.0f;

    printf("Output resistance ro = %.6f ohms\n", ro);

    // STORE RESULT
    results.ro = ro;

    float Av = gm * ro;          // intrinsic gain
    printf("Intrinsic gain Av = %.6f V/V\n", Av);

    // STORE RESULT
    results.Av = Av;

    printf("=======================================================\n\n");
}



// Function 2
void device_component_selection_calculations(void)
{
    clear_screen();

    printf("\n>> Device Component Selection Calculations\n");
    print_device_parameters(2);      // show Section 2 values

    float overdrive = device.VGS - device.VTH;

    if (device.mu_n <= 0 || device.Cox <= 0 || device.W <= 0 || device.L <= 0 || overdrive <= 0)
    {
        printf("\nWARNING: Current MOSFET parameters are incomplete or invalid.\n");
        printf("Ron cannot be computed with the existing values.\n");
        printf("You may want to update device parameters first.\n\n");
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

    const float k = 1.380649e-23f;

    float vn_rms = 0.0f;  // kt/c noise
    if (device.Cs > 0 && device.Temperature > 0) {
        vn_rms = sqrtf(k * device.Temperature / device.Cs);
        printf("kT/C noise (vn_rms) = %.6e V\n", vn_rms);

        results.vn_rms = vn_rms;
        results.Psampling = k * device.Temperature / device.Cs;

    } else {
        printf("kT/C noise cannot be computed (Cs or Temperature <= 0)\n");
        results.vn_rms = 0.0f;
        results.Psampling = 0.0f;
    }

    if (device.vrms > 0) {
        float Cs_required = k * device.Temperature / (device.vrms * device.vrms);
        printf("Required sampling capacitance Cs = %.6e F\n", Cs_required);

        results.Cs_required = Cs_required;
    } else {
        printf("Required Cs cannot be computed (vrms <= 0)\n");
        results.Cs_required = 0.0f;
    }

    if (device.Area > 0) {
        float sigmaC_over_C = device.AC / sqrtf(device.Area);
        printf("Capacitor mismatch (sigmaC/C) = %.6e\n", sigmaC_over_C);

        results.mismatch = sigmaC_over_C;
    } else {
        printf("Capacitor mismatch cannot be computed (Area <= 0)\n");
        results.mismatch = 0.0f;
    }

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

    results.Psampling = Psampling;
    results.vn_rms    = vn_rms;

    // Quantization noise power
    float LSB = device.VFS / (float)(1u << device.Nbits);
    float Pq  = (LSB * LSB) / 12.0f;

    printf("Quantization noise power (Pq) = %.6e V^2\n", Pq);

    results.Pq = Pq;

    // Overall SNR
    float SNR = (device.VFS * device.VFS / 8.0f) / (Pq + Psampling);
    float SNRdB = 10.0f * log10f(SNR);

    printf("Overall SNR = %.6e\n", SNR);
    printf("Overall SNR (dB) = %.6f dB\n", SNRdB);

    results.SNR_linear = SNR;
    results.SNR_dB     = SNRdB;

    // Effective number of bits (ENOB)
    float ENOB = (SNRdB - 1.76f) / 6.02f;
    printf("Effective Number of Bits (ENOB) = %.6f bits\n", ENOB);

    results.ENOB = ENOB;

    printf("================================================================\n\n");

}


void modify_single_parameter(void)
{
    clear_screen();

    printf("\n>> Modify Individual Device Parameters\n");
    print_device_parameters(0);   // print all parameters

    printf("\nType the parameter name to modify.\n");
    printf("Accepted names:\n");
    printf("  mu_n, Cox, W, L, VGS, VTH, VDS, lambda\n");
    printf("  Cs, Temperature, vrms, AC, Area\n");
    printf("  VFS, Nbits\n");
    printf("  Or type 'back' to return.\n");
    printf("\nAll inputs are case-sensitive and must match the prompt exactly!\n");

    char input[64];

    for (;;) {

        printf("\nEnter parameter name: ");

        if (!fgets(input, sizeof(input), stdin)) {
            printf("Input error. Exiting.\n");
            exit(1);
        }

        input[strcspn(input, "\r\n")] = '\0';

        if (strcmp(input, "back") == 0) {
            return;    
        }

        if (strcmp(input, "mu_n") == 0) {
            device.mu_n = get_confirmed_value("electron mobility mu_n");
        }
        else if (strcmp(input, "Cox") == 0) {
            device.Cox = get_confirmed_value("oxide capacitance Cox");
        }
        else if (strcmp(input, "W") == 0) {
            device.W = get_confirmed_value("channel width W");
        }
        else if (strcmp(input, "L") == 0) {
            device.L = get_confirmed_value("channel length L");
        }
        else if (strcmp(input, "VGS") == 0) {
            device.VGS = get_confirmed_value("gate-source voltage VGS");
        }
        else if (strcmp(input, "VTH") == 0) {
            device.VTH = get_confirmed_value("threshold voltage VTH");
        }
        else if (strcmp(input, "VDS") == 0) {
            device.VDS = get_confirmed_value("drain-source voltage VDS");
        }
        else if (strcmp(input, "lambda") == 0) {
            device.lambda = get_confirmed_value("lambda");
        }
        else if (strcmp(input, "Cs") == 0) {
            device.Cs = get_confirmed_value("sampling capacitor Cs");
        }
        else if (strcmp(input, "Temperature") == 0) {
            device.Temperature = get_confirmed_value("Temperature (K)");
        }
        else if (strcmp(input, "vrms") == 0) {
            device.vrms = get_confirmed_value("target RMS noise");
        }
        else if (strcmp(input, "AC") == 0) {
            device.AC = get_confirmed_value("Pelgrom coefficient AC");
        }
        else if (strcmp(input, "Area") == 0) {
            device.Area = get_confirmed_value("device area");
        }
        else if (strcmp(input, "VFS") == 0) {
            device.VFS = get_confirmed_value("full-scale voltage VFS");
        }
        else if (strcmp(input, "Nbits") == 0) {
            device.Nbits = (int)get_confirmed_value("ADC resolution Nbits");
        }
        else {
            printf("Unknown name. Please try again.\n");
            continue;
        }

        // After successful modification
        printf("\nParameter updated successfully.\n");
        print_device_parameters(0);
        printf("\nModify another (or type 'back'):\n");
    }
}


void export_all_parameters_to_file(void)
{
    FILE *fp = fopen("device_report.csv", "w");
        if (!fp) {
            printf("ERROR: Unable to create CSV file.\n");
            return;
        }

        // Add a timestamp
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fprintf(fp, "# Report generated on: %04d-%02d-%02d %02d:%02d:%02d\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

        // Row 1
        fprintf(fp,
            "mu_n,Cox,W,L,VGS,VTH,VDS,lambda,Temperature,AC,Area,Cs,vrms,Ron,VFS,Nbits,VOV,ID_linear,ID_saturation,gm,ro,Av,vn_rms,Cs_required,mismatch,Psampling,Pq,SNR_linear,SNR_dB,ENOB\n");

        // Row 2
        fprintf(fp,"%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%d,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g,%.10g\n",

            device.mu_n, device.Cox, device.W, device.L,device.VGS, device.VTH, device.VDS, device.lambda,device.Temperature, device.AC, device.Area, device.Cs,device.vrms, device.Ron,device.VFS, device.Nbits,

            results.VOV,results.ID_linear,results.ID_saturation,results.gm,results.ro,results.Av,results.vn_rms,results.Cs_required,results.mismatch,results.Psampling,results.Pq,results.SNR_linear,results.SNR_dB,results.ENOB
        );

        fclose(fp);

        printf("\nCSV successfully generated\n");
}
