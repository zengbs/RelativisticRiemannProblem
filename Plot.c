#include <stdio.h>
#include "Struct.h"
#include "Prototypes.h"




//    1    2           3            4   5     6
// Left  Fan ContactLeft ContactRight Fan Right



void Plot( int Pattern, struct RiemannProblem *RP, struct PlotParams plot )
{

  FILE *fptr[999999];

  double DT      = plot.DT     ;
  double End_T   = plot.End_T  ;
  double X_Left  = plot.X_Left ;
  double X_Right = plot.X_Right;
  int    NCell   = plot.NCell  ;

  double dX = (X_Right-X_Left)/(double)NCell;
  double X0 = 0.5*( X_Left + X_Right );

  char fileName[100];
  sprintf (fileName, "./000000.dat");
  fptr[0] = fopen (fileName, "w");

  double time = 0.0;
  fprintf (fptr[0], "# time = %e\n", time);
  fprintf (fptr[0], "# x[1]       d1[2]       v1[3]       p1[4]\n");

  if ( Pattern == 1 )
  {
    fprintf (fptr[0], "%e %e %e %e\n", X_Left,  RP->SS.Leftt.DensUpStream, RP->SS.Leftt.VelyUpStream, RP->SS.Leftt.PresUpStream);
    fprintf (fptr[0], "%e %e %e %e\n", X0-dX,   RP->SS.Leftt.DensUpStream, RP->SS.Leftt.VelyUpStream, RP->SS.Leftt.PresUpStream);

    fprintf (fptr[0], "%e %e %e %e\n", X0,      RP->SS.Right.DensUpStream, RP->SS.Right.VelyUpStream, RP->SS.Right.PresUpStream);
    fprintf (fptr[0], "%e %e %e %e\n", X_Right, RP->SS.Right.DensUpStream, RP->SS.Right.VelyUpStream, RP->SS.Right.PresUpStream);
  }
  else if ( Pattern == 2 )
  {
    fprintf (fptr[0], "%e %e %e %e\n", X_Left,  RP->RS.Leftt.DensUpStream, RP->RS.Leftt.VelyUpStream, RP->RS.Leftt.PresUpStream);
    fprintf (fptr[0], "%e %e %e %e\n", X0-dX,   RP->RS.Leftt.DensUpStream, RP->RS.Leftt.VelyUpStream, RP->RS.Leftt.PresUpStream);

    fprintf (fptr[0], "%e %e %e %e\n", X0,      RP->RS.Right.DensUpStream, RP->RS.Right.VelyUpStream, RP->RS.Right.PresUpStream);
    fprintf (fptr[0], "%e %e %e %e\n", X_Right, RP->RS.Right.DensUpStream, RP->RS.Right.VelyUpStream, RP->RS.Right.PresUpStream);
  }
  else if ( Pattern == 3 )
  {
    fprintf (fptr[0], "%e %e %e %e\n", X_Left,  RP->SR.Leftt.DensUpStream, RP->SR.Leftt.VelyUpStream, RP->SR.Leftt.PresUpStream);
    fprintf (fptr[0], "%e %e %e %e\n", X0-dX,   RP->SR.Leftt.DensUpStream, RP->SR.Leftt.VelyUpStream, RP->SR.Leftt.PresUpStream);

    fprintf (fptr[0], "%e %e %e %e\n", X0,      RP->SR.Right.DensUpStream, RP->SR.Right.VelyUpStream, RP->SR.Right.PresUpStream);
    fprintf (fptr[0], "%e %e %e %e\n", X_Right, RP->SR.Right.DensUpStream, RP->SR.Right.VelyUpStream, RP->SR.Right.PresUpStream);
  }
  else if ( Pattern == 4 )
  {
    fprintf (fptr[0], "%e %e %e %e\n", X_Left,  RP->RR.Leftt.DensUpStream, RP->RR.Leftt.VelyUpStream, RP->RR.Leftt.PresUpStream);
    fprintf (fptr[0], "%e %e %e %e\n", X0-dX,   RP->RR.Leftt.DensUpStream, RP->RR.Leftt.VelyUpStream, RP->RR.Leftt.PresUpStream);

    fprintf (fptr[0], "%e %e %e %e\n", X0,      RP->RR.Right.DensUpStream, RP->RR.Right.VelyUpStream, RP->RR.Right.PresUpStream);
    fprintf (fptr[0], "%e %e %e %e\n", X_Right, RP->RR.Right.DensUpStream, RP->RR.Right.VelyUpStream, RP->RR.Right.PresUpStream);
  }



  int j = 1;

  double X_Max, X_Min;
  int Ng_1, Ng_2, Ng_3, Ng_4;
  double Cs, DensFan, PresFan, VelocityFan;

  time += DT;

  while (time <= End_T)
  {
    sprintf (fileName, "./%06d.dat", j);
    fptr[j] = fopen (fileName, "w");
    fprintf (fptr[j], "# time = %e\n", time);
    fprintf (fptr[j], "# x=[1], d1=[2], v1=[3], p1=[4], D1=[5], M1=[6], E1=[7], Lorentz fac.=[8]\n");

    int region;

    for ( region = 1; region <= 6; region++)
    {
        switch (region)
        {
          case 1:
		    if ( Pattern == 1 )
		    {
              X_Min = 0.0;
              X_Max = RP->SS.Leftt.ShockVelocity * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->SS.Leftt.DensUpStream, RP->SS.Leftt.VelyUpStream, RP->SS.Leftt.PresUpStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->SS.Leftt.DensUpStream, RP->SS.Leftt.VelyUpStream, RP->SS.Leftt.PresUpStream );
		    }
		    else if ( Pattern == 2 )
		    {
              X_Min = 0.0;
              X_Max = RP->RS.Leftt.VelocityHead * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->RS.Leftt.DensUpStream, RP->RS.Leftt.VelyUpStream, RP->RS.Leftt.PresUpStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->RS.Leftt.DensUpStream, RP->RS.Leftt.VelyUpStream, RP->RS.Leftt.PresUpStream );
		    }
		    else if ( Pattern == 3 )
		    {
              X_Min = 0.0;
              X_Max = RP->SR.Leftt.ShockVelocity * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->SR.Leftt.DensUpStream, RP->SR.Leftt.VelyUpStream, RP->SR.Leftt.PresUpStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->SR.Leftt.DensUpStream, RP->SR.Leftt.VelyUpStream, RP->SR.Leftt.PresUpStream );
		    }
			else if ( Pattern == 4 )
			{
              X_Min = 0.0;
              X_Max = RP->RR.Leftt.VelocityHead * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->RR.Leftt.DensUpStream, RP->RR.Leftt.VelyUpStream, RP->RR.Leftt.PresUpStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->RR.Leftt.DensUpStream, RP->RR.Leftt.VelyUpStream, RP->RR.Leftt.PresUpStream );
			}
          break;

          case 2:
		    if ( Pattern == 2 )
		    {
			  X_Min = RP->RS.Leftt.VelocityHead * time + X0;
			  X_Max = RP->RS.Leftt.VelocityTail * time + X0 - dX;

			  RP->RS.Leftt.Xi = ( X_Min - X0 )/time;

              do
			  {
                Cs          = GetSoundSpeedInFan( &(RP->RS.Leftt) );
                DensFan     = GetDensInFan(Cs, RP -> RS.Leftt.PresUpStream, RP -> RS.Leftt.DensUpStream );
                PresFan     = GetPresInFan(DensFan, RP -> RS.Leftt.PresUpStream, RP -> RS.Leftt.DensUpStream );
                VelocityFan = GetVelocityInFan(Cs, RP->RS.Leftt.Xi, false);

                fprintf (fptr[j], "%e %e %e %e\n", RP->RS.Leftt.Xi*time+X0, DensFan, VelocityFan, PresFan );

			    RP->RS.Leftt.Xi += dX/time;
			  }
			  while( RP->RS.Leftt.Xi <= (X_Max-X0+dX)/time );
		    }
			else if ( Pattern == 4 )
			{
			  X_Min = RP->RR.Leftt.VelocityHead * time + X0;
			  X_Max = RP->RR.Leftt.VelocityTail * time + X0 - dX;

			  RP->RR.Leftt.Xi = ( X_Min - X0 )/time;

              do
			  {
                Cs          = GetSoundSpeedInFan( &(RP->RR.Leftt) );
                DensFan     = GetDensInFan(Cs, RP -> RR.Leftt.PresUpStream, RP -> RR.Leftt.DensUpStream );
                PresFan     = GetPresInFan(DensFan, RP -> RR.Leftt.PresUpStream, RP -> RR.Leftt.DensUpStream );
                VelocityFan = GetVelocityInFan(Cs, RP->RR.Leftt.Xi, false);

                fprintf (fptr[j], "%e %e %e %e\n", RP->RR.Leftt.Xi*time+X0, DensFan, VelocityFan, PresFan );

			    RP->RR.Leftt.Xi += dX/time;
			  }
			  while( RP->RR.Leftt.Xi <= (X_Max-X0+dX)/time );
			}
		  break;

          case 3:
		    if ( Pattern == 1 )
		    {
              X_Min = RP->SS.Leftt.ShockVelocity  * time + X0;
              X_Max = RP->SS.Leftt.VelyDownStream * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->SS.Leftt.DensDownStream, RP->SS.Leftt.VelyDownStream, RP->SS.Leftt.PresDownStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->SS.Leftt.DensDownStream, RP->SS.Leftt.VelyDownStream, RP->SS.Leftt.PresDownStream );
		    }
		    else if ( Pattern == 2 )
		    {
              X_Min = RP->RS.Leftt.VelocityTail   * time + X0;
              X_Max = RP->RS.Leftt.VelyDownStream * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min,  RP->RS.Leftt.DensDownStream, RP->RS.Leftt.VelyDownStream, RP->RS.Leftt.PresDownStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max,  RP->RS.Leftt.DensDownStream, RP->RS.Leftt.VelyDownStream, RP->RS.Leftt.PresDownStream );
		    }
		    else if ( Pattern == 3 )
		    {
              X_Min = RP->SR.Leftt.ShockVelocity  * time + X0;
              X_Max = RP->SR.Leftt.VelyDownStream * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e %d\n", X_Min,  RP->SR.Leftt.DensDownStream, RP->SR.Leftt.VelyDownStream, RP->SR.Leftt.PresDownStream, 3 );
              fprintf (fptr[j], "%e %e %e %e %d\n", X_Max,  RP->SR.Leftt.DensDownStream, RP->SR.Leftt.VelyDownStream, RP->SR.Leftt.PresDownStream, 3 );
		    }
		    else if ( Pattern == 4 )
		    {
              X_Min = RP->RR.Leftt.VelocityTail   * time + X0;
              X_Max = RP->RR.Leftt.VelyDownStream * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min,  RP->RR.Leftt.DensDownStream, RP->RR.Leftt.VelyDownStream, RP->RR.Leftt.PresDownStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max,  RP->RR.Leftt.DensDownStream, RP->RR.Leftt.VelyDownStream, RP->RR.Leftt.PresDownStream );
		    }
          break;

          case 4:
		    if ( Pattern == 1 )
		    {
              X_Min = RP->SS.Right.VelyDownStream* time + X0;
              X_Max = RP->SS.Right.ShockVelocity * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->SS.Right.DensDownStream, RP->SS.Right.VelyDownStream, RP->SS.Right.PresDownStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->SS.Right.DensDownStream, RP->SS.Right.VelyDownStream, RP->SS.Right.PresDownStream );
		    }
		    else if ( Pattern == 2 )
		    {
              X_Min = RP->RS.Right.VelyDownStream* time + X0;
              X_Max = RP->RS.Right.ShockVelocity * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->RS.Right.DensDownStream, RP->RS.Right.VelyDownStream, RP->RS.Right.PresDownStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->RS.Right.DensDownStream, RP->RS.Right.VelyDownStream, RP->RS.Right.PresDownStream );
		    }
		    else if ( Pattern == 3 )
		    {
              X_Min = RP->SR.Leftt.VelyDownStream * time + X0;
              X_Max = RP->SR.Right.VelocityTail   * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e %d\n", X_Min, RP->SR.Right.DensDownStream, RP->SR.Right.VelyDownStream, RP->SR.Right.PresDownStream, 4 );
              fprintf (fptr[j], "%e %e %e %e %d\n", X_Max, RP->SR.Right.DensDownStream, RP->SR.Right.VelyDownStream, RP->SR.Right.PresDownStream, 4 );
		    }
		    else if ( Pattern == 4 )
		    {
              X_Min = RP->RR.Right.VelyDownStream * time + X0;
              X_Max = RP->RR.Right.VelocityTail   * time + X0 - dX;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->RR.Right.DensDownStream, RP->RR.Right.VelyDownStream, RP->RR.Right.PresDownStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->RR.Right.DensDownStream, RP->RR.Right.VelyDownStream, RP->RR.Right.PresDownStream );
		    }
          break;

          case 5:
		    if ( Pattern == 3 )
		    {
			  X_Min = RP->SR.Right.VelocityTail * time + X0;
			  X_Max = RP->SR.Right.VelocityHead * time + X0 - dX;

			  RP->SR.Right.Xi = ( X_Min - X0 )/time;

              do
			  {
                Cs          = GetSoundSpeedInFan( &(RP->SR.Right) );
                DensFan     = GetDensInFan(Cs, RP -> SR.Right.PresUpStream, RP -> SR.Right.DensUpStream );
                PresFan     = GetPresInFan(DensFan, RP -> SR.Right.PresUpStream, RP -> SR.Right.DensUpStream );
                VelocityFan = GetVelocityInFan(Cs, RP->SR.Right.Xi, true);

                fprintf (fptr[j], "%e %e %e %e %d\n", RP->SR.Right.Xi*time+X0, DensFan, VelocityFan, PresFan, 5 );

			    RP->SR.Right.Xi += dX/time;
			  }
			  while( RP->SR.Right.Xi <= (X_Max-X0+dX)/time );
		    }
			else if ( Pattern == 4 )
			{
			  X_Min = RP->RR.Right.VelocityTail * time + X0;
			  X_Max = RP->RR.Right.VelocityHead * time + X0 - dX;

			  RP->RR.Right.Xi = ( X_Min - X0 )/time;

              do
			  {
                Cs          = GetSoundSpeedInFan( &(RP->RR.Right) );
                DensFan     = GetDensInFan(Cs, RP -> RR.Right.PresUpStream, RP -> RR.Right.DensUpStream );
                PresFan     = GetPresInFan(DensFan, RP -> RR.Right.PresUpStream, RP -> RR.Right.DensUpStream );
                VelocityFan = GetVelocityInFan(Cs, RP->RR.Right.Xi, true);

                fprintf (fptr[j], "%e %e %e %e %d\n", RP->RR.Right.Xi*time+X0, DensFan, VelocityFan, PresFan, 5 );

			    RP->RR.Right.Xi += dX/time;
			  }
			  while( RP->RR.Right.Xi <= (X_Max-X0+dX)/time );
			}
		  break;

          case 6:
		    if ( Pattern == 1 )
		    {
              X_Min =  RP->SS.Right.ShockVelocity * time + X0;
              X_Max =  X_Right;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->SS.Right.DensUpStream, RP->SS.Right.VelyUpStream, RP->SS.Right.PresUpStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->SS.Right.DensUpStream, RP->SS.Right.VelyUpStream, RP->SS.Right.PresUpStream);
		    }
		    else if ( Pattern == 2 )
		    {
              X_Min =  RP->RS.Right.ShockVelocity * time + X0;
              X_Max =  X_Right;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->RS.Right.DensUpStream, RP->RS.Right.VelyUpStream, RP->RS.Right.PresUpStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->RS.Right.DensUpStream, RP->RS.Right.VelyUpStream, RP->RS.Right.PresUpStream);
		    }
		    else if ( Pattern == 3 )
		    {
              X_Min = RP->SR.Right.VelocityHead * time + X0;
              X_Max = X_Right;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->SR.Right.DensUpStream, RP->SR.Right.VelyUpStream, RP->SR.Right.PresUpStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->SR.Right.DensUpStream, RP->SR.Right.VelyUpStream, RP->SR.Right.PresUpStream );
		    }
			else if ( Pattern == 4 )
			{
              X_Min = RP->RR.Right.VelocityHead * time + X0;
              X_Max = X_Right;

              fprintf (fptr[j], "%e %e %e %e\n", X_Min, RP->RR.Right.DensUpStream, RP->RR.Right.VelyUpStream, RP->RR.Right.PresUpStream );
              fprintf (fptr[j], "%e %e %e %e\n", X_Max, RP->RR.Right.DensUpStream, RP->RR.Right.VelyUpStream, RP->RR.Right.PresUpStream );
			}
          break;
        }
    }

    fclose (fptr[j]);

    j++;

    time = j * DT;
  }

}