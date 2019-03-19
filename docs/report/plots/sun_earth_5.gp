set terminal epslatex size 12cm,6cm
set output "sun_earth_5.tex"

set xl '$t\ [\mathrm{a}]$'
set yl '$E$'

set xr [-10:250]
set yr [-150:80]
set key bottom left spacing 1.3 width -12
set ytics 50

plot 'results_euler_0.05.txt' u 2:5 w lines lw 3 title 'Expliziter Euler-Integrator',\
  'results_seuler_0.05.txt' u 2:5 w lines lw 3 title 'Symplektischer Euler-Integrator',\
  'results_leapfrog_0.05.txt' u 2:5 w lines lw 3 title 'Leapfrog-Integrator',\
  'results_rk4_0.05.txt' u 2:5 w lines lw 3 title 'RK4-Integrator'
