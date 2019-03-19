set terminal wxt

plot 'results_euler_0.05.txt' u 2:5 w lines,\
	'results_seuler_0.05.txt' u 2:5 w lines,\
	'results_leapfrog_0.05.txt' u 2:5 w lines,\
	'results_rk4_0.05.txt' u 2:5 w lines
