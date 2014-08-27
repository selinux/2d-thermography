include <fork.scad>
include <support_thermo.scad>


fork();

translate([0,40,15])
    rotate([90,0,0])
        support();
