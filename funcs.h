#ifndef FUNCS_H
#define FUNCS_H

void Calculate_MOS_device_parameter(void);
void device_component_selection_calculations(void);
void performance_calculation(void);
void export_all_parameters_to_file(void);
float get_user_input_custom(const char *variable_name);
float get_confirmed_value(const char *variable_name);
int is_number(const char *s);
void print_device_parameters(int choice);
int confirm_new_input_or_use_existing(void);
void clear_screen(void);
void modify_single_parameter(void);

typedef struct {

    float mu_n;      // electron mobility
    float Cox;       // oxide capacitance
    float W;         // channel width
    float L;         // channel length
    float VGS;       // gate-source voltage
    float VTH;       // threshold voltage
    float VDS;       // drain-source voltage
    float lambda;    // modulation parameter


    float Temperature;   // T in Kelvin
    float AC;            // Pelgrom mismatch coefficient
    float Area;          // device area for Pelgrom
    float Cs;            // capacitor value (kT/C)
    float vrms;          // rms noise
    float Ron;           // on-resistance from μn, Cox, etc.


    float VFS;           // full-scale voltage
    int   Nbits;         // ADC resolution bits

} DeviceData;

extern DeviceData device;


typedef struct
{
    float VOV;
    float ID_linear;
    float ID_saturation;
    float gm;
    float ro;
    float Av;

    float vn_rms;
    float Cs_required;
    float mismatch;

    float Psampling;
    float Pq;
    float SNR_linear;
    float SNR_dB;
    float ENOB;

} CalcResults;

extern CalcResults results;

#endif