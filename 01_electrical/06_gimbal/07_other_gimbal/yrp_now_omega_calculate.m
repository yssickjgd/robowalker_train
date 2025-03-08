% other gimbal for yaw-roll-pitch

syms ywx ywy ywz rwx rwy rwz pwx pwy pwz cwx cwy cwz alpha beta theta ca sa cb sb ct st;

eq1 = ywx == ca * cwx + sa * cwy;
eq2 = ywy == ca * cwy - sa * cwx;
eq3 = ywz == cwz + alpha;
eq4 = rwx == ywx + theta;
eq5 = rwy == ct * ywy + st * ywz;
eq6 = rwz == ct * ywz - st * ywy;
eq7 = pwx == cb * rwx - sb * rwy;
eq8 = pwy == rwy + beta;
eq9 = pwz == sb * rwx + cb * rwy;

S = solve(eq1, eq2, eq3, eq4, eq5, eq6, eq7, eq8, eq9, ywx, ywy, ywz, rwx, rwy, rwz, alpha, beta, theta)
display(S.alpha);
display(S.beta);
display(S.theta);