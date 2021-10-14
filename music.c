/*
 * music.c
 *
 *  Created on: Dec 9, 2019
 *      Author: JSchirm, CStahl, AGoff, HGhitan, RWillis
 */
#include "open_interface.h"
#include "music.h"


void load_songs() {

    unsigned char mario2NumNotes = 16;
    unsigned char mario2Notes[65]    = {50, 52, 54, 55, 57, 58, 59, 59, 59, 59, 59, 55, 64, 63, 64, 0, 55, 57, 59, 60, 62, 64, 63, 65, 64,  0, 55, 62, 61, 62,  0, 55, 57, 59, 60, 61, 62, 55, 65, 64,  0, 55, 67, 67, 67, 67, 69,  0, 67, 65, 65, 65, 65, 67,  0, 65, 64, 57, 59, 65, 64, 64, 64, 59, 60 };
    unsigned char mario2Duration[65] = {18, 18, 18, 18, 18, 18,  9,  9, 18, 18, 45,  9, 54, 54, 54, 9,  9,  9,  9,  9,  9, 54, 36, 18, 54, 45,  9, 54, 54, 54,  9,  9,  9,  9,  9,  9, 54, 36, 18, 54, 45,  9, 54, 54, 54, 18, 18,  9,  9, 54, 54, 54, 18, 18,  9,  9, 54, 18, 18, 18,  9,  9, 27,  9, 54 };
    oi_loadSong(1, mario2NumNotes, mario2Notes, mario2Duration);
}
