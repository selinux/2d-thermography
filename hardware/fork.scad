
difference(){
	rotate_extrude(convexity=10, $fn = 100){
		import (file = "2d-sections/fork0.dxf");
	}

	translate([0,50,0]){
		rotate(90, [1,0,0]){
			linear_extrude(h=100){
				import (file = "2d-sections/fork1.dxf");
			}
		}
	}

	translate([0,0,-20]){
		cylinder(r=2.5, h=25, $fn=100);
	}
}

