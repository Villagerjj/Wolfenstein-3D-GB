#include <gb/gb.h>
#include <stdint.h>
#include <gb/drawing.h>
#include <stdio.h>
//#include "LUTs.h"
//#include <gb/uint2fs.c>
//#include <gb/fsadd.c>
#define scale 16    // 0/16 (make your decimal into a fraction with the bottom number being 16, then do this: EX: 0.5 = 8/16 0x8)
#define halfscale 8 // same as normal scale but / 2
#define SCALE 8
#define HALF_SCALE 4
#define FIXED_VAL 128

typedef int16_t fixed8_t;

#define PI 402 // this is 3.14 in fixed points
// uint8_t PI = 0x5;

#define DoubleToFixed(x) (x * (double)(1 << SCALE))
#define FIXED_TO_INT(x) (x >> SCALE)
#define INT_TO_FIXED(x) (x << SCALE)
#define FIXED_MUL(x, y) (((x) >> HALF_SCALE * (y) >> HALF_SCALE) >> 0)
#define FIXED_DIV(x, y) (((x) << (HALF_SCALE - 1)) / (y) << (HALF_SCALE + 1))
#define FIXED_ADD_2_WHOLE(x, y) ((x) + (y << SCALE))
#define FIXED_SUB_WHOLE(x, y) ((x) - (y << SCALE))
#define ViewHeight 32//SCREENHEIGHT // was 144
#define ViewLength 32//SCREENWIDTH  // width, also was 160
#define depth 16
#define FOV 100 // this is 3.14 / 4.0 in fixed points
//#include "Player.c"
uint8_t pmapx = 1; // player x position in the map
uint8_t pmapy = 1; // player y position in the map



uint16_t mspeed = 16;

uint8_t mapX = 8, mapY = 8, mapS = 8;
uint8_t map[] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1

    };

/* uint16_t round(uint16_t x) {
    uint8_t looper;
    uint8_t rounds;
    uint8_t temp = x;
     while (looper < rounds)
     {
         temp=- 1.1;
         rounds++;
         if ((x - temp)/rounds = 1)
         {
             return x - rounds*0.1;
         }
     }
    return x;
} */

/*
uint16_t fixed_to_double(fixed_point_t input)
{
    return ((uint16_t)input / (uint16_t)(1 << FIXED_POINT_FRACTIONAL_BITS));
}

fixed_point_t double_to_fixed(double input)
{
    return (fixed_point_t)(input * (1 << FIXED_POINT_FRACTIONAL_BITS));
}
*/

fixed8_t cosR[360] = {128, 128, 128, 128, 128, 128, 127, 127, 127, 126, 126, 126, 125, 125, 124, 124, 123, 122, 122, 121, 120, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 106, 105, 104, 102, 101, 100, 98, 97, 95, 94, 92, 91, 89, 87, 86, 84, 82, 81, 79, 77, 75, 73, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 35, 33, 31, 29, 27, 25, 22, 20, 18, 16, 13, 11, 9, 7, 5, 2, 0, -2, -4, -7, -9, -11, -13, -15, -18, -20, -22, -24, -26, -29, -31, -33, -35, -37, -39, -42, -44, -46, -48, -50, -52, -54, -56, -58, -60, -62, -64, -66, -68, -70, -71, -73, -75, -77, -79, -80, -82, -84, -86, -87, -89, -90, -92, -94, -95, -96, -98, -99, -101, -102, -103, -105, -106, -107, -108, -110, -111, -112, -113, -114, -115, -116, -117, -118, -119, -119, -120, -121, -122, -122, -123, -124, -124, -125, -125, -126, -126, -126, -127, -127, -127, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -127, -127, -127, -126, -126, -126, -125, -125, -124, -124, -123, -122, -122, -121, -120, -120, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109, -107, -106, -105, -104, -102, -101, -100, -98, -97, -95, -94, -92, -91, -89, -87, -86, -84, -82, -81, -79, -77, -75, -74, -72, -70, -68, -66, -64, -62, -60, -58, -56, -54, -52, -50, -48, -46, -44, -42, -40, -38, -36, -33, -31, -29, -27, -25, -23, -20, -18, -16, -14, -11, -9, -7, -5, -3, 0, 2, 4, 6, 9, 11, 13, 15, 18, 20, 22, 24, 26, 28, 31, 33, 35, 37, 39, 41, 43, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 69, 71, 73, 75, 77, 79, 80, 82, 84, 85, 87, 89, 90, 92, 93, 95, 96, 98, 99, 101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 124, 125, 125, 126, 126, 126, 127, 127, 127, 127, 128, 128, 128, 128}; // sin cos in valuse 0-360 degrees
fixed8_t sinR[360] = {0, 2, 4, 7, 9, 11, 13, 16, 18, 20, 22, 24, 27, 29, 31, 33, 35, 37, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 73, 75, 77, 79, 81, 82, 84, 86, 87, 89, 90, 92, 94, 95, 97, 98, 99, 101, 102, 104, 105, 106, 107, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 124, 125, 125, 126, 126, 126, 127, 127, 127, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 127, 127, 127, 126, 126, 126, 125, 125, 124, 124, 123, 122, 122, 121, 120, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 106, 105, 104, 102, 101, 100, 98, 97, 95, 94, 92, 91, 89, 87, 86, 84, 82, 81, 79, 77, 75, 74, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 35, 33, 31, 29, 27, 25, 22, 20, 18, 16, 14, 11, 9, 7, 5, 2, 0, -2, -4, -6, -9, -11, -13, -15, -18, -20, -22, -24, -26, -29, -31, -33, -35, -37, -39, -41, -44, -46, -48, -50, -52, -54, -56, -58, -60, -62, -64, -66, -68, -70, -71, -73, -75, -77, -79, -80, -82, -84, -85, -87, -89, -90, -92, -93, -95, -96, -98, -99, -101, -102, -103, -105, -106, -107, -108, -110, -111, -112, -113, -114, -115, -116, -117, -118, -119, -119, -120, -121, -122, -122, -123, -124, -124, -125, -125, -126, -126, -126, -127, -127, -127, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -127, -127, -127, -126, -126, -126, -125, -125, -124, -124, -123, -123, -122, -121, -120, -120, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109, -108, -106, -105, -104, -102, -101, -100, -98, -97, -95, -94, -92, -91, -89, -88, -86, -84, -83, -81, -79, -77, -76, -74, -72, -70, -68, -66, -64, -62, -60, -58, -56, -54, -52, -50, -48, -46, -44, -42, -40, -38, -36, -34, -31, -29, -27, -25, -23, -20, -18, -16, -14, -12, -9, -7, -5, -3};
fixed8_t cos[360] = {128, 69, -53, -127, -84, 36, 123, 96, -19, -117, -107, 1, 108, 116, 18, -97, -123, -35, 85, 127, 52, -70, -128, -68, 54, 127, 83, -37, -123, -96, 20, 117, 107, -2, -109, -116, -16, 98, 122, 34, -85, -126, -51, 71, 128, 67, -55, -127, -82, 38, 124, 95, -21, -118, -106, 3, 109, 115, 15, -99, -122, -33, 86, 126, 50, -72, -128, -66, 56, 127, 81, -40, -124, -94, 22, 118, 106, -4, -110, -115, -14, 99, 122, 32, -87, -126, -49, 73, 128, 65, -57, -127, -80, 41, 124, 93, -23, -118, -105, 5, 110, 114, 13, -100, -121, -31, 88, 126, 48, -74, -128, -64, 58, 127, 79, -42, -124, -93, 24, 119, 104, -6, -111, -114, -12, 101, 121, 30, -89, -126, -47, 75, 128, 63, -59, -127, -78, 43, 125, 92, -25, -119, -104, 7, 112, 113, 11, -102, -120, -29, 90, 125, 46, -76, -128, -62, 60, 128, 78, -44, -125, -91, 26, 120, 103, -8, -112, -113, -10, 102, 120, 28, -90, -125, -45, 77, 128, 61, -61, -128, -77, 45, 125, 90, -28, -120, -102, 10, 113, 112, 8, -103, -120, -26, 91, 125, 44, -78, -128, -60, 62, 128, 76, -46, -125, -90, 29, 120, 102, -11, -113, -112, -7, 104, 119, 25, -92, -125, -43, 78, 127, 59, -63, -128, -75, 47, 126, 89, -30, -121, -101, 12, 114, 111, 6, -104, -119, -24, 93, 124, 42, -79, -127, -58, 64, 128, 74, -48, -126, -88, 31, 121, 100, -13, -114, -110, -5, 105, 118, 23, -93, -124, -41, 80, 127, 57, -65, -128, -73, 49, 126, 87, -32, -122, -99, 14, 115, 110, 4, -106, -118, -22, 94, 124, 40, -81, -127, -56, 66, 128, 72, -50, -126, -86, 33, 122, 99, -15, -115, -109, -3, 106, 118, 21, -95, -124, -38, 82, 127, 55, -67, -128, -71, 51, 126, 85, -34, -122, -98, 16, 116, 109, 2, -107, -117, -20, 96, 123, 37, -83, -127, -54, 68, 128, 70, -52, -127, -85, 35, 123, 97, -18, -116, -108, -1, 107, 117, 19, -97, -123, -36, 84, 127, 53, -69, -128, -69, 53, 127, 84}; // sin cos in valuse 0-360 degrees
fixed8_t sin[360] = {0, 108, 116, 18, -97, -123, -36, 84, 127, 53, -70, -128, -69, 54, 127, 83, -37, -123, -96, 19, 117, 107, -1, -108, -116, -17, 98, 122, 35, -85, -126, -52, 71, 128, 68, -55, -127, -82, 38, 123, 95, -20, -117, -106, 2, 109, 115, 16, -98, -122, -34, 86, 126, 51, -72, -128, -67, 56, 127, 82, -39, -124, -95, 21, 118, 106, -3, -110, -115, -15, 99, 122, 32, -87, -126, -50, 72, 128, 66, -57, -127, -81, 40, 124, 94, -23, -118, -105, 5, 110, 114, 14, -100, -121, -31, 87, 126, 49, -73, -128, -65, 58, 127, 80, -41, -124, -93, 24, 119, 105, -6, -111, -114, -12, 100, 121, 30, -88, -126, -48, 74, 128, 64, -59, -127, -79, 42, 124, 92, -25, -119, -104, 7, 111, 113, 11, -101, -121, -29, 89, 125, 46, -75, -128, -63, 60, 128, 78, -43, -125, -92, 26, 119, 103, -8, -112, -113, -10, 102, 120, 28, -90, -125, -45, 76, 128, 62, -61, -128, -77, 44, 125, 91, -27, -120, -103, 9, 112, 112, 9, -103, -120, -27, 91, 125, 44, -77, -128, -61, 62, 128, 76, -45, -125, -90, 28, 120, 102, -10, -113, -112, -8, 103, 119, 26, -92, -125, -43, 78, 128, 60, -63, -128, -75, 46, 125, 89, -29, -121, -101, 11, 113, 111, 7, -104, -119, -25, 92, 124, 42, -79, -127, -59, 64, 128, 74, -48, -126, -88, 30, 121, 100, -12, -114, -111, -6, 105, 119, 24, -93, -124, -41, 80, 127, 58, -65, -128, -73, 49, 126, 87, -31, -121, -100, 14, 114, 110, 5, -105, -118, -23, 94, 124, 40, -81, -127, -57, 66, 128, 72, -50, -126, -87, 32, 122, 99, -15, -115, -110, -3, 106, 118, 21, -95, -124, -39, 82, 127, 56, -67, -128, -72, 51, 126, 86, -34, -122, -98, 16, 115, 109, 2, -106, -117, -20, 95, 123, 38, -82, -127, -55, 68, 128, 71, -52, -126, -85, 35, 122, 98, -17, -116, -108, -1, 107, 117, 19, -96, -123, -37, 83, 127, 54, -69, -128, -70, 53, 127, 84, -36, -123, -97, 18, 116, 108, 0, -108, -116, -18, 97};

typedef struct
{
    fixed8_t x, y;
    fixed8_t a;
    uint16_t l;
} player;
player P;

uint16_t degToRad(uint16_t x)
{
    x = x * -1;
    return x;
}

typedef uint16_t fixed_point_t;

void drawMap2D()
{
    uint8_t x, y, xo, yo;
    for (y = 0; y < mapY; y++)
    {
        for (x = 0; x < mapX; x++)
        {
            if (map[y * mapX + x] == 1)
            {
                color(BLACK, WHITE, 1);
            }
            else if (map[y * mapX + x] == 2)
            {
                color(2, 2, 0);
            }
            else
            {
                color(WHITE, WHITE, 1);
            }
            xo = x * mapS;
            yo = y * mapS;
            line(xo, yo, mapS + xo, yo);
            line(xo, yo, xo, mapS + yo);
            line(xo, mapS + yo, mapS + xo, mapS + yo);
            line(mapS + xo, mapS + yo, mapS + xo, yo);
            /*
                1 0
                8 0 */
        }
    }
}

/*
void draw3DWorld_meh()
{


 fixed8_t r,mx,my,mp,dof,side; fixed8_t vx,vy,rx,ry,ra; fixed8_t disV,disH;

 ra= cos[pa2+30];   //ray set back 30 degrees
 fixed8_t ra2 = sin[pa2+30];                                                             //ray set back 30 degrees

 for(r=0;r<60;r++)
 {
  //---Vertical---
  dof=0; side=0; disV=100000;
  fixed8_t Tan= tan(ra); // add tan lut
       if(ra> -1){ rx=((P.x>>6)<<6)+64;      ry=(P.x-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
  else if(ra< 0){ rx=((P.x>>6)<<6)-0; ry=(P.x-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
  else { rx=px; ry=py; dof=8;}                                                  //looking up or down. no hit
  while(dof<8)
  {
   mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
   if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disV=ra*(rx-px)-ra2*(ry-py);}//hit
   else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
  }
  vx=rx; vy=ry;

  //---Horizontal---
  dof=0; disH=100000;
  Tan=1.0/Tan;
       if(ra> 0){ ry=(((uint16_t)py>>6)<<6)-0; rx=(py-ry)*Tan+px; yo =-64; xo=- yo * Tan;}//looking up
  else if(ra< -0){ ry=(((uint16_t)py>>6)<<6)+64;rx=(py-ry)*Tan+px; yo = 64; xo=- yo * Tan;}//looking down
  else{ rx=px; ry=py; dof=8;}                                                   //looking straight left or right

  while(dof<8)
  {
   mx=(uint16_t)(rx)>>6; my=(uint16_t)(ry)>>6; mp=my*mapX+mx;
   if(mp>0 && mp<mapX*mapY && map[mp]==1)
   {
    dof=8;
    disH= ra*(rx-px)-ra2*(ry-py);
   }//hit
   else
   {
    rx+=xo;
    ry+=yo;
    dof+=1;
    }                                               //check next horizontal
  }

  color(BLACK, WHITE, 1);
  if(disV<disH){ rx=vx; ry=vy; disH=disV; color(BLACK, WHITE, 1);}                  //horizontal hit first
  //line(px,py,rx,ry);//draw 2D ray

  uint16_t ca= getCos(pa-ra);
    disH=disH*ca;                            //fix fisheye
  uint16_t lineH = (mapS*144)/(disH); if(lineH>144){ lineH=144;}                     //line height and limit
  uint16_t lineOff = 160 - (lineH>>1);                                               //line offset

 box(r*8+530,lineOff,r*8+530,lineOff+lineH+8, SOLID);//draw vertical wall

  ra=ra-1;
                                                      //go to next ray
 }
}
*/


void draw3DWorld()
{
    for (uint8_t x = 0; x < ViewLength; x++)
    {
        fixed8_t DeltaWall = 0;
        uint8_t HitWall = 0;
        fixed8_t RayA = (P.a - 30) + (x/ViewLength) * 60;
        if (RayA < 0)
        {
            RayA = 360;
        }
        else if (RayA > 360)
        {
            RayA = 0;
        }
      //  fixed8_t RayA = FIXED_MUL((INT_TO_FIXED(P.a) - 50) + INT_TO_FIXED(x/160), FOV);
     //   fixed8_t RayA = FIXED_DIV((INT_TO_FIXED(P.a) - FOV),INT_TO_FIXED(2));
      //  RayA += INT_TO_FIXED(x/SCREENWIDTH);
      //  RayA = FIXED_MUL(RayA, FOV);
        // /(P.a - 50f) + (x / 160) * 100f;
       // RayA = FIXED_MUL(RayA, FOV);
      //  uint8_t RAYAINT = FIXED_TO_INT(RayA);
      fixed8_t EyeX = sin[RayA];
      fixed8_t EyeY = cos[RayA];
      
    
        
        //fixed8_t dy = sin[RayA];
       // fixed8_t dx = cos[RayA];

        while (HitWall != 1 && FIXED_TO_INT(DeltaWall) < depth)
        {
            DeltaWall += 13; // 0.1 in fixed point 0x1;//(1 << 4) / 2;///(1 << 4) / 2; 
                            //  DeltaWall >> 4;
            
            //fixed8_t TestX = FIXED_MUL((P.x + EyeX), DeltaWall);
            //fixed8_t TestY = FIXED_MUL((P.y + EyeY), DeltaWall);
            fixed8_t TestX = (P.x + FIXED_MUL(EyeX , DeltaWall));
            fixed8_t TestY = (P.y + FIXED_MUL(EyeY , DeltaWall));
            // Test if ray is out of bounds
            if (FIXED_TO_INT(TestX) <= 0 || FIXED_TO_INT(TestX) >= mapY || FIXED_TO_INT(TestY) <= 0 || FIXED_TO_INT(TestY) >= mapX) //(TestX >= mapX || TestY >= mapY)
            {
                HitWall = 1;
                DeltaWall = depth;  
                //   DeltaWall = IntToFP(depth);//Make_FP(depth, 16);
            }
            else
            {
                // uint8_t x,y;
                if (map[FIXED_TO_INT(TestX) * mapY + FIXED_TO_INT(TestY)] == 1) //
                {
                    HitWall = 1;
                }
            }
        }

        uint16_t Roof = INT_TO_FIXED(16) - FIXED_DIV(INT_TO_FIXED(32), DeltaWall);
        uint16_t Floor = 32 - FIXED_TO_INT(Roof);
        // uint16_t lineOff = ViewLength - (Roof>>1);
        uint8_t Shade = 2;
        //   DeltaWall = FP_to_Int(DeltaWall);
/*
        if (FIXED_TO_INT(DeltaWall) <= (4))
        {
              Shade = 3; // Very close
        }
        else if (FIXED_TO_INT(DeltaWall) < (5))
        { 
              Shade = 2;
        }
        else if (FIXED_TO_INT(DeltaWall) < (8))
        {
              Shade = 1;
        }
        else
        {
              Shade = 2; // Too far away
        }*/
            

        for (uint8_t y = 0; y < ViewHeight; y++)
        {

            if (y <= FIXED_TO_INT(Roof))
            {
                plot(x, y, 0, SOLID);
                // plot(x + 1, y, 2, 1);
            }

            else if (y > FIXED_TO_INT(Roof) && y <= FIXED_TO_INT(Floor))
            {
                //  x += 16;
                // y -= 16;
                plot(x, y, 2, SOLID);
                // plot(x + 1, y, Shade, 1);
            }
            else // Floor
            {
                plot(x, y, 1, SOLID);
                // plot(x + 1, y, 2, 1);
            }
        }
        
    }

    // clearscreen();
}

void drawPlayer()
{
    
    move_sprite(0, P.x, P.y);
    // line(px, py, px+pdx*5, py+pdy*5);
}

void clearscreen()
{
    // color(WHITE, WHITE, 0);
    for (uint8_t x = 0; x < ViewLength; x++)
    {
        for (uint8_t y = 0; x < ViewHeight; y++)
        {
            plot(x, y, 0, 0);
        }
    }
}

void VarDebug(int x, int y)
{
    printf("X = %d Y = %d",x,y);
}

void DebugPlayer2D()
{
  plot(FIXED_TO_INT(P.x), FIXED_TO_INT(P.y), 3, 0);
  delay(1000);
}

void update_world()
{
    // drawMap2D();
    // delay(1000);
    draw3DWorld();
    //VarDebug(FIXED_TO_INT(P.x), FIXED_TO_INT(P.y));
    //DebugPlayer2D();
//wait(100);
    // clearscreen();
}

void display()
{
    update_world();
    
   // drawPlayer();
    // clearscreen();
}

void init()
{
            // player y position in the map
    P.x = 1882; // 14.7 in fixed point player x position in fixed point
    P.y = 652; // 5.09  in fixed point player y position in fixed point
    // pa2 = dir*45;
    // pdx=getCos(pa2);
    // pdy=-getSin(pa2);
}

void main(void)
{
   // drawMap2D();
    //draw3DWorld();
    //set_sprite_data(0, 8, Player_Data);
   // set_sprite_tile(0, 0);
    init();
    SHOW_BKG;
    SHOW_SPRITES;
    while (1)
    {
        uint8_t cur = joypad();

        if (cur & J_A)
        {
        }

        if (cur & J_B)
        {
        }

        // Iterate through the available blocks.
        if (cur & J_SELECT)
        {
        }

        if (cur & J_START)
        {
        }

        fixed8_t dx = sinR[P.a];
        fixed8_t dy = cosR[P.a];

        if (cur & J_UP)
        {
           // P.x += FIXED_TO_INT(dx);
           // P.y += FIXED_TO_INT(dy);
           P.x -= dx;
           P.y -= dy;
            //VarDebug(FIXED_TO_INT(P.x), FIXED_TO_INT(P.y));
        }
        else if (cur & J_DOWN)
        {
           // P.x -= FIXED_TO_INT(dx);
           // P.y -= FIXED_TO_INT(dy);
           P.x += dx;
           P.y += dy;
            //VarDebug(FIXED_TO_INT(P.x), FIXED_TO_INT(P.y));
        }

        if (cur & J_LEFT)
        {
            if (P.a < 356)
            {
                P.a += 4;
                //VarDebug(FIXED_TO_INT(P.x), FIXED_TO_INT(P.y));
            }
            else
            {
                P.a = 0;
            }
        }
        else if (cur & J_RIGHT)
        {
            

            if (P.a > 0)
            {
                P.a -= 4;
                //VarDebug(FIXED_TO_INT(P.x), FIXED_TO_INT(P.y));
            }
            else
            {
                P.a = 356;
            }
        }

        display();
    }
}
