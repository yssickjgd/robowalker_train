syms a b t

a = pi/2.;
b = -pi/6.;
t = pi/6.;

rz = [
    cos(a) -sin(a) 0;
    sin(a) cos(a) 0;
    0 0 1;
    ];

ry = [
    cos(b) 0 sin(b);
    0 1 0;
    -sin(b) 0 cos(b);
    ];

rx = [
    1 0 0;
    0 cos(t) -sin(t);
    0 sin(t) cos(t);
    ];

rz*ry*rx