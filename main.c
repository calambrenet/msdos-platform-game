#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

#define FRAMES_PER_SECOND 30

#define DIRECTION_LEFT 1
#define DIRECTION_RIGHT 2


/* set up a timer for animation */
volatile int ticks = 0;
void ticker(void)
{
    ticks++;
}
END_OF_FUNCTION(ticker)


BITMAP *buffer;
BITMAP *sprite_buffer;
PALETTE palette;

int frame, frame_number = 0;

void animate(void)
{
    while (frame > ticks) {
       rest(1000 / FRAMES_PER_SECOND);
    }

    frame++;

    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    clear_bitmap(buffer);

    if (frame_number == 9)
        frame_number = 0;
    else
        frame_number++;
}


int main(int argc, char *argv[])
{
    if (allegro_init() != 0)
        return 1;

    install_keyboard(); 
    install_timer();
    LOCK_FUNCTION(ticker);
    LOCK_VARIABLE(ticks);
    install_int_ex(ticker, BPS_TO_TIMER(30));

    /* set a graphics mode sized 320x200 */
    if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
        allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
        return 1;
    }

    sprite_buffer = load_bitmap("data/cat.pcx", palette);
    if (sprite_buffer == NULL) {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Unable to load data/cat.pcx\n");
        return 1;
    }

    clear_keybuf();

    buffer = create_bitmap(SCREEN_W, SCREEN_H);
        //si el gato está saltando, aplicar la física
        //la velocidad inicial se reduce por la gravedad
        //cuando la velocidad es 0, el gato cae a la posición inicial. Para eso jumpMode se pone en 2
        //ya que es el modo de bajada. El gato cae hasta la posición inicial y se detiene. Entonces jumpMode se pone en 0
    int initialPosX = SCREEN_W/2 - sprite_buffer->w/2;
    int initialPosy = SCREEN_H - sprite_buffer->h;
    int initialJumpForce = 10;
    int initialVelocity = 6;

    int posx = initialPosX;
    int posy = initialPosy;

    int jumpForce = initialJumpForce;
    int jumpMode = 0;
    int gravity = 1;
    int velocity = 0; //factor de velocidad de todos los movimientos del gato
    int verticalMovement = 0;
    int direction = DIRECTION_RIGHT;

    do {

        //mover el gato usando las teclas de dirección
        if (key[KEY_UP]) {
            posy -= 1;
        }
        if (key[KEY_DOWN]) {
            posy += 1;
        }
        if (key[KEY_LEFT]) {
            verticalMovement = 1;
            velocity = initialVelocity;
            posx -= 1;
            direction = DIRECTION_LEFT;
        }
        if (key[KEY_RIGHT]) {
            verticalMovement = 2;
            velocity = initialVelocity;
            posx += 1;
            direction = DIRECTION_RIGHT;
        }

        if(verticalMovement != 0 && velocity > 0) {
            velocity -= 1;
            if(verticalMovement == 1)
                posx -= 1 * velocity;
            else
                posx += 1 * velocity;

            if (velocity < 0) {
                velocity = 0;
                verticalMovement = 0;
            }
        }

        if (key[KEY_SPACE] && jumpMode == 0) {
            jumpMode = 1;
            jumpForce = initialJumpForce;
        }

        if (jumpMode == 1) {
            posy -= jumpForce;
            jumpForce -= gravity;
            if (jumpForce == 0) {
                jumpMode = 2;
            }
        } else if (jumpMode == 2) {
            posy += jumpForce;
            jumpForce += gravity;
            if (posy >= initialPosy) {
                posy = initialPosy;
                jumpMode = 0;
            }
        }

        clear_to_color(buffer, makecol(0, 0, 0));

        if(direction == DIRECTION_LEFT)
            draw_sprite(buffer, sprite_buffer, posx, posy);
        else if(direction == DIRECTION_RIGHT)
            draw_sprite_h_flip(buffer, sprite_buffer, posx, posy);

        char text[100];
        sprintf(text, "Posición: %d, %d, V: %d", posx, posy, velocity);
        textout_centre_ex(buffer, font, text, SCREEN_W/2, 0, makecol(255,255,255), -1);

        animate();

    } while ((!key[KEY_ESC]));

    destroy_bitmap(buffer);

    release_screen();

    return 0;
}

END_OF_MAIN()
