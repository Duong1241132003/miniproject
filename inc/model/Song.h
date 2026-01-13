#ifndef SONG_H
#define SONG_H

#include <string>

/*
 * Represents a single audio track in the system.
 * This struct only stores metadata and does not contain playback logic.
 */
struct Song
{
    int id {};                  /* identifier for the song */
    std::string title;          /* song title */
    std::string artist;         /* artist name */
    std::string album;          /* album name */
    int duration {};            /* in seconds */
    std::string path;           /* file path */
};

#endif
