#ifndef SMART_PLAYLIST_H
#define SMART_PLAYLIST_H

#include <string>
#include "MusicLibrary.h"
#include "PlaybackQueue.h"

/*
 * Adds all songs belonging to a specific album
 * from the music library to the playback queue.
 */
void addAlbumToQueue(const std::string& albumName,
                     const MusicLibrary& library,
                     PlaybackQueue& queue);

#endif
