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
#ifndef CALCULATION_H
#define CALCULATION_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

#include "unit.h"
#include "error.h"
#include "constants.h"
#include "quantity.h"
#include "oilproducts.h"
#include "parameternames.h"


/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/** 
 *@file calculation.h
* Contains the functions that perform the actual calculations.
*
* The calculations input and ouput paramters are set in 
* ::ApiCalculationRequest, which will contain the  resulting output
* parameters after a successful calculation. In addition there are 
* a few convienence function that allow to safely append text and numbers
* to a char buffer. 
*/ 

/**  @defgroup calcGroup VCF calculation  */
/*@{*/


/**
* Structure that contains  
* calculation input and output parameters
*/
typedef struct {
   /* Input parameters */
    ApiTemperature * baseTemp;   /**<  The base temperature to which this 
                                       calculation is performed. If a 
				       ApiCalculationRequest is created with
				       ApiCalculationRequest_initialize()  
                                       this is initialized to #API_BASE_TEMP */
    ApiPressure * basePres;      /**< The base pressure to which this 
                                       calculation is performed. If a 
				       ApiCalculationRequest is created with
				       ApiCalculationRequest_initialize()  
                                       this is initialized to #API_BASE_PRES */

    int commodityName;           /**< The commodity name. The available
                                      commodity names are 
                                      #API_CRUDE_OIL_NAME,
                                      #API_LUBRICATING_OIL_NAME, and 
                                      #API_REFINED_PRODUCTS_NAME  
                                      defined in parameternames.h. If set to -1
				      it is assumed that  
                                      ApiCalculationRequest::alpha60 is supplied. */
    ApiAlpha60 * alpha60;        /**< The thermal expansion factor to use.
                                      If it is #NULL  it is assumed that a 
                                      suitable value for
                                      ApiCalculationRequest::commodityName 
                                       is supplied */
    ApiTemperature * obTemp;     /**<  Temperature at  which input data were 
                                      observed  */
    ApiPressure * obPres;        /**<  Pressure at which input data were 
                                      observed  */
    ApiDensity * obDens;         /**< The observed density at
                                      ApiCalculationRequest::obTemp and
                                      ApiCalculationRequest::obPres  */

    ApiTemperature *altTemp;     /**< Desired alternate temperature.
                                      Volume correction factors will be 
                                      calculated to transform from <br>
                                      ApiCalculationRequest::obTemp and
                                      ApiCalculationRequest::obPres  to <br>
                                      ApiCalculationRequest::baseTemp and 
                                      ApiCalculationRequest::basePres  and from <br>
                                      ApiCalculationRequest::baseTemp and
                                      ApiCalculationRequest::basePres to <br>
                                      ApiCalculationRequest::altTemp and
                                      ApiCalculationRequest::altPres */
    ApiPressure *altPres;        /**< Desired alternate pressure.
                                      Volume correction factors will be 
                                      calculated to transform from <br>
                                      ApiCalculationRequest::obTemp and
                                      ApiCalculationRequest::obPres  to <br>
                                      ApiCalculationRequest::baseTemp and 
                                      ApiCalculationRequest::basePres  and from <br>
                                      ApiCalculationRequest::baseTemp and
                                      ApiCalculationRequest::basePres to <br>
                                      ApiCalculationRequest::altTemp and
                                      ApiCalculationRequest::altPres */
  
    ApiVolume *obVol;            /**< Volume at   ApiCalculationRequest::obTemp and
                                      ApiCalculationRequest::obPres.  
                                      Only one or none  of  
                                      ApiCalculationRequest::obVol,
                                      ApiCalculationRequest::baseVol and 
                                      ApiCalculationRequest::altVol  may be supplied.
                                      If at least one is supplied the other two
                                      will be calculated. */
    ApiVolume *altVol;           /**< Volume at   ApiCalculationRequest::altTemp and
                                      ApiCalculationRequest::altPres.  
                                      Only one or none  of  
                                      ApiCalculationRequest::obVol,
                                      ApiCalculationRequest::baseVol and 
                                      ApiCalculationRequest::altVol  may be supplied.
                                      If at least one is supplied the other two
                                      will be calculated. */
    ApiVolume *baseVol;          /**< Volume at   ApiCalculationRequest::baseTemp and
                                      ApiCalculationRequest::basePres.  
                                      Only one or none  of  
                                      ApiCalculationRequest::obVol,
                                      ApiCalculationRequest::baseVol and 
                                      ApiCalculationRequest::altVol  may be supplied.
                                      If at least one is supplied the other two
                                      will be calculated. */

    /*  Calculated values */
    ApiExpansionFactor * ctlAlt;   /**<  The ctl  factor to transform
                                         directly from  
                                         ApiCalculationRequest::obTemp to
                                         ApiCalculationRequest::altTemp.
                                         This is used internally to calculate
                                         the volume correction factors if 
                                         ApiCalculationRequest::baseTemp  and 
                                         ApiCalculationRequest::basePres are not
                                         60F and 0psi, respectively.  */
    ApiExpansionFactor * cplAlt;   /**<  The cpl  factor to transform
                                         directly from  
                                         ApiCalculationRequest::obPres to
                                         ApiCalculationRequest::altPres.
                                         This is used internally to calculate
                                         the volume correction factors if 
                                         ApiCalculationRequest::baseTemp  and 
                                         ApiCalculationRequest::basePres are not
                                         60F and 0psi, respectively.  */
    ApiExpansionFactor * ctplAlt;  /**<  The ctpl  factor to transform
                                         directly from  
                                         ApiCalculationRequest::obPres and
                                         ApiCalculationRequest::obTem to
                                         ApiCalculationRequest::altPres and
                                         ApiCalculationRequest::altTemp.
                                         This is used internally to calculate
                                         the volume correction factors if 
                                         ApiCalculationRequest::baseTemp  and 
                                         ApiCalculationRequest::basePres are not
                                         60F and 0psi, respectively.  */

    ApiDensity * altDens;           /**< Density at 
                                         ApiCalculationRequest::altTemp  and 
                                         ApiCalculationRequest::altPres */
    ApiScaledFactor *scaledCompAlt; /**< The Fp factor to transform from
                                         ApiCalculationRequest::baseTemp  and 
                                         ApiCalculationRequest::basePres  to 
                                         ApiCalculationRequest::altTemp  and 
                                         ApiCalculationRequest::altPres */
   
    ApiDensity *baseDens;              /**< Density at
                                         ApiCalculationRequest::baseTemp  and 
                                         ApiCalculationRequest::basePres */ 
    ApiExpansionFactor * ctlObToBase;  /**< The ctl  factor to transform from
                                         ApiCalculationRequest::obTemp to
                                         ApiCalculationRequest::baseTemp. */
    ApiExpansionFactor * cplObToBase;  /**< The cpl  factor to transform from
                                         ApiCalculationRequest::obPres to
                                         ApiCalculationRequest::basePres. */
    ApiExpansionFactor * ctplObToBase; /**< The ctpl  factor to transform from
                                         ApiCalculationRequest::obTemp and
                                         ApiCalculationRequest::obPres to
                                         ApiCalculationRequest::baseTemp and
                                         ApiCalculationRequest::basePres.*/

    ApiScaledFactor *scaledCompOb;      /**<  The Fp factor to transform from
                                         ApiCalculationRequest::obTemp  and 
                                         ApiCalculationRequest::obPres  to 
                                         ApiCalculationRequest::baseTemp  and 
                                         ApiCalculationRequest::basePres */  
    ApiExpansionFactor * ctlBaseToAlt;  /**< The ctl  factor to transform from
                                         ApiCalculationRequest::baseTemp to
                                         ApiCalculationRequest::altTemp. */
    ApiExpansionFactor * cplBaseToAlt;  /**< The cpl  factor to transform from
                                         ApiCalculationRequest::basePres to
                                         ApiCalculationRequest::altPres. */
    ApiExpansionFactor * ctplBaseToAlt; /**<  The ctpl  factor to transform from
                                         ApiCalculationRequest::baseTemp and
                                         ApiCalculationRequest::basePres to
                                         ApiCalculationRequest::altTemp and
                                         ApiCalculationRequest::altPres.*/
   char *comments;                      /**< The calculation might put comment,
                                              like those mentioned in the not to the 
                                              11.1.6.2 calculation in step 4.
                                              If there are no comments, this is
                                              either NULL or an empty string */ 
   int commentLength;                    /**< The allocated length of comments.
					    Used internally */ 
 

   char *intermediate;                 /**< A buffer to report 
                                            input and ouptut results.
                                            If it is #NULL no results are 
                                            reported. If  
                                            ApiCalculationRequest::reportIntermediate
                                            is #TRUE intermediate results are
                                            also reported */ 
   int intLength;                      /**< The length of
	 				    ApiCalculationRequest::intermediate */
   bool reportIntermediate;            /**< Should intermediate result be reported.
                                            If set to #TRUE intermediate results
                                            are written to 
                                            ApiCalculationRequest::intermediate, 
                                            otherwise only the input and output
                                            values are reported. If
                                            ApiCalculationRequest::intermediate is
                                            #NULL, the value for this variable
                                            is ignored. */
   
   ApiParameterNames_getOilProductName getOilProductName; /**< A function to
                         transform commodity numbers to human readable  form.
                         If #NULL only the numbers are reported.  A function
                         that can be used to transform commodity numbers to
                         human readable form is ApiCalculation_getProductName() */
   ApiParameterNames_getParameterName getParameterName;   /**<  A function to
                         transform input parameter numbers to human readable  form.
                         If #NULL only the numbers are reported.  A function
                         that can be used to transform input parameter numbers to
                         human readable form is ApiCalculation_getParameterName() */
} dll_export ApiCalculationRequest;

/**
* Do any necessary clean up. This is called by the DLL upon
* unloading. It can be called repeately without any danger
*/
dll_export void ApiCalculation_cleanVBAExport();

/**
* Initialize all resources needed to do a calculation.
* This  needs to be done before any calculation can be performed,
* but the call does not need to be repeated for each subsequent 
* calculation.
*
* @return 0 if all resources where successfully allocated.
*          Otherwise an error code describing the error will be returned.
*/
dll_export int ApiCalculationRequest_initialize();

/**
* Free all resources allocated by ApiCalculationRequest_initialize().
*/
dll_export void ApiCalculationRequest_destroy();


/**
* Create and initialize an  ApiCalculationRequest object.
* All structure variables are set to 0, except:
* <ul>  
*  <li> ApiCalculationRequest::baseTemp is set to #API_BASE_TEMP.
*  <li> ApiCalculationRequest::basePres is set to #API_BASE_PRES.
*  <li> ApiCalculationRequest::reportIntermediate is set to #FALSE.
* </ul>
*
* @param errorCode a variable to report errors. If the 
*        initialization was successful this will be set to 0. Otherwise
*        it will be set to an error code describing the error.
* @return a newly initialized ApiCalculationRequest object
*         or #NULL if an error occured
*/
dll_export ApiCalculationRequest * ApiCalculationRequest_init(int * errorCode);


/**
* Release all the resources associated with an ApiCalcualtionRequest.
* 
* @param request the ApiCalcualtionRequest to release
*/
dll_export void  ApiCalculationRequest_cleanUp(ApiCalculationRequest * request);

 
/**
* Sets all the output parameter to #NULL to indicate
* that a calculation error occured.
*
* @param request the request for which to set the error status.
*/
dll_export void ApiCalculationRequest_setErrorStatus(ApiCalculationRequest * request);


/**
* Append char data to the end of the indicated buffer. 
* This routine first tests whether the data can be safely appended.
* If there is not enough
* space  left the buffer, the data are not appended and an error code
* of #BUFFER_OVERFLOW is returned.
*
* @param buffer the buffer to which to append data
* @param len the length of the buffer
* @param result the data to append to the buffer
* 
* @return 0 if the data where successfully appended, 
*          otherwise #BUFFER_OVERFLOW 
*/
dll_export int ApiCalculationRequest_appendBuffer(char * buffer,
					int len,
					const char * result);


/**
* Append a double value to the end of the indicated buffer. 
* The double is always written as 25.15f.
* This routine first tests whether the data can be safely appended.
* If there is not enough
* space  left the buffer, the data are not appended and an error code
* of #BUFFER_OVERFLOW is returned.
*
* @param buffer the buffer to which to append data
* @param len the length of the buffer
* @param value the data to append to the buffer
* 
* @return 0 if the data where successfully appended, 
*          otherwise #BUFFER_OVERFLOW 
*/
dll_export int ApiCalculationRequest_appendDouble(char * buffer,
					int len,
					double value);


/** 
* Write the name and value of the quantity to the indicated buffer.
* This routine first tests whether the data can be safely appended.
* If there is not enough
* space  left the buffer, the data are not appended and an error code
* of #BUFFER_OVERFLOW is returned.
* 
* @param calcReq the ApiCalculationRequeststructure that contains the buffer 
*                to  which to write the result
* @param isIntermediate is this an intermediate result
* @param name the name of the parameter as defined in parameternames.h
* @param value the quantity to print
* @param rounded should the value of the quantity be rounded  before output
* @param unit the unit we want to print it in.
*              If #NULL, the given unit is used
*/
dll_export void ApiCalculationRequest_reportQuantity(ApiCalculationRequest *calcReq,
					  bool isIntermediate,
					  int name,
					  ApiQuantity * value,
					  bool rounded,
					  ApiUnit * unit);

/**
*   Perform a volume correction calculation.
*  The input parameters are
*  all defined in  calcReq. Upon returning from this
*  method calcReq will have the output parameters filled
*  with the  results. If an error occured the output values will
*  all be set to #NULL and an error code describing the error will
*  be returned.
*
* @param request the ApiCalculationRequest object
*                describing the calculation to be done.
* @return 0 if all is well, an error code otherwise
*/
dll_export int ApiCalculationRequest_performCalculation(ApiCalculationRequest * request);

/*@}*/
/* close protected C++ bracket **/
#ifdef __cplusplus
}
#endif

#endif /* CALCULATION_H  */
