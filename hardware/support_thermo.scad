//support();


module support(){

    a = 14;
    a1 = 10;
    b = 21;
    c = 30;
    d = 35+5;

    difference(){
        union(){
        
            difference(){
            linear_extrude(height = a)
                import("2d-sections/support_therm1.dxf");
            // round base (impossible to print)
//            difference(){
//                translate([0,0,4])
//                    cube([40,40,10], center=true);
//                translate([0,0,35])
//                    rotate([0,90,0])
//                        cylinder(r=35,h=100, center=true);
//            }
            }
            
            translate([0,0,a])
                linear_extrude(height = b-a1, scale=[0.6,0.7])
                    import("2d-sections/support_therm2.dxf");
//            translate([0,0,a+b-a1])
//                linear_extrude(height = a1)
//                    import("2d-sections/support_therm20.dxf");
            
            translate([0,0,a])
                linear_extrude(height = c+b)
                    import("2d-sections/support_therm3.dxf");
        }
    
    
        translate([0,0,-5])
            linear_extrude(height = d)
                import("2d-sections/support_therm10.dxf");
    
        translate([0,6,-5])
            cylinder(d=8.3, h = 100);
        translate([0,6,-5])
            cylinder(d=9, h = 55+5);
        translate([0,-6.5,-5])
            cylinder(d=8, h = 55+5);


    }
}


