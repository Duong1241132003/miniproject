#ifndef SMART_PLAYLIST_H
#define SMART_PLAYLIST_H

#include "MusicLibrary.h"
#include "PlaybackQueue.h"

/*
 * Generates a smart playlist using BFS traversal.
 */
PlaybackQueue generateSmartPlaylist(
    Song& startSong,
    MusicLibrary& library,
    int maxSize
);

#endif
