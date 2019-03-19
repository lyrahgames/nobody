set terminal epslatex size 12cm,6cm
set output "sun_earth_2_zoom.tex"

set xl '$t\ [\mathrm{a}]$'
set yl '$E$'

set xr [-1:65]
set yr [-19.7:-19.686]
set key bottom left spacing 1.3 width -8
set ytics 0.002
set format y '$%.3f$'

plot 'results_leapfrog_0.02.txt' u 2:5 w lines lw 3 title 'Leapfrog-Integrator',\
  'results_rk4_0.02.txt' u 2:5 w lines lw 3 title 'RK4-Integrator'
