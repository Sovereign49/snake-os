#include "efi.h"
// GLOBAL VARIABLES //
#define COL 80
#define ROW 25
typedef enum {
  UP,DOWN,LEFT,RIGHT
} DIRECTION;
typedef struct Vec2 {
  INTN X;
  INTN Y;
} Vec2;
EFI_SYSTEM_TABLE* ST;
Vec2 snake[COL-2*ROW-2] = { { .X=-1, .Y=-1 } };
UINTN snake_size = 0;
BOOLEAN grow = 0;
DIRECTION dir = RIGHT;

VOID init_board() {
  // Initialize I/O
  ST->ConOut->SetMode(ST->ConOut, 0); // Screen is 80x25 characters
  ST->ConOut->Reset(ST->ConOut, 1);
  ST->ConIn->Reset(ST->ConIn, 1);
  ST->ConOut->EnableCursor(ST->ConOut, 1);
  // Draw borders
  for(UINT8 x=0; x<COL; x++) {
    for(UINT8 y=0; y<ROW; y++) {
        if (x==0 || x == COL-1 || y==1 || y==ROW-1) {
          ST->ConOut->SetCursorPosition(ST->ConOut, x, y);
          ST->ConOut->OutputString(ST->ConOut, u"#");
        }
    }
  }
  // Init snake
  snake[0].X=1;
  snake[0].Y=1;
  snake_size = 1;
}

VOID move_snake(INT8 dx, INT8 dy) {
  for (UINT16 i=snake_size-1; i>0; i--) {
    snake[i]=snake[i-1];
  }
  snake[0].X += dx;
  snake[0].Y += dy;
}


VOID read_keyboard() {
  EFI_KEY_DATA keyData;
  UINTN idx;
  ST->BootServices->WaitForEvent(1, &ST->ConIn->WaitForKeyEx, &idx);
  ST->ConIn->ReadKeyStrokeEx(ST->ConIn, &keyData);
  if (keyData.Key.UnicodeChar == u'w') {
    dir = UP;
  }
  else if (keyData.Key.UnicodeChar == u's') {
    dir = DOWN;
  }
  else if (keyData.Key.UnicodeChar == u'a') {
    dir = LEFT;
  }
  else if (keyData.Key.UnicodeChar == u'd') {
    dir = RIGHT;
  }
  else if (keyData.Key.UnicodeChar == u' ') {
    grow = 1;
  }
}


VOID draw_snake() {
  ST->ConOut->SetCursorPosition(ST->ConOut, snake[0].X, snake[0].Y);
  ST->ConOut->OutputString(ST->ConOut, u"@");
}

BOOLEAN snake_on_self() {
  if (snake_size > 1) {
    for (UINT16 i=1; i<snake_size; i++) {
      if (snake[0].X == snake[i].X && snake[0].Y == snake[i].Y)
        return 1;
    }
  }
  return 0;
}

VOID game_over() {
  while(1) {
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->SetAttribute(ST->ConOut, EFI_RED);
    ST->ConOut->OutputString(ST->ConOut, u"Game Over: Press a key to shutdown");
    UINTN idx;
    ST->BootServices->WaitForEvent(1, &ST->ConIn->WaitForKeyEx, &idx);
    ST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, 0);
  }
}


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
  // TODO: remove this line when using input params
  (void)ImageHandle;
  ST = SystemTable;

  init_board();
  //Snake pos can be within (1,1) and (78,22)
  while(1) {
    if (snake[0].X <=0 || snake[0].X >= COL-1 || snake[0].Y <= 0 || snake[0].Y >= ROW-1) {
      game_over();
    } 
    draw_snake();
    read_keyboard();
    if (grow) {
      snake_size++;
    }
    if (dir==UP) {
      if (!grow) {
        ST->ConOut->SetCursorPosition(ST->ConOut, snake[snake_size-1].X, snake[snake_size-1].Y);
        ST->ConOut->OutputString(ST->ConOut, u" ");
      }
      move_snake(0,-1);
    }
    if (dir==DOWN) {
      if (!grow) {
        ST->ConOut->SetCursorPosition(ST->ConOut, snake[snake_size-1].X, snake[snake_size-1].Y);
        ST->ConOut->OutputString(ST->ConOut, u" ");
      }
      move_snake(0,1);
    }
    if (dir==LEFT) {
      if (!grow) {
        ST->ConOut->SetCursorPosition(ST->ConOut, snake[snake_size-1].X, snake[snake_size-1].Y);
        ST->ConOut->OutputString(ST->ConOut, u" ");
      }
      move_snake(-1,0);
    }
    if (dir==RIGHT) {
      if (!grow) {
        ST->ConOut->SetCursorPosition(ST->ConOut, snake[snake_size-1].X, snake[snake_size-1].Y);
        ST->ConOut->OutputString(ST->ConOut, u" ");
      }
      move_snake(1,0);
    }
    grow = 0;
    if (snake_on_self()) {
      game_over();
    }
  }

  return EFI_SUCCESS;
}
