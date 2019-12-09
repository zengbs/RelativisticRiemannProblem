#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Struct.h"
#include "Prototypes.h"
#include "Global.h"
#include "Macro.h"

// Pattern 1: shock-shock
//         2: rarefaction-shock
//         3: rarefaction-rarefaction

void QuadraticSolver( double A, double B, double C , double *PlusRoot, double *MinusRoot);

int GetWavePattern( struct InitialCondition *IC )
{
  double DensLeft      = IC -> DensLeft     ;
  double VelocityLeft  = IC -> VelocityLeft ;
  double PresLeft      = IC -> PresLeft     ;
  double DensRight     = IC -> DensRight    ;
  double VelocityRight = IC -> VelocityRight;
  double PresRight     = IC -> PresRight    ;

  double SS, RS, RR;
  double V_LC, V_RC;
  bool Shock_Yes = true;
  bool Shock_No  = false;

  double A_Plus, A_Minus;

  // shock-shock
  double A, B, C, EnthalpyRight, Root, Engy_Temp, EngyRight;

  EnthalpyRight = Flu_Enthalpy ( PresRight, DensRight );
  EngyRight     = Flu_TotalEngy( PresRight, DensRight );

  A = 1.0 + ( Gamma_1 / Gamma ) * ( PresRight / PresLeft - 1.0 );
  B = - ( Gamma_1 / Gamma ) * ( PresRight / PresLeft - 1.0 );
  C = EnthalpyRight * ( PresRight - PresLeft ) / DensRight - SQR(EnthalpyRight);

  QuadraticSolver( A, B, C, &Root, NULL ); 

  Engy_Temp = ( Gamma / Gamma_1 ) * ( Root / (Root-1.0) ) * PresLeft - PresLeft;// eq. (4.165)

  SS = sqrt( ( PresLeft-PresRight )*( Engy_Temp-EngyRight )/( Engy_Temp+PresRight )/( EngyRight+PresLeft ) );

  // rarefaction-shock
  double DensStarLeft = DensLeft*pow(  PresRight/PresLeft, 1.0/Gamma );

  A_Plus = A_PlusFun( PresRight, DensStarLeft, PresLeft, DensLeft );

  V_LC  = ( 1.0 - A_Plus ) / ( 1.0 + A_Plus ); // eq.(4.172)

  RS = V_LC;

  // rarefaction-rarefaction

  A_Plus  = A_PlusFun ( 0.0, NAN, PresLeft,  DensLeft  );
  A_Minus = A_MinusFun( 0.0, NAN, PresRight, DensRight );

  RR = - ( A_Plus - A_Minus )/( A_Plus + A_Minus ); // put p3=p3'=0 into eq.(4.177) 

  // relative velocity  
  double RelitiveVelocity;
  int Pattern;
  RelitiveVelocity = ( VelocityLeft - VelocityRight ) / ( 1.0 - VelocityLeft*VelocityRight );

  printf("RelitiveVelocity=%e\n", RelitiveVelocity);
  printf("SS=%e, RS=%e, RR=%e\n", SS, RS, RR);

  if ( RelitiveVelocity >= SS )
  {
    Pattern = 1;
	printf("you have shock-shock wave pattern !!\n", Pattern);
  }
  else if (  RS <= RelitiveVelocity && RelitiveVelocity < SS )
  {
    Pattern = 2;
	printf("you have rarefaction-shock wave pattern !!\n", Pattern);
  }
  else if ( RR <= RelitiveVelocity && RelitiveVelocity < RS )
  {
    Pattern = 3;
	printf("you have rarefaction-rarefaction wave pattern !!\n", Pattern);
  }
  else
  {
    printf("wave pattern was not found!!\n");
	exit(1);
  }

  return Pattern;

}


double Velocity_LC ( double PresStar, double DensStarLeft, double PresLeft, double DensLeft, bool Shock )
{
  double Velocity_LC;

  if ( Shock == true )
  {
     double EngyStarLeft, EngyLeft, EnthalpyStarLeft;

     EnthalpyStarLeft = TaubAdiabatic( PresLeft, DensLeft, PresStar );
 printf("PresStar=%e\n", PresStar);
	 EngyStarLeft = PresStar * ( EnthalpyStarLeft / (EnthalpyStarLeft-1.0) ) * ( Gamma / Gamma_1 ) - PresStar;
     printf("EnthalpyStarLeft=%e, EngyStarLeft=%e\n", EnthalpyStarLeft, EngyStarLeft );
	 EngyLeft = Flu_TotalEngy( PresLeft, DensLeft );

     Velocity_LC  = ( PresStar - PresLeft ) * ( EngyStarLeft - EngyLeft );
	 Velocity_LC /= ( EngyLeft + PresStar ) * ( EngyStarLeft + PresLeft );

     if ( Velocity_LC < 0.0 )
	 {
	   printf("Velocity_LC = nan !!", Velocity_LC);
	   exit(1);
	 }

	 Velocity_LC  = sqrt( Velocity_LC );
  
     return Velocity_LC;
  }
  else
  {
     if ( DensStarLeft != DensStarLeft )
	 {
	   printf( "DensStarLeft should be provided!!\n" );
	   exit(1);
	 }

     Velocity_LC  = 1.0 - A_PlusFun( PresStar, DensStarLeft, PresLeft, DensLeft ); 
     Velocity_LC /= 1.0 + A_PlusFun( PresStar, DensStarLeft, PresLeft, DensLeft ); 
  
     return Velocity_LC; 
  }
}

double Velocity_RC ( double PresStar, double DensStarRight, double PresRight, double DensRight, bool Shock )
{
  double Velocity_RC;

  if ( Shock == true )
  {
     double EngyStarRight, EngyRight, EnthalpyStarRight;

     EnthalpyStarRight = TaubAdiabatic( PresRight, DensRight, PresStar );

	 EngyStarRight = PresStar * ( EnthalpyStarRight / (EnthalpyStarRight-1.0) ) * ( Gamma / Gamma_1 ) - PresStar;

	 EngyRight = Flu_TotalEngy( PresRight, DensRight );

     Velocity_RC  = ( PresStar - PresRight ) * ( EngyStarRight - EngyRight );

	 printf("PresStar=%e, PresRight=%e, EngyStarRight=%e, EngyRight=%e\n", 
			PresStar, PresRight, EngyStarRight, EngyRight); 

	 Velocity_RC /= ( EngyRight + PresStar ) * ( EngyStarRight + PresRight );

     if ( Velocity_RC < 0.0 )
	 {
	   printf("Velocity_RC = nan !!", Velocity_RC);
	   exit(1);
	 }

	 Velocity_RC  = -sqrt( Velocity_RC );

     return Velocity_RC;
  }
  else
  {
     if ( DensStarRight != DensStarRight )
	 {
	   printf( "DensStarRight should be provided!!\n" );
	   exit(1);
	 }

     Velocity_RC  = 1.0 - A_MinusFun( PresStar, DensStarRight, PresRight, DensRight ); 
     Velocity_RC /= 1.0 + A_MinusFun( PresStar, DensStarRight, PresRight, DensRight ); 
  
     return Velocity_RC; 
  }
}


double A_PlusFun ( double Pres, double Dens, double PresLeft, double DensLeft )
{
    double CsLeft, Cs, Sqrt_Gamma_1, A_Plus;

	CsLeft = Flu_SoundSpeed ( PresLeft, DensLeft );
	Cs     = Flu_SoundSpeed ( Pres    , Dens     );

    Sqrt_Gamma_1 = sqrt( Gamma_1 );

	A_Plus  = ( Sqrt_Gamma_1 - Cs     ) / ( Sqrt_Gamma_1 + Cs     );
	A_Plus *= ( Sqrt_Gamma_1 + CsLeft ) / ( Sqrt_Gamma_1 - CsLeft );
    A_Plus  = pow( A_Plus, 2.0/Sqrt_Gamma_1 );

	return A_Plus;
}


double A_MinusFun ( double Pres, double Dens, double PresRight, double DensRight )
{
    double CsRight, Cs, Sqrt_Gamma_1, A_Minus;

	CsRight = Flu_SoundSpeed ( PresRight, DensRight );
	Cs      = Flu_SoundSpeed ( Pres     , Dens      );

    Sqrt_Gamma_1 = sqrt( Gamma_1 );

	A_Minus  = ( Sqrt_Gamma_1 - Cs      ) / ( Sqrt_Gamma_1 + Cs      );
	A_Minus *= ( Sqrt_Gamma_1 + CsRight ) / ( Sqrt_Gamma_1 - CsRight );
    A_Minus  = pow( A_Minus, -2.0/Sqrt_Gamma_1 );

	return A_Minus;
}

double TaubAdiabatic ( double PresUp, double DensUp, double PresDown )
{
    double EnthalpyUp, EnthalpyDown, PresDiff;

	EnthalpyUp = Flu_Enthalpy( PresUp, DensUp );

    PresDiff = PresUp - PresDown;

    double A, B, C;

	A = 1.0 + Gamma_1 * PresUp / PresDown;
	B = - Gamma_1 * PresDiff / PresDown;
	C = - Gamma_1 * PresDiff * EnthalpyUp / PresUp - ( 1.0 + Gamma_1 * PresDown / PresUp )*SQR(EnthalpyUp);

    QuadraticSolver( A, B, C, &EnthalpyDown, NULL );

    return EnthalpyDown;
}

double PresFunction( double PresStar, void  *params )
{

  if ( PresStar <= 0.0 )
  {
    printf("PresStar=%e !!\n", PresStar);
	exit(1);
  }

  struct InitialCondition *IC = ( struct InitialCondition * ) params;
  double DensLeft      = IC -> DensLeft     ;
  double VelocityLeft  = IC -> VelocityLeft ;
  double PresLeft      = IC -> PresLeft     ;
  double DensRight     = IC -> DensRight    ;
  double VelocityRight = IC -> VelocityRight;
  double PresRight     = IC -> PresRight    ;
  
  int Pattern;
  bool Shock_Yes = true;
  bool Shock_No  = false;
  double V_LC, V_RC, V_LR;
  double DensStarLeft, DensStarRight;

  Pattern = GetWavePattern( IC );

  if ( Pattern == 1 )
  {
    V_LC = Velocity_LC( PresStar, NAN, PresLeft,   DensLeft, Shock_Yes ); // left side of eq. (4.161)
    V_RC = Velocity_RC( PresStar, NAN, PresRight, DensRight, Shock_Yes ); // right side of eq. (4.161)

    V_LR = ( V_LC - V_RC )/( 1.0 - V_LC*V_RC );
  }
  else if ( Pattern == 2 )
  {
    DensStarLeft = DensLeft*pow(  PresStar/PresLeft, 1.0/Gamma );

    V_LC = Velocity_LC( PresStar, DensStarLeft, PresLeft,   DensLeft, Shock_No  ); // eq. (4.168)
    V_RC = Velocity_RC( PresStar, NAN,         PresRight,  DensRight, Shock_Yes ); // right side of eq. (4.161) 

    V_LR = ( V_LC - V_RC )/( 1.0 - V_LC*V_RC );
  }
  else if ( Pattern == 3 )
  {
    DensStarLeft  = DensLeft *pow(  PresStar/PresLeft,  1.0/Gamma );
    DensStarRight = DensRight*pow(  PresStar/PresRight, 1.0/Gamma );

	double A_Plus, A_Minus;

	A_Plus  = A_PlusFun( PresStar,  DensStarLeft,  PresLeft,  DensLeft );
	A_Minus = A_MinusFun( PresStar, DensStarRight, PresRight, DensRight ); 

    V_LR    = - ( A_Plus - A_Minus ) / ( A_Plus + A_Minus ); // eq. (4.177)
  }

  double RelitiveVelocity;

  RelitiveVelocity = ( VelocityLeft - VelocityRight ) / ( 1.0 - VelocityLeft*VelocityRight );
  return RelitiveVelocity - V_LR;
}



void QuadraticSolver( double A, double B, double C , double *PlusRoot, double *MinusRoot)
{
  double Delta;

  Delta = sqrt( B*B - 4.0*A*C );

  if ( PlusRoot  != NULL )  *PlusRoot  = -2.0*C/( +B + Delta);
  if ( MinusRoot != NULL )  *MinusRoot = +2.0*C/( -B + Delta);

}
