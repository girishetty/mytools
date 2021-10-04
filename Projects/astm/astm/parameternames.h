/* Copyright (c) 2003 American Petroleum Institute
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

#ifndef PARAMETER_NAMES_H
#define PARAMETER_NAMES_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/** @file parameternames.h
* Defines human readable values for input and output parameters and commodities.
*/ 

/*-----------------------------------------------------
*  Define the names for the Oil products
*-----------------------------------------------------*/

/** The number of commodties defined for the calculations */ 
#define API_OIL_PRODUCT_MAX_NUMBER 7

/** The symbolic number for the Crude Oil commodity type */ 
#define API_CRUDE_OIL_NAME         0

/** The symbolic number for the Lubrication Oil commodity type */ 
#define API_LUBRICATING_OIL_NAME   1

/** The symbolic number for the Refined Products commodity type */ 
#define API_REFINED_PRODUCTS_NAME  2

/** The symbolic number for the Fuel Oil commodity type */ 
#define API_FUEL_OIL_NAME          3

/** The symbolic number for the Jet Oil commodity type */ 
#define API_JET_FUEL_NAME          4

/** The symbolic number for the Transition Zone commodity type */ 
#define API_TRANSITION_ZONE_NAME   5

/** The symbolic number for the Gasoline  commodity type */ 
#define API_GASOLINE_NAME          6

/** The symbolic number for the commodity if alpha 60is supplied  */ 
#define API_COMMODITY_NOT_GIVEN   -1

/** 
* Prototype for a function that converts Oil product numbers into chars 
*
* @param name the number for the Oil product
* @param buffer the buffer to which to append the name
* @param length the length of the buffer to which to append the data
* @return 0 if the data where written successfully, false otherwise
*/
typedef int (*ApiParameterNames_getOilProductName)(int name,
						   char * buffer,
						   int length);


/*-----------------------------------------------------
*  Define the names for the Input and output parameters
*-----------------------------------------------------*/

/** The number of defined input and output parameters */
#define API_PARAMETERS_MAX_NUMBER 22

/** The symbolic number for the base temperature input parameter */ 
#define API_BASE_TEMP_NAME         0

/** The symbolic number for the base pressure input parameter */ 
#define API_BASE_PRES_NAME         1

/** The symbolic number for the commodity type input parameter */ 
#define API_COMMODITY_NAME         2

/** The symbolic number for the alpha 60  input parameter */ 
#define API_ALPHA_60_NAME          3

/** The symbolic number for the observed temperature input parameter */ 
#define API_OB_TEMP_NAME           4

/** The symbolic number for the observed  pressure input parameter */ 
#define API_OB_PRES_NAME           5

/** The symbolic number for the observed density input parameter */ 
#define API_OB_DENS_NAME           6

/** The symbolic number for the alternate temperature input parameter */ 
#define API_ALT_TEMP_NAME          7

/** The symbolic number for the alternate pressure input parameter */ 
#define API_ALT_PRES_NAME          8

/** The symbolic number for the observed volume  input parameter */ 
#define API_OB_VOL_NAME            9

/** The symbolic number for the alternate volume input parameter */ 
#define API_ALT_VOL_NAME          10

/** The symbolic number for the base volume input parameter */ 
#define API_BASE_VOL_NAME         11

/** The symbolic number for the alternate density output parameter */ 
#define API_ALT_DENS_NAME         12

/**
* The symbolic number for the scaled compensation factor output parameter.
* This is for transformation from base to alternate conditions.
*/ 
#define API_SCALED_COMP_ALT_NAME  13

/** The symbolic number for the base density output parameter */ 
#define API_BASE_DENS_NAME        14

/**
* The symbolic number for the ctl  output parameter.
* This is for transformation from observed to base conditions.
*/ 
#define API_CTL_OB_TO_BASE_NAME   15

/**
* The symbolic number for the cpl  output parameter.
* This is for transformation from observed to base conditions.
*/ 
#define API_CPL_OB_TO_BASE_NAME   16

/**
* The symbolic number for the ctpl  output parameter.
* This is for transformation from observed to base conditions.
*/ 
#define API_CTPL_OB_TO_BASE_NAME  17

/**
* The symbolic number for the scaled compensation factor output parameter.
* This is for transformation from observed to base  conditions.
*/ 
#define API_SCALED_COMP_OB_NAME   18

/**
* The symbolic number for the ctl output parameter.
* This is for transformation from base to alternate  conditions.
*/ 
#define API_CTL_BASE_TO_ALT_NAME  19

/**
* The symbolic number for the cpl output parameter.
* This is for transformation from  base to alternate conditions.
*/ 
#define API_CPL_BASE_TO_ALT_NAME  20

/**
* The symbolic number for the ctl output parameter.
* This is for transformation from  base to alternate conditions.
*/ 
#define API_CTPL_BASE_TO_ALT_NAME 21


/** 
* Prototype for a function that converts parameter numbers into chars 
*
* @param name the number for the parameter
* @param buffer the buffer to which to append the name
* @param length the length of the buffer to which to append the data
* @return 0 if the data where written successfully, false otherwise
*/
typedef int (*ApiParameterNames_getParameterName)(int name,
						  char * buffer,
						  int length);


/* close protective C++ bracket */
#ifdef __cplusplus
}
#endif

#endif /* PARAMETER_NAMES_H  */
