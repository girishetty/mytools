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
#ifndef API_CONSTANTS_H
#define API_CONSTANTS_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/** @file constants.h
* This file contains all the definitions for constants used in the calculation.
* This includes values for the base parameters as well as limits for the 
* density and conversion parameters.
* <p>
* If you need to change any of these constants, please recompile the
* <b>ENTIRE</B> project as the constants are set at compile time.
*/ 

/**  @defgroup constGroup Calculation Constants */
/*@{*/

/**
* Define 0 for assigning to pointers
*/
#define NULL 0

#ifndef __cplusplus
/** A type definition for a boolean type value. */
typedef int bool;
#endif


/**
* Define  a value for boolean true
*/
#define TRUE 1


/**
* Define  a value for boolean false
*/
#define FALSE 0

/**
* The line break character to use.
* The actual line break character depends on the operating system.
*/
#define API_EOL "\n"


/** 
 * The maximal difference between two double numbers that are considered equal. 
*  Thus if  a and b are doubles and 
*  |a - b| < API_EPSILON the numbers are considered
* equal.
*/
#define API_EPSILON  1e-7

/**
* The conversion criteria. 
* The iteration procedure described in Paragraph 11.1.6.2, page 51
* defines a  limit  for the difference between rho(m+1) and rho(m)
* below which it is assumed that conversion has been reached. This
* is  the limit used for the test of conversion 
*/
#define API_CONVERSION_CRITERIA  0.000001


/** 
* Slope value to convert temperture values.
* As of paragraph 11.1.5.1, page 22 the formula to convert
* temperature from  C to F is:<br>
* \f$ T[^{0}F] = a * T[ ^{0}C] + m \f$  <br>
* This constant defines the value of the slope.
*/
#define API_TEMP_C_TO_F_SLOPE  1.8 

/** 
* Intercept value to convert temperture values.
* As of paragraph 11.1.5.1, page 22 the formula to convert
* temperature from  C to F is:<br>
* \f$ T[^{0}F] = a * T[ ^{0}C] + m \f$  <br>
* This constant defines the value of the intercept..
*/
#define API_TEMP_C_TO_F_INTERCEPT 32.0

/**
* Rounding increment for  temperature in C.
* See paragraph 1.1.5.4, page 30.
*/
#define API_TEMP_C_ROUNDING_INCREMENT  0.05

/**
* Rounding increment for  temperature in F.
* See paragraph 1.1.5.4, page 30.
*/
#define API_TEMP_F_ROUNDING_INCREMENT  0.1


/** 
 * The scale factor  to correct temperature to IPTS-68 base.
* This defines the factor used in step 3 of the procedure to 
* correct temperature to IPTS-68 base.
* See paragraph 11.1.5.3, page 28.
*/
#define API_TEMP_IPTS68_SCALER  630.0

/** 
* The coefficients in the power series used to 
* correct temperature to IPTS-68 base.
* See paragraph 11.1.5.3, page 28.
*/
#define API_TEMP_IPTS68_CONSTANTS  {-0.148759, -0.267408, 1.080760, 1.269056, -4.089591, -1.871251, 7.438081, -3.536296}  


/**
* Rounding increment for  pressure in psi.
* See paragraph 1.1.5.4, page 30.
*/
#define  API_RESSURE_PSI_ROUNDING_INCREMENT  1.0

/**
* Rounding increment for  pressure in kPa.
* See paragraph 1.1.5.4, page 30.
*/
#define API_PRESSURE_KPA_ROUNDING_INCREMENT  5.0

/**
* Rounding increment for  pressure in bar.
* See paragraph 1.1.5.4, page 30.
*/
#define API_PRESSURE_BAR_ROUNDING_INCREMENT  0.05

/**
* Slope constants to convert pressure values. 
* As of aragraph 11.1.5.1, page 22 the formula to convert
* pressure from kPa  to psi is:<br>  
* \f$ P[psi] = a * P[kPa]\f$  <br>
* This  constants defines the value for a.
*/ 
#define API_PRESSURE_KPA_TO_PSI_FACTOR  6.894757

/**
*  Slope constants to convert pressure values.
* As of paragraph 11.1.5.1, page 22 the formula to convert
* pressure from kPa  to psi is: <br> 
*   \f$ P[psi] = a * P[bar]\f$ <br>
*  This constants defines the slope.
*/ 
#define API_PRESSURE_BAR_TO_PSI_FACTOR  0.06894757

/**
* Rounding increment for  density in kg/m^3.
* See paragraph 1.1.5.4, page 30.
*/
#define API_DENSITY_KGM3_ROUNDING_INCREMENT  0.1

/**
* Rounding increment for  density in API.
* See paragraph 1.1.5.4, page 30.
*/
#define API_DENSITY_API_ROUNDING_INCREMENT  0.1

/**
* Rounding increment for  density in Relative Density units.
* See paragraph 1.1.5.4, page 30.
*/
#define API_DENSITY_RELATIVE_ROUNDING_INCREMENT  0.0001

/**
* Conversion factor for pressure from API to  kg/m^3.
* As of Paragraph 11.1.5.1, page 22 the factor to convert 
* density in relative density to density in kg/m^3 is the 
* density of water. This value is defined here 
*/ 
#define API_DENSITY_RELATIVE_TO_KGM3_FACTOR  999.016

/**
* Conversion factor to transform from API to kg/m^3. 
* As of Paragraph 11.1.5.1, page 22 the formula to transform
* density in API to kg/m^3 is: <br>
* \f$ \rho [kg/m^{3}] = \frac{ a * b} {\rho[API] + c} \f$<br>
* This is the constant a to use. The constant b is 
* #API_DENSITY_RELATIVE_TO_KGM3_FACTOR.
*/
#define API_DENSITY_API_TO_KGM3_UPPER  141.5

/**
* Conversion factor to transform from API to kg/m^3. 
* As of Paragraph 11.1.5.1, page 22 the formula to transform
* density in API to kg/m^3 is: <br>
* \f$ \rho [kg/m^{3}] = \frac{ a * b} {\rho[API] + c} \f$<br>
* This is the constant c to use. The constant b is 
* #API_DENSITY_RELATIVE_TO_KGM3_FACTOR.
*/
#define API_DENSITY_API_TO_KGM3_LOWER 131.5

/**
* Rounding increment for  Thermal Expansion Factor in 1/F units.
* See paragraph 1.1.5.4, page 30.
*/
#define API_THERMAL_EXPANSION_F_ROUNDING_INCREMENT  0.1e-6

/**
* Rounding increment for  Thermal Expansion Factor in 1/C units.
* See paragraph 1.1.5.4, page 30.
*/
#define API_THERMAL_EXPANSION_C_ROUNDING_INCREMENT  0.2e-6
  
/** 
 * Conversion factor for thermal expansion quantity.
* As of Paragraph 1.1.5.1, page 22 
* the factor to use to transform the Thermal Expansion Factor
* between 1/F and 1/C.
*/
#define API_THERMAL_EXPANSION_REVF_TO_REVC  1.8

/**
* Rounding increment for any of the expansion factors.
* The expansion factors are 
* CTL, CPL and CTPL.  See paragraph 1.1.5.4, page 30.
*/
#define API_EXPANSION_FACTOR_ROUNDING_INCREMENT  0.00001

/**
* Rounding increment for  Scaled Compressibility Factor in 1/psi units
* See paragraph 1.1.5.4, page 30.
*/
#define API_SCALED_COMP_REV_PSI_ROUNDING_INCREMENT  0.001

/**
* Rounding increment for  Scaled Compressibility Factor in 1/kPa units
* See paragraph 1.1.5.4, page 30.
*/
#define API_SCALED_COMP_REV_KPA_ROUNDING_INCREMENT  0.0001

/**
* Rounding increment for  Scaled Compressibility Factor in 1/bar units
* See paragraph 1.1.5.4, page 30.
*/
#define API_SCALED_COMP_REV_BAR_ROUNDING_INCREMENT  0.01

/** 
* The K-Values  defined in Paragraph 11.1.6.1, page34  
* for Crude Oil 
*/
#define API_CRUDE_OIL_KVALUES  {341.0957, 0.0, 0.0}
 
/** 
* The K-Values  defined in Paragraph 11.1.6.1, page34  
* for Fuel Oil 
*/
#define API_FUEL_OIL_KVALUES  {103.8720, 0.2701, 0.0}

/** 
* The K-Values  defined in Paragraph 11.1.6.1, page 34  
* for Jet Fuel 
*/
#define API_JET_FUEL_KVALUES {330.3010, 0.0, 0.0}

/** 
* The K-Values  defined in Paragraph 11.1.6.1, page 34  
* for Transition Zone commodities 
*/
#define API_TRANSITION_ZONE_KVALUES {1489.0670, 0.0, -0.00186840}

/** 
* The K-Values  defined in Paragraph 11.1.6.1, page 34  
* for Gasoline
*/
#define API_GASOLINE_KVALUES {192.4571, 0.2438, 0.0} 

/** 
* The K-Values  defined in Paragraph 11.1.6.1, page 34  
* for Lubrication Oil
*/
#define API_LUBRICATION_OIL_KVALUES  {0.0, 0.34878, 0.0}

/** 
* The D_alpha value defined in  Paragraph 11.1.6.2, page  54
* for Crude Oil
*/
#define API_CRUDE_OIL_D_ALPHA  2.0

/** 
* The D_alpha value defined in  Paragraph 11.1.6.2, page 54
* for Fuel Oil
*/
#define API_FUEL_OIL_D_ALPHA  1.3

/** 
* The D_alpha value defined in  Paragraph 11.1.6.2, page 54
* for Jet Fuel
*/
#define API_JET_FUEL_D_ALPHA  2.0

/** 
* The D_alpha value defined in  Paragraph 11.1.6.2, page 54
* for Transition Zone commodities.
*/
#define API_TRANSITION_ZONE_D_ALPHA  8.5

/** 
* The D_alpha value defined in  Paragraph 11.1.6.2, page 54
* for Gasoline.
*/
#define API_GASOLINE_D_ALPHA  1.5

/** 
* The D_alpha value defined in  Paragraph 11.1.6.2, page 54
* for Lubrication Oil.
*/
#define API_LUBRICATION_OIL_D_ALPHA  1.0

/** 
* The delta_60 value used in Paragraph 11.1.6.1, page 34 
* in Step 5
*/
#define API_DELTA_60  0.01374979547

/** 
* The correction factor for the temperature used in
*  Paragraph 11.1.6.1, page 34  in Step 5
*/
#define API_IPTS_68_BASE  60.0068749

/** 
* The procedure to calculate Fp is given in paragraph 11.1.6.1, Step 6, page 35.
* The formula is:<br>
*     \f$ \exp \left ( fp[0] + fp[1] * t^{*} +
*          \frac { fp[2] + fp[3] * f^{*} }{ \rho^{*2} } \right ) \f$<br>
*
* Here we define the factors fp[0] - fp[3]
*/
#define API_FP_STEP_6_FACTOR  { -1.9947, 0.00013427, 793920.0 , 2326.0 }
  
/**
*  For procedure 11.1.6.2, Step 5, page 51 the formula to calculate 
*  dp is:<br>
    \f$ dp = - \frac{  2 * c_{pl} * P_{0} * F_{p} * 
          ( dp[0] + dp[1] * t_{0})} { \rho_{60}^{2}} \f$
*  <br>Here we define the dp[0] and dp[1] factors.
*/
#define API_DP_STEP_5_FACTOR  {7.93920, 0.02326}

/** The number of iterations to perform for 11.1.6.2 calculations */
#define API_ITERATION_STEPS  15

/** The number of iterations steps to perform for 11.1.5.2 calculation */
#define API_THERMAL_REGRESSION_ITER  6


/**
*  The base temperature in F, (that is  60 F) on which all 
*  calculations are based. Conversion is always first to base
*  conditions defined by API_BASE_TEMP and  API_BASE_PRES and 
*  then to the alternate conditions.
*/
#define API_BASE_TEMP 60


/**
*  The base pressure in psi, (that is 0 psi) on which all  calculations are based. 
*  Conversion is always first to base 
*  conditions defined by API_BASE_PRES and API_BASE_TEMP 
*  and then to the alternate conditions.
*/
#define API_BASE_PRES 0

 
/** 
* The Density Limits on Crude Oil. This is defined in 
* Paragraph 11.1.6.1, page 32 and in Paragraph 11.1.6.2, page 54.
* These limits are used to determine the K-values and D_alpha
* values to use. The values are given in kg/m^3.
*/
#define API_CRUDE_OIL_DENSITY_LIMITS  {610.6,1163.5}

/** 
* The Density Limits on  Fuel Oil. This is defined in 
* Paragraph 11.1.6.1, p 32 and in Paragraph 11.1.6.2, page 54.
* These limits are used to determine the K-values and D_alpha
* values to use. The values are given in kg/m^3.
*/
#define API_FUEL_OIL_DENSITY_LIMITS {838.3127,1163.5}

/** 
* The Density Limits on  Jet Fuel. This is defined in 
* Paragraph 11.1.6.1, p 32 and in Paragraph 11.1.6.2, page 54.
* These limits are used to determine the K-values and D_alpha
* values to use.  The values are given in kg/m^3.
*/
#define API_JET_FUEL_DENSITY_LIMITS {787.5195, 838.3127}

/** 
* The Density Limits on  Transition Zone commodities. This is defined in 
* Paragraph 11.1.6.1, p 32 and in Paragraph 11.1.6.2, page 54.
* These limits are used to determine the K-values and D_alpha
* values to use. The values are given in kg/m^3.
*/
#define API_TRANSITION_ZONE_DENSITY_LIMITS {770.3520, 787.5195}

/** 
* The Density Limits on  Gasoline. This is defined in 
* Paragraph 11.1.6.1, p 32 and in Paragraph 11.1.6.2, page 54.
* These limits are used to determine the K-values and D_alpha
* values to use. The values are given in kg/m^3.
*/
#define API_GASOLINE_DENSITY_LIMITS {610.6, 770.3520}

/** 
* The Density Limits on  Lubrication Oil. This is defined in 
* Paragraph 11.1.6.1, p 32 and in Paragraph 11.1.6.2, page 54.
* These limits are used to determine the K-values and D_alpha
* values to use. The values are given in kg/m^3.
*/
#define API_LUBRICATION_OIL_DENSITY_LIMITS {800.9,1163.5}

/** 
* The Density Limits on commodities in the Refined Product
* range. This is defined in 
* Paragraph 11.1.6.1, p 32 and in Paragraph 11.1.6.2, page 54.
* These limits are used to determine the K-values and D_alpha
* values to use. The values are given in kg/m^3.
*/
#define API_REFINED_PRODUCTS_DENSITY_LIMITS {610.6, 1163.5}

/**
* The density limits on Crude oil as defined in 
* Paragraph 11.1.6.2, page 54. This is the rho range 
* commodities have to fall in  before the iteration procedure
* in 11.1.6.2 type calculations are entered into.
*  The values are given in kg/m^3.
*/
#define API_CRUDE_OIL_RHO_LIMITS {470.5,1201.8}

/**
* The density limits on Refined Products as defined in 
* Paragraph 11.1.6.2, page 54. This a the rho range 
* commodities have to fall in  before the iteration procedure
* in 11.1.6.1 type calculations are entered into.
*  The values are given in kg/m^3.
*/
#define API_REFINED_PRODUCTS_RHO_LIMITS { 470.4, 1209.5}

/**
* The density limits on Lubrication Oil as defined in 
* Paragraph 11.1.6.2, page 54. This is the rho range 
* commodities have to fall in  before the iteration procedure
* in 11.1.6.2 type calculations are entered into.
* The values are given in kg/m^3.
*/
#define API_LUBRICATION_OIL_RHO_LIMITS  {714.3, 1208.3}


/** 
* The temperature range for which the calculation can be
* performed. If an observed or alternate temperature is outside
* of this range, the calculation procedure will be terminated.
* The values are given in F. 
*/
#define API_ALLOWED_TEMPERATURE_RANGE {-58.0,302.0}

/** 
* The pressure range for which the calculation can be
* performed. If an observed or alternate pressure is outside
* of this range, the calculation procedure will be terminated.
* The values are given in PSI
*/
#define API_ALLOWED_PRESSURE_RANGE  {0, 1500}

/** 
* The  range allowed for the  alpha_60 values. If the
* alpha_60 value is supplied instead of the commodity and 
* if it is not in the range specified here, the calculation
* will be terminated. The values are given in 1/F
* 
*/
#define API_ALLOWED_THERMAL_EXPANSION_RANGE {230.0e-6, 930.0e-6}


/**
* The comment to place into the calculation request if we encounter
* the situtation described in the 11.1.6.2 calculation, note to step 4, i.e.
* convergence is not reached, but the value is considered to be acceptable
*/
#define API_CONVERGENCE_NOTE_11162 "Convergence failed. Last iteration value is accepted as final value." 

/**
* 
*/
#define API_CONVERSION_NOTE_11161_CUTOFF 1
   
/*@}*/
/* close protective C++ bracket **/
#ifdef __cplusplus
}
#endif

#endif /* API_CONSTANTS_H  */
