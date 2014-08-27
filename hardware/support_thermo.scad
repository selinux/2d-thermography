//support();


module support(){

    a = 14;
    a1 = 10;
    b = 21;
    c = 30+a1;
    d = 35+5;
    difference(){
        union(){
        
            linear_extrude(height = a)
                import("2d-sections/support_therm1.dxf");
            
            translate([0,0,a])
                linear_extrude(height = b-a1)
                    import("2d-sections/support_therm2.dxf");
//            translate([0,0,a+b-a1])
//                linear_extrude(height = a1)
//                    import("2d-sections/support_therm20.dxf");
            
            translate([0,0,a+b-a1])
                linear_extrude(height = c)
                    import("2d-sections/support_therm3.dxf");
        }
    
    
        translate([0,0,-5])
            linear_extrude(height = d)
                import("2d-sections/support_therm10.dxf");
    
        translate([0,6,-5])
            cylinder(d=8.3, h = 100);
        translate([0,6,-5])
            cylinder(d=9, h = 55+5);
    }
}


