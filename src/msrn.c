#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include <types.h>

#include "graphics/title_tileset.h"
#include "graphics/title_tilemap.h"
#include "graphics/stage_tileset.h"
#include "graphics/stage_tilemap.h"
#include "graphics/puzzle_hi_tileset.h"
#include "graphics/puzzle_hi_tilemap.h"
#include "puzzle/puzzle.h"
#include "puzzle/puzzle_stage.h"
#include "puzzle/puzzle_view.h"

void main( void ) {
//    printf( "Hello World!" );
    HIDE_BKG;
//    set_bkg_data(0, 228, TITLE_TILESET);
//    set_bkg_tiles(0, 0, TITLE_TILEMAP_WIDTH, TITLE_TILEMAP_HEIGHT, TITLE_TILEMAP);

    initView();
    
    SHOW_BKG;
    SHOW_SPRITES;

    BOOLEAN isRunning = TRUE;
    while (isRunning) {
        wait_vbl_done();

        updateView();
        drawView();
    }

//    CRITICAL {
//        add_SIO(nowait_int_handler);
//    }
//    set_interrupts(SIO_IFLAG);
}