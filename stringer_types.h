/*
 * AER8875 - Projet Integrateur IV en Aerospatial
 * Safety Margin Calculator Tool - An Implementation For Python
 * Created by K. Abdallah
 * Revision History:
 *  Version Date          Author         Comment
    1.0     02/26/2023    K. Abdallah    Creation
*/

/*
 * File:    stringer_types.h
 * -------------------------
 * Header file for stringer_types.c
*/

#ifndef STRINGER_TYPES_H
    #define STRINGER_TYPES_H

    #ifndef NULLPTR
        #define NULLPTR
        #define nullptr (void*)0
    #endif // NULLPTR
    
    typedef struct stringer {
        char type;
        int num_of_elem;
        double* b;
        double* h;
        double* d;
        double* BC;
        double* bi;
        double* ti;
        double A_pa;
        double A_st;
        double Y_st;
        double Y_bar;
        double I_st;
        double rho_st;
    } stringer;

    /* Function declaration */
    void update_stringer_z(stringer* st, double in_skin_thickness, double in_skin_width,
        double in_bw, double in_tw, double in_bf, double in_tf, double in_tp, double in_bp);

    #ifndef STRINGER_TYPES
        #define STRINGER_TYPES
        #include "stringer_types.c"
    #endif // STRINGER_TYPES

#endif // STRINGER_TYPES_H
