#include <cmath>
#include <iostream>
#include <windows.h>

//*********************************************************************************************************************
// 										Basic constants
//*********************************************************************************************************************
const int res_X=132; //resolution x
const int res_Y=60; //resolution y; right-click on the window title, select properties and set the proper size
const int fov=600; //field of view, in 0.1 degree increments (60 degrees)

//*********************************************************************************************************************
// 										Math, lokup tables
//*********************************************************************************************************************
const double torad=M_PI/180; //degrees to radians conversion factor
const double todeg=180/M_PI; //radians to degrees conversion factor
double sintab[3600]; //lookup table of sine values, every 0.1 degree
double fisheye[res_X]; //lookup table for fisheye correction, 1 value for every screen column

//*********************************************************************************************************************
// 										Input\output & system stuff
//*********************************************************************************************************************
POINT point0; //mouse starting point
const int mouse_speed=100; //mouse speed division
LARGE_INTEGER nStartTime,nStopTime,nElapsed,nFrequency; //for computing fps
int fps=0;
int debug[16]; //various flags/values for testing stuff

//*********************************************************************************************************************
// 										Graphics buffers for drawing
//*********************************************************************************************************************
char char_buff[res_X*res_Y]; //screen character buffer
WORD color_buff[res_X*res_Y]; //screen color buffer
char char_grad[93]=" `.-':_,~=;><*+!rc/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@"; //character gradient used to denote brightness
const int grad_length=90; //must be 2 smaller than above
int hmap[res_X]; //map of heights of wall columns
double lmap[res_X]; //map of light/brightness
int nmap[res_X]; //map of wall normals
int tmap[res_X]; //map of texture coordinates of wall columns
int typemap[res_X]; //wall type (texture number)
double wallxmap[res_X];//x,y grid coordinates of wall column
double wallymap[res_X]; 
double walldmap[res_X]; //distance to wall slice
double depth_map[res_X*res_Y]; //screen sized depth map to determine when to draw sprites

//*********************************************************************************************************************
// 										Textures, graphics
//*********************************************************************************************************************
int textures[32*32*16]; //1-D array containing 16 32x32 textures (1024 pixels); 
//first byte is character, second one is color, 3rd is surface normal, 4th unused yet

int sky[res_X*2*res_Y]; //sky texture
int sky_color;

WORD pal[16][3]; //each of the 16 console colors has 3 brightness variants (like red->light red->yellow)
const int pal_thr1=30; //threshold for 1st palette switch
const int pal_thr2=85; //threshold for 2nd palette switch

int sprites[32*32*16];
//first byte is character, second one is color, 3rd is alpha
//*********************************************************************************************************************
// 										World and player state
//*********************************************************************************************************************

//World map
const int map_size=58; //square map size
int map[map_size][map_size]; //world map

//Structure holding player data
struct{
	double x,y,z; //coordinates
	double vx,vy,vz; //velocities
	double ang_h,ang_v; //horizontal and vertical angle
	double friction=0.1; //friction coefficient for movement; set to 0.01 and go ice skating :)
	double accel=0.01; //acceleration coefficient for movement
	double grav=0.01; //gravity
	double jump_h=0.18; //jump height (initial velocity)
} player;

struct{
	int enabled; //flag if enemy is on the map
	int type; //sprite number
	double x,y,z; //coordinates
	double vx,vy,vz; //velocities
	double friction=0.1; //friction coefficient for movement; 
	double accel=0.01; //acceleration coefficient for movement
	double grav=0.01; //gravity	
} enemies[16];

int horizon_pos=0; //position of the horizon, in characters; 0=middle of the screen

//Global flags
int F_exit=0; //turns to 1 when player presses esc.
double key_delay; //for toggle on/off keys, to avoid toggling things 100 times per second

//*********************************************************************************************************************
// 										Light
//*********************************************************************************************************************
double light_global; //global brightness
double light_faloff; //distance brightness faloff 
double light_aperture; //eye/camera aperture; dynamically reacts to scene brightness
double light_flashlight; //flashlight type source
double flashlight_coeff[res_X*res_Y]; //pre-computed brightness map (faloff from screen center) 
double sky_light; //amount of light from open sky

double lightmap[map_size*16][map_size*16]; //brightness map, every square divided into 16x16 sub-squares
double light_tmp[map_size*16][map_size*16]; //secondary helper buffer for lightmap
double static_lights[64][4];//64 lights; x,y,strength,height; for calculating lightmap

//*********************************************************************************************************************
// 										Various helper functions
//*********************************************************************************************************************

//checks if there is a straight line connection between 2 points; useful for casting light rays
int checkray(double x1, double y1, double x2, double y2, int steps) 
{
double dx=(x2-x1)/(1.0*steps);
double dy=(y2-y1)/(1.0*steps);
int k=1;
int mcx,mcy;

for(int i=0;i<steps;i++)
{
x1=x1+dx;
y1=y1+dy;
mcx=(int)x1;
mcy=(int)y1;
if((mcx>0)&&(mcy>0)&&(mcx<map_size)&&(mcy<map_size))
if ((map[mcx][mcy]%256)>0){k=0;break;}
}
return k;
}

//*********************************************************************************************************************

void clear_buffers()
{
for(int x=0;x<res_X;x++)
for(int y=0;y<res_Y;y++)
 {
 char_buff[x+y*res_X]=' ';
 color_buff[x+y*res_X]=0;
 }	
}

//*********************************************************************************************************************

void show_map() //just show the map on the screen, good for debugging map generator
{	
for(int x=0;x<map_size;x++)
for(int y=0;y<map_size;y++)
 {
 char_buff[x+y*res_X]='#';
 color_buff[x+y*res_X]=4*((map[x][y]%256)>0);
 }
}



//*********************************************************************************************************************
// 										Procedural texture generation
//*********************************************************************************************************************

void gen_texture(int number, int type, int p1, int p2, int p3, int p4, int p5, int p6) //generate texture at given number; parameters p1-p4 depend on type 
{
	
if(type==0) //brick texture: p1=brick brightness, p2=mortar brightness, p3=brick height, p4=brick row offset, p5=brick color, p6=mortar color
	{
	for(int x=0;x<32;x++)//texture generation
	for(int y=0;y<32;y++)
		{
		textures[x+y*32+1024*number]=(p1-p2*((y%p3==0)||((x+p4*(y/p3))%16==0))+rand()%2)+(p5+(p6-p5)*((y%p3==0)||((x+p4*(y/p3))%16==0)))*256;
		textures[x+y*32+1024*number]+=65536*(128+64*((y%p3==0)||((x+p4*(y/p3)-1)%16==0))-64*((y%p3==0)||((x+p4*(y/p3)+1)%16==0))+rand()%32-16); //surface normal - bricks+some roughness
		}
	}

if(type==1) //large, monocolored bricks/plates; p1=brick brightness, p2=mortar brightness, p3=color
	{
	for(int x=0;x<32;x++)//texture generation
	for(int y=0;y<32;y++)
		{
		textures[x+y*32+1024*number]=p1-p2*((y%31==0)||((x+4*(y/31))%16==0))+rand()%2+p3*256;
		textures[x+y*32+1024*number]+=65536*(128+rand()%32-16); //surface normal - random roughness
		}
	}
	
}

//*********************************************************************************************************************
// 										Procedural sprite generation
//*********************************************************************************************************************
void gen_pacman_ghost(int number, int brightness, int color)
{
	for(int x=0;x<32;x++)
	for(int y=0;y<32;y++)
	{
	double dist=(x-16)*(x-16)+(y-16)*(y-16);
	if((dist<225)||((y>16)&&(x>1)&&(x<31)&&(y<32-x%4)))sprites[x+y*32+1024*number]=brightness+256*color+65536;
	dist=(x-11)*(x-11)+(y-12)*(y-12);
	if(dist<16)sprites[x+y*32+1024*number]=brightness+256*15+65536;
	if(dist<2)sprites[x+y*32+1024*number]=brightness+256*0+65536;
	dist=(x-21)*(x-21)+(y-12)*(y-12);
	if(dist<16)sprites[x+y*32+1024*number]=brightness+256*15+65536;
	if(dist<2)sprites[x+y*32+1024*number]=brightness+256*0+65536;
	}
}

//*********************************************************************************************************************
// 										Procedural sky
//*********************************************************************************************************************
void gen_sky(int brightness)
{
int sky_randoms[res_X*2*res_Y];//helper buffer

for(int x=0;x<res_X*2;x++)
for(int y=0;y<res_Y;y++)
sky_randoms[x+y*2*res_X]=rand()%brightness;	//generate map of random values

for(int iter=1;iter<8;iter++) //7 iterations of Perlin noise	
for(int x=0;x<res_X*2;x++)
for(int y=0;y<res_Y;y++)
	{
	sky[x+y*2*res_X]+=sky_randoms[x/iter+y/iter*2*res_X];
	}		
}

//*********************************************************************************************************************
// 										Map generation
//*********************************************************************************************************************

void gen_map()
{
for(int x=0;x<map_size;x++)
for(int y=0;y<map_size;y++)map[x][y]=1+256*1+65536*1; //fill map with wall/floor/ceiling type 1

int px,py,dx,dy;
int num_light,num_enemy;
int dir;
int directions[4]={-1,0,1,0};
//start at the middle
px=map_size/2; 
py=map_size/2; 
player.x=px+0.5; //0.5 puts the player in the middle of the floor block
player.y=py+0.5;

static_lights[0][0]=px+0.5; //test light
static_lights[0][1]=py+0.5;
static_lights[0][2]=20; 

for(int i=0;i<(map_size*map_size);i++) //number of random steps = 1/2 of map area
{
map[px][py]=0+256*1+65536*3; //no wall, floor 1, ceiling 3; note "+1" - cordinates px,py are always > 0

if((rand()%4==0))dir=rand()%4; //change direction randomly
dx=directions[dir%4];
dy=directions[(dir+1)%4];	

px+=dx; if(px<2)px=2;if(px>map_size-3)px=map_size-3;//update coordinates; thick map border to be rally sure that nothing goes past map array
py+=dy; if(py<2)py=2;if(py>map_size-3)py=map_size-3;

if((rand()%64==0)&&(num_light<16)) //random lights
{
num_light++;
static_lights[num_light][0]=px+0.5; //test light
static_lights[num_light][1]=py+0.5;
static_lights[num_light][2]=20; 
}


if((rand()%64==0)&&(num_enemy<16)) //random enemies
{
num_enemy++;
enemies[num_enemy].x=px+0.5;
enemies[num_enemy].y=py+0.5;
enemies[num_enemy].enabled=1;
enemies[num_enemy].type=0;
enemies[num_enemy].z=32;
}

}
//set half of the map as sky
for(int px=0;px<map_size/2;px++)
for(int py=0;py<map_size;py++)
map[px][py]=map[px][py]%256+256*4; //erase bytes 2-3

}


//*********************************************************************************************************************
// 										Initialization
//*********************************************************************************************************************

void set_palette()
{
pal[0][0]=0;pal[0][1]=8;pal[0][2]=8; //black->dark gray
pal[1][0]=1;pal[1][1]=9;pal[1][2]=11; //dark blue -> light blue -> cyan
pal[2][0]=2;pal[2][1]=10;pal[2][2]=6; //dark green -> light green -> yellow
pal[3][0]=3;pal[3][1]=11;pal[3][2]=15; ///...and so on
pal[4][0]=4;pal[4][1]=12;pal[4][2]=14;
pal[5][0]=5;pal[5][1]=13;pal[5][2]=12;
pal[6][0]=6;pal[6][1]=14;pal[6][2]=15;
pal[7][0]=7;pal[7][1]=15;pal[7][2]=15;
pal[8][0]=8;pal[8][1]=7;pal[8][2]=15;
pal[9][0]=9;pal[9][1]=11;pal[9][2]=15;
pal[10][0]=10;pal[10][1]=6;pal[10][2]=15;
pal[11][0]=11;pal[11][1]=15;pal[11][2]=15;
pal[12][0]=12;pal[12][1]=14;pal[12][2]=15;
pal[13][0]=13;pal[13][1]=12;pal[13][2]=15;
pal[14][0]=14;pal[14][1]=15;pal[14][2]=15;
pal[15][0]=15;pal[15][1]=15;pal[15][2]=15;
}

//**********************************************************************************************************************

void calculate_lights()
{
double cx,cy; //current coordinates
int k;

for(int i=0;i<64;i++) //go through all lights
for(int x=1;x<map_size*16;x++)//go through whole lightmap, x coord.
if(fabs(x/16-static_lights[i][0])<12) //light closer than 12 squares?
for(int y=1;y<map_size*16;y++)//go through whole lightmap, y coord.
if(fabs(y/16-static_lights[i][1])<12) //light closer than 12 squares?
{
cx=1.0/16.0*x; //map coordinate x
cy=1.0/16.0*y; //map coordinate y

double dst=(cx-static_lights[i][0])*(cx-static_lights[i][0])+(cy-static_lights[i][1])*(cy-static_lights[i][1]); //distance to light
if(dst<144)k=checkray(cx,cy,static_lights[i][0],static_lights[i][1],256);else k=0; //check if there is unobstructed line to the light
lightmap[x][y]+=1.0*k*static_lights[i][2]/sqrt(dst); //update lightmap
}

for(int x=1;x<map_size*16-1;x++) //apply sky
for(int y=1;y<map_size*16-1;y++)
if ((map[x/16][y/16]/65536)==0) //sky tile?
lightmap[x][y]+=sky_light;

for(int x=1;x<map_size*16-1;x++)
for(int y=1;y<map_size*16-1;y++)
light_tmp[x][y]=0.2*(lightmap[x][y]+lightmap[x+1][y]+lightmap[x-1][y]+lightmap[x][y+1]+lightmap[x][y-1]); //simple blur - average of neighbors

for(int x=1;x<map_size*16-1;x++)
for(int y=1;y<map_size*16-1;y++)
lightmap[x][y]=light_tmp[x][y]; //save the effect of blur


//flashlight brightness map
for (int x=0; x<res_X;x++)
for (int y=0; y<res_Y;y++)
{
double lghtx=5.0*(x-res_X/2)/res_X; //horizontal faloff
double lghty=5.0*(y-res_Y/2)/res_Y; //vertical faloff
double lght=exp(-lghtx*lghtx)*exp(-lghty*lghty); //full faloff coefficient
flashlight_coeff[x+y*res_X]=512.0*lght*(1+0.2*((abs(y)%2)+(abs(x)%2))); //final calc + dithering
}

}

//**********************************************************************************************************************
void init() //initialization, precalculation
{
//precalculate sine values. Important, we add 0.001 to avoid even angles where sin=0 or cos=0
for(int i=0;i<3600;i++)sintab[i]=sin((i+0.001)*0.1*torad); 

//fisheye correction term (cosine of ray angle relative to the screen center, e.g. res_X/2)
for(int i=0;i<res_X;i++)fisheye[i]=cos(0.1*(i-res_X/2)*torad*fov/res_X); 

//Texture generation - one could use randomized parameters for more variety
gen_texture(0,0,12,8,6,4,4,8); //red brick
gen_texture(1,1,8,4,8,0,0,0); //gray plates
gen_texture(2,1,14,4,8,0,0,0); //gray plates, brighter
gen_texture(3,1,8,6,5,0,0,0); //magenta plates
gen_texture(4,1,12,0,2,0,0,0); //green grass

GetCursorPos(&point0); //record starting point of the mouse

//Light settings
light_global=0.05;
light_faloff=1.0;
light_flashlight=1;
sky_light=40;
sky_color=1;
}


//*********************************************************************************************************************
// 										Ray Casting
//*********************************************************************************************************************

void cast()//main ray casting function
{
//some speedup might be possible by declaring all variables beforehand here instead of inside the loops

for (int xs=0;xs<res_X;xs++)//go through all screen columns
	{
	//ray angle = player angle +-half of FoV at screen edges; 
	//add 360 degrees to avoid negative values when using lookup table later
	int r_angle=(int)(3600+player.ang_h+(xs-res_X/2)*fov/res_X);

	//ray has a velocity of 1. Now we calculate its horizontal and vertical components; 
	//horizontal uses cosine (e.g. sin(a+90 degrees))
	//use %3600 to wrap the angles to 0-360 degree range
	double r_vx=sintab[(r_angle+900)%3600];
	//we will ned an integer step to navigate the map; +1/-1 depending on sign of r_vx
	int r_ivx=(r_vx>0) ? 1 : -1;

	//now the same for vertical components
	double r_vy=sintab[r_angle%3600];
	int r_ivy=(r_vy>0) ? 1 : -1;

	//initial position of the ray; precise and integer values
	//ray starts from player position; tracing is done on doubles (x,y), map checks on integers(ix,iy)
	double r_x=player.x;
	double r_y=player.y;
	int r_ix=(int)r_x;
	int r_iy=(int)r_y;
	double r_dist=0; //travelled distance
	double t1,t2; //time to intersect next vertical/horizontal grid line;

		//ray tracing; we check only intersections with horizontal/vertical grid lines, so maximum of 2*map_size is possible
		for(int i=0;i<2*(map_size-1);i++)
		{
		if((map[r_ix][r_iy]%256)>0)break;//map>0 is a wall; hit a wall? end tracing

		//calculate time to intersect next vertical grid line; 
		//distance to travel is the difference between double and int coordinate, +1 if moving to the right 
		//example: x=0.3, map x=0, moving to the right, next grid is x=1 and distance is 1-0.3=0.7
		//to get time, divide the distance by speed in that direction 
		t1=(r_ix-r_x+(r_vx>0))/r_vx; 
		//the same for horizontal lines
		t2=(r_iy-r_y+(r_vy>0))/r_vy; 

		//now we select the lower of two times, e.g. the closest intersection
		if(t1<t2)
			{//intersection with vertical line
			r_y+=r_vy*t1; //update y position
			r_ix+=r_ivx; //update x map position by +-1
			r_x=r_ix-(r_vx<0)*r_ivx; //we are on vertical line -> x coordinate = integer coordinate
			r_dist+=t1; //increment distance by velocity (=1) * time
			}
			else
			{//intersection with horizontal line
			r_x+=r_vx*t2;
			r_iy+=r_ivy;
			r_y=r_iy-(r_vy<0)*r_ivy;
			r_dist+=t2;
			}
		}
		//end of tracing; the distance is updated during steps, so there is no need to calculate it
		
		hmap[xs]=(int)(res_Y/2/r_dist/fisheye[xs]);//record wall height (~1/distance) apply fisheye correction term
		typemap[xs]=map[r_ix][r_iy]%256-1;//record the wall type; subtract 1 so map[x][y]=1 means wall type 0
		tmap[xs]=(t1<t2) ? 32*fabs(r_y-(int)(r_y)) : 32*fabs(r_x-(int)(r_x)); //record the texture coordinate (fractional part of x/y coordinate * texture size)
		lmap[xs]=(t1<t2) ? fabs(r_vx) : fabs(r_vy);//lighting based on ray normal
		lmap[xs]*=15.0*light_global*(light_faloff*hmap[xs]/res_Y+1-light_faloff);//calculate brightness; it is proportional to height, 15.0 is arbitrary constant
		nmap[xs]=(t1<t2) ? 1 : 0;//record wall normal - good thing we have only 90 degree walls :)
		wallxmap[xs]=r_x; //record final ray position
		wallymap[xs]=r_y;
		walldmap[xs]=r_dist;
	}
}


//*********************************************************************************************************************
// 										Drawing functions
//*********************************************************************************************************************

void draw()
{
//go through the screen, column by column
for (int x=0;x<res_X;x++)
	{
	int plusy=(int)(-player.z*(hmap[x]+1)); //player vertical pos modifier
	//upper limit of the wall, capped at half vertical resolution (middle of the screen=0)
	int lm1=-((hmap[x]+horizon_pos+plusy)>res_Y/2 ? res_Y/2 : (hmap[x]+horizon_pos+plusy)); 
	//lower limit of the wall, capped at -half vertical resolution (middle of the screen=0)
	int lm2=((hmap[x]-horizon_pos-plusy+1)>res_Y/2 ? res_Y/2 : (hmap[x]-horizon_pos-plusy+1)); 

	//array offset for putting characters
	int offset=x; //we draw on the column x
	double character; //the number of the character from gradient to draw
	int color; //the color of the character to draw
	double normal; //texture normal
		
	int r_angle=(int)(3600+player.ang_h+(x-res_X/2)*fov/res_X);//ray angle, needed for normal maps
	double r_vx=sintab[(r_angle+900)%3600];//ray step x
	double r_vy=sintab[r_angle%3600];//ray step y

	for (int y=-res_Y/2;y<res_Y/2;y++) //go along the whole screen column, drawing either wall or floor/ceiling
		{
		int ang=(int)(3600+player.ang_h+(x-res_X/2)*fov/res_X);//calculate ray angle; needed for floor
		double dx=sintab[(ang+900)%3600]; //steps in x and y direction, the same as in tracing, needed for floor
		double dy=sintab[ang%3600];
		character=0;color=0; //defaults
		if(y>=lm1&&y<=lm2&&hmap[x]>0) //are we drawing a wall?
		{
		int crdx=tmap[x];//we get texture x coordinate from coordinate buffer made in tracing step 
		int crdy=16+((int)(14*(y+horizon_pos+plusy)/hmap[x]))%16;//texture y coordinate depends on y, horizon position and height
		int crd=crdx+32*crdy+1024*typemap[x];//calculate coordinate to use in 1-d texture buffer
		character=textures[crd]%256;//get texture pixel (1st byte)
		color=(textures[crd]/256)%256;//get texture color (2nd byte)
		normal=1.0/128*((textures[crd]/65536)%256-128);//get texture normal (3rd byte)
		character+=((abs(y)%2)+(abs(x)%2));//add dithering to avoid ugly edges
		character=character*lmap[x]; //multiply by the brightness value of 1-d light map
		character+=lightmap[(int)(16*wallxmap[x])][(int)(16*wallymap[x])]; //apply 2-D lightmap
		character+=light_flashlight*flashlight_coeff[x+(y+res_Y/2)*res_X]*hmap[x]/res_Y*lmap[x]; //flashlight
		character*=(nmap[x]*(fabs(r_vx)+r_vy*normal)+(1-nmap[x])*(fabs(r_vy)+r_vx*normal));//apply texture normals
		}
		else //floor/ceiling?
		{
		double plusy2;
		(y+horizon_pos>0) ? plusy2=32.0*player.z : plusy2=-32.0*player.z; //player height modif.
		//calculate distance to the floor pixel; y and horizon_pos are in pixels, 0.1 is added here to avoid division by 0
		double dz=(res_Y/2+plusy2)/(fabs(y+horizon_pos)+0.0)/fisheye[x]; 
		if((dz<16)&&(dz>0)) //ignore extremely far things
		{
		int crdx=(int)(1024+32.0*(player.x+dx*dz))%32; //floor/ceiling texture coordinates
		int crdy=(int)(1024+32.0*(player.y+dy*dz))%32; //1024 is here just to avoid negative numbers
		int mcx=(int)(player.x+dx*dz)%map_size; //floor/ceiling map coordinates
		int mcy=(int)(player.y+dy*dz)%map_size;
		int crd=crdx+32*crdy; //base texture coordinate
		if(y>(-horizon_pos))crd+=1024*((map[mcx][mcy]/256)%256); //2nd byte = floor type
		else crd+=1024*((map[mcx][mcy]/65536)%256); //3rd byte = ceiling type
		
		if(((map[mcx][mcy]/65536)>0)||(y>(-horizon_pos))) //ground or non-sky?
			{
			character=textures[crd]%256;//get texture pixel (1st byte)
			color=(textures[crd]/256)%256;//get texture color (2nd byte)
			character+=((abs(y)%2)+(abs(x)%2));//add dithering
			character*=0.2*light_global*(light_faloff*abs(y+horizon_pos)/(dz+1)+1-light_faloff);//distance-based gradient
			character+=lightmap[(int)(16*(player.x+dx*dz))][(int)(16*(player.y+dy*dz))]; //apply 2-D lightmap
			character+=0.2*(light_flashlight*flashlight_coeff[x+(y+res_Y/2)*res_X]/(dz+2)); //flashlight
			}
		else
			{
			character=sky[(res_X*2*res_Y+x/8+(int)(r_angle/8)+(y+res_Y/2+horizon_pos)*2*res_X)%(res_X*res_Y)];
			character+=((abs(y)%2)+(abs(x)%2));//add dithering 
			color=sky_color;
			}
		}
		}		
		//limit the value to the limits of character gradient (especially important if there are multiple brightness modifiers)
		if(character>grad_length)character=grad_length;
		if((character<0)||std::isnan(character))character=0; 
		char_buff[offset]=char_grad[(int)character];//save the character in character buffer
		color_buff[offset]=pal[color][(character>30)+(character>85)];//save the color in color buffer
		depth_map[offset]=walldmap[x]; //record depth map
		offset+=res_X; //go down by 1 row
		}//end of column
	}//end of drawing
}

//*********************************************************************************************************************
void draw_sprite(int pos, int number) //draw a sprite at specific point in char buffer - will be used for interface, weapon etc.
{
for(int x=0;x<32;x++)
for(int y=0;y<32;y++)
if(sprites[x+y*32+1024*number]/65536) //alpha>0?
	{
	char_buff[pos+x+res_X*y]=char_grad[sprites[x+y*32]%grad_length];
	color_buff[pos+x+res_X*y]=(sprites[x+y*32]/256)%256;
	}
}

//*********************************************************************************************************************
void draw_enemies()
{
int charn,color; //character number and color value to draw
//helper variables for calculating various distances and angles
int column,cx,cy; //screen column of sprite center, x,y coordinates to draw to
double ang0,ang1; //player angle and relative angle to sprite
double dx,dy,dx2,dy2; //for distance calculations
double dst,scale; //distance and sprite scale
double brightness; //brightness modifier for drawing sprite
	
for(int i=0;i<16;i++)
if(enemies[i].enabled==1)
	{
	ang0=player.ang_h/10.0; //player angle, in degrees

	dx=enemies[i].x-player.x; //x,y distance to enemy
	dy=enemies[i].y-player.y;
	dx2=cos(ang0*torad); //player heading vector
	dy2=sin(ang0*torad);

	double dot = dx2*dx + dy2*dy;      //dot product between [x1, y1] and [x2, y2]
	double det = dx2*dy - dy2*dx;      //determinant
	ang1 = atan2(det, dot)*todeg; 	   //player to enemy angle, degrees

	dst=sqrt(dx*dx+dy*dy); //distance to enemy
	scale=32.0/dst; //distance-based scaling
	column=(int)(res_X*(ang1)/(0.1*fov)); //screen column to draw on
	int plusy=(int)(32.0*player.z/dst); //player vertical pos modifier
	
	if(column>-res_X&&column<res_X&&scale<256) //we are within the screen? isn't sprite too big?
	for(int x=0;x<scale;x++)
	for(int y=0;y<scale;y++)
		{
		charn=sprites[((int)(32.0*x/scale)+32*(int)(32.0*y/scale))%1024+1024*enemies[i].type]; //base brightness
		cx=(int)(res_X/2-scale/2+x+column); //coordinate x
		cy=(int)(res_Y/2-scale/2+y-horizon_pos+plusy); //coordinate y
		if((charn/65536>0)&&(cy<res_Y)&&(cy>0)&&(cx<res_X)&&(cx>0)&&(depth_map[cx+cy*res_X]>dst)) //>0 alpha, we are within screen, not obscured (depth map)
			{
			color=(charn/256)%16; //record color

			brightness=32*light_global; //base global value
			brightness+=16*lightmap[(int)(16*enemies[i].x)][(int)(16*enemies[i].y)]; //apply 2-D lightmap
			brightness+=light_flashlight*flashlight_coeff[cx+cy*res_X]; //apply flashlight
			brightness=1E-6*(brightness+scale*4); //apply distance scaling coefficient
			charn=((int)(charn*brightness)); //final character value
			if(charn>grad_length)charn=grad_length;if(charn<0)charn=0; //value clamping	
			char_buff[cx+cy*res_X]=char_grad[charn]; //save character to bffer
			color_buff[cx+cy*res_X]=pal[color][(charn>pal_thr1)+(charn>pal_thr2)]; //save color to buffer 
			depth_map[cx+cy*res_X]=dst; //record depth value - so sprites can obscure each other; 
			//closer sprites will overdraw farther, farther cannot be drawn on closer due to above depth map update
			}
		}//end of enemy drawing	
	}//end of going through enemies
}

//*********************************************************************************************************************
// 										//main display function; widows-specific
//*********************************************************************************************************************

void display()
{
HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE); //handle to console window
COORD C;C.X=0;C.Y=0;//define coordinates, set them to 0,0
SetConsoleCursorPosition(hand, C); //set the cursor to 0,0
DWORD written; //helper flag, unused

WriteConsoleOutputCharacter(hand,char_buff,res_X*res_Y, C, &written); //write the character contents of char_buff[]
WriteConsoleOutputAttribute(hand, color_buff, res_X*res_Y, C, &written); //set colors according to color_buff[]
}


//*********************************************************************************************************************
// 										Controls (windows-specific), game logic
//*********************************************************************************************************************

void controls()//handles keyboard, mouse controls and player movement; windows-specific
{
double dx=player.accel*sintab[(int)player.ang_h%3600]; //x step in the direction player is looking; 
double dy=player.accel*sintab[((int)player.ang_h+900)%3600]; //y step in the direction player is looking
if (GetKeyState(0x41) & 0x8000){player.vx+=dx;player.vy-=dy;}; //WASD movement
if (GetKeyState(0x44) & 0x8000){player.vx-=dx;player.vy+=dy;};
if (GetKeyState(0x57) & 0x8000){player.vx+=dy;player.vy+=dx;};
if (GetKeyState(0x53) & 0x8000){player.vx-=dy;player.vy-=dx;};

if ((GetKeyState(0x46) & 0x8000)&&(key_delay<0.1)){light_flashlight=(1-light_flashlight);key_delay=1;}; //F for flashlight

if ((GetKeyState(VK_SPACE) & 0x8000)&&(player.z<0.05)){player.vz=player.jump_h;}; //space for jump

if (GetKeyState(VK_ESCAPE) & 0x8000)F_exit=1; //esc for exit

key_delay*=0.9; //delay so that toggle buttons (like flashlight) do not trigger 100x per second

if(player.x>(map_size-2))player.x=map_size-2; if(player.x<2)player.x=2; //failsafes from going out of map
if(player.y>(map_size-2))player.y=map_size-2; if(player.y<2)player.y=2;

if (map[(int)(player.x+1*player.vx)][(int)player.y]%256>0)player.vx=-player.vx/2;//collisions in x axis - bounce back with half the velocity
if (map[(int)player.x][(int)(player.y+1*player.vy)]%256>0)player.vy=-player.vy/2;//collisions in y axis
player.x+=player.vx; //update x,y values with x,y velocities
player.y+=player.vy; 
player.z+=player.vz;
player.vx*=(1-player.friction); //friction reduces velocity values
player.vy*=(1-player.friction); 
player.vz*=(1-player.friction); 

if(player.z>0){player.vz-=player.grav;}//gravity
if(player.z<=0){player.vz=0;player.z=0;} //standing on the ground

POINT point;//for mouse
if (GetCursorPos(&point)) {
 //set player angles according to mouse position. 500 and 20 are arbitraty values that just work OK
 player.ang_h=500.0*(point.x-point0.x)/mouse_speed; //player horizontal angle
 player.ang_v=20.0*(point.y-point0.y)/mouse_speed; //player vertical angle
 horizon_pos=(int)player.ang_v; //position of the horizon, for looking up/down 0=in the middle
}
if(player.ang_h<3600)player.ang_h+=3600; //if player angle is less than 360 degrees, add 360 degrees so its never negative
}


//*********************************************************************************************************************
// 									 Main game loop
//*********************************************************************************************************************

int main()
{
init();
set_palette();
gen_map();
gen_sky(10);
clear_buffers();show_map();display(); //show the map while program is calculating lights
calculate_lights();
gen_pacman_ghost(0,30,2); //sprite #1, brightness 30, color 2

while(F_exit==0)
{
controls();
cast();
draw();
//draw_sprite(0,0); //for testing purposes
draw_enemies();
display();
std::cout<<"fps:"<<fps<<"   ";
::QueryPerformanceFrequency(&nFrequency); 
::QueryPerformanceCounter(&nStartTime);
Sleep(1);
::QueryPerformanceCounter(&nStopTime);
nElapsed.QuadPart = (nStopTime.QuadPart - nStartTime.QuadPart) * 1000000;
nElapsed.QuadPart /= nFrequency.QuadPart;
fps=1000000/nElapsed.QuadPart;
}

}






