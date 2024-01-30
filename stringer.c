/*
 * AER8875 - Projet Integrateur IV en Aerospatial
 * Safety Margin Calculator Tool - An Implementation For Python
 * Created by K. Abdallah
 * Revision History:
 *  Version Date          Author         Comment
    1.0     01/19/2023    K. Abdallah    Creation
    2.0     02/25/2023    K. Abdallah    Slight revisions and refactoring
*/

/*
 * File:    stringer.c
 * -------------------
 * Implements all necessary functions to calculate safety margins
*/


#include <stdlib.h>
#include <math.h>


/**
 * @brief Convenience function to square a number.
 * 
 * @param x Number to double
 * @return double
 */
double square(double x){
    return x*x;
}

/**
 * @brief Convenience function to raise to the cube a number.
 * 
 * @param x Number to raise to the cube power
 * @return double 
 */
double cube(double x){
    return x*x*x;
}

/**
 * @brief Convenience function to calculate the area of a rectangle.
 * 
 * @param a Size of side a
 * @param b Size of side b
 * @return double 
 */
double area(double a, double b){
    return a*b;
}

/**
 * @brief Convenience function to calculate the moment of inertia
 *  of a rectangle.
 * 
 * @param b Size of the base of the rectangle
 * @param h Size of the height of the rectangle
 * @return double 
 */
double inertia_rect(double b, double h){
    return b*cube(h)/12;
}

/**
 * @brief Convenience function to calculate the cross-sectional area
 * of a panel.
 * 
 * @param skin_thickness Thickness of the skin
 * @param skin_width Width of the panel
 * @return double 
 */
double panel_area(double skin_thickness, double skin_width){
    return skin_thickness*skin_width;
}

/**
 * @brief Populates a vector containing areas.
 * 
 * @param num_of_elem Size of the vector
 * @param b Pointer to an array of rectangle bases
 * @param h Pointer to an array of rectangle heights
 * @return double* 
 */
double* stringer_area_vector(int num_of_elem, double* b, double* h){
    double* A = (double*)malloc(num_of_elem * sizeof(double));
    for (int i = 0; i < num_of_elem; i++){
        A[i] = area(b[i], h[i]);
    }
    return A;
}

/**
 * @brief Given a pointer to a vector of areas, calculates the sum of all
 * elements.
 * 
 * @param num_of_elem Number of elements in the vector
 * @param A Pointer to the vector of areas
 * @return double 
 */
double stringer_area(int num_of_elem, double* A){
    double area_stringer = 0;
    for (int i = 0; i < num_of_elem; i++){
        area_stringer += A[i];
    }
    // free(A);
    return area_stringer;
}

/**
 * @brief Populates a vector containing inertias.
 * 
 * @param num_of_elem 
 * @param b Pointer to an array of rectangle bases
 * @param h Pointer to an array of rectangle heights
 * @return double* 
 */
double* stringer_inertia_vector(int num_of_elem, double* b, double* h){
    double* I = (double*)malloc(num_of_elem * sizeof(double));
    for (int i = 0; i < num_of_elem; i++){
        I[i] = inertia_rect(b[i], h[i]);
    }
    return I;
}

/**
 * @brief Given a pointer to a vector of inertias, calculates total inertia.
 * 
 * @param num_of_elem Number of elements in the vector
 * @param I Pointer to the vector of intertias
 * @param b Pointer to an array of rectangle bases
 * @param h Pointer to an array of rectangle heights
 * @param d Pointer to an array of distances from each rectangle to the desired axis
 * @return double 
 */
double stringer_inertia(int num_of_elem, double* I, double* b, double* h, double* d){
    double inertia_stringer = 0;
    for (int i = 0; i < num_of_elem; i++){
        inertia_stringer += I[i] + b[i]*h[i]*square(d[i]);
    }
    // free(I);
    return inertia_stringer;
}

/**
 * @brief Calculates the first moment of area.
 * 
 * @param num_of_elem Number of elements in the vectors
 * @param A Pointer to an array of areas
 * @param y_bar_elem Pointer the an array of centroids
 * @return double 
 */
double first_moment_of_area(int num_of_elem, double* A, double* y_bar_elem){
    double Ay = 0;
    for (int i = 0; i < num_of_elem; i++){
        Ay += A[i]*y_bar_elem[i];
    }
    return Ay;
}

/**
 * @brief Calculates the centroid of a stringer.
 * 
 * @param Ay First moment of area
 * @param A Total area
 * @return double 
 */
double stringer_centroid(double Ay, double A){
    return Ay/A;
}

/**
 * @brief Calculates the radius of gyration of the stringer.
 * 
 * @param I Moment of inertia of the stringer
 * @param A Total area
 * @return double 
 */
double stringer_gyration_radius(double I, double A){
    return sqrt(I/A);
}

/**
 * @brief Populates a vector of Fcc.
 * 
 * @param num_of_elem Number of elements
 * @param BC Pointer to an array of boundary conditions
 * @param bi Pointer to an array of bases
 * @param ti Pointer to an array of thicknesses
 * @param Ec Young's modulus in compression
 * @param Fcy Yield strength
 * @param F Applied stress
 * @return double* 
 */
double* stringer_fcci(int num_of_elem, double* BC, double* bi, double* ti, double Ec, double Fcy, double F){
    double* fcci = (double*)malloc((num_of_elem-1) * sizeof(double));
    double Ce;
    for (int i = 0; i < (num_of_elem-1); i++){
        if (BC[i] == 1){
            Ce = 0.556;
        }
        else{
            Ce = 1.425;
        }
        fcci[i] = Ce * pow(F, .6) * pow(Ec, .4) / pow(bi[i]/ti[i], .8);
        if (fcci[i] > Fcy){
            fcci[i] = Fcy;
        }
    }
    return fcci;
}

/**
 * @brief Calculates the slenderness ratio lambda.
 * 
 * @param fixity_const Boundary condition of beam
 * @param in_len Stringer length
 * @param rho Radius of gyration
 * @return double 
 */
double stringer_lambda(double fixity_const, double in_len, double rho){
    return in_len/rho/sqrt(fixity_const);
}

/**
 * @brief Calculates permissible crippling stress in a beam.
 * 
 * @param num_of_elem Number of elements in the beam
 * @param BC Pointer to an array of boundary conditions
 * @param bi Pointer to an array of bases
 * @param ti Pointer to an array of thicknesses
 * @param Ec Young's modulus in compression
 * @param Fcy Yield strength
 * @param F Applied stress
 * @return double 
 */
double stringer_crippling(int num_of_elem, double* BC, double* bi, double* ti, double Ec, double Fcy, double F){
    double btfcc = 0;
    double bt = 0;
    double* fcci;
    fcci = stringer_fcci(num_of_elem, BC, bi, ti, Ec, Fcy, F);
    for (int i = 0; i < (num_of_elem-1); i++){
        bt += bi[i]*ti[i];
        btfcc += bi[i]*ti[i]*fcci[i];
    }
    free(fcci);
    return btfcc/bt;
}

/**
 * @brief Calculates permissible buckling stress in a beam.
 * 
 * @param Ec Young's modulus in compression
 * @param lambda Slenderness ratio
 * @return double 
 */
double stringer_buckling(double Ec, double lambda){
    return square(PI) * Ec / square(lambda);
}

/**
 * @brief Calculates permissible stress in Euler-Johnson Interaction in a beam.
 * 
 * @param num_of_elem Number of elements in the beam
 * @param lambda Slenderness ration
 * @param BC Pointer to an array of boundary conditions
 * @param bi Pointer to an array of bases
 * @param ti Pointer to an array of thicknesses
 * @param Ec Young's modulus in compression
 * @param Fcy Yield strength
 * @return double 
 */
double stringer_euler_johnson_interaction(int num_of_elem, double lambda, double* BC, double* bi, double* ti, double Ec, double Fcy){
    double fcc;
    fcc = stringer_crippling(num_of_elem, BC, bi, ti, Ec, Fcy, Fcy);
    return fcc - square(fcc)/4/square(PI)/Ec*square(lambda);
}

/**
 * @brief Calculates the critical slenderness ratio for column buckling.
 * 
 * @param Ec Young's modulus in compression
 * @param fcc Crippling stress
 * @return double 
 */
double stringer_lambda_crit(double Ec, double fcc){
    return sqrt(2*square(PI)*Ec/fcc);
}

/**
 * @brief Effective skin width in buckling.
 * 
 * @param in_skin_thickness Skin thickness
 * @param A_st Stringer total area
 * @param Y_st Stringer centroid
 * @param Ec Young's modulus in compression
 * @param F Applied stress
 * @return double 
 */
double stringer_area_effective_width(double in_skin_thickness, double A_st, double Y_st, double Ec, double F){
    double Aesk, be, s;
    be = 1.9 * in_skin_thickness * sqrt(Ec/F);
    Aesk = area(be, in_skin_thickness);
    return Aesk;
}

/**
 * @brief Calculates stringer radius of gyration taking into account effective skin width.
 * 
 * @param A_st Stringer area
 * @param rho_st Stringer radius of gyration
 * @param Aesk Effective skin width
 * @param Y_st Stringer centroid
 * @param in_skin_thickness Skin thickness
 * @return double 
 */
double stringer_rho_effective_width(double A_st, double rho_st, double Aesk, double Y_st, double in_skin_thickness){
    double s, rho;
    s = Y_st - in_skin_thickness/2;
    double a = 1 + square(s/rho_st);
    double b = Aesk / A_st;
    double c = 1 + b;
    double d = square(c);
    // rho = sqrt((1 + (1 + square(s / rho_st)) * Aesk / A_st) / square(1 + Aesk / A_st)) * rho_st;
    rho = sqrt((1 + a*b)/d) * rho_st;
    return rho;
}

/**
 * @brief 
 * 
 * @param num_of_elem Number of elements in the beam
 * @param fixity_const Boundary condition of beam
 * @param iterator_err_thresh Err threshold between iterations
 * @param in_skin_thickness Skin thickness
 * @param in_len Stringer length
 * @param A_st Stringer area
 * @param Y_st Stringer centroid
 * @param rho_st Stringer radius of gyration
 * @param Ec Young's modulus in compression
 * @param Fcy Yield strength
 * @param BC Pointer to an array of boundary conditions
 * @param bi Pointer to an array of bases
 * @param ti Pointer to an array of thicknesses
 * @return double 
 */
double stringer_stress_routine(
    int num_of_elem, double fixity_const, double iterator_err_thresh, double in_skin_thickness, double in_len,
    double A_st, double Y_st, double rho_st, double Ec, double Fcy, double* BC, double* bi, double* ti){
    double err = 1;
    double F, F1, Aesk, lambda, lambda_crit, rho_e;

    F = stringer_crippling(num_of_elem, BC, bi, ti, Ec, Fcy, Fcy);
    lambda = stringer_lambda(fixity_const, in_len, rho_st);
    lambda_crit = stringer_lambda_crit(Ec, F);

    while (err > iterator_err_thresh){
        F1 = F;
        Aesk = stringer_area_effective_width(in_skin_thickness, A_st, Y_st, Ec, F);
        rho_e = stringer_rho_effective_width(A_st, rho_st, Aesk, Y_st, in_skin_thickness);
        lambda = stringer_lambda(fixity_const, in_len, rho_e);

        if (0 <= lambda && lambda <= 20){
            F = stringer_crippling(num_of_elem, BC, bi, ti, Ec, Fcy, F);
        }
        else if (20 < lambda && lambda <= lambda_crit){
            F = stringer_euler_johnson_interaction(num_of_elem, lambda, BC, bi, ti, Ec, Fcy);
        }
        else if (lambda_crit < lambda){
            F = stringer_buckling(Ec, lambda);
        }
        err = (F1 - F) / F;
    }
    return F;
}