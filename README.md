<img width="567" height="360" alt="Screenshot 2026-09-20 at 11 49 39" src="https://github.com/user-attachments/assets/adf8bdbe-7a26-4a3e-b420-52c284e9585e" />
<img width="1920" height="1280" alt="image_20260920_114626" src="https://github.com/user-attachments/assets/97d99020-7fca-46d9-bda8-f8b44796b088" />
"Below image is a schematic representation (artistic render) of V(φ) = 1/2 m² φ². Raw output is the terminal screenshot above."

# Cosmic Inflation Simulation

A lightweight, dependency-free C++ simulation that models the dynamics of early-universe cosmic inflation and reheating using an ASCII-based terminal GUI.

The engine numerically integrates the coupled inflaton field equations and the Friedmann expansion rate using a 4th-order Runge-Kutta (RK4) integrator, rendering the scalar field's motion within its potential well in real time.

---

## Theoretical Background

The simulation models standard chaotic inflation driven by a single minimally coupled scalar field $\phi$ (the inflaton) rolling down a quadratic potential:

$$V(\phi) = \frac{1}{2}m^2\phi^2$$

### Equations of Motion

1. **Inflaton Dynamics (Klein-Gordon Equation):**
   $$\ddot{\phi} + 3H\dot{\phi} + V'(\phi) = 0$$
   where $3H\dot{\phi}$ acts as dynamical friction ("Hubble friction") caused by the expanding spacetime, and $V'(\phi) = m^2\phi$.

2. **Cosmic Expansion (First Friedmann Equation):**
   $$H^2 = \frac{8\pi G}{3}\rho = \frac{1}{3 M_{\text{pl}}^2}\rho$$
   The total energy density $\rho$ consists of kinetic and potential components:
   $$\rho = \frac{1}{2}\dot{\phi}^2 + V(\phi)$$

### Evolutionary Phases

* **Slow-Roll Phase ($\epsilon, \eta \ll 1$):** When potential energy dominates ($\frac{1}{2}\dot{\phi}^2 \ll V(\phi)$), the universe expands quasi-exponentially ($a(t) \sim e^{Ht}$), generating roughly $60$ or more $e$-folds ($N$).
* **Reheating & Oscillation Phase:** Once $\phi$ drops below the Planck scale, the field decelerates, slow-roll breaks down, and the inflaton rapidly oscillates around the minimum of the potential ($V(\phi=0) = 0$), depositing energy into standard-model particles.

---

## Key Technical Features

* **RK4 Numerical Integrator:** Uses a 4th-order Runge-Kutta integration scheme with intermediate slopes ($k_1, k_2, k_3, k_4$) to handle the stiff transitions between slow-roll and rapid coherent oscillations.
* **Flicker-Free Terminal Rendering:** Employs direct ANSI escape sequences (`\033[H` for home-cursor repositioning and `\033[K` for row clearing) instead of destructive full-screen clears (`\033[2J`), enabling smooth frame rendering at ~40 FPS.
* **Bounded ASCII Rasterization:** Clamps spatial coordinates ($x \in [0, \text{width}]$) to prevent line-wrapping artifacts and buffer issues during negative-amplitude field oscillations.
* **Zero External Dependencies:** Built entirely on the C++17 Standard Library (`<iostream>`, `<chrono>`, `<thread>`, `<cmath>`, `<iomanip>`).

---

## Build and Run

### Prerequisites

* A C++17 compliant compiler (`g++`, `clang++`, or MSVC)
* A terminal emulator supporting ANSI escape codes with a recommended minimum geometry of **80x25**

### Compilation

Compile with `-O3` optimization for smooth timing execution:

```bash
g++ -O3 -std=c++17 cosmicinflation.cpp -o sim
./sim

==================================================
        COSMIC INFLATION & REHEATING SIMULATION   
==================================================
 Phase : [ SLOW-ROLL INFLATION ]
 e-folds (N) : [####################------]  54.12 / 65.00
 Field Value :  3.2145 M_pl
 Hubble (H)  :  0.0014 M_pl

 Potential Well V(phi) = 1/2 m^2 phi^2:
  |                      .
  |                    .
  |                  .
  |                .
  |              (O)
  |            .
  |          .
  |        .
  +------------------------
 0.0                      15.0 M_pl
