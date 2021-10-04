#include "apiVCF.h"
#include "VBAExport.h"

int main(int argc, char* argv[])
{
	double ctl = 0.0;
	double cpl = 0.0;	
				
	int code = ApiCalculationRequest_initialize();	
				   
	double fp = _fpC11171kPa(0.0005763, 853.7, 29.18, 395.0, 15.0, &ctl, &cpl, &code);
				
	ApiCalculation_cleanVBAExport();
	ApiCalculationRequest_destroy();	
		
    return 0;
}