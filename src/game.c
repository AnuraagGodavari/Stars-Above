#include "game.h"

Game_Data gamedata = { 0 };

Bool clickable()
{
    return 0;
}

void game_loop()
{

}

int main(int argc, char* argv[])
{

    int done = 0;
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

    Entity* test;

    vector2d_copy(gamedata.screensize, vector2d(1280, 720));

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
    background = gf2d_sprite_load_image("resources/images/background/bg_base.png");

    mouse_idle = gf2d_sprite_load_all("resources/images/ui/cursor.png", 32, 32, 1);
    mouse_click = gf2d_sprite_load_all("resources/images/ui/cursor_click.png", 32, 32, 1);
    mouse_drag = gf2d_sprite_load_all("resources/images/ui/cursor_drag.png", 32, 32, 1);

    //Initialize Camera
    gamedata.camera = camera_init(vector2d(0, 0), gamedata.screensize, vector2d(2, 2));

    //Initialize Entity Manager
    entity_manager_init(100);

    //Initialize Font System
    font_init(50);

    parallax0 = parallax_init(gf2d_sprite_load_image("resources/images/background/bg_overlay.png"), vector2d(0, 0), 0.5);
    parallax1 = parallax_init(gf2d_sprite_load_image("resources/images/background/bg_overlay_02.png"), vector2d(0, 0), 0.2);

    /* entity test BEGIN*/
    test = entity_init
    (
        gf2d_sprite_load_image("resources/images/bibbutest.png"),
        vector2d(0,0),
        COLL_CIRCLE,
        0,
        0
    );
    /* entity test END*/

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

        entities_draw();//if (test != NULL) { entity_draw(test); }

        mouse_update(mouse_idle, mouse_click, mouse_drag);
        
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition

        while (SDL_PollEvent(&sdlevent))
        {
            if (sdlevent.type == SDL_QUIT)
            {
                done = 1;
            }

        }
        
        //slog("Rendering at %f FPS", gf2d_graphics_get_frames_per_second());
    }

    slog("---==== END ====---");

    entity_manager_free();

    return 0;
}

Bool mouse_clickable()
{

}

void mouse_update(Sprite* idle, Sprite* hover, Sprite* drag)
{

    int mx, my;

    int camx = 0, camy = 0; 

    SDL_GetMouseState(&mx, &my);

    Sprite* curr;

    //Move camera

    if (mx < gamedata.screensize.x * 0.04) { camx = -1; }

    if (mx > gamedata.screensize.x - (gamedata.screensize.x * 0.04)) { camx = 1;  }

    if (my < gamedata.screensize.y * 0.07) { camy = -1; }

    if (my > gamedata.screensize.y - (gamedata.screensize.y * 0.07)) { camy = 1;  }

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
