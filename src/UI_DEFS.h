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
  int *variable;
  int values[4];
} MENU_ITEM;

#define MAX_ITEMS_MENU_PAGE 10

typedef struct {
  char* title;
  bool visible;
  MENU_ITEM items[MAX_ITEMS_MENU_PAGE];
  //Backfunction ?
  //Page_destination ?
} MENU_PAGE;

int volumeTest = 50;  //[0;100]
int checkTest = 0;    //[0;1]

#define choice_menu_index 0
#define pause_menu_index 1
MENU_PAGE ListMenuPage[]={
  {"CHOIX", false,
    .items = {
      {"BTN1",  false,CHOICE_ITEM, 0},
      {"BTN2",  false,CHOICE_ITEM, 0},
      {"BTN3",  false,CHOICE_ITEM, 0},
      {"BTN4",  false,CHOICE_ITEM, 0},
    }
  },
  {"Test", false,
    .items = {
      {"Button",  true,CHOICE_ITEM, 0},
      {"Slider",  true,SLIDER,        0,0,{0,100}},
      {"CheckBox",true,CHECKBOX,      0,0, {0,1}},
      {"Function",true,SCRIPT_RUNNER, 0,0, {0,1}},
    }
  }  
};

bool inMenuChoice = false;
bool inMenuPause = false;

int MenuPageNumber = 2;


int UI_BOX_POSITION_X = screenWidth/2;
int UI_BOX_POSITION_Y = screenHeight/2;
int UI_BOX_WIDTH = 300;
int UI_BOX_HEIGHT = 300;

int UI_ITEMS_MARGIN = 10;

int UI_CHOICE_WIDTH = 300;
int UI_CHOICE_HEIGHT = 30;
int UI_CHOICE_FONT_SIZE = 20;
int UI_CHOICE_FONT_MARGIN_TOP = 5;

int UI_TEXT_SPACING = 10; //Font size/default font size


void DrawUI(int pCursor){
  for (int i = 0; i < MenuPageNumber; i++)
  {
    if (ListMenuPage[i].visible){
      if (i==pause_menu_index){
        DrawRectangle(0,0,screenWidth,screenHeight,(Color){0,0,0,150});
      }
      for (int i2 = 0; i2 < MAX_ITEMS_MENU_PAGE; i2++)
      {
        if (ListMenuPage[i].items[i2].visible==true)
        {
          Color box_color;
          switch (ListMenuPage[i].items[i2].type)
          {
          case CHOICE_ITEM:
            box_color = LIGHTGRAY;
            if (pCursor==i2){
              box_color = GREEN;
            }

            DrawRectangle(UI_BOX_POSITION_X-UI_CHOICE_WIDTH/2,UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+((i2)*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN)),UI_CHOICE_WIDTH,UI_CHOICE_HEIGHT, box_color);
            DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-(strlen(ListMenuPage[i].items[i2].label)*(UI_CHOICE_FONT_SIZE-UI_TEXT_SPACING))/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            break;
          case SLIDER:
            box_color = LIGHTGRAY;
            if (pCursor==i2){
              box_color = GREEN;
            }
            DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-(strlen(ListMenuPage[i].items[i2].label)*(UI_CHOICE_FONT_SIZE-UI_TEXT_SPACING))/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            
            break;
          case CHECKBOX:
            DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-(strlen(ListMenuPage[i].items[i2].label)*(UI_CHOICE_FONT_SIZE-UI_TEXT_SPACING))/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            
            break;
          case INPUT:
            DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-(strlen(ListMenuPage[i].items[i2].label)*(UI_CHOICE_FONT_SIZE-UI_TEXT_SPACING))/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            
            break;
          case LIST:
            DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-(strlen(ListMenuPage[i].items[i2].label)*(UI_CHOICE_FONT_SIZE-UI_TEXT_SPACING))/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            
            break;
          case SCRIPT_RUNNER:
            DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-(strlen(ListMenuPage[i].items[i2].label)*(UI_CHOICE_FONT_SIZE-UI_TEXT_SPACING))/2,
                    UI_BOX_POSITION_Y-UI_CHOICE_HEIGHT/2+(i2*(UI_CHOICE_HEIGHT+UI_ITEMS_MARGIN))+UI_CHOICE_FONT_MARGIN_TOP,
                    UI_CHOICE_FONT_SIZE, DARKGRAY);
            
            break;
          case MENU_NAV:
            DrawText(ListMenuPage[i].items[i2].label,
                    UI_BOX_POSITION_X-(strlen(ListMenuPage[i].items[i2].label)*(UI_CHOICE_FONT_SIZE-UI_TEXT_SPACING))/2,
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
