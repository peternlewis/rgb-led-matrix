//     RGB Matrix Project - RGBLEDMatrix
//     Copyright (C) 2017 Michael Kamprath
//
//     This file is part of RGB Matrix Project.
// 
//     RGB Matrix Project is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     RGB Matrix Project is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with RGB Matrix Project.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __RGBLEDMATRIX_H__
#define __RGBLEDMATRIX_H__
#include "TimerAction.h"
#include "RGBImage.h"
#include "LEDMatrixBits.h"
#include "SPIConnection.h"

#if TWENTY_FOUR_BIT_COLOR
#define MAX_SCAN_PASS_COUNT 15
#else
#define MAX_SCAN_PASS_COUNT 3
#endif


class RGBLEDMatrix : public TimerAction {
public:
	typedef enum {
		// Each individual LED's RGB bits are consecutive in column order.
		// For a 4-column matrix, each row is ordered: RGB-RGB-RGB-RGB
		INDIVIDUAL_LEDS,	
		
		// All elements of the same color are consecutive in column order. The color
		// are order Red, Green Blue
		// For a 4-column matrix, each row is ordered: RRRR-GGGG-BBBB
		RGB_GROUPS
	} RGBLEDBitLayout;

private:
	const RGBLEDBitLayout _bitLayout;
	int _rows;
	int _columns;
	MutableRGBImage *_screen_data;
	
	LEDMatrixBits **_curScreenBitFrames;
	LEDMatrixBits *_screenBitFrames[2*MAX_SCAN_PASS_COUNT];
	bool _screenBitFrameToggle;
	
	int _scanPass;
	int _scanRow;
	int _isDrawingCount;

	SPIConnection	_spi;
	
	void copyScreenDataToBits(const RGBImageBase& image);	
	void shiftOutRow( int row, int scanPass );
	void setRowBitsForFrame(
			int row,
			size_t frame,
			LEDMatrixBits* framePtr,
			const RGBImageBase& image
		);
	size_t maxFrameCountForValue(ColorType value);
protected:
	virtual void action();
public:
  

	RGBLEDMatrix(
			int rows,
			int columns,
			int slavePin = 10,			
			RGBLEDBitLayout bitLayout = INDIVIDUAL_LEDS
		);
	virtual ~RGBLEDMatrix();
	
	void setup();
	
	MutableRGBImage& image(void)				{ return *_screen_data; }
	const MutableRGBImage& image(void) const	{ return *_screen_data; }
  

	void startDrawing(void)   			{ _isDrawingCount++; }
	void stopDrawing(void)    			{ _isDrawingCount--; if (_isDrawingCount < 0) { _isDrawingCount = 0; }}
	bool isDrawing(void) const			{ return (_isDrawingCount > 0); }
	
	int rows() const          			{ return _rows; }
	int columns() const       			{ return _columns; }

	void startScanning(void);
	void stopScanning(void);

	void shiftOutCurrentRow(void);
	unsigned int nextTimerInterval(void) const;
	void incrementScanRow( void );
};

#endif //__SCREEN_H__
