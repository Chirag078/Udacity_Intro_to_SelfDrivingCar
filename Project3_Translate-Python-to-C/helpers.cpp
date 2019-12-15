/**
	helpers.cpp

	Purpose: helper functions which are useful when
	implementing a 2-dimensional histogram filter.

	This file is incomplete! Your job is to make the
	normalize and blur functions work. Feel free to 
	look at helper.py for working implementations 
	which are written in python.
*/

#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
// #include "debugging_helpers.cpp"

using namespace std;

/**
	TODO - implement this function

    Normalizes a grid of numbers. 

    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.

    @return - a new normalized two dimensional grid where the sum of 
    	   all probabilities is equal to one.
*/
vector< vector<float> > normalize(vector< vector <float> > grid) {
	
	vector< vector<float> > newGrid(grid.size(),vector<float> (grid[0].size(),0));
	int r,c;
  	float total = 0.0;

	for(r=0;r<grid.size();++r)
      for(c=0;c<grid[0].size();++c)
    	total = total + grid[r][c];
        
 	for(r=0;r<grid.size();++r)
      for(c=0;c<grid[0].size();++c)
        newGrid[r][c] = grid[r][c]/total;

  return newGrid;
}

/**
	TODO - implement this function.

    Blurs (and normalizes) a grid of probabilities by spreading 
    probability from each cell over a 3x3 "window" of cells. This 
    function assumes a cyclic world where probability "spills 
    over" from the right edge to the left and bottom to top. 

    EXAMPLE - After blurring (with blurring=0.12) a localized 
    distribution like this:

    0.00  0.00  0.00 
    0.00  1.00  0.00
    0.00  0.00  0.00 

    would look like this:
	
	0.01  0.02	0.01
	0.02  0.88	0.02
	0.01  0.02  0.01

    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.

	@param blurring - a floating point number between 0.0 and 1.0 
		   which represents how much probability from one cell 
		   "spills over" to it's neighbors. If it's 0.0, then no
		   blurring occurs. 

    @return - a new normalized two dimensional grid where probability 
    	   has been blurred.
*/
vector < vector <float> > blur(vector < vector < float> > grid, float blurring) {
  // new Grid //
	vector< vector<float> > newGrid(grid.size(),vector<float> (grid[0].size(),0));
  	int height,width;
	height = grid.size();
    width  = grid[0].size();

  // Blurring Window  //
  
  	float center_prob,corner_prob,adjacent_prob;
  
    center_prob = 1.0-blurring;
    corner_prob = blurring / 12.0;
    adjacent_prob = blurring / 6.0;
	
  vector < vector < float> > window(height,vector <float> (width,0.0));
  
  window[0][0] = corner_prob;
  window[0][1] = adjacent_prob;
  window[0][2] = corner_prob;
  window[1][0] = adjacent_prob;
  window[1][1] = center_prob;
  window[1][2] = adjacent_prob;
  window[2][0] = corner_prob;
  window[2][1] = adjacent_prob;
  window[2][2] = corner_prob;
 
  
  vector < vector < float> > newone(height,vector<float> (width,0.0));
  for(int i=0;i<height;i++)
  	for(int j=0;j<width;j++)
      	newone[i][j] = 0.0;

  // Dx Dy impliemtation //
 vector <int> dx(3,0);
 vector <int> dy(3,0);
 dx[0] = -1;
 dx[1] = 0;
 dx[2] = 1;
 dy[0] = -1;
 dy[1] = 0;
 dy[2] = 1;
 
 int i,j,new_i,new_j,k,m;
 float grid_val,mult;
 for(i=0;i<height;++i)
 {
    for(j=0;j<width;++j)
    {
    	 grid_val = grid[i][j];
         {	
          	for(k=-1;k<dx.size()-1;++k)
            {
	        	for(m=-1;m<dy.size()-1;++m)
                {	
                	mult = window[dx[k]+1][dy[m]+1];
                    new_i = (i + dy[m]) % height;
                    new_j = (j + dx[k]) % width;
                    newone[new_i][new_j] += mult * grid_val;
                }
            }
        }
    }
 }
	return normalize(newGrid);

}

/** -----------------------------------------------
#
#
#	You do not need to modify any code below here.
#
#
# ------------------------------------------------- */


/**
    Determines when two grids of floating point numbers 
    are "close enough" that they should be considered 
    equal. Useful for battling "floating point errors".

    @param g1 - a grid of floats
    
    @param g2 - a grid of floats

    @return - A boolean (True or False) indicating whether
    these grids are (True) or are not (False) equal.
*/
bool close_enough(vector < vector <float> > g1, vector < vector <float> > g2) {
	int i, j;
	float v1, v2;
	if (g1.size() != g2.size()) {
		return false;
	}

	if (g1[0].size() != g2[0].size()) {
		return false;
	}
	for (i=0; i<g1.size(); i++) {
		for (j=0; j<g1[0].size(); j++) {
			v1 = g1[i][j];
			v2 = g2[i][j];
			if (abs(v2-v1) > 0.0001 ) {
				return false;
			}
		}
	}
	return true;
}

bool close_enough(float v1, float v2) { 
	if (abs(v2-v1) > 0.0001 ) {
		return false;
	} 
	return true;
}

/**
    Helper function for reading in map data

    @param s - a string representing one line of map data.

    @return - A row of chars, each of which represents the
    color of a cell in a grid world.
*/
vector <char> read_line(string s) {
	vector <char> row;

	size_t pos = 0;
	string token;
	string delimiter = " ";
	char cell;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		s.erase(0, pos + delimiter.length());

		cell = token.at(0);
		row.push_back(cell);
	}

	return row;
}

/**
    Helper function for reading in map data

    @param file_name - The filename where the map is stored.

    @return - A grid of chars representing a map.
*/
vector < vector <char> > read_map(string file_name) {
	ifstream infile(file_name.c_str());
	vector < vector <char> > map;
	if (infile.is_open()) {

		char color;
		vector <char> row;
		
		string line;

		while (std::getline(infile, line)) {
			row = read_line(line);
			map.push_back(row);
		}
	}
	return map;
}

/**
    Creates a grid of zeros

    For example:

    zeros(2, 3) would return

    0.0  0.0  0.0
    0.0  0.0  0.0

    @param height - the height of the desired grid

    @param width - the width of the desired grid.

    @return a grid of zeros (floats)
*/
vector < vector <float> > zeros(int height, int width) {
	int i, j;
	vector < vector <float> > newGrid;
	vector <float> newRow;

	for (i=0; i<height; i++) {
		newRow.clear();
		for (j=0; j<width; j++) {
			newRow.push_back(0.0);
		}
		newGrid.push_back(newRow);
	}
	return newGrid;
}
/*
 int main() {
 	vector < vector < char > > map = read_map("maps/new.txt");
    float bluring_float = 0.12  	//show_grid(map);
   vector < vector <char> > blur_data = blur(map,bluring_float);
   for(int i=0;i<blur_data.size();i++)
   {
     for(int j=0;j<blur_data[0].size();++j)
     {
       cout << blur_data[i][j] << " ";
     }
     cout << '\n';
   }
 	return 0;
 }
*/