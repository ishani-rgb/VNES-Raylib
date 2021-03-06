//UI variable (should be int or define ?)
#define screenWidth 800
#define screenHeight 450

enum MENU_ITEM_TYPE
{
  CHOICE_ITEM,
  SLIDER,
  CHECKBOX,
  INPUT,
  LIST,
  SCRIPT_RUNNER,
  MENU_NAV
};

// UI struct
typedef struct
{
  char *label;
  bool visible;
  enum MENU_ITEM_TYPE type;
  int param; //Change type later //Originally planned to be a function to call //Label index if 
  int *variable; //variable to edit
  int values[4];
  int (*function)(int);

} MENU_ITEM;

#define MAX_ITEMS_MENU_PAGE 10

typedef struct {
  char* title;
  bool visible;
  MENU_ITEM items[MAX_ITEMS_MENU_PAGE];
  //Backfunction ?
  //Page_destination ?
} MENU_PAGE;

typedef struct {
  int volume;
  int check;
  int cps; //
} OPTION_STRUCT;

Sound beep;

void playSomeSound(){
  PlaySound(beep);
}

OPTION_STRUCT OPTION ={
  .volume = 100,
  .check = 0,
  .cps = 1
};

typedef struct UI_IMG
{
  char* mainmenu_logo_file;
  char* textbox_file;
  char* ctc_file;
  Texture2D textbox;
  Texture2D mainmenu_logo;
  Texture2D ctc;

  Color ctc_color;
} UI_IMG;

UI_IMG UI_IMAGE;

void loadUI_Texture()
{
  char filename[32] = "./assets/img/";
  strcat(filename, UI_IMAGE.textbox_file);
  strcat(filename, ".png");

  UI_IMAGE.textbox = VN_LoadTexture("./assets/img/textbox.png");
  // UI_IMAGE.mainmenu_logo_name = "mainmenu_logo";

  strcpy(filename, "./assets/img/");
  strcat(filename, UI_IMAGE.mainmenu_logo_file);
  strcat(filename, ".png");

  UI_IMAGE.mainmenu_logo = VN_LoadTexture(filename);

  UI_IMAGE.ctc_file = "ctc";

  strcpy(filename, "./assets/img/");
  strcat(filename, UI_IMAGE.ctc_file);
  strcat(filename, ".png");

  UI_IMAGE.ctc = VN_LoadTexture(filename);
  // UI_IMAGE.ctc = VN_LoadTexture("./assets/img/ctc.png");
  UI_IMAGE.ctc_color = WHITE;
  
}

bool SAVEGAME();
bool LOADGAME();
void NEWGAME();

#define choice_menu_index 0
#define pause_menu_index 1
#define main_menu_index 3
MENU_PAGE ListMenuPage[]={
  {"CHOICE", false,
    .items = {
      {"BTN1",  false,CHOICE_ITEM, 0},
      {"BTN2",  false,CHOICE_ITEM, 0},
      {"BTN3",  false,CHOICE_ITEM, 0},
      {"BTN4",  false,CHOICE_ITEM, 0},
    }
  },
  {"PAUSE", false,
    .items = {
      {"Volume",  true,SLIDER, 0, &OPTION.volume,{0,100}},
      {"Mute",true,CHECKBOX, 0, &OPTION.check, {0,1}},

      {"CPS",  true,SLIDER, 0, &OPTION.cps,{0,120}},

      {"Autre menu",true,MENU_NAV, 2},
      {"SAVE",true,SCRIPT_RUNNER, .function=SAVEGAME},
      {"LOAD",true,SCRIPT_RUNNER, .function=LOADGAME},
    }
  },
  {"SUBMENU", false,
    .items = {
      {"BTN1",  true,CHOICE_ITEM, 0},
      {"Slider",  true,SLIDER, 0, &OPTION.volume,{0,100}},
      {"RETOUR",true,MENU_NAV, pause_menu_index},
    }
  },
  {" ", false,
    .items = {
      {"NEW GAME",  true,SCRIPT_RUNNER, .function=NEWGAME},
      {"LOAD GAME",true,SCRIPT_RUNNER, .function=LOADGAME}
    }
  },

};

bool inMenuChoice = false;
bool inMenuPause = false;

int MenuPageNumber = 3;


int UI_BOX_POSITION_X = screenWidth/2;
int UI_BOX_POSITION_Y = screenHeight/2;
int UI_BOX_WIDTH = 300;
int UI_BOX_HEIGHT = 300;

int UI_ITEMS_MARGIN = 10;

int UI_CHOICE_WIDTH = 300;
int UI_CHOICE_HEIGHT = 30;
int UI_CHOICE_FONT_SIZE = 20;
int UI_CHOICE_FONT_MARGIN_TOP = 5;

int UI_SLIDER_LABEL_MARGIN = 20;
int UI_SLIDER_MAIN_WIDTH = 175;
int UI_SLIDER_MAIN_HEIGHT = 10;
int UI_SLIDER_CURSOR_WIDTH = 10;
int UI_SLIDER_CURSOR_HEIGHT = 20;

int UI_CHECKBOX_LABEL_MARGIN = 20;
int UI_CHECKBOX_LABEL_MARGIN_RIGHT = 40;
int UI_CHECKBOX_MAIN_WIDTH = 30;
int UI_CHECKBOX_MAIN_HEIGHT = 30;

Color UI_TEXTBOX_NAME_COLOR = DARKGRAY;
Color UI_TEXTBOX_TEXT_COLOR = DARKGRAY;



int UI_TEXT_SPACING = 10; //Font size/default font size


void DrawUI(int pCursor){
  for (int i = 0; i < MenuPageNumber; i++)
  {
    if (i==pause_menu_index && inMenuPause){
      VN_DrawRectangle(0,0,screenWidth,screenHeight,(Color){0,0,0,150});
    }
    if (ListMenuPage[i].visible){
      if (i>choice_menu_index){
        VN_DrawText(ListMenuPage[i].title,screenWidth/2-VN_MeasureText(ListMenuPage[i].title, 30)/2,30,30,WHITE);
      }
      else{
        if (!inMenuChoice){ //if we are not in a choice, this menu shouldn't be visible
          ListMenuPage[choice_menu_index].visible = false;
          break;
        }
      }

      for (int i2 = 0; i2 < MAX_ITEMS_MENU_PAGE; i2++)
      {
        if (ListMenuPage[i].items[i2].visible==true)
        {
          Color box_color;
          switch (ListMenuPage[i].items[i2].type)
          {
          case MENU_NAV:
          case CHOICE_ITEM:
            box_color = LIGHTGRAY;
            if (pCursor==i2){
              box_color = GREEN;
            }

            VN_DrawRectangle(UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2,UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+((i2)*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN)),UI_CHOICE_WIDTH,UI_CHOICE_HEIGHT, box_color);
            VN_DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-VN_MeasureText(ListMenuPage[i].items[i2].label,UI_CHOICE_FONT_SIZE)/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            break;
          case SLIDER:
            box_color = LIGHTGRAY;
            if (pCursor==i2){
              box_color = GREEN;
            }
            VN_DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2+UI_SLIDER_LABEL_MARGIN,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, box_color);
            //Main
            VN_DrawRectangle(
                    UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2+UI_SLIDER_LABEL_MARGIN + VN_MeasureText(ListMenuPage[i].items[i2].label,UI_CHOICE_FONT_SIZE)+UI_SLIDER_LABEL_MARGIN,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP*2,
                    UI_SLIDER_MAIN_WIDTH,
                    UI_SLIDER_MAIN_HEIGHT,
                    box_color
            );
            //Curseur
            VN_DrawRectangle(
                    UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2+UI_SLIDER_LABEL_MARGIN + VN_MeasureText(ListMenuPage[i].items[i2].label,UI_CHOICE_FONT_SIZE)+UI_SLIDER_LABEL_MARGIN + ((*ListMenuPage[i].items[i2].variable)*UI_SLIDER_MAIN_WIDTH)/ListMenuPage[i].items[i2].values[1],
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP*2-UI_SLIDER_CURSOR_HEIGHT/2+UI_SLIDER_MAIN_HEIGHT/2,
                    UI_SLIDER_CURSOR_WIDTH,
                    UI_SLIDER_CURSOR_HEIGHT,
                    box_color
            );
            
            break;
          case CHECKBOX:
            box_color = LIGHTGRAY;
            if (pCursor==i2){
              box_color = GREEN;
            }
            VN_DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2+UI_CHECKBOX_LABEL_MARGIN,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, box_color);

            if (*ListMenuPage[i].items[i2].variable!=0){
            VN_DrawRectangle(
                    UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2+UI_CHECKBOX_LABEL_MARGIN + VN_MeasureText(ListMenuPage[i].items[i2].label,UI_CHOICE_FONT_SIZE)+UI_CHECKBOX_LABEL_MARGIN_RIGHT,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP*2-UI_CHECKBOX_MAIN_HEIGHT/2,
                    UI_CHECKBOX_MAIN_WIDTH,
                    UI_CHECKBOX_MAIN_HEIGHT,
                    box_color
            );
            }
            else{
            VN_DrawRectangleLines(
                    UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2+UI_CHECKBOX_LABEL_MARGIN + VN_MeasureText(ListMenuPage[i].items[i2].label,UI_CHOICE_FONT_SIZE)+UI_CHECKBOX_LABEL_MARGIN_RIGHT,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP*2-UI_CHECKBOX_MAIN_HEIGHT/2,
                    UI_CHECKBOX_MAIN_WIDTH,
                    UI_CHECKBOX_MAIN_HEIGHT,
                    box_color
            );
            }
            
            break;
          case INPUT:
            VN_DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-VN_MeasureText(ListMenuPage[i].items[i2].label,UI_CHOICE_FONT_SIZE)/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            
            break;
          case LIST:
            VN_DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-VN_MeasureText(ListMenuPage[i].items[i2].label,UI_CHOICE_FONT_SIZE)/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            
            break;
          case SCRIPT_RUNNER:
            box_color = LIGHTGRAY;
            if (pCursor==i2){
              box_color = GREEN;
            }
            VN_DrawRectangle(UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2,UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+((i2)*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN)),UI_CHOICE_WIDTH,UI_CHOICE_HEIGHT, box_color);
            VN_DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-VN_MeasureText(ListMenuPage[i].items[i2].label,UI_CHOICE_FONT_SIZE)/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            
            break;
          default:
            break;
          }
        }   

      }
    }
  }
};

int GetVisibleChoiceNumber(pPage){
  static int count;
  count = 0;

  for (int i = 0; i < MAX_ITEMS_MENU_PAGE; i++)
  {
    if (ListMenuPage[pPage].items[i].visible){count++;}
    else break; /*Break, because in no circonstence should we have an invisible choice in the middle
                (at least with the current implementation)*/
  }
  

  return count;
}
