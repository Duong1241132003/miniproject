#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <string>
#include <iostream>

#include "MusicLibrary.h"
#include "PlaybackQueue.h"
#include "PlaybackHistory.h"
#include "PlayNextQueue.h"
#include "ShuffleManager.h"
#include "SmartPlaylist.h"

/*
 * Class: MusicPlayer
 * ------------------
 * Acts as the central controller for the application.
 * Manages the logic between the Music Library, Playback Queues,
 * History, and the Audio Thread.
 */
class MusicPlayer
{
private:
    /* The central repository of all loaded songs. */
    MusicLibrary library;

    /* The standard list of songs to be played. */
    PlaybackQueue playbackQueue;

    /* High-priority queue for user-selected "Play Next" songs. */
    PlayNextQueue playNextQueue;

    /* Stack storing previously played songs for "Back" functionality. */
    PlaybackHistory playbackHistory;

    /* The song object currently active in the player. */
    Song currentSong;

    /* Flag indicating if a song is currently loaded (playing or paused). */
    bool hasCurrentSong = false;
    
public:
    /*
     * Constructor: Initializes the library and starts the audio thread.
     */
    MusicPlayer();

    /* =============================================================
     * CORE PLAYBACK LOGIC
     * ============================================================= */

    /*
     * Selects a song by its unique ID and begins playback immediately.
     * Clears paused state and updates history if necessary.
     */
    void selectAndPlaySong(int songID);

    /*
     * Plays the next song based on priority:
     * 1. 'Play Next' Queue
     * 2. Standard Playback Queue
     */
    void playNext();

    /*
     * Returns to the previously played song from history (LIFO).
     */
    void playPrevious();

    /* =============================================================
     * QUEUE MANAGEMENT
     * ============================================================= */

    /*
     * Adds a specific song to the high-priority "Play Next" queue.
     */
    void addSongToPlayNext(int songID);

    /*
     * Randomizes the current playback order using the ShuffleManager.
     * Replaces the current playback queue with the shuffled version.
     */
    void enableShuffle();

    /*
     * Generates a "Smart Playlist" using Breadth-First Search (BFS)
     * starting from a seed song.
     */
    void BFS(int startSongID, int maxSize);

    /* =============================================================
     * DATA ACCESSORS (Getters & Setters)
     * ============================================================= */

    /* Retrieves the main music library. */
    MusicLibrary& getLibrary();

    /* Retrieves the standard playback queue. */
    PlaybackQueue& getPlaybackQueue();

    /* Retrieves the priority playback queue. */
    PlayNextQueue& getPlayNextQueue();

    /* Retrieves the playback history stack. */
    PlaybackHistory& getPlaybackHistory();

    /* Overwrites the current playback queue with a new one. */
    void setPlaybackQueue(PlaybackQueue& pb);

    /*
     * Friend Declaration:
     * Allows the global playSong function to access private members
     * (specifically the audio thread synchronization primitives).
     */
    friend void playSong(const Song& song);
};

/* =============================================================
 * GLOBAL AUDIO INTERFACE
 * These functions interact with the Audio Thread and MCI.
 * ============================================================= */

/*
 * Signals the audio thread to load and play a specific song.
 */
void playSong(const Song& song);

/*
 * Signals the audio thread to pause playback.
 */
void pauseSong();

/*
 * Signals the audio thread to resume playback if paused.
 * Handles thread synchronization to prevent state conflicts.
 */
void resumeSong();

/*
 * Helper function used in main.cpp to stop playback completely.
 */
void stopSong();

/*
 * Utility: Loads song metadata from a CSV file into the library.
 * Format: id,title,artist,album,duration,path
 */
void loadLibraryFromCSV(const std::string& filePath, MusicLibrary& library);

#endif // MUSIC_PLAYER_H