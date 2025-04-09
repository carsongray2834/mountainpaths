#include "Bridges.h"
#include "ColorGrid.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
//Added for Abs - FM Mon Apr 7 10:38 PM
#include <cmath>
#include <cstdlib>

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
	//Note FM Tue Apr 8 7:58 PM
	// I watch a couple of videos on Dijkstra's Algorithm
	// I noticed cg doesn't have a get x or y method but they do have a getWidth and getRow(), That could be something we need to use
	// End of Note
	// Write path to the colorgrid
	int x = 0;
	int y = startRow;
	//int top_right = 
	//cout << cg.getWidth() << endl;
	//Commented this out  didn't work int top_right = abs(cg.get(y+1,x+1) - cg.get(y,x));
	while (x < cg.getWidth()) {
		//cg.get(y,x);
         
	//int top_right = cg.get(((y+1)-y),((x+1)-x));
		cg.set(y, x, Color(255, 0, 0));
		x++;
//		y-=1;
	}
}

int main(int argc, char **argv) {

	// bridges object initialization


	// initialize Bridges
	Bridges bridges(123, "ID_USER", "API_KEY");

	// defaults for row number and data file
	int startRow = 50;

	// set title
	bridges.setTitle("Mountain Paths - Greedy Algorithms Example");
	bridges.setDescription("Your goal is to find a path that takes you through the points with the lowest elevation changes, in an effort to minimize the overall effort in walking through the path.");

	// get elevation data
    // Notes FM Mon Apr 7 11:42 PM 
	// ElevationData (int cols, int rows, int xll, int yll, float cellsize, int maxVal, int minVal)
	DataSource ds;
	ElevationData elev_data = ds.getElevationData(6.02, 44.10, 9.70, 47.77, 0.02);

	// color grid for visualization
	ColorGrid cg = getImage(elev_data);

	// find path by applying a greedy algorithm
	startRow = elev_data.getRows()/2;
	findPath (elev_data, startRow, cg);

	// visualize
	bridges.setDataStructure(cg);
	bridges.visualize();

	return 0;
}
