#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <string>
#include <iomanip>

struct State {
    double phi;
    double phi_dot;
    double N;
};

const double M_INFLATON = 1e-5;

double potential(double phi) {
    return 0.5 * M_INFLATON * M_INFLATON * phi * phi;
}

double potential_prime(double phi) {
    return M_INFLATON * M_INFLATON * phi;
}

double compute_H(double phi, double phi_dot) {
    double rho = 0.5 * phi_dot * phi_dot + potential(phi);
    return std::sqrt(rho / 3.0);
}

State evaluate_derivatives(const State& st) {
    double H = compute_H(st.phi, st.phi_dot);
    State deriv;
    deriv.phi = st.phi_dot;
    deriv.phi_dot = -3.0 * H * st.phi_dot - potential_prime(st.phi);
    deriv.N = H;
    return deriv;
}

State rk4_step(const State& s, double dt) {
    State k1 = evaluate_derivatives(s);

    State s2 = {s.phi + 0.5 * dt * k1.phi, s.phi_dot + 0.5 * dt * k1.phi_dot, s.N + 0.5 * dt * k1.N};
    State k2 = evaluate_derivatives(s2);

    State s3 = {s.phi + 0.5 * dt * k2.phi, s.phi_dot + 0.5 * dt * k2.phi_dot, s.N + 0.5 * dt * k2.N};
    State k3 = evaluate_derivatives(s3);

    State s4 = {s.phi + dt * k3.phi, s.phi_dot + dt * k3.phi_dot, s.N + dt * k3.N};
    State k4 = evaluate_derivatives(s4);

    State next;
    next.phi = s.phi + (dt / 6.0) * (k1.phi + 2.0 * k2.phi + 2.0 * k3.phi + k4.phi);
    next.phi_dot = s.phi_dot + (dt / 6.0) * (k1.phi_dot + 2.0 * k2.phi_dot + 2.0 * k3.phi_dot + k4.phi_dot);
    next.N = s.N + (dt / 6.0) * (k1.N + 2.0 * k2.N + 2.0 * k3.N + k4.N);
    return next;
}

void draw_frame(const State& s, double t, bool active) {
    std::cout << "\033[H";

    std::cout << "========================================================\033[K\n";
    std::cout << "        COSMIC INFLATION SIMULATION (C++ Engine)        \033[K\n";
    std::cout << "========================================================\033[K\n";

    std::cout << " Time (t)   : " << static_cast<int>(t) << " t_Planck\033[K\n";
    std::cout << " Field (phi): " << std::fixed << std::setprecision(4) << s.phi << " M_pl\033[K\n";
    std::cout << " Expansion  : e^" << static_cast<int>(s.N) << " folds (~10^"
              << static_cast<int>(s.N * 0.434) << ")\033[K\n";
    std::cout << " Phase      : "
              << (active ? "\033[1;32m[ SLOW-ROLL INFLATION ]\033[0m" : "\033[1;31m[ REHEATING / OSCILLATION ]\033[0m")
              << "\033[K\n\n";

    std::cout << "--- Potential V(phi) = 1/2 m^2 phi^2 ---\033[K\n";
    const int width = 24;
    int ball = static_cast<int>((s.phi / 15.0) * width);
    if (ball < 0) ball = 0;
    if (ball > width) ball = width;

    for (int y = 5; y >= 0; --y) {
        std::string row = " |";
        for (int x = 0; x <= width; ++x) {
            int curve = static_cast<int>((static_cast<double>(x * x) / (width * width)) * 5.0);
            if (x == ball && y == curve) {
                row += "\033[1;33m(O)\033[0m";
            } else if (y == curve) {
                row += ".";
            } else {
                row += " ";
            }
        }
        std::cout << row << "\033[K\n";
    }
    std::cout << " +--------------------------\033[K\n";
    std::cout << "  phi=0                   phi=15\033[K\n\n";

    std::cout << "--- Universe Scale Horizon a(t) ---\033[K\n ";
    int bar = static_cast<int>((s.N / 65.0) * 26);
    if (bar > 26) bar = 26;
    if (bar < 0) bar = 0;
    std::cout << "[" << std::string(bar, '#') << std::string(26 - bar, ' ') << "]\033[K\n";
    std::cout << "========================================================\033[K\n";
}

int main() {
    std::cout << "\033[2J";

    State s;
    s.phi = 15.0;
    s.phi_dot = 0.0;
    s.N = 0.0;

    double t = 0.0;
    double dt = 250.0;

    while (t < 2.5e6) {
        double H = compute_H(s.phi, s.phi_dot);
        double epsilon = (s.phi_dot * s.phi_dot) / (2.0 * H * H);
        bool active = (epsilon < 1.0 && s.phi > 0.8);

        draw_frame(s, t, active);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));

        s = rk4_step(s, dt);
        t += dt;

        if (!active && t > 1.7e6) break;
    }

    std::cout << "\n>> SIMULATION FINISHED: Hot Big Bang epoch initiated.\n";
    return 0;
}

g++ -O3 -std=c++17 cosmicinflation.cpp -o sim
./sim
