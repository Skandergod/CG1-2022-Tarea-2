#version 330 core

// in vec2 fragPos;
// uniform sampler2D tex;
// uniform int mode = 1;
// uniform float imgWidth;
// uniform float imgHeight;
// uniform float test;
// 
// 
uniform vec3 color;
out vec4 fragColor;



void main(){



	fragColor = vec4(color,1.0f);


//	vec2 actPos = (fragPos.xy+1)/2.f;
//// actPos = fragColor.xy;
//
//	
//	actPos.y = 1-actPos.y;
//	vec3 texColor = texture(tex, actPos).rgb;
//	vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);
//	int mode = 1;
//
//	float g;
//	if(mode == 0){
//		texColor =texColor ;
//	}else if(mode == 1){
//		vec3 grey = vec3(0.2125f, 0.7154f, 0.0721f);
//		g = dot(texColor, grey);
//		texColor = vec3(g);
//	}
//	else if (mode == 2){
//		vec3 avg = vec3(0);
//
//
//		vec2 uAcum = vec2(0);
//		int width = 3;
//		int height = 3;
//		 
//		float conv[9] = float[](
//			0.1f, 0.1f, 0.1f,
//			0.1f, 0.1f, 0.1f,
//			0.1f, 0.1f, 0.1f
//		);
//
////		float conv[9] = float[](
////			-1.f, 0, 1.f,
////			-1.f, 0, 1.f,
////			-1.f, 0, 1.f
////		);
//
//		int convI = 0;
//		for(int yy = 0; yy < height; yy++, uAcum.y += d.y){
//			for(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){
//				vec2 nUv = actPos + uAcum;
//				avg+= texture(tex, nUv).rgb * conv[convI];
//			}
//		}
////		avg = texture(tex, actPos ).rgb;
//		texColor = avg;
//	}
//
//	if(g < 0.6f){
//		texColor.rgb = vec3(0.f);
//	}
//
//	
//	fragColor = vec4(texColor,1);
//	// fragColor=vec4(test, 0,0,1);
//	// fragColor = vec4(1,0,0,1);
}																						    