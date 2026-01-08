#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <thread>
#include <atomic>

#include "MusicLibrary.h"
#include "PlaybackQueue.h"
#include "PlaybackHistory.h"
#include "PlayNextQueue.h"
#include "ShuffleManager.h"
#include "SmartPlaylist.h"
// #include "SmartPlaylist.h"

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
     */
    MusicPlayer();

    /*
     * Selects a song by ID and starts playback.
     */
    void selectAndPlaySong(int songID);

    /*
     * Play next song.
     */
    void playNext();

    /*
     * Play previous song (from history).
     */
    void playPrevious();

    /*
     * Add song (by ID) to Play Next queue.
     */
    void addSongToPlayNext(int songID);

    /*
     * Enable shuffle mode.
     */
    void enableShuffle();

    /*
     * Generate smart playlist using BFS.
     */
    void BFS(int startSongID, int maxSize);

    /*
     * Get library
     */
    MusicLibrary& getLibrary();

    /*
     * Get playbackQueue.
     */
    PlaybackQueue& getPlaybackQueue();

    /*
     * Get playNextQueue.
     */
    PlayNextQueue& getPlayNextQueue();

    /*
     * Get shuffleManager.
     */
    PlaybackHistory& getPlaybackHistory();

    /*
     * Set playbackQueue.
     */
    void setPlaybackQueue(PlaybackQueue& pb);

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
 * pause current song
 */
void pauseSong();
/*
 * Loads songs from a CSV file into the music library.
 * Expected CSV format
 * id,title,artist,album,duration,path
 */
void loadLibraryFromCSV(const std::string& filePath, MusicLibrary& library);

#endif
