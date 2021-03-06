/*******************************************************************************************
*
*   VNES - Raylib port
*
********************************************************************************************/

#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "NSTD_custom_lib.h"

#include "system/input_system.h"
#include "system/graphics_system.h"
#include "system/audio_system.h"
//#include "font_system.h" //(?? Or just manage that in the graphics system I guess)

#include "UI_DEFS.h"


//----Enums

enum GAME_STATE
{
  SPLASH_SCREEN,
  MAIN_MENU,
  DIAL,
  END
};
enum GAME_STATE game_st = MAIN_MENU;

//-----Variables utiles

unsigned int index = 0; //index //We absolutlty need to change the name before porting
int cursor = 1;
char disp_text[64]; //Current text being displayed
char *text_to_display; //Text to be displayed

int choice_sel;
int nb_choice;

char *chara_name = "Character Name";
char buffText[64];
char *first_word; //TODO:Replace by all of them by a char* array Word[]
char *second_word;
char *third_word;
char *fourth_word;


#define FRENCH 1 //bolean, 1 = FR 0 = EN
#include "script_parser.h"
#include "system/saveload_system.h"

void NEWGAME(){
  index = 0;
  game_st = DIAL;
  init_dial();
}

void ParseLabels()
{
  for (int i2 = 0; i2 < LABELS_NUMBERS; i2++)
  {
    ListLabels[i2].name = "null";
    ListLabels[i2].value = 0;
  }

  for (int i = 0; i < sizeof(SCRPT) / sizeof(SCRPT[0]); i++)
  {
    if (SCRPT[i].t == LABEL)
    {
      //L'ajouter à la liste, au premier endroit vide
      for (int i2 = 0; i2 < LABELS_NUMBERS; i2++)
      {
        if (!strcmp(ListLabels[i2].name,"null"))
        {
          ListLabels[i2].name = SCRPT[i].c;
          ListLabels[i2].value = i;
          break;
        }
      }
    }
  }
}

void loadCharacterSprites()
{
  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    //CharaList[i].image_name

    //Load base sprite
    char filename[32] = "./assets/img/";
    strcat(filename, CharaList[i].image_name);
    strcat(filename, " ");

    char base_filename[32];
    strcpy(base_filename, filename);
    strcat(base_filename, "base.png");

    char expression_filename[32];
    strcpy(expression_filename, filename);
    // strcat(expression_filename, "1.png");

    CharaList[i].base_image = VN_LoadTexture(base_filename);

    CharaList[i].y = screenHeight - CharaList[i].base_image.height;
    CharaList[i].x = (screenWidth - CharaList[i].base_image.width)/2; //Center of the screen
    CharaList[i].gotox = CharaList[i].x;
    CharaList[i].gotoy = CharaList[i].y;

    char buff[1];

    //Load expressions
    for (int i2 = 0; i2 < MAX_EXPRESSION; i2++)
    {
      itoa(i2 + 1, buff, 10);
      char file_expression[32];
      strcpy(file_expression, expression_filename);
      strcat(file_expression, buff);
      strcat(file_expression, ".png");
      // printf(file_expression);
      CharaList[i].expression[i2] = VN_LoadTexture(file_expression);
    }

    CharaList[i].expression_index = 0; //default

    if (CharaList[i].color_name.a==0) //If the color is transparent, we asume it has not be defined
    {
      CharaList[i].color_name = DARKGRAY;
    }
  }
}

void loadBackgroundSprites()
{
  for (int i=0; i<MAX_BACKGROUND; i++){
    char filename[32] = "./assets/img/";
    strcat(filename, Background.images[i]);
    strcat(filename, ".png");

    Background.texture[i] = VN_LoadTexture(filename);
  }
  
  Background.bg_index = 0; //default
  Background.bg_next = 1;
  Background.opacity = 250;
  Background.dissolve_duration = 0.5;

}

void loadAudio(){
  //Load Music
  //VN_LoadMusicStream(path)

  
  
  for (int i=0; i<MAX_MUSIC; i++)
  {
    if (MusicList.music_name[i])
    {
      char filename[64] = "./assets/audio/music/";
      // strcat(filename, "./assets/audio/music/");
      strcat(filename, MusicList.music_name[i]);
      strcat(filename, ".mp3");

      MusicList.music_list[i] = VN_LoadMusicStream(filename);
    }
  }

  MusicList.music_playing = 0;
  

  // //Load Sounds
  // //VN_LoadSound(path)
  
  for (int i=0; i<MAX_SOUND; i++)
  {
    if (SoundList.sound_name[i])
    {
      char filename[64] = "./assets/audio/sound/";
      // strcat(filename, "./assets/audio/sound/");
      strcat(filename, SoundList.sound_name[i]);
      strcat(filename,".wav");

      SoundList.sound_list[i] = VN_LoadSound(filename);
    }
  }

  MusicList.music_playing = 0;


}

void DrawBackground()
{
  if (Background.in_transition)
  {
    if (Background.opacity>0)
    {
      Background.opacity -= min(Background.opacity, (int)(255/Background.dissolve_duration)*GetFrameTime()); //C'est linéaire mais mieux que rien

    }
    else {
      // playSomeSound();
      Background.in_transition = false;
      Background.bg_index = Background.bg_next; //Swap !
      Background.opacity = 255;
    }
  }


  VN_DrawTexture(Background.texture[Background.bg_next],0,0,WHITE);
  VN_DrawTexture(Background.texture[Background.bg_index],0,0,(Color){255,255,255,Background.opacity});
}

void draw_dial()
{
  DrawBackground();
  

  //Draw characters on the screen
  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    if (CharaList[i].visible)
    { //will change later
      VN_DrawTexture(CharaList[i].base_image, CharaList[i].x, CharaList[i].y, WHITE);
      VN_DrawTexture(CharaList[i].expression[CharaList[i].expression_index], CharaList[i].x, CharaList[i].y, WHITE);
    }
  }

  //DRAW Text & Textbox


  if (!inMenuChoice)
  {
    VN_DrawTexture(UI_IMAGE.textbox,(screenWidth-UI_IMAGE.textbox.width)/2,315,WHITE);

    VN_DrawText(chara_name, 163+10, 315+5, 25, UI_TEXTBOX_NAME_COLOR);
    VN_DrawText(disp_text, 163+10, 315+30, 20, UI_TEXTBOX_TEXT_COLOR);
    // DrawTextEx(GetFontDefault(),disp_text,(Vector2){163+20,315+40},(float)20,(float)1,DARKGRAY);

    if (cursor >= strlen(text_to_display))
    {
      UI_IMAGE.ctc_color = UI_TEXTBOX_NAME_COLOR;
      VN_DrawTexture(UI_IMAGE.ctc, (screenWidth+UI_IMAGE.textbox.width)/2 - UI_IMAGE.ctc.width - 20,390,UI_IMAGE.ctc_color);
    }

  }

  DrawUI(choice_sel);
  char buffer1[20];
  VN_DrawText(itoa(index,buffer1,10),10,100,20,BLACK);
}

void updt_dial()
{
  static float timer;
  float delta_time;
  timer += GetFrameTime();
  delta_time = GetFrameTime();

  //"Tweening"
  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    if (CharaList[i].x != CharaList[i].gotox)
    {
      CharaList[i].x += (CharaList[i].gotox - CharaList[i].x) / 10;

      //Todo : have some real tweening going on
    }
  }

  if (SCRPT[index].t == A || SCRPT[index].t == N)
  {
    if (!inMenuPause)
    {
      if (BTNP("A"))
      {
        if (cursor < strlen(text_to_display))
        {
          cursor = strlen(text_to_display);
        } //display the whole text
        else
        {
          if (index < sizeof(SCRPT) / sizeof(SCRPT[0]) - 1)
          {
            index++;
            cursor = 0;
            timer = 0;

            memset(disp_text, 0, 64); //Vider le string

            init_dial();
          }
          else
          {
            game_st = END;
            cursor = 0;
          }
        }
      }

      if (cursor < strlen(text_to_display))
      {
        if (OPTION.cps==0)
        {
          cursor = strlen(text_to_display);
        }
        else if (timer*OPTION.cps>=1)
        {
          cursor += (int)(timer*OPTION.cps);
          if (cursor >= strlen(text_to_display)) {cursor = strlen(text_to_display);}
          timer=0;
        }
      }
      strncpy(&disp_text, text_to_display, cursor);
    }
  }

  //MENU MANAGER
  if (BTNP("START"))
  {
    // playSomeSound();
    choice_sel = 0;
    if (inMenuPause)
    {
      ListMenuPage[pause_menu_index].visible = false;
      for (int i = pause_menu_index; i < MenuPageNumber; i++)
      {
        ListMenuPage[i].visible = false;
      }

      timer=0;

      SAVECONFIG();
    }
    else
    {
      ListMenuPage[pause_menu_index].visible = true;
    }
    inMenuPause = !inMenuPause;

    if (inMenuPause)
    {
      nb_choice = GetVisibleChoiceNumber(pause_menu_index); //Ugly but at least it doesn't crash
      //Check how many visible choice exist
      //Maybe do the same for the choice for consistency sake
    }
    else
    {
      // nb_choice = 0; //reset, if C: it will be handled in the parser
      init_dial();
    }
  }

  if (inMenuChoice || inMenuPause)
  {
    //Handle Input when in menu
    //Up & Down
    if (BTNP("DOWN"))
    {
      if (choice_sel < nb_choice - 1)
      {
        choice_sel++;
      }
      else{choice_sel=0;}
    }
    if (BTNP("UP"))
    {
      if (choice_sel > 0)
      {
        choice_sel--;
      }
      else{choice_sel=nb_choice;}
    }
  }

  if (inMenuChoice && !inMenuPause)
  {
    //Press A in choice
    if (BTNP("A"))
    {
      index = ListMenuPage[choice_menu_index].items[choice_sel].param;
      timer=0;

      inMenuChoice = false;
      ListMenuPage[choice_menu_index].visible = false;
      for (int i2 = 0; i2 < MAX_ITEMS_MENU_PAGE; i2++)
      {
        ListMenuPage[choice_menu_index].items[i2].visible = false;
      }

      choice_sel = 0;
      init_dial();
    }
  }

  if (inMenuPause)
  {
    // CHOICE_ITEM,
    // SLIDER,
    // CHECKBOX,
    // INPUT,
    // LIST,
    // SCRIPT_RUNNER,
    // MENU_NAV

    static MENU_ITEM item_menu;
    for (int i = pause_menu_index; i < MAX_ITEMS_MENU_PAGE; i++)
    {
      if (ListMenuPage[i].visible)
      {
        item_menu = ListMenuPage[i].items[choice_sel];

        switch (item_menu.type)
        {
        case CHECKBOX:
        {
          if (BTNP("A"))
          {
            *item_menu.variable = !*item_menu.variable;
          }
          break;
        }
        case SLIDER:
        {
          if (BTN("LEFT"))
          {
            if (*item_menu.variable - 1 >= item_menu.values[0])
            {
              *item_menu.variable -= 1;
            }
          }
          else if (BTN("RIGHT"))
          {
            if (*item_menu.variable + 1 <= item_menu.values[1])
            {
              *item_menu.variable += 1;
            }
          }
          break;
        }
        case SCRIPT_RUNNER:
          if (BTNP("A"))
          {
            item_menu.function(item_menu.variable); //It's super hacky, but I love it
          }
          break;
        case MENU_NAV:
          if (BTNP("A"))
          {
            // item_menu.function(item_menu.variable); //It's super hacky, but I love it
            ListMenuPage[item_menu.param].visible = true;
            ListMenuPage[i].visible = false;
            choice_sel = 0;
            nb_choice = GetVisibleChoiceNumber(item_menu.param);
            playSomeSound();
          }
          break;
        case INPUT:
        {
          if (BTN("LEFT"))
          {
            if (*item_menu.variable - 1 >= item_menu.values[0])
            {
              *item_menu.variable -= 1;
            }
          }
          else if (BTN("RIGHT"))
          {
            if (*item_menu.variable + 1 <= item_menu.values[1])
            {
              *item_menu.variable += 1;
            }
          }
          break;
        }
        }
      }
    }
  }
};

void draw_menu()
{
  static float time;
  time += GetFrameTime();

  int logoX;
  int logoY;

  logoX = screenWidth/2-UI_IMAGE.mainmenu_logo.width/2;
  logoY = screenHeight/2-UI_IMAGE.mainmenu_logo.height/2;
  
  VN_DrawTexture(UI_IMAGE.mainmenu_logo,logoX,logoY+sin(time)*10, WHITE);
  

  VN_DrawText("PRESS A TO START", screenWidth/2 - VN_MeasureText("PRESS A TO START",30)/2,400,30,(Color){0,0,0,(sin(time*2)+1)*255/2});

  DrawUI(choice_sel);
}

void updt_menu()
{
  if (BTNP("A"))
  {
    // if (ListMenuPage[main_menu_index].visible==false)
    // {
    //   playSomeSound();
    //   ListMenuPage[main_menu_index].visible = true;
    // }
    // if (LOADGAME()){
      
    // }
    NEWGAME();
  }
}

void draw_end()
{
  //draw_dial();
  //   vrambuf_put(NTADR_A(14,10),"FIN",3);
  //   vrambuf_put(NTADR_A(5,20),"Merci d'avoir joue! *",21);
}

void updt_end()
{
  if (BTNP("A"))
  {
    game_st = MAIN_MENU;
    index = 0;
  }
}

int main(int argc, char *argv[])
{
  // Initialization
  //--------------------------------------------------------------------------------------

  InitWindow(screenWidth, screenHeight, "VNES_PC");
  VN_InitAudioDevice();

  Music music_test;
  // music_test = VN_LoadMusicStream("./assets/audio/music/nichijou1.mp3");
  beep = VN_LoadSound("./assets/audio/sound/beep1.wav");
  Text_font = VN_LoadFont("./assets/font/Ubuntu.ttf");

  LOADCONFIG();

  loadCharacterSprites();
  loadBackgroundSprites();
  loadUI_Texture();
  loadAudio();

  // PlayMusicStream(MusicList.music_list[0]);

  ParseLabels();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
  init_dial();

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    

    switch (game_st)
    {
    case MAIN_MENU:
    {
      updt_menu();

      break;
    }
    case DIAL:
    {
      updt_dial();
      
      VN_SetMusicVolume(MusicList.music_list[0], (float)OPTION.volume/2/100);

      if (!OPTION.check){
        VN_UpdateMusicStream(MusicList.music_list[MusicList.music_playing]);
      }

      break;
    }
    case END:
    {
      
      updt_end();

      break;
    }
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch (game_st)
    {
    case MAIN_MENU:
    {
      draw_menu();
      break;
    }
    case DIAL:
    {
      draw_dial();

      break;
    }
    case END:
    {
      draw_end();

      break;
    }
    }

    VN_DrawFPS(10, 10);

    if (a_pressed)
    {
      VN_DrawText("A", 200, 10, 20, DARKGRAY);
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------

  //Textures
  for (int i = 0; i < CHARACTER_NUMBER; i++)
  {
    VN_UnloadTexture(CharaList[i].base_image);
    for (int i2 = 0; i2 < MAX_EXPRESSION; i2++)
    {
      VN_UnloadTexture(CharaList[i].expression[i2]);
    }
  }

  for (int i = 0; i < MAX_BACKGROUND; i++)
  {
    VN_UnloadTexture(Background.texture[i]);
  }

  //Sound/Musics
  for (int i = 0; i < MAX_MUSIC; i++)
  {
    VN_UnloadMusicStream(MusicList.music_list[i]);
  }

  for (int i = 0; i < MAX_SOUND; i++)
  {
    VN_UnloadSound(SoundList.sound_list[i]);
  }

  VN_UnloadFont(Text_font);

  CloseWindow(); // Close window and OpenGL context
  CloseAudioDevice(); 
  //--------------------------------------------------------------------------------------

  return 0;
}
