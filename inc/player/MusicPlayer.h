#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <mmsystem.h>

#include "MusicLibrary.h"
#include "PlaybackQueue.h"
#include "PlaybackHistory.h"
#include "PlayNextQueue.h"

/*
 * MusicPlayer
 * -----------
 * Central controller
 */
class MusicPlayer
{
public:
    /*
     * Constructor
     * Loads the music library from CSV.
     */
    MusicPlayer();

    /*
     * Selects a song by ID and starts playback.
     */
    void selectAndPlaySong(int songID);

    /*
     * Adds a song to the playNextQueue.
     */
    void addSongToPlayNext(const Song& song);

    /*
     * Advances playback to the next song.
     * Priority order:
     * 1. PlayNextQueue
     * 2. Main PlaybackQueue
     */
    void playNext();

    /*
     * Get library
     */
    MusicLibrary& getLibrary();

    /*
     * Set playbackQueue.
     */
    void setPlaybackQueue(PlaybackQueue pb);

private:
    /*
     * Music library containing all songs.
     */
    MusicLibrary library;

    /*
     * Main playback queue.
     */
    PlaybackQueue playbackQueue;

    /*
     * Priority queue for managing next songs.
     */
    PlayNextQueue playNextQueue;

    /*
     * Playback history for "Back" functionality.
     */
    PlaybackHistory playbackHistory;

    /*
     * Currently playing song.
     */
    Song currentSong;

    /*
     * Indicates whether a song is currently playing.
     */
    bool hasCurrentSong {false};
};

/*
 * Simulates playing a song.
 * This is a placeholder for real audio playback logic.
 */
void playSong(const Song& song);

/*
 * Loads songs from a CSV file into the music library.
 * Expected CSV format
 * id,title,artist,album,duration,path
 */
void loadLibraryFromCSV(const std::string& filePath, MusicLibrary& library);

#endif
