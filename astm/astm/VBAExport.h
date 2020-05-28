/*
 * Copyright (c) 2003 American Petroleum Institute
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL WIARDA AND
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; BUSINESS INTERRUPTION; OR ENVIRONMENTAL IMPACTS) 
 * HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 */

#ifndef VBA_EXPORT_H
#define VBA_EXPORT_H

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

#include "apiVCF.h"

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

dll_export void ApiCalculation_cleanVBAExport();

dll_export int _ApiCalculation_Perform(int commodity,
 				        double alpha60,  int au,
   				        double obTemp,   int tuo,
				        double altTemp,  int tua,
				        double obPres,   int puo,
				        double altPres,  int pua,
				        double obDens,   int duo,
				        double baseTemp, int tub,
				        double basePres, int pub,
						double *ctlObToBase, double *cplObToBase,
						double *ctlBaseToAlt, double *cplBaseToAlt,
						double *baseDens, double *altDens);

/** 
 *@file VBAExport.h
* Contains function that can be called from VBA.
*
* These function wrap the functions in the calculation group to make 
* them easier to call from VBA.
*/ 

/**  @defgroup vbFunctions VBA functions  */
/*@{*/


/*--------------------------------------------------------------
*   11.1.6.1 type calculations - get ctpl
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplA11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplA11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplB11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/			 
dll_export double _ctplB11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplD11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplD11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplC11161API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplC11161API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/*-------- Density in Relative Density ------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplA11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplA11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplB11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
	
/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplB11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);


/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplD11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplD11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);


/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplC11161RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 int *errorCode);
	
/**
* Calculate the ctpl factor for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplC11161RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/*--------------------------------------------------------------
*   11.1.7.1 type calculations - get ctpl
*--------------------------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplA11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplA11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl, 
			   double *cpl,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplB11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplB11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);
			   
/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplD11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplD11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplC11171bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplC11171bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);
			   
/*--------  Base Temperature 1 5C, Pressure in kPa -------------*/

/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplA11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
			  
/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplA11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0kPa
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplB11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
	
/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0kPa
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplB11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);


/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplD11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplD11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplC11171kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
			   
/**
* Calculate the ctpl factor for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from base to alternate conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplC11171kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);


/*--------------------------------------------------------------
*   11.1.6.2 type calculations - get ctpl
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the ctpl factor for transformation observed conditioons to
* 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at observed conditions in units of API
* @param temp the observed temperature in F
* @param pres the observed  pressure in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplA11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);	 
	
/**
* Calculate the ctpl factor for transformation observed conditioons to
* 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at observed conditions in units of API
* @param temp the observed temperature in F
* @param pres the observed  pressure in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplA11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the ctpl factor for transformation from
* observed conditions to  60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at observed conditions in units of API
* @param temp the  the observed temperature in F
* @param pres the observed  pressure in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from  alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplB11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);
	
/**
* Calculate the ctpl factor for transformation from
* observed conditions to  60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at observed conditions in units of API
* @param temp the  the observed temperature in F
* @param pres the observed  pressure in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from  alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplB11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/**
* Calculate the ctpl factor for transformation from 
* alternate conditions to 60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of API
* @param temp the temperature at the  alternate conditions in F
* @param pres the pressure at the alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplD11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);
			
/**
* Calculate the ctpl factor for transformation from 
* alternate conditions to 60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of API
* @param temp the temperature at the  alternate conditions in F
* @param pres the pressure at the alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplD11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the ctpl factor for transformation from  alternate 
* conditions to 60F, 0psi to for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alterante conditions in units of API
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplC11162API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
		     int *errorCode);
		
/**
* Calculate the ctpl factor for transformation from  alternate 
* conditions to 60F, 0psi to for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alterante conditions in units of API
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplC11162API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/*-------- Density in Relative Density ------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from alternate conditions
* to  60F, 0psi to for Crude Oil
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at  alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplA11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
			 
/**
* Calculate the ctpl factor for transformation from alternate conditions
* to  60F, 0psi to for Crude Oil
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at  alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplA11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions
* to 60F, 0psi for  Refined products
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplB11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions
* to 60F, 0psi for  Refined products
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplB11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to
*  60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplD11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to
*  60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplD11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);


/**
* Calculate the ctpl factor for transformation from alternate 
* conditions to 60F, 0psi  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplC11162RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 int *errorCode);
	
/**
* Calculate the ctpl factor for transformation from alternate 
* conditions to 60F, 0psi  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplC11162RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/*--------------------------------------------------------------
*   11.1.7.2 type calculations - get ctpl
*--------------------------------------------------------------*/

/**
* Calculate the ctpl factor for transformation from 
* alternate conditions to baseTemp in C, 0bar  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at  lternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplA11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
	
/**
* Calculate the ctpl factor for transformation from 
* alternate conditions to baseTemp in C, 0bar  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at  lternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplA11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0bar for  Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplB11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0bar for  Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplB11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);


/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0bar to for Lubircation oil.
*
* @param density the density at alternateconditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplD11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0bar to for Lubircation oil.
*
* @param density the density at alternateconditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplD11172bar(double density, 
		       double temp,
		       double pres,
			   double baseTemp,
			   double *ctl,
			   double *cpl,		       
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to
*  baseTemp in C, 0bar for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplC11172bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to
*  baseTemp in C, 0bar for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplC11172bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/*--------  Base Temperature 1 5C, Pressure in kPa -------------*/

/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplA11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
		
/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplA11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to 
* baseTemp in C, 0kPa for Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplB11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
	
/**
* Calculate the ctpl factor for transformation from alternate conditions to 
* baseTemp in C, 0kPa for Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplB11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl, 
			   double *cpl,
		       int *errorCode);


/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Lubircation oil.
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplD11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Lubircation oil.
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplD11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);


/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at  alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double ctplC11172kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
	
/**
* Calculate the ctpl factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at  alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the ctpl factor to transform from alternate to base  conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _ctplC11172kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl, 
		       int *errorCode);

/** Density functions */
/*--------------------------------------------------------------
*   11.1.6.1 type calculations - get dens
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the density in API at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densA11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);
	
/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in API at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densA11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the density  for transformation from 60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the density in API at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densB11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);
	
/**
* Calculate the density  for transformation from 60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in API at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densB11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the  density in API at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densD11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the  density in API at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densD11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the density in API at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densC11161API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
		     int *errorCode);
	
/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in API at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densC11161API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/*-------- Density in Relative Density ------------------------------------------*/

/**
* Calculate the density  for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densA11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
		
/**
* Calculate the density  for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densA11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densB11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densB11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);
			 

/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densD11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
	
/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densD11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);


/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densC11161RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 int *errorCode);
			 
/**
* Calculate the density for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densC11161RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/*--------------------------------------------------------------
*   11.1.7.1 type calculations - get dens
*--------------------------------------------------------------*/

/**
* Calculate the density for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densA11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
	
/**
* Calculate the density for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densA11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densB11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densB11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);


/**
* Calculate the density  for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densD11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density  for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densD11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density  for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densC11171bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density  for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densC11171bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);
			   
/*--------  Base Temperature 1 5C, Pressure in kPa -------------*/

/**
* Calculate the density  for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densA11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
		
/**
* Calculate the density  for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densA11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density for transformation from baseTemp in C, 0kPa
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densB11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
	
/**
* Calculate the density for transformation from baseTemp in C, 0kPa
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densB11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return   the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densD11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return   the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densD11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density  for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densC11171kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
			   
/**
* Calculate the density  for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the  density in kg/m^3 at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densC11171kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl, 
			   double *cpl,
		       int *errorCode);
			   
/*--------------------------------------------------------------
*   11.1.6.2 type calculations - get dens
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the density for transformation observed conditioons to
* 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at observed conditions in units of API
* @param temp the observed temperature in F
* @param pres the observed  pressure in psi
* @param errorCode a variable to report error conditions
*
* @return the density in API at base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densA11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);
		
/**
* Calculate the density for transformation observed conditioons to
* 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at observed conditions in units of API
* @param temp the observed temperature in F
* @param pres the observed  pressure in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in API at base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densA11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);			 

/**
* Calculate the density  for transformation from
* observed conditions to  60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at observed conditions in units of API
* @param temp the  the observed temperature in F
* @param pres the observed  pressure in psi
* @param errorCode a variable to report error conditions
*
* @return the density in API at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densB11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);
	
/**
* Calculate the density  for transformation from
* observed conditions to  60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at observed conditions in units of API
* @param temp the  the observed temperature in F
* @param pres the observed  pressure in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in API at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densB11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl, 
			 double *cpl, 
		     int *errorCode);


/**
* Calculate the density for transformation from 
* alternate conditions to 60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of API
* @param temp the temperature at the  alternate conditions in F
* @param pres the pressure at the alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the density in API at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densD11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the density for transformation from 
* alternate conditions to 60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of API
* @param temp the temperature at the  alternate conditions in F
* @param pres the pressure at the alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the density in API at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densD11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl, 
		     int *errorCode);

/**
* Calculate the density  for transformation from  alternate 
* conditions to 60F, 0psi to for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alterante conditions in units of API
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the  density in API at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densC11162API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
		     int *errorCode);
			 
/**
* Calculate the density  for transformation from  alternate 
* conditions to 60F, 0psi to for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alterante conditions in units of API
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the  density in API at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densC11162API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl, 
		     int *errorCode);


/*-------- Density in Relative Density ------------------------------------------*/

/**
* Calculate the density for transformation from alternate conditions
* to  60F, 0psi to for Crude Oil
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at  alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the  density in Rel. Dens. units  at base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densA11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
			
/**
* Calculate the density for transformation from alternate conditions
* to  60F, 0psi to for Crude Oil
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at  alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the  density in Rel. Dens. units  at base conditions.
*          The returned factor is rounded. 
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densA11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the density  for transformation from alternate conditions
* to 60F, 0psi for  Refined products
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densB11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the density  for transformation from alternate conditions
* to 60F, 0psi for  Refined products
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in Rel. Dens. units  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densB11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
*  60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the density in Rel. Dens. units  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densD11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
*  60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the density in Rel. Dens. units  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densD11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the density for transformation from alternate 
* conditions to 60F, 0psi  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the density in Rel. Dens. units  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densC11162RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 int *errorCode);
	
/**
* Calculate the density for transformation from alternate 
* conditions to 60F, 0psi  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the density in Rel. Dens. units  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densC11162RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/*--------------------------------------------------------------
*   11.1.7.2 type calculations - get dens
*--------------------------------------------------------------*/

/**
* Calculate the density for transformation from 
* alternate conditions to baseTemp in C, 0bar  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at  lternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densA11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density for transformation from 
* alternate conditions to baseTemp in C, 0bar  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at  lternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densA11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0bar for  Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return   the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/

dll_export double densB11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0bar for  Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return   the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densB11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0bar to for Lubircation oil.
*
* @param density the density at alternateconditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return   the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densD11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0bar to for Lubircation oil.
*
* @param density the density at alternateconditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return   the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densD11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density  for transformation from alternate conditions to
*  baseTemp in C, 0bar for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densC11172bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density  for transformation from alternate conditions to
*  baseTemp in C, 0bar for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densC11172bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/*--------  Base Temperature 1 5C, Pressure in kPa -------------*/

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densA11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
		
/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densA11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to 
* baseTemp in C, 0kPa for Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densB11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to 
* baseTemp in C, 0kPa for Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densB11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Lubircation oil.
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densD11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Lubircation oil.
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densD11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0kPa  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at  alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double densC11172kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the density for transformation from alternate conditions to
* baseTemp in C, 0kPa  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at  alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the density in kg/m^3 at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _densC11172kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);			   

/** Scaled compensation  factor */
/*--------------------------------------------------------------
*   11.1.6.1 type calculations - get dens
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the scaled comp. factor  for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpA11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);
			 
/**
* Calculate the scaled comp. factor  for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpA11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpB11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpB11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpD11161API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);
			 
/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpD11161API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpC11161API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
		     int *errorCode);
		
/**
* Calculate the scaled comp. factor  for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of API
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpC11161API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/*-------- Density in Relative Density ------------------------------------------*/

/**
* Calculate the scaled comp. factor  for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpA11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
		
/**
* Calculate the scaled comp. factor  for transformation from 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpA11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from 60F, 0psi to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpB11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from 60F, 0psi to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpB11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);			 

/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpD11161RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
	
/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpD11161RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);


/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpC11161RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 int *errorCode);
			
/**
* Calculate the scaled comp. factor for transformation from 60F, 0psi to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at base conditions in units of Rel. Dens.
* @param temp the temperature at the desired alternate conditions in F
* @param pres the pressure at the desired alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/psi  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpC11161RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/*--------------------------------------------------------------
*   11.1.7.1 type calculations - get dens
*--------------------------------------------------------------*/

/**
* Calculate the scaled comp  for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpA11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
			   
/**
* Calculate the scaled comp  for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpA11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpB11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
			 
/**
* Calculate the scaled comp. factor for transformation from 
* baseTemp in C, 0bar to
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpB11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);


/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpD11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpD11171bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpC11171bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
			   
/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0bar to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in bar
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpC11171bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);


/*--------  Base Temperature 1 5C, Pressure in kPa -------------*/

/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpA11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
			   
/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Crude Oil
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpA11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0kPa
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpB11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0kPa
* the desired alternate conditions Refined products
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpB11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl, 
			   double *cpl,
		       int *errorCode);
			   
/**
* Calculate the scaled comp. factor transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpD11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the scaled comp. factor transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for Lubircation oil.
*
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpD11171kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpC11171kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
		
/**
* Calculate the scaled comp. factor  for transformation from baseTemp in C, 0kPa to
* the desired alternate conditions for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at base conditions in units of kg/m^3
* @param temp the temperature at the desired alternate conditions in C
* @param pres the pressure at the desired alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa  at alternate conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpC11171kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);


/*--------------------------------------------------------------
*   11.1.6.2 type calculations - get dens
*--------------------------------------------------------------*/

/*-------- Density in API ------------------------------------------*/

/**
* Calculate the scaled comp. factor  for transformation observed conditioons to
* 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at observed conditions in units of API
* @param temp the observed temperature in F
* @param pres the observed  pressure in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpA11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation observed conditioons to
* 60F, 0psi to
* the desired alternate conditions for Crude Oil
*
* @param density the density at observed conditions in units of API
* @param temp the observed temperature in F
* @param pres the observed  pressure in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpA11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);
			 
/**
* Calculate the scaled comp. factor  for transformation from
* observed conditions to  60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at observed conditions in units of API
* @param temp the  the observed temperature in F
* @param pres the observed  pressure in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpB11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from
* observed conditions to  60F, 0psi to
* the desired alternate conditions for Refined products
*
* @param density the density at observed conditions in units of API
* @param temp the  the observed temperature in F
* @param pres the observed  pressure in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpB11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/**
* Calculate the scaled comp. factor  for transformation from 
* alternate conditions to 60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of API
* @param temp the temperature at the  alternate conditions in F
* @param pres the pressure at the alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpD11162API(double density, 
		     double temp, 
		     double pres,
		     int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from 
* alternate conditions to 60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of API
* @param temp the temperature at the  alternate conditions in F
* @param pres the pressure at the alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpD11162API(double density, 
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);


/**
* Calculate the scaled comp. factor  for transformation from  alternate 
* conditions to 60F, 0psi to for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alterante conditions in units of API
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpC11162API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
		     int *errorCode);
		
/**
* Calculate the scaled comp. factor  for transformation from  alternate 
* conditions to 60F, 0psi to for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alterante conditions in units of API
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpC11162API(double alpha60,
		     double density,
		     double temp, 
		     double pres,
			 double *ctl,
			 double *cpl,
		     int *errorCode);

/*-------- Density in Relative Density ------------------------------------------*/

/**
* Calculate the scaled comp. factor for transformation from alternate conditions
* to  60F, 0psi to for Crude Oil
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at  alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpA11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
		
/**
* Calculate the scaled comp. factor for transformation from alternate conditions
* to  60F, 0psi to for Crude Oil
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at  alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpA11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from alternate conditions
* to 60F, 0psi for  Refined products
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpB11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);
	
/**
* Calculate the scaled comp. factor  for transformation from alternate conditions
* to 60F, 0psi for  Refined products
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpB11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);


/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
*  60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpD11162RelDens(double density, 
			 double temp, 
			 double pres,
			 int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
*  60F, 0psi for Lubircation oil.
*
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpD11162RelDens(double density, 
			 double temp, 
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from alternate 
* conditions to 60F, 0psi  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpC11162RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 int *errorCode);
			 
/**
* Calculate the scaled comp. factor  for transformation from alternate 
* conditions to 60F, 0psi  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/F
* @param density the density at alternate conditions in units of Rel. Dens.
* @param temp the temperature at alternate conditions in F
* @param pres the pressure at alternate conditions in psi
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/psi  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpC11162RelDens(double alpha60,
			 double density,
			 double temp,
			 double pres,
			 double *ctl,
			 double *cpl,
			 int *errorCode);

/*--------------------------------------------------------------
*   11.1.7.2 type calculations - get dens
*--------------------------------------------------------------*/

/**
* Calculate the scaled comp. factor for transformation from 
* alternate conditions to baseTemp in C, 0kPa  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at  lternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpA11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
	
/**
* Calculate the scaled comp. factor for transformation from 
* alternate conditions to baseTemp in C, 0kPa  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at  lternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpA11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
* baseTemp in C, 0kPa for  Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpB11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
* baseTemp in C, 0kPa for  Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpB11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
* baseTemp in C, 0kPa to for Lubircation oil.
*
* @param density the density at alternateconditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpD11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
* baseTemp in C, 0kPa to for Lubircation oil.
*
* @param density the density at alternateconditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpD11172bar(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the scaled comp factor  for transformation from alternate conditions to
*  baseTemp in C, 0kPa for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpC11172bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
	
/**
* Calculate the scaled comp factor  for transformation from alternate conditions to
*  baseTemp in C, 0kPa for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in bar
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/bar  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpC11172bar(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl, 
		       int *errorCode);


/*--------  Base Temperature 1 5C, Pressure in kPa -------------*/

/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/kPa at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpA11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
		
/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Crude Oil
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/kPa at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpA11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl,
			   double *cpl,
		       int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from alternate conditions to 
* baseTemp in C, 0kPa for Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/kPa  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpB11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from alternate conditions to 
* baseTemp in C, 0kPa for Refined products
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return the scaled compensation factor in 1/kPa  at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpB11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl, 
			   double *cpl, 
		       int *errorCode);
			   
/**
* Calculate the scaled comp. factor  for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Lubircation oil.
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpD11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);

/**
* Calculate the scaled comp. factor  for transformation from alternate conditions to
* baseTemp in C, 0kPa  for Lubircation oil.
*
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpD11172kPa(double density, 
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl, 
			   double *cpl, 
		       int *errorCode);

/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at  alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double fpC11172kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
		       int *errorCode);
			   
/**
* Calculate the scaled comp. factor for transformation from alternate conditions to
* baseTemp in C, 0kPa  for a supplied value of alpha60.
*
* @param alpha60 the alpha60 value in units of 1/C
* @param density the density at alternate conditions in units of kg/m^3
* @param temp the temperature at  alternate conditions in C
* @param pres the pressure at alternate conditions in kPa
* @param baseTemp the desired base temperature in C
* @param ctl correction for the effect of temperature on the liquid
* @param cpl correction for the effect of pressure on the liquid
* @param errorCode a variable to report error conditions
*
* @return  the scaled compensation factor in 1/kPa at base conditions.
*          If an error occured, 0 will be returned and the errorCode is set.
*/
dll_export double _fpC11172kPa(double alpha60,
		       double density,
		       double temp,
		       double pres,
		       double baseTemp,
			   double *ctl, 
			   double *cpl, 
		       int *errorCode);
/*@}*/
/* close protective C++ bracket */


#ifdef __cplusplus
}
#endif

#endif /* VBA_EXPORT_H  */
