/*
 * AER8875 - Projet Integrateur IV en Aerospatial
 * Safety Margin Calculator Tool - An Implementation For Python
 * Created by K. Abdallah
 * Revision History:
 *  Version Date          Author         Comment
    1.0     02/25/2023    K. Abdallah    Creation
*/

/*
 * File:    solve.c
 * ----------------
 * Main compiled file to solve MS problems
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "stringer.h"
#include "stringer_types.h"

#define DllExport __declspec(dllexport)


/**
 * @brief Returns the safety margin of an axialy loaded beam.
 * 
 * @param stringer_type Stringer shape
 * @param F Applied force on the beam [ksi]
 * @param in_len Stringer length [in]
 * @param in_skin_thickness Skin thickness [in]
 * @param in_skin_width Skin width [in]
 * @param Fcy Material yield strength [ksi]
 * @param Fty Material tensile strength [ksi]
 * @param Ec Material Young's modulus in compression [ksi]
 * @param Nu Material Poisson's ratio [-]
 * @param in_bw Beam web base [in]
 * @param in_tw Beam web thickness [in]
 * @param in_bf Beam flange base [in]
 * @param in_tf Beam flange thickness [in]
 * @param in_bp Beam pad-up base [in]
 * @param in_tp Beam pad-up thickness [in]
 * @return double 
 */
DllExport double solve(int stringer_type, double F, double in_len, double in_skin_thickness, double in_skin_width, 
    double Fcy, double Fty, double Ec, double Nu, double in_bw, double in_tw, double in_bf, double in_tf,
    double in_bp, double in_tp) {

    // Create stringer object
    stringer* st = new_stringer(stringer_type, in_skin_thickness, in_skin_width, in_bw, in_tw, in_bf, in_tf,
        in_bp, in_tp);

    double MS;

    // If beam is in tension
    if (F >= 0.0) {
        double f_allowed = Fcy;
        double f_applied = F/(st->A_pa+st->A_st);
        MS = fabs(f_allowed/f_applied) - 1;
    }
    // If beam is in compression
    else {
        double fixity_const = 1;
        double iterator_err_thresh = 0.05;
        double f_allowed = stringer_stress_routine(st->num_of_elem, fixity_const, iterator_err_thresh, in_skin_thickness,
            in_len, st->A_st, st->Y_bar, st->rho_st, Ec, Fcy, st->BC, st->bi, st->ti);
        double Aesk = stringer_area_effective_width(in_skin_thickness, st->A_st, st->Y_bar, Ec, f_allowed);
        double f_applied = F/(Aesk+st->A_st);
        MS = fabs(f_allowed/f_applied) - 1;
    }

    // Deleting stringer object
    del_stringer(st);

    return MS;
}
