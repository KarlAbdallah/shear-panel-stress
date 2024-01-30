import ctypes


# dll = ctypes.WinDLL(".\\make\\solve.dll")
dll = ctypes.WinDLL("solve.dll")

dll_solve_prototype = ctypes.WINFUNCTYPE(
    ctypes.c_double,
    ctypes.c_int,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double,
    ctypes.c_double
)

dll_solve_params = (1,"stringer_type"), (1,"F"), (1,"in_len"), (1,"in_skin_thickness"), \
    (1,"in_skin_width"), (1,"Fcy"), (1,"Fty"), (1,"Ec"), (1,"Nu"), (1,"in_bw"), (1,"in_tw"), \
    (1,"in_bf"), (1,"in_tf"), (1,"in_bp"), (1,"in_tp")

safety_margin = dll_solve_prototype(("solve", dll), dll_solve_params)

MS = safety_margin(stringer_type=1,
                   F=-5,
                   in_len=15,
                   in_skin_thickness=0.1,
                   in_skin_width=6,
                   Fcy=70,
                   Fty=70,
                   Ec=10700,
                   Nu=0.330,
                   in_bw=.94,
                   in_tw=0.08,
                   in_bf=0.5,
                   in_tf=0.08,
                   in_bp=0.7,
                   in_tp=0.02)

print(MS)