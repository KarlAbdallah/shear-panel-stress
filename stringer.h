/*
 * AER8875 - Projet Integrateur IV en Aerospatial
 * Safety Margin Calculator Tool - An Implementation For Python
 * Created by K. Abdallah
 * Revision History:
 *  Version Date          Author         Comment
    1.0     02/25/2023    K. Abdallah    Creation
*/

/*
 * File:    stringer.h
 * -------------------
 * Header file for stringer.c
*/


#ifndef STRINGER_H
    #define STRINGER_H

    #ifndef NULLPTR
        #define NULLPTR
        #define nullptr (void*)0
    #endif // NULLPTR

    #ifndef PI
        #define PI 3.14159265358979323846   /* pi */
    #endif // PI

    #ifndef STRINGER
        #define STRINGER
        #include "stringer.c"
    #endif // STRINGER

#endif // STRINGER_H