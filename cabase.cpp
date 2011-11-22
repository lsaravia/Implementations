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

#include <math.h>
#include <time.h>

#include "cabase.hpp"
#include "bgi.hpp"
#include "smattpl.hpp"
#include "randlib.h"

#ifdef __GNUC__ 
#include "fortify.h"
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif


double CABase::Pi2 = M_PI * 2;


void CABase::Init( unsigned numSp, int dimX, int dimY, int rr )
	{
	A = 0;
	N = 1;
	T = 0;
	DimX = dimX;
	DimY = dimY;
	NumSpecies = numSp;

	if( rr == 0 )
		rndSeed = time(NULL);
	else
		rndSeed = rr;

//	r250_init(rndSeed);

   Initialized = 1;
    
	setall(rndSeed,rndSeed+1);

	//int s= time(NULL);
	//rmarin(s,s+1);
	}

CABase::CABase()
	{
	//Sp	= NULL;
	A = 0;
	N = 1;
	T = 0;
	DimX = 0;
	DimY = 0;
	NumSpecies = 0;
   Initialized = 0;
	}


CABase::~CABase()
	{
	//if( Sp!=NULL) delete Sp;
	//delete C;
	}

void CABase::Evaluate()
	{
	int x,y;
	A = T % 2;
	N = (T+1) % 2;
	switch( Rand(4) )
		{
		case 0:
			for(x=0; x<DimX; x++)
				for(y=0; y<DimY; y++)
					EvalCell(x,y);
			break;

		case 2:
			for(x=DimX-1; x>=0; x--)
				for(y=0; y<DimY; y++)
					EvalCell(x,y);
			break;

		case 3:
			for(x=0; x<DimX; x++)
				for(y=DimY-1; y>=0; y--)
					EvalCell(x,y);
			break;

		case 1:
			for(x=DimX-1; x>=0; x--)
				for(y=DimY-1; y>=0; y--)
					EvalCell(x,y);
		}
	T++;
	EmptyPrevious();
	}


#ifdef GRAPHICS
void CABase::EndGraph()
	{
	EGraph();
	}

void CABase::InitGraph()
	{
	IGraph(DimX,DimY);
/*	char buff[20];

	for( int i=1; i<=NumSpecies; i++)
		{
		itoa(i,buff,10);
		GLabel(buff,i);
		} */
	}

/*void CABase::PrintGraph()=0;
	{
//	for(int i=0; i<DimX; i++)
//		{
//		for(int j=0;j<DimY;j++)
//			PPix(i,j,C[N].elem(i,j));
//		}
	//getch();
	}
*/

#else

void CABase::InitGraph() {}
void CABase::EndGraph() {}

#endif
void CABase::PrintGraph() {}
