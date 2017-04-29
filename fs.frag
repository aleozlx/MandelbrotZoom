#version 330
#extension GL_ARB_gpu_shader_fp64 : enable
out vec3 color;

uniform int WINDOW_SIZE_X;
uniform int WINDOW_SIZE_Y;
uniform double zoomfactor;
uniform double centerx;
uniform double centery;
uniform int useriterations;
double stepx = 4.0 / WINDOW_SIZE_X;
double stepy = 4.0 / WINDOW_SIZE_Y;
const int MaxIterations = 500;


//(x+iy)*(x+iy) = x^2+xiy+xiy+y^2i^2 = x^2+2xyi-y^2 => RE: x^2-y^2 IM: 2xy

void main()
{
	dvec2 z, c;
	z.x = 0;
	z.y = 0;

	c.x = (gl_FragCoord.x * stepx-2.0)*zoomfactor+centerx;
	c.y = (gl_FragCoord.y * stepy-2.0)*zoomfactor+centery;

	int i;
	for(i = 0; i < useriterations; i++)
	{
		double x = z.x*z.x - z.y*z.y + c.x;
		double y = 2*(z.x*z.y) + c.y;
		z.x = x;
		z.y = y;

		if(((z.x*z.x+z.y*z.y) >= 4))
		{
			break;
		}
	}

	if(i == useriterations)
	{
		color = vec3(0,0,0);
	}
	else
	{
		color = vec3(0,0,(i/255.0f)*(255.0f/useriterations));
	}
}