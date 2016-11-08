//Name: Juan David bravo
//Class: EE 445L T-TH 2:00-3:30
//

#include "Fixed.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "../EE445L/Lab1/ST7735.h"
#include "../EE445L/inc/tm4c123gh6pm.h"
// error strings that are outputed when a value is out of boundary
static char error[] = " *.***";
static char error2[] = " ***.**";

// Global variables for graphing functions
int xStep = 0;
int yStep = 0;
int xNorm = 0;
int yNorm = 0;
int len_y = 120;
int len_x = 120;
/* sDecOut3 will input a signed 32 bit integer and output it as a fixed point decimal
 * with delta value of 0.001
 */
void ST7735_sDecOut3(int32_t n){
	if ( n > 9999 || n < -9999){ 			// checks if n is inside limits or else it outputs error
		ST7735_OutString(error);
	}
	else{
		if(n < 0){
			ST7735_OutChar('-');					// - sign output for negative values
			n *=-1;												// positive calculations 
		}
		else{
			ST7735_OutChar(' ');					// if positive leave a space
		}
		ST7735_OutChar(n/1000 + 0x30);	// outputs the one value
		ST7735_OutChar('.');						// decimal
		n%=1000;
		ST7735_OutChar(n/100 + 0x30);		// tenth
		n%=100;
		ST7735_OutChar(n/10 + 0x30);		//hundredth
		n%=10;
		ST7735_OutChar(n + 0x30);				//thousandth 
	}

}
	
/* sDecOut3 will input an  unsigned 32 bit integer and output it as a fixed point decimal
 * with delta value of 1/256
 */
void ST7735_uBinOut8(uint32_t n){
	if ( n >= 256000 ) {							// checks if n is inside limits or else it outputs error
		ST7735_OutString(error2);
	}
	else {
		ST7735_OutChar(' ');
		uint32_t x = ((n*100)/256 );
		uint32_t hundred, tenth;
		hundred = x/10000+0x30; 
		if(hundred != 0x30){
			ST7735_OutChar(hundred);
		}else{
			ST7735_OutChar(' ');
		} 
		tenth = ((x/1000)%10 + 0x30);
		if(hundred != 0x30 || tenth != 0x30){
			ST7735_OutChar(tenth);
		}else{
			ST7735_OutChar(' ');
		}
		x %=1000;
		ST7735_OutChar( x/100 + 0x30);
		ST7735_OutChar('.');	
		x %= 100;
		ST7735_OutChar(x/10 + 0x30);
		x %= 10;
		ST7735_OutChar(x + 0x30);
	}
}

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY, int32_t freq){
	//init screen
	ST7735_FillRect(0, 20, 132, 162, ST7735_WHITE);
	yNorm = 0 - minY;
	xNorm = 0 - minX;
	xStep = (maxX - minX)/len_x; //units per pixel on x axis
	yStep = (maxY - minY)/len_y; //units per pixel on y axis
	//int lineX = (0 - minX)/xStep;
	//int lineY = 147 - ((0 - minY)/yStep);
// draws a coordinate plane, where the zero point
// location is dependent on the min and max values
//	ST7735_PlotClear(20, 148);
// draw x line
	if (maxX >= 0 && minX <= 0) {
		//ST7735_DrawFastVLine(lineX, 20, len_y, ST7735_Color565(4, 4, 4));
//		if (lineX > 10) {
//			ST7735_SetCursor(lineX - 10, lineY - 10);
//		} else {
//			ST7735_SetCursor(lineX + 10, lineY - 10);
//		}
//		ST7735_OutChar('0');
	} else if (minX > 0){
		//lineX = 0;
		//ST7735_DrawFastVLine(lineX, 20, len_y, ST7735_Color565(4, 4, 4));
		
	} else {
		//lineX = 127;
		//ST7735_DrawFastVLine(lineX, 20, len_y, ST7735_Color565(4, 4, 4));
	}
	
	// draw y line
	if (maxY >= 0 && minY <= 0) {
		//ST7735_DrawFastHLine(0, lineY, len_x, ST7735_Color565(4, 4, 4));
//		if (lineX > 10) {
//		ST7735_SetCursor(lineX - 10, lineY - 10);
//		} else {
//			ST7735_SetCursor(lineX + 10, lineY - 10);
//		}
//		ST7735_OutChar('0');
	} else if (minY > 0) {
		//lineY = 147;
		//ST7735_DrawFastHLine(0, lineY, len_x, ST7735_Color565(4, 4, 4));
	} else {
		//lineX = 20;
		//ST7735_DrawFastHLine(0, lineY, len_x, ST7735_Color565(4, 4, 4));
	}
//title
  ST7735_SetCursor(0,0);
	ST7735_OutString(title);
	ST7735_OutString(" MinX:");
	ST7735_OutUDec(minX);
	ST7735_OutString(" MaxX:");
	ST7735_OutUDec(maxX);
	ST7735_OutChar('\n');
	ST7735_OutString("most repeated:");
	ST7735_OutUDec(freq);
} 

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]){
	for (int i = 0; i < num; ++i) {
		int xCoor = (xNorm + bufX[i])/xStep;   //units / number of units per pixel
		int yCoor = (yNorm + bufY[i])/yStep;
		// invert y
		yCoor = len_y - yCoor;
		ST7735_DrawPixel(xCoor, yCoor + 20, ST7735_BLUE); // draw blue pixel at location
	}
}

//************* ST7735_Line********************************************
//  Draws one line on the ST7735 color LCD
//  Inputs: (x1,y1) is the start point
//          (x2,y2) is the end point
// x1,x2 are horizontal positions, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
// y1,y2 are vertical positions, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ST7735_Color565() 
// Output: none
void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	int temp;
	if (x1 > 128) {x1 = 128;}
	if (y1 > 180) {y1 = 180;}
  if (x2 > 128) {x2 = 128;}
	if (y2 > 180) {y2 = 180;}
	if (x2 < x1) {temp = x1; x1 = x2; x2 = temp;}
	if (y2 < y1) {temp = x1; x1 = x2; x2 = temp;}
	uint16_t deltax = x2 - x1; 
	uint16_t deltay = y2 - y1;
	uint16_t y_temp = y1;
	uint16_t sigma = deltay - deltax; 
	if ( x1 < x2){
		for (int i = x1; i<x2; i++){
			ST7735_DrawPixel(i,y_temp,color);
			if (sigma >0){
				y_temp++;
				sigma -= deltax;
			}
			sigma +=deltay;
		}
	}
}
const int32_t CircleXbuf[180] = { 2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798, 1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176, 1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70, 0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939, -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576, -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941, -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970, -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658, -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060, -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140, -70, 0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999
};
const int32_t CircleYbuf[180] = {0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999, 2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798, 1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176, 1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70, 0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939, -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576, -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941, -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970, -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658, -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060, -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140, -70
};
void ST7735_ClockInit(uint32_t hours, uint32_t minutes) {
	ST7735_XYplotInit("Circle",-2500, 2500, -2500, 2500, 0);
  ST7735_XYplot(180,(int32_t *)CircleXbuf,(int32_t *)CircleYbuf);
	uint32_t xCenter = 64;
	uint32_t yCenter = 80;
	uint32_t xMinute = CircleXbuf[3*minutes];
	uint32_t yMinute = CircleYbuf[3*minutes];
	uint32_t hourOffset = (180/12*(hours%12)) + (minutes/60)*(180/12);
	uint32_t xHour = CircleXbuf[hourOffset];
	uint32_t yHour = CircleYbuf[hourOffset];
	ST7735_Line(xCenter, yCenter, xMinute, yMinute, ST7735_GREEN);
	ST7735_Line(xCenter, yCenter, (xHour*3/4), (yHour*3/4), ST7735_GREEN);
}
int gHeight;
int gWidth;
void ST7735_LineGraphInit(int height, int width) {
	ST7735_InitR(INITR_REDTAB);
	ST7735_FillScreen(0);
	ST7735_DrawFastVLine(12, 10, height, ST7735_GREEN);
	ST7735_DrawFastHLine(12, 10 + height, width, ST7735_GREEN);
	ST7735_DrawChar(0,5,'4',ST7735_WHITE,ST7735_BLACK,1);
		ST7735_DrawChar(5,5,'0',ST7735_WHITE,ST7735_BLACK,1);
		ST7735_DrawChar(0,(10+height/4),'3',ST7735_WHITE,ST7735_BLACK,1);
		ST7735_DrawChar(5,(10+height/4),'0',ST7735_WHITE,ST7735_BLACK,1);
		ST7735_DrawChar(0,(10+height/2),'2',ST7735_WHITE,ST7735_BLACK,1);
		ST7735_DrawChar(5,(10+height/2),'0',ST7735_WHITE,ST7735_BLACK,1);
		ST7735_DrawChar(0,(10+height*.75),'1',ST7735_WHITE,ST7735_BLACK,1);
		ST7735_DrawChar(5,(10+height*.75),'0',ST7735_WHITE,ST7735_BLACK,1);
		ST7735_DrawChar(5,(10+height),'0',ST7735_WHITE,ST7735_BLACK,1);
	for (int i = 0; i < width; i += 4) {
		int x = 12 + i;
		ST7735_DrawFastVLine(x, 5 + height, 10, ST7735_GREEN);
	}
	gHeight = height;
	gWidth = width;
}
int x = 13;
void ST7735_PlotNewPoint(int val) {
	ST7735_DrawFastVLine(x, 10, gHeight-5, ST7735_BLACK);
	ST7735_DrawFastVLine(x + 1, 10, gHeight-5, ST7735_BLACK);
	val = (val*(gHeight/40)) + 10;
	ST7735_DrawPixel(x, val, ST7735_BLUE);
	ST7735_DrawPixel(x + 1, val, ST7735_BLUE);
	ST7735_DrawPixel(x, val + 1, ST7735_BLUE);
	ST7735_DrawPixel(x + 1, val + 1, ST7735_BLUE);
	x = x + 4;
	if (x > gWidth + 12) {
		x = 13;
		ST7735_FillRect(13, 10, gWidth + 5, gHeight -5, ST7735_BLACK); 
	}

}
double vals[26];
int i = 0;
void addPoint(double val) {
	vals[i] = val;
	i += 1;
	i%=26;
}

void ST7735_PlotArray(void) {
	int j = i;
	while (x < gWidth + 13) {
		ST7735_DrawFastVLine(x, 10, gHeight-5, ST7735_BLACK);
		ST7735_DrawFastVLine(x + 1, 10, gHeight-5, ST7735_BLACK);
		int val = ((40 - vals[j])*((gHeight*.75)/30)) + 10;
		ST7735_DrawPixel(x, val, ST7735_BLUE);
		ST7735_DrawPixel(x + 1, val, ST7735_BLUE);
		ST7735_DrawPixel(x, val + 1, ST7735_BLUE);
		ST7735_DrawPixel(x + 1, val + 1, ST7735_BLUE);
		j += 1;
		j%=26;
		x += 4;
	}
	x = 13;
}

int adcVals[53] = {4096, 3207,
3112,
3019,
2927,
2837,
2748,
2661,
2576,
2491,
2409,
2328,
2248,
2169,
2093,
2017,
1943,
1871,
1799,
1730,
1661,
1594,
1528,
1464,
1401,
1339,
1278,
1219,
1160,
1104,
1048,
993,
940,
887,
836,
786,
737,
689,
642,
596,
551,
508,
465,
423,
381,
341,
302,
264,
226,
189,
153,
118,
0};

double temps[51] = {10,
10.6,
11.2,
11.8,
12.4,
13.0,
13.6,
14.2,
14.8,
15.4,
16.0,
16.6,
17.2,
17.8,
18.4,
19.0,
19.6,
20.2,
20.8,
21.4,
22.0,
22.6,
23.2,
23.8,
24.4,
25.0,
25.6,
26.2,
26.8,
27.4,
28.0,
28.6,
29.2,
29.8,
30.4,
31.0,
31.6,
32.2,
32.8,
33.4,
34.0,
34.6,
35.2,
35.8,
36.4,
37.0,
37.6,
38.2,
38.8,
39.4,
40};
double getTemp(int adcVal) {
	for (int i = 0; i < 52; i++) {
		if (adcVals[i] >= adcVal && adcVals[i+1] <= adcVal) {
			return temps[i];
		}
	}
	return 0;
}
