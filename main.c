#include <gb/gb.h>
#include <stdint.h>
#include <gb/drawing.h>
#include <stdio.h>
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
        1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,
        0,
        1,
        1,
        0,
        0,
        1,
        1,
        1,
        0,
        1,
        1,
        0,
        0,
        1,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        1,
        1,
        0,
        0,
        1,
        1,
        0,
        0,
        1,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
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

fixed8_t cos[360] = {128, 128, 128, 128, 128, 128, 127, 127, 127, 126, 126, 126, 125, 125, 124, 124, 123, 122, 122, 121, 120, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 106, 105, 104, 102, 101, 100, 98, 97, 95, 94, 92, 91, 89, 87, 86, 84, 82, 81, 79, 77, 75, 73, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 35, 33, 31, 29, 27, 25, 22, 20, 18, 16, 13, 11, 9, 7, 5, 2, 0, -2, -4, -7, -9, -11, -13, -15, -18, -20, -22, -24, -26, -29, -31, -33, -35, -37, -39, -42, -44, -46, -48, -50, -52, -54, -56, -58, -60, -62, -64, -66, -68, -70, -71, -73, -75, -77, -79, -80, -82, -84, -86, -87, -89, -90, -92, -94, -95, -96, -98, -99, -101, -102, -103, -105, -106, -107, -108, -110, -111, -112, -113, -114, -115, -116, -117, -118, -119, -119, -120, -121, -122, -122, -123, -124, -124, -125, -125, -126, -126, -126, -127, -127, -127, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -127, -127, -127, -126, -126, -126, -125, -125, -124, -124, -123, -122, -122, -121, -120, -120, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109, -107, -106, -105, -104, -102, -101, -100, -98, -97, -95, -94, -92, -91, -89, -87, -86, -84, -82, -81, -79, -77, -75, -74, -72, -70, -68, -66, -64, -62, -60, -58, -56, -54, -52, -50, -48, -46, -44, -42, -40, -38, -36, -33, -31, -29, -27, -25, -23, -20, -18, -16, -14, -11, -9, -7, -5, -3, 0, 2, 4, 6, 9, 11, 13, 15, 18, 20, 22, 24, 26, 28, 31, 33, 35, 37, 39, 41, 43, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 69, 71, 73, 75, 77, 79, 80, 82, 84, 85, 87, 89, 90, 92, 93, 95, 96, 98, 99, 101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 124, 125, 125, 126, 126, 126, 127, 127, 127, 127, 128, 128, 128, 128}; // sin cos in valuse 0-360 degrees
fixed8_t sin[360] = {0, 2, 4, 7, 9, 11, 13, 16, 18, 20, 22, 24, 27, 29, 31, 33, 35, 37, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 73, 75, 77, 79, 81, 82, 84, 86, 87, 89, 90, 92, 94, 95, 97, 98, 99, 101, 102, 104, 105, 106, 107, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 124, 125, 125, 126, 126, 126, 127, 127, 127, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 127, 127, 127, 126, 126, 126, 125, 125, 124, 124, 123, 122, 122, 121, 120, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 106, 105, 104, 102, 101, 100, 98, 97, 95, 94, 92, 91, 89, 87, 86, 84, 82, 81, 79, 77, 75, 74, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 35, 33, 31, 29, 27, 25, 22, 20, 18, 16, 14, 11, 9, 7, 5, 2, 0, -2, -4, -6, -9, -11, -13, -15, -18, -20, -22, -24, -26, -29, -31, -33, -35, -37, -39, -41, -44, -46, -48, -50, -52, -54, -56, -58, -60, -62, -64, -66, -68, -70, -71, -73, -75, -77, -79, -80, -82, -84, -85, -87, -89, -90, -92, -93, -95, -96, -98, -99, -101, -102, -103, -105, -106, -107, -108, -110, -111, -112, -113, -114, -115, -116, -117, -118, -119, -119, -120, -121, -122, -122, -123, -124, -124, -125, -125, -126, -126, -126, -127, -127, -127, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -127, -127, -127, -126, -126, -126, -125, -125, -124, -124, -123, -123, -122, -121, -120, -120, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109, -108, -106, -105, -104, -102, -101, -100, -98, -97, -95, -94, -92, -91, -89, -88, -86, -84, -83, -81, -79, -77, -76, -74, -72, -70, -68, -66, -64, -62, -60, -58, -56, -54, -52, -50, -48, -46, -44, -42, -40, -38, -36, -34, -31, -29, -27, -25, -23, -20, -18, -16, -14, -12, -9, -7, -5, -3};

typedef struct
{
    uint16_t x, y;
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
void drawRays2D_old()
{


 INT32 r,mx,my,mp,dof,side; INT32 vx,vy,rx,ry,ra; INT32 disV,disH;

 ra= getCos(pa2+30);   //ray set back 30 degrees
 INT32 ra2 = getSin(pa2+30);                                                             //ray set back 30 degrees

 for(r=0;r<60;r++)
 {
  //---Vertical---
  dof=0; side=0; disV=100000;
  uint16_t Tan=getTan(ra);
       if(ra> -1){ rx=(((uint16_t)px>>6)<<6)+64;      ry=(px-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
  else if(ra< 0){ rx=(((uint16_t)px>>6)<<6)-0; ry=(px-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
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
        
        fixed8_t RayA = FIXED_MUL((P.a - FIXED_DIV(FOV, 2)) + (x / ViewLength), FOV);
        RayA = FIXED_MUL(RayA, FOV);
        BOOLEAN HitWall = 0;
        fixed8_t EyeX = sin[FIXED_TO_INT(RayA)];
        fixed8_t EyeY = cos[FIXED_TO_INT(RayA)];

        while (HitWall == 0 && FIXED_TO_INT(DeltaWall) < depth)
        {
            DeltaWall += 13; // 0x1;//(1 << 4) / 2;///(1 << 4) / 2; // find way to make go up by 0.5
                            //  DeltaWall >> 4;
            uint16_t TestX = FIXED_TO_INT(FIXED_MUL((P.x + EyeX), DeltaWall));
            uint16_t TestY = FIXED_TO_INT(FIXED_MUL((P.y + EyeY), DeltaWall));
            // Test if ray is out of bounds
            if (TestX <= 0 || TestX >= mapY || TestY <= 0 || TestY >= mapX) //(TestX >= mapX || TestY >= mapY)
            {
                HitWall = TRUE;
                DeltaWall = depth;
                //   DeltaWall = IntToFP(depth);//Make_FP(depth, 16);
            }
            else
            {
                // uint8_t x,y;
                if (map[TestX * mapY + TestY] == 1) //
                {
                    HitWall = TRUE;
                }
            }
        }

        fixed8_t Roof = FIXED_DIV(ViewHeight , 2) - FIXED_DIV(ViewHeight , DeltaWall);
        Roof = FIXED_DIV(Roof, DeltaWall);
        fixed8_t Floor = ViewHeight - Roof;
        // uint16_t lineOff = ViewLength - (Roof>>1);

        uint8_t Shade;
        //   DeltaWall = FP_to_Int(DeltaWall);

        if (FIXED_TO_INT(DeltaWall) <= (depth / 4))
            Shade = 3; // Very close
        else if (FIXED_TO_INT(DeltaWall) < (depth / 3))
            Shade = 2;
        else if (FIXED_TO_INT(DeltaWall) < (depth / 2))
            Shade = 1;
        else
            Shade = 0; // Too far away

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
                plot(x, y, Shade, SOLID);
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

void update_world()
{
    // drawMap2D();
    // delay(1000);
    draw3DWorld();
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
    P.x = 2; // player x position in fixed point
    P.y = 2; // player y position in fixed point
    // pa2 = dir*45;
    // pdx=getCos(pa2);
    // pdy=-getSin(pa2);
}

void main(void)
{
   // drawMap2D();
    draw3DWorld();
    //set_sprite_data(0, 8, Player_Data);
   // set_sprite_tile(0, 0);
    init();
    SHOW_BKG;
    SHOW_SPRITES;
    while (1)
    {
        int cur = joypad();

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

        fixed8_t dx = sin[P.a]*10;
        fixed8_t dy = cos[P.a]*10;

        if (cur & J_UP)
        {
            P.x += FIXED_TO_INT(dx);
            P.y += FIXED_TO_INT(dy);
        }
        else if (cur & J_DOWN)
        {
            P.x -= FIXED_TO_INT(dx);
            P.y -= FIXED_TO_INT(dy);
        }

        if (cur & J_LEFT)
        {
            if (P.a != 0)
            {
                P.a -= 1;
            }
            else
            {
                P.a = 360;
            }
        }
        else if (cur & J_RIGHT)
        {
            if (P.a != 359)
            {
                P.a += 1;
            }
            else
            {
                P.a = 0;
            }
        }

        display();
    }
}
