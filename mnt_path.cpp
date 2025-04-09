#include "Bridges.h"
#include "ColorGrid.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

#include "DataSource.h"
#include "data_src/ElevationData.h"
using namespace std;
using namespace bridges;

// This program implements the mountain paths assignment in C++. See the README.md for a
// detailed description


// takes in the processed elevation data and returns a color grid for
// visualization using BRIDGES
ColorGrid getImage(const ElevationData& elev_data) {
	//TODO
	//
	//return a Color Grid that represents the elevation as gray scale
	//
	// access elevation with elev_data.getVal(row, col)
	int min, max;
	min = elev_data.getMinVal();
	max = elev_data.getMaxVal();
	double scalar = 255.0 / (max - min);
	// compute the intensity by linear interpolation between elev_data.getMinVal(); and elev_data.getMaxVal();
	//
	// create ColorGrid with ColorGrid(nbrows, nbcols)
	// fill ColorGrid with .set(row, col, color) 
	ColorGrid cg (elev_data.getRows(),elev_data.getCols());
	for (int i = 0; i < elev_data.getRows(); i++) {
		for (int j = 0; j < elev_data.getCols(); j++) {
	  		int val = round(scalar * (elev_data.getVal(i, j) - min));
			cg.set(i, j, Color(val, val, val));
		}
	}

	return cg;
}

// determines the least effort path through the mountain starting a point on
// the left edge of the image
void findPath(const ElevationData&  elev_data, int startRow, ColorGrid& cg) {
	//        Run the greedy path from (0, startRow) to the right of the image
	//
	// always move right, but select the right cell, the top right cell, or bottom right cell
	// by minimizing the difference of elevation.
	//
	// Write path to the colorgrid
	int x = 0;
	int y = startRow;
	cg.set(y, x, Color(255, 0, 0));
	for (int i = 0; i < (cg.getWidth() - 1); i++) {
		int upright = 0, right = 0, downright =  0;
		int curr = cg.get(y, x).getRed();
		if (y < (cg.getHeight() - 1 )) upright = abs(cg.get(y+1,x+1).getRed() - curr);
		right = abs(cg.get(y,x+1).getRed() - curr);
		if (y > 0) downright = abs(cg.get(y-1,x+1).getRed() - curr);

		if ((y > 0) && (downright <= upright) && (downright <= right)) y--;
		if ((y < (cg.getHeight() - 1)) && (upright <= downright) && (right <= upright)) y++;

		x++;
		cg.set(y, x, Color(255, 0, 0));
	}
}

int main(int argc, char **argv) {

	// bridges object initialization


	// initialize Bridges
	Bridges bridges(123, "carsongray2834", "1195473891463");

	// defaults for row number and data file
	int startRow = 50;
	cin >> startRow;

	// set title
	bridges.setTitle("Greedy Algorithm Example");
	bridges.setDescription("Utilizes a Greedy Algorithm in an effort to find a path of minimal effort.");

	// get elevation data

	DataSource ds;
	ElevationData elev_data = ds.getElevationData(6.02, 44.10, 9.70, 47.77, 0.02);

	// color grid for visualization
	ColorGrid cg = getImage(elev_data);

	// find path by applying a greedy algorithm
	findPath (elev_data, startRow, cg);

	// visualize
	bridges.setDataStructure(cg);
	bridges.visualize();

	return 0;
}
