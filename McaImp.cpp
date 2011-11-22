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

//
// CADis : CA Discreto con bordes continuos
//

#include <ctype.h>
#include <iomanip.h>
#include <strstream.h>
#include "caimp.hpp"

int main(int argc, char * argv[])
	{
	int nEval=0,nRuns=0,argi=3;
	int inter=0,inicio=0;
	char gr,de,xy,sa,baseName[12];
	CAImp ca;

	if( argc > 1 )
		{
		ca.ReadParms( nEval,nRuns,argv[1] );
		if(argc >2 )
			{
			if( strstr(argv[2],"sed")!=NULL )
				ca.ReadSeed(argv[2]);	// Lee mapa ASCII de las especies y edades
			else if( strstr(argv[2],"set")!=NULL )
				ca.ReadSetSeed(argv[2]);
			}
		if( argc>3 )
			{
			if( strstr(argv[3],"set")!=NULL )
				{
				ca.ReadSetSeed(argv[3]);
				argi = 4;
				}
			}

		if( argc>=9 )
			{
			gr = toupper( *argv[argi] );
			inter = atoi(  argv[argi+1] );
			inicio = atoi(  argv[argi+2] );
			de = toupper( *argv[argi+3] );
			xy = toupper( *argv[argi+4] );
			sa = toupper( *argv[argi+5] );
			if( sa=='S' && argc>9 )
				strcpy(baseName, argv[argi+6]);
			else
				strcpy(baseName, "sed");

			goto NoPide;
			}
		}
	else
		cerr << "Falta archivo de parametros ???" << endl;

	cerr << "Graficos ? :";
    cin >> gr;
    gr = toupper(gr);

	cerr << "Cantidad de pasos para salida : ";
	cin >> inter;

	cerr << "Inicio de toma de datos : ";
	cin >> inicio;

    cerr << "Imprime densidades de las especies :";
    cin >> de;
    de = toupper( de );

	cerr << "Salva archivo xy al final ? :";
	cin >> xy;
	xy = toupper( xy );

	cerr << "Salvar estado al final :";
	cin >> sa;

	sa = toupper( sa );
	if( sa=='S' )
		{
		cerr << "Nombre de archivo base :";
		cin >> setw(6) >> baseName;
		}

	NoPide:

	int privez=1;
	double prom=0,dens=0;
	
	for(int r=0; r<nRuns; r++)
		{
		prom =0;
		if( privez )
			privez = 0;
		else
			{
			ca.Reset();
			if( argc>3 )
				{
				if( strstr(argv[2],"sed")!=NULL )
					ca.ReadSeed(argv[2]);	// Lee mapa ASCII de las especies y edades
				else if( strstr(argv[2],"set")!=NULL )
					ca.ReadSetSeed(argv[2]);
				if( argi == 4 )
					ca.ReadSetSeed(argv[3]);

				}
			}


		if( gr=='S' )
			ca.InitGraph();
		int i;
        for(i=0; i<nEval; i++)
            {
            if( i == 0 )
            	{
            	ca.SetN(0);
       	        if( gr=='S' )
                	ca.PrintGraph();
                else
   	                cerr << "Evaluaci¢n " << i << endl;
                if( de=='S' && (i+1)>=inicio)
                    prom += ca.PrintDensity();
                ca.SetN(1);
                }
			ca.Evaluate();
			if( ((i+1) % inter)==0 || i==0 )
                {
       	        if( gr=='S' )
                	ca.PrintGraph();
                else
   	                cerr << "Evaluaci¢n " << i+1 << endl;
				if( de=='S' && (i+1)>=inicio)
					{
					dens = ca.PrintDensity();
					prom += dens;
					if( dens == 0 )
                    	break;
                    }
                }
            }
        if( de=='S')
        	cout << "average: " << prom/(nEval-inicio) << endl;
        
        if( gr=='S' )
        	EGraph();
		if( xy=='S')
			ca.PrintMapXY( baseName );
		if( sa=='S')
			{
			ostrstream name;
			name << baseName << (i) << ".sed" << ends;
			ca.SaveSeed( name.str() );
			}
		}
	return 0;
	}
