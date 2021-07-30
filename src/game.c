#include "game.h"

Game_Data gamedata = { 0 };

int done = 0;

void game_load(char* savefile)
{

}

UI_State* test_ui(void* self, Game_Event* gameEvent_prev)
{
    UI_State* test_uiState;
    UI_Arrangement* test_uiArr;
    Entity* test_uiObj;

    time_t t;
    srand((unsigned)time(&t));

    /* ui test BEGIN*/
    
    //Create new UI State
    test_uiState = ui_state_new(
        5,
        NULL,
        test_ui,
        NULL,
        test_ui
    );

    test_uiState->prev_game_event = gameEvent_prev;

    //Add first button
    test_uiObj = entity_init
    (
        gf2d_sprite_load_image("assets/images/ui/textbox_brown.png"),
        vector2d(10, 10),
        COLL_BOX,
        0,
        0
    );

    entity_add_clickevent(test_uiObj, game_event_new(
        test_uiObj,
        NULL,
        (int)command_game_TEST,
        0,
        NULL,
        test_uiState,
        test_ui
    ));

    //Create UI Arr with first button
    test_uiArr = ui_arr_new(
        ui_object_new(
            test_uiObj
        ),
        0,
        10
    );

    ui_state_pushback(test_uiState, test_uiArr);

    entity_add_text(test_uiArr->top->ent, "TESTING ONE TWO THREE", font_load("assets/fonts/futura light bt.ttf", 16));

    //Add second button
    test_uiObj = entity_init
    (
        gf2d_sprite_load_image("assets/images/ui/textbox_brown.png"),
        vector2d(0, 0),
        COLL_BOX,
        0,
        0
    );

    char temp[128];
    sprintf(temp, "RANDOM NUMBER %d", rand() % 5000);

    entity_add_text(test_uiObj, temp, font_load("assets/fonts/futura light bt.ttf", 16));

    entity_add_clickevent(test_uiObj, game_event_new(
        test_uiObj,
        NULL, 
        (int)command_game_TEST,
        0, 
        NULL,
        test_uiState,
        test_ui
    ));

    ui_arr_add(
        test_uiArr,
        ui_object_new(
            test_uiObj
        )
    );

    return test_uiState;
}

Bool clickable()
{
    return 0;
}

void game_loop()
{

}

int main(int argc, char* argv[])
{
    const Uint8* keys;

    SDL_Event sdlevent;

    Sprite* background;

    Parallax_Layer* parallax0;
    Parallax_Layer* parallax1;

    Sprite* mouse;

    Sprite* mouse_idle;
    Sprite* mouse_click;
    Sprite* mouse_drag;

    Vector4D mouseColor = { 255,100,255,200 };

    vector2d_copy(gamedata.screensize, vector2d(1600, 900));

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Stars Above",
        gamedata.screensize.x,
        gamedata.screensize.y,
        gamedata.screensize.x,
        gamedata.screensize.y,
        vector4d(0, 0, 0, 255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    /*demo setup*/
    background = gf2d_sprite_load_image("assets/images/background/bg_1600_900.png");

    mouse_idle = gf2d_sprite_load_all("assets/images/ui/cursor.png", 32, 32, 1);
    mouse_click = gf2d_sprite_load_all("assets/images/ui/cursor_click.png", 32, 32, 1);
    mouse_drag = gf2d_sprite_load_all("assets/images/ui/cursor_drag.png", 32, 32, 1);

    //Initialize Camera
    camera_init(vector2d(0, 0), gamedata.screensize, vector2d(2, 2));

    //Initialize Entity Manager
    entity_manager_init(100, 100);

    //Initialize Font System
    font_init(50);

    parallax0 = parallax_init(gf2d_sprite_load_image("assets/images/background/bg_overlay.png"), vector2d(0, 0), 0.5);
    parallax1 = parallax_init(gf2d_sprite_load_image("assets/images/background/bg_overlay_02.png"), vector2d(0, 0), 0.2);

   // gamedata.uiState_curr = test_ui(NULL, NULL);

    /*main game loop*/
    while (!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
        gf2d_sprite_draw_image(background, vector2d(0, 0));

        camera_parallax_draw(parallax0);
        camera_parallax_draw(parallax1);

        game_loop();

        all_entities_draw();//if (test != NULL) { entity_draw(test); }

        mouse_update(mouse_idle, mouse_click, mouse_drag);
        
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition

        //Poll SDL Events
        sdl_event();
        
        //slog("Rendering at %f FPS", gf2d_graphics_get_frames_per_second());
    }

    slog("---==== END ====---");

    entity_manager_free();

    return 0;
}

void game_recieve_event(Game_Event* gameEvent)
{

    if (!gameEvent)
    {
        slog("Cannot recieve NULL Game Event"); return;
    }

    if (gameEvent->event_command == (int)command_game_TEST)
    {
        slog("GAME EVENT COMMAND == command_game_TEST");
    }
}

void game_set_ui_state(UI_State* ui_state)
{
    if (!ui_state)
    {
        slog("Cannot set game's current UI_State to NULL state"); return;
    }

    gamedata.uiState_curr = ui_state;
}

void sdl_event()
{
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {

        switch (sdl_event.type) {

        case SDL_QUIT:
            done = 1;
            break;

        case SDL_MOUSEBUTTONDOWN:

            if (sdl_event.button.button == SDL_BUTTON_LEFT)
            {
                if (gamedata.hovering_ent)
                {
                    entity_onClick(gamedata.hovering_ent);
                }
            }

            break;

        case SDL_KEYDOWN:

            if (sdl_event.key.keysym.sym == SDLK_BACKSPACE)
            {
                if (gamedata.uiState_curr)
                    gamedata.uiState_curr = previous_ui_state(gamedata.uiState_curr);
            }

        }
    }
}

void mouse_update(Sprite* idle, Sprite* hover, Sprite* drag)
{

    int mx, my;

    int camx = 0, camy = 0; 

    SDL_GetMouseState(&mx, &my);

    Sprite* curr;

    //Move camera

    if (mx < gamedata.screensize.x * 0.01) { camx = -1; }

    if (mx > gamedata.screensize.x - (gamedata.screensize.x * 0.01)) { camx = 1;  }

    if (my < gamedata.screensize.y * 0.02) { camy = -1; }

    if (my > gamedata.screensize.y - (gamedata.screensize.y * 0.02)) { camy = 1;  }

    camera_move(vector2d(camx, camy));

    //slog("%f, %f", camera_get()->pos.x, camera_get()->pos.y);

    gamedata.hovering_ent = entities_clickable(vector2d(mx, my));

    if (gamedata.hovering_ent != NULL)
    {
        curr = hover;
    }

    else
    {
        curr = idle;
    }

    //Draw
    gf2d_sprite_draw(
        curr,
        vector2d(mx, my),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        0);
}

void process_keys()
{

}
