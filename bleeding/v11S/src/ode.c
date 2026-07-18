#include "ml_compiler.h"
#include "ml_ode.h"

ML_API double ml_ode_euler(ml_ode_func_t f, double t0, double y0, double dt, int steps) {
    double t = t0, y = y0;
    for (int i = 0; i < steps; i++) {
        y += dt * f(t, y);
        t += dt;
    }
    return y;
}

ML_API double ml_ode_rk4(ml_ode_func_t f, double t0, double y0, double dt, int steps) {
    double t = t0, y = y0;
    for (int i = 0; i < steps; i++) {
        double k1 = f(t, y);
        double k2 = f(t + 0.5 * dt, y + 0.5 * dt * k1);
        double k3 = f(t + 0.5 * dt, y + 0.5 * dt * k2);
        double k4 = f(t + dt, y + dt * k3);
        y += (dt / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
        t += dt;
    }
    return y;
}
