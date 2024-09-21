#include "bitmap.h"

// clang-format off
const uint8_t smallDigits_8ptBitmaps[BM_NUM_MAX][SMALLFONT_WIDTH] PROGMEM = {
	{0x3E, 0x51, 0x49, 0x45, 0x3E},  // 0x30 0
	{0x00, 0x42, 0x7F, 0x40, 0x00},  // 0x31 1
	{0x72, 0x49, 0x49, 0x49, 0x46},  // 0x32 2
	{0x21, 0x41, 0x49, 0x4D, 0x33},  // 0x33 3
	{0x18, 0x14, 0x12, 0x7F, 0x10},  // 0x34 4
	{0x27, 0x45, 0x45, 0x45, 0x39},  // 0x35 5
	{0x3C, 0x4A, 0x49, 0x49, 0x31},  // 0x36 6
	{0x41, 0x21, 0x11, 0x09, 0x07},  // 0x37 7
	{0x36, 0x49, 0x49, 0x49, 0x36},  // 0x38 8
	{0x46, 0x49, 0x49, 0x29, 0x1E},  // 0x39 9
	{0x00, 0x00, 0x00, 0x00, 0x00},  // EMPTY
};

const uint8_t smallChars_8ptBitmaps[BM_CHR_MAX][SMALLFONT_WIDTH] PROGMEM = {
	{0x00, 0x80, 0x70, 0x30, 0x00},  // 0x2C ,
	{0x70, 0x7F, 0x7F, 0x0F, 0x00},  // BIG COMA
	{0x1F, 0x20, 0x40, 0x20, 0x1F},  // 0x56 V
	{0x7C, 0x12, 0x11, 0x12, 0x7C},  // 0x41 A
	{0x3E, 0x41, 0x41, 0x41, 0x22},  // 0x43 C
	{0x00, 0x06, 0x09, 0x09, 0x06},	 // DEGREE
	{0x00, 0x00, 0x00, 0x00, 0x00},  // EMPTY
};

const uint8_t cascadiaCode_22ptBitmaps[BM_NUM_MAX][BIGFONT_WIDTH*3] PROGMEM = 
{
	/* @0 '0' (15 pixels wide) */
	//      #####     
	//    #########   
	//   ###########  
	//   ####   ####  
	//  ####     #### 
	//  ###       ### 
	//  ###       ### 
	//  ###       ### 
	//  ###  ###  ### 
	//  ### ##### ### 
	//  ### ##### ### 
	//  ###  ###  ### 
	//  ###       ### 
	//  ###       ### 
	//  ###       ### 
	//  ####     #### 
	//   ####   ####  
	//   ###########  
	//    #########   
	//      #####     
	{0x00, 0xF0, 0xFC, 0xFE, 0x1E, 0x0F, 0x07, 0x07, 0x07, 0x0F, 0x1E, 0xFE, 0xFC, 0xF0, 0x00, 
	0x00, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x00, 
	0x00, 0x00, 0x03, 0x07, 0x07, 0x0F, 0x0E, 0x0E, 0x0E, 0x0F, 0x07, 0x07, 0x03, 0x00, 0x00}, 

	/* @45 '1' (15 pixels wide) */
	//      ####      
	//   #######      
	//   #######      
	//   ### ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//       ###      
	//  ############# 
	//  ############# 
	//  ############# 
	{0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x07, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x00}, 

	/* @90 '2' (15 pixels wide) */
	//     ######     
	//   ##########   
	//  ############  
	//  ####    ####  
	//   #       ###  
	//           ###  
	//           ###  
	//           ###  
	//           ###  
	//           ###  
	//          ###   
	//         ####   
	//         ###    
	//       ####     
	//      ####      
	//     ####       
	//   #####        
	//  ############# 
	//  ############# 
	//  ############# 
	{0x00, 0x0C, 0x1E, 0x0E, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x0F, 0xFE, 0xFE, 0xFC, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0x78, 0x3C, 0x1F, 0x0F, 0x03, 0x00, 0x00, 
	0x00, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x00}, 

	/* @135 '3' (15 pixels wide) */
	//     #######    
	//   ##########   
	//  ############  
	//   ###    ####  
	//           ###  
	//           ###  
	//           ###  
	//          ###   
	//        ####    
	//     #####      
	//     ########   
	//          ####  
	//           ###  
	//            ### 
	//            ### 
	//           #### 
	//  #       ####  
	//  ############  
	//  ###########   
	//   ########     
	{0x00, 0x04, 0x0E, 0x0E, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x8F, 0xFF, 0xFE, 0x7C, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x07, 0x07, 0x0D, 0x9D, 0xFC, 0xF8, 0xE0, 0x00, 
	0x00, 0x07, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x07, 0x07, 0x03, 0x00, 0x00}, 

	/* @180 '4' (15 pixels wide) */
	//           ###  
	//    ###    ###  
	//    ###    ###  
	//   ###     ###  
	//   ###     ###  
	//   ###     ###  
	//   ###     ###  
	//   ###     ###  
	//  ####     ###  
	//  ###      ###  
	//  ###      ###  
	//  ###      ###  
	//  ###      ###  
	//  ##############
	// ###############
	// ###############
	//           ###  
	//           ###  
	//           ###  
	//           ###  
	{0x00, 0x00, 0xF8, 0xFE, 0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
	0xC0, 0xFF, 0xFF, 0xFF, 0xE1, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00}, 

	/* @225 '5' (15 pixels wide) */
	//   ###########  
	//   ###########  
	//   ###########  
	//   ###          
	//   ###          
	//   ###          
	//   ###          
	//   #########    
	//   ###########  
	//   ############ 
	//           #### 
	//            ### 
	//            ### 
	//             ## 
	//  ###       ### 
	//  ###       ### 
	//  ####     #### 
	//   ###########  
	//    #########   
	//     #######    
	{0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x07, 0x07, 0x00, 0x00, 
	0x00, 0xC0, 0xC3, 0xC3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0xDF, 0xFF, 0xFE, 0x00, 
	0x00, 0x01, 0x03, 0x07, 0x0F, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x07, 0x03, 0x01, 0x00}, 

	/* @270 '6' (15 pixels wide) */
	//          ####  
	//       #######  
	//      ########  
	//    ######      
	//    ####        
	//   ####         
	//   ###          
	//  ###   ####    
	//  ### ########  
	//  ### ######### 
	//  #####    #### 
	//  ####      ### 
	//  ###       ### 
	//  ###        ## 
	//  ###       ### 
	//  ###       ### 
	//  ####     #### 
	//   ###########  
	//    #########   
	//     #######    
	{0x00, 0x80, 0xE0, 0xF8, 0x78, 0x3C, 0x1E, 0x8E, 0x8E, 0x87, 0x87, 0x07, 0x07, 0x00, 0x00, 
	0x00, 0xFF, 0xFF, 0xFF, 0x0C, 0x07, 0x03, 0x03, 0x03, 0x03, 0x07, 0xDF, 0xFF, 0xFE, 0x00, 
	0x00, 0x01, 0x03, 0x07, 0x0F, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x07, 0x03, 0x01, 0x00}, 

	/* @315 '7' (15 pixels wide) */
	//  ##############
	//  ##############
	//  ############# 
	//  ###       ### 
	//  ###       ### 
	//  ###      ###  
	//  ###      ###  
	//           ###  
	//          ###   
	//          ###   
	//         ####   
	//         ###    
	//         ###    
	//        ####    
	//        ###     
	//        ###     
	//       ####     
	//       ###      
	//       ###      
	//      ####      
	{0x00, 0x7F, 0x7F, 0x7F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xE7, 0xFF, 0xFF, 0x1F, 0x03, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFC, 0xFF, 0x3F, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0F, 0x0F, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, 

	/* @360 '8' (15 pixels wide) */
	//     #######    
	//   ###########  
	//   ###########  
	//  ####     #### 
	//  ###       ### 
	//  ###       ### 
	//   ##       ##  
	//    ###   ###   
	//      #####     
	//     #######    
	//   ###########  
	//  ####     #### 
	//  ###       ### 
	//  ###       ### 
	//  ###       ### 
	//  ###       ### 
	//  ####     #### 
	//   ###########  
	//   ###########  
	//     #######    
	{0x00, 0x38, 0x7E, 0xFE, 0x8F, 0x87, 0x07, 0x07, 0x07, 0x87, 0x8F, 0xFE, 0x7E, 0x38, 0x00, 
	0x00, 0xF8, 0xFC, 0xFC, 0x0E, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0E, 0xFC, 0xFC, 0xF8, 0x00, 
	0x00, 0x01, 0x07, 0x07, 0x0F, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x07, 0x07, 0x01, 0x00}, 

	/* @405 '9' (15 pixels wide) */
	//     #######    
	//    #########   
	//   ###########  
	//  ####     #### 
	//  ###       ### 
	//  ###       ### 
	//  ###       ### 
	//  ###       ### 
	//  ###       ### 
	//  #####   ##### 
	//   ############ 
	//   ######## ### 
	//     ####   ### 
	//           ###  
	//          ####  
	//         ####   
	//       ######   
	//   #########    
	//   #######      
	//   ####         
	{0x00, 0xF8, 0xFC, 0xFE, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0F, 0xFE, 0xFC, 0xF8, 0x00, 
	0x00, 0x03, 0x0F, 0x0F, 0x1E, 0x1E, 0x1C, 0x1C, 0x8C, 0xCE, 0xE6, 0xFF, 0x7F, 0x1F, 0x00, 
	0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x0E, 0x07, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00}, 
};

const uint8_t verdana_12ptBitmaps[BM_NUM_MAX][BIGFONT_WIDTH*2] PROGMEM= 
{
	/* @0 '0' (9 pixels wide) */
	//   ####   
	//  #    #  
	// #      # 
	// #      # 
	// #      # 
	// #      # 
	// #      # 
	// #      # 
	// #      # 
	// #      # 
	//  #    #  
	//   ####   
	{0xFC, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0xFC, 0x00, 
	0x03, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x03, 0x00}, 

	/* @18 '1' (9 pixels wide) */
	//    #     
	//    #     
	//  ###     
	//    #     
	//    #     
	//    #     
	//    #     
	//    #     
	//    #     
	//    #     
	//    #     
	//  #####   
	{0x00, 0x04, 0x04, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x08, 0x08, 0x0F, 0x08, 0x08, 0x00, 0x00, 0x00}, 

	/* @36 '2' (9 pixels wide) */
	//  #####   
	// #     #  
	//        # 
	//        # 
	//        # 
	//       #  
	//      #   
	//     #    
	//    #     
	//   #      
	//  #       
	// ######## 
	{0x02, 0x01, 0x01, 0x01, 0x81, 0x41, 0x22, 0x1C, 0x00, 
	0x08, 0x0C, 0x0A, 0x09, 0x08, 0x08, 0x08, 0x08, 0x00}, 

	/* @54 '3' (9 pixels wide) */
	//  #####   
	// #     #  
	//        # 
	//        # 
	//       #  
	//    ###   
	//       #  
	//        # 
	//        # 
	//        # 
	// #     #  
	//  #####   
	{0x02, 0x01, 0x01, 0x21, 0x21, 0x21, 0x52, 0x8C, 0x00, 
	0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x03, 0x00}, 

	/* @72 '4' (9 pixels wide) */
	//       #  
	//      ##  
	//     # #  
	//    #  #  
	//   #   #  
	//  #    #  
	// #     #  
	// #     #  
	// #########
	//       #  
	//       #  
	//       #  
	{0xC0, 0x20, 0x10, 0x08, 0x04, 0x02, 0xFF, 0x00, 0x00, 
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0F, 0x01, 0x01}, 

	/* @90 '5' (9 pixels wide) */
	//  ####### 
	//  #       
	//  #       
	//  #       
	//  #####   
	//       #  
	//        # 
	//        # 
	//        # 
	//        # 
	// #     #  
	//  #####   
	{0x00, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x21, 0xC1, 0x00, 
	0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x03, 0x00}, 

	/* @108 '6' (9 pixels wide) */
	//    ####  
	//   #      
	//  #       
	// #        
	// # ####   
	// ##    #  
	// #      # 
	// #      # 
	// #      # 
	// #      # 
	//  #    #  
	//   ####   
	{0xF8, 0x24, 0x12, 0x11, 0x11, 0x11, 0x21, 0xC0, 0x00, 
	0x03, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x03, 0x00}, 

	/* @126 '7' (9 pixels wide) */
	// ######## 
	//        # 
	//       #  
	//       #  
	//      #   
	//      #   
	//     #    
	//     #    
	//    #     
	//    #     
	//   #      
	//   #      
	{0x01, 0x01, 0x01, 0x01, 0xC1, 0x31, 0x0D, 0x03, 0x00, 
	0x00, 0x00, 0x0C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}, 

	/* @144 '8' (9 pixels wide) */
	//   ####   
	//  #    #  
	// #      # 
	// #      # 
	//  #    #  
	//   ####   
	//  #    #  
	// #      # 
	// #      # 
	// #      # 
	//  #    #  
	//   ####   
	{0x8C, 0x52, 0x21, 0x21, 0x21, 0x21, 0x52, 0x8C, 0x00, 
	0x03, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x03, 0x00}, 

	/* @162 '9' (9 pixels wide) */
	//   ####   
	//  #    #  
	// #      # 
	// #      # 
	// #      # 
	// #      # 
	//  #    ## 
	//   #### # 
	//        # 
	//       #  
	//      #   
	//  ####    
	{0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0xFC, 0x00, 
	0x00, 0x08, 0x08, 0x08, 0x08, 0x04, 0x02, 0x01, 0x00}, 
};

// clang-format on