// https://youtu.be/BJs29RicyPw

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define FIELD_WIDTH (10)
#define FIELD_HEIGHT (20)

#define SHAPE_WIDTH_MAX (4)
#define SHAPE_HEIGHT_MAX (4)

#define FPS (1)
#define INTERVAL (1000/FPS) // 更新頻度

enum
{
    /*
        I-テトリミノ（水色）
        O-テトリミノ（黄色）
        S-テトリミノ（緑）
        Z-テトリミノ（赤）
        J-テトリミノ（青）
        L-テトリミノ（オレンジ）
        T-テトリミノ（紫）
    */

   SHAPE_I,
   SHAPE_O,
   SHAPE_S,
   SHAPE_Z,
   SHAPE_J,
   SHAPE_L,
   SHAPE_T,
   SHAPE_MAX
};

typedef struct
{
    int width, height;
    int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];    
}SHAPE;

typedef struct
{
    int x,y;
    SHAPE shape;
}MINO;


SHAPE shapes[SHAPE_MAX] =
{
    // SHAPE_I,
    {
        // int width, height;
        4, 4,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    // SHAPE_O,
    {
        // int width, height;
        2, 2,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,1},
            {1,1},
        }
    },
    // SHAPE_S,
    {
        // int width, height;
        3, 3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,1,1},
            {1,1,0},
            {0,0,0}
        }
    },
    // SHAPE_Z,
    {
        // int width, height;
        3, 3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,1,0},
            {0,1,1},
            {0,0,0}
        }
    },
    // SHAPE_J,
    {
        // int width, height;
        3, 3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,0,0},
            {1,1,1},
            {0,0,0}
        }
    },
    // SHAPE_L,
    {
        // int width, height;
        3, 3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,0,1},
            {1,1,1},
            {0,0,0}
        }
    },
    // SHAPE_T,
    {
        // int width, height;
        3, 3,
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,1,0},
            {1,1,1},
            {0,0,0}
        }
    }
};

int field[FIELD_HEIGHT][FIELD_WIDTH];
int screen[FIELD_HEIGHT][FIELD_WIDTH];

MINO mino;

bool MinoIntersectField()
{
    for(int y=0;y<mino.shape.height;y++)
    {
        for(int x=0;x<mino.shape.width;x++)
        {
            if(mino.shape.pattern[y][x])
            {
                if((mino.y+y<0) || (mino.y+y>=FIELD_HEIGHT) || (mino.x+x<0) || (mino.x+x>=FIELD_WIDTH))
                {
                    return true;
                }
                if(field[mino.y+y][mino.x+x])
                {
                    return true;
                }
            }   
        }
    }

    return false;

}

void DrawScreen()
{
    memcpy(screen,field,sizeof(field));

    for(int y=0;y<mino.shape.height;y++)
    {
        for(int x=0;x<mino.shape.width;x++)
        {
            if(mino.shape.pattern[y][x])
            {
                screen[mino.y+y][mino.x+x] |= 1;
            }
        }
    }

    system("cls");

    for(int y=0;y<FIELD_HEIGHT;y++)
    {
        printf("#");
        for(int x=0;x<FIELD_WIDTH;x++)
        {
            printf("%s",screen[y][x]?"$":" ");
        }
        printf("#");
        printf("\n");
    }
    for(int x=0;x<FIELD_WIDTH+2;x++)
    {
        printf("#");
    }
}

void InitMino()
{
    mino.shape = shapes[rand()%SHAPE_MAX];
    mino.x = (FIELD_WIDTH - mino.shape.width) / 2; // align: center;
    mino.y = 0;

}

void Init()
{
    memset(field,0,sizeof(field));

    InitMino();

    DrawScreen();

}

int main()
{
    srand((unsigned int)time(NULL));
    Init();

    clock_t lastClock = clock(); // how time passed since started
    while (1)
    {
        clock_t nowClock = clock();
        if(nowClock>=lastClock+INTERVAL)
        {
            lastClock = nowClock;

            MINO lastMino = mino;
            mino.y++;
            
            if(MinoIntersectField())
            {
                mino = lastMino; // 無かったことに

                for(int y=0;y<mino.shape.height;y++)
                {
                    for(int x=0;x<mino.shape.width;x++)
                    {
                        if(mino.shape.pattern[y][x])
                        {
                            field[mino.y+y][mino.x+x] |= 1;
                        }
                    }
                }

                for(int y=0;y<FIELD_HEIGHT;y++)
                {
                    bool completed = true;
                    for(int x=0;x<FIELD_WIDTH;x++)
                    {
                        if(!field[y][x]) // そろってない
                        {
                            completed = false;
                            break;
                        }
                    }
                    if(completed)
                    {
                        for(int x=0;x<FIELD_WIDTH;x++)
                        {
                            field[y][x] = 0;
                        }
                        for(int y2=y;y2>=1;y2--)
                        {
                            for(int x=0;x<FIELD_WIDTH;x++)
                            {
                                field[y2][x] = field[y2-1][x];   
                                field[y2-1][x] = 0;
                            }
                        }
                    }
                }

                InitMino();

            }


            DrawScreen();
        }

        if(_kbhit())
        {
            MINO lastMino = mino;
            switch (_getch())
            { // https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q1477852264
                case 'w':
                    break;
                case 's':
                    mino.y++;
                    break;
                case 'a':
                    mino.x--;
                    break;
                case 'd':
                    mino.x++;
                    break;
                case ' ':
                    {
                        MINO newMino = mino;

                        for(int y=0;y<mino.shape.height;y++)
                        {
                            for(int x=0;x<mino.shape.width;x++)
                            {
                                newMino.shape.pattern[mino.shape.width-1-x][y] = mino.shape.pattern[y][x];
                            }
                        }
                        mino = newMino;
                    }
                    break;
                default:
                    break;
            }

            if(MinoIntersectField())
            {
                mino = lastMino; // 無かったことに
            }

            DrawScreen();
        }
    }
    
}

/*
    // before move
    0000
    111|
    0000
    0000

    '|': (x,y) = (3,1)

    // after move
    0|00
    0100
    0100
    0100

    '|': (x',y') = (1,0) = (y,W-1-x) // W-1-x = 4-1-3 = 0

    // after move
    0000
    0000
    |111
    0000

    '|': (x'',y'') = (0,2) = (y',W-1-x')

    // after move
    0010
    0010
    0010
    00|0

    '|': (x''',y''') = (2,3)

    (x,y) -> ()

*/