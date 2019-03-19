set terminal epslatex size 12cm,6cm
set output "diagram.tex"

set xl '$t\ [\mathrm{a}]$'
set yl '$E$'

set xr [-10:200]
set key center left

plot 'results_euler_0.02.txt' u 2:5 w lines title 'Expliziter Euler-Integrator',\
  'results_seuler_0.02.txt' u 2:5 w lines title 'Symplektischer Euler-Integrator',\
  'results_leapfrog_0.02.txt' u 2:5 w lines title 'Leapfrog-Integrator',\
  'results_rk4_0.02.txt' u 2:5 w lines title 'RK4-Integrator'
