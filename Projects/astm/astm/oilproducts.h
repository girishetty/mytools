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

#ifndef OILPRODUCTS_H
#define OILPRODUCTS_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

#include "unit.h"
#include "error.h"
#include "constants.h"
#include "quantity.h"


/* protect code so it can be compiled with a C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/** @file oilproducts.h
* This file contains  definitions for the different commodity types.
*/ 



/*-----------------------------------------------------
* The type definitions for all the objects defined in 
* this include file
*-----------------------------------------------------*/

/** A structure to describe different OilProducts */
typedef struct api_oilproduct_ ApiOilProduct; 



/**
* The global initialization for  all static commodity objects.
* This needs to be called before any of the ApiOilProduct objects can be used
*
* @return 0 if initialization was successful,
*         INITIALIZE_FAILED if the initalization failed
*/
dll_export int ApiOilProduct_initalize();

/**
* The global destroy function for all resources connected to the 
* static commodity objects. This should be called upon program termination
*/
dll_export void ApiOilProduct_destroy();

/*----------------------------------------------------
* The public interface for the OilProducts
*-----------------------------------------------------*/

/**
* Return the  number of the oil product
*
* @param product the  OilProduct for  which to retrieve the name
* @param errorCode a variable to report errors
* @return the number of the indicated oil product
*/
dll_export int ApiOilProduct_productName(ApiOilProduct * product, int * errorCode);
 


/**
*  Return the K-values for this OilProduct.
*  This is a copy of the internal values, so that the 
*  internal values cannot inadvertently be changed.
*  Some products, namely Refined Products, do not have K-values, 
*  since they have sub-commidities that define the K-values to use.
*  If there are no K-Values, the errorCode is set and a NULL value is
*  returned.
*
*  @param product the  OilProduct for  which to retrieve the K-Values
*  @param values the array that will be filled with the k-values
* 
*  @return 0 if no error occured, an error code otherwise
*/
dll_export int  ApiOilProduct_getKValues(ApiOilProduct * product, double * values);


/**
* Return the D_alpha value for this Oil Product
*
*  @param product the  OilProduct for  which to retrieve the D_alpha value
*  @param errorCode the place holder to report errors
*
*   @return the D_alpha value for this Oil Product
*/
dll_export double ApiOilProduct_getDalpha(ApiOilProduct * product, int * errorCode);

/**
* Returns the density limits  for this product. 
*
* @param product the  OilProduct for  which to retrieve the density limits
* @param errorCode the place holder to report errors
*
* @return  the density limits for this product
*/
dll_export ApiDensity **  ApiOilProduct_getLimits(ApiOilProduct * product, int * errorCode);


/**
* Returns the rho density limits  for 11.1.6.2 calculations
* for this product. Some products, namely subcategories of Refined Products, 
* do not have a rho-range. In these cases an errorCode is set and
* a null pointer is returned
*
* @param product the  OilProduct for  which to retrieve the rho limits
* @param errorCode the place holder to report errors
*
* @return  the rho limits for 11.1.6.2 calculations for this product
*/
dll_export ApiDensity **  ApiOilProduct_getRhoRange(ApiOilProduct * product, int * errorCode);


/**
* Test whether  the indicated density is in the range of 
* Densities defined for this OilProduct object.
*
* @param product the  OilProduct for  which to do the check
* @param density the density to check
* @param errorCode the place holder to report errors
*
* @return TRUE if the density is within range, FALSE otherwise
*/
dll_export bool  ApiOilProduct_isInRange(ApiOilProduct * product,
                              ApiDensity * density,
                              int * errorCode);

/**
* Test whether  the indicated density is in the rho-range 
*  defined for this OilProduct object.
* Some products, namely subcategories of Refined Products, 
* do not have a rho-range. In these cases the errorCode is set and
* we return FALSE.
* 
* @param product the  OilProduct for  which to do the check
* @param density the density to check
* @param errorCode the place holder to report errors
*
* @return TRUE if the density is within range, FALSE otherwise
*/
dll_export bool ApiOilProduct_isInRhoRange(ApiOilProduct * product,
				ApiDensity * density,
				int *  errorCode);

/** 
* Return the correct OilProducts instance by name
*
* @param name the number of the desired oil product
* @param errorCode the place holder to report errors
*
* @return the correct ApiOilProduct or null if no product was found
*/
dll_export ApiOilProduct * ApiOilProduct_getProductByName(int name, int * errorCode);


/** 
* Return the correct OilProducts instance by name and
* density.  Except in the case of Refined Products, this 
* method return the same result as  ApiOilProduct_getProductByName. In the 
* case of Refined Products, we search through  the sub-categories
* and return the OilProduct for which the density is in  range.
*
* @param name the number of the oil product to find. If no product by
*             that name exist, we return null and set the errorCode
* @param density the observed density
* @param errorCode the place holder to report errors
* @return the correct ApiOilProduct or null if no product was found
*/
dll_export ApiOilProduct * ApiOilProduct_getProductByDensity(int name,
						  ApiDensity *density,
						  int * errorCode);

/* close protected C++ bracket **/
#ifdef __cplusplus
}
#endif

#endif /* OILPRODUCTS_H  */

