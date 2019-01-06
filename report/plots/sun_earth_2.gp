set terminal epslatex size 12cm,6cm
set output "sun_earth_2.tex"

set xl '$t\ [\mathrm{a}]$'
set yl '$E$'

set xr [-10:200]
set key center left spacing 1.3
set ytics 5

plot 'results_euler_0.02.txt' u 2:5 w lines lw 3 title 'Expliziter Euler-Integrator',\
  'results_seuler_0.02.txt' u 2:5 w lines lw 3 title 'Symplektischer Euler-Integrator',\
  'results_leapfrog_0.02.txt' u 2:5 w lines lw 3 title 'Leapfrog-Integrator',\
  'results_rk4_0.02.txt' u 2:5 w lines lw 3 title 'RK4-Integrator'
