include <fork.scad>
include <support_thermo.scad>


fork();

angle = 0;
high = 35*sin(angle) + 15;
translate([0,40,high])
    rotate([90+angle,0,0])
        support();
