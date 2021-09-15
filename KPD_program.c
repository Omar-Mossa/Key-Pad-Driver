/***************************************************************************************/                                                            
/*********	Name	:	Omar Mossa	****************************************************/                                                            
/*********  Date	:	5/9/2020	****************************************************/                                                            
/*********	SWC		:	KPD			****************************************************/                                                            
/*********	Version	:	V1.0		****************************************************/                                                            
/***************************************************************************************/                                                            
                                                                                                                                                     
#define F_CPU	8000000UL                                                                                                                            
                                                                                                                                                     
/**** LIB LARYER ****/                                                                                                                               
#include <util\delay.h>                                                                                                                              
#include "STD_TYPES.h"                                                                                                                               
#include "BIT_MATH.h"                                                                                                                                
                                                                                                                                                     
/**** MCAL ****/                                                                                                                                     
#include "DIO_interface.h"                                                                                                                           
                                                                                                                                                     
/**** HAL ****/                                                                                                                                      
#include "KPD_interface.h"                                                                                                                           
#include "KPD_private.h"                                                                                                                             
#include "KPD_config.h"                                                                                                                              
                                                                                                                                                     
                                                                                                                                                     
                                                                                                                                                     
static u8 KPD_Au8KEYPADMAPPING[4][4]= KPD_ARRAY_MAPPING ; // The Key Pad Buttons                                                                     
                                                                                                                                                     
static u8 KPD_Au8RowsPins[4] = { KPD_ROW1_PIN , KPD_ROW2_PIN , KPD_ROW3_PIN , KPD_ROW4_PIN } ; // Related to DIO                                     
static u8 KPD_Au8ColsPins[4] = { KPD_COL1_PIN , KPD_COL2_PIN , KPD_COL3_PIN , KPD_COL4_PIN } ; // Related To DIO                                     
                                                                                                                                                     
                                                                                                                                                     
u8 KPD_u8GetKeyValue( u8 *Copy_Pu8PressedKeySymbol )                                                                                                 
{                                                                                                                                                    
	u8 Local_u8StateError = OK  ;                                                                                                                    
	u8 Local_u8ReturnedPinValue = DIO_LOW_INPUT ;                                                                                                    
                                                                                                                                                     
	if ( Copy_Pu8PressedKeySymbol != NULL )                                                                                                          
	{                                                                                                                                                
		/*	 Valid Address	*/                                                                                                                       
		*Copy_Pu8PressedKeySymbol = KPD_KEY_NOT_PRESSED ; // Initialize Pressing Status                                                              
                                                                                                                                                     
		/* Step 1 --> Activate Rows one by one  */                                                                                                   
		for ( u8 Local_u8RowCounter = 0 ; Local_u8RowCounter < 4 ; Local_u8RowCounter++ )                                                            
		{                                                                                                                                            
			DIO_u8SetPinValue( KPD_PORT_ID , KPD_Au8RowsPins[ Local_u8RowCounter ] , OUTPUT_LOW );                                                   
                                                                                                                                                     
			/* Step 2 --> Get The Reading Of Each Coloumn */                                                                                         
			for (u8 Local_u8ColCounter = 0; Local_u8ColCounter < 4 ; Local_u8ColCounter++ )                                                          
			{                                                                                                                                        
			 	/**** Debouncing Algorithm ****/                                                                                                     
                                                                                                                                                     
			 	DIO_u8GetPinValue( KPD_PORT_ID , KPD_Au8ColsPins[ Local_u8ColCounter ] , &Local_u8ReturnedPinValue ); // Get The Value For first time
                                                                                                                                                     
			 	if( DIO_LOW_INPUT == Local_u8ReturnedPinValue )                                                                                      
			 	{                                                                                                                                    
			 		/*** The Key Is Pressed ***/                                                                                                     
			 		/** Delay 20ms To Debounce **/                                                                                                   
			 		_delay_ms(20);                                                                                                                   
			 		while( Local_u8ReturnedPinValue == DIO_LOW_INPUT )                                                                               
			 		{                                                                                                                                
						/****   Don't Go Out Till The Pin Return HIGH Again	****/                                                                    
		 				DIO_u8GetPinValue( KPD_PORT_ID , KPD_Au8ColsPins[ Local_u8ColCounter ] , &Local_u8ReturnedPinValue );                        
		 			}                                                                                                                                
		 			*Copy_Pu8PressedKeySymbol = KPD_Au8KEYPADMAPPING[Local_u8RowCounter][Local_u8ColCounter] ; // Saving The pressed Button          
			 		break ; // Exit The Coloumn For Loop                                                                                             
			 	}                                                                                                                                    
			 }                                                                                                                                       
                                                                                                                                                     
                                                                                                                                                     
			/*** Step 3 ---> Deactivate The Current Row ***/                                                                                         
			DIO_u8SetPinValue( KPD_PORT_ID , KPD_Au8RowsPins[ Local_u8RowCounter ] , OUTPUT_HIGH );                                                  
                                                                                                                                                     
                                                                                                                                                     
			if ( *Copy_Pu8PressedKeySymbol != KPD_KEY_NOT_PRESSED )                                                                                  
		 	{                                                                                                                                        
		 		/*** The Pressed Key Has Been Found ***/                                                                                             
		 		break ; // Exit The Rows For Loop                                                                                                    
		 	}                                                                                                                                        
		}                                                                                                                                            
	}                                                                                                                                                
	else                                                                                                                                             
	{                                                                                                                                                
		Local_u8StateError = NOK ;                                                                                                                   
	}                                                                                                                                                
	return Local_u8StateError ;                                                                                                                      
}                                                                                                                                                    
                                                                                                                                                     