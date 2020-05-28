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
#ifndef API_UNIT_H
#define API_UNIT_H

#ifdef _WIN32
#define dll_export __declspec(dllexport)
#else
#define dll_export
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**  @ingroup quantGroup */
/*@{*/

/** @file unit.h
* Defines the units for all available quantities.
*
* The units are defined as global parameters and are accessible via 
* the respective functions defined in this file that allow to call 
* the different unit objects.
*/ 


/*-----------------------------------------------------
* The type definitions for all the objects defined in 
* this include file
*-----------------------------------------------------*/
 
/** Type definitition for ApiUnit from which all Unit will inherit */
typedef struct api_unit_ ApiUnit; 



/*-----------------------------------------------------
* 
* Public interfaces for the unit objects
*
*-----------------------------------------------------*/

/**
* The global initialization for  all static unit objects.
* This needs to be called before any of the Unit objects can be used
*
* @return 0 if initialization was successful,
*         INITIALIZE_FAILED if the initalization failed
*/
dll_export int ApiUnit_initalize();


/**
* Return the human readable name of the unit.
*
* @param unit the unit from which to print the unit
* @param buffer the string array  to which to add the unit text
* @param len the length of the array to which to add the unit
* 
* @return 0 if  the string was written okay and error code otherwise
*/
dll_export int ApiUnit_unitName(ApiUnit * unit, char * buffer, int len);


/**
* The global destroy function for all resources connected to the 
* static unit objects. This should be called upon program termination
*/
dll_export void ApiUnit_destroy();

/** 
* The  possible values for ApiUnit if dealing with Density.
* This returns a fixed object and can thus be compared by
* directly comparing the pointers.
*/
dll_export ApiUnit * ApiUnit_Density_API();
dll_export ApiUnit * ApiUnit_Density_RELATIVE();
dll_export ApiUnit * ApiUnit_Density_KGM3();

/** 
* The  possible values for ApiUnit if dealing with Pressure
* This returns a fixed object and can thus be compared by
* directly comparing the pointers.
*/
dll_export ApiUnit * ApiUnit_Pressure_PSI();
dll_export ApiUnit * ApiUnit_Pressure_KPA();
dll_export ApiUnit * ApiUnit_Pressure_BAR();

/** 
* The  possible values for ApiUnit if dealing with Temperature
* This returns a fixed object and can thus be compared by
* directly comparing the pointers.
*/
dll_export ApiUnit * ApiUnit_Temperature_F();
dll_export ApiUnit * ApiUnit_Temperature_C();


/** 
* The  possible values for ApiUnit if dealing with Volume
* This returns a fixed object and can thus be compared by
* directly comparing the pointers.
*/
dll_export ApiUnit * ApiUnit_Volume_BARREL();
dll_export ApiUnit * ApiUnit_Volume_LITER();
dll_export ApiUnit * ApiUnit_Volume_M3();


/** 
* The  possible values for ApiUnit if dealing with 
* dimensionless  quantities.
* This returns a fixed object and can thus be compared by
* directly comparing the pointers.
*/
dll_export ApiUnit * ApiUnit_Expansion_DIMLESS();


/** 
* The  possible values for ApiUnit if dealing with
* Scaled Comp Factors.
* This returns a fixed object and can thus be compared by
* directly comparing the pointers.
*/
dll_export ApiUnit * ApiUnit_ScaledComp_REV_PSI();
dll_export ApiUnit * ApiUnit_ScaledComp_REV_KPA();
dll_export ApiUnit * ApiUnit_ScaledComp_REV_BAR();

/** 
* The  possible values for ApiUnit if dealing with
* Thermal Expansion Factor.
* This returns a fixed object and can thus be compared by
* directly comparing the pointers.
*/
dll_export ApiUnit * ApiUnit_ThermalExp_REV_C();
dll_export ApiUnit * ApiUnit_ThermalExp_REV_F();


/*@}*/
#ifdef __cplusplus
}
#endif

#endif /* API_UNIT_H  */
