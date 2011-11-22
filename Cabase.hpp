/*  Copyright 2009 Leonardo A. Saravia
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

#ifndef __CABASE_HPP
#define __CABASE_HPP

#include <math.h>

#ifdef R250
#include "r250.h"
#else
#include "randlib.h"
#endif

#ifdef __GNUC__
#include "fortify.h"
#endif

extern "C" {
//#include "randlib.h"
}

//void IGraph(int dimX,int dimY);
//void PPix(int x,int y, int color, int type=0);
//void EGraph();
//void GLabel(char * buff,int color);


class CABase
	{
	protected:
	unsigned A; 	// actual
	unsigned N; 	// proximo

	int DimX,DimY;
	unsigned NumSpecies;
    unsigned T;
	int rndSeed;
	static double Pi2;
    unsigned char Initialized;


	//simplmat <Cell> * C;

	double Rand() { return ranf(); };
	int Rand(int num) {
    	return ignuin(0,num);
        };

	public:

	CABase( unsigned nroSp, int dimX, int dimY, int rr=0) { Init(nroSp,dimX,dimY,rr); };
	CABase();
	void Init(unsigned nroSp, int dimX, int dimY, int rr=0 );
	virtual ~CABase();

	virtual void EvalCell(int x,int y)=0;

	virtual void Evaluate();
	virtual void EmptyPrevious(){};

	virtual void PrintGraph();
	virtual void InitGraph();
	virtual void EndGraph();

	int  ReadParms( int &nEval, int &nRuns, char * file ){return 1;};
	virtual int  ReadSeed( char * file ){return 1;};
	virtual int  SaveSeed( char * fname ){return 1;};

	};

#endif
