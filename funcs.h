#ifndef FUNCS_H
#define FUNCS_H

void Calculate_MOS_device_parameter(void);
void menu_item_2(void);
void menu_item_3(void);
void menu_item_4(void);
float get_user_input_custom(const char *variable_name);
float get_confirmed_value(const char *variable_name);
int is_number(const char *s);
void print_device_parameters(void);
int confirm_new_input_or_use_existing(void);

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
    float vrms;          // rms noise if you want user-input option
    float Ron;           // on-resistance from μn, Cox, etc.


    float VFS;           // full-scale voltage
    int   Nbits;         // ADC resolution bits

} DeviceData;

extern DeviceData device;



#endif