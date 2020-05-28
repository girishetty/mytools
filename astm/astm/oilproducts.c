/**
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


#include "oilproducts.h"
#include "parameternames.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>


/**
* The structure that defines ApiOilProducts
*/ 
struct api_oilproduct_{
  int  name;                 /** The name of the ApiOilProduct */
  ApiDensity *limits[2];     /** The density limits for this ApiOilProduct */
  ApiDensity *rhoLimits[2];  /** The density limits for 11.1.6.2 calculations */
  double KValues[3];         /** The K-Values, used in 11.1.6.1 calculations */
  double Dalpha;             /** The D_alpha value used in 11.1.6.2 calculations */
  bool hasRhoLimits;         /** Does this ApiOilProduct have density limits */
                             /** for 11.1.6.2 calculations */
  bool hasKValues;           /** Does this product have K-Values */
  bool isRefined;            /** Is this a refined product */
};


/** The available ApiOilProducts */
static ApiOilProduct * API_OIL_PRODUCT[API_OIL_PRODUCT_MAX_NUMBER];

void ApiOilProduct_cleanUp(ApiOilProduct * product){
  if( product != NULL){
    int i;

    for( i = 0; i < 2; i++){
      if( product->limits[i] != NULL) free( product->limits[i] );
      if( product->rhoLimits[i] != NULL) free( product->rhoLimits[i] );
      product->limits[i] = NULL;
      product->rhoLimits[i] = NULL;
    }

    free(product);
  }
};


ApiOilProduct * ApiOilProduct_init(int name,
				   double limits[2],
				   double rhoLimits[2],
				   double kvalues[3],
				   double dalpha,
				   bool hasKValues,
                                   bool hasRhoLimits,
				   bool isRefined){
  ApiOilProduct * product;
  int errorCode = 0;
  int i;


  /** Initialize the product */
  product = (ApiOilProduct*)malloc(sizeof(ApiOilProduct));
  if( product == NULL){
    return product;
  }
  memset( product, 0, sizeof(ApiOilProduct));

  /** Set the name for the product */
  product->name = name;

  /** Set the density limits */
  for( i = 0; i < 2; i++){
     product->limits[i] =  ApiDensity_init(limits[i],  
					   ApiUnit_Density_KGM3(),
					   FALSE,
					   &errorCode);
     if( errorCode != 0 || product->limits[i] == NULL){
        ApiOilProduct_cleanUp(product);
        product = NULL;
        return product; 
     } 
     product->rhoLimits[i] =  ApiDensity_init(rhoLimits[i],  
					      ApiUnit_Density_KGM3(),
					      FALSE,
					      &errorCode);
     if( errorCode != 0 || product->rhoLimits[i] == NULL){
        ApiOilProduct_cleanUp(product);
        product = NULL;
        return product; 
     } 
  }


  /** Set the K-Values */
  for( i = 0; i < 3; i++){
    product->KValues[i] = kvalues[i];
  }

  /** Set the D_alpha value */
  product->Dalpha = dalpha;
  

  /** Set whether we have rho limits  and k values*/
  product->hasRhoLimits = hasRhoLimits;
  product->hasKValues = hasKValues; 

  /** Set the isRefined value */
  product->isRefined = isRefined;

  return product;
};


/**
* The initialization routine
*/
int ApiOilProduct_initalize(){
  int errorCode = 0;

  /** Crude Oil */
  if( API_OIL_PRODUCT[API_CRUDE_OIL_NAME] == NULL){
     double limits[2] =  API_CRUDE_OIL_DENSITY_LIMITS;
     double rhoLimits[2] = API_CRUDE_OIL_RHO_LIMITS;
     double kvalue[3] = API_CRUDE_OIL_KVALUES;


     API_OIL_PRODUCT[API_CRUDE_OIL_NAME] = ApiOilProduct_init(API_CRUDE_OIL_NAME,
							      limits,
							      rhoLimits,
							      kvalue,
							      API_CRUDE_OIL_D_ALPHA,
							      TRUE,
							      TRUE,
							      FALSE);
     if( API_OIL_PRODUCT[API_CRUDE_OIL_NAME]  == NULL){
       errorCode  = INITIALIZE_FAILED;
       return errorCode;
     }
  }  


  /** Fuel Oil */
  if( API_OIL_PRODUCT[API_FUEL_OIL_NAME]  == NULL){
     double limits[2] =  API_FUEL_OIL_DENSITY_LIMITS;
     double rhoLimits[2] = {0,0};
     double kvalue[3] = API_FUEL_OIL_KVALUES;

     API_OIL_PRODUCT[API_FUEL_OIL_NAME] = ApiOilProduct_init(API_FUEL_OIL_NAME,
							     limits,
							     rhoLimits,
							     kvalue,
							     API_FUEL_OIL_D_ALPHA,
							     TRUE,
							     FALSE,
							     TRUE);

     if( API_OIL_PRODUCT[API_FUEL_OIL_NAME] == NULL){
       errorCode  = INITIALIZE_FAILED;
       return errorCode;
     }
  }  


  /** Jet Fuel */
  if( API_OIL_PRODUCT[API_JET_FUEL_NAME]  == NULL){
     double limits[2] =  API_JET_FUEL_DENSITY_LIMITS;
     double rhoLimits[2] = {0,0};
     double kvalue[3] = API_JET_FUEL_KVALUES;

     API_OIL_PRODUCT[API_JET_FUEL_NAME]  = ApiOilProduct_init(API_JET_FUEL_NAME,
							      limits,
							      rhoLimits,
							      kvalue,
							      API_JET_FUEL_D_ALPHA,
							      TRUE,
							      FALSE,
							      TRUE);

     if(  API_OIL_PRODUCT[API_JET_FUEL_NAME] == NULL){
       errorCode  = INITIALIZE_FAILED;
       return errorCode;
     }
  }  

  /** Transition Zone */
  if( API_OIL_PRODUCT[API_TRANSITION_ZONE_NAME]  == NULL){
     double limits[2] =  API_TRANSITION_ZONE_DENSITY_LIMITS;
     double rhoLimits[2] = {0,0};
     double kvalue[3] = API_TRANSITION_ZONE_KVALUES;

     API_OIL_PRODUCT[API_TRANSITION_ZONE_NAME] = 
                       ApiOilProduct_init(API_TRANSITION_ZONE_NAME,
					  limits,
					  rhoLimits,
					  kvalue,
					  API_TRANSITION_ZONE_D_ALPHA,
					  TRUE,
					  FALSE,
					  TRUE);
     if(  API_OIL_PRODUCT[API_TRANSITION_ZONE_NAME] == NULL){
       errorCode  = INITIALIZE_FAILED;
       return errorCode;
     }
  }  

  /* Gasoline  */
  if( API_OIL_PRODUCT[API_GASOLINE_NAME]  == NULL){
     double limits[2] =  API_GASOLINE_DENSITY_LIMITS;
     double rhoLimits[2] = {0,0};
     double kvalue[3] = API_GASOLINE_KVALUES;

     API_OIL_PRODUCT[API_GASOLINE_NAME] = ApiOilProduct_init(API_GASOLINE_NAME,
							     limits,
							     rhoLimits,
							     kvalue,
							     API_GASOLINE_D_ALPHA,
							     TRUE,
							     FALSE,
							     TRUE);
     if(  API_OIL_PRODUCT[API_GASOLINE_NAME] == NULL){
       errorCode  = INITIALIZE_FAILED;
       return errorCode;
     }
  }  


  /** Lubrication  Oil */
  if( API_OIL_PRODUCT[API_LUBRICATING_OIL_NAME]  == NULL){
     double limits[2] =  API_LUBRICATION_OIL_DENSITY_LIMITS;
     double rhoLimits[2] = API_LUBRICATION_OIL_RHO_LIMITS;
     double kvalue[3] = API_LUBRICATION_OIL_KVALUES;

     API_OIL_PRODUCT[API_LUBRICATING_OIL_NAME] = 
                                     ApiOilProduct_init(API_LUBRICATING_OIL_NAME,
							limits,
							rhoLimits,
							kvalue,
							API_LUBRICATION_OIL_D_ALPHA,
							TRUE,
							TRUE,
							FALSE);
     if(  API_OIL_PRODUCT[API_LUBRICATING_OIL_NAME] == NULL){
       errorCode  = INITIALIZE_FAILED;
       return errorCode;
     }
  }  


  /** Refined Products */
  if( API_OIL_PRODUCT[API_REFINED_PRODUCTS_NAME]  == NULL){
     double limits[2] =  API_REFINED_PRODUCTS_DENSITY_LIMITS;
     double rhoLimits[2] = API_REFINED_PRODUCTS_RHO_LIMITS;
     double kvalue[3] = {0,0,0};

     API_OIL_PRODUCT[API_REFINED_PRODUCTS_NAME] =
                                      ApiOilProduct_init(API_REFINED_PRODUCTS_NAME,
							 limits,
							 rhoLimits,
							 kvalue,
							 0,
							 FALSE,
							 TRUE,
							 FALSE);
     if(  API_OIL_PRODUCT[API_REFINED_PRODUCTS_NAME] == NULL){
       errorCode  = INITIALIZE_FAILED;
       return errorCode;
     }
  }  

  return errorCode;
}

/**
 * Delete all resources used
 */
void ApiOilProduct_destroy(){
   int i; 

   for( i  = 0; i < API_OIL_PRODUCT_MAX_NUMBER; i++){
      ApiOilProduct_cleanUp( API_OIL_PRODUCT[i] );
      API_OIL_PRODUCT[i] = NULL;
   }
}


/**
* Return the human readable name of the OilProduct
*
*/
int ApiOilProduct_productName(ApiOilProduct * product, int * errorCode){
  int result = API_COMMODITY_NOT_GIVEN;

  *errorCode = 0;
  if( product != 0){
    result = product->name;
  } 
  else *errorCode = NULL_POINTER_EXCEPTION;
  return result;
}



/**
*  Return the K-values for this OilProduct.
*/
int  ApiOilProduct_getKValues(ApiOilProduct * product, double * values){
  int i;
  int errorCode = 0;

  if( product != NULL ){
    if( product->hasKValues){ 
      for( i = 0; i < 3; i++) values[i] = product->KValues[i];
    }
    else{
      for( i = 0; i < 3; i++) values[i] = 0;
      errorCode = UNDEFINED_K_VALUES;
    }
  }
  else{
      for( i = 0; i < 3; i++) values[i] = 0;
      errorCode = NULL_POINTER_EXCEPTION;
  }

  return errorCode;
}

/**
* Return the D_alpha value for this Oil Product
*/
double ApiOilProduct_getDalpha(ApiOilProduct * product, int * errorCode){
  double result = 0;

  *errorCode  = 0;

  if( product != NULL) result = product->Dalpha;
  else                 *errorCode = NULL_POINTER_EXCEPTION;

  return result;
}

/**
* Returns the density limits  for this product. 
*
*/
ApiDensity **  ApiOilProduct_getLimits(ApiOilProduct * product, int * errorCode){
  *errorCode  = 0;

  if( product != NULL) return product->limits;
  else                 *errorCode = NULL_POINTER_EXCEPTION;

  return NULL;
}



/**
* Returns the rho density limits  for 11.1.6.2 calculations
* for this product.
*/
ApiDensity **  ApiOilProduct_getRhoRange(ApiOilProduct * product, int * errorCode){
  if( product != NULL ){
    if( product->hasRhoLimits){ 
      return product->rhoLimits;
    }
    else{
      *errorCode = UNDEFINED_RHO_LIMITS;
    }
  }
  else{
      *errorCode = NULL_POINTER_EXCEPTION;
  }

  return NULL;
}


/**
* Test whether  the indicated density is in the range of 
* Densities defined for this OilProduct object.
*/
bool  ApiOilProduct_isInRange(ApiOilProduct * product,
                              ApiDensity * density,
                              int *  errorCode){
    bool  result = TRUE;
    int comp;

    *errorCode = 0;

    if( product == NULL || density == NULL){
       result = FALSE;
       *errorCode = NULL_POINTER_EXCEPTION;
       return result;
    }  

    comp = ApiQuantity_compareTo((ApiQuantity*)product->limits[0], 
                                 (ApiQuantity*)density, 
				 errorCode);
    if( *errorCode != 0){
       result = FALSE;
       return result;
    }
    if( comp > 0 ){
      result  = FALSE;
    }

    comp = ApiQuantity_compareTo((ApiQuantity*)product->limits[1],
				 (ApiQuantity*)density,
				 errorCode);
    if( *errorCode != 0){
       result = FALSE;
       return result;
    }
    if( comp < 0 ){
      result  = FALSE;
    }

    return result;
}


/**
* Test whether  the indicated density is in the rho-range 
*  defined for this OilProduct object.
*/
bool ApiOilProduct_isInRhoRange(ApiOilProduct * product,
				ApiDensity * density,
				int * errorCode){
    bool  result = TRUE;
    int comp;

    *errorCode = 0;

    if( product == NULL || density == NULL){
       result = FALSE;
       *errorCode = NULL_POINTER_EXCEPTION;
       return result;
    }  
    
    if( !product->hasRhoLimits){
      *errorCode = UNDEFINED_RHO_LIMITS;
      result = FALSE;
      return result;
    }

    comp = ApiQuantity_compareTo((ApiQuantity*)product->rhoLimits[0], 
                                 (ApiQuantity*)density, 
				 errorCode);
    if( *errorCode != 0){
       result = FALSE;
       return result;
    }
    if( comp > 0 ){
      result  = FALSE;
    }

    comp = ApiQuantity_compareTo((ApiQuantity*)product->rhoLimits[1],
				 (ApiQuantity*)density,
				 errorCode);
    if( *errorCode != 0){
       result = FALSE;
       return result;
    }
    if( comp < 0 ){
      result  = FALSE;
    }

    return result;
}


/** 
* Return the correct OilProducts instance by name
*/
ApiOilProduct * ApiOilProduct_getProductByName(int name, int * errorCode){
    ApiOilProduct * product = NULL;
   
    *errorCode = 0;

    if( name == API_CRUDE_OIL_NAME ){
      product = API_OIL_PRODUCT[ API_CRUDE_OIL_NAME ];
    }
    else if( name == API_LUBRICATING_OIL_NAME){
      product = API_OIL_PRODUCT[ API_LUBRICATING_OIL_NAME ];
    }
    else if(  name == API_REFINED_PRODUCTS_NAME ){
      product = API_OIL_PRODUCT[ API_REFINED_PRODUCTS_NAME ];
    }

    if( product == NULL) *errorCode = UNDEFINED_OIL_PRODUCT;

    return product;
}

/** 
* Return the correct OilProducts instance by name and
* density.
*/
ApiOilProduct * ApiOilProduct_getProductByDensity(int  name,
						  ApiDensity *density,
						  int * errorCode){
  ApiOilProduct * product = ApiOilProduct_getProductByName(name, errorCode);
  int i;

  if( product == NULL || *errorCode != 0){
      *errorCode = UNDEFINED_OIL_PRODUCT;
      return product;
  }
  
  *errorCode = 0;

  if( product == API_OIL_PRODUCT[ API_REFINED_PRODUCTS_NAME ] ){
    bool result;

    for( i = 0; i < API_OIL_PRODUCT_MAX_NUMBER; i++){
      if( API_OIL_PRODUCT[i] == NULL){
	*errorCode = NULL_POINTER_EXCEPTION;
	return NULL;
      }
      if( API_OIL_PRODUCT[i]->isRefined == TRUE){
          result  = ApiOilProduct_isInRange(API_OIL_PRODUCT[i],
					    density,
					    errorCode);
	  if( *errorCode != 0){
	    product = NULL;
	    return product;
	  }

	  if( result) {
	    product = API_OIL_PRODUCT[i];
	    return product;
	  }
      }
    }
  }

  return product;
}

